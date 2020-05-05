#ifndef AGENTS_H
#define AGENTS_H
#include "places.h"
#include "paths.h"
#include "agent.h"
#include "disease.h"
#include "movement.h"
#include <fstream>
#include <omp.h>
#include "agentFactory.h"
/**
\file agents.cpp

\brief This is a layer that acts as a container for agents, 
initialises them and controls their update cycle.

*/
class agents:public layer{
  float dt;

public:
    vector<agent*> ags;

//-----------------------------------------------------------------------------------------------------------------
agents(){
    setName("agents");
    model::getInstance().agentList=&ags;
    cout<<"Building agents...";
    agentFactory& F=agentFactorySelector::select(parameters::getInstance().agentFactoryType);
    F.createAgents(ags);
    
    //infect 10 agentsin random spatial locations
    shuffle (ags.begin(), ags.end(), std::default_random_engine(parameters::getInstance().randomSeed));
    for (unsigned i=0;i<10;i++)ags[10000+i]->infectWith("covid");

    for (auto a:ags)a->addProcess(new movement(a));
}
//-----------------------------------------------------------------------------------------------------------------
~agents(){
;
}
//-----------------------------------------------------------------------------------------------------------------
bool update(){
        //auto start=chrono::steady_clock::now();
        for (unsigned i=0;i<ags.size();i++){
            ags[i]->preUpdate();
        }
        //auto end1=chrono::steady_clock::now();
        //cout<<"time1 "<<chrono::duration_cast<chrono::milliseconds>(end1-start).count()<<endl;
        //is parallel here thread safe?
        #pragma omp parallel num_threads(omp_get_max_threads())
        {
        #pragma omp for schedule(dynamic)
        for (unsigned i=0;i<ags.size();i+=1){
            ags[i]->update();
        }
        }
        //auto end2=chrono::steady_clock::now();
        //cout<<"time2 "<<chrono::duration_cast<chrono::milliseconds>(end2-end1).count()<<endl;
        #pragma omp parallel num_threads(omp_get_max_threads())
        {
        #pragma omp for schedule(dynamic)
        for (unsigned i=0;i<ags.size();i+=1){
            ags[i]->applyUpdate();
        }
        }
        //auto end3=chrono::steady_clock::now();
        //cout<<"time3 "<<chrono::duration_cast<chrono::milliseconds>(end3-end2).count()<<endl;
   
   //make sure agents are properly in the search grid. - very expensive!
   //model::getInstance().g.check(ags);
   //calling this on the whole grid turns out to be similar speed...this version llelizes somewhat
    model::getInstance().g.check();    

   //auto end4=chrono::steady_clock::now();
   //cout<<"time4 "<<chrono::duration_cast<chrono::milliseconds>(end4-end3).count()<<endl;

 return true;
}
};
unsigned agent::idnum=0;

#endif
