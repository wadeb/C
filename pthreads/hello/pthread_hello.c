#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/* print hello world from 10 threads (really 11 cause of main thread) */

void *hello(void *);

int main(void)
{
	pthread_t threads[10];
	int i, rc, *tmp;

	printf("About to make some threads...\n");
	for (i = 0; i < 10; i++) {
		tmp = malloc(sizeof(int));
		*tmp = i;
		rc = pthread_create(&threads[i], NULL, hello, (void *)tmp);
		if (rc) {
			printf("Something went wrong, error code %d\n", rc);
			exit(1);
		}
	}

	/* blocks the main thread until all children are done */
	pthread_exit(NULL);
}

/* thread_id: pointer to the thread id */
void *hello(void *thread_id)
{
	/* 
 	 * apparantly printf() may be atomic depending on 
 	 * implementation
 	 */
	printf("Hello World from thread %d\n", *(int *)thread_id);

	/* cleanup, for free to behave properly it needs an int* */
	free((int *)thread_id);
	pthread_exit(NULL);
}
