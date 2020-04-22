//Read gridded data from and ESRI ascii raster grid file and store in a vector.
//Note that the file is read one line at a time as a text string, then converted to numeric format as appropriate
//The data is stored with the first line being the top (Northernmost) row of the raster grid, listed west-to-east along the rows
//Data is assumed regularly gridded (i.e. same grid cell size in x and y directions)
#ifndef _ASCIIGRID_H
#define _ASCIIGRID_H
#include <vector>
#include <fstream>
#include <point2D.h>
class asciiGrid{
    int _ncols;
    int _nrows;
    double _xllcorner;
    double _yllcorner;
    double _cellSize;
    double _NODATA_value;
    std::vector<std::vector<double>> _data;

public:
    std::vector<unsigned> _index; // stores and index of valid cells (i.e. those that are not NODATA)
    asciiGrid()= default;
    asciiGrid(std::string);
    void readFile( const std::string&  );
    void readData(std::ifstream& );
    void readHeader(std::ifstream& );
    const std::vector<std::string> StringToWords( const std::string& , const char  ) const;
    const std::vector<double> LineToDouble( const std::string& , const char  ) const;
    double StringToDouble( const std::string& string ) const ;
    int StringToInt( const std::string& string ) const;
    point2D getValidPoint(unsigned);
    point2D getValidRandomisedPoint(unsigned);
    double getDataAt(unsigned);
    bool isValid(unsigned);
    double xOrigin();
    double yOrigin();
    double xSize();
    double ySize();
};

#endif
