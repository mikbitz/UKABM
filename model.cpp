#include "model.h"
#include "parameters.h"
#include "timing.h"
#include "config.h"
#include "movement.h"
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
  places* p=places::getInstance();
  p->init();

  c=new configuration();
  files=new outputs();
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
    bool b=false;
    for (unsigned i=0;i<layers.size();i++){
      bool u=layers[i]->update();
      b=b || u;
    }
    if (tick%10==0){cout<<timing::getInstance().now()<<endl;}

    if (b){
        if (tick % parameters::getInstance().outputInterval ==0) files->writeAll();
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
  
}

