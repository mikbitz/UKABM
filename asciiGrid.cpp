//Read gridded data from and ESRI ascii raster grid file and store in a vector.
//Note that the file is read one line at a time as a text string, then converted to numeric format as appropriate
//The data is stored with the first line being the top (Northernmost) row of the raster grid, listed west-to-east along the rows
//Data is assumed regularly gridded (i.e. same grid cell size in x and y directions)
#include "asciiGrid.h"
#include "model.h"
#include <iostream>
#include <sstream>


    asciiGrid::asciiGrid(std::string s){
        readFile(s);
    }
    //----------------------------------------------------------------------------------------------
    void asciiGrid::readFile( const std::string& filePath ) {
        
        std::cout << "Reading ARC ascii grid file \"" << filePath << "\"..." << std::endl;
        std::ifstream fileStream( filePath.c_str( ), std::ios::in );
        
        if( fileStream.is_open( ) ) {
            
            readHeader(fileStream);

            readData(fileStream);
           
            fileStream.close( );
            for (unsigned y=0;y<_data.size();y++){
                for (unsigned x=0;x<_data[y].size();x++){
                    if (_data[y][x]>_NODATA_value) _index.push_back(x+_ncols*y);
                }
            }
        } else {
            std::cout << "File path \"" << filePath << "\" is invalid." << std::endl;
            exit(0);
        }
    }
    //---------------------------------------------------------------------------------------------- 
    point2D asciiGrid::getValidPoint(unsigned i){
         //find a valid location

         unsigned x=_index[i] % _ncols;
         unsigned y=_index[i] / _ncols;
         double xr=model::getInstance()->random.number();
         double yr=model::getInstance()->random.number();
         return point2D(_xllcorner+(x+xr)*_cellSize,_yllcorner+(_nrows-1-y+yr)*_cellSize);
     }
     //---------------------------------------------------------------------------------------------- 
     double asciiGrid::getDataAt(unsigned i){
         //find a valid location
         unsigned x=_index[i] % _ncols;
         unsigned y=_index[i] / _ncols;

         return _data[y][x];
     }
     //---------------------------------------------------------------------------------------------- 
     bool asciiGrid::isValid(unsigned i){
         return i<_index.size();
     }
    //----------------------------------------------------------------------------------------------
    void asciiGrid::readData(std::ifstream& fileStream){
        //NB because this is read row by row the data order is data[y][x] where x is West to East and y is North to South
        std::string readLine;        
        while( std::getline( fileStream, readLine ) ) {
            _data.push_back( LineToDouble( readLine, ' ' ) );
        }
    }
    //----------------------------------------------------------------------------------------------
    void asciiGrid::readHeader(std:: ifstream& fileStream){
        std::string readLine;
        //ascii header has the form
        //ncols 36
        //nrows 13 
        //xllcorner -180
        //yllcorner  -65
        //cellsize  10
        //NODATA_value -9999
        //StringToWords returns a string split into a vector: ARC ascii uses space
        std::cout<<"Header data"<<std::endl;
        std::getline( fileStream, readLine );
        _ncols= StringToDouble(StringToWords( readLine, ' ' )[1]);
        std::cout<<"Columns:"<<" "<<_ncols<<std::endl;
        std::getline( fileStream, readLine );
        _nrows= StringToDouble(StringToWords( readLine, ' ' )[1]);
        std::cout<<"Rows:   "<<_nrows<<std::endl;
        std::getline( fileStream, readLine );
        _xllcorner= StringToDouble(StringToWords( readLine, ' ' )[1]);
        std::cout<<"X lower left corner:"<<_xllcorner<<std::endl;
        std::getline( fileStream, readLine );
        _yllcorner= StringToDouble(StringToWords( readLine, ' ' )[1]);
        std::cout<<"Y lower left corner:"<<_yllcorner<<std::endl;
        std::getline( fileStream, readLine );
        _cellSize= StringToDouble(StringToWords( readLine, ' ' )[1]);
        std::cout<<"cellsize:"<<_cellSize<<std::endl;
        std::getline( fileStream, readLine );
        _NODATA_value= StringToDouble(StringToWords( readLine, ' ' )[1]);
        std::cout<<"Nodata:"<<_NODATA_value<<std::endl;
    }
    //----------------------------------------------------------------------------------------------
    const std::vector<std::string> asciiGrid::StringToWords( const std::string& inputString, const char wordTerminationCharacter ) const {
        // convert a line read from the file into a set of individual strings using a given separator
        std::stringstream stringStream( inputString );
        
        std::string word = "";
        std::vector<std::string> wordList;
        
        while( std::getline( stringStream, word, wordTerminationCharacter ) ) {
            wordList.push_back( word );
        }
        
        return wordList;
    }
    //----------------------------------------------------------------------------------------------
    const std::vector<double> asciiGrid::LineToDouble( const std::string& inputString, const char wordTerminationCharacter ) const {
         // convert a line read from the file into a set of doubles using a given separator
       
        std::stringstream stringStream( inputString );
        
        std::string word = "";
        std::vector<double> doubleList;
        
        while( std::getline( stringStream, word, wordTerminationCharacter ) ) {
            doubleList.push_back( StringToDouble(word) );
        }
        
        return doubleList;
    }
    //----------------------------------------------------------------------------------------------
    double asciiGrid::StringToDouble( const std::string& string ) const {
        // convert an individual string to double
        double number = std::stod( string.c_str( ), NULL );
        
        return number;
    }
    //----------------------------------------------------------------------------------------------
    int asciiGrid::StringToInt( const std::string& string ) const {
        // convert an individual string to int
        double number = std::stoi( string.c_str( ), NULL );
        
        return number;
    }
    //----------------------------------------------------------------------------------------------



