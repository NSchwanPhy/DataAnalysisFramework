#include "DITrack.h"
#include "DITrackStub.h"
#include "CentralStorage.h"
#include "TVector3.h"
#include "TString.h"
#include "TMath.h"
using namespace ExampleFramework; 
MyImp(DITrack); // implementation of GetName function with preprocessor macro

DITrack::DITrack(int uniqueid, const DITrackStub* stub, double kappa, double phi0, double theta, double d0, double z0) 
      : DataItem(uniqueid) 
{ 
   fHits = stub->fHits;
   fKappa=kappa;
   fPhi0=phi0;
   fTheta=theta;
   fD0=d0;
   fZ0=z0;
   FillMomentumAtPCA();   
}

void DITrack::Read()
{
   theStorage->Read(fKappa);
   theStorage->Read(fPhi0);
   theStorage->Read(fTheta);
   theStorage->Read(fD0);
   theStorage->Read(fZ0);
   int nhits;
   theStorage->Read(nhits);
   const DataItem* poi;
   for (int i=0;i<nhits;i++) {
      theStorage->Read(poi);
      fHits.push_back(dynamic_cast<const DITrackerHit*>(poi));
   }
   FillMomentumAtPCA();
}

void DITrack::Write() const 
{
   theStorage->Write(fKappa);
   theStorage->Write(fPhi0);
   theStorage->Write(fTheta);
   theStorage->Write(fD0);
   theStorage->Write(fZ0);
   theStorage->Write(int(fHits.size()));
   for (uint i=0;i<fHits.size();i++) {
      theStorage->Write(fHits[i]);
   }
   
}

//! The PCA, or perigree is the point of closest approach to the origin of the detector (in the transverse plane). The momentum at the PCA is usually the best estimate for the momentum before the track is bent in the magnetic field 
void DITrack::FillMomentumAtPCA(){
   double R,pt,pz,eta;
   if (fKappa==0.) R=1.e10;
   else R=TMath::Abs(1./fKappa);
   // todo fix me pt = fDetectorModel->GetBField() * R * 0.3 * 10.; // R in cm and pt in MeV
   pt = 4.0 * R * 0.3 * 10.; // R in cm and pt in MeV //ToDo hardcoded BField!!
   fMomentumAtPCA.SetPtThetaPhi(pt,fTheta,fPhi0);
   fPCA.SetXYZ(fD0*TMath::Cos(fPhi0+TMath::Pi()/2.),fD0*TMath::Sin(fPhi0+TMath::Pi()/2.),fZ0); //ToDo check sign of D0
}
