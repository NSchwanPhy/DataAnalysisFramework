#ifndef UTDICaloJet_H
#define UTDICaloJet_H

#include "UnitTest.h"
#include "DICaloJet.h"
#include "DICaloCluster.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "CentralStorage.h"
#include "DICaloJet.h"
#include "CaloClusterer.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
// DICaloJet unit tests

namespace ExampleFramework {

class utDICaloJet : public UnitTesting::UnitTest
{
private:
   CentralStorage* fStorage;
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utDICaloJet() 
      : UnitTest("DICaloJet"), fROOTName("out.root"), fOutFileName("out.dat"), fROOTFile(0)
   { }

   void run(){

      vector <TString> inlist; // empty input
      inlist.push_back("../data/testdata.dat"); 
      Framework* theFramework = new Framework(inlist, fOutFileName, 100);
      DICaloCluster* cluster = new DICaloCluster();
      DICaloJet* jet = new DICaloJet();
      test_(jet->GetName()=="DICaloJet");

      //first creating two clusterCol including two hits each
      int id=10;
      DataCollection* hitCol = new DataCollection();             //Collection of hit objects
      DataCollection* clusterCol = new DataCollection();    //Collection of cluster objects
      DataCollection* jetCol = new DataCollection();   //Collection of jet objects
      int module=112; // fdummy value
      const int layer=1;
      DICalorimeterHit* thit = new DICalorimeterHit(0.1, 0.2, 0.3, 1.5, module, layer, id++);
      hitCol->push_back(thit);
      cluster->Add(thit);
      thit = new DICalorimeterHit(0.1001, 0.21, 0.3, 2.5, module+1, layer+1, id++);
      hitCol->push_back(thit);
      cluster->Add(thit);
      jet->Add(cluster);
      clusterCol->push_back(cluster);

      cluster = new DICaloCluster();
      hitCol = new DataCollection();
      module = 23;
      thit = new DICalorimeterHit(0.3,0.1,0.2,5.5, module, layer, id++);
      hitCol->push_back(thit);
      cluster->Add(thit);
      thit = new DICalorimeterHit(0.301,0.12,0.21,4.5, module+1, layer+1, id++);
      hitCol->push_back(thit);
      cluster->Add(thit);
      jet->Add(cluster);
      clusterCol->push_back(cluster);
      jetCol->push_back(jet);

      // Are the clusters added correctly?
      vector<const DataItem*> Clusters = jet->GetCaloClusters();
      const DICaloCluster* C1 = dynamic_cast<const DICaloCluster*>(Clusters[0]);
      const DICaloCluster* C2 = dynamic_cast<const DICaloCluster*>(Clusters[1]);
      double newE = C1->GetEnergy() + C2->GetEnergy();
      double CmsX = C1->GetX() * (C1->GetEnergy() / newE) + C2->GetX() * (C2->GetEnergy() / newE);
      double CmsY = C1->GetY() * (C1->GetEnergy() / newE) + C2->GetY() * (C2->GetEnergy() / newE);
      double CmsZ = C1->GetZ() * (C1->GetEnergy() / newE) + C2->GetZ() * (C2->GetEnergy() / newE);
      TVector3 V(0.1,0.2,0.3);
      double kti = jet->GetKt();
      double ktj = V.Pt();
      double Dij = min(kti * kti, ktj * ktj) * (jet->GetV()).DeltaR(V);

      test_(jet->GetSize()==2);
      test_(fabs(newE - jet->GetEnergy()) < 0.01);
      test_(fabs(CmsX - jet->GetX()) < 0.01);
      test_(fabs(CmsY - jet->GetY()) < 0.01);
      test_(fabs(CmsZ - jet->GetZ()) < 0.01);
      test_(fabs(kti - jet->GetV().Pt()) < 0.01);
      test_(fabs(Dij - jet->GetDij(V)) < 0.01);

      // Do the Setter and Get functions work?
      jet->SetV(V);
      test_(V == jet->GetV());
      jet->SetEnergy(10);
      jet->SetCmsX(1);
      jet->SetCmsY(2);
      jet->SetCmsZ(3);
      
      test_(jet->GetEnergy() == 10 );
      test_(jet->GetX() == 1);
      test_(jet->GetY() == 2);
      test_(jet->GetZ() == 3);

      //testing one example for the distance calculation
      TVector3 Va(501.450051,12.538864,-462.392624);
      TVector3 Vb(495.432810,-79.030476,-462.475414);
      test_(fabs(Va.Pt()*Va.Pt() - 251609) < 1);
      test_(fabs(Vb.Pt()*Vb.Pt() - 251699) < 1);
      test_(fabs(min(Va.Pt()*Va.Pt(), Vb.Pt()*Vb.Pt()) * Va.DeltaR(Vb) - 46091.1) < 1);


      
      delete theFramework;
   }
   

};

}
#endif
