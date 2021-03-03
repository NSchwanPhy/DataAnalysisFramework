#ifndef DICaloJet_H
#define DICaloJet_H

#include "DataItem.h"
#include "DICaloCluster.h"
#include "TString.h"
#include "TVector3.h"
#include "math.h"

namespace ExampleFramework {
/*! 
  Jet data Class used for all jet finder alogrithms 
*/   
class DICaloJet : public DataItem
{
 private:
   // data 
   double fCmsX;
   double fCmsY;
   double fCmsZ;
   double fEnergy;
   TVector3 fV;
   vector<const DataItem*> fCaloClusters;
   ~DICaloJet(){ } //!< private destructor
protected:
   void Read();
   void Write() const;
 public:
   // empty constructor
   DICaloJet(); 
   // main constructor
   DICaloJet(int uniqueid);
   //! dysfunctional copy constructor
   DICaloJet( const DICaloJet& di) { ReportError("no copy constructor defined"); }
   //! dysfunctional assignment operator
   DICaloJet& operator = ( const DICaloJet& di){ ReportError("no assignment operator defined");}
   void Add(const DICaloCluster* cluster);
   void Add(const DataItem* cluster){Add(dynamic_cast <const DICaloCluster*>(cluster)); }
  

   // Getter Functions
   double GetKt() const { return fV.Pt(); }
   double GetDij(TVector3 Vj) const;
   double GetEnergy() const { return fEnergy; }
   double GetX() const { return fV.X(); }
   double GetY() const { return fV.Y(); }
   double GetZ() const { return fV.Z(); }
   double GetEta() const {return fV.Eta();}
   double GetPhi() const {return fV.Phi();}
   int GetSize() const {return fCaloClusters.size();}
   const TVector3& GetV() const {return fV;}
   vector <const DataItem* > GetCaloClusters() const { return fCaloClusters; }
   // Setter Functions
   void SetEnergy(double Energy) { fEnergy = Energy; }
   void SetCmsX(double CmsX)     { fCmsX = CmsX; fV.SetX(CmsX); }
   void SetCmsY(double CmsY)     { fCmsY = CmsY; fV.SetY(CmsY); }
   void SetCmsZ(double CmsZ)     { fCmsZ = CmsZ; fV.SetZ(CmsZ); }
   void SetCaloClusters(vector<const DataItem*> CaloClusters) { fCaloClusters = CaloClusters; }
   void SetV(TVector3 V);         
   




   MyDef(DICaloJet) 
};

}
#endif
