/* ****************************************************************************
 * FILE:        linked_list.h
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 * 
 * PURPOSE:     Header file for linked_list.c
 *
 * LAST MOD:    14/04/20
 * ***************************************************************************/

#ifndef LL
#define LL

// Represents a request (floor to dest)
typedef struct Request
{
    int start;
    int destination;
} Request;

// A node in the list
typedef struct RequestNode 
{
    Request* req;
    struct RequestNode* next;
} RequestNode;

// The list
typedef struct 
{
    int size;
    RequestNode* head;
    RequestNode* tail;
} LinkedList;

#endif

// Prototype Declarations

/* ****************************************************************************
 * NAME:        createLinkedList
 * 
 * PURPOSE:     To generate a blank linked list.
 *
 * EXPORT:      pointer to a linked list.
 * ***************************************************************************/
LinkedList* createLinkedList();

/* ****************************************************************************
 * NAME:        removeStart
 * 
 * PURPOSE:     To remove and free the frontmost Request in the list.
 *              Return's the Request (not node), which must be free'd later.
 * 
 * IMPORT:      list (pointer to a linked list)
 * EXPORT:      req (Request struct)
 * ***************************************************************************/
Request* removeStart(LinkedList* list);

/* ****************************************************************************
 * NAME:        insertLast
 * 
 * PURPOSE:     To insert a new element at the tail of the list.
 *              Return's the data, which must be free'd later.
 * 
 * IMPORT:      list (pointer to a linked list), 
 *              req (Request struct)
 * ***************************************************************************/
void insertLast(LinkedList* list, Request* req);

/* ****************************************************************************
 * NAME:        printLinkedList
 * 
 * PURPOSE:     To print the contents of the list to the terminal in descending
 *              order (i.e. from head to tail).
 * 
 * IMPORT:      list (pointer to a linked list)
 * ***************************************************************************/
void printLinkedList(LinkedList* list);

/* ****************************************************************************
 * NAME:        freeLinkedList
 * 
 * PURPOSE:     To free all nodes and their corresponding requests from heap
 *              memory, in addition the list itself.
 * 
 * IMPORT:      list (pointer to a linked list)
 * ***************************************************************************/
void freeLinkedList(LinkedList* list);
