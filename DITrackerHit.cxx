#include "DITrackerHit.h"
#include "CentralStorage.h"

using namespace ExampleFramework; MyImp(DITrackerHit);

//constructors
DITrackerHit::DITrackerHit() :  
   DataItem(),
   fXpos(0), fYpos(0), fZpos(0),
   fPulseHeight(0), fModule(0), 
   fLayer(0), fV(0,0,0) { } 

DITrackerHit::DITrackerHit(int uniqueid) :  
   DataItem(uniqueid), 
   fXpos(0), fYpos(0), fZpos(0),
   fPulseHeight(0), fModule(0), 
   fLayer(0), fV(0,0,0) 
{ } 

DITrackerHit::DITrackerHit(double x, double y, double z, double pulse, 
                           int module, int lay, int uniqueid) 
   : DataItem(uniqueid) , fXpos(x), fYpos(y), fZpos(z),
     fPulseHeight(pulse), fModule(module), fLayer(lay), fV(x,y,z)
{ }
   
void DITrackerHit::Read()
{
   theStorage->Read(fXpos);
   theStorage->Read(fYpos);
   theStorage->Read(fZpos);
   theStorage->Read(fPulseHeight);
   theStorage->Read(fModule);
   theStorage->Read(fLayer);
   fV.SetXYZ(fXpos,fYpos,fZpos);
}
void DITrackerHit::Write() const 
{
   theStorage->Write(fXpos);
   theStorage->Write(fYpos);
   theStorage->Write(fZpos);
   theStorage->Write(fPulseHeight);
   theStorage->Write(fModule);
   theStorage->Write(fLayer);
}
