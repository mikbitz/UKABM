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
public:
    
    populationBuilder();
    point2D getNextLocation();
    void startCount();
    int personAtNextLocation();
    void configurePopulation(std::vector<agent*>agents);
    void setSex(agent* a);
    void setAge(agent* a);
    void findPartner(agent* a);
    void findParents(agent* a);
    void education(agent* a);
    void setUpWork(agent* a);
};
#endif
