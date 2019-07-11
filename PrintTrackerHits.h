#ifndef PRINTTRACKERHITS_H
#define PRINTTRACKERHITS_H

#include "Processor.h"

namespace ExampleFramework {

/*! Example processor that creates histograms and prints a few events.
   (c) E. von Toerne, U. of Bonn 2009
 */   
class PrintTrackerHits : public Processor {

private:
   
public:
   // constructor 
   PrintTrackerHits(Framework* fram);
   // destructor
   ~PrintTrackerHits(){ }
   
   bool EventLoop();
   
   bool InitRun(){ return true;}   
   
   bool FinalizeRun(){ return true;}

   TString GetName(){ return TString("PrintTrackerHits");}
};

} 
#endif
