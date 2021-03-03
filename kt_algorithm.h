#ifndef kt_algorithm_H
#define kt_algorithm_H

#include "Processor.h"
#include "JetFinder.h"
#include "DICaloCluster.h"
#include "DICaloJet.h"
#include "TVector3.h"
#include <map>
#include <algorithm>
#include <string>
#include <assert.h>
#include <iostream>

namespace ExampleFramework {
/*! 
  The kt algorithm which uses the DICaloClusters (derived in CaloClusterer) to create DICaloJet objects.
  Based an the transverse momentum and the DeltaR between two Clusters.
  (based on: Dispelling the N^3 myth for the kt jet-finder from Matteo Cacciari and Gavin P. Salam)
*/   
class kt_algorithm : public JetFinder {
 private:

 public:
   
   // constructor 
    kt_algorithm(Framework* fram);

    ~kt_algorithm() {}

    bool EventLoop();
    bool InitRun();
    bool FinalizeRun();

    void makeHisto(const DICaloJet* jet);
    TString GetName(){ return TString("kt_algorithm");}

};  // end class kt_algorithm


}   // end namespace ExampleFramework

#endif // end kt_algorithm_H
