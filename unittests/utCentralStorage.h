#ifndef UTCentralStorage_H
#define UTCentralStorage_H

#include "UnitTest.h"
#include "CentralStorage.h"
#include "DIRunNumber.h"

#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include <exception>
// CentralStorage unit tests

namespace ExampleFramework{

class utCentralStorage : public UnitTesting::UnitTest
{
private:
   CentralStorage fStorage;

   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utCentralStorage() 
      : UnitTest("CentralStorage"), fStorage("out.dat", 1007), fROOTName("out.root"), fOutFileName("out.dat"), fROOTFile(0)
   { }

   void run() {
      bool caught1=false,caught2=false,caught3=false,caught4=false,caught5=false;
      // example code
      DataCollection* RunNumberContainer = new DataCollection();
      try {
         // storing new collection under invalid name (contains space)
         fStorage.StoreData("Run Number",RunNumberContainer);
      } catch(...) { caught1=true; }

      DIRunNumber* poi = new DIRunNumber(1008);
      RunNumberContainer->push_back(poi);
      fStorage.StoreData("RunNumber",RunNumberContainer);
      DataCollection* RunNumberContainer2 = new DataCollection();
      try {
         // storing new collection under same name
         fStorage.StoreData("RunNumber",RunNumberContainer2);
      } catch(...) { caught2=true;}

      RunNumberContainer2->push_back(poi);
      try {
         // storing new collection under new name with old dataitem
         fStorage.StoreData("RunNumber2",RunNumberContainer2);
      } catch(...) { caught3=true; }

      test_(caught1);
      test_(caught2);
      test_(caught3);
      test_(fStorage.RetrieveData("RunNumber","DIRunNumber"));
      test_(fStorage.RetrieveData("RunNumber","donotcare"));
      test_(!fStorage.RetrieveData("Run Number","donotcare")); // must fail
      test_(!fStorage.RetrieveData("RunNumberXX","donotcare")); // must fail
      test_(!fStorage.RetrieveData("RunNumber","DIWrong")); // must fail

      DataCollection* RunNumberContainer3 = new DataCollection();
      fStorage.StoreData("RunNumber3",RunNumberContainer3);
      test_(fStorage.RetrieveData("RunNumber3","DIRunNumber")); // retrieval of empty object
      test_(fStorage.RetrieveData("RunNumber3","donotcare")); // retrieval of empty object

      DataCollection* RunNumberContainer4 = new DataCollection();
      RunNumberContainer4->SetPersistent(true);
      fStorage.StoreData("RunNumber4",RunNumberContainer4);
      test_(fStorage.RetrieveData("RunNumber4","DIRunNumber")); // retrieval of empty object
      fStorage.Clear(true); // clear transient items, runnumber must still be there
      test_(fStorage.RetrieveData("RunNumber4","DIRunNumber")); // retrieval of empty object
      fStorage.Clear(false); // clear transient items, runnumber must still be there
      test_(!fStorage.RetrieveData("RunNumber4","DIRunNumber")); // now everything must be cleared
   }

};
}
#endif
