#ifndef _ASCIIGRIDFILEWRITER_H
#define _ASCIIGRIDFILEWRITER_H
#include <vector>
#include <fstream>
class asciiGridFileWriter{
    unsigned _ncols;
    unsigned _nrows;
    double _xllcorner;
    double _yllcorner;
    double _cellSize;
    double _NODATA_value;
    std::ofstream _fileStream;
    std::string _filePath;
public:
    asciiGridFileWriter(const std::string& filePath,unsigned ncols, unsigned nrows,double xllc,double yllc,double cellSize,double nodata);
    ~asciiGridFileWriter();
    void writeToFile(const std::vector<std::vector<double>>&  );
    void writeHeader(std::ofstream&);
    void writeExtraLabel(const std::string&);
};

#endif
