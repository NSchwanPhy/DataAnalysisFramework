#ifndef ExampleTrackClusterMatcher_H
#define ExampleTrackClusterMatcher_H

#include "Processor.h"

namespace ExampleFramework {
/*! 
  Example Processor, please add decription of processor here
*/   
class ExampleTrackClusterMatcher : public Processor {
 private:
   TString fInputCollectionName2;
 public:
   
   // constructor 
   ExampleTrackClusterMatcher(Framework* fram);

   // destructor
   ~ExampleTrackClusterMatcher(){ }

   bool EventLoop();   //!< is executed in every event

   bool InitRun();     //!< is executed at begin of job

   bool FinalizeRun(); //!< is executed at end of job

   TString GetName(){ return TString("ExampleTrackClusterMatcher");}
};  // end class ExampleTrackClusterMatcher


}   // end namespace ExampleFramework

#endif // end ExampleTrackClusterMatcher_H
