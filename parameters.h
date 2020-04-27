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
double disease(const std::string& ,const std::string& );
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
std::string diseaseParameterFile;
std::string diseaseHospitalRates;
double latencyTime,recoveryTime,infectionRate,infectionDist;
void setRunNumber(std::string n){runNumber=n;}
protected:
parameters();
parameters(string);
static parameters* instance;
//store parameter name/values
map<std::string,std::stringstream>named;
//2D table of hospital and ciritcalcare rates, indexed by disease name
std::map<std::string, std::map<std::string,  std::map<std::string, double> > > _hospitalParameters;
std::map<std::string,std::map<std::string,double>>_diseaseParameters;
};
#endif
