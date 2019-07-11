
#include "ExampleTrackClusterMatcher.h"
#include "DITrack.h"
#include "DICaloCluster.h"
using namespace ExampleFramework;
   

// constructor 
ExampleTrackClusterMatcher::ExampleTrackClusterMatcher(Framework* fram) 
  : Processor("ExampleTrackClusterMatcher", fram)
{
   DefineProperty("InputCollection", &fInputCollectionName, "TrackCollection");
   DefineProperty("InputCollection2", &fInputCollectionName2, "CaloClusterCollection");
   DefineProperty("OutputCollection", &fOutputCollectionName, "none");
}

bool ExampleTrackClusterMatcher::EventLoop(){
   const DataCollection* trackinputs = fStorage->RetrieveData(fInputCollectionName,"donotcare");
   const DataCollection* clusterinputs = fStorage->RetrieveData(fInputCollectionName2,"donotcare");
   // loop over tracks
   int ntrack = trackinputs->size();
   int nclus = clusterinputs->size();
   for (int itrk = 0;itrk<ntrack; itrk++){
      const DITrack* trackpoi = Get<DITrack>(trackinputs,itrk);
      // alternatively access tracks this way:        
      // const DITrack* trackpoi =   dynamic_cast< const DITrack* >((*trackinputs)[itrk]);
      TVector3 ptrack = trackpoi->GetMomentumAtPCA();    
      // for each track find the nearest cluster in deltaR
      float drmin = 10000.;
      // loop over cluster
      const DICaloCluster* matchedpoi=0;
      for (int iclus=0; iclus<nclus; iclus++){
         const DICaloCluster* cluspoi = Get<DICaloCluster>(clusterinputs,iclus);         
         TVector3 pclus = cluspoi->GetMomentum();   
         float dr = ptrack.DeltaR(pclus);
         if (dr<drmin){
            drmin=dr;
            matchedpoi=cluspoi;
         }      
      }
      if (!matchedpoi) continue;
      fHistoService->FastFillTH1D("dr to nearest track", drmin, 200, 0., 2.);      
   }
   return true;
}

bool ExampleTrackClusterMatcher::InitRun(){ 
   return true;
}

bool ExampleTrackClusterMatcher::FinalizeRun(){ 
   return true;
}

