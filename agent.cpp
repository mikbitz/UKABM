#include "agent.h"
#include "disease.h"
unsigned agent::idnum=0;
//-----------------------------------------------------------------------------------------------------------------
agent::agent(){
    ID=idnum;idnum++;
    _inHospital=false,_critical=false,_died=false;
    numberInfected=0;
    _age=0;
    _sex='f';
    cellIndex=-1;
    _jobType=std::numeric_limits<unsigned>::max();//default for no job
    _workStatus=unemployed;
    _educationStatus=secondary;//UK adults should all have at least this?
    _partner=nullptr;
    _mother=nullptr;
    _father=nullptr;
    _size=1.;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::init(){
    
    loc =knownLocations[places::getInstance()["home"]];//the actual co-ordinates of this type of place
    oldPlace=places::getInstance()["home"];//an integer that indicates the type of place - used in updating movement between locations
    dest=knownLocations[places::getInstance()["home"]];
    newPlace=places::getInstance()["home"];
    _pathSet.setKnownPaths();//paths between types of places
    
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setAge(double a){
    _age=a;//age input in years
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setSex(const char& s){
    assert(s=='m' || s=='f');
    _sex=s;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setWorkPlace(place* p){
    _workPlace=p;
    knownLocations[places::getInstance()["work"]]=p->getLocation();
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setJobType(const unsigned& j ){
    _jobType=j;
    _workStatus=working;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setWorkStatus(const std::string& s){
    if (s=="retired")    _workStatus=retiree;
    if (s=="ineducation")_workStatus=ineducation;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setEducationStatus(const std::string& s){
    if (s=="preschool")     _educationStatus=preschool;
    if (s=="primary")       _educationStatus=primary;
    if (s=="uppersecondary")_educationStatus=uppersecondary;
    if (s=="secondary")     _educationStatus=secondary;
    if (s=="higher")        _educationStatus=higher;
    if (s=="postgrad")      _educationStatus=postgrad;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::makePartner(agent* a){
    _partner=a;
    a->_partner=this;
    //a moves in with partner
    a->knownLocations[places::getInstance()["home"]]=knownLocations[places::getInstance()["home"]];
    a->loc=loc;a->dest=a->loc;a->oldPlace=oldPlace;a->newPlace=oldPlace;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::makeParents(agent* a){
    _mother=a;//input assumed female at present
    a->addChild(this);
    if (a->partner()!=nullptr){_father=a->partner();a->partner()->addChild(this);}
    //child moves in with mother
    if (age()<18){
        knownLocations[places::getInstance()["home"]]=a->knownLocations[places::getInstance()["home"]];
        loc=a->loc;dest=loc;oldPlace=a->oldPlace;newPlace=oldPlace;
    }
}
//-----------------------------------------------------------------------------------------------------------------
void agent::addChild(agent* a){
    _children.push_back(a);
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
    for (unsigned i=0;i<processes.size();i++)processes[i]->preUpdate();
    _age+=parameters::getInstance().timeStep/3600./24./365.;//in years...ignoring leaps! timeStep is in seconds

}
//-----------------------------------------------------------------------------------------------------------------
void agent::update(){
    if (_died)return;
    for (unsigned i=0;i<processes.size();i++)processes[i]->update();
    updateInfections();
    
}
//-----------------------------------------------------------------------------------------------------------------
void agent::applyUpdate(){
    if (_died)return;
    for (unsigned i=0;i<processes.size();i++)processes[i]->applyUpdate();
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
    //be careful here not to accidentally insert "name"
    if (hasDisease(name)) return _diseases[name].recovered();
    return false;
}
//------------------------------------------------------------------------------------------------------------
void agent::infectWith(std::string name){
    _diseases[name]=disease(name);
    _diseases[name].infect();
}
//-----------------------------------------------------------------------------------------------------------------
void agent::setDest(unsigned e){
    //cout<<"set Dest "<<newPlace<<" "<< oldPlace<<" "<<e<<endl;
    oldPlace=newPlace;
    newPlace=e;
    assert(oldPlace != newPlace);
    currentPath=_pathSet.paths[oldPlace][newPlace];
    pathState=1;
    dest=knownLocations[currentPath.getStep(pathState)];
    loc=dest;
}
//-----------------------------------------------------------------------------------------------------------------
void agent::addProcess(process* p){
    processes.push_back(p); 
}
//-----------------------------------------------------------------------------------------------------------------
double agent::X(){
    return loc.X(); 
}
//-----------------------------------------------------------------------------------------------------------------
double agent::Y(){
    return loc.Y(); 
}
//-----------------------------------------------------------------------------------------------------------------
bool agent::hasWork(){
    return _workStatus==working; 
}
//-----------------------------------------------------------------------------------------------------------------
bool agent::worker(){
    return (_workStatus==unemployed || _workStatus==working); 
}
//-----------------------------------------------------------------------------------------------------------------
bool agent::retired(){
    return (_workStatus==retiree); 
}//-----------------------------------------------------------------------------------------------------------------
bool agent::inEducation(){
    return (_workStatus==ineducation); 
}
//-----------------------------------------------------------------------------------------------------------------
char agent::sex(){
    return _sex;
}
//-----------------------------------------------------------------------------------------------------------------

double agent::age(){
    return _age;
}
//-----------------------------------------------------------------------------------------------------------------
agent* agent::partner(){
    return _partner;
}
//-----------------------------------------------------------------------------------------------------------------
agent* agent::mother(){
    return _mother;
}
//-----------------------------------------------------------------------------------------------------------------
agent* agent::father(){
    return _father;
}
//-----------------------------------------------------------------------------------------------------------------
vector<agent*>& agent::children(){
    return _children;
}
