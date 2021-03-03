#ifndef UTFastKtAlgorithm_H
#define UTFastKtAlgorithm_H

#include "UnitTest.h"
#include "FastKtAlgorithm.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "FastKtAlgorithm.h"
#include "CaloClusterer.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
// FastKtAlgorithm unit tests

namespace ExampleFramework {

class utFastKtAlgorithm : public UnitTesting::UnitTest
{
private:
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utFastKtAlgorithm() 
      : UnitTest("FastKtAlgorithm"), fROOTName("out.root"), fOutFileName("out.dat"), fROOTFile(0)
   { }

   void init(string filname) {
      int jobID=138;
      vector <TString> inlist;
      inlist.push_back(filname); 
      Framework* theFramework = new Framework(inlist, fOutFileName, jobID);
      theFramework->Silence();
      theFramework->AddProcessor(new CaloRawHitProcessor(theFramework));
      theFramework->AddProcessor(new CaloClusterer(theFramework));
      theFramework->AddProcessor(new FastKtAlgorithm(theFramework));
      
      theFramework->ProcessAll();
      delete theFramework;
      fROOTFile=TFile::Open(fROOTName);
   }

   void run(){
      runSingle("../data/chargedpion10GeV.dat", 10000.);
      runSingle("../data/chargedpion50GeV.dat", 50000.);
      runSingle("../data/electron10GeV.dat", 10000.);
      runSingle("../data/electron50GeV.dat", 50000.);
      runSingle("../data/muon10GeV.dat", 10000.);
      runSingle("../data/muon50GeV.dat", 10000.);
   }

   void runSingle(string filname, float Etruth){
      init(filname);
      TH1D* hist = (TH1D*) fROOTFile->Get("JetFinder/jetEnergy");
      test_(hist->GetMean()>0.5*Etruth);
      test_(hist->GetMean()<2.*Etruth);
      TH1D* hSiz = (TH1D*) fROOTFile->Get("JetFinder/jetSize");
      test_(hSiz->GetMean()>1.0);
      test_(hSiz->GetMean()<10.);
      fROOTFile->Close();
   }
};

}
#endif
