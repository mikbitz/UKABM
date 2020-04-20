#include "parameters.h"
#include <iostream>
#include <fstream>
//-----------------------------------------------------------------------------------------------------------------
parameters* parameters::instance;
//-----------------------------------------------------------------------------------------------------------------
parameters::~parameters(){
if (instance!=NULL)delete instance;
}
//-----------------------------------------------------------------------------------------------------------------
parameters* parameters::getInstance(){
 if (instance==NULL){
   instance=new parameters();
 }
 return instance;
}
//-----------------------------------------------------------------------------------------------------------------
parameters* parameters::getInstance(string f){
 if (instance==NULL){
   instance=new parameters(f);
 }
 return instance;
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
    //                 if( readLine[ 0 ] != Constants::cTextFileCommentCharacter ) {
std::cout<< "Input Parameter File "<<parameterFile     <<std::endl;
std::cout<< "----------------------------- "           <<std::endl;
std::cout<< "Successfully found parameters "           <<std::endl;
std::cout<< "timeStep              "<< timeStep        <<std::endl;
std::cout<< "number of timesteps   "<< nsteps          <<std::endl;
std::cout<< "outputInterval        "<< outputInterval  <<std::endl;
std::cout<< "outputFileName        "<< outputFileName  <<std::endl;
std::cout<< "diseaseLocationFileName        "<< diseaseLocationFileName  <<std::endl;
std::cout<< "recoveryLocationFileName        "<< recoveryLocationFileName  <<std::endl;
std::cout<< "isRestart             "<< isRestart       <<std::endl;
std::cout<< "restartInterval       "<< restartInterval <<std::endl;
std::cout<< "restartFileName       "<< restartFileName <<std::endl;
std::cout<< "inputFileName         "<< inputFileName   <<std::endl;
std::cout<< "initialDate           "<< initialDate     <<std::endl;
std::cout<< "finalDate             "<< finalDate       <<std::endl;
std::cout<< "recoveryTime          "<< recoveryTime    <<std::endl;
std::cout<< "infectionRate         "<< infectionRate   <<std::endl;
std::cout<< "infectionDist         "<< infectionDist   <<std::endl;
std::cout<< "randomSeed            "<< randomSeed      <<std::endl;
std::cout<< "show GUI              "<< visible         <<std::endl;
std::cout<< "grid number of x bins "<< NxCells         <<std::endl;
std::cout<< "grid number of y bins "<< NyCells         <<std::endl;
std::cout<< "grid xSize            "<< xSize           <<std::endl;
std::cout<< "grid ySize            "<< ySize           <<std::endl;
std::cout<< "xOrigin               "<< x0               <<std::endl;
std::cout<< "yOrigin               "<< y0               <<std::endl;
std::cout<< "----------------------------- "           <<std::endl;
}
//--------------------------------------------------------------------------------------------
void parameters::getParameters(){

  std::cout<<"Hoping to use values from parameter file "<<parameterFile<<std::endl;
  try{
   std::fstream f;
   f.exceptions ( ifstream::failbit | ifstream::badbit );
   f.open(parameterFile.c_str(),ios::in);
   std::string label;
   for (int i=0;i<23;i++){
    f>>label;
    if (label=="timeStep:")       f>> timeStep;        else {
    if (label=="nsteps:")         f>> nsteps;          else {
    if (label=="outputInterval:") f>> outputInterval;  else {
    if (label=="outputFile:")     f>> outputFileName;  else {
    if (label=="diseaseLocationFile:")     f>> diseaseLocationFileName;  else {
    if (label=="recoveryLocationFile:")     f>> recoveryLocationFileName;  else {
    if (label=="isRestart:")      f>> isRestart;       else {
    if (label=="restartInterval:")f>> restartInterval; else {
    if (label=="restartFile:")    f>> restartFileName; else {
    if (label=="inputFile:")      f>> inputFileName;   else {
    if (label=="initialDate:")    f>> initialDate;     else {
    if (label=="finalDate:")      f>> finalDate;       else {
    if (label=="recoveryTime:")   f>> recoveryTime;    else {
    if (label=="infectionRate:")  f>> infectionRate;   else {
    if (label=="infectionDist:")  f>> infectionDist;   else {
    if (label=="randomSeed:")     f>> randomSeed;      else {
    if (label=="showGUI:")        f>> visible;         else {
    if (label=="xBins:")          f>> NxCells;         else {
    if (label=="yBins:")          f>> NyCells;         else {
    if (label=="xSize:")          f>> xSize;           else {
    if (label=="ySize:")          f>> ySize;           else {
    if (label=="xOrigin:")        f>> x0;              else {
    if (label=="yOrigin:")        f>> y0;              else {
    std::cout<<"Unrecognised label "<<label<<" in file "<<parameterFile<<" exiting..."<<std::endl;exit(1); }}}}}}}}}}}}}}}}}}}}}}}
   }
   f.close();
   printParameters();

  } catch (fstream::failure err){
   std::cout<<"IO error while reading data file "<<parameterFile<<std::endl;
   exit(1);
  }
}


