#ifndef OUTPUTS_H
#define OUTPUTS_H
#include <vector>
#include<fstream>
#include "asciiGridFileWriter.h"
class outputs{
    asciiGridFileWriter* _infections;
    asciiGridFileWriter* _population;
public:
    outputs();
    ~outputs();
    void writeAll();
    void test();
private:
    std::ofstream _summaryFile;
    double _outputCellSize;
};
#endif
