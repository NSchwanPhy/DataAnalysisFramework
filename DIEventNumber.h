#ifndef DIEVENTNUMER_H
#define DIEVENTNUMER_H

#include "TString.h"
#include "DataItem.h"

namespace ExampleFramework {

//! class stores event number, Each pocessor inherits a member function that provides easy access to event number Processor::GetEventNumber()
class DIEventNumber : public DataItem
{
private:
   int    fEventNumber;
   ~DIEventNumber(){ } //!< private destructor
   
public:
   // constructor
   DIEventNumber() : DataItem() { } 
   
   DIEventNumber(int evtno) 
      : DataItem(-1), fEventNumber(evtno)
   { }
   
   // dysfunctional copy constructor 
   DIEventNumber( const DIEventNumber& ev) { ReportError("no copy constructor defined"); }
   // dysfunctional assignment operator
   DIEventNumber& operator = ( const DIEventNumber& ev){ ReportError("no assignment operator defined");}
   int GetEventNumber() const {return fEventNumber;}
   MyDef(DIEventNumber) 
protected:
   void Read();
   void Write() const;
};

}
#endif
