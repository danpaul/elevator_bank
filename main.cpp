#include<cstdlib>
#include<fstream>
#include <iostream>
#include<string>

#include "ElevatorBank.h"

using namespace std;

/*******************************************************************************
                            DATA/CONSTANTS
*******************************************************************************/
struct ElevatorBankData
{
    int elevators;
    int floors;
    int weight_limit;
    int min_weight;
    int max_weight;
    float customers; //per second probability of customer appearing on floor
                     //defined in file as per/minute
};

string ELEVATOR_BANK_DATA_KEYS[6] = {"elevators", "floors", "weight_limit",
                                     "min_weight","max_weight","customers"};
int NUMBER_OF_KEYS = 6;
float ELEVATOR_SPEED = .166666; //elevator velocity in floors/second.
float LOAD_TIME = 60; //time in seconds it takes to load/unload agents on floor
char COMMENT_CHAR = '#';
int SIMULATION_RUN_TIME = 28000; //time in seconds to run simulation



/*******************************************************************************
                            FUNCTION DEFINITIONS
*******************************************************************************/

/*******************************************************************************
PRE-CONDITION:  Takes struct with ElevatorBankData (defined above), a char
                repesenting the first char of comment lines, argc and argv
                passed from command line.
POST-CONDITION: Extracts ElevatorBankData from properly formatted file or
                ends program and reports error.
*******************************************************************************/
bool parseFile(struct ElevatorBankData *dataContainer, string keys[],
               char commentChar, int argc, char *argv[]);

/*******************************************************************************
PRE-CONDITION:  Helper function for parseFile().
POST-CONDITION: Takes non-comment, non-blank file line and extracts
                ElevatorBankData if any are present.
*******************************************************************************/
void updateElevatorBankData(struct ElevatorBankData *elevatorBankData,
                            string key, string line);

/*******************************************************************************
PRE-CONDITION:  Helper function for updateElevatorBankData().
                Takes a C++ string.
POST-CONDITION: Returns first int or float in string if present.
                Program terminates with error message if no numbers are present.
*******************************************************************************/
float getFloatInString(string stringIn);



/*******************************************************************************
                            MAIN
*******************************************************************************/
int main(int argc, char *argv[])
{
    srand(time(NULL));

    ElevatorBankData elevatorBankData;

    parseFile(&elevatorBankData, ELEVATOR_BANK_DATA_KEYS, COMMENT_CHAR, argc,
              argv);

    ElevatorBank elevatorBank(elevatorBankData.elevators,
                              elevatorBankData.floors,
                              elevatorBankData.weight_limit,
                              elevatorBankData.min_weight,
                              elevatorBankData.max_weight,
                              elevatorBankData.customers,
                              ELEVATOR_SPEED,
                              LOAD_TIME);


    ElevatorBank elevatorBank2(elevatorBankData.elevators-1,
                              elevatorBankData.floors,
                              elevatorBankData.weight_limit,
                              elevatorBankData.min_weight,
                              elevatorBankData.max_weight,
                              elevatorBankData.customers,
                              ELEVATOR_SPEED,
                              LOAD_TIME);

    elevatorBank.runSimulation(SIMULATION_RUN_TIME);
    elevatorBank2.runSimulation(SIMULATION_RUN_TIME);

    return 0;
}

/*******************************************************************************
                            FUNCTIONS
*******************************************************************************/
bool parseFile(struct ElevatorBankData *elevatorBankData, string keys[],
               char commentChar, int argc, char *argv[])
{
    int numberOfKeys = NUMBER_OF_KEYS; //must be changed if numberOfKeys changes

    //Sets values of elevatorBankData to invalid values for error checking at
    //end of function.
    elevatorBankData->elevators = -1;
    elevatorBankData->floors = -1;
    elevatorBankData->weight_limit = -1;
    elevatorBankData->min_weight = -1;
    elevatorBankData->max_weight = -1;
    elevatorBankData->customers = -1;

    //Open File and validate input.
    ifstream fin;
    if(argc!=2)
    {
        cerr << "Error: incorrect number of command line arguments.\n";
        exit(-1);
    }
    fin.open(argv[1]);
    if(fin.fail())
    {
        cerr << "Invalid file input.\n";
        exit(-1);
    }

    //File parsing.
    string nextLine;
    int counter = 0;
    bool foundKey = false;
    while(getline(fin, nextLine))
    {
        if(nextLine[0]!=commentChar&&nextLine.size()>0)
        {
            counter = 0;
            foundKey = false;

            while(counter<numberOfKeys&&!foundKey)
            {
                if(nextLine.find(keys[counter])!= string::npos)
                {
                    updateElevatorBankData(elevatorBankData, keys[counter],
                                           nextLine);
                    foundKey = true;
                }
                counter++;
            }
        }
    }

    fin.close();

    //Error checking.
    if(elevatorBankData->elevators<0||elevatorBankData->floors<0||
        elevatorBankData->weight_limit<0||elevatorBankData->min_weight<0||
        elevatorBankData->max_weight<0||elevatorBankData->customers<0)
       {
           cerr << "Invalid value(s) supplied by input file.\n";
           exit(-1);
       }

    return true;
}

void updateElevatorBankData(struct ElevatorBankData *elevatorBankData, string key,
                            string line)
{
    if(key=="elevators")
        {elevatorBankData->elevators = getFloatInString(line);}
    else if(key=="floors")
        {elevatorBankData->floors = getFloatInString(line);}
    else if(key=="weight_limit")
        {elevatorBankData->weight_limit = getFloatInString(line);}
    else if(key=="min_weight")
        {elevatorBankData->min_weight = getFloatInString(line);}
    else if(key=="max_weight")
        {elevatorBankData->max_weight = getFloatInString(line);}
    else if(key=="customers")
        {
            float perMinuteCustomerRate = getFloatInString(line);
            if(perMinuteCustomerRate<=0)
            {
                cerr << "Customer rate can not be 0 or less.";
                exit(-1);
            }
            elevatorBankData->customers = perMinuteCustomerRate/60;
            }
    else
    {
        cerr << "unkown key in updateElevatorBankData function";
        exit(-1);
    }
}

float getFloatInString(string stringIn)
{
    string numbers = ".0123456789";
    size_t startOfNumberString = stringIn.find_first_of(numbers);
    if(startOfNumberString == string::npos)
    {
        cerr << "Invalid formatting of input file.";
        exit(-1);
    }
    size_t endOfNumberString = stringIn.find_first_not_of(numbers,
                                                          startOfNumberString);
    string numberString = stringIn.substr(startOfNumberString,
                                          endOfNumberString-startOfNumberString);
    float returnValue;
    sscanf(numberString.c_str(), "%f", &returnValue);
    return(returnValue);
}
