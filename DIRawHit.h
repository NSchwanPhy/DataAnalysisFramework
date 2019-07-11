#ifndef DIRawHit_H
#define DIRawHit_H

#include "DataItem.h"
#include "TString.h"

namespace ExampleFramework {
/*! 
  Raw hit class, for both raw calo hits and raw tracker hits. Nuch smaller data size compared to standard tracker or calo hits
*/   
class DIRawHit : public DataItem
{
 private:
   // data 
   int    fCellID;      //!< Global CellId
   double fPulseHeight; //!< pulse height, proportional to the deposited energy
   ~DIRawHit(){ } //!< private destructor
   
 public:
   // empty constructor
   DIRawHit() :  DataItem(), fCellID(0), fPulseHeight(0.) { } 
   //! main constructor for read
   DIRawHit(int uniqueid)
      : DataItem(uniqueid), fCellID(0), fPulseHeight(0.) { }
   // main constructor
   DIRawHit(int uniqueid, int id, double pulse)
      : DataItem(uniqueid),fCellID(id), fPulseHeight(pulse) { }   
   //! dysfunctional copy constructor
   DIRawHit( const DIRawHit& di) { ReportError("no copy constructor defined");}
   //! dysfunctional assignment operator
   DIRawHit& operator = ( const DIRawHit& di){ ReportError("no assignment operator defined");}
   // Getter Functions
   int GetCellID() const { return fCellID;}
   double GetPulseHeight() const { return fPulseHeight;}
   
   MyDef(DIRawHit) //!< implementation of GetName function
protected:
   void Read();
   void Write() const;
};

}
#endif
