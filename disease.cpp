#include "disease.h"
#include "model.h"
#include "agent.h"
#include "parameters.h"
#include <omp.h>
#include <math.h>
// constant rate of infection per unit time implies prob. exponentially distributed.
// depending on disease other distributions may be appropriate (e.g. infectiousness may be gamma distributed)
// If infection rate is r, then prob. of
// *not* being infected after finite time dt is exp(-r*dt)
// so rand(0,1)>exp(-rdt) for infection, or -log(rand)<r*dt. NB this decays
// more slowly than rand<r*dt, since there is a finite possibility of not being
// infected when r*dt>1
disease::disease(){
    _infected=false;
    _recovered=false;
    _infectious=false;
    _asymptomatic=false;
    _died=false;
    _timer=0;
    _infectionRate= parameters::getInstance().infectionRate;//per day
    _latencyTime  = parameters::getInstance().latencyTime  ;//in days
    _recoveryTime = parameters::getInstance().recoveryTime ;//in days
    _infectionDistance=parameters::getInstance().infectionDist;//in metres
}
//------------------------------------------------------------------
disease::disease(std::string name){
    _infected=false;
    _recovered=false;
    _infectious=false;
    _asymptomatic=false;
    _died=false;
    _timer=0;
    _infectionRate= parameters::getInstance().disease(name,"infectionRate");//per day
    _latencyTime  = parameters::getInstance().disease(name,"latencyTime"  );//in days
    _recoveryTime = parameters::getInstance().disease(name,"recoveryTime" );//in days
    _infectionDistance=parameters::getInstance().disease(name,"infectionDist");//in metres
    if (model::getInstance().random.number()<parameters::getInstance().disease(name,"asymptomaticRate")/100)_asymptomatic=true;

    if (_asymptomatic)_infectionRate=_infectionRate*parameters::getInstance().disease(name,"asymptomaticInfectionRatio");
}
//------------------------------------------------------------------
void disease::update(){
    if (_died || _recovered)return;
    _timer++;
    maybeBecomeInfectious();
    tryToRecover();
}
//------------------------------------------------------------------
void disease::infect(){_infected=true;_timer=0;}
//------------------------------------------------------------------
bool disease::maybeBecomeInfectious(){
    if (-log(model::getInstance().random.number())<  1./(_latencyTime+0.000001)*parameters::getInstance().timeStep/24./3600.){
        _infectious=true;
        return true;
    }
    return false;
}
//------------------------------------------------------------------
bool disease::tryToRecover(){
    //add small number to _recoveryTime to avoid errors if set to zero!
    if (-log(model::getInstance().random.number())<  1./(_recoveryTime+0.000001)*parameters::getInstance().timeStep/24./3600.)
        {
            _recovered=true;
            _infectious=false;
            return true;
        }
    return false;
}
//------------------------------------------------------------------
bool disease::infectionOccurs(){
    //Imperial report 9 has this gamma distributed.
     return -log(model::getInstance().random.number())< _infectionRate*parameters::getInstance().timeStep/24./3600.;

}
//------------------------------------------------------------------
bool disease::needHospitalisation(double& age,const std::string& name){
    //infectious are assumed displaying symptoms...
    if (_asymptomatic || !_infectious)return false;
    bool result=false;
    auto decade=getDecade(age);
    //assume typical life of disease is recovery time?
    auto hosp=parameters::getInstance().needsCare(name,decade,"hosp")/100;
    auto hospitalisationTimescale=parameters::getInstance().disease(name,"hospitalisationTimescale");
    if (model::getInstance().random.number()<-log(hosp)/(hospitalisationTimescale+0.000001)*parameters::getInstance().timeStep/24/3600)result=true;
    return result;
}
//------------------------------------------------------------------
bool disease::needCriticalCare(double& age,const std::string& name){
    bool result=false;
    auto decade=getDecade(age);
    //chance of going into crit.
    auto crit=parameters::getInstance().needsCare(name,decade,"crit")/100;
    auto timeInHospital=parameters::getInstance().disease(name,"inHospitalTimescale");
    //if time in hospital 8 days then rate would seem to be -ln(crit.)/(8*24*3600)?
    if (-log(model::getInstance().random.number())<-log(crit)/timeInHospital*parameters::getInstance().timeStep/24/3600)result=true;
    return result;
}
//------------------------------------------------------------------
std::string disease::getDecade(double& age){
    //rates are given in bins of 10 years
    int dc=(int(age/10)+1)*10; if (dc>90)dc=90;
    std::string decade=to_string(dc);
    return decade;
}
//------------------------------------------------------------------
bool disease::criticalFatality(const std::string& name){
    bool result=false;
    //chance of dying in crit. (apparently about 50%)
    auto fatal=parameters::getInstance().disease(name,"criticalDeathRate")/100;
    auto timeInCritical=parameters::getInstance().disease(name,"inCriticalCareTimescale");
    //if time in crit it 8 days then rate would seem to be -ln(fatal)/(8*24*3600)?
    if (-log(model::getInstance().random.number())<-log(fatal)/timeInCritical*parameters::getInstance().timeStep/24/3600){
     result=true;
     _infectious=false;//would not be true for ebola...possibly also not for covid?
    }
    return result;
}
//------------------------------------------------------------------
bool disease::infected(){return _infected;}
//------------------------------------------------------------------
bool disease::recovered(){return _recovered;}
//------------------------------------------------------------------
bool disease::infectious(){return _infectious;}
//------------------------------------------------------------------
double disease::infectionDistance(){return _infectionDistance;}
//------------------------------------------------------------------
//Testing section
//------------------------------------------------------------------
void disease::test(){
     cout<<"---Disease process testing---"<<endl;
     cout<<"Check some vales from parameter files"<<endl;
     bool success=true;
     success=success&& (parameters::getInstance().disease("covid","asymptomaticRate")==20);
     success=success&& (parameters::getInstance().disease("covid","criticalDeathRate")==50);
     success=success&& (parameters::getInstance().disease("covid","inHospitalTimescale")==8);
     success=success&& (parameters::getInstance().needsCare("covid","10","crit")==5);
     success=success&& (parameters::getInstance().needsCare("covid","80","hosp")==24.3);
     disease::testMessage("Test 1",success);
     disease d=disease("covid");
     int j=0;
     double t_per_day=1./parameters::getInstance().timeStep*24*3600;
     for (int i=0;i<t_per_day*100;i++)if (d.infectionOccurs())j++;
     cout<<"Expected value  2, got: "<<j/100.<<" "<<endl;
     cout<<"Since 2 is large, values likely to be poor unless timestep<~ day/50"<<endl;
     j=0;
     cout<<d._recoveryTime<<endl;
     for (int i=0;i<t_per_day*10000;i++){if (d.tryToRecover())j++;}
     cout<<"Expected value  19, got: "<<10000./j<<" "<<endl;
}

//------------------------------------------------------------------
void disease::testMessage(std::string s,bool success){
   if (success) cout<<s + " passed" <<endl; else cout<<s +" failed horribly! ARG!"<<endl;
}
//------------------------------------------------------------------
//disease* disease::testDisease=NULL;
