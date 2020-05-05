#ifndef PLACES_H
#define PLACES_H
#include "layer.h"
#include "point2D.h"
#include <map>
#include <string>
class place{
public:
    point2D loc;
    unsigned type;
    place(){type=0;init();}
    place(unsigned t){type=t;init();}
    void init(){_ID=idnum;idnum++;}
    void setType(unsigned t){type=t;}
    void setLocation(point2D p){loc=p;}
    void setLocation(double x, double y){loc=point2D(x,y);}
    unsigned placeType(){
        return type;
    }
    unsigned ID(){return _ID;}
private:
    static unsigned idnum;
    unsigned _ID;
};

//------------------------------------------------------------------
class places{
protected:
    places();
    
public:
    static places& getInstance();
    enum place{home,work};
    void getLocations(std::map<places::place, point2D>& locations);
    void init();
    bool update();
    static places* instance;
    std::map<std::string,unsigned>placeTypes;
    
};
#endif

    
