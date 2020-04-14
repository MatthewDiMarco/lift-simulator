/* ****************************************************************************
 * FILE:        fileio.h
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     [todo]
 *
 * LAST MOD:    12/04/20 
 * ***************************************************************************/
#include "linked_list.h"

// Constants
#define FALSE 0
#define TRUE !FALSE
#define BUF 10

// Prototype Declarations
int readRequests(char* filename, LinkedList* reqList, const int min, const int max);
int writeRequests();
int writeLiftActivity();