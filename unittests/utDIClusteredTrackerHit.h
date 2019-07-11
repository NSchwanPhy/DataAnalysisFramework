#ifndef UTDIClusteredTrackerHit_H
#define UTDIClusteredTrackerHit_H

#include "UnitTest.h"
#include "DIClusteredTrackerHit.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "CentralStorage.h"
#include "PrintTrackerHits.h"
#include "TrackFinder.h"
#include "CaloClusterer.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
// DIClusteredTrackerHit unit tests

namespace ExampleFramework{

class utDIClusteredTrackerHit : public UnitTesting::UnitTest
{
private:
   CentralStorage* fStorage;
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utDIClusteredTrackerHit() 
      : UnitTest("DIClusteredHits"), fROOTName("out.root"), fOutFileName("out.dat"), fROOTFile(0)
   { }

   void run() {
      vector <TString> inlist; // empty input
      inlist.push_back("../data/testdata.dat"); 
      Framework* theFramework = new Framework(inlist, fOutFileName, 100);
      DIClusteredTrackerHit* hit = new DIClusteredTrackerHit();
      DITrackerHit* thit = hit;
      test_(hit->GetName()=="DIClusteredTrackerHit");
      test_(thit->GetName()=="DIClusteredTrackerHit");
      
      int id=10;  
      fStorage = new CentralStorage("utDIClusteredTrackerHit.dat",1010);
      DataCollection* hits = new DataCollection();
      DataCollection* clusteredhits = new DataCollection();
      int module=112; // fdummy value
      const int layer=1;
      thit = new DITrackerHit(0.1, 0.2, 0.3, 1.5, module, layer, id++);
      hits->push_back(thit);
      hit->Add(thit);
      thit = new DITrackerHit(0.1001, 0.21, 0.3, 2.5, module+1, layer, id++);
      hits->push_back(thit);
      hit->Add(thit);
      test_(hit->GetSize()==2);
      test_(hit->GetLayer()==layer);
      test_(fabs(hit->GetX()-0.1)<0.01);
      test_(fabs(hit->GetY()-0.205)<0.01);
      test_(fabs(hit->GetZ()-0.3)<0.01);
      test_(fabs(hit->GetX()-hit->GetV().X())<0.0001);
      double hitx=hit->GetX();
      clusteredhits->push_back(hit);
      fStorage->StoreData("TrackerHitCollection",hits);
      fStorage->StoreData("ClusteredTrackerHitCollection",clusteredhits);
      fStorage->WriteRunHeader();
      fStorage->WriteEvent();
      delete fStorage;
      fStorage = new CentralStorage("out.dat",1010);
      fStorage->SetInputFile(new AnalysisFile("utDIClusteredTrackerHit.dat","read"));
      fStorage->ReadRunHeader();
      fStorage->ReadNextEvent();
      const DataCollection* readclus = fStorage->RetrieveData("ClusteredTrackerHitCollection","DIClusteredTrackerHit");
      const DIClusteredTrackerHit* clus2=dynamic_cast<const DIClusteredTrackerHit*>(*(readclus->begin()));
      test_(fabs(hitx - clus2->GetX())<1.e-4);
      test_(clus2->GetSize()==2);
      test_(clus2->GetLayer()==layer);
      // delete hit; //test if destructor is private, should not compile
      delete theFramework;
   }

   //TH1D* hist = (TH1D*) fROOTFile->Get("PrintTrackerHits/TrackerHitPulseHeight");
   //cout << "Tracker Pulse Mean=" << hist->GetMean()<<endl;
   //test_(hist->GetMean()>1.);

};

}

#endif
