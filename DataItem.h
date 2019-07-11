#ifndef DATAITEM_H
#define DATAITEM_H


#include <cstdlib>
#include "Riostream.h"
#include "TString.h"
#include <string>
#include "DataCollection.h"

namespace ExampleFramework { 

#define stringof(X) #X

// preprocessor macro to define class name specific functions, similar to ClassDef macro"
#define MyDef(VAR) \
private: \
static const std::string fDefName; \
public: \
virtual const std::string& GetName() const { return fDefName;}

#define MyImp(VAR) \
const std::string VAR::fDefName=stringof(VAR)


class CentralStorage;

//! Base class for all data objects, (c) E. von Toerne, U. of Bonn 2009
class DataItem{

friend class CentralStorage;
   
private:
   int  fUniqueID;   //!< ID of data item. Unique for this event
   bool fClean;      //!< Flags consistent data
protected: 
   static CentralStorage* theStorage; // ToDo improve data safety, make sure only safe functions may be called 
   // virtual base class destructor
   virtual ~DataItem();
   virtual void Read() =0;
   virtual void Write() const =0;
   void ReportError(const std::string& typ);
   void PRead();
   void PWrite() const;
   int  GetUniqueID() const { return fUniqueID; }
public:
   // constructors
   DataItem(int iD); 
   DataItem();
   virtual const std::string& GetName() const = 0; //!< function to be implemented by preprocessor macros MyDef(classname); and MyImp(classname)
   bool IsClean() const {return fClean;}    //!< If return status false, DataItem has not been initialized.

};

   
} 

#endif
