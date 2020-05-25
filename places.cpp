#include "places.h"
#include "model.h"
#include "readcsv.h"
#include <assert.h>
unsigned place::idnum=0;
places * places::instance;
unsigned places::unknownType=std::numeric_limits<unsigned>::max();
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
            if (_placeTypes.find(s)==_placeTypes.end())return unknownType;
            return _placeTypes[s];
        }
//------------------------------------------------------------------------------------------------------------------------
        point2D places::locationOf(std::string s){
            if (_gazetteer.find(s)==_gazetteer.end()){cout<<"Warning: place "<<s<<"not found in gazetteer: returning (0,0)";return point2D(0,0);}
            return _gazetteer[s];
        }
//------------------------------------------------------------------------------------------------------------------------
 void places::init(){   
     //read in placeTypes
     auto types=readcsv(parameters::getInstance().placeTypeFile);
     
     //care here - extra whitespace at end of file can be a problem
     for (unsigned i=0;i<types.nrows();i++)if (types[i].size()>0)_placeTypes[types[i][0]]=i;
                
     //now actual x-y co-ordinates of particular places of (possibly) each type
     auto locations=readcsv(parameters::getInstance().placeFile);

     //add places to the model grid
     
     for (unsigned row=0;row<locations.nrows();row++){

         place* p=new place(_placeTypes[locations[row][0]]);
         _allPlaces.push_back(p);
         p->setLocation(std::stod(locations[row][1]),std::stod(locations[row][2]));
         model::getInstance().g.add(p);
     }
     //a few named "locations" e.g. London that are useful for finding nearby agents?
     auto gazzy=readcsv("simpleGazeteer.csv");
     for (unsigned row=0;row<gazzy.nrows();row++){
         _gazetteer[gazzy[row][0]]=point2D(std::stod(gazzy[row][1]),std::stod(gazzy[row][2]));
     }
     
 }
//------------------------------------------------------------------------------------------------------------------------
    bool places::update(){return false;}
//------------------------------------------------------------------------------------------------------------------------
    void places::printWorkForceSizes(){for (auto p:_allPlaces)std::cout<<p->workForceSize()<<std::endl;}
