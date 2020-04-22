#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <string>
#include <vector>
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
void getParameters();
double timeStep;
bool visible;
int randomSeed,nsteps;
string initialDate,finalDate;
string populationGridFile;
string agentFactoryType;
string parameterFile,summaryFileName,infectionMapFileName,recoveryLocationFileName,inputFileName,restartFileName;
int numberOfAgents;
double agentFraction;
int NxCells,NyCells,restartInterval,outputInterval;
double xSize,ySize,x0,y0;
bool isRestart;
double recoveryTime,infectionRate,infectionDist;
protected:
parameters();
parameters(string);
static parameters* instance;
};
#endif
