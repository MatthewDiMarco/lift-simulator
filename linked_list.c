/* ****************************************************************************
 * FILE:        linked_list.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 * 
 * PURPOSE:     []
 *
 * LAST MOD:    12/04/20
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

/* ****************************************************************************
 * NAME:        createLinkedList
 * 
 * PURPOSE:     To generate a blank linked list.
 *
 * EXPORT:      pointer to a linked list.
 * ***************************************************************************/
LinkedList* createLinkedList()
{
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

/* ****************************************************************************
 * NAME:        removeStart
 * 
 * PURPOSE:     To remove and free the frontmost Request in the list.
 *              Return's the Request (not node), which must be free'd later.
 * 
 * IMPORT:      list (pointer to a linked list)
 * EXPORT:      req (Request struct)
 * ***************************************************************************/
Request* removeStart(LinkedList* list)
{
    RequestNode* temp;
    Request* req = NULL;

    if(list->size > 0)
    {
        temp = list->head;
        req = list->head->req;
        list->head = list->head->next; // Update to new head

        // If last element, head and tail should point to NULL
        if(list->head == NULL)
        {
            list->tail = NULL;
        }

        free(temp); 
        list->size--;
    }

    return req;
}

/* ****************************************************************************
 * NAME:        insertLast
 * 
 * PURPOSE:     To insert a new element at the tail of the list.
 *              Return's the data, which must be free'd later.
 * 
 * IMPORT:      list (pointer to a linked list), 
 *              req (Request struct)
 * ***************************************************************************/
void insertLast(LinkedList* list, Request* req)
{
    // Prepare new node
    RequestNode* newNode = (RequestNode*)malloc(sizeof(RequestNode));
    newNode->req = req;
    newNode->next = NULL; 

    // Check if list is empty
    if(list->size < 1)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        list->tail = newNode; 
    }

    list->size++;
}

/* ****************************************************************************
 * NAME:        printLinkedList
 * 
 * PURPOSE:     To print the contents of the list to the terminal in descending
 *              order (i.e. from head to tail).
 * 
 * IMPORT:      list (pointer to a linked list)
 * ***************************************************************************/
void printLinkedList(LinkedList* list)
{
    RequestNode *node;
    if((list->head) == NULL)
    {
        printf("\nlist is empty\n\n");
    }
    else
    {
        node = list->head;
        while(node != NULL)
        {   
            Request* req = node->req;
            printf("%d %d\n", req->start, req->destination);
            node = node->next;
        }
        printf("\n");
    }
}

/* ****************************************************************************
 * NAME:        freeLinkedList
 * 
 * PURPOSE:     To free all nodes and their corresponding requests from heap
 *              memory, in addition the list itself.
 * 
 * IMPORT:      list (pointer to a linked list)
 * ***************************************************************************/
void freeLinkedList(LinkedList* list)
{
    RequestNode *node, *nextNode;
    node = list->head;
    while(node != NULL)
    {
        nextNode = node->next;
        free(node->req);
        free(node);        
        node = nextNode;
    }
    free(list);
    list = NULL;
}
