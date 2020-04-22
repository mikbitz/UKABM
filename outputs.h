#ifndef OUTPUTS_H
#define OUTPUTS_H
#include <vector>
#include<fstream>
#include "asciiGridFileWriter.h"
class outputs{
    asciiGridFileWriter* _infections;
public:
    outputs();
    ~outputs();
    void writeAll();
    void test();
private:
    std::ofstream _summaryFile;
};
#endif
