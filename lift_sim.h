/* ****************************************************************************
 * FILE:        lift_sim.h
 * AUTHOR:      Matthew Di Marco
 * UNIT:        Operating Systems
 *
 * PURPOSE:     Header file for implementations A and B.
 *
 * LAST MOD:    14/04/20 
 * ***************************************************************************/

// Constants
#define SIM_INPUT "sim_input.csv"
#define SYNTAX "./lift_sim_A/B <buffer-size> <lift-delay> <optional_input_file>"
#define ERR "buffer should be >= 1, lift-delay should be >= 0"

#define GROUND_FLOOR 1
#define NUM_FLOORS 20

#define NUM_LIFTS 3

// Min and Max number of requests for a given file
#define MIN_REQ 50
#define MAX_REQ 100

#ifndef LIFT
#define LIFT

// Struct for representing lifts
typedef struct Lift
{
    int id;
    int currFloor;
    int delay;
    int numRequests;
    int numMovements;
} Lift;

#endif

// Protoype Declarations
int main(int argc, char *argv[]);
void startSim(int bufferSize, int liftDelay, char* filename);
void* request(void* arg);
void* lift(void* arg);
void move(Lift* lift, int to);