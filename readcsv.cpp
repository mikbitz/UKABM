#include "readcsv.h"
#include "model.h"
#include <iostream>
#include <sstream>


    readcsv::readcsv(std::string s,bool header,bool commented){
                
        if (commented)_commentCharacter='#'; else _commentCharacter=' ';//set to ' ' for no comments
        readFile(s,header);

    }
    //----------------------------------------------------------------------------------------------
    void readcsv::readFile( const std::string& filePath,bool header ) {
        
        std::cout << "Reading csv file \"" << filePath << "\"..." << std::endl;
        std::ifstream fileStream( filePath.c_str( ), std::ios::in );
        
        if( fileStream.is_open( ) ) {
            
            if (header) readHeader(fileStream);

            readData(fileStream);
           
            fileStream.close( );

        } else {
            std::cout << "File path \"" << filePath << "\" is invalid." << std::endl;
            exit(0);
        }
    }
    //----------------------------------------------------------------------------------------------
    void readcsv::readData(std::ifstream& fileStream){
        std::string readLine;        
        while( std::getline( fileStream, readLine ) ) {
            if (_commentCharacter==' ' || readLine[ 0 ] != _commentCharacter)
                _data.push_back(StringToWords(readLine, ','));
        }
    }
    //----------------------------------------------------------------------------------------------
    void readcsv::readHeader(std:: ifstream& fileStream){
        std::string readLine;
        //StringToWords returns a string split into a vector
        std::cout<<"Reading header data"<<std::endl;
        std::getline( fileStream, readLine );
        if  (_commentCharacter !=' ') while (readLine[ 0 ] == _commentCharacter)std::getline( fileStream, readLine );
        _header=StringToWords(readLine, ',');

    }
    //----------------------------------------------------------------------------------------------
    const std::vector<std::string> readcsv::StringToWords( const std::string& inputString, const char wordTerminationCharacter ) const {
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
    const std::vector<double> readcsv::LineToDouble( const std::string& inputString, const char wordTerminationCharacter ) const {
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
    double readcsv::StringToDouble( const std::string& string ) const {
        // convert an individual string to double
        double number = std::stod( string.c_str( ), NULL );
        
        return number;
    }
    //----------------------------------------------------------------------------------------------
    int readcsv::StringToInt( const std::string& string ) const {
        // convert an individual string to int
        double number = std::stoi( string.c_str( ), NULL );
        
        return number;
    }
    //----------------------------------------------------------------------------------------------
    std::vector<std::string> readcsv::operator[](unsigned i){return _data[i];}
    //----------------------------------------------------------------------------------------------
    std::vector<std::string>& readcsv::getHeader(){return _header;}
    //----------------------------------------------------------------------------------------------
    unsigned readcsv::nrows(){return _data.size();}
