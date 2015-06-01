#include "../list.h"

#include <stdio.h>
#include <stdlib.h>

/* test struct definition */
struct integer {
	int num;
	struct list_entry linking;
};

int main(void)
{
	struct list_entry *list_head = malloc(sizeof(struct list_entry));
	struct list_entry *iterator;
	struct integer *tmp_int;
	int i;

	/* initialize the list */
	list_init(list_head);
	
	/* insert #'s 0-999 (minus 499) into the list */
	for (i = 0; i < 1000; i++) {
		if (i == 499) continue;
		tmp_int = malloc(sizeof(struct integer));
		tmp_int->num = i;
		list_insert_tail(list_head, &tmp_int->linking);
	}

	/* iterate through the list and print the numbers */
	iterator = list_head->next;
	while (iterator != list_head) {
		tmp_int = list_item(iterator, struct integer, linking);
		printf("%d\n", tmp_int->num);
		iterator = iterator->next;
	}
	printf("\n");

	/* insert 499 where it is supposed to be */
	iterator = list_head->next;
	while (iterator != list_head) {
		tmp_int = list_item(iterator, struct integer, linking);
		if (tmp_int->num == 498) {
			tmp_int = malloc(sizeof(struct integer));
			tmp_int->num = 499;
			list_insert_after(iterator, &tmp_int->linking);
			break;
		}
		iterator = iterator->next;
	}

	/* iterate through the list and print the numbers */
	iterator = list_head->prev;
	while (iterator != list_head) {
		tmp_int = list_item(iterator, struct integer, linking);
		printf("%d\n", tmp_int->num);
		iterator = iterator->prev;
	}
	printf("\n");

	/* cleanup our memory mess... */
	while (!list_empty(list_head)) {
		iterator = list_remove_tail(list_head);
		tmp_int = list_item(iterator, struct integer, linking);
		free(tmp_int);
	}
	free(list_head);

	return 0;
}
