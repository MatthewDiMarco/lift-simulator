/* ****************************************************************************
 * FILE:        lift_sim_B.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     Implementation B of the Lift Simulator.
 *              Lift Simulator ...
 *
 * LAST MOD:    15/04/20 
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "lift_sim.h"
#include "fileio.h"
#include "linked_list.h"
#include "buffer.h"

typedef struct Shared
{   
    int numRequestsServed;
    int totalRequests;
    Buffer* buffer;
    sem_t mutex;
    sem_t full;
    sem_t empty;
} Shared;

// Initialise shared memory
Shared* shm;

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
    LinkedList* requests = createLinkedList();
    if (readRequests(SIM_INPUT, requests, GROUND_FLOOR, NUM_FLOORS) == -1)
    {
        printf("Failed to read %s", SIM_INPUT);
        freeLinkedList(requests);
    }
    else
    {
        //TEMP
        LinkedList* requestsCopy = createLinkedList();
        RequestNode* thisNode = requests->head;
        while(thisNode != NULL)
        {
            insertLast(requestsCopy, thisNode->req);
            thisNode = thisNode->next;
        }
        //...

        // Allocate shared memory and attach to address space
        int shmId_1 = shmget(1000, 1024, 0666 | IPC_CREAT);
        int shmId_2 = shmget(2000, 1024, 0666 | IPC_CREAT);
        int shmId_3 = shmget(3000, 1024, 0666 | IPC_CREAT);

        shm = (Shared*)shmat(shmId_1, NULL, 0); 
        shm->buffer = (Buffer*)shmat(shmId_2, NULL, 0);
        shm->buffer->buf = (Request**)shmat(shmId_3, NULL, 0);

        // Initialise shared objects and semaphores
        shm->totalRequests = requests->size;
        shm->numRequestsServed = 0;
        sem_init(&shm->mutex, 1, 1);
        sem_init(&shm->empty, 1, shm->buffer->capacity);
        sem_init(&shm->full, 1, 0);

        // Initialise Buffer
        shm->buffer->capacity = bufferSize;
        shm->buffer->next = 0;
        shm->buffer->start = 0;
        for (int ii = 0; ii < bufferSize; ii++)
        {
            shm->buffer->buf[ii] = NULL; // null means slot is empty
        }

        // Create Lists
        Lift** lifts = (Lift**)malloc(sizeof(Lift*) * NUM_LIFTS);
        for (int ii = 0; ii < NUM_LIFTS; ii++)
        {
            lifts[ii] = (Lift*)malloc(sizeof(Lift));
            lifts[ii]->id = ii + 1;
            lifts[ii]->currFloor = 1;
            lifts[ii]->delay = liftDelay;
        }

        // Create 3 processes: 1 for each lift
        pid_t cpid1, cpid2, cpid3;
        cpid1 = fork();
        if (cpid1 == 0)
        {
            cpid2 = fork();
            if (cpid2 == 0)
            {
                lift(lifts[0]); // Lift-1

                // Free
                for (int ii = 0; ii < NUM_LIFTS; ii++)
                {
                    free(lifts[ii]);
                }  
                free(lifts);
                freeLinkedList(requests);
                freeLinkedList(requestsCopy);
            }
            else if (cpid2 > 0)
            {
                cpid3 = fork();
                if (cpid3 == 0)
                {
                    lift(lifts[1]); // Lift-2

                    // Free
                    for (int ii = 0; ii < NUM_LIFTS; ii++)
                    {
                        free(lifts[ii]);
                    }  
                    free(lifts);
                    freeLinkedList(requests);
                    freeLinkedList(requestsCopy);
                }
                else if (cpid3 > 0)
                {
                    lift(lifts[2]); // Lift-3

                    // Free
                    for (int ii = 0; ii < NUM_LIFTS; ii++)
                    {
                        free(lifts[ii]);
                    }  
                    free(lifts);
                    freeLinkedList(requests);
                    freeLinkedList(requestsCopy);
                }
            }
        }
        else if (cpid1 > 0) // The parent is lift-R (this process)
        {
            request(requests);

            // Wait for all children to finish up before closing
            int status = 0;
            while ((wait(&status)) > 0);

            // Free
            for (int ii = 0; ii < NUM_LIFTS; ii++)
            {
                free(lifts[ii]);
            }  
            free(lifts);
            freeLinkedList(requests);
            freeLinkedList(requestsCopy);
        }
    }  
}

void* request(void* arg)
{
    LinkedList* requests = (LinkedList*)arg;
    Request* thisReq = removeStart(requests);

    while (thisReq != NULL)
    {
        sem_wait(&shm->empty); // CRITICAL SECTION START
        sem_wait(&shm->mutex);

        printf("NEW REQUEST: %d to %d\n", thisReq->start, thisReq->destination);
        addToBuffer(shm->buffer, thisReq);

        sem_post(&shm->mutex);
        sem_post(&shm->full); // CRITICAL SECTION END

        thisReq = removeStart(requests);
    }

    return 0;
}

void* lift(void* arg)
{
    Lift* lift = (Lift*)arg;
    Request* req;

    while (shm->numRequestsServed < shm->totalRequests)
    {
        sem_wait(&shm->full); // CRITICAL SECTION START
        sem_wait(&shm->mutex);

        req = popBuffer(shm->buffer);
        shm->numRequestsServed++;

        sem_post(&shm->mutex);
        sem_post(&shm->empty); // CRITICAL SECTION END

        // Serve request
        if (req != NULL)
        {
            if (lift->currFloor != req->start)
            {
                move(lift, req->start);
            }

            move(lift, req->destination);
        }
    }

    return 0;
}

void move(Lift* lift, int to)
{
    printf("lift %d: moving from %d to %d\n", 
            lift->id, lift->currFloor, to);
    sleep(lift->delay);
    lift->currFloor = to;
}
