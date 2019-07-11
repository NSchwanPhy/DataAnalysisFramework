#include "DICaloCluster.h"
#include "CentralStorage.h"

using namespace ExampleFramework; MyImp(DICaloCluster);

DICaloCluster::DICaloCluster() 
   :  DataItem(), 
      fCmsX(0.), fCmsY(0.), fCmsZ(0.), fEnergy(0.), fLambda(0),
      fCaloHits(), fV(0,0,0) { } 
 
   // main constructor
DICaloCluster::DICaloCluster(int uniqueid) 
   : DataItem(uniqueid), 
     fCmsX(0.), fCmsY(0.), fCmsZ(0.), fEnergy(0.), fLambda(0),
     fCaloHits(), fV(0,0,0) { }

DICaloCluster::DICaloCluster( const DICaloCluster& di, int id): DataItem(id) 
{ 
   fCmsX=di.fCmsX;
   fCmsY=di.fCmsY;
   fCmsZ=di.fCmsZ;
   fEnergy=di.fEnergy;
   fLambda=di.fLambda;
   fCaloHits = di.fCaloHits;
   fV = di.fV; 
}

void DICaloCluster::Add(const DICalorimeterHit* hit)
{ 
   if (!hit) {ReportError("CaloClusterer, trying to add empty hit pointer");} 
   //! adding a hit and updating center-of-gravity position
   double ehit = hit->GetEnergy();
   double enew = fEnergy + ehit;
   double xlayer = hit->GetLayer();
   fCmsX =  fCmsX * (fEnergy / enew)  +  hit->GetX() * (ehit / enew);
   fCmsY =  fCmsY * (fEnergy / enew)  +  hit->GetY() * (ehit / enew);
   fCmsZ =  fCmsZ * (fEnergy / enew)  +  hit->GetZ() * (ehit / enew);
   fLambda = fLambda * (fEnergy / enew)  +  xlayer * (ehit / enew);
   fV.SetXYZ( fCmsX, fCmsY,  fCmsZ);
   fEnergy = enew;
   fCaloHits.push_back(hit);
} 

void DICaloCluster::Read()
{
   //! read CaloCluster 
   // supported read/writes are double, int, DataItem*, vector<DataItem* >
   theStorage->Read(fCmsX);
   theStorage->Read(fCmsY);
   theStorage->Read(fCmsZ);
   theStorage->Read(fEnergy);
   theStorage->Read(fLambda);
   fV.SetXYZ( fCmsX, fCmsY,  fCmsZ);
   theStorage->Read(fCaloHits);
}

void DICaloCluster::Write() const 
{
   //! write CaloCluster 
   //! writing of data in the same order as in the read function
   theStorage->Write(fCmsX);
   theStorage->Write(fCmsY);
   theStorage->Write(fCmsZ);
   theStorage->Write(fEnergy);
   theStorage->Write(fLambda);
   theStorage->Write(fCaloHits);
}

double DICaloCluster::GetLayerQuantile(double fraction) const 
{
   double xquantile;
   const int maxlayer=20;
   double elayer[maxlayer];
   int iLayer;
   for (iLayer =0; iLayer<maxlayer; iLayer++) elayer[iLayer]=0.;
   
   for (int i=0;i< fCaloHits.size();i++){
      elayer[((DICalorimeterHit*)fCaloHits[i])->GetLayer()]+=((DICalorimeterHit*) fCaloHits[i])->GetEnergy();
   }
   double esum=0.;
   iLayer=0;
   while (esum< fraction*fEnergy && iLayer<maxlayer){
      esum += elayer[iLayer];
      iLayer++;
   }
   iLayer--;
   if (iLayer<=0 ) return 0.;
   if (esum - fraction*fEnergy<=0 || elayer[iLayer-1]<=1.e-10 ) return double(iLayer);
   // linear extrapolation
   xquantile= iLayer - (esum - fraction*fEnergy)/elayer[iLayer-1];
   return xquantile;
}
