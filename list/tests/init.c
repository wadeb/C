#include "../list.h"
#include <stdio.h>

/* test struct definition */
struct three_numbers {
	int one, two, three;
	struct list_entry list_head;
};

int main(void)
{
	/* mainly a test for compilation */
	struct three_numbers test, *result;
	test.one = test.two = test.three = 0;
	list_init(test.list_head);

	/* just cause we can, test list_item */
	result = list_item(test.list_head, struct three_numbers, list_head);

	if (result == &test) {
		printf("Hey, it worked!\n");
		return 0;
	} else {
		printf("Hey, that was an error!\n");
		return 1;
	}
}
