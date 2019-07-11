#include "DataItem.h"
#include "CentralStorage.h"

using namespace ExampleFramework;

// constructor
DataItem::DataItem(int iD) 
   : fClean(false), fUniqueID(iD)
{ 
   theStorage->RegisterID(fUniqueID,this);
} 

DataItem::DataItem() 
    : fClean(false), fUniqueID(-1)  
{ 

}

DataItem::~DataItem() 
{ 
   theStorage->DeregisterID(fUniqueID);
}

CentralStorage* DataItem::theStorage = 0;

void DataItem::ReportError(const std::string& typ)
{
   cout << "problem in DataItem::" << typ<< " exiting from class" 
        << GetName() << endl;
   exit(1);
}

void DataItem::PRead()
{
   if (fClean) ReportError("PRead, dataitem has already been filled");
   fClean = true;
   theStorage->Read(fUniqueID);
   theStorage->RegisterID(fUniqueID,this);
}

void DataItem::PWrite() const
{
   theStorage->Write(fUniqueID);
}
