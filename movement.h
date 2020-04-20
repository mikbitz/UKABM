#ifndef movement_H
#define movement_H
#include "process.h"
#include "point2D.h"
class agent;
class movement:public process{
public:
movement(agent* a);
void preUpdate();
void update();
void applyUpdate();
double xdel,ydel,dvx,dvy,vdesired,vmax,vwalk,trelax,randomWobble;
};
#endif
