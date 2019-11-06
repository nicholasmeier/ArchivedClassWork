#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"

//Initialize the linked list
void llist_init(LinkedList * list)
{
        list->head = NULL;
}

// Prints the linked list
void llist_print(LinkedList * list) {
        
        ListNode * e;

        if (list->head == NULL) {
                printf("{EMPTY}\n");
                return;
        }

        printf("{");

        e = list->head;
        while (e != NULL) {
                printf("%d", e->value);
                e = e->next;
                if (e!=NULL) {
                        printf(", ");
                }
        }
        printf("}\n");
}

// Problem ( 1/4 ) 
/******************************************************************************
 * TODO: Return the number of elements (ListNodes) in the linked list.
 *
 * Parameters: list -- find the number of elements contained in this single
 * linked list
 *
 * Return: the number of elements 
 *
 * Return Type: integer
 *****************************************************************************/
int llist_number_elements(LinkedList * list) {
	int count = 0;
	ListNode * e; 
	if (list->head == NULL){
		return 0;
	}

	e = list->head;
	while (e != NULL){
		count++;
		e = e->next;
	}
	return count;
}

// Problem ( 2/4 ) 
/******************************************************************************
 * TODO: Create a new ListNode and add it to the end of the single linked
 * 	 list. There is no check if the value exists in the list. 
 *
 * Parameters: list -- The single linked list to which you will insert the new
 * 		       ListNode to the end of.
 *             value -- The new ListNode will contain this value.
 *
 * Return: void
 * 
 * Return Type: void
 *****************************************************************************/
void llist_insert_last(LinkedList * list, int value) {
	ListNode * r = (ListNode *) malloc(sizeof(ListNode));
	r->value = value;
	r->next = NULL;
	
	ListNode * e = list->head;

	if (list->head == NULL){
		list->head = r;
	}else{
		if (e->next == NULL){
			e->next = r;
		}else{
			while(1){
				if (e->next == NULL){
					e->next = r;
					break;
				}
				e = e->next;
			}
		}
	}
}

// Problem ( 3/4 )
/******************************************************************************
 * TODO: Remove the ith entry from the single linked list.   	 
 * 
 * Parameters: list -- The single linked list to which you will be removing
 * 		       the listNode from.
 * 	       ith -- The index where you will remove the listNode from the
 * 	              single linked list. 
 *
 * Return: Return 1 if the ListNode is removed.
 * 	   Return 0 if the ith entry does not exist in the list or the list is
 * 	   empty.
 *
 * Return Type: integer
 *****************************************************************************/
int llist_remove_ith(LinkedList * list, int ith) {
	ListNode * e = list->head;
	if (e == NULL){
		return 0;
	}
	if (ith == 0){
		list->head = e->next;
		free(e);
		return 1;
	}
	int x;
	for (x = 0; e != NULL && x < ith-1; x++){
		e = e->next;
	}
	if (e == NULL || e->next == NULL){
		return 0;
	}
	ListNode * n = e->next->next;
	free(e->next);
	e->next = n;
	return 1;
}

// Problem ( 4/4 )
/******************************************************************************
 * TODO: Iterate through list1 and list2. Check if any values intersect between
 *       the two lists. If the values match, add the intersecting elements
 *       to list3. Use the function llist_add(LinkedList * list, int value)
 *       to add a value to the list.
 * 
 * Parameters: list1 -- singled linked list, find values that intersect with
 *                      list2
 *             list2 -- single linked list, find values that intersect with
 *                      list1
 *             list3 -- store the intersection of list1 and list2 in this list
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void llist_intersection(LinkedList * list1, LinkedList * list2, LinkedList * list3){
	ListNode * e = list1->head;
	ListNode * f = list2->head;

	while (e != NULL){
		while (f != NULL){
			if (e->value == f->value){
				llist_add(list3, e->value);
				f = list2->head;
				break;
			}
			f = f->next;
		}
		f = list2->head;
		e = e->next;
	}

}
