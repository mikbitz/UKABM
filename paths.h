#ifndef PATHS_H
#define PATHS_H
#include "places.h"
struct path{
    vector<places::placeE> steps;
    unsigned state;
    path(){state=0;}
    void addStep(places::placeE p){steps.push_back(p);}
    places::placeE getStep(unsigned i){return steps[i];}
    bool moreSteps(unsigned i){return i<steps.size();}
    
};

struct pathSet{
    map<places::placeE, map<places::placeE, path> > paths;
    pathSet(){
        
    }
    void setKnownPaths(){
        homeToWork();
        workToHome();
    }
    void homeToWork(){
        path p1;
        p1.addStep(places::home);
        p1.addStep(places::work);
        paths[places::home][places::work]=p1;
    }
    void workToHome(){
        path p1;
        p1.addStep(places::work);
        p1.addStep(places::home);
        paths[places::work][places::home]=p1;
    }

};
#endif
