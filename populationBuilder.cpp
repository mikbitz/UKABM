//Read gridded data from and ESRI ascii raster grid 
//Provide function to allow agents to be built at each location with the frequency given in a file
//and selecting only a fraction.
//Note, if the resolution of the file is very fine the fraction selected may not be very accurate... 
#include "populationBuilder.h"
#include "asciiGrid.h"
#include "model.h"
#include "readcsv.h"
#include <iostream>
#include <sstream>
#include <point2D.h>

    populationBuilder::populationBuilder(){
        _asciiG.readFile(parameters::getInstance().populationGridFile);
        //resize the grid - NB grid cellsize is set by user, not from the input file
        //as this controls the efficiency of search
        model::getInstance().g.resize(
                   _asciiG.xOrigin(),
                   _asciiG.yOrigin(),
                   _asciiG.xSize(),
                   _asciiG.ySize(),
                   parameters::getInstance().NxCells,
                   parameters::getInstance().NyCells);
        //places need to be initialised after grid resize
         places& p=places::getInstance();
         p.init();
        _iter=0;
        _remainingHere=0;
        _frac=parameters::getInstance().agentFraction;
    }
    //----------------------------------------------------------------------------------------------
    point2D populationBuilder::getNextLocation(){
        return _asciiG.getValidRandomisedPoint(_iter);
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::startCount(){
        _iter=0;
        //frac very roughly tries to select a population subset -not guaranteed to work exactly.
        _remainingHere=_asciiG.getDataAt(_iter)*_frac;
    }
    //----------------------------------------------------------------------------------------------
    int populationBuilder::personAtNextLocation(){
        //count down the population at each location in an asciiGrid
        //if the pop is done at current location, advance to the next.
        //return 0 is location is empty, 1 is there is a person, -1 if no more people anywhere
        if (!_asciiG.isValid(_iter)) return -1;
        int success=0;
        if (_remainingHere>=1){
            _remainingHere-=1;
            success=1;
        }else{
            //allow for fractional values in population data (regarding the grid values as a kind of average frequency in a cell)
            if (model::getInstance().random.number()> _remainingHere){
                _iter++;
                success=0;
                if (_asciiG.isValid(_iter)) _remainingHere=_asciiG.getDataAt(_iter)*_frac;
                else success=-1;
            }else{
                _remainingHere-=1;
                success=1;
            }
        }
        return success;
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::configurePopulation(std::vector<agent*>&agents){
        //randomize the agent order so that we don't have preferred agents by grid location  
        shuffle (agents.begin(), agents.end(), std::default_random_engine(parameters::getInstance().randomSeed));
        _workingPop=agents.size();
        for (auto& a:agents){
            setSex(a);
            setAge(a);
            findPartner(a);
            findParents(a);
            setUpEducation(a);
            findFriends(a);
        }
        setUpWork(agents);
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::setSex(agent* a){
        if (model::getInstance().random.number()<0.5)a->setSex('m');//defaults to f
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::setAge(agent* a){
        double r=model::getInstance().random.number();
        //uniform age up to 60 plus 20% over 60 with a linear decline to 90 (implies quadratic cumulative distribution)
        a->setAge(r*60);
        r=model::getInstance().random.number();
        if (r>0.8)a->setAge(90-30*pow((1-(r-0.8)/0.2),0.5));
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::findPartner(agent* a){
        ;//if (age>16)findPartner()//allow for singles, partners move in together (homeless?), allow for boy/girlfriends? 
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::findParents(agent* a){
        ;//if (age<16)moveToHome()//percent in care?percent single families?- also homeless?
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::setUpEducation(agent* a){
        retired(a);
        if (a->age()<=65)worker(a);
        if (a->age()<21)university(a);
        if (a->age()<18)sixthform(a);
        if (a->age()<16)schoolchild(a);
        if (a->age()< 5)preschool(a);
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::retired(agent* a){
        if ((a->age()>60 && a->sex()=='f') || (a->age()>65)){
            _workingPop--;
            a->setWorkStatus("retired");//workstatus defaults to unemployed - unemployed and retired currently have work set to same as home.
        }
        //set agent to retired, workplace=home
        if (a->age()>80);//in care (random?) home and work to carehome
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::worker(agent* a){
        ;//inwork//includes unemployed/not participating in work - disabilities?//UK workforce participation is about 64% - unemployment about 4% - so about 33M employed? Nearly equal M/F
        ;//above age distrib. gives about 58% 
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::university(agent* a){
        if (a->age()>=18 && model::getInstance().random.number() < 0.3){
            _workingPop--;
            a->setWorkStatus("ineducation");//schools and universities currently are all "at home"! need a way to set these.
            a->setEducationStatus("higher");
        }//in unversity
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::sixthform(agent* a){
        _workingPop--;
        a->setWorkStatus("ineducation");
        a->setEducationStatus("uppersecondary");//upper secondary school - all agents under 18 taken out of workingPop - workplace=school
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::schoolchild(agent* a){
        if (a->age()>=12)a->setEducationStatus("secondary");//lower secondary
        else             a->setEducationStatus("primary");//primary
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::preschool(agent* a){
        a->setEducationStatus("preschool");//neither in work nor school (but maybe in nursery?)
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::setUpWork(std::vector<agent*>&agents){
        cout<<"Labour participation expected "<<float(_workingPop)/agents.size()<<endl;
        
        auto jobTypes=readcsv("jobTypes");//table of jobtypes and workplaces, with fraction of jobType in each placeType
        auto placeTypes=jobTypes.getHeader();//header gives placetype string for each job
        
        //loop over types of job
        for (unsigned jt=0;jt<jobTypes.nrows();jt++){
            for (unsigned p=1;p<placeTypes.size();p++){
                //retrieve placeType index - this allows one to find the places in the grid
                auto placeType=places::getInstance()[placeTypes[p]];
                //cout<<placeTypes[p]<<" "<<placeType<<endl;
                if (placeType!=places::unknownType){
                    //number of workers expected - could be emtpy
                    if (jobTypes[jt].size()>p && jobTypes[jt][p]!=""){
                        double fraction=std::stod(jobTypes[jt][p]);
                        if (fraction>0){
                            unsigned number=_workingPop*fraction;
                            cout<<jobTypes[jt][0]<<endl;
                            for (auto a:agents){
                                if (number>0 && a->worker() && !a->hasWork()){//number counts for this jobtype and placetype
                                    double d=commuteDistance();
                                    if (jobTypes[jt][0] !="generic"){
                                        auto possibleWorkPlaces=model::getInstance().g.inRadius(a,placeType,d);//gridded places give locations
                                        //returned places are ordered by distance, closest first - what if none found?
                                        if (possibleWorkPlaces.size()>0){
                                            number--;
                                            unsigned k=possibleWorkPlaces.size()-1;//choose k to start from largest distance (should be closest to attempted commute distance)
                                            a->setWorkPlace(possibleWorkPlaces[k]);// one of closest to commute distance with not all places taken -currently places take any number!
                                            possibleWorkPlaces[k]->incrementWorkForce();
                                            a->setJobType(jt);
                                        }
                                    }else{
                                        a->setJobType(jt);
                                        double pi=acos(-1.);
                                        //random direction from home to work
                                        double dcc=d*cos(2*pi*model::getInstance().random.number());
                                        double dcs=d*sin(2*pi*model::getInstance().random.number());
                                        a->knownLocations[places::getInstance()["work"]]=a->knownLocations[places::getInstance()["home"]]+point2D(dcc,dcs);
                                    }
                                }
                            }
                            //cout<<number<<endl;
                        }
                    }
                }
            }
        }
        //places::getInstance().printWorkForceSizes();
    }
    //----------------------------------------------------------------------------------------------
    void populationBuilder::findFriends(agent* a){
        ;
        //allow for age and work/school assortativity...
    }
    //----------------------------------------------------------------------------------------------
    double populationBuilder::commuteDistance(){
        //1/x^2 distrubtion for the moment, up to 100km (=>inverse linear cumulative distrib.)
        return 1010 / (1.01 - model::getInstance().random.number());
    }
