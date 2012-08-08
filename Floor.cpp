#include <cstdlib>
#include <iostream>

#include "Floor.h"


Floor::Floor(float agentAppearanceRateIn, int minWeightIn, int maxWeightIn,
             int thisFloorIn, int topFloorIn)
{
    minWeight = minWeightIn;
    maxWeight = maxWeightIn;
    agentAppearanceRate = agentAppearanceRateIn;

    thisFloor = thisFloorIn;
    topFloor = topFloorIn;
}

void Floor::generateAgents(float timeElapsed, float currentTime)
{
    if((rand()/double(RAND_MAX))<(timeElapsed*agentAppearanceRate))
    {
        int floorDesired = -1;
        while(floorDesired == -1)
        {
            floorDesired = rand()%(topFloor+1);
            if(floorDesired == thisFloor){floorDesired = -1;}
        }
        if((floorDesired<thisFloor)&&(floorDesired>=0))
        {
            Agent newAgent(floorDesired, currentTime,
                           rand()%(maxWeight-minWeight)+minWeight);
            downAgents.push_back(newAgent);
        }
        else if((floorDesired>thisFloor)&&(floorDesired<=topFloor))
        {
            Agent newAgent(floorDesired, currentTime,
                           rand()%(maxWeight-minWeight)+minWeight);
            upAgents.push_back(newAgent);
        }
        else
        {
            std::cerr << "Invalid value allowed for Floor.";
            exit(-1);
        }
    }
}

int Floor::getNextAgentsWeight(char upOrDownKey)
{
    if(upOrDownKey=='u'){return(upAgents.front().getWeight());}
    else if(upOrDownKey=='d'){return(downAgents.front().getWeight());}
    else
    {
        std::cerr << "Invalid value to Floor.getNextAgentsWeight\n";
        exit(-1);
    }
}

Agent Floor::getNextAgent(char upOrDownKey)
{
    if(upOrDownKey=='u')
    {
        Agent returnAgent = upAgents.front();
        upAgents.pop_front();
        return(returnAgent);
    }
    else if(upOrDownKey=='d')
    {
        Agent returnAgent = downAgents.front();
        downAgents.pop_front();
        return(returnAgent);
    }
    else
    {
        std::cerr << "Invalid value to Floor.getNextAgent\n";
        exit(-1);
    }
}

bool Floor::hasUpRequest()
{
    if(!upAgents.empty()){return true;}
}

bool Floor::hasDownRequest()
{
    if(!downAgents.empty()){return true;}
}

