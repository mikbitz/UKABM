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
@file agents.h

@class agents

@brief This is a layer that acts as a container for agents, 
initialises them and controls their update cycle.

*/
class agents:public layer{
    /**
     * @brief If true then output the time taken in each step of updating the layer
     * 
     */
    bool _timingOutput;
public:
    /**
     * @brief A vector that has pointers to all agents in the layer
     * 
     */
    vector<agent*> ags;

//-----------------------------------------------------------------------------------------------------------------
agents(){
    _timingOutput=true;
    setName("agents");
    model::getInstance().agentList=&ags;
    cout<<"Building agents...";
    auto start=chrono::steady_clock::now();
    agentFactory& F=agentFactorySelector::select(parameters::getInstance().agentFactoryType);
    F.createAgents(ags);
    auto end=chrono::steady_clock::now();
    if (_timingOutput)cout<<"time0: agent creation "<<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<endl;

    //infect 10 agentsin random spatial locations
    shuffle (ags.begin(), ags.end(), std::default_random_engine(parameters::getInstance().randomSeed));
    for (unsigned i=0;i<10;i++)ags[10000+i]->infectWith("covid");
    //agent movement process
    //for (auto a:ags)a->addProcess(new movement(a));
}
//-----------------------------------------------------------------------------------------------------------------
~agents(){
;
}
//-----------------------------------------------------------------------------------------------------------------
bool update(){
        auto start=chrono::steady_clock::now();
       
        for (unsigned i=0;i<ags.size();i++){
            ags[i]->preUpdate();
        }
        auto end1=chrono::steady_clock::now();
        if (_timingOutput)cout<<"time1: agent PreUpdate "<<chrono::duration_cast<chrono::milliseconds>(end1-start).count()<<endl;
        //is parallel here thread safe?
        #pragma omp parallel num_threads(omp_get_max_threads())
        {
        #pragma omp for schedule(dynamic)
        for (unsigned i=0;i<ags.size();i+=1){
            ags[i]->update();
        }
        }
        auto end2=chrono::steady_clock::now();
        if (_timingOutput)cout<<"time2: agent Update "<<chrono::duration_cast<chrono::milliseconds>(end2-end1).count()<<endl;
        #pragma omp parallel num_threads(omp_get_max_threads())
        {
        #pragma omp for schedule(dynamic)
        for (unsigned i=0;i<ags.size();i+=1){
            ags[i]->applyUpdate();
        }
        }
        auto end3=chrono::steady_clock::now();
        if (_timingOutput)cout<<"time3: agent applyUpdate "<<chrono::duration_cast<chrono::milliseconds>(end3-end2).count()<<endl;
   
   //make sure agents are properly in the search grid. - very expensive!
   //model::getInstance().g.check(ags);
   //calling this on the whole grid turns out to be similar speed...this version llelizes somewhat
    model::getInstance().g.check();    

   auto end4=chrono::steady_clock::now();
   if (_timingOutput)cout<<"time4: check gridding  "<<chrono::duration_cast<chrono::milliseconds>(end4-end3).count()<<endl;

 return true;
}
//-----------------------------------------------------------------------------------------------------------------
void output(){
       // for (unsigned i=0;i<ags.size();i++){
    int i=20;
    //while (ags[i]->age()>16){cout<<i<<endl;i++;}
         cout<<ags[i]->X()<<" "<<ags[i]->Y()<<" "<<ags[i]->age()<<" "<<ags[i]->mother()->hasWork()<<" "<<ags[i]->father()->hasWork()<<endl;//<<" "<<ags[i]->children().size()<<" "<<ags[i]->hasWork()<<endl;
         for (auto& t:ags[i]->mother()->children())cout<<t->age()<<" "<<t->X()<<" "<<t->Y()<<endl;
         cout<<ags[i]->mother()->X()<<" "<<ags[i]->mother()->Y()<<" "<<ags[i]->father()->X()<< " "<<ags[i]->father()->Y()<<endl;
       // }

}
};

#endif
