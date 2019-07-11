#ifndef DIElectron_H
#define DIElectron_H

#include "DataItem.h"
#include "TString.h"
#include "DITrack.h"
#include "DICaloCluster.h"

namespace ExampleFramework {
/*! 
  Example DataItem, please add decription of processor here
*/   
class DIElectron : public DataItem
{
 private:
   // data 
   const DITrack* fTrack;
   const DICaloCluster* fCluster;
   
   ~DIElectron(){ } //!< private destructor
protected:
   void Read();
   void Write() const;
 public:
   // main constructor
   DIElectron(DITrack* trk, DICaloCluster* clus, int uniqueid) : DataItem(uniqueid) , fTrack(trk), fCluster(clus) {}
   
   // empty constructor
   DIElectron() :  DataItem() { } 
   // main constructor
   DIElectron(int uniqueid) 
      : DataItem(uniqueid) 
   { }
   
   //! dysfunctional copy constructor
   DIElectron( const DIElectron& di) { ReportError("no copy constructor defined"); }
   //! dysfunctional assignment operator
   DIElectron& operator = ( const DIElectron& di){ ReportError("no assignment operator defined");}
   
   // Getter Functions
   const DITrack* GetTrack() const {return fTrack;}
   const DICaloCluster* GetCluster() const {return fCluster;}
   float GetEoverP() const {return fCluster->GetEnergy()/fTrack->GetMomentumAtPCA().Mag();} // this return cluster energy divided by track momentum
   int GetCharge() const {return int(fTrack->GetCharge());} 

   // implementation of GetName function with preprocessor macro
   MyDef(DIElectron) 
};

}
#endif
