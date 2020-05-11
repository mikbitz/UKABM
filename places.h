#ifndef PLACES_H
#define PLACES_H
#include "layer.h"
#include "point2D.h"
#include <map>
#include <string>
class place{
public:

    place(){_type=0;init();}
    place(unsigned t){_type=t;init();}
    void init(){_ID=idnum;idnum++;_capacity=500;}
    void setType(unsigned t){_type=t;}
    void setLocation(point2D p){loc=p;}
    void setLocation(double x, double y){loc=point2D(x,y);}
    unsigned placeType(){
        return _type;
    }
    unsigned ID(){return _ID;}
    double X(){return loc.x;}
    double Y(){return loc.y;}
    point2D loc;
private:
    static unsigned idnum;
    unsigned _ID;
    unsigned _capacity;
    unsigned _type;
};

//------------------------------------------------------------------
class places{
protected:
    places();
    
public:
    static places& getInstance();
    unsigned operator[](std::string);
    enum placeE{home,work};
    void getLocations(std::map<places::placeE, point2D>& locations);
    void init();
    bool update();
    static places* instance;
    std::map<std::string,unsigned>_placeTypes;
    
};
#endif

    
