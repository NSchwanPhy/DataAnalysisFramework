#ifndef CaloClusterer_H
#define CaloClusterer_H

#include "DICalorimeterHit.h"
#include "DICaloCluster.h"
#include "Processor.h"

namespace ExampleFramework {

//! bool function to sort CaloHits in descending energy order
inline bool compareCalorimeterHitEnergy(DICalorimeterHit const *h1, DICalorimeterHit const *h2) {
   return (h1->GetEnergy()>h2->GetEnergy()); 
}

/*!
  Combines all CalorimeterHits in InputDataCollection into DICaloCluster objects and stores them into a new DataCollection. Name of output collection set by property OutputDataCollection
 */
class CaloClusterer : public Processor {
 private:

   double fClusterRadius;
   double fSeedThreshold;
 public:
   
   // constructor 
   CaloClusterer(Framework* fram);

   // destructor
   ~CaloClusterer(){ }

   bool EventLoop();   //!< is executed in every event

   bool InitRun();     //!< is executed at begin of job

   bool FinalizeRun(); //!< is executed at end of job

   TString GetName(){ return TString("CaloClusterer");}

};  // end class CaloClusterer


}   // end namespace ExampleFramework

#endif // end CaloClusterer_H
