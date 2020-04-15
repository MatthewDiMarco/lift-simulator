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
#define SYNTAX "./lift_sim_A/B <buffer-size> <lift-delay>"
#define ERR "both args should be a numeric value >= 1"
#define GROUND_FLOOR 1
#define NUM_FLOORS 20
#define NUM_LIFTS 3

// Struct for representing lifts
typedef struct Lift
{
    int id;
    int currFloor;
    double delay;
} Lift;

// Protoype Declarations
int main(int argc, char const *argv[]);
void startSim(int bufferSize, double liftDelay);
void* request(void* arg);
void* lift(void* arg);