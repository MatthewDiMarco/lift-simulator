/* ****************************************************************************
 * FILE:        buffer.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 * 
 * PURPOSE:     For manipulating the state of the buffer -- a structure used
 *              to and manage access to requests.
 *
 * LAST MOD:    14/04/20
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "linked_list.h"

/* ****************************************************************************
 * NAME:        createBuffer
 * 
 * PURPOSE:     To generate an empty buffer
 * 
 * EXPORT:      pointer to the buffer struct
 * ***************************************************************************/
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

/* ****************************************************************************
 * NAME:        popBuffer
 * 
 * PURPOSE:     Remove and return the next request in the buffer.
 * 
 * IMPORT:      Pointer to the buffer
 * EXPORT:      Pointer to the request
 * ***************************************************************************/
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

/* ****************************************************************************
 * NAME:        addToBuffer
 * 
 * PURPOSE:     Insert a request into the buffer.
 * 
 * IMPORT:      Pointer to the buffer
 *              Pointer to the request
 * ***************************************************************************/
void addToBuffer(Buffer* buf, Request* inReq)
{
    if (buf->buf[buf->next_in] == NULL) // Check for free slot
    {
        buf->buf[buf->next_in] = inReq;
        buf->next_in = (buf->next_in + 1) % buf->capacity;
    }
}

/* ****************************************************************************
 * NAME:        isEmpty
 * 
 * PURPOSE:     Returns 1 if the buffer is empty.
 * 
 * IMPORT:      Pointer to the buffer
 * EXPORT:      Integer (1 = empty)
 * ***************************************************************************/
int isEmpty(Buffer* buf)
{
    int empty = 0;
    if (buf->next_in == buf->next_out && buf->buf[buf->next_in] == NULL)
    {
        empty = 1;
    }

    return empty;
}

/* ****************************************************************************
 * NAME:        isFull
 * 
 * PURPOSE:     Returns 1 if the buffer is full.
 * 
 * IMPORT:      Pointer to the buffer
 * EXPORT:      Integer (1 = full)
 * ***************************************************************************/
int isFull(Buffer* buf)
{
    int full = 0;
    if (buf->next_in == buf->next_out && buf->buf[buf->next_in] != NULL)
    {
        full = 1;
    }
    
    return full;
}

/* ****************************************************************************
 * NAME:        freeBuffer
 * 
 * PURPOSE:     Free entire buffer state including all requests in it.
 * 
 * IMPORT:      Pointer to the buffer
 * ***************************************************************************/
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