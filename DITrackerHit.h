#ifndef DITRACKERHIT_H
#define DITRACKERHIT_H

#include "DataItem.h"
#include "TString.h"
#include "TVector3.h"

namespace ExampleFramework {

// data either belong to an event or are static and are only stored once  
//! standard tracker hit of a pixelized tracker. A typical trackerlayer is much thicker than a normal tracker. Energy loss is a few MeV.
class DITrackerHit : public DataItem
{
protected:
   double fXpos;
   double fYpos;
   double fZpos;
   TVector3 fV; //!< position of center of pixel cell
   double fPulseHeight;
   int    fModule; //!< Each tracker cell has a different module id.
   int    fLayer;  //!< The tracker is arranged in layers. Each layer is a cylindric shell

   ~DITrackerHit(){ } //!< private destructor
   
 public:
   DITrackerHit();
   //! standard constructor for read
   DITrackerHit(int uniqueid);
   DITrackerHit(double x, double y, double z, double pulse, 
                int module, int lay, int uniqueid); 
   //! dysfunctional copy constructor
   DITrackerHit( const DITrackerHit& di) { ReportError("no copy constructor defined"); }
   //! dysfunctional assignment operator
   DITrackerHit& operator = ( const DITrackerHit& di){ ReportError("no assignment operator defined");}
   
   double GetPulseHeight() const {return fPulseHeight;}
   double GetX() const {return fXpos;}
   double GetY() const {return fYpos;}
   double GetZ() const {return fZpos;}
   int GetLayer() const {return fLayer;}
   //! Get position of pixel cell center
   const TVector3& GetV() const {return fV;} 
   MyDef(DITrackerHit) 
 protected:
   virtual void Read(); //!< read function called by CentralStorage
   virtual void Write() const; //!< write function called by CentralStorage

};

}
#endif
