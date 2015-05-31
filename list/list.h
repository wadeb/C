#ifndef _LIST_H_
#define _LIST_H_

/* 
 * - Wade Bonkowski 05/30/2015 -
 *
 * Implemenation of a doubly-linked list in C.
 * To use, imbed a linking field into the structure
 * that you want to put into the list.
 * To maintain you should only need to keep a reference
 * to the list head.
 *
 * The list is circularly linked with a list head that is the
 * same list entry structure.
 *
 */

struct list_entry {
	struct list_entry *next, *prev;
};

/*
 ************************************
 * Macros for interacting with list *
 ************************************
 */

/*
 * Gets the containing struct of this list entry
 * entry: list_entry that we want to *dereference*
 * type: type of the containging struct the entry is embedded in
 * member: name of the struct field that is the list link.
 */

#define list_item(entry, type, member)	\
	((type *)((char *)(&entry) - (unsigned long)(&((type *)0)->member)))

/*
 * Initializes the list head
 */

#define list_init(head)				\
	do {					\
		head.next = head.prev = &head;	\
	} while (0)

#endif /* _LIST_H_ */
