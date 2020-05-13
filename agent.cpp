#include "agent.h"
#include "disease.h"
//-----------------------------------------------------------------------------------------------------------------
agent::agent(){
    ID=idnum;idnum++;
    _inHospital=false,_critical=false,_died=false;
    numberInfected=0;
    _sex='f';
    cellIndex=-1;
    _jobType=std::numeric_limits<unsigned>::max();
    _workStatus=unemployed;
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
void agent::setAge(double a){
    _age=a;//age input in years
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setSex(const char& s){
    _sex=s;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setWork(place* p){
    _workPlace=p;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setJobType(const unsigned& j ){
    _jobType=j;
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
    _age+=parameters::getInstance().timeStep/3600./24./365.;//in years...ignoring leaps! timeStep is in seconds
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
                _inHospital=d.needHospitalisation(_age,name);
            }else{
                //if in hospital check for need of critical care
                _critical=d.needCriticalCare(_age,name);
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
void agent::setDest(places::placeE e){
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
//-----------------------------------------------------------------------------------------------------------------
double agent::X(){
    return loc.x; 
}
//-----------------------------------------------------------------------------------------------------------------
double agent::Y(){
    return loc.y; 
}
//-----------------------------------------------------------------------------------------------------------------
bool agent::hasWork(){
    return _jobType!=std::numeric_limits<unsigned>::max(); 
}
//-----------------------------------------------------------------------------------------------------------------
bool agent::worker(){
    return (_workStatus==unemployed || _workStatus==working); 
}
