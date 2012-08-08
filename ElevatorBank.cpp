#include <cstdlib>
#include <iostream>

#include "ElevatorBank.h"

using namespace std;

ElevatorBank::ElevatorBank(int numberOfElevatorsIn, int numberOfFloorsIn,
                           int weightLimitIn, int minWeightIn, int maxWeightIn,
                           float agentAppearanceRateIn, float elevatorSpeedIn,
                           float loadTimeIn)
{

    numberOfElevators = numberOfElevatorsIn;
    numberOfFloors = numberOfFloorsIn;
    weightLimit = weightLimitIn;
    minWeight = minWeightIn;
    maxWeight = maxWeightIn;
    agentAppearanceRate = agentAppearanceRateIn;
    elevatorSpeed = elevatorSpeedIn;
    loadTime = loadTimeIn;

    waitTime = 0.0;

    numberOfAgents = 0;
    timer = 0;

    //time in seconds it takes for elevator to move one floor
    timeElapsedPerCycle = 1/elevatorSpeed;

    elevators = new Elevator *[numberOfElevators];
    floors = new Floor *[numberOfFloors];

    //initialize all elevators
    for(int i = 0; i < numberOfElevators; i++)
    {
        elevators[i] = new Elevator(weightLimit, numberOfFloors, i,
                                    elevatorSpeed, loadTime);
    }
    for(int i = 0; i < numberOfFloors; i++)
    {
        floors[i] = new Floor(agentAppearanceRate, minWeight, maxWeight,
                              i, numberOfFloors-1);
    }
}



void ElevatorBank::runSimulation(float duration){

    /**
    For requests, -1 is an unanswered request, 0 is no requests, 1 is an
    answered request.  ElevatorRespondingTo... holds the number of the elevator
    responding to the request.  -1 if not elevator is responding
    */
    int* upRequests;
    int* elevatorRespondingToUpRequests;
    int* downRequests;
    int* elevatorRespondingToDownRequests;

    upRequests = new int[numberOfFloors];
    elevatorRespondingToUpRequests = new int[numberOfFloors];
    downRequests = new int[numberOfFloors];
    elevatorRespondingToDownRequests = new int[numberOfFloors];

/*******************************************************************************
                INITIALIZE ELEVATORS AND FLOOR REQUEST ARRAYS
*******************************************************************************/

    for(int i = 0; i < numberOfFloors; i++)
    {
        upRequests[i] = 0;
        elevatorRespondingToUpRequests[i] = -1;
        downRequests[i] = 0;
        elevatorRespondingToDownRequests[i] = -1;
    }
    /**Error checking, prevents devide by zero.*/
    if(elevatorSpeed<=0){
        cerr << "Invalid elevator speed.";
        exit(-1);
    }

/*******************************************************************************
                MAIN LOOP OF FUNCTION
*******************************************************************************/

    float proximity;
    int closestElevator;
    while(timer<duration)
    {
/*******************************************************************************
                    GENERATE NEW AGENTS
*******************************************************************************/
        for(int i = 0; i < numberOfFloors; i++)
        {
            floors[i]->generateAgents(timeElapsedPerCycle,timer);
        }

/*******************************************************************************
                    LOAD AGENTS INTO AVAILABLE ELEVATORS
*******************************************************************************/
        for(int i = 0; i < numberOfElevators; i++)
        {
            if(elevators[i]->isAtFloor())
            {
                int direction = elevators[i]->getDirection();
                int floorElevatorIsAt = elevators[i]->getCurrentFloor();

                char directionKey;
                if(direction==1){directionKey='u';}
                else if(direction==-1){directionKey='d';}

                else if(direction==0)
                {
                    if(floors[floorElevatorIsAt]->hasUpRequest())
                    {
                        directionKey='u';
                        elevators[i]->setDirection(1);
                    }
                    else if(floors[floorElevatorIsAt]->hasDownRequest())
                    {
                        directionKey='d';
                        elevators[i]->setDirection(-1);
                    }
                }
                else
                {
                    std::cerr << "Invalid value for elevator direction in "
                              << "ElevatorBank.runSimulation()\n";
                    exit(-1);
                }
                bool weightExceided = false;
                if(directionKey=='u')
                {
                    while(floors[floorElevatorIsAt]->hasUpRequest()&&
                          (!weightExceided))
                    {

                        if(elevators[i]->checkIfCanAdd
                           (floors[floorElevatorIsAt]->
                            getNextAgentsWeight(directionKey)))
                        {
                            elevators[i]->loadAgent((floors[floorElevatorIsAt])
                                                    ->getNextAgent(directionKey),
                                                    timer);
                        }
                        else{weightExceided = true;}
                    }
                }
                else if(directionKey=='d')
                {
                    while(floors[floorElevatorIsAt]->hasDownRequest()&&
                          (!weightExceided))
                    {
                        if(elevators[i]->checkIfCanAdd
                           (floors[floorElevatorIsAt]->
                            getNextAgentsWeight(directionKey)))
                        {
                            elevators[i]->loadAgent((floors[floorElevatorIsAt])
                                                    ->getNextAgent(directionKey),
                                                    timer);
                        }
                        else{weightExceided = true;}
                    }
                }
                else
                {
                    std::cerr << "Error in ElevatorBank.runSimulation\n";
                    exit(-1);

                }
            }
        }
/*******************************************************************************
                        FIND UNANSWERED FLOOR REQUESTS
*******************************************************************************/
        for(int i = 0; i < numberOfFloors; i++)
        {
            if(floors[i]->hasUpRequest()&&upRequests[i]==0)
            {
                upRequests[i] = -1;
            }
            if(floors[i]->hasDownRequest()&&downRequests[i]==0)
            {
                downRequests[i] = -1;
            }
        }

/*******************************************************************************
                        REMOVE INVALID ELEVATOR REQUESTS
*******************************************************************************/
        /**check request arrays, if there are no longers customers at a floor,
           but an elevator was asigned to it, notifty elevator to remove
           request*/
        for(int i = 0; i < numberOfFloors; i++)
        {
            if((elevatorRespondingToUpRequests[i]!=-1)
               &&(!(floors[i]->hasUpRequest())))
            {
                upRequests[i] = -1;
                elevators[elevatorRespondingToUpRequests[i]]->removeRequest(i);
                elevatorRespondingToUpRequests[i]=-1;///////////////////////////////potential error by removing all requests, not just the ones from the elev. bank
            }
            if((elevatorRespondingToDownRequests[i]!=-1)
               &&(!(floors[i]->hasDownRequest())))
            {
                downRequests[i] = -1;
                elevators[elevatorRespondingToDownRequests[i]]->removeRequest(i);
                elevatorRespondingToDownRequests[i]=-1;
            }
        }
/*******************************************************************************
                        FIND RECENTLY FILLED ELEVATORS, SEARCH REQUEST ARRAY
                        FOR FLOORS THEY ARE DESIGNATED TO RESPOND TO AND
                        UPDATE REQUEST SIGNAL TO UNANSWERED (-1)
*******************************************************************************/

        for(int i = 0; i < numberOfElevators; i++)
        {
            if(elevators[i]->isFull())
            {
                for(int j = 0; j < numberOfFloors; j++)
                {
                    if(elevatorRespondingToUpRequests[j]==i)
                    {
                        elevatorRespondingToUpRequests[i] = -1;
                        upRequests[i] = -1;
                    }
                    if(elevatorRespondingToDownRequests[j]==i)
                    {
                        elevatorRespondingToDownRequests[i] = -1;
                        downRequests[i] = -1;
                    }
                }
            }
        }

/*******************************************************************************
                            ATTEMPT TO ASIGN REQUESTS TO ELEVATORS
*******************************************************************************/
        for(int floor = 0; floor < numberOfFloors; floor++)
        {
            proximity = numberOfFloors+1;
            closestElevator = -1;
            /**Check if there is a pending upRequest*/
            if(upRequests[floor]==-1)
            {
                /**Find nearest elevator able to respond (if one is available
                   and asign it.  This alogrithm could be improved by including
                   analysis of number of stops prior to reaching calling floor.
                   */
                for(int elev = 0; elev < numberOfElevators; elev++)
                {
                    if((!elevators[elev]->isFull())&&
                       ((elevators[elev]->getPosition()<floor)&&
                        ((elevators[elev]->direction==1||
                          (elevators[elev]->direction==0))&&
                         (abs(floor-elevators[elev]->getPosition())<proximity)))||
                       ((elevators[elev]->getPosition()>floor)&&
                        (elevators[elev]->direction==0)&&
                        (abs(elevators[elev]->getPosition()-floor)<proximity)))

                          {
                              proximity = abs(floor-elevators[elev]->
                                              getPosition());
                              closestElevator = elev;
                          }
                }
                if(closestElevator!=-1)
                {
                    elevators[closestElevator]->updateRequestList(floor,'+','e');
                    upRequests[floor] = 1;
                    elevatorRespondingToUpRequests[floor] = closestElevator;
                }
            }

            proximity = numberOfFloors+1;
            closestElevator = -1;

            if(downRequests[floor]==-1)
            {
                for(int elev = 0; elev < numberOfElevators; elev++)
                {
                    if((!elevators[elev]->isFull())&&

                       ((elevators[elev]->getPosition()>floor)&&
                        ((elevators[elev]->direction==-1)||
                         (elevators[elev]->direction==0))&&
                        (abs(elevators[elev]->getPosition())-floor<proximity))||
                       (((elevators[elev]->getPosition()<floor)&&
                         (elevators[elev]->direction==0))
                        &&abs(floor-(elevators[elev]->getPosition())<proximity)))
                          {
                              proximity = abs(floor-elevators[elev]->
                                              getPosition());
                              closestElevator = elev;
                          }
                }
                if(closestElevator!=-1)
                {
                    elevators[closestElevator]->updateRequestList(floor,'+','e');
                    downRequests[floor] = 1;
                    elevatorRespondingToDownRequests[floor] = closestElevator;
                }
            }
        }

/*******************************************************************************
                            UPDATE POSITION OF ELEVATORS
                            INCREMENT TIMER
*******************************************************************************/

        for(int i = 0; i<numberOfElevators; i++)
        {
            elevators[i]->updatePosition(timeElapsedPerCycle,timer);
        }

        timer += timeElapsedPerCycle;
    }



    float timeWaitingForElevator;
    for(int i = 0; i < numberOfElevators; i++)
    {
        numberOfAgents += elevators[i]->getNumberOfAgents();
        waitTime += elevators[i]->getWaitTime();
        timeWaitingForElevator += elevators[i]->getTimeWaitingForElevator();
    }

    std::cout << "For " << numberOfElevators
              << " elevators average total wait time is "
              << waitTime/numberOfAgents/60 << " minutes.\n";

    std::cout << "For " << numberOfElevators
              << " elevators average time waiting for elevator is "
              << timeWaitingForElevator/numberOfAgents/60 << " minutes.\n\n";

    delete[] upRequests;
    delete[] elevatorRespondingToUpRequests;
    delete[] downRequests;
    delete[] elevatorRespondingToDownRequests;

}
