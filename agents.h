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

        for (unsigned i=0;i<ags.size();i++){
            ags[i]->preUpdate();
        }
        //currently doesn't really seem to scale here in parallel...
        //#pragma omp parallel num_threads(omp_get_max_threads())
        //{
        //#pragma omp for schedule(dynamic)
        for (unsigned i=0;i<ags.size();i+=1){
            ags[i]->update();
        }
        //}

        for (unsigned i=0;i<ags.size();i+=1){
            ags[i]->applyUpdate();
        }

   
   //make sure agents are properly in the search grid.
   model::getInstance().g.check(ags);

 return true;
}
};
unsigned agent::idnum=0;

#endif
