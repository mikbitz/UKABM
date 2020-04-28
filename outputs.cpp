#include "outputs.h"
#include "agent.h"
#include "model.h"
//--------------------------------------------------------------------------
outputs::outputs(){
    parameters& p=parameters::getInstance();
    std::string path=model::getInstance().filepath();
    //csv file to hold summary of disease information across whole agent set
    _summaryFile.open(path+parameters::getInstance().summaryFileName);
    //header line
    _summaryFile<<"step,date,susceptible,exposed,infectious,inhospital,critical,recovered,died,totalPop."<<endl;
    //Note cellsize here *must* match those in writeALL below
    _outputCellSize=10000;

    std::string nFrames="maxFrames "+std::to_string(p.nsteps/p.outputInterval);


    _infections=model::getInstance().g.getAsciiFileWriter(path,parameters::getInstance().infectionMapFileName,_outputCellSize,-9999);
    _infections->writeExtraLabel(nFrames);
    _population=model::getInstance().g.getAsciiFileWriter(path,parameters::getInstance().populationMapFileName,_outputCellSize,-9999);
    _population->writeExtraLabel(nFrames);
}
//--------------------------------------------------------------------------
outputs::~outputs(){
    _summaryFile.close();
}
//--------------------------------------------------------------------------
void outputs::writeAll(){
    //summary of all agents across the whole model
    int inf=0,rec=0,exp=0,sus=0,died=0,inhospital=0,critical=0;
    model& m=model::getInstance();
    for (unsigned i=0;i<m.agentList->size();i++){
        agent* a=(*m.agentList)[i];
        if (a->dead())died++;
        else{
            if (a->exposed())exp++;
            if (a->infectious())inf++;
            if (a->inHospital())inhospital++;
            if (a->critical())critical++;
            if (!a->hasDisease("covid"))sus++;
            if (a->recoveredFrom("covid") )rec++;
            }
        }
    
    _summaryFile<<model::getInstance().tick<<","<<timing::getInstance().now()<<","<<sus<<","<<exp<<","<<inf<<","<<inhospital<<","<<critical<<","<<rec<<","<<died<<","<<m.agentList->size()-died<<endl;
    
    //gridded spatial maps of counts of agents with a given property- 
    //argument to g.count can be any function or variable in agent that returns bool.
    //these files are similar to arc ascii grid, with the same global header,
    //but have multiple data arrays with a timestamp line at the start of each
    _infections->writeExtraLabel(to_simple_string(timing::getInstance().now()));
    _infections->writeToFile(m.g.count(&agent::infectious,_outputCellSize));
    
    _population->writeExtraLabel(to_simple_string(timing::getInstance().now()));
    _population->writeToFile(m.g.count(_outputCellSize));
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void outputs::test(){
    auto a=new asciiGridFileWriter("./","test",20,20,-10,-10,10,-9999);
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


