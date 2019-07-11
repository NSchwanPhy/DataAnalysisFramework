#include "DIElectron.h"
#include "CentralStorage.h"

using namespace ExampleFramework; 
MyImp(DIElectron); // implementation of GetName function with preprocessor macro

void DIElectron::Read()
{
   const DataItem* poi;
   theStorage->Read(poi);
   fTrack = dynamic_cast<const DITrack*>(poi);
   theStorage->Read(poi);
   fCluster = dynamic_cast<const DICaloCluster*>(poi);
}
void DIElectron::Write() const 
{
   theStorage->Write(fTrack);
   theStorage->Write(fCluster);
}
