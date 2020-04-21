#include "agent.h"
#include "populationBuilder.h"
class agentFactory{
public:
    agentFactory(){;}
    virtual ~agentFactory(){;}
    virtual void createAgents(vector<agent*>&)=0;
};
class worldpopFactory:public agentFactory{
void createAgents(vector<agent*>& agents){
    cout<<"Creating agents ...";
    populationBuilder p;
    p.startCount();
    int k=0;
    int newAgentExists=p.personAtNextLocation();

    while (newAgentExists> -1){
        if (newAgentExists==1){
        agent* a=new agent();
        p.getNextLocation();
        model::getInstance()->g.add(a);
        agents.push_back(a);
        k++;
        if (k%100000==0)cout<<k<<"...";
        }

        newAgentExists=p.personAtNextLocation();
    }
    cout<<endl;
}
};
class simpleRandomFactory:public agentFactory{
void createAgents(vector<agent*>& agents){
    //simple random
    int k=0;
    cout<<"Starting simple random agent generator..."<<endl;
    for (int i=0;i<10000000;i++){
     agent* a=new agent();
     a->init();
     model::getInstance()->g.add(a);
     agents.push_back(a);
     if (k%100000==0)cout<<k<<"...";
    }
    cout<<endl;
}
};
