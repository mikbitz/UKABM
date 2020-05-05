#include "parameters.h"
#include "model.h"
#include "readcsv.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "boost/date_time/posix_time/posix_time.hpp"
//-----------------------------------------------------------------------------------------------------------------
parameters* parameters::instance;
//-----------------------------------------------------------------------------------------------------------------
parameters::~parameters(){
if (instance!=NULL)delete instance;
}
//-----------------------------------------------------------------------------------------------------------------
parameters& parameters::getInstance(){
 if (instance==NULL){
   instance=new parameters();
 }
 return *instance;
}
//-----------------------------------------------------------------------------------------------------------------
parameters& parameters::getInstance(string f){
 if (instance==NULL){
   instance=new parameters(f);
 }
 return *instance;
}
//-----------------------------------------------------------------------------------------------------------------
parameters::parameters(){
cout<<"The default model parameters were put in place"<<endl;
randomSeed=10;
timeStep=0.2;
nsteps=35000;
summaryFileName="defaultOutput";
parameterFile="defaultParameters";
initialDate="2020-Jan-10";
finalDate="2020-Jan-11";
runNumber=""; 
experimentOutputDirectory=".";
experimentName="";
}
//-----------------------------------------------------------------------------------------------------------------
parameters::parameters(string f):parameterFile(f){
getParameters();
}
//--------------------------------------------------------------------------------------------
void parameters::printParameters(){
std::cout<< "Input Parameter File: "<<parameterFile      <<std::endl;
std::cout<< "----------------------------- "            <<std::endl;
std::cout<< "Successfully found parameters "            <<std::endl;
for (auto& [label,value]:named){
    std::string name=label;
    while(name.length()<30)name+=" ";
    cout<<name<<"  "<<value.str()<<endl;
}
std::cout<< "----------------------------- "            <<std::endl;
}
//--------------------------------------------------------------------------------------------
void parameters::saveParameters(){
std::fstream f;
f.open(model::getInstance().filepath()+"RunParameters",ios::out);
f<<"Run started at: "<<boost::posix_time::second_clock::local_time()<<endl;
f<< "----------------------------- "            <<std::endl;
f<< "Input Parameter File: "<<parameterFile      <<std::endl;
f<< "----------------------------- "            <<std::endl;
for (auto& [label,value]:named){
    std::string name=label;
    while(name.length()<30)name+=" ";
    f<<name<<"  "<<value.str()<<endl;
}
f<< "----------------------------- "            <<std::endl;
}
//--------------------------------------------------------------------------------------------
void parameters::getParameters(){
    
    std::cout<<"Hoping to use values from parameter file "<<parameterFile<<std::endl;
    try{
    std::fstream f;
    //f.exceptions ( ifstream::failbit | ifstream::badbit );
    f.open(parameterFile,ios::in);
    std::string label;
    
    while (!f.eof()){
         std::string label;
        f>>label;
        //set # as comment character
        if (label[ 0 ] != '#'){
            bool success=false;

            if (label=="timing.timeStep:")             {f>> timeStep;                  named[label]<<timeStep;                  success=true;}
            if (label=="timing.nsteps:")               {f>> nsteps;                    named[label]<<nsteps;                    success=true;}
            if (label=="timing.initialDate:")          {f>> initialDate;               named[label]<<initialDate;               success=true;}     
            if (label=="timing.finalDate:")            {f>> finalDate;                 named[label]<<finalDate;                 success=true;}             
            if (label=="experiment.name:")             {f>> experimentName;            named[label]<<experimentName;            success=true;}
            if (label=="experiment.output.directory:") {f>> experimentOutputDirectory; named[label]<<experimentOutputDirectory; success=true;}
            if (label=="experiment.description:")      {f>> experimentDescription;     named[label]<<experimentDescription;     success=true;}
            if (label=="experiment.run.number:")       {f>> runNumber;                 named[label]<<runNumber;                 success=true;}
            if (label=="output.summaryFileName:")      {f>> summaryFileName;           named[label]<<summaryFileName;           success=true;}  
            if (label=="output.infectionMapFileName:") {f>> infectionMapFileName;      named[label]<<infectionMapFileName;      success=true;}  
            if (label=="output.populationMapFileName:"){f>> populationMapFileName;     named[label]<<populationMapFileName;     success=true;}  
            if (label=="output.outputInterval:")       {f>> outputInterval;            named[label]<<outputInterval;            success=true;}
            if (label=="restart.isRestart:")           {f>> isRestart;                 named[label]<<isRestart;                 success=true;}       
            if (label=="restart.restartInterval:")     {f>> restartInterval;           named[label]<<restartInterval;           success=true;} 
            if (label=="restart.restartFile:")         {f>> restartFileName;           named[label]<< restartFileName;          success=true;}     
            if (label=="disease.recoveryTime:")        {f>> recoveryTime;              named[label]<<recoveryTime;              success=true;}
            if (label=="disease.latencyTime:")         {f>> latencyTime;               named[label]<<latencyTime;               success=true;}
            if (label=="disease.infectionRate:")       {f>> infectionRate;             named[label]<<infectionRate;             success=true;}   
            if (label=="disease.infectionDist:")       {f>> infectionDist;             named[label]<<infectionDist;             success=true;}   
            if (label=="random.Seed:")                 {f>> randomSeed;                named[label]<<randomSeed;                success=true;}      
            if (label=="grid.xBins:")                  {f>> NxCells;                   named[label]<<NxCells;                   success=true;}         
            if (label=="grid.yBins:")                  {f>> NyCells;                   named[label]<<NyCells;                   success=true;}         
            if (label=="grid.xSize:")                  {f>> xSize;                     named[label]<<xSize;                     success=true;}           
            if (label=="grid.ySize:")                  {f>> ySize;                     named[label]<<xSize;                     success=true;}           
            if (label=="grid.xOrigin:")                {f>> x0;                        named[label]<<x0;                        success=true;}              
            if (label=="grid.yOrigin:")                {f>> y0;                        named[label]<<y0;                        success=true;}
            if (label=="agents.populationGridFile:")   {f>>populationGridFile;         named[label]<<populationGridFile;        success=true;}
            if (label=="agents.agentFactoryType:")     {f>>agentFactoryType;           named[label]<<agentFactoryType;          success=true;}
            if (label=="agents.numberOfAgents:")       {f>>numberOfAgents;             named[label]<<numberOfAgents;            success=true;}
            if (label=="agents.agentFraction:")        {f>>agentFraction;              named[label]<<agentFraction;             success=true;}
            if (label=="disease.ParameterFile:")       {f>>diseaseParameterFile;       named[label]<<diseaseParameterFile;      success=true;}
            if (label=="disease.HospitalRates:")       {f>>diseaseHospitalRates;       named[label]<<diseaseHospitalRates;      success=true;}
            if (label=="placeTypeFile:")               {f>>placeTypeFile;              named[label]<<placeTypeFile;             success=true;}
            if (label=="placeFile:")                   {f>>placeFile;                  named[label]<<placeFile;                 success=true;}


            
            if (!f.eof() && !success){
                std::cout<<"Unrecognised label "<<label<<" in file "<<parameterFile<<" exiting..."<<std::endl;
                exit(1); 
            }
        }else{
            //ignore all text on lines beginning with #
            f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    f.close();
    printParameters();

    } catch (fstream::failure err){
      std::cout<<"IO error while reading data file "<<parameterFile<<std::endl;
      exit(1);
    }
    readDiseaseParameters();
}
//--------------------------------------------------------------------------------------------
void parameters::readDiseaseParameters(){
    //read in disease parameter files
    readcsv dpars(diseaseParameterFile);
    //expect disease name as first item on each line
    cout<<"found disease "<<dpars[0][0]<<endl;
    auto& header=dpars.getHeader();
    
    //header holds the name for each disease parameter (header[0] is just "name")
    for (unsigned i=1;i<dpars[0].size();i++)_diseaseParameters[dpars[0][0]][header[i]]=std::stod(dpars[0][i]);
    
    readcsv dHosp(diseaseHospitalRates);
    header=dHosp.getHeader();
    cout<<"found hospital rates for "<<header[0]<<endl;
    for (unsigned row=1;row<dHosp.nrows();row++){
        for (unsigned col=1;col<dHosp[0].size();col++){
        std::string age=dHosp[0][col];
        std::string value=dHosp[row][0];
        _hospitalParameters[header[0]][age][value]=std::stod(dHosp[row][col]);
        }
    }
    
}
//--------------------------------------------------------------------------------------------
double parameters::disease(const std::string& name,const std::string& parameter){
    return _diseaseParameters[name][parameter];
}
//--------------------------------------------------------------------------------------------
double parameters::needsCare(const std::string& name,const std::string& age,const std::string& value){
    return _hospitalParameters[name][age][value];
}
