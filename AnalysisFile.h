#ifndef ANALYSISFILE_H
#define ANALYSISFILE_H
#include "TString.h"
#include <iostream>
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;

namespace ExampleFramework {
//! basic class to handle input and output data files
class AnalysisFile{
 private:
   TString fFilename;
   bool fRead; //!< =true is file is read from
   bool fWrite; //!< =true is file is written to
   bool fValid;
   ifstream fIn; //!< file stream object for reading from a file
   ofstream fOut; //!< file stream object for writing to a file
 public:
   AnalysisFile(TString filename, TString accessmode);   
   ~AnalysisFile(){ Close();}
   void Close(); //!< Closes file. 
   bool IsValid(){ return fValid;}
   istream& GetInStream() {return fIn;}  //!< Reference to input file stream
   ostream& GetOutStream() {return fOut;} //!< Reference to output file stream
};

} 
#endif
