/* ****************************************************************************
 * FILE:        lift_sim_A.c
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     Implementation B of the Lift Simulator.
 *              Lift Simulator ...
 *
 * LAST MOD:    14/04/20 
 * ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lift_sim.h"
#include "fileio.h"
#include "linked_list.h"

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
    //...
}

void* request(void* arg)
{
    return 0;
}

void* lift(void* arg)
{
    return 0;
}