
#include "FastKtAlgorithm.h"

using namespace ExampleFramework;

typedef pair<const DICaloJet* ,const DICaloJet*> JetPair;
typedef map<JetPair, double> M;
   

// constructor 
FastKtAlgorithm::FastKtAlgorithm(Framework* fram) : JetFinder(fram) { }

bool FastKtAlgorithm::EventLoop(){
   //setting Output Collections
   DataCollection* myOutCollection = new DataCollection();
   DataCollection* oldJets = new DataCollection();
   DICaloJet* emptyJet = new DICaloJet(CreateUniqueObjectID());

   const DataCollection* inputs = fStorage->RetrieveData("CaloClusterCollection", TString("DICaloCluster"));
   assert(inputs);

   /*  ------------------------------------------
    * creating vector including all Clusters as inital def. of Jets
      -------------------------------------------*/    

   vector<const DICaloJet*> Jets;
   for(int i = 0; i<inputs->size();i++){                                                      
      DICaloJet* jet = new DICaloJet(CreateUniqueObjectID());
      jet->Add((*inputs)[i]);
      Jets.push_back(jet);
   }

   // ------------------------------------------------------

      /*  ------------------------------------------
       * calculation of all beam and particle distances
       -------------------------------------------*/   
   
   M distance;
   for(int i=0; i < Jets.size(); i++){
      const DICaloJet* iJet = Jets[i];
      pair<const DICaloJet* , const DICaloJet*> BeamP(iJet, emptyJet);
      distance.insert(pair<JetPair, double>(BeamP, iJet->GetKt() * iJet->GetKt()));                     //calulating beam disance as transferse momentum squared

      for(int j= i+1; j < Jets.size(); j++){
         const DICaloJet* jJet = Jets[j];
         JetPair PariticleP(iJet,jJet);           
         distance.insert(pair<JetPair, double>(PariticleP, iJet->GetDij(jJet->GetV())));               //calulating particle disance as min(kti*kti ,ktj*ktj) * DeltaR
      }
   }
   // -------------------------------------------------

   M::iterator itMin;
   while(Jets.size() != 0){

      itMin = min_element(distance.begin(), distance.end(), valueComparer);                            // Getting the min Element of the map (distance)

        /*  ------------------------------------------
        *  If the closed object is the Beam
            -------------------------------------------*/

      if(itMin->first.second == emptyJet){

         myOutCollection->push_back(itMin->first.first);                                           // store Jet in final collection
         makeHisto(itMin->first.first);

         int positionOfPointer = std::distance(Jets.begin(), find(Jets.begin(), Jets.end(), itMin->first.first));
         for(int i = 0; i<Jets.size(); i++){                                                       // delete all map objects related to the Jet
            if(i < positionOfPointer){
               JetPair Key(Jets[i], itMin->first.first);
               distance.erase(distance.find(Key));
            }
            else if(i > positionOfPointer){
               JetPair Key(itMin->first.first, Jets[i]);
               distance.erase(distance.find(Key));
            }
         }
         distance.erase(itMin);
         Jets.erase(Jets.begin()+positionOfPointer);                                               // erase the Jet from the active Jet vector (Jets)

      }
        // -------------------------------------------------

        /*  ------------------------------------------
        *  If the closed object is a Jet
            -------------------------------------------*/
        
      else{

         // Delete all map objects related to the Jets
         int positionOfPointer1 = std::distance(Jets.begin(), find(Jets.begin(), Jets.end(), itMin->first.first));
         int positionOfPointer2 = std::distance(Jets.begin(), find(Jets.begin(), Jets.end(), itMin->first.second));
         for(int i = 0; i<Jets.size(); i++){
            // Deleting all elements corresponding to the first Jet                                               
            if(i < positionOfPointer1 && i != positionOfPointer2){
               JetPair Key(Jets[i], itMin->first.first);
               distance.erase(distance.find(Key));
            }
            else if(i > positionOfPointer1 && i != positionOfPointer2){
               JetPair Key(itMin->first.first, Jets[i]);
               distance.erase(distance.find(Key));
            }
            // Deleting all elements corresponding to the second Jet
            if(i < positionOfPointer2 && i != positionOfPointer1){
               JetPair Key(Jets[i], itMin->first.second);
               distance.erase(distance.find(Key));
            }
            else if(i > positionOfPointer2 && i != positionOfPointer1){
               JetPair Key(itMin->first.second, Jets[i]);
               distance.erase(distance.find(Key));
            }
            
         }
         JetPair Key1(itMin->first.first, emptyJet);
         distance.erase(distance.find(Key1));
         JetPair Key2(itMin->first.second, emptyJet);
         distance.erase(distance.find(Key2));

         DICaloJet* MergedJet = Merge(itMin->first.first, itMin->first.second);                    // Merge those two Jets
         oldJets->push_back(itMin->first.first);                                                   // Save the old Jets (ControllCollection)
         oldJets->push_back(itMin->first.second);

         // Delete the two from active Jet vector (Jets)
         Jets.erase(Jets.begin()+positionOfPointer1);                                                     
         if(positionOfPointer1 < positionOfPointer2){
            Jets.erase(Jets.begin()+positionOfPointer2-1);
         }
         else{
            Jets.erase(Jets.begin()+positionOfPointer2);
         }
         distance.erase(itMin);
         Jets.push_back(MergedJet);                                                                // Include new merged Jet into active Jet vector (Jets)

         //calculate new map objects
         JetPair BeamP(MergedJet,emptyJet);
         distance.insert(pair<JetPair, double>(BeamP, MergedJet->GetKt() * MergedJet->GetKt()));   //calulating beam disance as transferse momentum squared
         for(int i = 0; i < Jets.size()-1; i++){
            const DICaloJet* iJet = Jets[i];
            JetPair PariticleP(iJet,MergedJet);           
            distance.insert(pair<JetPair, double>(PariticleP, iJet->GetDij(MergedJet->GetV())));        //calulating particle disance as min(kti*kti ,ktj*ktj) * DeltaR               
         }    

      }
        // -------------------------------------------------      

   }

   myOutCollection->push_back(emptyJet);
   // final storage for the DIJets events
   fStorage->StoreData(fOutputCollectionName, myOutCollection);
   fStorage->StoreData("ControllCollection", oldJets);       

   return true;
}


bool FastKtAlgorithm::InitRun(){ 
   return true;
}

bool FastKtAlgorithm::FinalizeRun(){ 
   return true;
}

void FastKtAlgorithm::makeHisto(const DICaloJet* jet){
    fHistoService->FastFillTH1D("jetEta", jet->GetEta(), 550, -5.5, 5.5);
    fHistoService->FastFillTH1D("jetSize", jet->GetSize(), 50, -0.5, 49.5);
    fHistoService->FastFillTH1D("jetEnergy", jet->GetEnergy(), 5000, 0., 500000.);
    fHistoService->FastFillTH1D("jetLogEnergy", log(jet->GetEnergy()), 250., 0., 25.);
    fHistoService->FastFillTH1D("jetKt", jet->GetKt(), 100, 0., 1000.);
    if(TMath::Abs(jet->GetEta())<1.5){
        fHistoService->FastFillTH2D("JetlogEnergy_size_central", log(jet->GetEnergy()),jet->GetSize(), 100, 0., 25., 20, 0.5 , 20.5);
        fHistoService->FastFillTH1D("JetLogEnergy_centeral", log(jet->GetEnergy()), 250, 0., 25.);
    }
}

