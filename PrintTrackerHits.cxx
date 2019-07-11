#include "PrintTrackerHits.h"
#include "TString.h"
#include "math.h"
#include "TVector3.h"
#include "DITrackerHit.h"
#include "DICalorimeterHit.h"

using namespace ExampleFramework;
      
// constructor 
PrintTrackerHits::PrintTrackerHits(Framework* fram) 
  : Processor("PrintTrackerHits", fram)
{
   DefineProperty("InputCollection", &fInputCollectionName, "TrackerHitCollection");
}

bool PrintTrackerHits::EventLoop()
{
   const DataCollection* inputs = fStorage->RetrieveData(fInputCollectionName, TString("donotcare")  );
   int evtNo = GetEventNumber();
   if (evtNo<3) cout << "PrintTrackerHits found container"<< fInputCollectionName
                      << " with nelements=" << inputs->size()<<endl;   
   const double rmax=700.;
   const double zmax=1600;
   // loop over inputs and create output
   for (int i=0;i<inputs->size();i++){
      const DITrackerHit* hit = dynamic_cast< const DITrackerHit* >((*inputs)[i]); 
      if (evtNo<3 && i<5) cout << " hit has pulseHeight=" << hit->GetPulseHeight()<<endl;
      double x =  hit->GetX();
      double y =  hit->GetY();
      double z =  hit->GetZ();
      TVector3 v(x,y,z);
      
      // book histos 
      fHistoService->FastFillTH1D("TrackerHitPulseHeight",
                           hit->GetPulseHeight(), 200, 0., 100.);
      fHistoService->FastFillTH2D("Tracker2D", x, y, 
                           500, -rmax, rmax, 
                           500, -rmax, rmax);
      fHistoService->FastFillTH2D("Tracker2D_yz", z, y, 
                           500, -zmax, zmax, 
                           500, -rmax, rmax);
      if (evtNo<10) fHistoService->FastFillTH2D(TString::Format("Tracker2D_yz%d",evtNo), z, y, 
                                          500, -zmax, zmax, 
                                          500, -rmax, rmax);
      if (evtNo<10) fHistoService->FastFillTH2D(TString::Format("Tracker2D%d",evtNo), x, y, 
                                          500, -rmax, rmax, 
                                          500, -rmax, rmax);
      if (evtNo<10) fHistoService->FastFillTH2D(TString::Format("Tracker2D%d_etaphi",evtNo), v.Eta(), v.Phi(), 
                                          500, -5., 5., 
                                          500, -5., 5.);
   }   

    const DataCollection* calo = fStorage->RetrieveData("CalorimeterHitCollection",
                                                   TString("DICalorimeterHit")  );
    for (int i=0;i<calo->size();i++){
       const DICalorimeterHit* hit = dynamic_cast< const DICalorimeterHit* >((*calo)[i]); 
       double x =  hit->GetX();
       double y =  hit->GetY();
       double z =  hit->GetZ(); 
       double rxy = sqrt(x*x+y*y);
       TVector3 v(x,y,z);
       fHistoService->FastFillTH1D("CaloHitPulseHeight",hit->GetEnergy(), 
                            200, 0., 60000.);
       fHistoService->FastFillTH2D("Calo2D", x, y, 
                            500, -rmax, rmax, 
                            500, -rmax, rmax);
       fHistoService->FastFillTH2D("Calo2D_yz", z, y, 
                            500,  zmax, zmax, 
                            500, -rmax, rmax);
       if (evtNo<10) fHistoService->FastFillTH2D(TString::Format("Calo2D_yz%d",evtNo), z, y, 
                            500, -zmax , zmax, 
                            500, -rmax, rmax);
       if (evtNo<10) fHistoService->FastFillTH2D(TString::Format("Calo2D%d",evtNo), x, y, 
                            500, -rmax, rmax, 
                            500, -rmax, rmax);
       if (evtNo<20){
          int en = hit->GetEnergy()/500; 
          for (int j=0;j<en;j++)
             fHistoService->FastFillTH2D(TString::Format("Calo2D%d_etaphi",evtNo), v.Eta(), v.Phi(), 
                                  500, -5., 5., 
                                  500, -5., 5.);
       }
    }

   return true;

}


