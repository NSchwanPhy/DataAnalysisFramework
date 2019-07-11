#ifndef DICaloCluster_H
#define DICaloCluster_H

#include "DataItem.h"
#include "DICalorimeterHit.h"
#include "TString.h"
#include "TVector3.h"
#include "math.h"

namespace ExampleFramework {

class DICaloCluster : public DataItem
{
 private:
   // data 
   double fCmsX;
   double fCmsY;
   double fCmsZ;
   double fEnergy;
   double fLambda;
   vector <const DataItem* > fCaloHits;
   TVector3 fV;
   ~DICaloCluster(){ } //!< private destructor
   
 public:
   // empty constructor
   DICaloCluster();
   // main constructor
   DICaloCluster(int uniqueid);
   //! copy constructor with new dataItemId
   DICaloCluster( const DICaloCluster& di, int id);  
   //! dysfunctional assignment operator
   DICaloCluster& operator = ( const DICaloCluster& di){ ReportError("no assignment operator defined");}   
   void Add(const DICalorimeterHit* hit);
   void Add(const DataItem* hit){Add(dynamic_cast <const DICalorimeterHit*>(hit));}
   // Getter Functions   
   double GetEnergy() const {return fEnergy;}
   double GetEt() const {return fEnergy*sin(fV.Theta());}
   double GetLambda() const {return fLambda;} //!< longitudinal shower shape: Average layer number
   double GetX() const {return fV.X();}
   double GetY() const {return fV.Y();}
   double GetZ() const {return fV.Z();}
   double GetEta() const {return fV.Eta();}
   double GetPhi() const {return fV.Phi();}
   int GetSize() const {return fCaloHits.size();} 
   const TVector3& GetV() const {return fV;}
   TVector3 GetMomentum() const {return (fEnergy/fV.Mag())*fV;}
   double GetLayerQuantile(double fraction) const; //!< layer up to which the given fraction of cluster energy is deposited

   MyDef(DICaloCluster) 
protected:
   void Read();
   void Write() const;
};

}
#endif
