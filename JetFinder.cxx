
#include "JetFinder.h"

using namespace ExampleFramework;
   

// constructor 
JetFinder::JetFinder(Framework* fram) : Processor("JetFinder", fram){

    DefineProperty("InputCollection", &fInputCollectionName, "CaloClusterCollection");
    DefineProperty("OutputCollection", &fOutputCollectionName, "JetCollection");
}

DICaloJet* JetFinder::Merge(const DICaloJet* di1,const DICaloJet* di2){
   DICaloJet* res = new DICaloJet(CreateUniqueObjectID());           

   res->SetEnergy( di2->GetEnergy() + di1->GetEnergy() );
   double x = di2->GetX() * (di2->GetEnergy() / res->GetEnergy())  + di1->GetX() * (di1->GetEnergy() / res->GetEnergy());
   double y = di2->GetY() * (di2->GetEnergy() / res->GetEnergy())  + di1->GetY() * (di1->GetEnergy() / res->GetEnergy());
   double z = di2->GetZ() * (di2->GetEnergy() / res->GetEnergy())  + di1->GetZ() * (di1->GetEnergy() / res->GetEnergy());
   res->SetCmsX(x);
   res->SetCmsY(y);
   res->SetCmsZ(z);

   vector <const DataItem* > MergedCaloClusters = di1->GetCaloClusters();
   vector <const DataItem* > CaloClusters = di2->GetCaloClusters();
   MergedCaloClusters.insert(MergedCaloClusters.end(), CaloClusters.begin(), CaloClusters.end());
   res->SetCaloClusters(MergedCaloClusters);  
   return res;
}

