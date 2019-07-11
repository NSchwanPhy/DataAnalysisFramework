#include "DIRawHit.h"
#include "CentralStorage.h"

using namespace ExampleFramework; MyImp(DIRawHit);

void DIRawHit::Read()
{
   // supported read/writes are double, int, DataItem*, vector<DataItem* >
   // read exaple theStorage->Read(fXpos); 
   theStorage->Read(fCellID);
   theStorage->Read(fPulseHeight);
}
void DIRawHit::Write() const 
{
   // write example theStorage->Write(fXpos);
   // writing of data in the same order as in the read function
   theStorage->Write(fCellID);
   theStorage->Write(fPulseHeight);
}
