#include "DIVertex.h"
#include "CentralStorage.h"

using namespace ExampleFramework; 
MyImp(DIVertex); // implementation of GetName function with preprocessor macro

void DIVertex::Read()
{
   theStorage->Read(fX);
   theStorage->Read(fY);
   theStorage->Read(fZ);
}
void DIVertex::Write() const 
{
   theStorage->Write(fX);
   theStorage->Write(fY);
   theStorage->Write(fZ);
}
