#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H
//These classes allow for the creation of agent popuations using a variety of different methods
#include "agent.h"
#include "populationBuilder.h"
#include "places.h"


class agentFactory{
public:
    agentFactory(){;}
    virtual ~agentFactory(){;}
    virtual void createAgents(vector<agent*>&)=0;
};
class worldpopFactory:public agentFactory{
    void createAgents(vector<agent*>& agents){
        //use a file with gridded population data to create agents in given locations
        cout<<"Creating agents ...";
        populationBuilder p;
        p.startCount();
        int k=0;
        int newAgentExists=p.personAtNextLocation();
        
        while (newAgentExists> -1){
            if (newAgentExists==1){
                agent* a=new agent();
                a->knownLocations[places::home]=p.getNextLocation();
                a->knownLocations[places::work]=model::getInstance().g.getRandomPoint();
                a->init();
                model::getInstance().g.add(a);
                agents.push_back(a);
                k++;
                if (k%100000==0)cout<<k<<"...";
            }
            
            newAgentExists=p.personAtNextLocation();
        }
        cout<<endl;
        cout<<"Total agents created:"<<agents.size()<<endl;
    }
};
class simpleRandomFactory:public agentFactory{
    void createAgents(vector<agent*>& agents){
        //simple random allocation anywhere in the model search grid
        int k=0;
        cout<<"Starting simple random agent generator..."<<endl;
        for (int i=0;i<parameters::getInstance().numberOfAgents;i++){
            agent* a=new agent();
            a->knownLocations[places::home]=model::getInstance().g.getRandomPoint();
            a->knownLocations[places::work]=model::getInstance().g.getRandomPoint();
            a->init();
            model::getInstance().g.add(a);
            agents.push_back(a);
            k++;
            if (k%1000==0)cout<<k<<"...";
        }
        cout<<endl;
        cout<<"Total agents created:"<<agents.size()<<endl;
    }
};
class agentFactorySelector{
public:
    static agentFactory&  select(std::string name){
        agentFactory* F=nullptr;
        if (name=="worldpop")F=new worldpopFactory();
        if (name=="simple"  )F=new simpleRandomFactory();
        if (F==nullptr)cout<<"Name "<<name<<" not recognised in agentFactorySelector"<<endl;
        assert(F!=nullptr);
        return *F;
    }
};
#endif
