#include "movement.h"
#include "agent.h"
   movement::movement(agent* a):process(a){
        vdesired=0.5;trelax=0.6;vmax=1.3*vdesired;
        vwalk=0.5;dvx=0;dvy=0;randomWobble=0.01;
    }
//-----------------------------------------------------------------------------------------------------------------
    void movement::preUpdate(){
        double dt=parameters::getInstance().timeStep;
        randomWobble=.05;trelax=0.6;
        xdel=owner->dest.x-owner->loc.x;
        ydel=owner->dest.y-owner->loc.y;

        if (owner->pathState==1)vdesired=vwalk;
        float sz=sqrt(xdel*xdel+ydel*ydel);
        if (sz>1.e-7){xdel=xdel/sz;ydel=ydel/sz;}else{xdel=0;ydel=0;}
        //are we close enough to the current target?  Also we need to take account of the distance moved in a timestep.
//         if (sz<owner->size/2+dt*sqrt(owner->vel.x*owner->vel.x + owner->vel.y*owner->vel.y)){
//             owner->pathState++;
//             if (owner->currentPath.moreSteps(owner->pathState)){
//                 owner->dest=owner->knownLocations[owner->currentPath.getStep(owner->pathState)];
//             }else{
//                 if (sz<vdesired)vdesired+=0.2*(sz-vdesired);
//             }
//         }
    }
    //-----------------------------------------------------------------------------------------------------------------
    void movement::update(){
        double dt=parameters::getInstance().timeStep;
        dvx=(vdesired*xdel-owner->vel.x)/trelax+randomWobble*(0.5-model::getInstance().random.number());
        dvy=(vdesired*ydel-owner->vel.y)/trelax+randomWobble*(0.5-model::getInstance().random.number());
    }
    //-----------------------------------------------------------------------------------------------------------------
    void movement::applyUpdate(){
        double dt=parameters::getInstance().timeStep;
        owner->vel.x+=dvx*dt;
        owner->vel.y+=dvy*dt;
        float sz=sqrt(owner->vel.x*owner->vel.x+owner->vel.y*owner->vel.y);
        if (sz>vmax){owner->vel.x*=vmax/sz;owner->vel.y*=vmax/sz;}
        owner->loc.x+=owner->vel.x*dt;
        owner->loc.y+=owner->vel.y*dt; 
    }
