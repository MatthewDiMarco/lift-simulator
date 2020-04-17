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

#ifndef BUFFER
#define BUFFER

// Struct representing the buffer
// next_in = next_in free slot, next_out = first filled slot.
typedef struct Buffer
{
    Request** buf;
    int capacity;
    int next_in;
    int next_out;
} Buffer;

#endif

// Prototype Declarations
Buffer* createBuffer(int size);
Request* popBuffer(Buffer* buf);
void addToBuffer(Buffer* buf, Request* inReq);
int isEmpty(Buffer* buf);
int isFull(Buffer* buf);
void freeBuffer(Buffer* buf);