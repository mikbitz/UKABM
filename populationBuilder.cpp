//Read gridded data from and ESRI ascii raster grid 
//Provide function to allow agents to be built at each location with the frequency given in a file
//and selecting only a fraction.
//Note, if the resolution of the file is very fine the fraction selected may not be very accurate... 
#include "populationBuilder.h"
#include "asciiGrid.h"
#include "model.h"
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





