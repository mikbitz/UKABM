//Create a synthetic population
#ifndef POPULATIONBUILDER_H
#define POPULATIONBUILDER_H
#include "asciiGrid.h"
#include <iostream>
#include <sstream>
#include <point2D.h>

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
    };
#endif
