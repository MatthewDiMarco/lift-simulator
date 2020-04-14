/* ****************************************************************************
 * FILE:        buffer.h
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 * 
 * PURPOSE:     []
 *
 * LAST MOD:    14/04/20
 * ***************************************************************************/
#include "linked_list.h"

#ifndef BUF
#define BUF

// Struct representing the buffer
// next = next free slot, start = first filled slot.
typedef struct Buffer
{
    Request** buf;
    int capacity;
    int next;
    int start;
} Buffer;

#endif

// Prototype Declarations
Buffer* createBuffer(int size);
Request* popBuffer(Buffer* buf);
void addToBuffer(Buffer* buf, Request* inReq);
int isEmpty(Buffer* buf);
int isFull(Buffer* buf);
void freeBuffer(Buffer* buf);