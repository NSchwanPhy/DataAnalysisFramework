#ifndef DIEXAMPLE_H
#define DIEXAMPLE_H

#include "DataItem.h"
#include "TString.h"

namespace ExampleFramework {
/*! 
  Example DataItem, please add decription of processor here
*/   
class DIExample : public DataItem
{
 private:
   // data 

   ~DIExample(){ } //!< private destructor
protected:
   void Read();
   void Write() const;
 public:
   // empty constructor
   DIExample() :  DataItem() { } 
   // main constructor
   DIExample(int uniqueid) 
      : DataItem(uniqueid) 
   { }   
   //! dysfunctional copy constructor
   DIExample( const DIExample& di) { ReportError("no copy constructor defined"); }
   //! dysfunctional assignment operator
   DIExample& operator = ( const DIExample& di){ ReportError("no assignment operator defined");}
   
   // Getter Functions
   //double GetPulseHeight(){return fPulseHeight;} //!< example getter function
   
   // implementation of GetName function with preprocessor macro
   MyDef(DIExample) 
};

}
#endif
