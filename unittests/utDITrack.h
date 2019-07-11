#ifndef UTDITrack_H
#define UTDITrack_H

#include "UnitTest.h"
#include "DITrack.h"

#include "TVector3.h"
#include "TString.h"
#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "CentralStorage.h"
#include "AnalysisFile.h"
#include "DITrackStub.h"
#include "CaloClusterer.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
// DITrack unit tests
// storing data in a file and retrieving it again, using CentralStorage
// unittest class must be friend of CentralStorage to make this work

namespace ExampleFramework{


class utDITrack : public UnitTesting::UnitTest
{
private:
   CentralStorage* fStorage;
   TString fOutFileName;
   TString fROOTName;
   TFile* fROOTFile;

public:
   utDITrack() 
      : UnitTest("DITrack"), fROOTName("out.root"), fOutFileName("out.dat"), fROOTFile(0), fStorage(0)
   { }

   void run() {
      fStorage = new CentralStorage("utDITrack.dat",1010);
      DataCollection* hits = new DataCollection();
      int newid=300;
      //   DITrackerHit(double x, double y, double z, double pulse, int module, int lay, int uniqueid); 
      TVector3 pos;
      float dphi=0.;
      for (int i=0;i<4;i++){
         pos.SetPtThetaPhi(0.3+float(i)*0.2,0.2,0.5+dphi*float(i));
         hits->push_back(new DITrackerHit(pos.X(), pos.Y(), pos.Z(), 100., 1000+i, i,newid++));
      }
      DITrackStub* stb=new DITrackStub(dynamic_cast<const DITrackerHit*>((*hits)[0]), newid++);
      test_(stb->GetNHits()==1);
      test_(stb->AddHit(dynamic_cast<const DITrackerHit*>((*hits)[1])));
      test_(stb->AddHit(dynamic_cast<const DITrackerHit*>((*hits)[2])));
      test_(stb->AddHit(dynamic_cast<const DITrackerHit*>((*hits)[3])));
      stb->ForceToBeDone();
      test_(stb->IsDone());
      test_(stb->GetNHits()==4);
      double trkphi0=0.3;
      int trkid=newid++; 
      //DITrack(int uniqueid, const DITrackStub* stub, double kappa, double phi0, double theta, double d0, double z0);
      DITrack* trk=new DITrack(trkid,stb, 0., trkphi0, 0.2, 0.1, 0.1);
      test_(trk->GetHits().size()==stb->GetNHits());
      test_(TMath::Abs(trk->GetPhi()-trkphi0)<1.e-5);
      fStorage->PrintStorage();
      DataCollection* trkcol = new DataCollection();
      trkcol->push_back(trk);
      //fStorage->SetDebug(true);
      fStorage->StoreData("TrackerHitCollection",hits);
      fStorage->StoreData("TrackCollection",trkcol);
      fStorage->PrintStorage();
      fStorage->WriteRunHeader();
      fStorage->WriteEvent();
      delete fStorage;
      fStorage = new CentralStorage("out.dat",1010);
      fStorage->SetInputFile(new AnalysisFile("utDITrack.dat","read"));
      fStorage->ReadRunHeader();
      fStorage->ReadNextEvent();
      const DataCollection* trkcol2 = fStorage->RetrieveData("TrackerHitCollection","DITrackerHit");
      const DataCollection* trkcol3 = fStorage->RetrieveData("TrackCollection","DITrack");

      test_(trkcol3->size()==1);
      const DITrack* readtrk=dynamic_cast<const DITrack*>(*(trkcol3->begin()));
      test_(readtrk);
      test_(readtrk->GetHits().size()==4);
      test_(TMath::Abs(readtrk->GetPhi()-trkphi0)<1.e-5);
   }

};

}
#endif
