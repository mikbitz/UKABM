#ifndef AGENT_H
#define AGENT_H
#include "timetable.h"
#include "paths.h"
#include "process.h"
#include "places.h"
#include "model.h"
#include "disease.h"
class agent{
public:
    map<places::place,point2D> knownLocations;
    places::place oldPlace,newPlace;
    path currentPath;
    unsigned pathState;
    pathSet _pathSet;
    point2D dest,loc,vel;
    float size;
    float age;
    vector<agent*> *friends;
    timeTable tTable;
    bool infected, exposed,recovered;
    static unsigned idnum;
    unsigned ID,classNum;
    int cellIndex;
    vector<process*> processes;
    vector<disease*> diseases;
    agent();
    void init();
    void preUpdate();
    void update();
    void applyUpdate();
    void setDest(places::place e);
    void addProcess(process* p);
};
#endif
