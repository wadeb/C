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
 **************************************************
 * Macros and functions for interacting with list *
 **************************************************
 */

/*
 * Gets the containing struct of this list entry
 * ptr: list_entry* that we want to *dereference*
 * type: type of the containing struct the entry is embedded in
 * member: name of the struct field that is the list link.
 */

#define list_item(ptr, type, member)	\
	((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

/*
 * Initializes the list head
 * head: list_entry* that is the head of the list
 */

inline void list_init(struct list_entry *head)
{
	head->next = head->prev = head;
}

/*
 * Returns 1 if the list is empty, 0 otherwise
 * head: list_entry* that is the head of the list
 */

inline int list_empty(struct list_entry *head)
{
	return ((head->next == head) && (head->prev == head));
}

/*
 * Insert element at the tail of the list
 * head: list_entry* that is the head of the list
 * new_entry: list_entry* that we are inserting
 */

inline void list_insert_tail(struct list_entry *head,
			     struct list_entry *new_entry)
{
	head->prev->next = new_entry;
	new_entry->prev = head->prev;
	new_entry->next = head;
	head->prev = new_entry;
}

/*
 * Insert element at the head of the list
 * head: list_entry* that is the head of the list
 * new_entry: list_entry* that we are inserting
 */

inline void list_insert_head(struct list_entry *head,
			     struct list_entry *new_entry)
{
	head->next->prev = new_entry;
	new_entry->next = head->next;
	new_entry->prev = head;
	head->next = new_entry;
}

/*
 * Removes and returns pointer to the element at the head of the list
 * head: list_entry* that is the head of the list
 */

inline struct list_entry* list_remove_head(struct list_entry *head)
{
	struct list_entry *target = head->next;
	
	head->next = target->next;
	target->next->prev = head;

	return target;
}

/*
 * Removes and returns pointer to the element at the tail of the list
 * head: list_entry* that is the head of the list
 */

inline struct list_entry* list_remove_tail(struct list_entry *head)
{
	struct list_entry *target = head->prev;

	head->prev = target->prev;
	target->prev->next = head;

	return target;
}

#endif /* _LIST_H_ */
