/* ****************************************************************************
 * FILE:        fileio.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     [todo]
 *
 * LAST MOD:    12/04/20 
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "linked_list.h"

int readRequests(char *filename, LinkedList* reqList, const int min, const int max)
{
    char startStr[BUF], destStr[BUF];
    int numRequests = 0, linenum = 0, status = 0;

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

int writeRequest()
{
    return 0;
}

int writeLiftActivity()
{
    return 0;
}