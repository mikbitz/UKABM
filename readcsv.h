//Read gridded data from comma delimited text file.
//Note that the file is read one line at a time as a text string
//Each line is stored as a vector of strings
//A header row is assumed by default
//Lines starting # are assumed to be comments...
#ifndef _READCSV_H
#define _READCSV_H
#include <vector>
#include <map>
#include <fstream>
#include <point2D.h>
class readcsv{
    std::vector<std::vector<std::string>> _data;
    std::vector<std::string> _header;
    char _commentCharacter;
    const std::vector<std::string> StringToWords( const std::string& , const char  ) const;
    const std::vector<double> LineToDouble( const std::string& , const char  ) const;
    double StringToDouble( const std::string& string ) const ;
    int StringToInt( const std::string& string ) const;
public:
    readcsv(std::string,bool header=true, bool commented=true);
    std::vector<std::string> operator[](unsigned i){return _data[i];}
    void readFile( const std::string&,bool  );
    void readData(std::ifstream&);
    void readHeader(std::ifstream&);
    std::vector<std::string>& getHeader(){return _header;}
    unsigned nrows(){return _data.size();}

};

#endif
