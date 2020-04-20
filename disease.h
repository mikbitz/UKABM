#ifndef disease_H
#define disease_H
#include "process.h"
#include <fstream>
class agent;
class disease:public process{

public:
disease(agent* a);
~disease(){};
void preUpdate();
void update();
void applyUpdate();
//characteristics of the disease
double infectionLikelihood,infectionDistance,duration,recoveryLikelihood;
static void test(),fullMixTest(),fixedSpatialTest();
private:
static disease* testDisease;
void testMessage(string,bool);

};
#endif
