
#include "TMath.h"
#include "TVector3.h"
#include "TRandom.h"
#include <iostream>
#include <cstdlib>
#include "ModuleID.h"

using namespace ExampleFramework;
using std::cout;
using std::endl;

ModuleID::ModuleID() 
   : fHisto(0), fDebug(false) //fDebug(true) 
{ 
   // tracker
   fR.push_back(50.);
   fR.push_back(100.);
   fR.push_back(200.);
   fR.push_back(300.);
   fR.push_back(400.);
   // calo layer
   fR.push_back(500.); 
   fR.push_back(510.); 
   fR.push_back(520.); 
   fR.push_back(530.); 
   fR.push_back(540.); 
   fR.push_back(550.); 
   fR.push_back(560.); 
   fR.push_back(570.); 
   
   fBarrelEta.push_back(1.5);
   fBarrelEta.push_back(1.5);
   fBarrelEta.push_back(1.5);
   fBarrelEta.push_back(1.5);
   fBarrelEta.push_back(1.5);
   fBarrelEta.push_back(1.7);
   fBarrelEta.push_back(1.7);
   fBarrelEta.push_back(1.7);
   fBarrelEta.push_back(1.7);
   fBarrelEta.push_back(1.7);
   fBarrelEta.push_back(1.7);
   fBarrelEta.push_back(1.7);
   fBarrelEta.push_back(1.7);
   
   fMaxEta.push_back(2.5);
   fMaxEta.push_back(2.5);
   fMaxEta.push_back(2.5);
   fMaxEta.push_back(2.5);
   fMaxEta.push_back(2.5);
   fMaxEta.push_back(4.8);
   fMaxEta.push_back(4.8);
   fMaxEta.push_back(4.8);
   fMaxEta.push_back(5.);
   fMaxEta.push_back(5.);
   fMaxEta.push_back(5.);
   fMaxEta.push_back(5.);
   fMaxEta.push_back(5.);
   
   fDEta.push_back(0.02);
   fDEta.push_back(0.02);
   fDEta.push_back(0.02);
   fDEta.push_back(0.02);
   fDEta.push_back(0.02);
   fDEta.push_back(0.05);
   fDEta.push_back(0.05);
   fDEta.push_back(0.05);
   fDEta.push_back(0.1);
   fDEta.push_back(0.1);
   fDEta.push_back(0.1);
   fDEta.push_back(0.1);
   fDEta.push_back(0.1);
   
   fDPhi.push_back(0.02);
   fDPhi.push_back(0.02);
   fDPhi.push_back(0.02);
   fDPhi.push_back(0.02);
   fDPhi.push_back(0.02);
   fDPhi.push_back(0.05);
   fDPhi.push_back(0.05);
   fDPhi.push_back(0.05);
   fDPhi.push_back(0.1);
   fDPhi.push_back(0.1);
   fDPhi.push_back(0.1);
   fDPhi.push_back(0.1);
   fDPhi.push_back(0.1);
   
   if ( fDEta.size() != fNLayers ||
        fDEta.size() != fDPhi.size() || 
        fDEta.size() != fMaxEta.size() || 
        fDEta.size() != fBarrelEta.size() || 
        fDEta.size() != fR.size() ) {
      cout << "error initializing ModuleID"<<endl;
      exit(1);
   }
   
   for (int i=0; i<fNLayers; i++) fZ.push_back(fR[i]*TMath::SinH(fBarrelEta[i]));
}
   
void ModuleID::SetHistogramService(HistogramService* hist)
{
   if (fHisto!=0){
      cout << "error in  ModuleID::SetHistogramService "
           << fHisto<<endl;
      return;
   }
   fHisto=hist;
   return;
}

void ModuleID::ReportError(TString name){
   cout<< "ModuleID: "<< name<<endl;
   exit(1);
}

int ModuleID::GetEtaBin(double eta, double mineta, double delta){
   int etabin = (int) ((eta - mineta)/delta);
   if (etabin <0 ) ReportError ("negative etabin");
   if (etabin >=fMaxEtaBin)  ReportError (" too large etabin");
   return etabin;
}

int ModuleID::GetPhiBin(double phi, double delta)
{
   if (phi<0.) phi += TMath::TwoPi();
   if (phi>=TMath::TwoPi()) phi -= TMath::TwoPi();
   int phibin = (int) (phi/delta);
   if (phibin <0 ) ReportError ("negative phibin");
   if (phibin >=fMaxPhiBin)  ReportError (" too large phibin");
   return phibin;
}
inline double ModuleID::GetEtaFromBin(int bin, double mineta, double delta)
{
   return mineta + (bin+0.5) * delta;
}

inline double ModuleID::GetPhiFromBin(int bin, double delta){
   return (bin+0.5) * delta;
}
  
int ModuleID::GetIDFromPosition(double x, double y, double z, int lay)
{ 
   int id, etabin;
   TVector3 v(x,y,z);
   if (lay >= fNLayers || lay<0){
      if (fDebug) cout << "GetIDFromPosition bad layer "<< lay<< endl;
      return -1; 
   }
   double phi    = v.Phi();
   double phibin = GetPhiBin(phi, fDPhi[lay]);
   double rxy    = sqrt(x*x+y*y);
   double eta    = v.Eta();
   if (fDebug) cout <<"GetIDFromPosition start "<<x<<" "<<y<<" " <<z<<endl; 
   if (TMath::Abs(eta)>fMaxEta[lay]){
      //cout << "eta beyond sensitive detector volume"<< eta<< endl;
      return -1;
   }
   
   if (fDebug) cout << "GetID eta="<<eta << " lay=" << lay<< " Phi="<<phi<<" r="<<rxy<<endl;
   if (TMath::Abs(rxy - fR[lay])>1.e-5 && 
       TMath::Abs(TMath::Abs(z)-fZ[lay])>1.e-5) {
      cout <<"GetIDFromPosition inconsistent data "<<rxy<<" " 
           <<z<< " l=" << lay << " fR="<< fR[lay] << " fZ="<<fZ[lay]<<endl;
      //ReportError( "bad data"); 
      return -1;
   }
   id = lay * fLayerOffset;
   if (TMath::Abs(eta)<fBarrelEta[lay]){ 
      if (fDebug) cout << "barrel" <<endl;
      etabin = GetEtaBin(eta, -fBarrelEta[lay], fDEta[lay]);
      id += fBarrelOffset +fMaxPhiBin*etabin+phibin;         
   }
   else if (z<0){ 
      if (fDebug) cout << "left endcap" <<endl;
      etabin = GetEtaBin(-eta, fBarrelEta[lay], fDEta[lay]);
      id += fEndcap1Offset +fMaxPhiBin*etabin+phibin;
   }         
   else { 
      if (fDebug) cout << "right endcap" <<endl;
      etabin = GetEtaBin(eta, fBarrelEta[lay], fDEta[lay]);
      id += fEndcap2Offset +fMaxPhiBin*etabin+phibin;         
   }         

   fHisto->FastFillTH1D("ID Good=0, bad = 1", 0, 2, -0.5, 1.5);

   if (ValidID(id)) return id; 

   fHisto->FastFillTH1D("ID Good=0, bad = 1", 1, 2, -0.5, 1.5);
   fHisto->FastFillTH1D("badIdLayer", lay, 30, -0.5, 29.5);

   TVector3 tt(x,y,z);   
   fHisto->FastFillTH2D("badIdEtaR", tt.Eta(), tt.Pt(), 600, -6., 6., 500, 0., 1000.);
   fHisto->FastFillTH2D("badIdRPhi", tt.Pt(), tt.Phi(), 500, 0., 1000., 700, -3.5 , 3.5 );
   fHisto->FastFillTH2D("badIdRZ", tt.Pt(), tt.Z(), 500, 0., 1000., 1000, -2000, 2000. );
   fHisto->FastFillTH2D("badIdEtaPhi", tt.Eta(), tt.Phi(), 600, -6., 6., 700, -3.5, 3.5 );
   return -1; 
}
   
bool ModuleID::GetPositionFromID(int id, int& layer, double& x, double& y, double& z, bool& isCalo)
{ 
   if (!ValidID(id)) {
      if (fDebug) cout << "GetPositionFromID invalid id="<<id<<endl;
      return false;
   }
   int etabin, phibin;
   double eta, phi, rxy;
   layer = id / fLayerOffset;
   if (fDebug) cout << "-------------GetPositionFromID with "<<id<<" lay="<<layer<<endl;
   id = id % fLayerOffset;
   if (id>=fEndcap2Offset){
      if (fDebug) cout << "right endcap" <<endl;
      id -= fEndcap2Offset;
      etabin = id / fMaxPhiBin;
      phibin = id % fMaxPhiBin;
      eta = GetEtaFromBin(etabin, fBarrelEta[layer], fDEta[layer]);
      phi = GetPhiFromBin(phibin, fDPhi[layer]);
      z = fZ[layer];
      rxy = z / TMath::SinH(eta);
      x = rxy * cos(phi);
      y = rxy * sin(phi);
   }
   else if (id>=fEndcap1Offset){ 
      if (fDebug) cout << "left endcap" <<endl;
      id -= fEndcap1Offset;
      etabin = id / fMaxPhiBin;
      phibin = id % fMaxPhiBin;
      eta = -1. * GetEtaFromBin(etabin, fBarrelEta[layer], fDEta[layer]);
      phi = GetPhiFromBin(phibin, fDPhi[layer]);
      z = -fZ[layer];
      rxy = TMath::Abs(z / TMath::SinH(eta)); // info the missing Abs here was a badbug
      x = rxy * cos(phi);
      y = rxy * sin(phi);
   }
   else { 
      if (fDebug) cout << "barrel" <<endl;
      id -= fBarrelOffset;
      etabin = id / fMaxPhiBin;
      phibin = id % fMaxPhiBin;
      eta = GetEtaFromBin(etabin, -fBarrelEta[layer], fDEta[layer]);
      phi = GetPhiFromBin(phibin, fDPhi[layer]);
      rxy = fR[layer];
      x = rxy * cos(phi);
      y = rxy * sin(phi);
      z = rxy * TMath::SinH(eta);
   }
   if (fDebug) cout << "GetPosFromID translates to "
                    << x << " " << y << " " << z<<endl;
   isCalo = GetLayerIsCalo(layer);
   return true;
}

bool ModuleID::ValidID(int id)
{
   if (fDebug) cout << "ValidID id="<<id<<endl;
   if (id<0) return false;
   int etabin, phibin, layer;
   double eta,phi;
   layer = id / fLayerOffset;
   id = id % fLayerOffset;
   if (id>=fEndcap2Offset){ 
      id -= fEndcap2Offset;         
      etabin = id / fMaxPhiBin;         
      eta = GetEtaFromBin(etabin, fBarrelEta[layer], fDEta[layer]);
      //cout << "l="<<layer<<" eta endc2="<<eta<<" " << fMaxEta[layer] <<endl;
      if (eta >fMaxEta[layer]+1.e-2) return false;
   }
   else if (id>=fEndcap1Offset){
      id -= fEndcap1Offset;
      etabin = id / fMaxPhiBin;
      eta = -1. * GetEtaFromBin(etabin, fBarrelEta[layer], fDEta[layer]);
      //cout << "l="<<layer<<" eta endc1="<<eta<<" " << fMaxEta[layer] <<endl;
      if (eta <-fMaxEta[layer]-1.e-2) return false;
   }
   else {
      id -= fBarrelOffset;
      etabin = id / fMaxPhiBin;
      eta = GetEtaFromBin(etabin, -fBarrelEta[layer], fDEta[layer]);
      //cout << "l="<<layer<<" eta bar="<<eta<<" " << fBarrelEta[layer] <<endl;
      if (TMath::Abs(eta) >fBarrelEta[layer]+1.e-2) return false;
   }
   //cout << "l="<<layer<<" eta ok="<<etabin<<endl;
   phibin = id % fMaxPhiBin;
   if (phibin >= (int) (TMath::TwoPi()/fDPhi[layer]+1.e-2)) return false;
   //cout << "l="<<layer<<" phiok="<<phibin<<endl;
   //cout <<"id="<<id<<" is valid"<<endl;
   return true;
}





