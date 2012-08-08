
/*******************************************************************************

*******************************************************************************/
#include <cstdlib>
#include <iostream>
#include "Elevator.h"

Elevator::Elevator(int weightLimitIn, int numberOfFloorsIn, int idIn,
                   float speedIn, float loadTimeIn)
{
    weightLimit = weightLimitIn;
    numberOfFloors = numberOfFloorsIn;
    id = idIn;
    elevatorSpeed = speedIn;
    loadTime = loadTimeIn;

    isFullVariable = false;
    isAtFloorVariable = false;

    position = 0.0;
    timeWaitingForElevator = 0.0;

    currentFloor = 0;
    direction = 0;
    nextFloor = 0;
    numberOfAgents = 0;
    timer = 0;
    waitTime = 0;
    weight = 0;
}

bool Elevator::checkIfCanAdd(int weightIn)
{
    if(weight+weightIn >= weightLimit)
    {
        isFullVariable = true;
        elevatorBankRequests.clear();
        return false;
    }
    else{return true;}
}

bool Elevator::isAtFloor()
{
    if(isAtFloorVariable){return true;}
}
bool Elevator::isFull()
{
    if(isFullVariable){return true;}
}

float Elevator::getPosition()
{
    return position;
}

float Elevator::getWaitTime()
{
    return(waitTime);
}

float Elevator::getTimeWaitingForElevator()
{
    return(timeWaitingForElevator);
}

int Elevator::getCurrentFloor()
{
    return(currentFloor);
}
int Elevator::getDirection()
{
    return direction;
}

int Elevator::getNumberOfAgents()
{
    return(numberOfAgents);
}
/**probable location of bugs
*/
void Elevator::orderedInsert(char listId, int valueToInsert)
{
    if(listId=='a')
    {
        if(agentRequests.empty())
        {
            agentRequests.push_front(valueToInsert);
        }
        else
        {
            intIterator = agentRequests.begin();
            for(intIterator; intIterator!=agentRequests.end(); intIterator++)
            {
                if(*intIterator>=valueToInsert)
                {
                    if(*intIterator==valueToInsert){return;}
                    else{
                        agentRequests.insert(intIterator,valueToInsert);
                        return;
                    }
                }
            }
            agentRequests.push_back(valueToInsert);
            return;
        }
    }

    else if(listId=='e')
    {
        if(elevatorBankRequests.empty())
        {
            elevatorBankRequests.push_front(valueToInsert);
        }
        else
        {
            intIterator = elevatorBankRequests.begin();
            for(intIterator; intIterator!=elevatorBankRequests.end();
                intIterator++)
            {

                if(*intIterator>=valueToInsert)
                {
                    if(*intIterator==valueToInsert){return;}
                    else{
                        elevatorBankRequests.insert(intIterator,valueToInsert);
                        return;
                    }
                }
            }
            elevatorBankRequests.push_back(valueToInsert);
            return;
        }
    }
    else
    {
        std::cerr << "invalid char in Elevator.orderedInsert\n";
        exit(-1);
    }
}

void Elevator::updateRequestList(int floor, char addOrRemoveKey, char listKey)
{
    if(addOrRemoveKey=='+')
    {
        if((listKey=='a')||(listKey=='e')){
            orderedInsert(listKey, floor);
            if(floor>position)
            {
                direction = 1;
            }
            else if(floor<position)
            {
                direction = -1;
            }
            else
            {
                std::cerr << "Invalid value supplied to "
                          << "Elevator.updateRequestList (can't be same as fl\n";
                exit(-1);
            }
        }
        else
        {
            std::cerr << "Invalid value supplied to Elevator.updateRequestList\n";
            exit(-1);
        }
    }
    else if(addOrRemoveKey=='-')
    {
        if(listKey=='a'){agentRequests.remove(floor);}
        else if(listKey=='e'){elevatorBankRequests.remove(floor);}
        else
        {
            std::cerr << "Invalid value supplied to Elevator.updateRequestList\n";
            exit(-1);
        }
    }
    else
    {
        std::cerr << "Invalid value supplied to Elevator.updateRequestList\n";
        exit(-1);
    }
}


void Elevator::updateDirectionAndNextFloor()
{
    if(agentRequests.empty()&&elevatorBankRequests.empty())
    {
        direction = 0;
    }
    else if(!(isAtFloorVariable))
    {
        int highestRequest, lowestRequest;

        if(agentRequests.empty())
        {
            highestRequest = elevatorBankRequests.back();
            lowestRequest = elevatorBankRequests.front();
        }
        else if(elevatorBankRequests.empty())
        {
            highestRequest = agentRequests.back();
            lowestRequest = agentRequests.front();
        }
        else
        {
            if(elevatorBankRequests.back()>=agentRequests.back())
            {
                highestRequest = elevatorBankRequests.back();
            }
            else{highestRequest = agentRequests.back();}
            if(elevatorBankRequests.front()<=agentRequests.front())
            {
                lowestRequest = elevatorBankRequests.front();
            }
            else{lowestRequest = agentRequests.front();}
        }

        if(lowestRequest<position)
        {
            direction = -1;
            nextFloor = highestRequest;

            if(highestRequest>position)
            {
                std::cerr << "Error in Elevator.updateDirectionAndFloor\n";
                exit(-1);
            }
        }
        else if(highestRequest>position)
        {
            direction = 1;
            nextFloor = lowestRequest;
            if(lowestRequest<position)
            {
                std::cerr << "Error in Elvator.updateDirectionAndFloor\n";
                exit(-1);
            }

        }
        else
        {
            std::cerr << "Error in Elevator.updateDirectionAndFloor\n"
                      << "Request made at current floor\n";
            exit(-1);
        }
    }
}

void Elevator::updatePosition(float timePassed, float currentTime)
{
    if(isAtFloorVariable)
    {
        timer += timePassed;
        if(timer >= loadTime)
        {
            isAtFloorVariable=false;
            timer = 0;
        }
    }


    else
    {
        updateDirectionAndNextFloor();
        if(direction!=0)
        {
            position+=((float)direction)*(timePassed*elevatorSpeed);
            if(direction==1)
            {
                if(nextFloor<=position)
                    {arrivedAtFloor(nextFloor,currentTime);}
            }
            else
            {
                if(nextFloor>=position)
                    {arrivedAtFloor(nextFloor,currentTime);}
            }
            updateDirectionAndNextFloor();
        }
    }
}

void Elevator::arrivedAtFloor(int floor, float currentTime)
{
    position = floor;
    currentFloor = floor;
    isAtFloorVariable = true;
    agentRequests.remove(floor);
    elevatorBankRequests.remove(floor);
    unloadAgents(floor, currentTime);
}

void Elevator::unloadAgents(int floor, float currentTime)
{
    bool unloadedAgent = true;

    while(unloadedAgent)
    {
        unloadedAgent = false;
        if(!agents.empty())
        {
            agentIterator = agents.begin();
            while(agentIterator!=agents.end())
            {
                if(agentIterator->getFloorDesired()==floor)
                {
                    unloadedAgent = true;
                    if(currentTime<agentIterator->getStartTime())
                    {
                        std::cerr << "error in Elevator.unloadAgents() "
                                  << "max value of time exceided\n";
                        exit(-1);
                    }
                    waitTime += currentTime-(agentIterator->getStartTime());
                    numberOfAgents++;
                    weight-=agentIterator->getWeight();
                    if(isFullVariable){isFullVariable=false;}
                    agentIterator = agents.erase(agentIterator);
                }
                else{agentIterator++;}
            }
        }
    }
}

void Elevator::loadAgent(Agent newAgent, float currentTime)
{
    weight += newAgent.getWeight();
    timeWaitingForElevator += currentTime-newAgent.getStartTime();
    orderedInsert('a',newAgent.getFloorDesired());
    agents.push_back(newAgent);
}

void Elevator::removeRequest(int floorNumber)
{
    elevatorBankRequests.remove(floorNumber);
}

void Elevator::setDirection(int newDirection)
{
    direction = newDirection;
}
