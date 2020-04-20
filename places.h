#ifndef PLACES_H
#define PLACES_H
#include "layer.h"
#include "point2D.h"
#include <map>
#include <string>
class places{
protected:
    places();
    
public:
    static places* getInstance();
    enum place{home,work};
    void getLocations(std::map<places::place, point2D>& locations);
    void init();
    bool update();
    static places* instance;
    
};
#endif

    
