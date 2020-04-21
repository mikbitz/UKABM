#include "parameters.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
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
visible=false;
randomSeed=10;
timeStep=0.2;
nsteps=35000;
outputFileName="defaultOutput";
parameterFile="defaultParameters";
initialDate="2020-Jan-10";
finalDate="2020-Jan-11";
}
//-----------------------------------------------------------------------------------------------------------------
parameters::parameters(string f):parameterFile(f){
getParameters();
}
//--------------------------------------------------------------------------------------------
void parameters::printParameters(){
std::cout<< "Input Parameter File "<<parameterFile      <<std::endl;
std::cout<< "----------------------------- "            <<std::endl;
std::cout<< "Successfully found parameters "            <<std::endl;
std::cout<< "timeStep              "<< timeStep         <<std::endl;
std::cout<< "number of timesteps   "<< nsteps           <<std::endl;
std::cout<< "outputInterval        "<< outputInterval   <<std::endl;
std::cout<< "outputFileName        "<< outputFileName   <<std::endl;
std::cout<< "diseaseLocationFileName        "<< diseaseLocationFileName  <<std::endl;
std::cout<< "recoveryLocationFileName        "<< recoveryLocationFileName  <<std::endl;
std::cout<< "isRestart             "<< isRestart        <<std::endl;
std::cout<< "restartInterval       "<< restartInterval  <<std::endl;
std::cout<< "restartFileName       "<< restartFileName  <<std::endl;
std::cout<< "inputFileName         "<< inputFileName    <<std::endl;
std::cout<< "initialDate           "<< initialDate      <<std::endl;
std::cout<< "finalDate             "<< finalDate        <<std::endl;
std::cout<< "recoveryTime          "<< recoveryTime     <<std::endl;
std::cout<< "infectionRate         "<< infectionRate    <<std::endl;
std::cout<< "infectionDist         "<< infectionDist    <<std::endl;
std::cout<< "randomSeed            "<< randomSeed       <<std::endl;
std::cout<< "show GUI              "<< visible          <<std::endl;
std::cout<< "grid number of x bins "<< NxCells          <<std::endl;
std::cout<< "grid number of y bins "<< NyCells          <<std::endl;
std::cout<< "grid xSize            "<< xSize            <<std::endl;
std::cout<< "grid ySize            "<< ySize            <<std::endl;
std::cout<< "xOrigin               "<< x0               <<std::endl;
std::cout<< "yOrigin               "<< y0               <<std::endl;
std::cout<< "agentFactoryType:     "<<agentFactoryType  <<std::endl;
std::cout<< "numberOfAgents:       "<<numberOfAgents    <<std::endl;
std::cout<< "populationGridFile:   "<<populationGridFile<<std::endl;
std::cout<< "agentFraction:        "<<agentFraction     <<std::endl;
std::cout<< "----------------------------- "            <<std::endl;
}
//--------------------------------------------------------------------------------------------
void parameters::getParameters(){
    
    std::cout<<"Hoping to use values from parameter file "<<parameterFile<<std::endl;
    try{
    std::fstream f;
    //f.exceptions ( ifstream::failbit | ifstream::badbit );
    f.open(parameterFile.c_str(),ios::in);
    std::string label;
    
    while (!f.eof()){
         std::string label;
        f>>label;
        //set # as comment character
        if (label[ 0 ] != '#'){
            bool success=false;

            if (label=="timeStep:")            {f>> timeStep;                success=true;}
            if (label=="nsteps:")              {f>> nsteps;                  success=true;}          
            if (label=="outputInterval:")      {f>> outputInterval;          success=true;}  
            if (label=="outputFile:")          {f>> outputFileName;          success=true;}  
            if (label=="diseaseLocationFile:") {f>> diseaseLocationFileName; success=true;}  
            if (label=="recoveryLocationFile:"){f>> recoveryLocationFileName;success=true;}  
            if (label=="isRestart:")           {f>> isRestart;               success=true;}       
            if (label=="restartInterval:")     {f>> restartInterval;         success=true;} 
            if (label=="restartFile:")         {f>> restartFileName;         success=true;} 
            if (label=="inputFile:")           {f>> inputFileName;           success=true;}   
            if (label=="initialDate:")         {f>> initialDate;             success=true;}     
            if (label=="finalDate:")           {f>> finalDate;               success=true;}       
            if (label=="recoveryTime:")        {f>> recoveryTime;            success=true;}    
            if (label=="infectionRate:")       {f>> infectionRate;           success=true;}   
            if (label=="infectionDist:")       {f>> infectionDist;           success=true;}   
            if (label=="randomSeed:")          {f>> randomSeed;              success=true;}      
            if (label=="showGUI:")             {f>> visible;                 success=true;}         
            if (label=="xBins:")               {f>> NxCells;                 success=true;}         
            if (label=="yBins:")               {f>> NyCells;                 success=true;}         
            if (label=="xSize:")               {f>> xSize;                   success=true;}           
            if (label=="ySize:")               {f>> ySize;                   success=true;}           
            if (label=="xOrigin:")             {f>> x0;                      success=true;}              
            if (label=="yOrigin:")             {f>> y0;                      success=true;}
            if (label=="populationGridFile:")  {f>>populationGridFile;       success=true;}
            if (label=="agentFactoryType:")    {f>>agentFactoryType;         success=true;}
            if (label=="numberOfAgents:")      {f>>numberOfAgents;           success=true;}
            if (label=="agentFraction:")       {f>>agentFraction;            success=true;}

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
}
