#ifndef FLOOR_H
#define FLOOR_H

#include "Agent.h"
#include <list>
/*******************************************************************************
NOTE:           If agents are to be generated at a rate greater than, less than
                one per minute, generateAgents() must be modified.
*******************************************************************************/
class Floor
{
    private:
        std::list<Agent> upAgents;
        std::list<Agent> downAgents;

        float agentAppearanceRate;
        int minWeight, maxWeight;
        int thisFloor, topFloor;


    public:
/*******************************************************************************
POST-CONDITION: Floor object randomly generates agents between min and maxWeight
                at a rate of agentAppearanceRate defined as
*******************************************************************************/
        Floor(float agentAppearanceRateIn, int minWeightIn, int maxWeightIn,
              int thisFloor, int topFloor);

/*******************************************************************************
PRE-CONDITION:  Char key 'u' or 'd' representing an agent with a request to
                either go up or down.
POST-CONDITION: Return agent going in direction 'u' or 'd' for up or down.
*******************************************************************************/
        Agent getNextAgent(char upOrDownKey);

/*******************************************************************************
PRE-CONDITION:  Char key 'u' or 'd' representing an agent with a request to
                either go up or down.
POST-CONDITION: Returns the weight of the next agent going up or down.
*******************************************************************************/
        int getNextAgentsWeight(char upOrDownKey);

/*******************************************************************************
POST-CONDITION: Returns true if there is a pending agent going up.
*******************************************************************************/
        bool hasDownRequest();

/*******************************************************************************
POST-CONDITION: Returns true if there is a pending agent going down.
*******************************************************************************/
        bool hasUpRequest();

/*******************************************************************************
PRE-CONDITION:  Takes float representing time elapsed since last call to
                function and float representing current global time.
POST-CONDITION: Generates agents per timeElapsed period at a probability
                defined in Floor constructor.
*******************************************************************************/
        void generateAgents(float timeElapsedIn, float currentTime);
};


#endif
