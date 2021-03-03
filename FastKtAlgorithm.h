#ifndef FastKtAlgorithm_H
#define FastKtAlgorithm_H

#include "Processor.h"
#include "JetFinder.h"
#include "DICaloJet.h"
#include <map>
#include <assert.h>

//typedef pair<DICaloJet* , DICaloJet*> JetPair;
//typedef map<JetPair, double> M;

namespace ExampleFramework {
/*! 
  A Faster implimentaion of the kt algorithm
  (based on: Dispelling the N^3 myth for the kt jet-finder from Matteo Cacciari and Gavin P. Salam)
*/   
class FastKtAlgorithm : public JetFinder {
 private:

 public:
   
   // constructor 
   FastKtAlgorithm(Framework* fram);

   // destructor
   ~FastKtAlgorithm(){ }

   bool EventLoop();   //!< is executed in every event
   bool InitRun();     //!< is executed at begin of job
   bool FinalizeRun(); //!< is executed at end of job
   //void InitDistance(M& distance, vector<const DICaloJet*>& Jets);

   void makeHisto(const DICaloJet* jet);

   TString GetName(){ return TString("FastKtAlgorithm");}
};  // end class FastKtAlgorithm


}   // end namespace ExampleFramework

#endif // end FastKtAlgorithm_H
