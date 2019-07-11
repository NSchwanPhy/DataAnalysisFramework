#ifndef TrackerRawHitProcessor_H
#define TrackerRawHitProcessor_H

#include "Processor.h"
#include "ModuleID.h"

namespace ExampleFramework {
 /*!
   conversion of DIRawHits to DITrackerHits
   (c) E. von Toerne, U. of Bonn 2011
 */    
class TrackerRawHitProcessor : public Processor {
 private:
   ModuleID fModuleTranslator;

 public:
   
   // constructor 
   TrackerRawHitProcessor(Framework* fram);

   // destructor
   ~TrackerRawHitProcessor(){ }

   bool EventLoop();   //!< is executed in every event

   bool InitRun();     //!< is executed at begin of job

   bool FinalizeRun(); //!< is executed at end of job

   TString GetName(){ return TString("TrackerRawHitProcessor");}

};  // end class TrackerRawHitProcessor


}   // end namespace ExampleFramework

#endif // end TrackerRawHitProcessor_H
