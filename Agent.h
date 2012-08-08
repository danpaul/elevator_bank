#ifndef AGENT_H
#define AGENT_H


class Agent
{
    private:
        int floorDesired, weight;
        float startTime;
    public:
/*******************************************************************************
POST-CONDITION: Agent is constucted with a request defined by floorDesiredIn,
                startTime defined by global time, and a weight.
*******************************************************************************/
        Agent(int floorDesiredIn, float startTimeIn, int weightIn);

/*******************************************************************************
POST-CONDITION: Returns floor agent requests to go to.
*******************************************************************************/
        int getFloorDesired();

/*******************************************************************************
POST-CONDITION: Returns agents weight.
*******************************************************************************/
        int getWeight();

/*******************************************************************************
POST-CONDITION: Returns time agent was created.
*******************************************************************************/
        float getStartTime();
};

#endif
