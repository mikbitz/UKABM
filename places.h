#ifndef PLACES_H
#define PLACES_H
#include "layer.h"
#include "point2D.h"
#include <map>
#include<vector>
#include <string>
#include <iostream>
class place{
public:

    place(){_type=0;init();}
    place(unsigned t){_type=t;init();}
    void init(){_ID=idnum;idnum++;_workForceSize=0;_capacity=500;}
    void setType(unsigned t){_type=t;}
    void setLocation(point2D p){loc=p;}
    void setLocation(double x, double y){loc=point2D(x,y);}
    void incrementWorkForce(){_workForceSize++;}
    unsigned placeType(){
        return _type;
    }
    unsigned ID(){return _ID;}
    double X(){return loc.x;}
    double Y(){return loc.y;}
    unsigned workForceSize(){return _workForceSize;}
    point2D loc;
private:
    static unsigned idnum;
    unsigned _ID;
    unsigned _workForceSize;
    unsigned _capacity;
    unsigned _type;
};

//------------------------------------------------------------------
class places{
protected:
    places();
    std::vector<place*>_allPlaces;
public:
    static places& getInstance();
    unsigned operator[](std::string);
    void init();
    bool update();
    static places* instance;
    static unsigned unknownType; 
    std::map<std::string,unsigned>_placeTypes;
    void printWorkForceSizes();
    
};
#endif

    
