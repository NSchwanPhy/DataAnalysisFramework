#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <vector>
#include "stdlib.h"
using namespace std;

#include "TString.h"
#include "CentralStorage.h"
#include "AnalysisFile.h"
#include "PropertyHandler.h"
#include "HistogramService.h"
 
namespace ExampleFramework {
   
class Framework; // forward declaration

/*! 
  Base class of all processors. 
  (c) Eckhard von Toerne, University of Bonn, April 2009
*/
class Processor : public PropertyHandler {

   friend class Framework;

 private:
   int         fID;             //!< unique processor ID
   Framework*  fFramework;      //!< pointer to framework
   bool        fIDBeenSet;      //!< flag if ID has been set 
   TString     fProcessorTitle; //!< Title of Processor set by user. Default Title=Name 
   TString     fProcessorName;  //!< Class Name of processor 

public:   
   //! default constructor 
   Processor(const char* title, Framework* fram);
   virtual ~Processor(){ }  //!< virtual destructor of BaseClass
   virtual bool EventLoop()=0;  //!< virtual functions to process one event   
   virtual bool InitRun()=0;    //!< virtual function invoked at begin of job   
   virtual bool FinalizeRun()=0;//!< virtual function invoked at end of job
   virtual TString GetName()=0; //!< Get class name
   virtual void SetTitle(const char* title) final;
   virtual const TString& GetTitle() final { return fProcessorTitle;} //!< title of individual processor object, default is class name.
protected:
   CentralStorage*   fStorage;  //!< pointer to CentralStorage object 
   HistogramService* fHistoService;    //!< pointer to histogramservice
   TString           fInputCollectionName; //!< Name of input collection 
   TString           fOutputCollectionName; //!< Name of output collection 
   int               CreateUniqueObjectID(); //!< Obtain running object number from framework. This number is unique for the given event
   bool SetUp(int ID, CentralStorage* storage, HistogramService* hist);
   void ReportError(const char* typ, const char* option=""); //!< Print error then throws exception using exit();
   int GetEventNumber() const; //!< Easy access to event number

private:
   

};

} 
#endif
