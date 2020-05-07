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
    int num;
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
LinkedList* createLinkedList();
Request* removeStart(LinkedList* list);
void insertLast(LinkedList* list, Request* req);
void printLinkedList(LinkedList* list);
void freeLinkedList(LinkedList* list);
