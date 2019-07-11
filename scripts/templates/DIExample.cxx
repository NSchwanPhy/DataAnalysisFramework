#include "DIExample.h"
#include "CentralStorage.h"

using namespace ExampleFramework; 
MyImp(DIExample); // implementation of GetName function with preprocessor macro

void DIExample::Read()
{
   // supported read/writes are double, int, DataItem*, vector<DataItem* >
   // read exaple theStorage->Read(fXpos);
}
void DIExample::Write() const 
{
   // write example theStorage->Write(fXpos);
   // writing of data in the same order as in the read function
}
