#ifndef ELEVATORBANK_H
#define ELEVATORBANK_H

#include "Floor.h"
#include "Elevator.h"

#include <cmath>;

class ElevatorBank
{
/*******************************************************************************
NOTES:          Destructor needs to be defined and declared if main program
                does not end after execution of runSimulation().

                Efficiency algorithm is for efficiency of motion not time.  The
                nearest elevator responds to agent's requests even if the
                nearest elevator will take longer to respond (for instance, due
                to scheduled stops at intervening floors).
*******************************************************************************/
    private:
        Elevator **elevators;
        Floor **floors;

        int weightLimit;
        int minWeight;
        int maxWeight;
        int numberOfElevators;
        int numberOfFloors;
        int numberOfAgents; //total number of agents who used elevator

        float agentAppearanceRate;
        float elevatorSpeed; //speed of elevator in floors/second
        float loadTime; //length of time in seconds to load/unload passenger
        float timeElapsedPerCycle; //time in seconds passed per loop of program
        float timer; //global timer in seconds
        float waitTime;  //total wait time all agents waited

    public:
/*******************************************************************************
PRE-CONDITION:  Various int and float values corresponding to properites of
                the elvator bank.
POST-CONDITION: Constructs a new elevator object with properties defined by
                constructor arguments.
*******************************************************************************/
        ElevatorBank(int numberOfElevators, int numberOfFloors,
                     int weightLimitIn, int minWeightIn, int maxWeightIn,
                     float agentAppearanceRateIn, float elevatorSpeedIn,
                     float loadTimeIn);

/*******************************************************************************
PRE-CONDITION:  Number of seconds to run simulation for.

POST-CONDITION: Prints to screen average total wait time and average and
                average time waiting for elevator to arrive.

NOTE:           Logistical comments are embedded in function definition.
*******************************************************************************/
        void runSimulation(float numberOfSecondsIn);
};

#endif
