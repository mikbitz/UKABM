#ifndef MODEL_H
#define MODEL_H

/**
\file model.h
\brief This is the model class header file
*/
#include <vector>
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include "layer.h"
#include "searchGrid.h"
#include "parameters.h"
#include <random>
//------------------------------------------------------------------------
//Set up a wrapper class that will provide uniform random numbers between 0 and 1
class randomizer {
    std::uniform_real_distribution<> uniform_dist;
 
    // Use mersenne twister with fixed seed as the random number engine
    std::mt19937 twister;
public:
    randomizer(){
      uniform_dist=std::uniform_real_distribution<> (0,1);
      cout<<"A randomizer was set up with seed " <<parameters::getInstance()->randomSeed<<endl;
      twister.seed(parameters::getInstance()->randomSeed);
    }
    double number(){
     return uniform_dist(twister);
    }
    void setSeed(int s){
        twister.seed(s);
    }
};
//------------------------------------------------------------------------
class configuration;
using namespace std;
/**
\brief The model holds the agents in a vector and drives their updates

This class is a singleton (see e.g Shalloway and Trott "Design Patterns Explained")  - only one instance of the class can be created on any thread. The class has a protected constructor so that no instance can be created directly - instead the static function getInstance() must be used. The first time getInstance() is called, the class constructor is used to generate the required data - thereafter the constructor is not called again, and getInstance returns a pointer to the pre-existing instance. This means getInstance() can be called anywhere within the code and it will return the same environmental information\n
The init funtion should be called after first creation of the singleton for those items that depend on a fully 
completed singleton for their existence e.g. the configuration class, which calls model::add(layer*)
usage example:-\n
\code
model* m=model::getInstance();
m->init();
//advance to the next state
m->update();

\endcode 


@author Mike Bithell <mb425@cam.ac.uk>
*/

class model{
public:
     static model* getInstance();
     void init();
     bool update();
     void runOneStep(){
       update();
     }
     void run(){
       while(1){
         update();
       }
     }
     void runUpTo(long n){
       for (long i=0;i<n;i++){
         update();
       }
     }
     unsigned tick;
     float dt;
     ~model();
     void add(layer *l);
     layer* getLayer(unsigned);
     void finish();
     double getSize();
     string getText();
     searchGrid g;
     vector<agent*>* ugs;
     randomizer random;
private:

     static model* instance;
     model();
     void clean();
     vector<layer*> layers;
     configuration* c;
     string text;
  
};
#endif
