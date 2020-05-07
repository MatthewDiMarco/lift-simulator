/* ****************************************************************************
 * FILE:        test_buffer.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 * 
 * PURPOSE:     Test harness for buffer.c
 *
 * LAST MOD:    14/04/20
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "linked_list.h"

int main(int argc, char const *argv[])
{
    const int bufferSize = 5;
    Buffer* buf = NULL;
    Request* data;
    Request** requests = (Request**)malloc(sizeof(Request*) * bufferSize);
    for (int ii = 0; ii < bufferSize; ii++)
    {
        requests[ii] = (Request*)malloc(sizeof(Request));
        requests[ii]->start = ii + 1;
        requests[ii]->destination = ii + ii + 2;
    }

    // CREATING
    printf("*******************\n");
    printf("| Creating Buffer |\n");
    printf("*******************\n");

    // invalid
    printf("createBuffer(0): ");
    buf = createBuffer(0);

    if(buf != NULL)
    {
       printf("FAILED\n");
    }
    else
    {
       printf("PASSED\n");
    }

    // invalid
    printf("createBuffer(-1): ");
    buf = createBuffer(-1);

    if(buf != NULL)
    {
       printf("FAILED\n");
    }
    else
    {
       printf("PASSED\n");
    }

    // valid
    printf("createBuffer(5): ");
    buf = createBuffer(bufferSize);

    if(buf == NULL)
    {
        printf("FAILED\n");
    }
    else if (isEmpty(buf) == 1 && isFull(buf) == 0 &&
             buf->next_out == 0 && buf->next_in == 0 && 
             buf->capacity == bufferSize && buf->buf != NULL)
    {
        printf("PASSED\n");
    }
    else
    {
        printf("FAILED\n");
    }

    // ADDING
    printf("\n**********\n");
    printf("| Adding |\n");
    printf("**********\n");

    // add to empty buf
    printf("addToBuffer() 1: ");
    addToBuffer(buf, requests[0]);

    if (buf->buf[buf->next_out] != requests[0] ||
        isEmpty(buf) != 0 || isFull(buf) != 0)
    {
        printf("FAILED\n");
    }
    else
    {
        printf("PASSED\n");
    }

    // max out buf
    printf("addToBuffer() 2: ");
    for (int ii = 1; ii < buf->capacity; ii++)
    {
        addToBuffer(buf, requests[ii]);
    }

    if (isEmpty(buf) != 0 || isFull(buf) != 1)
    {
        printf("FAILED\n");
    }
    else
    {
        printf("PASSED\n");
    }

    // POPPING
    printf("\n***********\n");
    printf("| Popping |\n");
    printf("***********\n");

    // pop full buffer
    printf("popBuffer() 1: ");
    data = popBuffer(buf);

    if (data->start != requests[0]->start || 
        data->destination != requests[0]->destination ||
        isEmpty(buf) == 1 || isFull(buf) == 1)
    {
        printf("FAILED\n");
    }
    else
    {
        printf("PASSED\n");
    }

    // empty the buffer
    printf("popBuffer() 2: ");
    for (int ii = 1; ii < buf->capacity; ii++)
    {
        data = popBuffer(buf);
    }

    if (isEmpty(buf) != 1 || isFull(buf) != 0)
    {
        printf("FAILED\n");
    }
    else
    {
        printf("PASSED\n");
    }

    // FREEING
    printf("\n***********\n");
    printf("| Freeing |\n");
    printf("***********\n");

    addToBuffer(buf, requests[0]);
    addToBuffer(buf, requests[1]);
    addToBuffer(buf, requests[2]);
    addToBuffer(buf, requests[3]);
    addToBuffer(buf, requests[4]);

    printf("freeBuffer(): ");
    freeBuffer(buf);
    free(requests);
    
    printf("PASSED\n");   

    return 0;
}