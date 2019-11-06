#include <iostream>
#include <string>
#include "DLList.h"

// Constructor
// DO NOT CHANGE
DLList::DLList() 
{
    head = new ListNode();
    head->next = head;
    head->prev = head;
}

// Prints the list
// DO NOT CHANGE
void DLList::print_list() 
{
    std::cout << "Printing List..." << std::endl;
    if ( head->next == head ) {
	std::cout << "Empty List" << std::endl;
    }
    else {
	ListNode * e = head->next;  
	while ( e != head) {
	    if ( e->next != head )
		std::cout << e->value << ", ";
	    else 
		std::cout << e->value;
	    e = e->next;
	}
	std::cout << std::endl;
    }
}

// Prints the list given a list name
// DO NOT CHANGE
void DLList::print_list( std::string list_name ) 
{
    std::cout << "Printing " << list_name << "..." << std::endl;
    if ( head->next == head ) {
	std::cout << "Empty List" << std::endl;
    }
    else {
	ListNode * e = head->next;  
	while ( e != head) {
	    if ( e->next != head )
		std::cout << e->value << ", ";
	    else 
		std::cout << e->value;
	    e = e->next;
	}
	std::cout << std::endl;
    }
}

// Problem (1/5)
/******************************************************************************
 * TODO: Write the destructor. The destructor should delete all of the 
 * list nodes from the list. 
 *****************************************************************************/
DLList::~DLList()
{
	// Write Your Code Here
	
	if (head->next == head){
		delete(head);
	}else{
		ListNode * e = head->next;
		while(e != head){
			if (e->prev != e){
				delete(e->prev);
			}
			e = e->next;
		}
		delete(head);
	}
	
	
}

// Problem (2/5)
/******************************************************************************
 * TODO: Insert a new ListNode to the end of the double linked list. 
 * Remember the list head is a sentinel node, do not change head. 
 * Set the value of the new node to the parameter 'value'.
 * 
 * Parameters: value -- set the value of the node equal to this
 *
 * Return: void 
 *
 * Return Type: void
 *****************************************************************************/
void DLList::insert_last( int value ) 
{
	ListNode * r = head->next;
	ListNode * e = new ListNode();
	e->next = NULL;
	e->prev = NULL;
	if (head == NULL){
		e->next = head->next;
		e->prev = head;
		e->next->prev = e;
		e->prev->next = e;
		return ; 
	}
	e->value = value;
	while(r->next != head){
		r = r->next;
	}
	r->next = e;
	e->prev = r;
	e->next = head;
	e->next->prev = e;
	e->prev->next = e;
	
}

// Problem (3/5)
/******************************************************************************
 * TODO: Remove the node from the double linked list whose value equals the 
 * parameter 'value', return true after deleting the node. If the node does not
 * exist in the list or the list is empty, return false;
 * 
 * Parameters: value -- remove the node whose value is equal to this
 *
 * Return: true if the node was removed successfully
 *         false if the value does not exist in the list or the list is empty
 *
 * Return Type: void
 *****************************************************************************/
bool DLList::remove( int value ) 
{
	// Write Your Code Here
	ListNode * r = head->next;
	while(r != head){
		if (r->value == value){
			ListNode * a = r->prev;
			ListNode * b = r->next;
			a->next = b;
			b->prev = a;
			delete(r);
			return true;
		}
		r = r->next;
	}
	return false;

}

// Problem (4/5)
/*****************************************************************************
* TODO: Return the ith node inside of the double linked list. If the list is
* empty or has less than 'ith' entries, return NULL.
* 
* Return: the 'ith' node in the list
*         NULL if the node does not exist
*
* Return Type:  
*****************************************************************************/
ListNode * DLList::get_ith( int ith ) 
{
	int z = 0;
	ListNode * r = head->next;
	while(r != head){
		if (z == ith){
			return r;
		}
		z++;
		r = r->next;
	}
	return NULL;
}

// Problem (5/5)
/*****************************************************************************
* TODO: Overload the operator '==' for comparison. The comparison operator
* should compare the two lists to check if they have the all of the same
* elements. Return true if the two lists are equal, return false otherwise.
*****************************************************************************/
bool DLList::operator == (const DLList & listB) 
{
   /*
    ListNode * r = this->head;
    ListNode * e = listB.head;
    while ((r != head) && (e != head)){
    	if (r->value != e->value){
		return false;
	}
	if (((r->next == NULL) && (e->next != NULL)) || ((r->next != NULL) && (e->next == NULL))){
		return false;
	}
    }
    return true;
    */
}
