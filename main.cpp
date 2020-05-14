#include "parameters.h"
#include "model.h"
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

 using namespace std;

 int main(int argc, char *argv[])
 {
     cout<<"Model version -0.9"<<endl;
     cout<<"Run started at: "<<boost::posix_time::second_clock::local_time()<<endl;

    if (argc ==1){
        cout<<"Using defaults"<<endl;
        parameters::getInstance("parameterFile");
    }else{
        cout<<"Defaults overridden on command line"<<endl;
        parameters::getInstance(argv[1]);
    }
    model& m=model::getInstance();
    //Initialize the model singleton after first creation so that setup that depends on
    //the model being fully created has something to work with.
    m.init();

     int smax=parameters::getInstance().nsteps;
     for (int i=0;i<smax;i++){m.update();if (i%10==0)cout<<"model step:"<<i<<endl;}
     m.finish();
    cout<<"Run finished at: "<<boost::posix_time::second_clock::local_time()<<endl;
 }
/**
 * @mainpage
 * 
 * This model is aimed at representing the patterns of movement and interaction of agents that represent individual people as they go about their daily activities.
 * 
 * The current objective is to be able to model the COVID-19 outbreak of 2020, and to tie this to agent behaviour at the scale of an entire country.
 * 
 * @section intro_sec Introduction
 * @subsection Main Main ideas
 * @subsection Run Running the model
 **/
