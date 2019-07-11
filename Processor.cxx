#include "Processor.h"
#include "Framework.h"
#include "DIEventNumber.h"

using namespace ExampleFramework;

Processor::Processor(const char* title, Framework* fram) 
   : fID(0), fIDBeenSet(false), fProcessorTitle(title), 
     fStorage(0), fFramework(fram), fHistoService(0)
{ 
   if (fProcessorTitle=="" || fProcessorTitle==" ") fProcessorTitle=fProcessorName; 
}

void Processor::SetTitle(const char* title)
{ 
   if (!fIDBeenSet) fProcessorTitle=title;
   else ReportError("Processor has been frozen, not possible to set title anymore");
}

int Processor::CreateUniqueObjectID()
{
   return fFramework->GetUniqueObjectID();
}

void Processor::ReportError(const char* typ, const char* option)
{
   cout << "problem in Processor::" << fProcessorTitle<< " exiting ... id="
        << fID << " problem is: " << fProcessorName  <<" "<<option<< endl;
   exit(1);
}

bool Processor::SetUp(int ID, CentralStorage* storage, HistogramService* hist)
{ 
   if (!fIDBeenSet){
      fID=ID; 
      fIDBeenSet=true;
      fStorage = storage;
      fHistoService = hist;
      return true;
   }
   else { 
      cout << "ERROR ID already been set" << endl;
      return false;
   }
   return false;
}


int Processor::GetEventNumber() const
{
   const DataCollection* const evt =  fStorage->RetrieveData("EventNumber","DIEventNumber");
   return ((DIEventNumber* const) (*evt)[0])->GetEventNumber();
}
