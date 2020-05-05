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
    double age;
    char sex;
    map<agent*,agent*> children;
    map<agent*,agent*> friends;
    agent* mother,*father;
    agent* partner;

    timeTable tTable;
    bool _inHospital,_critical,_died;
    unsigned numberInfected;
    static unsigned idnum;
    unsigned ID;
    int cellIndex;
    vector<process*> processes;
    map<string,disease>_diseases;
    agent();
    void init();
    void preUpdate();
    void update();
    void applyUpdate();
    void setDest(places::place e);
    void addProcess(process* p);
    void setAge(double);
    void setSex(const char&);
    void updateInfections();
    bool hasDisease(std::string name);
    bool recoveredFrom(std::string name);
    void infectWith(std::string name);
    bool infectious();
    bool exposed();
    void die();
    bool dead();
    bool inHospital();
    bool critical();
};
#endif
