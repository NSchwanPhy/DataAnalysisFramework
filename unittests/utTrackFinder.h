#ifndef UTTrackFinder_H
#define UTTrackFinder_H

#include "UnitTest.h"
#include "TrackFinder.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "PrintTrackerHits.h"
#include "TrackFinder.h"
#include "CaloClusterer.h"
#include "SiliconClusterer.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
// TrackFinder unit tests

using namespace ExampleFramework;

class utTrackFinder : public UnitTesting::UnitTest
{
private:
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utTrackFinder() 
      : UnitTest("TrackFinder"), fROOTName("trackfinder.root"), fOutFileName("trackfinder.dat"), fROOTFile(0)
   { }

   void init() {
      int jobID=138;
      vector <TString> inlist;
      inlist.push_back("../data/muon20GeV.dat"); 
      Framework* theFramework = new Framework(inlist, fOutFileName, jobID);
      theFramework->Silence();
      theFramework->AddProcessor(new TrackerRawHitProcessor(theFramework));
      theFramework->AddProcessor(new SiliconClusterer(theFramework));
      TrackFinder* finder = new TrackFinder(theFramework);
      finder->SetProperty("InputCollection","ClusteredTrackerHitCollection");
      theFramework->AddProcessor(finder);
      
      theFramework->ProcessAll();
      delete theFramework;
      fROOTFile=TFile::Open(fROOTName);
   }
   
   void run(){
      init();
      TH1D* hntrk = (TH1D*) fROOTFile->Get("TrackFinder/NTracks");
      TH1D* hnhit = (TH1D*) fROOTFile->Get("TrackFinder/NHitsOnTrack");
      TH1D* hpt = (TH1D*) fROOTFile->Get("TrackFinder/Pt");
      //cout << "Tracker Pulse Mean=" << hist->GetMean()<<endl;
      test_(TMath::Abs(hntrk->GetMean()-1.)<0.1);
      test_(TMath::Abs(hnhit->GetMean()-ModuleID::fNTrackerLayers)<0.1);
      test_(TMath::Abs(hpt->GetMean()-20000.)/20000.<0.1);
      test_(hpt->GetRMS()/hpt->GetMean()<0.18);
   }

};
#endif
