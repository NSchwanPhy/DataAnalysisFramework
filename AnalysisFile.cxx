#include "AnalysisFile.h"

using namespace ExampleFramework;

AnalysisFile::AnalysisFile(TString filename, TString accessmode)
{
   accessmode.ToUpper(); // all uppercase
   fFilename = filename;
   if (accessmode=="READ"){
      fRead=true;
      fWrite=false;
      fIn.open(fFilename);
      fValid = fIn.is_open();
   }
   if (accessmode=="WRITE"){
      fRead=false;
      fWrite=true;
      fOut.open(fFilename);
      fValid = fOut.is_open();
   }
   
}
   
void AnalysisFile::Close()
{  
   if (fRead) fIn.close();
   if (fWrite) fOut.close();
}
