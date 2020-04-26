#include "asciiGridFileWriter.h"
#include <iostream>
#include <assert.h>
    asciiGridFileWriter::asciiGridFileWriter(const std::string& filePath,const std::string& fileName,unsigned ncols,unsigned nrows,double xllc,double yllc,double cellSize,double nodata):
    _ncols(ncols),
    _nrows(nrows),
    _xllcorner(xllc),
    _yllcorner(yllc),
    _cellSize(cellSize),
    _NODATA_value(nodata),
    _filePath(filePath)
    {
        _fileStream.open( filePath+fileName);
        std::cout << "Writing map output to file \"" << fileName << "\"..." << std::endl;
        if(_fileStream.is_open( ) ){
            writeHeader(_fileStream);
        }else {
            std::cout << "File path \"" << filePath << "\" is invalid." << std::endl;
            exit(2);
        }
    }
    //----------------------------------------------------------------------------------------------
    asciiGridFileWriter::~asciiGridFileWriter(){
        if(_fileStream.is_open( ) )_fileStream.close( );
    }
    //----------------------------------------------------------------------------------------------
    void asciiGridFileWriter::writeToFile(const std::vector<std::vector<double>>& regularGrid ) {
        //data assumed stored with x-direction as first index...
        assert(regularGrid.size()==_ncols && regularGrid[0].size()==_nrows);
        //f<<date<<endl;
        //data is assumed stored South to North, but is written by convention North to South
        //i.e. top-left corner first
        if( _fileStream.is_open( ) ) {
            for (int j=_nrows-1;j>= 0;j--){
                for (int i=0;i<(int)_ncols;i++){
                    _fileStream<<regularGrid[i][j]<<" ";    
                }
                _fileStream<<std::endl;
            }
        } else {
            std::cout << "File is "<<_filePath<<" is invalid." << std::endl;
        }
        
    }
    //---------------------------------------------------------------------------------------------- 
    void asciiGridFileWriter::writeExtraLabel(const std::string& s){
        _fileStream<<s<<std::endl;
    }
    //----------------------------------------------------------------------------------------------
    void asciiGridFileWriter::writeHeader(std::ofstream& fileStream){
        std::string readLine;
        //ascii header has the form
        //ncols 36
        //nrows 13 
        //xllcorner -180
        //yllcorner  -65
        //cellsize  10
        //NODATA_value -9999
        fileStream<<"ncols "<<_ncols<<std::endl;
        fileStream<<"nrows "<<_nrows<<std::endl;
        fileStream<<"xllcorner "<<_xllcorner<<std::endl;
        fileStream<<"yllcorner "<<_yllcorner<<std::endl;
        fileStream<<"cellsize "<< _cellSize<<std::endl;
    }
