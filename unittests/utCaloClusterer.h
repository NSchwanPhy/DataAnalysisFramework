#ifndef UTCaloClusterer_H
#define UTCaloClusterer_H

#include "UnitTest.h"
#include "CaloClusterer.h"

#include "TString.h"
#include <string>
using std::string;
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "PrintTrackerHits.h"
#include "CaloClusterer.h"
#include "CaloRawHitProcessor.h"
// CaloClusterer unit tests

using namespace ExampleFramework;

class utCaloClusterer : public UnitTesting::UnitTest
{
private:
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utCaloClusterer() 
      : UnitTest("CaloClusterer"), fROOTName("CaloClusterer.root"), fOutFileName("CaloClusterer.dat"), fROOTFile(0)
   { }

   void init(string filname) {
      int jobID=138;
      vector <TString> inlist;
      inlist.push_back(filname); 
      Framework* theFramework = new Framework(inlist, fOutFileName, jobID);
      theFramework->Silence();
      theFramework->AddProcessor(new CaloRawHitProcessor(theFramework));
      theFramework->AddProcessor(new CaloClusterer(theFramework));
      
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
      run1();
   }
   void runSingle(string filname, float Etruth){
      init(filname);
      TH1D* hist = (TH1D*) fROOTFile->Get("CaloClusterer/clusterEnergy");
      test_(hist->GetMean()>0.5*Etruth);
      test_(hist->GetMean()<2.*Etruth);
      TH1D* hSiz = (TH1D*) fROOTFile->Get("CaloClusterer/clusterSize");
      test_(hSiz->GetMean()>1.0);
      test_(hSiz->GetMean()<10.);
      TH1D* hN = (TH1D*) fROOTFile->Get("CaloClusterer/Ncluster");
      if (hN->GetMean()<0.9) cout << "mean nclus="<<hN->GetMean()<<endl;
      test_(hN->GetMean()>0.9);
      test_(hN->GetMean()<1.5);
      TH1D* hS = (TH1D*) fROOTFile->Get("CaloClusterer/SeedEnergy");
      test_(hS->GetMean()>1000.);
      fROOTFile->Close();
   }
   void run1(){
      init("../data/testdata.dat");
      TH1D* hist = (TH1D*) fROOTFile->Get("CaloClusterer/clusterEnergy");
      test_(hist->GetMean()>3000.);
      test_(hist->GetMean()<30000.);
      TH1D* hSiz = (TH1D*) fROOTFile->Get("CaloClusterer/clusterSize");
      test_(hSiz->GetMean()>1.0);
      test_(hSiz->GetMean()<5.);
      TH1D* hN = (TH1D*) fROOTFile->Get("CaloClusterer/Ncluster");
      test_(hN->GetMean()>15);
      test_(hN->GetMean()<200);
      fROOTFile->Close();
   }

};
#endif
