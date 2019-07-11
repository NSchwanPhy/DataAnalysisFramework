#ifndef UTSiliconClusterer_H
#define UTSiliconClusterer_H

#include "UnitTest.h"
#include "SiliconClusterer.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "PrintTrackerHits.h"
#include "TrackFinder.h"
#include "CaloClusterer.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
// SiliconClusterer unit tests

using namespace ExampleFramework;

class utSiliconClusterer : public UnitTesting::UnitTest
{
private:
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utSiliconClusterer() 
      : UnitTest("SiliconClusterer"), fROOTName("siliconclusterer.root"), fOutFileName("siliconclusterer.dat"), fROOTFile(0)
   { }

   void init() {
      int jobID=138;
      vector <TString> inlist;
      inlist.push_back("../data/muon20GeV.dat"); 
      Framework* theFramework = new Framework(inlist, fOutFileName, jobID);
      theFramework->Silence();
      theFramework->AddProcessor(new TrackerRawHitProcessor(theFramework));
      theFramework->AddProcessor(new SiliconClusterer(theFramework));
      
      theFramework->ProcessAll();
      delete theFramework;
      fROOTFile=TFile::Open(fROOTName);
   }

   void run(){
      init();
      run1();
      run2();
      run3();
      run4();
      run5();
      run6();
   }

   void run1(){ 
      TH1D* hnclus  = (TH1D*) fROOTFile->Get("SiliconClusterer/NCluster");
      TH1D* hwidth  = (TH1D*) fROOTFile->Get("SiliconClusterer/ClusterWidth");
      TH1D* hcharge = (TH1D*) fROOTFile->Get("SiliconClusterer/ClusterCharge");
      test_(TMath::Abs(hnclus->GetMean()-ModuleID::fNTrackerLayers)<1.);
      test_(hwidth->GetMean()>1.2);
      test_(hwidth->GetMean()<2.2);
      test_(hcharge->GetMean()>10.);
      test_(hcharge->GetMean()<100.);
   }
   void run2(){ 

   }
   void run3(){ 

   }
   void run4(){ 

   }
   void run5(){ 

   }
   void run6(){ 

   }

};
#endif
