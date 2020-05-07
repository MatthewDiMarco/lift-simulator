/* ****************************************************************************
 * FILE:        fileio.h
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     Header file for fileio.c
 *
 * LAST MOD:    12/04/20 
 * ***************************************************************************/
#include "linked_list.h"
#include "lift_sim.h"

// Constants
#define FALSE 0
#define TRUE !FALSE
#define BUF 10 // Assumption: no line will be more than 20 chars long 
#define OUT_FILE "sim_out.csv"

// Prototype Declarations
int readRequests(char* filename, LinkedList* reqList, const int min, const int max);
int writeRequest(Request* req);
int writeLiftActivity(Lift* lift, Request* req);