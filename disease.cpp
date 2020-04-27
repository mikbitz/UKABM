#include "disease.h"
#include "model.h"
#include "agent.h"
#include "parameters.h"
#include <omp.h>
#include <math.h>
disease::disease(){
    _infected=false;
    _recovered=false;
    _infectious=false;
    _timer=0;
    _infectionRate= parameters::getInstance().infectionRate;//per day
    _latencyTime  = parameters::getInstance().latencyTime  ;//in days
    _recoveryTime = parameters::getInstance().recoveryTime ;//in days
   _infectionDistance=parameters::getInstance().infectionDist;//in metres
}
disease::disease(std::string name){
    _infected=false;
    _recovered=false;
    _infectious=false;
    _timer=0;
    _infectionRate= parameters::getInstance().disease(name,"infectionRate");//per day
    _latencyTime  = parameters::getInstance().disease(name,"latencyTime"  );//in days
    _recoveryTime = parameters::getInstance().disease(name,"recoveryTime" );//in days
   _infectionDistance=parameters::getInstance().disease(name,"infectionDist");//in metres
}
void disease::infect(){_infected=true;_timer=0;}
void disease::tryToRecover(){
    //add small number to _recoveryTime to avoid errors if set to zero!
    if (-log(model::getInstance().random.number())<  1./(_recoveryTime+0.000001)*parameters::getInstance().timeStep/24./3600.)
        {
            _recovered=true;
            _infectious=false;
        }
}
void disease::maybeBecomeInfectious(){
    if (-log(model::getInstance().random.number())<  1./(_latencyTime+0.000001)*parameters::getInstance().timeStep/24./3600.)_infectious=true;
}
//constant rate of infection per unit time implies prob.
//exponentially distributed. If infection rate is r, then prob. of
//*not* being infected after finite time dt is exp(-r*dt)
// so rand(0,1)>exp(-rdt) for infection, or -log(rand)<r*dt. NB this decays
// more slowly than rand<r*dt, since there is a finite possibility of not being
// infected when r*dt>1
bool disease::infectionOccurs(){
    return -log(model::getInstance().random.number())< _infectionRate*parameters::getInstance().timeStep/24./3600.;
}
bool disease::infected(){return _infected;}
bool disease::recovered(){return _recovered;}
bool disease::infectious(){return _infectious;}
double disease::infectionDistance(){return _infectionDistance;}
void disease::update(){
    _timer++;
    if (!_recovered)maybeBecomeInfectious();
    tryToRecover();
    //if age (somecondition) die();
}
//------------------------------------------------------------------
//Testing section
//------------------------------------------------------------------
//void disease::test(){
//     cout<<"---Disease process testing---"<<endl;
//     bool success=true;
//     agent* a=new agent();
//     model::getInstance().g.add(a);
//     parameters& p=parameters::getInstance();
//     manipulate the rates for the disease to as to set them to constant test values 
//     p.infectionRate=24.*3600./p.timeStep * 0.001;//so likelihood is 0.001
//     p.infectionDist=0.75; 
//     p.recoveryTime=1./24./3600.*p.timeStep * 100;//so likelihood is 1./100 
//     testDisease=new disease(a);
//     a->addProcess(testDisease);
// 
//     make a infected
//     a->preUpdate();a->update();a->applyUpdate();
//     agent* b;
//     vector<agent*> v;
//     int pop=100;
//     for (int i=0;i<pop;i++){b=new agent();v.push_back(b);model::getInstance().g.add(b);}
//     now a can infect b - at first, though, don't let b spread the infection
// 
// 
//     int oldt=0,total=0;
//     a->infected=true;
//     /*for (int i=0;i<50;i++){
//        a->preUpdate();a->update();//don't call a->applyUpdate so that it does not recover
//        oldt=total;total=0;
//        for (auto p:v)if (p.exposed)total++;
//        fraction of uninfected pop. that are infected per unit time should be about constant
//        if (pop-oldt>0)cout<<pop-total<<" "<<double(total-oldt)/(pop-oldt)<<endl;
//     }
//     for (auto p:v){p.infected=true;}
//     for (int i=0;i<5000;i++){
//        total=0;
//        for (auto p:v){p.applyUpdate();if (p.recovered)total++;}
//        recovery should be exponential with 1/e life of 1000 steps
//        cout<<pop-total<<endl;
//     }*/
//     fullMixTest();
//     fixedSpatialTest();
//     delete testDisease;
//}
//------------------------------------------------------------------
//void disease::fullMixTest(){
//     cout<<"--- fully mixed (all agents in range of each other)---"<<endl;
//     //This tests whether the disease model is able to reproduce a standard well-mixed SIR model
//     //compare with output of diffEQ.py
//     //bool success=true;
//     parameters& p=parameters::getInstance();
//     //manipulate the rates for the disease to as to set them to constant test values
//     //here just look at one value of recovery rate.
//     double r=0.03;
//     p.recoveryTime=1./24./3600.*p.timeStep * 1./r; 
//     agent* a;
//     vector<agent*> v;
//     //strings for use in constructing filenames
//     string num,bs,rs,dir;
//     //a fairly small population size
//     int pop=100;
// 
// 
//     //do two initial infection sizes, 1 and 10 representing initial fractions of 0.01 and 0.1
//     for (int initPops=0;initPops<2;initPops++){
//         //set directories for output
//         int initInf0=1;dir="0.01";
//         if (initPops==1){initInf0=10; dir="0.1";}
//         //set up name string for recovery rate
//         stringstream sr; sr<<r;sr>>rs;
//         //loop over a set of infection rates, crossing the boundary of R0=b/r=1
//         for (double bn=0.01;bn<0.2;bn+=0.01){
//           //manipluate infection rate to get the right value
//           p.infectionRate=24.*3600./p.timeStep * bn/pop;
//           //string for filename
//           stringstream sb; sb<<bn;sb>>bs;
//           //run 200 replicates with different random seeds
//           for (int rn=0;rn<200;rn++){
//               
//                //construct filenames
//                stringstream s; s<<rn;s>>num;
//                ofstream f("testMix_Init_I_"+dir+"_withGoing/testMix_num_200_run_"+num+"_r_"+rs+"_b_"+bs);
//                //initialize random sequence
//                model::getInstance().random.setSeed(rn);
//                //delete any pre-existing agents from previous run
//                for (auto a:v){delete a;}
//                v.clear();model::getInstance().g.eraseAll();
//                //create new agents and add to model grid - all agents at position 0,0, so all can infect each other
//                for (int i=0;i<pop;i++){a=new agent();v.push_back(a);model::getInstance().g.add(a);}
//                //infect the initial population and make sure that the disease is fully active
//                for (int i=0;i<initInf0;i++){v[i]->addProcess(new disease(v[i]));v[i]->preUpdate();v[i]->update();v[i]->applyUpdate();}
//                //run the model for 1500 timesteps, accumulating totals
//                for (int i=0;i<1500;i++){
//                   int S=0,I=0,R=0;
//                   //coutn up totals at the current step
//                   for (auto a:v){
//                       if(!a->infected && !a->recovered)S++;
//                       if(a->infected )I++;
//                       if(a->recovered)R++;
//                   }
//                   //write current totals to output
//                   f<<S<<" "<<I<<" "<<R<<endl;
//                   //update agents
//                   for (auto a:v)a->preUpdate();
//                   for (auto a:v)a->update();
//                   for (auto a:v)a->applyUpdate();
//                }
//                //close output file
//                f.close();
//            }
//         }
//     }

//}
//------------------------------------------------------------------
//void disease::fixedSpatialTest(){
//     cout<<"--- test on a grid---"<<endl;
//     //This tests whether the disease model is able to produce a diffusing epidemic on a spatially ditributed but fixed agent set
//     //bool success=true;
//     parameters& p=parameters::getInstance();
//     //manipulate the rates for the disease to as to set them to constant test values
//     //here just look at one value of recovery rate.
//     double r=0.01;
//     p.recoveryTime=1./24./3600.*p.timeStep * 1./r; 
//     agent* a;
//     vector<agent*> v;
//     //strings for use in constructing filenames
//     string num,bs,rs,dir;
//     //a fairly small population size
//     int Ng=100;
//     float spacing=0.5;
//     p.infectionDist=1.;//0.75; 
//     float pop=p.infectionDist/spacing;
//     pop=pop*pop*acos(-1.);
// 
//     //do two initial infection sizes, 1 and 10 representing initial fractions of 0.01 and 0.1
//     for (int initPops=0;initPops<1;initPops++){
//         //set directories for output
//         //int initInf0=1;dir="0.01";
//         //if (initPops==1){initInf0=10; dir="0.1";}
//         //set up name string for recovery rate
//         stringstream sr; sr<<r;sr>>rs;
//         //loop over a set of infection rates, crossing the boundary of R0=b/r=1
//         for (double bn=0.1;bn<0.2;bn+=0.1){
//           //manipluate infection rate to get the right value: use the mean population within an infection distance
//           //as a scaling on the base bn - this can lead to rather small infection rates though...
//           p.infectionRate=24.*3600./p.timeStep*bn/pop;
//           //string for filename
//           stringstream sb; sb<<bn;sb>>bs;
//           //run 100 replicates with different random seeds
//           for (int rn=0;rn<1;rn++){
//               
//                //construct filenames
//                stringstream s; s<<rn;s>>num;
//                ofstream f("testSpatial_Init_I_"+dir+"/testSpatial_num_100_run_"+num+"_r_"+rs+"_b_"+bs);
//                //initialize random sequence
//                model::getInstance().random.setSeed(72);
//                //delete any pre-existing agents from previous run
//                for (auto a:v){delete a;}
//                v.clear();model::getInstance().g.eraseAll();
//                //create new agents and add to model grid 
//                //all agents distributed uniformly at spacing
//                for (float i=0;i<Ng;i++){
//                   for (float j=0;j<Ng;j++){
//                       float x=i*spacing,y=j*spacing;
//                       a=new agent();v.push_back(a);a->loc.x=x;a->loc.y=y;model::getInstance().g.add(a);
//                       if (i<=5 ){
//                         //infect the initial population and make sure that the disease is fully active
//                         a->addProcess(new disease(a));a->preUpdate();a->update();a->applyUpdate();
//                       }
//                   } 
//                }
//                //run the model for 1500 timesteps, accumulating totals
//                for (int i=0;i<900;i++){
//                   // random_shuffle(v.begin(),v.end());
// 
//                   int S=0,I=0,R=0;
//                   model::getInstance().tick=i+1;
//                   //coutn up totals at the current step
//                   for (auto a:v){
//                       if(!a->infected && !a->recovered)S++;
//                       if(a->infected )I++;
//                       if(a->recovered)R++;
//                   }
//                   //write current totals to output
//                   f<<S<<" "<<I<<" "<<R<<endl;
//                   //update agents
//                   for (auto a:v)a->preUpdate();
//                   for (auto a:v)a->update();
//                   for (auto a:v)a->applyUpdate();
//                }
//                //close output file
//                f.close();
//            }
//         }
//     }

//}
//------------------------------------------------------------------
//void disease::testMessage(string s,bool success){
 //   if (success) cout<<s + " passed" <<endl; else cout<<s +" failed horribly! ARG!"<<endl;
//}
//------------------------------------------------------------------
//disease* disease::testDisease=NULL;
