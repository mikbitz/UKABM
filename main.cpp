 #include "parameters.h"
 #include "model.h"
 #include <iostream>

 using namespace std;

 int main(int argc, char *argv[])
 {

    if (argc ==1){
        cout<<"Using defaults"<<endl;
        parameters::getInstance("parameterFile");
    }else{
        cout<<"Defaults overridden on command line"<<endl;
        parameters::getInstance(argv[1]);
    }

    model* m=model::getInstance();
    //Initialize the model singleton after first creation so that setup that depends on
    //the model being fully created has something to work with.
    m->init();
      
     int smax=parameters::getInstance()->nsteps;
     for (int i=0;i<smax;i++){m->update();if (i%10==0)cout<<i<<endl;}

 }
