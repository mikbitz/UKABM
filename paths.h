#ifndef PATHS_H
#define PATHS_H
#include "places.h"
struct path{
    vector<unsigned> steps;
    unsigned state;
    path(){state=0;}
    bool moreSteps(unsigned i){return i<steps.size();}
    void addStep(unsigned p){steps.push_back(p);}
    unsigned getStep(unsigned i){return steps[i];}

    
};

struct pathSet{
    map<unsigned,map<unsigned,path>>paths;//indexed with the placeType that this map leads from and to 
    pathSet(){
        
    }
    void setKnownPaths(){
        homeToWork();
        workToHome();
    }
    void homeToWork(){
        path phw;
        phw.addStep(places::getInstance()["home"]);
        phw.addStep(places::getInstance()["work"]);
        paths[places::getInstance()["home"]][places::getInstance()["work"]]=phw;
    }
    void workToHome(){
        path pwh;
        pwh.addStep(places::getInstance()["work"]);
        pwh.addStep(places::getInstance()["home"]);
        paths[places::getInstance()["work"]][places::getInstance()["home"]]=pwh;
    }

};
#endif
