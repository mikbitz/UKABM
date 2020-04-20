#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "model.h"
#include "agents.h"
#include "asciiGrid.h"


class configuration{
public:
  configuration(){
    //add layers to the model
      asciiGrid a("worldpop/gbr_ppp_2020UKnatgrid100m.asc");
      exit(0);
    model* thisModel=model::getInstance();
    thisModel->add(new agents());
    
  }

};

#endif
