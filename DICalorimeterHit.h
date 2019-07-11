#ifndef DICALORIMETERHIT_H
#define DICALORIMETERHIT_H


#include "TString.h"
#include "DataItem.h"
#include "TVector3.h"

namespace ExampleFramework {
/*!
  Standard Calorimeter hit, used by CaloClusterer. 
 */
class DICalorimeterHit : public DataItem
{
 private:
   double fPosX;
   double fPosY;
   double fPosZ;
   double fEnergy;
   int    fCellID;
   int    fLayer;
   TVector3 fV;
   ~DICalorimeterHit(){ } //!< private destructor
   //void SetEnergy(double e){ fEnergy =e;} 

 public:
   // constructor
 DICalorimeterHit() : DataItem() { } 
   //! standard constructor
   DICalorimeterHit(double x, double y, double z, double energy, int cellid, int layer, int uniqueid);
   
   //! dysfunctional copy constructor 
   DICalorimeterHit( const DICalorimeterHit& di) { ReportError("no copy constructor defined"); }

   //! dysfunctional assignment operator
   DICalorimeterHit& operator = ( const DICalorimeterHit& di){ ReportError("no assignment operator defined");}
   // Getter functions
   double GetEnergy() const {return fEnergy;} //!< Energy in cell in MeV.
   double GetX()      const {return fPosX;}
   double GetY()      const {return fPosY;}
   double GetZ()      const {return fPosZ;}
   double GetEta()    const {return fV.Eta();}
   double GetPhi()    const {return fV.Phi();}
   const TVector3& GetV() const {return fV;} //!< position corresponds to center of calorimeter cell
   int GetLayer() const {return fLayer;}
   MyDef(DICalorimeterHit)
protected:
   void Read();
   void Write() const;
};

}
#endif
