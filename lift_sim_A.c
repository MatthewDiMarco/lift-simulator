/* ****************************************************************************
 * FILE:        lift_sim_A.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     Implementation A of the Lift Simulator.
 *              Lift Simulator ...
 *
 * LAST MOD:    14/04/20 
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "lift_sim.h"
#include "fileio.h"
#include "linked_list.h"
#include "buffer.h"

// Declarations
static void move(Lift* lift, int to);

// Initialise shared memory
int numRequestsServed, totalRequests;
Buffer* buffer;
pthread_mutex_t bufLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bufNotFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t bufNotEmpty = PTHREAD_COND_INITIALIZER;

int main(int argc, char const *argv[])
{
    int bufferSize;
    double liftDelay;

    if (argc != 3)
    {
        printf("wrong args: format = %s\n", SYNTAX);
    }
    else
    {
        bufferSize = atoi(argv[1]);
        liftDelay = atof(argv[2]);

        if (bufferSize <= 0 || liftDelay <= 0)
        {
            printf("Error: %s\n", ERR);
        }
        else
        {
            startSim(bufferSize, liftDelay);
        }
    }

    return 0;
}

void startSim(int bufferSize, double liftDelay)
{
    buffer = createBuffer(bufferSize);
    LinkedList* requests = createLinkedList();
    if (readRequests(SIM_INPUT, requests, GROUND_FLOOR, NUM_FLOORS) == -1)
    {
        printf("Failed to read %s", SIM_INPUT);
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
        free(requests);
        freeBuffer(buffer);
    }  
}

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

        pthread_cond_signal(&bufNotEmpty);
        pthread_mutex_unlock(&bufLock); // CRITICAL SECTION END

        thisReq = removeStart(requests);
    }

    return 0;
}

void* lift(void* arg)
{
    Lift* lift = (Lift*)arg;
    Request* req;
    while (numRequestsServed < totalRequests)
    {
        pthread_mutex_lock(&bufLock); // CRITICAL SECTION START
        if (isEmpty(buffer))
        {
            pthread_cond_wait(&bufNotEmpty, &bufLock);
        }

        req = popBuffer(buffer);
        numRequestsServed++;

        pthread_cond_signal(&bufNotFull);
        pthread_mutex_unlock(&bufLock); // CRITICAL SECTION END

        // Serve request
        if (req != NULL)
        {
            if (lift->currFloor != req->start)
            {
                move(lift, req->start);
            }

            move(lift, req->destination);
            free(req);
        }

        // Check of there are anymore requests
        if (numRequestsServed < totalRequests)
        {
            // Other elevators will be stuck waiting.
            // Let them know buffer is indefinitely empty.
            pthread_cond_signal(&bufNotEmpty);
        }
    }
    return 0;
}

static void move(Lift* lift, int to)
{
    printf("lift %d: moving from %d to %d\n", 
            lift->id, lift->currFloor, to);
    sleep(lift->delay);
    lift->currFloor = to;
}