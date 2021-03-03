#include "DICaloJet.h"
#include "CentralStorage.h"

using namespace ExampleFramework; 
MyImp(DICaloJet); // implementation of GetName function with preprocessor macro

DICaloJet::DICaloJet() : DataItem(), fCmsX(0.), fCmsY(0.), fCmsZ(0.), fEnergy(0.),
   fCaloClusters(), fV(0,0,0) { }

DICaloJet::DICaloJet(int uniqueid) : DataItem(uniqueid), fCmsX(0.), fCmsY(0.), fCmsZ(0.), fEnergy(0.),
   fCaloClusters(), fV(0,0,0) { }

void DICaloJet::Add(const DICaloCluster* cluster){

   if(!cluster) {ReportError("CaloJet, trying to add empty cluster pointer");}
   //! adding a cluster and updating center-of-gravity position
   double ecluster = cluster->GetEnergy();
   double enew = fEnergy + ecluster;
   fCmsX =  fCmsX * (fEnergy / enew)  +  cluster->GetX() * (ecluster / enew);
   fCmsY =  fCmsY * (fEnergy / enew)  +  cluster->GetY() * (ecluster / enew);
   fCmsZ =  fCmsZ * (fEnergy / enew)  +  cluster->GetZ() * (ecluster / enew);
   fV.SetXYZ( fCmsX, fCmsY, fCmsZ);
   fEnergy = enew;
   fCaloClusters.push_back(cluster);
}

void DICaloJet::Read()
{
   //! read CaloJet
   // supported read/writes are double, int, DataItem*, vector<DataItem* >
   theStorage->Read(fCmsX);
   theStorage->Read(fCmsY);
   theStorage->Read(fCmsZ);
   theStorage->Read(fEnergy);
   fV.SetXYZ( fCmsX, fCmsY,  fCmsZ);
   theStorage->Read(fCaloClusters);
}
void DICaloJet::Write() const 
{
   //! write CaloJet
   // writing of data in the same order as in the read function
   theStorage->Write(fCmsX);
   theStorage->Write(fCmsY);
   theStorage->Write(fCmsZ);
   theStorage->Write(fEnergy);
   theStorage->Write(fCaloClusters);
}

double DICaloJet::GetDij(TVector3 Vj) const {
   double kti = this->GetKt();
   double ktj = Vj.Pt();
   double DeltaR = fV.DeltaR(Vj);
   return min(kti*kti, ktj*ktj) * DeltaR;
}

void DICaloJet::SetV(TVector3 V){
   fV = V;
   double* XYZ;
   fV.GetXYZ(XYZ);
   fCmsX = XYZ[0];
   fCmsY = XYZ[1];
   fCmsZ = XYZ[2];

}