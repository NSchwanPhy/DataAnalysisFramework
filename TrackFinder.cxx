#include "DITrack.h"
#include "DITrackStub.h"
#include "TrackFinder.h"
#include "ModuleID.h"
#include "TMath.h"
#include <cassert>

using namespace ExampleFramework;

// constructor 
TrackFinder::TrackFinder(Framework* fram) 
  : Processor("TrackFinder", fram)
{
   DefineProperty("InputCollection", 
                  &fInputCollectionName, "TrackerHitCollection");
   DefineProperty("OutputCollection", 
                  &fOutputCollectionName, "TrackCollection");
   DefineProperty("CurvatureCorrection", &fCurvatureCorrection, 1.);
   fOutermostTrackerLayer = ModuleID::fNTrackerLayers-1;
   fHitsUsedJustOnce=false;
}

bool TrackFinder::EventLoop(){

   CreateTrackStubs();

   // FillTrackStubs
   for (vector<DITrackStub*>::iterator st = fStubs.begin(); st != fStubs.end(); st++){
      DITrackStub* stub = *st; 
      while (!(stub->IsDone())){
         vector<const DITrackerHit*>::iterator besthit=fHits.end();
         double bestdistance = 1.e20;
         int nextLayer = stub->GetNextLayer();
         //cout << "nextLayer="<<nextLayer<<" nHits="<<stub->GetNHits()<<endl;
         for (vector<const DITrackerHit*>::iterator it = fHits.begin(); it != fHits.end(); it++){
            const DITrackerHit* hit = dynamic_cast< const DITrackerHit* >(*it);
            if (hit->GetLayer() != nextLayer) continue;
            double dist = stub->GetDistance(hit);
            if (dist < bestdistance){
               besthit = it;
               bestdistance = dist;
            }
         }
         if (besthit != fHits.end()){
            stub->AddHit( dynamic_cast< const DITrackerHit* >(*besthit) );
            if (fHitsUsedJustOnce) fHits.erase(besthit); 
         }
         else {stub->ForceToBeDone();}
      }
   }
   // what to do with leftover hits?

   CreateTracks(); // create tracks from stubs

   return true;
}

bool TrackFinder::InitRun(){ 
   return true;
}

bool TrackFinder::FinalizeRun(){ 
   return true;
}

bool TrackFinder::CreateTrackStubs()
{
   const DataCollection* inputs = fStorage->RetrieveData(fInputCollectionName,"donotcare");
   fStubs.clear();
   fHits.clear();
   // loop over hits
   for (DataCollection::const_iterator it = inputs->begin(); it != inputs->end(); it++){
      const DITrackerHit* hit = dynamic_cast< const DITrackerHit* >(*it);
      assert(hit);
      if (hit->GetLayer()==fOutermostTrackerLayer){
         // create trackstub
         fStubs.push_back(new DITrackStub(hit,CreateUniqueObjectID()));
      }
      else fHits.push_back(hit);
   }
   return true;
}

bool TrackFinder::CreateTracks()
{
   // create output
   DataCollection* myOutCollection = new DataCollection();
   
   for (vector<DITrackStub*>::iterator st = fStubs.begin(); st != fStubs.end(); st++){
      DITrackStub* stub = *st;
      //
      double kappa=0., phi0=0., theta=0., d0=0.,z0=0.;
      vector<const DITrackerHit*>& stubhits = stub->fHits;
      if (stubhits.size()<=3) continue;
      const DITrackerHit* hit5 = stubhits[0];
      const DITrackerHit* hit0 = stubhits[stubhits.size()-1];     
      TVector3 dir1=hit0->GetV();
      TVector3 dir5=hit5->GetV();
      double deltaR = TMath::Abs(dir5.Perp()-dir1.Perp());
      kappa = dir5.DeltaPhi(dir1)/deltaR  * fCurvatureCorrection * 2.;         
      TVector3 xX2   = hit0->GetV();
      theta = (dir5-dir1).Theta(); // !!!!!!!!, very crude theta of one hit;
      phi0 = (dir5-dir1).Phi(); // !!!!!!!!, very crude theta of one hit;
      TVector3 xDir2 = (dir5-dir1);
      xX2.SetZ(0.);
      xDir2.SetZ(0.);
      xDir2 = xDir2.Unit(); 
      xX2 = (xX2 -xX2.Dot(xDir2) * xDir2);
      d0 = xX2.Mag();
      if (xX2.X()*xDir2.Y()-xX2.Y()*xDir2.X() >0.) d0  = -d0; 
      fHistoService->FastFillTH1D("d0",d0,500,-50.,50.);
      if (TMath::Abs(tan(theta))>1.e-5) z0 = dir1.Z()-dir1.Perp()/tan(theta);
      else z0=0.;
      fHistoService->FastFillTH1D("z0",z0,500,-500.,500.);
      // create track 
      DITrack* mytrack=new DITrack(CreateUniqueObjectID(),stub,kappa,phi0,theta,d0,z0);
      
      fHistoService->FastFillTH1D("TrackPt",mytrack->GetPt()/1000.,1000,0.,100.);      
      // cut on pt
      if (mytrack->GetPt()<0.2 * 1000.) continue; //ToDO make constant
      myOutCollection->push_back(mytrack);
      fHistoService->FastFillTH1D("PCA_X",mytrack->GetPCA().X(),500,-5.,5.);
      fHistoService->FastFillTH1D("PCA_Y",mytrack->GetPCA().Y(),500,-5.,5.);
      fHistoService->FastFillTH1D("PCA_Z",mytrack->GetPCA().Z(),500,-100.,100.);
      fHistoService->FastFillTH1D("Pt",mytrack->GetPt(),500,0.,100000.);
      fHistoService->FastFillTH1D("NHitsOnTrack",stub->GetNHits(),50,-0.5,49.5);
      delete stub; // delete item not handed to CentralStorage
   }
   fHistoService->FastFillTH1D("NTracks",myOutCollection->size(),500,-0.5,499.5);
   fHistoService->FastFillTH1D("NTracksZoom",myOutCollection->size(),50,-0.5,49.5);
   // store output collection 
   fStorage->StoreData(fOutputCollectionName,myOutCollection);
   return true;
}
