#ifndef SiliconClusterer_H
#define SiliconClusterer_H

#include "Processor.h"

namespace ExampleFramework {
/*!
  Provides Cluster of TrackerHits. Running the track finder on ClusteredHits improves resolution and reduces ghost tracks.
   (c) E. von Toerne, U. of Bonn 2011
 */   
class SiliconClusterer : public Processor {
 private:
   double fSeedThreshold; // Threshold to start a cluster
   double fClusterRadius; // max difference in deltaR from c.o.g. to next hit  
 public:   
   // constructor 
   SiliconClusterer(Framework* fram);
   // destructor
   ~SiliconClusterer(){ }
   bool EventLoop();   //!< is executed in every event
   bool InitRun();     //!< is executed at begin of job
   bool FinalizeRun(); //!< is executed at end of job
   TString GetName(){ return TString("SiliconClusterer");}
};  // end class SiliconClusterer

}   // end namespace ExampleFramework

#endif // end SiliconClusterer_H
