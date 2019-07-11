#ifndef DIVertex_H
#define DIVertex_H

#include "DataItem.h"
#include "TString.h"
#include "TVector3.h"

namespace ExampleFramework {
/*! 
  simple Vertex class, no errors on vertex position. 
*/   
class DIVertex : public DataItem
{
 private:
   // data 
   double fX,fY,fZ;
      
   ~DIVertex(){ } //!< private destructor
protected:
   void Read();
   void Write() const;
 public:
   // empty constructor
   DIVertex() :  DataItem() { } 
   // main constructor
   DIVertex(int uniqueid) 
      : DataItem(uniqueid) 
   { }   
   //! dysfunctional copy constructor
   DIVertex( const DIVertex& di) { ReportError("no copy constructor defined"); }
   //! dysfunctional assignment operator
   DIVertex& operator = ( const DIVertex& di){ ReportError("no assignment operator defined");}
   
   // Getter Functions
   //double GetPulseHeight(){return fPulseHeight;} //!< example getter function
   TVector3 GetPosition() const { return TVector3(fX,fY,fZ);}
   
   // implementation of GetName function with preprocessor macro
   MyDef(DIVertex) 
};

}
#endif
