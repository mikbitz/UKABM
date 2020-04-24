#include "agent.h"
#include "disease.h"
//-----------------------------------------------------------------------------------------------------------------
agent::agent(){
    ID=idnum;idnum++;
    infected=false;exposed=false;recovered=false;numberInfected=0;
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
    updateDiseases();
    for (unsigned i=0;i<processes.size();i++)processes[i]->update();
    
}
//-----------------------------------------------------------------------------------------------------------------
void agent::applyUpdate(){
    
    //check in the agent's schedule to see whether it is time for the next activity.
    if (tTable.update()){setDest(tTable.getCurrent().place);}
    for (unsigned i=0;i<processes.size();i++)processes[i]->applyUpdate();
    
}
//------------------------------------------------------------------------------------------------------------
void agent::updateDiseases(){
    for (auto& ds:_diseases){auto& d=ds.second;d.update();if (d.infectious())infected=true;else infected=false;}
    //c++11 for (auto& ds:_diseases){auto& name=ds.first;auto& d=ds.second
    for (auto& [name,d]:_diseases){
        vector<agent*>neighbours=model::getInstance().g.inRadius(this,d.infectionDistance());
        // Loop over other agents
        for (unsigned i=0;i<neighbours.size();i++){
            if (neighbours[i]!=this){
                if (d.infectious() && !neighbours[i]->hasDisease(name) &&  d.infectionOccurs()){neighbours[i]->infectWith(name);numberInfected++;}}
        }
    }
}
//------------------------------------------------------------------------------------------------------------
bool agent::hasDisease(std::string name){
    return (_diseases.find(name)!=_diseases.end());
}
//------------------------------------------------------------------------------------------------------------
bool agent::recoveredFrom(std::string name){
    if (hasDisease(name)) return _diseases[name].recovered();
    return false;
}
//------------------------------------------------------------------------------------------------------------
void agent::infectWith(std::string name){
    _diseases[name].infect();
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
