#include "layer.h"
#include <iostream>

         layer::layer(){name="Default";}
void     layer::setName(std::string s){name=s;}
extent3D layer::getExtent(){return extent;}
void     layer::setExtent(double x,double y,double z){extent.setExtent(x,y,z);}

bool layer::update(){ std::cout<<"layer top level update - maybe not what you want"<<std::endl;return false;}
