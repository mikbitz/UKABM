#ifndef AGENT_H
#define AGENT_H
#include "timetable.h"
#include "paths.h"
#include "process.h"
#include "places.h"
#include "model.h"
#include "disease.h"
class agent{
private:
    place* _workPlace;
    place* _home;
    unsigned _jobType;
    enum workTypes{ineducation,unemployed,working,retired};//could also include casual,zero hours...
    workTypes _workStatus ;
    enum educationTypes{preschool,primary,secondary,uppersecondary,higher,postgrad};//coudl also include "further"
    educationTypes _educationStatus;
public:
    map<unsigned,point2D> knownLocations;
    unsigned oldPlace,newPlace;
    path currentPath;
    unsigned pathState;
    pathSet _pathSet;
    point2D loc,dest,vel;
    float size;
    double _age;
    char _sex;
    map<agent*,agent*> children;
    map<agent*,agent*> friends;
    agent* mother,*father;
    agent* partner;


    timeTable tTable;
    bool _inHospital,_critical,_died;
    /**/
    unsigned numberInfected;
    map<string,disease>_diseases;
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
    
    static unsigned idnum;
    unsigned ID;
    int cellIndex;
    vector<process*> processes;

    agent();
    void init();
    void preUpdate();
    void update();
    void applyUpdate();
    void setDest(unsigned);
    void addProcess(process* p);
    void setJobType(const unsigned&);
    void setWorkPlace(place*);
    void setWorkStatus(const std::string&);
    void setEducationStatus(const std::string&);
    void setAge(double);
    void setSex(const char&);
    char sex(){return _sex;}
    double age(){return _age;}
    double X();
    double Y();
    bool hasWork();
    bool worker();

};
#endif
