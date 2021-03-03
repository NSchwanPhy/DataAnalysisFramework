#ifndef ConeAlgorithm_H
#define ConeAlgorithm_H

#include "Processor.h"
#include "JetFinder.h"
#include "DICaloCluster.h"
#include "DICaloJet.h"
#include "TVector3.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <map>

namespace ExampleFramework {
/*! 
  A simple implenention of a SISCone like algorithm which uses the DICaloClusters (derived in CaloClusterer) to create DICaloJet objects.
  Based on the pseudorapidity, the azimutal angle and four user defined parameters
  N (amount of iterations for the proto jets), R (the Cone radius), Frac (Fraction of pT that can be shared between to jets)
  as well as pTcut (only proto jets with pT higher then pTcut will be final jets).
  Based on: Jets and jet algorithms from Victor Coco, Pierre-Antoine Delsart, Juan Rojo, Christian Sander Gregory Soyez
  and Introduction to Jet Reconstruction Algorithms from Ryan Atkin
*/   
class ConeAlgorithm : public JetFinder {
 private:

  int fN;        //! amount of iterations for the proto jets
  double fR;     //! Cone radius
  double fPtCut; //! only proto jets with pT higher then pTcut will be final jets
  double fFrac;  //! Fraction of pT that can be shared between to jets

 public:
   
   // constructor 
   ConeAlgorithm(Framework* fram);
   ConeAlgorithm(Framework* fram, int N, double R, double pTcut, double f);


   // destructor
   ~ConeAlgorithm(){ }

   bool EventLoop();   //!< is executed in every event
   bool InitRun();     //!< is executed at begin of job
   bool FinalizeRun(); //!< is executed at end of job

   void makeHisto(const DICaloJet* jet, string name);

   void SpiltAndMerge(vector<const DICaloJet*> &ProtoJets, DataCollection* FinalJetCollection); //! finials the Jets depending on there overlap
   const DICaloJet* HighestSharedJet(        
                              const DICaloJet* HighestPtJet,
                              vector<const DICaloJet*> &ProtoJets,
                              vector<const DataItem*> &SharedClusters); //! find Protojet with highest pt that shares Clusters with HighestPtJet (SharedJet)
   void shared(const DICaloJet* HighestPtJet, const DICaloJet* CanidateJet, vector<const DataItem*> &SharedClusters); //! Gets the SharedClusters
   double PtSum(vector<const DataItem*> &SharedClusters);

   const DICaloCluster* HighestEt(vector<const DICaloCluster*> &Clusters); //! Returns the Cluster with the highest transfers Energy
   bool isInside(double MidPointEta, double MidPointPhi, const DICaloCluster* Cluster); //! Is the Cluster inside the Radius?
   void GetNewMidpoint(double &MidPointEta, double &MidPointPhi, vector<const DICaloCluster*> &JetClusters); //! Sets the new Midpoint

   void RemoveIntersection(DataCollection* PrioHigh, DataCollection* PrioLow);

   TString GetName(){ return TString("ConeAlgorithm");}
};  // end class ConeAlgorithm


}   // end namespace ExampleFramework

#endif // end ConeAlgorithm_H
