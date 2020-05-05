#include "places.h"
#include "model.h"
#include "readcsv.h"
unsigned place::idnum=0;
places * places::instance;
//------------------------------------------------------------------------------------------------------------------------

    places::places(){

    }
//------------------------------------------------------------------------------------------------------------------------
    places& places::getInstance(){
        if (instance==NULL){
          instance=new places();
        }
        return *instance;
    }
 //------------------------------------------------------------------------------------------------------------------------
    void places::init(){   
      //read in placeTypes
      auto types=readcsv(parameters::getInstance().placeTypeFile);
      //now actual x-y co-ordinates
      auto locations=readcsv(parameters::getInstance().placeFile);
      //add places to the model grid
      //model::getInstance().g.add();
    }
//------------------------------------------------------------------------------------------------------------------------
    bool places::update(){return false;}
//------------------------------------------------------------------------------------------------------------------------

    void places::getLocations(std::map<place, point2D>& locations){
        locations[home]=model::getInstance().g.getRandomPoint();
        locations[work]=model::getInstance().g.getRandomPoint();
    }
//make 1200 hospitals proportional to population size
//put them in a grid.
