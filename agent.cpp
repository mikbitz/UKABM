#include "agent.h"
#include "disease.h"
//-----------------------------------------------------------------------------------------------------------------
    agent::agent(){
        ID=idnum;idnum++;
        infected=false;exposed=false;recovered=false;
        cellIndex=-1;
    }
//-----------------------------------------------------------------------------------------------------------------
    void agent::init(){

        size=1.;
        //places::getInstance()->getLocations(knownLocations);
 
        loc =knownLocations[places::home];oldPlace=places::home;
        dest=knownLocations[places::home];newPlace=places::home;

        _pathSet.setKnownPaths();

    }
//-----------------------------------------------------------------------------------------------------------------
    void agent::preUpdate(){
        for (unsigned i=0;i<processes.size();i++)processes[i]->preUpdate();
    }
    //-----------------------------------------------------------------------------------------------------------------
    void agent::update(){
        for (auto d:diseases)d.second->update();
        for (unsigned i=0;i<processes.size();i++)processes[i]->update();

    }
//-----------------------------------------------------------------------------------------------------------------
    void agent::applyUpdate(){
        if (exposed && !infected && !recovered){diseases["covid"]=new disease(this);}
        for (auto d:diseases)d.second->applyUpdate();
        //check in the agent's schedule to see whether it is time for the next activity.
        if (tTable.update()){setDest(tTable.getCurrent().place);}
        for (unsigned i=0;i<processes.size();i++)processes[i]->applyUpdate();

    }
//-----------------------------------------------------------------------------------------------------------------
    void agent::setDest(places::place e){
        //cout<<"set Dest "<<newPlace<<" "<< oldPlace<<" "<<e<<endl;
        oldPlace=newPlace;
        newPlace=e;
        assert(oldPlace != newPlace);
        currentPath=_pathSet.paths[oldPlace][newPlace];
        pathState=1;
        dest=knownLocations[currentPath.getStep(pathState)]; 
    }
//-----------------------------------------------------------------------------------------------------------------
    void agent::addProcess(process* p){
      processes.push_back(p); 
    }
