/* ****************************************************************************
 * FILE:        fileio.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     Handles all file input/output related tasks, including:
 *                  - loading the input file
 *                  - writing requests, and 
 *                  - writing lift operations
 *
 * LAST MOD:    12/04/20 
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "lift_sim.h"
#include "linked_list.h"

/* ****************************************************************************
 * NAME:        readRequests
 * 
 * PURPOSE:     Constructs a number of Lift Requests from an input file and
 *              appends them an imported list.
 * 
 * IMPORT:      filename - name of fiel
 *              reqList - list to house requests
 *              min - min floor
 *              max - max floor
 * EXPORT:      Error code (-1 = problem occured)
 * ***************************************************************************/
int readRequests(char *filename, LinkedList* reqList, const int min, const int max)
{
    char startStr[BUF], destStr[BUF];
    int numRequests = 0, linenum = 0, status = 0;

    // Remove old output file
    remove("sim_out.csv");

    // Open and check file
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("there was an error opening the file");
        status = -1;
    }
    else
    {
        // Begin looping through requests.
        // If request is valid, add to request list
        while (fscanf(file, "%s %s%*[^\n]\n", startStr, destStr) != EOF)
        { 
            linenum++;

            int start = atoi(startStr);
            int dest = atoi(destStr);

            if ((start == 0) || (dest == 0))
            {
                printf("error in %s at line %d: ", filename, linenum);
                printf("floor must be a positive numeric value\n");
            }
            else if ((start < min) || (start > max) ||
                     (dest < min) || (dest > max))
            {
                printf("error in %s at line %d: ", filename, linenum);
                printf("only %d to %d floors\n", min, max);
            }
            else // the request is valid
            {
                numRequests++;
                Request* req = (Request*)malloc(sizeof(Request));
                req->num = numRequests;
                req->start = start;
                req->destination = dest;
                insertLast(reqList, req);
            }
        }

        // Final error check
        if (ferror(file))
        {
            perror("there was an error closing the file");
            status = -1;
        }
        fclose(file);
    }

    return status;
}

/* ****************************************************************************
 * NAME:        writeRequest
 * 
 * PURPOSE:     Append a request struct to the output file.
 * 
 * IMPORT:      Pointer to a request
 * EXPORT:      Error code (-1 = problem occured)
 * ***************************************************************************/
int writeRequest(Request* req)
{
    int status = 0; 

    // Open and check file
    FILE* file = fopen(OUT_FILE, "a");
    if (file == NULL)
    {
        perror("there was an error opening the file");
        status = -1;
    }
    else
    {
        // Do writing
        fprintf(file, "------------------------------------------\n");
        fprintf(file, "New Lift Request From Floor %d to Floor %d\n",
                req->start, req->destination);

        fprintf(file, "Request No: %d\n", req->num);
        fprintf(file, "------------------------------------------\n");
        fprintf(file, "\n"); // Done

        // Final error check
        if (ferror(file))
        {
            perror("there was an error closing the file");
            status = -1;
        }
        fclose(file);
    }

    return status;
}

/* ****************************************************************************
 * NAME:        writeLiftActivity
 * 
 * PURPOSE:     Append some lift operation details to the output file.
 * 
 * IMPORT:      Pointer to the lift struct
 *              Pointer to the request
 * EXPORT:      Error code (-1 = problem occured)
 * ***************************************************************************/
int writeLiftActivity(Lift* lift, Request* req)
{
    int status = 0; 

    // Open and check file
    FILE* file = fopen(OUT_FILE, "a");
    if (file == NULL)
    {
        perror("there was an error opening the file");
        status = -1;
    }
    else
    {
        // Do writing
        fprintf(file, "Lift-%d Operation\n", lift->id);
        fprintf(file, "Previous position: Floor %d\n", lift->currFloor);
        fprintf(file, "Request: Floor %d to %d\n", req->start, req->destination);
        fprintf(file, "Detail operations:\n");

        if (lift->currFloor != req->start)
        {
            fprintf(file, "    Go from Floor %d to Floor %d\n",
                    lift->currFloor, req->start);
        }
        
        fprintf(file, "    Go from Floor %d to Floor %d\n",
                req->start, req->destination);

        int numMov = (abs(lift->currFloor - req->start)) + 
                     (abs(req->start - req->destination));
        fprintf(file, "    #movements for this request: %d\n", numMov);
        fprintf(file, "    #request: %d\n", lift->numRequests);
        fprintf(file, "    Total #movement: %d\n", lift->numMovements + numMov);
        fprintf(file, "Current position: Floor %d\n", req->destination);
        fprintf(file, "\n"); // Done

        // Final error check
        if (ferror(file))
        {
            perror("there was an error closing the file");
            status = -1;
        }
        fclose(file);
    }

    return status;
}