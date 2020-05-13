//Create a synthetic population
#ifndef POPULATIONBUILDER_H
#define POPULATIONBUILDER_H
#include "asciiGrid.h"
#include <iostream>
#include <sstream>
#include <point2D.h>
#include <agent.h>

class populationBuilder{
    unsigned _iter;
    double _remainingHere;
    double _frac;
    asciiGrid _asciiG;
    unsigned _workingPop;
    void retired(agent*);
    void worker(agent*);
    void university(agent*);
    void sixthform(agent*);
    void schoolchild(agent*);
    void preschool(agent*);
    void setSex(agent* a);
    void setAge(agent* a);
    void findPartner(agent* a);
    void findParents(agent* a);
    void setUpEducation(agent* a);
    void setUpWork(std::vector<agent*>&);
    void findFriends(agent* a);
    double commuteDistance();
public:
    
    populationBuilder();
    point2D getNextLocation();
    void startCount();
    int personAtNextLocation();
    void configurePopulation(std::vector<agent*>&agents);

};
#endif
