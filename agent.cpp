#include "agent.h"
#include "disease.h"
//-----------------------------------------------------------------------------------------------------------------
agent::agent(){
    ID=idnum;idnum++;
    _inHospital=false,_critical=false,_died=false;
    numberInfected=0;
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
void agent::setAge(double a){
    age=a;//age in put in years
}
//-----------------------------------------------------------------------------------------------------------------
bool agent::infectious(){
    bool result=false;
    if (!_died && hasDisease("covid"))result=_diseases["covid"].infectious();
    return result;
}
//-----------------------------------------------------------------------------------------------------------------
bool agent::exposed(){
    bool result=false;
    if (!_died && hasDisease("covid") && !recoveredFrom("covid"))result=!(_diseases["covid"].infectious());
    return result;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::preUpdate(){
    if (_died)return;
    age+=parameters::getInstance().timeStep/3600./24./365.;//in years...ignoring leaps! timeStep is in seconds
    for (unsigned i=0;i<processes.size();i++)processes[i]->preUpdate();
}
//-----------------------------------------------------------------------------------------------------------------
void agent::update(){
    if (_died)return;
    updateInfections();
    for (unsigned i=0;i<processes.size();i++)processes[i]->update();
    
}
//-----------------------------------------------------------------------------------------------------------------
void agent::applyUpdate(){
    if (_died)return;
    //allow all infections to complete before updating diseases (so that newly infected agents don't spread within timestep)
    for (auto& ds:_diseases){ds.second.update();}
    //check for hospitalisation
    for (auto& [name,d]:_diseases){
        if (!recoveredFrom(name)){
            if (!_inHospital){
                _inHospital=d.needHospitalisation(age,name);
            }else{
                //if in hospital check for need of critical care
                _critical=d.needCriticalCare(age,name);
            }
            //if critical check for death - this should get the CFR of those in hospital
            if (_critical){
                if (d.criticalFatality(name)) die();
            }
        } else {
            _inHospital=false;
            _critical=false;
        }
    }
    //check in the agent's schedule to see whether it is time for the next activity.
    if (tTable.update()){setDest(tTable.getCurrent().place);}
    for (unsigned i=0;i<processes.size();i++)processes[i]->applyUpdate();
    
}
//------------------------------------------------------------------------------------------------------------
void agent::updateInfections(){
     
    //c++11 needs the loop to look like: for (auto& ds:_diseases){auto& name=ds.first;auto& d=ds.second
    for (auto& [name,d]:_diseases){
        if (!recoveredFrom(name)){
            vector<agent*>neighbours=model::getInstance().g.inRadius(this,d.infectionDistance());
            // Loop over other agents
            for (unsigned i=0;i<neighbours.size();i++){
                if (neighbours[i]!=this){
                    if (d.infectious() && !neighbours[i]->hasDisease(name) &&  d.infectionOccurs()){neighbours[i]->infectWith(name);numberInfected++;}}
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------
void agent::die(){
    _died=true;
    _inHospital=false;
    _critical=false;
}
//------------------------------------------------------------------------------------------------------------
bool agent::dead(){
    return _died;
}
//------------------------------------------------------------------------------------------------------------
bool agent::inHospital(){
    return _inHospital;
}
//------------------------------------------------------------------------------------------------------------
bool agent::critical(){
    return _critical;
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
    _diseases[name]=disease(name);_diseases[name].infect();
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
