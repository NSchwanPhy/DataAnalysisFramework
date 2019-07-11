#include "DIClusteredTrackerHit.h"
#include "CentralStorage.h"

using namespace ExampleFramework; MyImp(DIClusteredTrackerHit);

void DIClusteredTrackerHit::Read()
{
   DITrackerHit::Read();
   theStorage->Read(fTrackerHits);
}
void DIClusteredTrackerHit::Write() const 
{
   DITrackerHit::Write();
   theStorage->Write(fTrackerHits);
}

void DIClusteredTrackerHit::Add(const DITrackerHit* hit)
{ 
   if (!hit) {ReportError("DIClusterTrackerHit, trying to add empty hit pointer");} 
   //! adding a hit and updating center-of-gravity position
   double ehit = hit->GetPulseHeight();
   double enew = fPulseHeight + ehit;
   if (fTrackerHits.size()==0) fLayer = hit->GetLayer();
   else if (fLayer != hit->GetLayer()) ReportError("DIClusterTrackerHit, trying to add hit from different layer");

   fXpos =  fXpos * (fPulseHeight / enew)  +  hit->GetX() * (ehit / enew);
   fYpos =  fYpos * (fPulseHeight / enew)  +  hit->GetY() * (ehit / enew);
   fZpos =  fZpos * (fPulseHeight / enew)  +  hit->GetZ() * (ehit / enew);
   //cout << "lay="<<fLayer<<" nhit="<<fTrackerHits.size()<<" addHit X="<<fXpos<< ",Y="<<fYpos<<" Z="<<fZpos<<endl;
   fV.SetXYZ( fXpos, fYpos,  fZpos);
   fPulseHeight = enew;
   fTrackerHits.push_back(hit);
} 
