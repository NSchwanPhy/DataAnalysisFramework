#ifndef DIRunNumber_H
#define DIRunNumber_H

#include "DataItem.h"
#include "TString.h"

namespace ExampleFramework {

class DIRunNumber : public DataItem
{
private:
   // data 
   int fRun;

   ~DIRunNumber(){ } //!< private destructor
   
public:
   //! empty constructor
   DIRunNumber() :  DataItem() { } 
 
   //! main constructor
   DIRunNumber(int run) 
      : DataItem(-2), fRun(run) 
   { }
   
   //! dysfunctional copy constructor
   DIRunNumber( const DIRunNumber& di) { ReportError("no copy constructor defined"); }
   //! dysfunctional assignment operator
   DIRunNumber& operator = ( const DIRunNumber& di){ ReportError("no assignment operator defined");}
   
   //! Get run number
   double GetRun(){return fRun;}
   
   MyDef(DIRunNumber)
protected:
   void Read();
   void Write() const;
};

}
#endif
