#include <cstdlib>
#include <iostream>

#include "Agent.h"


Agent::Agent(int floorDesiredIn, float startTimeIn, int weightIn)
{
    floorDesired = floorDesiredIn;
    startTime = startTimeIn;
    weight = weightIn;
}

float Agent::getStartTime()
{
    return(startTime);
}

int Agent::getFloorDesired()
{
    return(floorDesired);
}

int Agent::getWeight()
{
    return(weight);
}
