#ifndef UTTestRun_H
#define UTTestRun_H

#include "UnitTest.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"
#include "TH1D.h"

#include "Framework.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
#include "TrackFinder.h"
#include "CaloClusterer.h"

// TestRun unit tests

using namespace ExampleFramework;

class utTestRun : public UnitTesting::UnitTest
{
private:
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utTestRun() 
      : UnitTest("TestRun"), fROOTName("outrun.root"), fOutFileName("outrun.dat"), fROOTFile(0)
   { }

   void init() {
      int jobID=138;
      vector <TString> inlist;
      inlist.push_back("../data/testdata.dat"); 
      inlist.push_back("../data/electron20GeV.dat"); 
      Framework* theFramework = new Framework(inlist, fOutFileName, jobID);
      theFramework->Silence();
      theFramework->AddProcessor(new TrackerRawHitProcessor(theFramework));
      theFramework->AddProcessor(new CaloRawHitProcessor(theFramework));

      TrackFinder* finder = new TrackFinder(theFramework);
      finder->SetProperty("CurvatureCorrection", 1.01);
      theFramework->AddProcessor(finder);      
      theFramework->AddProcessor(new CaloClusterer(theFramework));
 
      theFramework->ProcessAll();
      delete theFramework;
      fROOTFile=TFile::Open(fROOTName);
   }

   void run(){
      init();
      TH1D* hist = (TH1D*) fROOTFile->Get("TrackerRawHitProcessor/TrackerHitPulseheight");
      cout << "Tracker Pulse Mean=" << hist->GetMean()<<endl;
      test_(hist->GetMean()>1.);
   }
};
#endif
