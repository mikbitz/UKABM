#include "places.h"
#include "model.h"
#include "readcsv.h"
#include <assert.h>
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
        unsigned places::operator[](std::string s){
            assert(_placeTypes.find(s)!=_placeTypes.end());
            return _placeTypes[s];
        }
//------------------------------------------------------------------------------------------------------------------------
 void places::init(){   
     //read in placeTypes
     auto types=readcsv(parameters::getInstance().placeTypeFile);
     std::map<string,unsigned> _placeTypes;
     for (unsigned i=0;i<types.nrows();i++)_placeTypes[types[i][0]]=i;
    
     //now actual x-y co-ordinates of particular places of (possibly) each type
     auto locations=readcsv(parameters::getInstance().placeFile);
     
     //add places to the model grid
     for (unsigned row=0;row<locations.nrows();row++){
         place* p=new place(_placeTypes[locations[row][0]]);
         p->setLocation(std::stod(locations[row][1]),std::stod(locations[row][2]));
         model::getInstance().g.add(p);
     }
 }
//------------------------------------------------------------------------------------------------------------------------
    bool places::update(){return false;}
//------------------------------------------------------------------------------------------------------------------------

    void places::getLocations(std::map<placeE, point2D>& locations){
        locations[home]=model::getInstance().g.getRandomPoint();
        locations[work]=model::getInstance().g.getRandomPoint();
    }
