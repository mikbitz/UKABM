#ifndef PLACES_H
#define PLACES_H
#include "layer.h"
#include "point2D.h"
#include <map>
#include<vector>
#include <string>
#include <iostream>
/**
 * @class place
 * 
 * @brief A place has x,y co-ordinates, and possibly a workforce and capacity
 * 
 * Each place has a unique ID number allocated in sequence as places are created, starting at 0. Thsi gets incremented in the constructor.
 * A place also has a generic type number, which maps to a string (stored in teh palces singleton)
 * 
 */

class place{
public:

    place(){_type=0;init();}
    place(unsigned t){_type=t;init();}
    void init(){_ID=idnum;idnum++;_workForceSize=0;_capacity=500;}
    void setType(unsigned t){_type=t;}
    void setLocation(point2D p){loc=p;}
    void setLocation(double x, double y){loc=point2D(x,y);}
    point2D getLocation(){return loc;}
    void incrementWorkForce(){_workForceSize++;}
    unsigned placeType(){
        return _type;
    }
    unsigned ID(){return _ID;}
    double X(){return loc.X();}
    double Y(){return loc.Y();}
    unsigned workForceSize(){return _workForceSize;}
    point2D loc;
private:
    /**
     * @brief This class vaiable keeps track of place IDs
     * 
     */
    static unsigned idnum;
    /**
     * @brief The unique ID number for this place
     * 
     */
    unsigned _ID;
    unsigned _workForceSize;
    unsigned _capacity;
    unsigned _type;
};
/**
 * @class places
 * 
 * @brief Places stores the place type index and a list of instantiated places
 * 
 * There are three main data stuctures here: a vector of pointers to all the places, so that they can be iterated over or retrieved without reference to spatial position.
 * Secondly there is an index that maps from strings to unsigned integers, so that places can be given an integer type. E.g. the string "hospital" might map to integer 0.
 * These indices are set up in init() according to the order place types are listed in the csv file pointed at by parameters placeTypeFile. This avoids having to store strings in other
 * parts of the model: for example, the searchGrid stores places in each cell indexed by type - there may be many of the same type in a given cell.
 * Finally there are named significant places that are in some sense representative of populated settlements such as London
 **/ 
//------------------------------------------------------------------
class places{
protected:
    /**
     * @brief Private constructor can only be called by the class itself
     * 
     * Since this is singleton the contructor should only ever be called once.
     * 
     */
    places();
    /**
     * @brief A vector of pointers to all the places currently defined in the model
     * 
     * Places is a singleton (there can only be one instance of this class in the model)
     * 
     */
    std::vector<place*>_allPlaces;
    /**
     * @brief The index of place types
     * 
     * Given a string as a key, this variable stores an index that can be used to refer to this type of place
     * 
     */
    std::map<std::string,unsigned>_placeTypes;
    /**
     * @brief A simple dictionary of a few named "locations" that allow one to find a representative point
     * 
     * Strings should be something like names of towns or cities, with the point being close to a centroid for the area represented by the name e.g. "London","Birmingham"
     * 
     */
    std::map<std::string,point2D>_gazetteer;
public:
    /**
     * @brief Get a reference to the place class
     * 
     * @return places&
     */
    static places& getInstance();
    /**
     * @brief Return the type index index for a named place
     * 
     * If a string is given as an argument, and there is no corresponding key then places::unknownType is returned 
     * 
     * @param  s: A string giving the name of the type of place
     * @return unsigned int
     */
    unsigned operator[](std::string);
    /**
     * @brief Return the co-ordinates of a place stored in the gazetteer, which lists the centroids of a few significant populated settlements 
     * 
     * @param s p_s: A string giving the name of the place
     * @return point2D
     */
    point2D  locationOf(std::string);
    /**
     * @brief Set up the index of place types and read in actual places
     * 
     * Caution with this function! Places are inserted into the searchGrid model::g so that they can be found by spatial searches.
     * If at any point the grid is re-sized, then this data may become invalid. Important therefore to make sure this function is
     * called *after* any grid re-sizing (which should in any case only happen during model initialisation) 
     * 
     */
    void init();
    /**
     * @brief Stub provided in case at some point the data in places becomes dynamic and needs its own updates
     * 
     * @return bool
     */
    bool update();
    /**
     * @brief The pointer that stores the instance of places
     * 
     */
    static places* instance;
    /**
     * @brief Returned if there is no know place of this type
     * 
     * Currently set to the maxium value for an unsigned integer (unlikely ever to be this manby place types!)
     * 
     */
    static unsigned unknownType;

    /**
     * @brief Function to report how many people are currently working in the list of all places
     * 
     */
    void printWorkForceSizes();
    
};
#endif

    
