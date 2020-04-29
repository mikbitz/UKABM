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
        //population builder will resize the model grid to fit.
        cout<<"Creating agents ...";
        populationBuilder p;
        p.startCount();
        int k=0;
        int newAgentExists=p.personAtNextLocation();
        
        while (newAgentExists> -1){
            if (newAgentExists==1){
                agent* a=new agent();
                //simple uniform age distrubtion
                a->setAge(model::getInstance().random.number()*80);
//                 if(age<16)schoolchild()//under fives?
//                 if(age>65)retired()//60 for women? in care?
//                 if (age>16)sixthform()//all? apprentices?
//                 if (age>18)university()//50%?
//                 if (!inEducation && !retired) working()//includes unemployed/not participating in work - disabilities?
//                 findParents()
//                 if (age>16)findPartner()//allow for single occupation - also homeless?
//                 if (age<16)moveToHome()//percent in care?percent single families?
//                 if (inWork){allocateWorkType();getWorkLocation()}
                a->knownLocations[places::home]=p.getNextLocation();
                //distance travelled distributed approximately like 1./x^2 for x>=1;
                //distances between 1km and 100km
                double d=1010 / (1.01 - model::getInstance().random.number());
                double pi=acos(-1.);
                //random direction
                double dcc=d*cos(2*pi*model::getInstance().random.number());
                double dcs=d*sin(2*pi*model::getInstance().random.number());
                a->knownLocations[places::work]=a->knownLocations[places::home]+point2D(dcc,dcs);
                a->init();
                if (model::getInstance().random.number()<0.5)a->setSex('m');//defaults to f
                model::getInstance().g.add(a);
                agents.push_back(a);
                k++;
                if (k%100000==0)cout<<k<<"...";
            }
            
            newAgentExists=p.personAtNextLocation();
        }
        cout<<endl;
        cout<<"Total agents created:"<<agents.size()<<endl;
        //set age??
    }
};
class simpleRandomFactory:public agentFactory{
    void createAgents(vector<agent*>& agents){
        //simple random allocation anywhere in the model search grid
        int k=0;
        cout<<"Starting simple random agent generator..."<<endl;
        for (int i=0;i<parameters::getInstance().numberOfAgents;i++){
            agent* a=new agent();
            a->setAge(model::getInstance().random.number()*80);
            a->knownLocations[places::home]=model::getInstance().g.getRandomPoint();
            a->knownLocations[places::work]=model::getInstance().g.getRandomPoint();
            a->init();
            if (model::getInstance().random.number()<0.5)a->setSex('m');//defaults to f
            model::getInstance().g.add(a);
            agents.push_back(a);
            k++;
            if (k%1000==0)cout<<k<<"...";
        }
        cout<<endl;
        cout<<"Total agents created:"<<agents.size()<<endl;
        //set age??
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
