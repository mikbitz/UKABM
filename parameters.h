#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

/**
\file parameters.h
\brief The parameters class header file

This class stores a set of model parameters. It is a singleton.
*/ 
class parameters{
public:
static parameters& getInstance();
static parameters& getInstance(string);
~parameters();
void printParameters();
void saveParameters();
void getParameters();
double timeStep;
bool visible;
int randomSeed,nsteps;
std::string initialDate,finalDate;
std::string populationGridFile;
std::string agentFactoryType;
std::string parameterFile,summaryFileName;
std::string infectionMapFileName,populationMapFileName,inputFileName,restartFileName;
std::string experimentOutputDirectory,experimentName,experimentDescription,runNumber;
int numberOfAgents;
double agentFraction;
int NxCells,NyCells,restartInterval,outputInterval;
double xSize,ySize,x0,y0;
bool isRestart;
double latencyTime,recoveryTime,infectionRate,infectionDist;
void setRunNumber(std::string n){runNumber=n;}
protected:
parameters();
parameters(string);
static parameters* instance;
map<std::string,std::stringstream>named;

};
#endif
