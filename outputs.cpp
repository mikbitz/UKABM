#include "outputs.h"
#include "agent.h"
#include "model.h"
//--------------------------------------------------------------------------
outputs::outputs(){
    //csv file to hold summary of disease information across whole agent set
    _summaryFile.open(parameters::getInstance().summaryFileName);
    //header line
    _summaryFile<<"step,susceptible,infected,recovered,totalPop."<<endl;
    _infections=model::getInstance().g.getAsciiFileWriter(parameters::getInstance().infectionMapFileName);
}
//--------------------------------------------------------------------------
outputs::~outputs(){
    _summaryFile.close();
}
//--------------------------------------------------------------------------
void outputs::writeAll(){
    //summary of all agents across the whole model
    int inf=0,rec=0;
    model& m=model::getInstance();
    for (unsigned i=0;i<m.agentList->size();i++){
        if ((*m.agentList)[i]->infected )inf++;
        if ((*m.agentList)[i]->recovered )rec++;
    }
    _summaryFile<<model::getInstance().tick<<","<<m.agentList->size()-inf-rec<<","<<inf<<","<<rec<<","<<m.agentList->size()<<endl;
    
    //gridded spatial maps of counts of agents with a given property- 
    //argument to g.count can be any function or variable in agent that returns bool.
    //these files are similar to arc ascii grid, with the same global header,
    //but have multiple data arrays with a timestamp line at the start of each
    _infections->writeExtraLabel(to_simple_string(timing::getInstance().now()));
    _infections->writeToFile(m.g.count(&agent::infected));
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void outputs::test(){
    auto a=new asciiGridFileWriter("test",20,20,-10,-10,10,-9999);
    std::vector<std::vector<double>> testData;
    testData.resize(20);
    for (int i=0;i<20;i++){testData[i].resize(20);}
    //constant array - testData[x][y]
    for (auto& o:testData)for(auto& e:o)e=1.;
    a->writeExtraLabel("first");
    a->writeToFile(testData);
    //write a second field to the same file
    a->writeExtraLabel("second");
    //Array constant in x, increasing in y from yllcorner up
    for (auto& o:testData)for(unsigned i=0;i<o.size();i++)o[i]=1.932+i;
    a->writeToFile(testData);
}


