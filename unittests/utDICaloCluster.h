#ifndef UTDICaloCluster_H
#define UTDICaloCluster_H

#include "UnitTest.h"
#include "DICaloCluster.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "CentralStorage.h"
#include "CaloClusterer.h"
// DICaloCluster unit tests

namespace ExampleFramework{

class utDICaloCluster : public UnitTesting::UnitTest
{
private:
   CentralStorage* fStorage;
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utDICaloCluster() 
      : UnitTest("DICaloCluster"), fROOTName("out.root"), fOutFileName("out.dat"), fROOTFile(0)
   { }

   void run() {
      vector <TString> inlist; // empty input
      inlist.push_back("../data/testdata.dat"); 
      Framework* theFramework = new Framework(inlist, fOutFileName, 100);
      fStorage = new CentralStorage("utDICaloCluster.dat",1010);
      DICaloCluster* hit = new DICaloCluster();
      test_(hit->GetName()=="DICaloCluster");
      
      int id=10;  
      DataCollection* hits = new DataCollection();
      DataCollection* clusteredhits = new DataCollection();
      int module=112; // fdummy value
      const int layer=1;
      DICalorimeterHit* thit = new DICalorimeterHit(0.1, 0.2, 0.3, 1.5, module, layer, id++);
      hits->push_back(thit);
      hit->Add(thit);
      thit = new DICalorimeterHit(0.1001, 0.21, 0.3, 2.5, module+1, layer+1, id++);
      hits->push_back(thit);
      hit->Add(thit);
      test_(hit->GetSize()==2);
      test_(fabs(hit->GetX()-0.1)<0.01);
      test_(fabs(hit->GetY()-0.205)<0.01);
      test_(fabs(hit->GetZ()-0.3)<0.01);
      test_(fabs(hit->GetX()-hit->GetV().X())<0.0001);
      double hitx=hit->GetX();
      double hitE=hit->GetEnergy();
      cout << "quantiles "<<hit->GetLayerQuantile(0.5)<< " 99="<<hit->GetLayerQuantile(0.99)<<endl;
      test_(hitE>3.5);
      clusteredhits->push_back(hit);
      fStorage->StoreData("CalorimeterHitCollection",hits);
      fStorage->StoreData("CaloClusterCollection",clusteredhits);
      fStorage->WriteRunHeader();
      fStorage->WriteEvent();
      delete fStorage;
      fStorage = new CentralStorage("out.dat",1010);
      fStorage->SetInputFile(new AnalysisFile("utDICaloCluster.dat","read"));
      fStorage->ReadRunHeader();
      fStorage->ReadNextEvent();
      const DataCollection* readclus = fStorage->RetrieveData("CaloClusterCollection","DICaloCluster");
      const DICaloCluster* clus2=dynamic_cast<const DICaloCluster*>(*(readclus->begin()));
      test_(fabs(hitx - clus2->GetX())<1.e-4);
      test_(clus2->GetSize()==2);
      test_(fabs(clus2->GetEnergy()-hitE)<1.e-4);
      test_(clus2->GetLayerQuantile(0.5)>0.5);
      test_(fabs(clus2->GetLayerQuantile(0.99)-2.)<0.1);
      // delete hit; //test if destructor is private, should not compile
      delete theFramework;
   }


};

}

#endif
