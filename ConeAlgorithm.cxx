
#include "ConeAlgorithm.h"

using namespace ExampleFramework;
   

// constructor 
ConeAlgorithm::ConeAlgorithm(Framework* fram)
: JetFinder(fram), fN(50), fR(0.4), fPtCut(150), fFrac(0.8) { }
ConeAlgorithm::ConeAlgorithm(Framework* fram, int N, double R, double pTcut, double f)
: JetFinder(fram), fN(N), fR(R), fPtCut(pTcut), fFrac(f) { }

bool ConeAlgorithm::EventLoop(){
   //getting input Clusters
   const DataCollection* inputs = fStorage->RetrieveData("CaloClusterCollection", TString("DICaloCluster") );
   assert(inputs);
   // create output
   DataCollection* ProtoJetCollection = new DataCollection();
      DataCollection* FinalJetCollection = new DataCollection();

   /*  ------------------------------------------
    * creating vector including all Clusters 
      -------------------------------------------*/
   vector<const DICaloCluster*> Clusters;
   for(int i = 0; i<inputs->size();i++){
      const DICaloCluster* Cluster = dynamic_cast <const DICaloCluster*>((*inputs)[i]);
      Clusters.push_back(Cluster);
   }
   // ------------------------------------------------------

   /*  ------------------------------------------
    * creating the Protojet objects
      -------------------------------------------*/

   vector<const DICaloJet*> ProtoJets;
   while(Clusters.size() != 0){

      const DICaloCluster* HighestEtCluster = HighestEt(Clusters);
      double MidPointEta = HighestEtCluster->GetEta();
      double MidPointPhi = HighestEtCluster->GetPhi();

      vector<const DICaloCluster*> JetClusters;
      for(int i = 0; i < fN; i++){                    //run until "stable" i.e. fN iterations
         for(int j = 0; j < Clusters.size(); j++){
            if(isInside(MidPointEta, MidPointPhi, Clusters[j])){
               JetClusters.push_back(Clusters[j]);
            }
         }

         GetNewMidpoint(MidPointEta, MidPointPhi, JetClusters);
         if(i != fN-1) { JetClusters.clear(); }
      }

      //Remove JetClusters from Clusters and save the ProtoJet
      DICaloJet* ProtoJet = new DICaloJet(CreateUniqueObjectID());
      for(int i = 0; i<JetClusters.size(); i++){
         Clusters.erase(find(Clusters.begin(), Clusters.end(), JetClusters[i]));
         ProtoJet->Add(JetClusters[i]);
      }

      // Filling the ProtoJets into a Collection
      ProtoJets.push_back(ProtoJet);
      ProtoJetCollection->push_back(ProtoJet);
      makeHisto(ProtoJet, "proto");
   }

   SpiltAndMerge(ProtoJets, FinalJetCollection);

   RemoveIntersection(FinalJetCollection, ProtoJetCollection);
   fStorage->StoreData("ProtoJetCollection", ProtoJetCollection);
   fStorage->StoreData(fOutputCollectionName, FinalJetCollection);

   return true;
}



void ConeAlgorithm::SpiltAndMerge(vector<const DICaloJet*> &ProtoJets, DataCollection* FinalJetCollection){
   DataCollection* MergeResults = new DataCollection();

   //remove all Protojects with pt < pTcut
   ProtoJets.erase(remove_if(
      ProtoJets.begin(), ProtoJets.end(),
      [&](const DICaloJet* x) {
         return x->GetKt() < fPtCut;
      } ), ProtoJets.end());

   while(ProtoJets.size() != 0){
      //find Protojet with highest pt
      const DICaloJet* HighestPtJet = ProtoJets[0]; 
      for(int i = 0; i < ProtoJets.size(); i++){
         if(ProtoJets[i]->GetKt() > HighestPtJet->GetKt()){
            HighestPtJet = ProtoJets[i];
         }
      }

      vector<const DataItem*> SharedClusters;
      const DICaloJet* SharedJet = HighestSharedJet(HighestPtJet, ProtoJets, SharedClusters);

      //If the Jet has no SharedJet store it as "real" Jet
      if(SharedClusters.size() == 0){ 
         FinalJetCollection->push_back(HighestPtJet);
         makeHisto(HighestPtJet, "Final");
         ProtoJets.erase(find(ProtoJets.begin(),ProtoJets.end(), HighestPtJet));
      }
      //Else merge the Jets as long as the shared pt is high enough (depending on f)
      else{ 
         if(PtSum(SharedClusters) < fFrac * SharedJet->GetKt()){
            FinalJetCollection->push_back(HighestPtJet);
            makeHisto(HighestPtJet, "Final");
            ProtoJets.erase(find(ProtoJets.begin(),ProtoJets.end(), HighestPtJet));
         }
         else{
            DICaloJet* MergedJet = Merge(HighestPtJet, SharedJet);
            ProtoJets.erase(find(ProtoJets.begin(),ProtoJets.end(), HighestPtJet));
            ProtoJets.erase(find(ProtoJets.begin(),ProtoJets.end(), SharedJet));
            ProtoJets.push_back(MergedJet);
            MergeResults->push_back(MergedJet);
         }
      }

   }

   //Have to remove already stored Jets from collection
   RemoveIntersection(FinalJetCollection, MergeResults);

   fStorage->StoreData("MergedJets", MergeResults);

}

bool ConeAlgorithm::InitRun(){ 
   return true;
}

bool ConeAlgorithm::FinalizeRun(){ 
   return true;
}

const DICaloCluster* ConeAlgorithm::HighestEt(vector<const DICaloCluster*> &Clusters){

   const DICaloCluster* HighestEtCluster= Clusters[0];
   for(int i = 0; i < Clusters.size(); i++){
      if(Clusters[i]->GetEt() > HighestEtCluster->GetEt()){
         HighestEtCluster = Clusters[i];
      }
   }

   return HighestEtCluster;
}

bool ConeAlgorithm::isInside(double MidPointEta, double MidPointPhi, const DICaloCluster* Cluster){
   double Eta = Cluster->GetEta();
   double Phi = Cluster->GetPhi();

   if(pow((Eta-MidPointEta),2) + pow((Phi-MidPointPhi),2) <= fR * fR){
      return true;
   }
   else { return false; }
}

void ConeAlgorithm::GetNewMidpoint(double &MidPointEta, double &MidPointPhi, vector<const DICaloCluster*> &JetClusters){
   MidPointEta = 0;
   MidPointPhi = 0;
   for(int j = 0; j < JetClusters.size(); j++){
      MidPointEta += JetClusters[j]->GetEta();
      MidPointPhi += JetClusters[j]->GetPhi();
   }
   MidPointEta = MidPointEta/JetClusters.size();
   MidPointPhi = MidPointPhi/JetClusters.size();

}

const DICaloJet* ConeAlgorithm::HighestSharedJet(
                                          const DICaloJet* HighestPtJet,
                                          vector<const DICaloJet*> &ProtoJets,
                                          vector<const DataItem*> &SharedClusters){
   const DICaloJet* SharedJet;
   double HighestPt = 0;
   for(int i = 0; i < ProtoJets.size(); i++){
      shared(HighestPtJet, ProtoJets[i], SharedClusters);
      if(ProtoJets[i] != HighestPtJet && SharedClusters.size() != 0){
         if(ProtoJets[i]->GetKt() > HighestPt){
            SharedJet = ProtoJets[i];
            HighestPt = SharedJet->GetKt();
         }
      }
   }

   if(HighestPt == 0){ SharedClusters.clear(); }
   return SharedJet;
}

void ConeAlgorithm::shared(const DICaloJet* HighestPtJet, const DICaloJet* CanidateJet, vector<const DataItem*> &SharedClusters){
   vector<const DataItem*> HighClusters = HighestPtJet->GetCaloClusters();
   vector<const DataItem*> CanClusters  = CanidateJet->GetCaloClusters();

   for(int i = 0; i < HighClusters.size(); i++){
      for(int j = 0; j < CanClusters.size(); j++){
         if(HighClusters[i] == CanClusters[j]){
            SharedClusters.push_back(HighClusters[i]);
         }

      }

   }

}

double ConeAlgorithm::PtSum(vector<const DataItem*> &SharedClusters){
   double pTsum = 0;
   for(int i = 0; i < SharedClusters.size(); i++){
      const DICaloCluster* clust = dynamic_cast<const DICaloCluster*>(SharedClusters[i]);
      pTsum += (clust->GetV()).Pt();
   }
}

void ConeAlgorithm::RemoveIntersection(DataCollection* PrioHigh, DataCollection* PrioLow){

   for(int i = 0; i < PrioHigh->size(); i++){
      const DataItem* Item = (*PrioHigh)[i];
      PrioLow->erase(remove_if(
      PrioLow->begin(), PrioLow->end(),
      [Item](const DataItem* x) {
         return x == Item;
      } ), PrioLow->end());
   }

}

void ConeAlgorithm::makeHisto(const DICaloJet* jet, string name){
    fHistoService->FastFillTH1D(name+"jetEta", jet->GetEta(), 550, -5.5, 5.5);
    fHistoService->FastFillTH1D(name+"jetSize", jet->GetSize(), 50, -0.5, 49.5);
    fHistoService->FastFillTH1D(name+"jetEnergy", jet->GetEnergy(), 5000, 0., 500000.);
    fHistoService->FastFillTH1D(name+"jetLogEnergy", log(jet->GetEnergy()), 250., 0., 25.);
    fHistoService->FastFillTH1D(name+"jetKt", jet->GetKt(), 100, 0., 1000.);
    if(TMath::Abs(jet->GetEta())<1.5){
        fHistoService->FastFillTH2D(name+"JetlogEnergy_size_central", log(jet->GetEnergy()),jet->GetSize(), 100, 0., 25., 20, 0.5 , 20.5);
        fHistoService->FastFillTH1D(name+"JetLogEnergy_centeral", log(jet->GetEnergy()), 250, 0., 25.);
    }
}

