#include "DICalorimeterHit.h"
#include "CaloClusterer.h"
#include <list>
#include "math.h"
using namespace ExampleFramework;
   

// constructor 
CaloClusterer::CaloClusterer(Framework* fram) 
  : Processor("CaloClusterer", fram)
{
   DefineProperty("InputCollection", 
                  &fInputCollectionName, "CalorimeterHitCollection");
   DefineProperty("OutputCollection", 
                  &fOutputCollectionName, "CaloClusterCollection");


   DefineProperty("ClusterRadius", &fClusterRadius, 0.07);
   DefineProperty("SeedThreshold", &fSeedThreshold, 800.);
}

bool CaloClusterer::EventLoop(){
   const DataCollection* inputs = fStorage->RetrieveData(fInputCollectionName,"donotcare");
   // create output
   DataCollection* myOutCollection = new DataCollection();
   // create list to hold hits
   list<const DICalorimeterHit*> hlist;
   // fill list with hits
   DataCollection::const_iterator it;
   for (it = inputs->begin(); it != inputs->end(); it++){
      const DICalorimeterHit* hit = dynamic_cast< const DICalorimeterHit* >(*it);
      hlist.push_back(hit);
   }

   list<const DICalorimeterHit*>::iterator seedIter,hp;
   hlist.sort(compareCalorimeterHitEnergy);
   while (hlist.size()>0){
      seedIter=hlist.begin();
      double maxenergy =(*seedIter)->GetEnergy();
      fHistoService->FastFillTH1D("SeedEnergy",maxenergy,500,0.,10000.);
      // break if maxenergy is below threshold
      if (maxenergy<fSeedThreshold) break;      
      // create cluster
      DICaloCluster* clus = new DICaloCluster(CreateUniqueObjectID());
      clus->Add(*seedIter);
      hlist.erase(seedIter);      

      hp = hlist.begin();
      while (hp != hlist.end()){       // find close hits in dR 
         double dR = clus->GetV().DeltaR((*hp)->GetV());
         if (dR < fClusterRadius){
            // add hit to cluster
            clus->Add(*hp);
            hp = hlist.erase(hp); // erase returns next valid iterator        
         }
         else hp++;
      }
      // store cluster 
      myOutCollection->push_back(clus);   
      // histograms
      fHistoService->FastFillTH1D("clusterEta", clus->GetEta(), 550, -5.5, 5.5);
      fHistoService->FastFillTH1D("clusterSize", clus->GetSize(), 50, -0.5, 49.5);
      fHistoService->FastFillTH1D("clusterEnergy", clus->GetEnergy(), 5000, 0., 500000.);
      fHistoService->FastFillTH1D("clusterLogEnergy", log(clus->GetEnergy()), 250, 0., 25.);
      fHistoService->FastFillTH1D("clusterLambda", clus->GetLambda(), 160, 0., 8.);
      fHistoService->FastFillTH1D("clusterL90", clus->GetLayerQuantile(0.9), 90, 0., 9.);
      if (clus->GetSize()>3) fHistoService->FastFillTH1D("clusterL90 size>3", clus->GetLayerQuantile(0.9), 90, 0., 9.);
      fHistoService->FastFillTH2D("cluster_lambda_logenergy", clus->GetLambda(),log(clus->GetEnergy()), 160, 0., 8., 100, 0., 25.);
      if (TMath::Abs(clus->GetEta())<1.5){
         fHistoService->FastFillTH2D("clusterlogEnergy_size_central", log(clus->GetEnergy()),clus->GetSize(), 100, 0., 25., 20, 0.5 , 20.5);
         fHistoService->FastFillTH1D("clusterlogEnergy_central", log(clus->GetEnergy()), 250, 0., 25.);
         fHistoService->FastFillTH1D("clusterLambda_central", clus->GetLambda(), 150, 0., 15.);
         fHistoService->FastFillTH2D("cluster_lambda_energy_central", clus->GetLambda(),log(clus->GetEnergy()), 160, 0., 8., 100, 0., 25.);
      }
   } // go back to finding next seed
   //cout << "CaloClusterer: finalizing cluster finding, there are "<<hlist.size()<<" hits left."<<endl;
   //cout << "found "<<myOutCollection->size()<<" clusters"<<endl;
   // store output collection 
   fStorage->StoreData(fOutputCollectionName,myOutCollection);
   fHistoService->FastFillTH1D("Ncluster",myOutCollection->size(), 150, -0.5, 149.5);
   return true;
}

bool CaloClusterer::InitRun(){ 
   return true;
}

bool CaloClusterer::FinalizeRun(){ 
   return true;
}

