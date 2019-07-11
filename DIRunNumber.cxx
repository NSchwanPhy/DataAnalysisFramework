#include "DIRunNumber.h"
#include "CentralStorage.h"

using namespace ExampleFramework; MyImp(DIRunNumber);

void DIRunNumber::Read() 
{
  theStorage->Read(fRun);
}
void DIRunNumber::Write() const 
{
  theStorage->Write(fRun);
}
