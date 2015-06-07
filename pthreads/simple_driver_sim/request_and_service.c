/*
 * - Wade Bonkowski 06/5/2015 -
 *
 * Super simple driver-like program
 *
 * Exercise in using pthreads:
 * Input to the program is n files with m integers in the file.
 * Each integer is a request that needs to be serviced. For a request
 * to be serviced it is to be placed into the request queue. A single
 * servicer thread will go through the queue as fast as possible and
 * service requests.
 *
 * The servicer will always service the request that has the closest
 * integer value to the last request that as serviced in this version.
 * Also for first iteration argc will be the number of threads.
 * Queue size is limited to QUEUE_SIZE.
 *
 * Perhaps for another version I will add on another constraint.
 * I eventually will make the threads joinable and cleanup mutexes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>

#include "list.h"

#define MAX_QUEUE_SIZE 5

struct requester_argument {
	int thread_num;
	char input_filename[255]; /* max filename length */
};

struct request {
	int thread_num;
	int value;
	struct list_entry linking;
};

/* pthread shared variables */
pthread_mutex_t queue_mutex;
pthread_cond_t capacity_cond;
pthread_cond_t service_cond;

/*
 * Shouldn't need a mutex for the array of cv's.
 * The entries of the array are mutually exclusive
 * by definition of the interations between requesters
 * and servicers. One requester thread per finish cv.
 */

pthread_cond_t **finish_cond;

/* shared queue, and its current size */
struct list_entry queue;
int queue_size; /* TODO: test changing this to an array */
int num_alive_requesters;
int num_requesters;

/* function to be passed to a request thread */
void *requester(void *);

/* function to be passed to the servicer thread */
void *servicer(void *);

/* helper function for servicer */
void find_and_service(void);

int main(int argc, char **argv)
{
	/* declarations */
	pthread_t request_threads[argc];
	pthread_t service_thread;
	pthread_attr_t attr;
	int rc, i;
	struct requester_argument *tmp;

	/* initialization */
	num_requesters = num_alive_requesters = argc - 1;
	finish_cond = malloc(sizeof(pthread_cond_t *) * num_alive_requesters);
	list_init(&queue);
	queue_size = 0;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_mutex_init(&queue_mutex, NULL);
	pthread_cond_init(&capacity_cond, NULL);
	pthread_cond_init(&service_cond, NULL);

	/* init each finish cond cv and launch requester threads */
	for (i = 1; i < argc; i++) {
		tmp = malloc(sizeof(struct requester_argument));
		tmp->thread_num = i - 1;
		strcpy(tmp->input_filename, argv[i]);
		finish_cond[tmp->thread_num] = malloc(sizeof(pthread_cond_t));
		pthread_cond_init(finish_cond[tmp->thread_num], NULL);
		rc = pthread_create(&request_threads[tmp->thread_num], &attr,
				    requester, (void *)tmp);
		if (rc) {
			printf("Not able to successfully create thread.\n");
			exit(1);
		}
	}

	/* launch our servicer thread */
	pthread_create(&service_thread, &attr, servicer, NULL);

	/* do some cleanup and then wait */
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

/* 
 * Read the provided file one line at a time, 
 * as a line is acquired it is sent to the queue.
 * We are done when we hit the end of the file
 */

void *requester(void *arg) {
	struct requester_argument *arg_ptr = (struct requester_argument *)arg;
	struct request *new_request;
	FILE *data = fopen(arg_ptr->input_filename, "r");
	char *line;
	size_t len;
	
	if (!data) {
		printf("Couldn't open one of the input files\n");
		exit(1);
	}

	/* I will let the lock go while I read from file */
	while (getline(&line, &len, data) != -1) {
		pthread_mutex_lock(&queue_mutex);
		while ((queue_size == MAX_QUEUE_SIZE) ||
		       (queue_size == num_alive_requesters)) {
			pthread_cond_wait(&capacity_cond, &queue_mutex);
		}

		/* we get to put something in the queue */
		printf("Requester %d issuing request for %s", 
		       arg_ptr->thread_num, line);
		new_request = malloc(sizeof(struct request));
		new_request->thread_num = arg_ptr->thread_num;
		new_request->value = atoi(line);
		list_insert_tail(&queue, &new_request->linking);
		queue_size++;

		/* tell the servicer that we put something in the queue */
		pthread_cond_signal(&service_cond);

		/* we sleep until our request is done */
		pthread_cond_wait(finish_cond[arg_ptr->thread_num],
				  &queue_mutex);

		/* we actually don't need the lock, about to read file */
		pthread_mutex_unlock(&queue_mutex);
	}

	/* cleanup and update global state */
	pthread_mutex_lock(&queue_mutex);
	num_alive_requesters--;
	pthread_mutex_unlock(&queue_mutex);

	/* servicer might be able to grab another request */
	pthread_cond_signal(&service_cond);

	pthread_cond_destroy(finish_cond[arg_ptr->thread_num]);
	/* next line is causing corruption for some reason...
	free(line); line = NULL; */
	free(arg_ptr); arg_ptr = NULL;
	fclose(data);

	pthread_exit(NULL);
}

/*
 * While there are still requesters, pull
 * requests from the queue and *service* them
 */

void *servicer(void *arg)
{	
	int i;

	assert(arg == NULL);
	pthread_mutex_lock(&queue_mutex);
	while (num_alive_requesters) {
		while (((num_alive_requesters >= MAX_QUEUE_SIZE) &&
		       (queue_size < MAX_QUEUE_SIZE)) ||
		       ((num_alive_requesters < MAX_QUEUE_SIZE) &&
		       (queue_size < num_alive_requesters))) {
			pthread_cond_wait(&service_cond, &queue_mutex);
		}

		if (!num_alive_requesters) break;

		find_and_service();
	}
	pthread_mutex_unlock(&queue_mutex);

	/* no longer need the remaining pthread stuctures */
	pthread_mutex_destroy(&queue_mutex);
	pthread_cond_destroy(&capacity_cond);
	pthread_cond_destroy(&service_cond);
	for (i = 0; i < num_requesters; i++) {
		free(finish_cond[i]); finish_cond[i] = NULL;
	}
	free(finish_cond); finish_cond = NULL;

	pthread_exit(NULL);
}

/* only call this function when the queue lock is held */
void find_and_service(void)
{
	/* declarations */
	struct list_entry *iter;
	struct request *tmp, *target;
	int difference, last_serviced = 0;
	unsigned closest_diff;

	/* find something from the queue to service */
	closest_diff = (unsigned)-1;
	for (iter = queue.next; iter != &queue; iter = iter->next) {
		tmp = list_item(iter, struct request, linking);
		difference = abs(tmp->value - last_serviced);
		if ((unsigned)difference < closest_diff) {
			target = tmp;
			closest_diff = difference;
		}
	}

	/* service the request */
	printf("Servicer servicing request for requester %d, value %d\n",
	       target->thread_num, target->value);
	last_serviced = target->value;
	list_remove(&target->linking);
	queue_size--;
	pthread_cond_signal(&capacity_cond);
	pthread_cond_signal(finish_cond[target->thread_num]);
	free(target); target = NULL;
}
