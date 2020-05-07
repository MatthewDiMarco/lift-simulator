/* ****************************************************************************
 * FILE:        lift_sim_A.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     Implementation A of the Lift Simulator.
 *              
 *              This program simulates 3 lifts servicing a 20 story building
 *              in synchronous harmony. It can handle 50-100 requests per input
 *              file, where each line in the file is a request formatted as:
 *              "[current_floor] [destination_floor]".
 *              After the simulation ends, consult the file 'sim_out.csv' for 
 *              details concerning all lift operations, and request pushes to 
 *              the buffer in the order they happened.
 * 
 *              Note: the two implementations (A and B) produce the same results
 *              but work differently 'under the hood'.
 * 
 *              Implementation A was made using threads and the pthread library
 *              to address and solve synchronisation issues.
 *
 * LAST MOD:    14/04/20 
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "lift_sim.h"
#include "fileio.h"
#include "linked_list.h"
#include "buffer.h"

// Initialise shared memory
int numRequestsServed, totalRequests;
Buffer* buffer;
pthread_mutex_t bufLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bufNotFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t bufNotEmpty = PTHREAD_COND_INITIALIZER;

/* ****************************************************************************
 * NAME:        main
 * 
 * IMPORT:      Takes three command line parameters:
 *              1. Buffer Size, i.e. max number of requests that can sit in the 
 *                 buffer at a given time (Integer >= 1)
 *              2. Lift Delay in seconds (Integer >= 0)
 *              3. (optional) specific input file
 * ***************************************************************************/
int main(int argc, char *argv[])
{
    int bufferSize;
    int liftDelay;

    if (argc < 3 || argc > 4)
    {
        printf("wrong args: format = %s\n", SYNTAX);
    }
    else
    {
        bufferSize = atoi(argv[1]);
        liftDelay = atoi(argv[2]);

        if (bufferSize <= 0 || liftDelay < 0)
        {
            printf("Error: %s\n", ERR);
        }
        else
        {
            if (argv[3] != NULL)
            {
                startSim(bufferSize, liftDelay, argv[3]);
            }
            else
            {
                startSim(bufferSize, liftDelay, SIM_INPUT);
            }
        }
    }

    return 0;
}

/* ****************************************************************************
 * NAME:        startSim
 * 
 * PURPOSE:     Start and close the simulation by reading the input file for 
 *              requests, initialising a buffer, spawning / joining threads, and
 *              eventually freeing all malloc'd memory.
 * 
 * IMPORT:      bufferSize - size of the buffer to initialise
 *              liftDelay - seconds to delay lift operations
 *              filename - input file
 * ***************************************************************************/
void startSim(int bufferSize, int liftDelay, char* filename)
{
    buffer = createBuffer(bufferSize);
    LinkedList* requests = createLinkedList();

    int stat = readRequests(filename, requests, GROUND_FLOOR, NUM_FLOORS);
    if (stat == -1)
    {
        printf("failed to read %s\n", filename);
        free(requests);
        freeBuffer(buffer);
    }
    else if(requests->size < 50 || requests->size > 100)
    {
        printf("the sim can only accomodate %d-%d requests\n", MIN_REQ, MAX_REQ);
        free(requests);
        freeBuffer(buffer);
    }
    else
    {
        // Create requests thread
        totalRequests = requests->size;
        numRequestsServed = 0;
        pthread_t lift_r;
        pthread_create(&lift_r, NULL, request, requests);

        // Create lift threads
        pthread_t lift_t[3];
        Lift** lifts = (Lift**)malloc(sizeof(Lift*) * NUM_LIFTS);
        for (int ii = 0; ii < NUM_LIFTS; ii++)
        {
            lifts[ii] = (Lift*)malloc(sizeof(Lift));
            lifts[ii]->id = ii + 1;
            lifts[ii]->currFloor = 1;
            lifts[ii]->delay = liftDelay;
            lifts[ii]->numRequests = 0;
            lifts[ii]->numMovements = 0;
            pthread_create(&lift_t[ii], NULL, lift, lifts[ii]);
        }

        // Join threads and discard lifts
        pthread_join(lift_r, NULL);
        for (int ii = 0; ii < NUM_LIFTS; ii++)
        {
            pthread_join(lift_t[ii], NULL);
            free(lifts[ii]);
        }

        // Free everything else
        free(lifts);
        free(requests); //freeLinkedList()??
        freeBuffer(buffer);
    }  
}

/* ****************************************************************************
 * NAME:        request
 * 
 * PURPOSE:     Function for the Lift-R thread.
 *              This thread is responsible for loading requests from the list
 *              into the buffer. Mutual exclusion is achieved through pthread
 *              locks to ensure the buffer is never accessed while other threads
 *              are in their critical sections.
 * 
 * IMPORT       Linked List of requests.
 * ***************************************************************************/
void* request(void* arg)
{
    LinkedList* requests = (LinkedList*)arg;
    Request* thisReq = removeStart(requests);

    while (thisReq != NULL)
    {
        pthread_mutex_lock(&bufLock); // CRITICAL SECTION START

        if (isFull(buffer))
        {
            pthread_cond_wait(&bufNotFull, &bufLock);
        }

        printf("NEW REQUEST: %d to %d\n", thisReq->start, thisReq->destination);
        addToBuffer(buffer, thisReq);
        writeRequest(thisReq);

        pthread_cond_signal(&bufNotEmpty);
        pthread_mutex_unlock(&bufLock); // CRITICAL SECTION END

        thisReq = removeStart(requests);
    }

    return 0;
}

/* ****************************************************************************
 * NAME:        lift
 * 
 * PURPOSE:     Function for the Lift-x thread.
 *              This thread is responsible for extracting requests from the 
 *              buffer and processing the lift's operation on the request. 
 *              Mutual exclusion is ensured through pthread locks.
 * 
 * IMPORT       Lift struct - contains lift state
 * ***************************************************************************/
void* lift(void* arg)
{
    Lift* lift = (Lift*)arg;
    Request* req;
    
    int finished = 0;
    while (finished != 1)
    {
        pthread_mutex_lock(&bufLock); // CRITICAL SECTION START

        // End loop if there are no more requests
        if (numRequestsServed >= totalRequests)
        {
            finished = 1;
            pthread_mutex_unlock(&bufLock);
        }
        else
        {
            // Wait and release the lock if buffer is empty
            if (isEmpty(buffer))
            {
                pthread_cond_wait(&bufNotEmpty, &bufLock);
            }

            req = popBuffer(buffer);

            // Serve request (if there is one)
            if (req != NULL)
            {
                // Write acitvity to log
                lift->numRequests++;
                writeLiftActivity(lift, req);

                // Add to num served before releasing mutex
                numRequestsServed++;
                pthread_cond_signal(&bufNotFull);
                pthread_mutex_unlock(&bufLock);

                // Serve
                if (lift->currFloor != req->start)
                {
                    move(lift, req->start);
                }

                move(lift, req->destination);

                // Request no longer needed
                free(req);
            }
            else
            {
                pthread_cond_signal(&bufNotFull);
                pthread_mutex_unlock(&bufLock); // CRITICAL SECTION END
            }
        }
    }

    return 0;
}

/* ****************************************************************************
 * NAME:        move
 * 
 * PURPOSE:     Processes a lift operation.
 * 
 * IMPORT       lift - the lift struct
 *              to - an Integer describing the destination floor
 * ***************************************************************************/
void move(Lift* lift, int to)
{
    printf("lift %d: moving from %d to %d\n", 
            lift->id, lift->currFloor, to);

    sleep(lift->delay);

    // Increment movements
    lift->numMovements += abs(lift->currFloor - to);
    lift->currFloor = to;
}