
#include "SiliconClusterer.h"
#include "DITrackerHit.h"
#include "DIClusteredTrackerHit.h"
#include "TString.h"
#include <list>

using namespace ExampleFramework;
// constructor 
SiliconClusterer::SiliconClusterer(Framework* fram) 
  : Processor("SiliconClusterer", fram)
{
   DefineProperty(TString("InputCollection"), 
                  &fInputCollectionName, TString("TrackerHitCollection"));
   DefineProperty(TString("OutputCollection"), 
                  &fOutputCollectionName, TString("ClusteredTrackerHitCollection"));
   DefineProperty("SeedThreshold", &fSeedThreshold, 2.);
   DefineProperty("ClusterRadius", &fClusterRadius, 0.1);
}

bool SiliconClusterer::EventLoop(){
   const DataCollection* inputs = fStorage->RetrieveData(fInputCollectionName,"donotcare");
   // create output
   DataCollection* myOutCollection = new DataCollection();
   // create list to hold hits
   list<const DITrackerHit*> hlist;
   // fill list with hits
   DataCollection::const_iterator it; // here we need const_iterator since the collection itself is const
   for (it = inputs->begin(); it != inputs->end(); it++){
      const DITrackerHit* hit = dynamic_cast< const DITrackerHit* >(*it);
      hlist.push_back(hit);
   }
   
   list<const DITrackerHit*>::iterator hp, seed;
   
   while (hlist.size()>0){
      double maxenergy =0.;
      // find best seed hit
      for (hp = hlist.begin(); hp!=hlist.end(); hp++){
         if ((*hp)->GetPulseHeight() > maxenergy){
            maxenergy = (*hp)->GetPulseHeight();
            seed = hp;
         }
      }
      // break if maxenergy is below threshold
      if (maxenergy<fSeedThreshold) break;
      
      // create cluster
      DIClusteredTrackerHit* clus = new DIClusteredTrackerHit(CreateUniqueObjectID());
      clus->Add(*seed);
      int seedLayer=(*seed)->GetLayer();
      const DITrackerHit* seedHit = (*seed); // the iterator seed is a pointer to the STL entry, thus a pointer to a pointer. Must dereference it once to obtain a pointer
      hlist.erase(seed);
      hp = hlist.begin();
      while (hp != hlist.end()){      // find close hits in dR in the same layer
         if (seedLayer == (*hp)->GetLayer()){
            //double dR = clus->GetV().DeltaR((*hp)->GetV());
            double dR = seedHit->GetV().DeltaR((*hp)->GetV());
            fHistoService->FastFillTH1D("dR", dR, 100, 0., 10.*fClusterRadius);
            if (dR < fClusterRadius){
               clus->Add(*hp);  // add hit to cluster
               hp =  hlist.erase(hp); // erase returns next valid iterator
            }
            else hp++;
         }
         else hp++;
      }
      fHistoService->FastFillTH1D("ClusterCharge", clus->GetPulseHeight(), 200, 0., 2000.);
      fHistoService->FastFillTH1D("ClusterChargeZoom", clus->GetPulseHeight(), 200, 0., 10*fSeedThreshold);
      fHistoService->FastFillTH1D("ClusterWidth", clus->GetSize(), 25, -0.5, 24.5);

      myOutCollection->push_back(clus); // store cluster   
   }

   fHistoService->FastFillTH1D("NClusterZoom", myOutCollection->size(), 50, -0.5, 49.5);
   fHistoService->FastFillTH1D("NCluster", myOutCollection->size(), 2000, -0.5, 1999.5);
   // store output collection 
   fStorage->StoreData(fOutputCollectionName,myOutCollection);
   return true;
}

bool SiliconClusterer::InitRun(){ 
   return true;
}

bool SiliconClusterer::FinalizeRun(){ 
   return true;
}

