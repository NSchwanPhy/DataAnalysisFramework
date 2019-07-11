#include "DICalorimeterHit.h"
#include "CentralStorage.h"
#include "TVector3.h"
#include "TString.h"

using namespace ExampleFramework; MyImp(DICalorimeterHit);

DICalorimeterHit::DICalorimeterHit(double x, double y, double z, double energy, int cellid, int layer, int uniqueid) 
   : DataItem(uniqueid) , fPosX(x), fPosY(y), fPosZ(z),
     fEnergy(energy), fCellID(cellid), fLayer(layer), fV(x,y,z)
{ }
 
void DICalorimeterHit::Read()
{
   theStorage->Read(fPosX);  
   theStorage->Read(fPosY);  
   theStorage->Read(fPosZ);  
   theStorage->Read(fEnergy);  
   theStorage->Read(fCellID);  
   theStorage->Read(fLayer);  
   fV.SetXYZ(fPosX,fPosY,fPosZ);
} 

void DICalorimeterHit::Write() const 
{
   theStorage->Write(fPosX);  
   theStorage->Write(fPosY);  
   theStorage->Write(fPosZ);  
   theStorage->Write(fEnergy);  
   theStorage->Write(fCellID);  
   theStorage->Write(fLayer);  
}
   
 
