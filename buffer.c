/* ****************************************************************************
 * FILE:        buffer.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 * 
 * PURPOSE:     []
 *
 * LAST MOD:    14/04/20
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "linked_list.h"

Buffer* createBuffer(int size)
{
    Buffer* newBuf = NULL;
    if (size >= 1)
    {
        newBuf = (Buffer*)malloc(sizeof(Buffer));

        newBuf->capacity = size;
        newBuf->next_out = 0;
        newBuf->next_in = 0;

        newBuf->buf = (Request**)malloc(sizeof(Request*) * size);
        for (int ii = 0; ii < size; ii++)
        {
            newBuf->buf[ii] = NULL; // null means slot is empty
        }
    }

    return newBuf;
}

Request* popBuffer(Buffer* buf)
{
    Request* req = buf->buf[buf->next_out];
    if (req != NULL) // Only change pointers if buf wasn't empty
    {
        buf->buf[buf->next_out] = NULL;
        buf->next_out = (buf->next_out + 1) % buf->capacity;
    }

    return req;
}

void addToBuffer(Buffer* buf, Request* inReq)
{
    if (buf->buf[buf->next_in] == NULL) // Check for free slot
    {
        buf->buf[buf->next_in] = inReq;
        buf->next_in = (buf->next_in + 1) % buf->capacity;
    }
}

int isEmpty(Buffer* buf)
{
    int empty = 0;
    if (buf->next_in == buf->next_out && buf->buf[buf->next_in] == NULL)
    {
        empty = 1;
    }

    return empty;
}

int isFull(Buffer* buf)
{
    int full = 0;
    if (buf->next_in == buf->next_out && buf->buf[buf->next_in] != NULL)
    {
        full = 1;
    }
    
    return full;
}

void freeBuffer(Buffer* buf)
{
    for(int ii = 0; ii < buf->capacity; ii++)
    {
        if (buf->buf[ii] != NULL)
        {
            free(buf->buf[ii]);
        }
    }

    free(buf->buf);
    free(buf);
}