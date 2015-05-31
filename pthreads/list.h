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
	list_entry *next, *prev;
};

/* Macros for manipulation of the list */

#endif /* _LIST_H_ */
