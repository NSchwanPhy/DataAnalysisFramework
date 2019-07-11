#ifndef UTDUMMY_H
#define UTDUMMY_H

#include "UnitTest.h"
#include "DUMMY.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "DUMMY.h"
#include "CaloClusterer.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
// DUMMY unit tests

namespace ExampleFramework {

class utDUMMY : public UnitTesting::UnitTest
{
private:
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utDUMMY() 
      : UnitTest("DUMMY"), fROOTName("out.root"), fOutFileName("out.dat"), fROOTFile(0)
   { }

   void init() {
      // example code
//       Framework* theFramework = new Framework("testdata.dat", fOutFileName, 111);
//       theFramework->AddProcessor(new TrackerRawHitProcessor(theFramework));
//       theFramework->AddProcessor(new CaloRawHitProcessor(theFramework));
//       theFramework->AddProcessor(new CaloClusterer(theFramework));
//       theFramework->AddProcessor(new DUMMY(theFramework));
      
//       theFramework->ProcessAll();
//       delete theFramework;
//       fROOTFile=TFile::Open(fROOTName);
   }

   void run(){
      init();
      //TH1D* hist = (TH1D*) fROOTFile->Get("DUMMY/NHits");
      //cout << "Tracker Pulse Mean=" << hist->GetMean()<<endl;
      //test_(hist->GetMean()>1.); // this function tests argument for being true      
      //fROOTFile->Close();
   }

};

}
#endif
