#include "DIEventNumber.h"
#include "CentralStorage.h"

using namespace ExampleFramework; MyImp(DIEventNumber);

void DIEventNumber::Read()
{
   theStorage->Read(fEventNumber);
}
void DIEventNumber::Write() const
{
   theStorage->Write(fEventNumber);
}

