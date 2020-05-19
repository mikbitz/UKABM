/**
 * @file agent.h 
 * @brief The agent class definition file
 * 
 * @author Mike Bithell
 **/
#ifndef AGENT_H
#define AGENT_H
#include "timetable.h"
#include "paths.h"
#include "process.h"
#include "places.h"
#include "model.h"
#include "disease.h"
/**
 * @class agent
 * 
 * @brief Agents represent individual people
 * 
 * 
 **/ 
class agent{
private:
    place* _workPlace;
    place* _home;
    unsigned _jobType;
    enum workTypes{ineducation,unemployed,working,retiree};//could also include casual,zero hours...
    workTypes _workStatus ;
    enum educationTypes{preschool,primary,secondary,uppersecondary,higher,postgrad};//coudl also include "further"
    educationTypes _educationStatus;
    char _sex;
    agent* _partner;
    agent* _mother,*_father;
    vector<agent*> _children;
public:
    map<unsigned,point2D> knownLocations;
    unsigned oldPlace,newPlace;
    path currentPath;
    unsigned pathState;
    pathSet _pathSet;
    point2D loc,dest,vel;
    float _size;
    double _age;

    vector<agent*> friends;

    void makePartner(agent*);
    void makeParents(agent*);
    void addChild(agent*);
    
    
    timeTable tTable;
    /**
     * @brief Set to true if the agent is currently in hospital
     * 
     */
    bool _inHospital;
    /**
     * @brief Set to true if the agent is in hospital and in critical care
     * 
     */
    bool _critical;
    /**
     * @brief Set to true if the agent has died (for any reason...)
     * 
     */
    bool _died;

    /**
     * @brief Accumulates the number of other agents infected by this agent
     * 
     * Currently this is not distinguished by disease so really only works if there is just a single disease 
     * @todo change to a version that can accumlate infections by named disease (NB this value is used in model.cpp to find R0)
     */
    unsigned numberInfected;
    /**
     * @brief A dictionary of named diseases currently carried by this agent
     * 
     * If the agent has never had a given disease, then there will be no entry in the dictionary. Such agents are assumed to be susceptible.
     * 
     */
    map<string,disease>_diseases;
    void updateInfections();
    /**
     * @brief Check to see whether the agent has ever carried a disease with the given name
     * 
     * Returns false if there is no entry in the dictionary of diseases, true if it is present, even if teh agent has recovered or died
     * This allows for testing recovered agents (or for them to have limited time immunity), or for dead agents still to be infectious
     * 
     * @param name :A string specifying the name of the disease to test for
     * @return bool
     */
    bool hasDisease(std::string);
    /**
     * @brief Check to see whether the agent has recovered from a named disease
     * 
     * @param  name :A string specifying the name of the disease to test for 
     *
     * @return bool
     */
    bool recoveredFrom(std::string);
    /**
     * @brief Infect this agent with a disease with a given name
     * 
     * This both adds the disease name to the dictionary and creates a new disease object with the name of the disease
     * The disease object is then updated to the "infected" state, which indicates that this is a newly acquired disease.
     * 
     * @param  name :A string specifying the name of the disease
     */
    void infectWith(std::string);
    /**
     * @brief True if the agent can infect other agents
     * 
     * Currently only applies for Covid19! This function is used in creating maps in output.cpp via the searchGrid, so be careful about modifying 
     * @todo Should take a disease name as the argument
     * 
     * @return bool
     */
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
    char sex();
    double age();
    agent* partner();
    agent* mother();
    agent* father();
    /**
     * @brief Return the current X location of the agent
     * 
     * @return double
     */
    double X();
    /**
     * @brief Return the current Y location of the agent
     * 
     * @return double
     */
    double Y();
    bool hasWork();
    bool worker();
    bool inEducation();
    bool retired();

};
#endif
