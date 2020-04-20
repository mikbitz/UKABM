#ifndef AGENTS_H
#define AGENTS_H
#include "places.h"
#include "paths.h"
#include "agent.h"
#include "disease.h"
#include "movement.h"
#include <fstream>
#include <omp.h>
/**
\file agents.cpp

\brief This is a layer that acts as a container for agents, 
initialises them and controls their update cycle.

*/
class agents:public layer{
  float dt;
  ofstream f;

public:
    vector<agent*> ags;

//-----------------------------------------------------------------------------------------------------------------
agents(){

    f.open(parameters::getInstance()->outputFileName.c_str());
    setName("diseaseSummary");
    f<<"step"<<" susceptible"<<" infected"<<" recovered"<<" totalPop."<<endl;
    model::getInstance()->ugs=&ags;

    for (int i=0;i<10000000;i++){
     agent* a=new agent();
     a->init();
     model::getInstance()->g.add(a);
     ags.push_back(a);
     if (i==5) {a->addProcess(new disease(a));a->infected=true;}
     a->addProcess(new movement(a));
    }

}
//-----------------------------------------------------------------------------------------------------------------
~agents(){
    f.close();
}
//-----------------------------------------------------------------------------------------------------------------
bool update(){

    #pragma omp parallel num_threads(omp_get_max_threads()) 
    {

//        #pragma omp for schedule(dynamic)
        for (unsigned i=0;i<ags.size();i++){
            ags[i]->preUpdate();
        }
        #pragma omp for schedule(dynamic)
        for (unsigned i=0;i<ags.size();i+=1){
            ags[i]->update();
        }

//        #pragma omp for schedule(dynamic)
        for (unsigned i=0;i<ags.size();i+=1){
            ags[i]->applyUpdate();
        }
    }//END PARALLEL REGION
   if(model::getInstance()->tick % parameters::getInstance()->outputInterval ==0){
    int inf=0,rec=0;
    for (unsigned i=0;i<ags.size();i+=1){
        if (ags[i]->infected )inf++;
        if (ags[i]->recovered )rec++;
    }
    f<<model::getInstance()->tick<<" "<<ags.size()-inf-rec<<" "<<inf<<" "<<rec<<" "<<ags.size()<<endl;
   }
   model::getInstance()->g.check(ags);
 return true;
}
};
unsigned agent::idnum=0;

#endif
