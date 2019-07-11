#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <iostream>
#include <vector>
#include "stdlib.h"

using namespace std;

#include "TString.h"
#include "CentralStorage.h"
#include "Processor.h"
#include "AnalysisFile.h"
#include "HistogramService.h"


namespace ExampleFramework {

/*! 
  Example class for a data processing framework. This class controls the complete project. It invokes processors to analyse individual evens and supports the processors via services, such as data storage (CentralStorage) or histogramming (HistogramService)
  (c) Eckhard von Toerne, University of Bonn, April 2009
*/
class Framework{

private:
   int                 fAnalysisID; //!< the ID of this analysis job.
   int                 fUniqueIDCounter; //!< Stores current running unique id.
   int                 fCurrentEventNumber;  
   vector<Processor*>  fProcessors; //!< Array of processors. Implemented as array of pointer to processor base class. Example on how to use polymorphism. This allows us to use processors without refering to specific processor classes. 
   // Inputs
   vector<TString>     fInputList; //!< list of input files
   AnalysisFile*       fCurrentInput; //!< pointer to current input file
   TString             fOutputname; //!< name of output file
   bool                fSuppressWriteOut; //!<!if true no data written to output.dat
   bool                fVerbose;  // true by default, 
   // services
   CentralStorage*     fStorage; //!< storage: data management and output
   HistogramService*   fHistogramService;

public:
   //! main constructor with one input file
   Framework(TString infile, TString outfile, int jobID=0); 
   //! main constructor multiple input files
   Framework(vector<TString>& inlist, TString outfile, int jobID=0); 
   //! copy constructor, dysfunctional, throws an error.
   Framework(const Framework& a){
      cout << "ERROR, not meant to be copied, exiting..."<<endl; exit(1);} 
   //! assignment operator, dysfunctional, throws an error.
   Framework& operator =(const Framework& a){
      cout << "ERROR, not meant to be assigned, exiting..."<<endl; exit(1);}
   ~Framework();    // destructor
   bool AddProcessor(Processor* pro); //!< Adds a processor to the execution list. First add all processors then execute them with ProcessAll(). 
   int GetUniqueObjectID(){return fUniqueIDCounter++;} //!< used by processor to obtain event-ids for DataItems.
   void GenerateEvents(int nevt); //!< For Monte Carlo generation
   void ProcessAll(); //!< Starts processing of all events specified in input list;
   //! If called no output data file is written. The only output is the histogram file.
   void SuppressWriteOut(){fSuppressWriteOut=true;}
   void Silence(){fVerbose=false;}
   Service* GetService(TString& name);
private:
   bool ReadNextEvent();
   void ResetIDCounter(int val = 1){fUniqueIDCounter=val;}
   void ProcessOneEvent();
   void Finalize();
   void Init();
};

} 
#endif
