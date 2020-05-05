#include "model.h"
#include "parameters.h"
#include "timing.h"
#include "config.h"
#include "movement.h"
#include <boost/filesystem.hpp>
#include <functional>

/**
\file model.cpp

\brief This is the model implementation

*/

//-----------------------------------------------------------------------------------------------------------------
model* model::instance=NULL;
//-----------------------------------------------------------------------------------------------------------------
void model::clean(){if (instance!=NULL) {delete instance;instance=NULL;}}
//-----------------------------------------------------------------------------------------------------------------
model::~model(){clean();}
//-----------------------------------------------------------------------------------------------------------------
model& model::getInstance(){
 if (instance==NULL){
   cout<<"A new model was set up"<<endl;
   instance=new model();
 }
 return *instance;
}
//-----------------------------------------------------------------------------------------------------------------
model::model(){

    tick=0;
    parameters& p=parameters::getInstance();
    dt=p.timeStep;
    g.init();
    //don't initialise outputs here - leads to infinite initialisation loop...

}
//-----------------------------------------------------------------------------------------------------------------
void model::init(){
    
    //searchGrid::test();
    //disease::test();
    //exit(0);
    places& p=places::getInstance();
    p.init();
    
    c=new configuration();

    //-----------------
    //naming convention for output files
    _filePrefix=   parameters::getInstance().experimentOutputDirectory+
    "/experiment."+parameters::getInstance().experimentName;
    if (!boost::filesystem::exists(_filePrefix))boost::filesystem::create_directories(_filePrefix);
    std::string runNumber= parameters::getInstance().runNumber;
    std::string m00="/run_";
    if (runNumber!=""){
        m00=m00+runNumber;
    }else{
        //auto-increment run number if run.number is not set
        int i=0;
        m00="/run_0000";
        while(boost::filesystem::exists(_filePrefix+m00)){    // Find a new directory name
            i++;
            std::stringstream ss;
            int zeroIndex=1000;while(zeroIndex>i && zeroIndex>1){ss<<"0";zeroIndex=zeroIndex/10;}
            ss<<i;
            runNumber=ss.str();
            m00="/run_"+runNumber;
        }
    }
    if (!boost::filesystem::exists(_filePrefix+m00))boost::filesystem::create_directories(_filePrefix+m00);
    parameters::getInstance().setRunNumber(runNumber);
    _filePrefix= _filePrefix+m00+"/";
    _filePostfix="";
    cout<<"Outputfiles will be named "<<_filePrefix<<"<Data Name>"<<_filePostfix<<".<filenameExtension>"<<endl;
    //-----------------
    files=new outputs();
    parameters::getInstance().saveParameters();
}
//-----------------------------------------------------------------------------------------------------------------
string model::getText(){
    return text;
}
//-----------------------------------------------------------------------------------------------------------------
void model::add(layer *l){
   layers.push_back(l);
}
//-----------------------------------------------------------------------------------------------------------------
layer* model::getLayer(unsigned i){
   if (i<layers.size())return layers[i];
   return NULL;
}
//-----------------------------------------------------------------------------------------------------------------
bool model::update(){
    //cout<<"------------------- step "<<tick<<" ---------------------"<<endl;        
    //auto start=chrono::steady_clock::now();
    if (tick % parameters::getInstance().outputInterval ==0) files->writeAll();
    //auto end=chrono::steady_clock::now();
    //cout<<"timew "<<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<endl;

    bool b=false;
    for (unsigned i=0;i<layers.size();i++){
      bool u=layers[i]->update();
      b=b || u;
    }
    if (tick%10==0){cout<<timing::getInstance().now()<<endl;}
    double R0=0,infs=0;
    for (auto a:(*agentList)){if (a->recoveredFrom("covid") && a->numberInfected>0){infs++; R0+=a->numberInfected;}}
    if (infs>0 && tick%10==0)cout<<"R0?? :"<<R0/infs<<endl;
    if (b){
        tick++;
        timing::getInstance().update();
    }

    return b;
}
//-----------------------------------------------------------------------------------------------------------------
double model::getSize(){
    double sz=0;
    for (unsigned i=0;i<layers.size();i++){
      if (layers[i]->getSize()>sz)sz=layers[i]->getSize();
    }
    return sz;
}
//-----------------------------------------------------------------------------------------------------------------
void model::finish(){
std::cout<<_filePrefix<<" Complete"<<endl;
}

