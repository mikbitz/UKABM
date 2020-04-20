#ifndef _ASCIIGRIDFILEWRITER_H
#define _ASCIIGRIDFILEWRITER_H
#include <vector>
#include <fstream>
class asciiGridFileWriter{
    int _ncols;
    int _nrows;
    double _xllcorner;
    double _yllcorner;
    double _cellSize;
    double _NODATA_value;
    asciiGridFileWriter(){
        readFile("gbr_ppp_2020.asc");
    }
    void readFile( const std::string& filePath ) {
        bool _verbose=false;
//        bool success = false;
        
        std::cout << "Reading text file \"" << filePath << "\"..." << std::endl;
        std::ofstream fileStream( filePath.c_str( ), std::ios::in );
        writeHeader();
        if( fileStream.is_open( ) ) {
    for (int la=NumLat-1;la>=0;la--){
     for (int lo=0;lo<NumLon;lo++){
              Biomaout<<endl;
     }
    }

            fileStream.close( );
            
        } else {
            std::cout << "File path \"" << filePath << "\" is invalid." << std::endl;
        }
        //return success;
        
    }
    //----------------------------------------------------------------------------------------------
    void writeHeader(ifstream& fileStream){
            std::string readLine;
            //ascii header has the form
            //ncols 36
            //nrows 13 
            //xllcorner -180
            //yllcorner  -65
            //cellsize  10
            //NODATA_value -9999
    fileStream<<"ncols "<<NumLon<<endl;
    fileStream<<"nrows "<<NumLat<<endl;
    fileStream<<"xllcorner     -180.0"<<endl;
    fileStream<<"yllcorner     -65.0"<<endl;
    fileStream<<"cellsize "<< Parameters::Get( )->GetGridCellSize( )<<endl;
}
};
/*
    void Madingley::AsciiOutput( unsigned step ) {
    stringstream A,B;
    string fAname,fBname;
    A<<"output/Abundance_SqKm_"<<(step+1)<<".asc";
    B<<"output/BiomassKg_SqKm_"<<(step+1)<<".asc";
    A>>fAname;
    B>>fBname;
    ofstream Abundout(fAname.c_str());
    Abundout.precision(15);
    ofstream Biomaout(fBname.c_str());
    Biomaout.precision(15);
    const unsigned int NumLon = Parameters::Get( )->GetLengthUserLongitudeArray( );
    const unsigned int NumLat = Parameters::Get( )->GetLengthUserLatitudeArray( );
    Abundout<<"ncols "<<NumLon<<endl;
    Abundout<<"nrows "<<NumLat<<endl;
    Abundout<<"xllcorner     -180.0"<<endl;
    Abundout<<"yllcorner     -65.0"<<endl;
    Abundout<<"cellsize "<< Parameters::Get( )->GetGridCellSize( )<<endl;
    Abundout<<"NODATA_value  -9999"<<endl;

    for (int la=NumLat-1;la>=0;la--){
     for (int lo=0;lo<NumLon;lo++){
      int ind=   lo + NumLon * la;
      double totalAbundance=0.;
      double totalCohortBiomass=0.;
      double totalStockBiomass=0;
      GridCell gcl=mModelGrid.mCells[ind];
      gcl.ApplyFunctionToAllCohorts( [&]( Cohort& c ) {
          totalAbundance += c.mCohortAbundance/gcl.GetCellArea();
          double CohortBiomass = ( c.mIndividualBodyMass + c.mIndividualReproductivePotentialMass ) * c.mCohortAbundance / 1000.;
          totalCohortBiomass += CohortBiomass/gcl.GetCellArea();
        } );

      gcl.ApplyFunctionToAllStocks( [&]( Stock & s ) {
         totalStockBiomass += s.mTotalBiomass / 1000./gcl.GetCellArea(); //convert from g to kg
      } );
      Abundout<<totalAbundance<<" ";
      Biomaout<<totalCohortBiomass<<" ";

     }
     Abundout<<endl;
     Biomaout<<endl;
     }
    }

        _Headings = StringToWords( readLine, " " );

        while( std::getline( fileStream, readLine ) ) {
            if( readLine[ 0 ] != "#" ) {
                auto descriptor = ( StringToWords( readLine, Constants::cDataDelimiterValue ) );
                _DataDescriptor["InternalName"]       = descriptor[0];
                _DataDescriptor["FilePath"]           = descriptor[1];
                _DataDescriptor["DefaultVariableName"]= descriptor[2];
                _FileDescriptor.push_back(_DataDescriptor);
            }
        }

    template< class T >
    const std::string ToString( const T& input ) const {
        std::stringstream stringStream;
        stringStream << input;

        return stringStream.str( );
    }
    
    double StringToNumber( const std::string& ) const;

    const std::vector<std::string> StringToWords( const std::string&, const char ) const;
    const std::string DoubleToPrecisionString( const double&, const unsigned& ) const;

    std::string ToLowercase( const std::string ) const;
    std::string RemoveWhiteSpace( const std::string ) const;
    */
#endif
