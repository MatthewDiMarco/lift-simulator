/* ****************************************************************************
 * FILE:        test_linked_list.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 * 
 * PURPOSE:     Test harness for linked_list.c
 *
 * LAST MOD:    14/04/20
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int main(int argc, char const *argv[])
{
    const int numRequests = 5;
    LinkedList* list = NULL;
    Request* data;
    Request** requests = (Request**)malloc(sizeof(Request*) * numRequests);
    for (int ii = 0; ii < numRequests; ii++)
    {
        requests[ii] = (Request*)malloc(sizeof(Request));
        requests[ii]->start = ii + 1;
        requests[ii]->destination = ii + ii + 2;
    }

    // CREATING
    printf("*****************\n");
    printf("| Creating List |\n");
    printf("*****************\n");

    printf("createLinkedList(): ");
    list = createLinkedList();

    if(list == NULL || list->head != NULL || list->tail != NULL)
    {
       printf("FAILED\n");
    }
    else
    {
       printf("PASSED\n");
    }

    // INSERT LAST
    printf("\n***************\n");
    printf("| Insert Last |\n");
    printf("***************\n");

    // (1) insert into empty list
    printf("insertLast() 1: ");
    insertLast(list, requests[0]);

    if(list->head == NULL || list->tail == NULL)
    {
       printf("FAILED\n");
    }
    else if((list->head->req->start == requests[0]->start) && 
            (list->head->req->destination == requests[0]->destination))
    {
       printf("PASSED\n");
    }
    else
    {
       printf("FAILED\n");
    }

    // (2) insert into NOT empty list
    printf("insertLast() 2: ");
    insertLast(list, requests[1]);

    if(list->head == NULL || list->tail == NULL)
    {
       printf("FAILED\n");
    }
    else if((list->head->req->start == requests[0]->start) && 
            (list->head->req->destination == requests[0]->destination) &&
            (list->head->next->req->start == requests[1]->start) &&
            (list->head->next->req->destination == requests[1]->destination))
    {
       printf("PASSED\n");
    }
    else
    {
       printf("FAILED\n");
    }


    // REMOVE START
    printf("\n****************\n");
    printf("| Remove Start |\n");
    printf("****************\n");

    // (1) removing from list with 2 elements
    printf("removeStart() 1: ");
    data = removeStart(list);
    
    if(list->head == NULL || list->tail == NULL)
    {
       printf("FAILED\n");
    }
    else if((list->head->req->start == requests[1]->start) && 
            (list->head->req->destination == requests[1]->destination) &&
            (data->start == requests[0]->start) &&
            (data->destination == requests[0]->destination))
    {
       printf("PASSED\n");
    }
    else
    {
       printf("FAILED\n");
    }

    /* (2) removing from list with a single element */
    printf("removeStart() 2: ");
    data = removeStart(list);

    if(list->head != NULL || list->tail != NULL)
    {
       printf("FAILED\n");
    }
    else if((data->start == requests[1]->start) &&
            (data->destination == requests[1]->destination))
    {
       printf("PASSED\n");
    }
    else
    {
       printf("FAILED\n");
    } 

    // (3) trying to remove from empty list
    printf("removeStart() 3: ");
    data = removeStart(list);

    if(data == NULL)
    {
       printf("PASSED\n");
    }
    else
    {
       printf("FAILED\n");
    }

    // FREEING
    printf("\n***********\n");
    printf("| Freeing |\n");
    printf("***********\n");

    insertLast(list, requests[0]);
    insertLast(list, requests[1]);
    insertLast(list, requests[2]);
    insertLast(list, requests[3]);
    insertLast(list, requests[4]);

    printf("freeLinkedList(): ");
    freeLinkedList(list);
    free(requests);
    
    printf("PASSED\n");   
 
    return 0;
}