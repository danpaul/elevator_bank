#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <list>
#include "Agent.h"

//using namespace std;
/*******************************************************************************

*******************************************************************************/
class Elevator
{
    private:
        std::list<Agent> agents;
        std::list<Agent>::iterator agentIterator;
        std::list<int> agentRequests;
        std::list<int> elevatorBankRequests;
        std::list<int>::iterator intIterator;

        bool isAtFloorVariable, isFullVariable;

        int currentFloor, id, numberOfAgents, nextFloor, numberOfFloors,
            weight, weightLimit;

        float elevatorSpeed, loadTime, timeWaitingForElevator, timer,
              position, waitTime;

    public:

        int direction; //bad design choice, should be made private

/*******************************************************************************
POST-CONDITION: Constructs elevtor object based on values passed.
*******************************************************************************/
        Elevator(int weightLimitIn, int numberOfFloorsIn, int idIn,
                 float elevatorSpeedIn, float loadTimeIn);

/*******************************************************************************
PRE-CONDITION:  Weight of next agent to be passed in.
POST-CONDITION: Determines if weight will be exceided by addition of next
                agent.
*******************************************************************************/
        bool checkIfCanAdd(int weightIn);

/*******************************************************************************
POST-CONDITION: Returns true if elevator is currently at a floor.
*******************************************************************************/
        bool isAtFloor();

/*******************************************************************************
POST-CONDITION: Returns ture if elevator has acheived maxWeight.
*******************************************************************************/
        bool isFull();

/*******************************************************************************
POST-CONDITION: Returns the floor elevator is currently at.
*******************************************************************************/
        int getCurrentFloor();

/*******************************************************************************
POST-CONDITION: Returns 1 if going up, -1 if going down, 0 if motionless.
*******************************************************************************/
        int getDirection();

/*******************************************************************************
POST-CONDITION: Returns number of agents that passed through elevator durring
                simulation.
*******************************************************************************/
        int getNumberOfAgents();

/*******************************************************************************
POST-CONDITION: Returns current postion of elevator.
*******************************************************************************/
        float getPosition();

/*******************************************************************************
POST-CONDITION: Returns total time agents spend waiting for this elevator.
*******************************************************************************/
        float getTimeWaitingForElevator();

/*******************************************************************************
POST-CONDITION: Returns total time agents spend waiting for and riding
                elevator.
*******************************************************************************/
        float getWaitTime();

/*******************************************************************************
PRE-CONDITION:  Takes int representing floor arrived at and current global time.
POST-CONDITION: Unloads agents and updates queues.
*******************************************************************************/
        void arrivedAtFloor(int floorArrivedAt, float currentTime);

/*******************************************************************************
PRE-CONDITION:  Takes agent and global time.
POST-CONDITION: Adds agent to elevator queue.
                Tabulates time agent spent waiting for elevator to arrive.
*******************************************************************************/
        void loadAgent(Agent newAgent, float currentTime);

/*******************************************************************************
PRE-CONDITION:  Takes char 'a' for agent request or 'e' for elevator bank
                request.
POST-CONDITION: Inserts into agentRequest or elevatorBankRequest list in order
                of lowest at front, hightest at back.
*******************************************************************************/
        void orderedInsert(char listId, int value);

/*******************************************************************************
POST-CONDITION: Removes all requests for floorNumber.
*******************************************************************************/
        void removeRequest(int floorNumber);

/*******************************************************************************
PRE-CONDITION:  Takes int correspoinding to direction 1 for up, -1 for down,
                0 for no direction.
POST-CONDITION: Sets elevators direction for value passed.
*******************************************************************************/
        void setDirection(int newDirection);

/*******************************************************************************
PRE-CONDITION:  Takes floor arrived at and current global time.
POST-CONDITION: Updates agent tally information for waitTime and updates lists.
*******************************************************************************/
        void unloadAgents(int floor, float currentTime);

/*******************************************************************************
POST-CONDITION: Updates direction and floor of elevator.
*******************************************************************************/
        void updateDirectionAndNextFloor();

/*******************************************************************************
PRE-CONDITION:  Takes time passed since last cycle and current global time.
POST-CONDITION: Updates position of elvator.
*******************************************************************************/
        void updatePosition(float timePassed, float currentTime);

/*******************************************************************************
PRE-CONDITION:  Takes floor for request, '+' for request to add floor, '-' for
                request to remove floor, and 'a' or 'e' for agent or elvatorBank
                requests list.
POST-CONDITION: Updates specified request list.
*******************************************************************************/
        void updateRequestList(int floor, char addOrRemoveKey, char listKey);

};

#endif
