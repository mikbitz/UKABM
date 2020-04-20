#include "places.h"
#include "model.h"
places * places::instance;
//------------------------------------------------------------------------------------------------------------------------

    places::places(){

    }
//------------------------------------------------------------------------------------------------------------------------
    places* places::getInstance(){
        if (instance==NULL){
          instance=new places();
        }
        return instance;
    }
 //------------------------------------------------------------------------------------------------------------------------
    void places::init(){   

    }
//------------------------------------------------------------------------------------------------------------------------
    bool places::update(){return false;}
//------------------------------------------------------------------------------------------------------------------------

    void places::getLocations(std::map<place, point2D>& locations){
        locations[home]=model::getInstance()->g.getRandomPoint();
        locations[work]=model::getInstance()->g.getRandomPoint();
    }
