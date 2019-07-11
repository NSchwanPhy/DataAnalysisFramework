#ifndef DITrack_H
#define DITrack_H

#include "DITrackerHit.h"
#include "TVector3.h"

namespace ExampleFramework {

class DITrackStub;
/*!
 Reconstructed particle track 
 (c) E. von Toerne, U. of Bonn 2009
 */
class DITrack : public DataItem
{
 private:
   // data 
   vector <const DITrackerHit* > fHits;
   TVector3 fMomentumAtPCA;
   double fKappa, fPhi0, fTheta, fD0, fZ0; // track parameters
   TVector3 fPCA;

   ~DITrack(){ } //!< private destructor

   void FillMomentumAtPCA(); //!< fills momentum 3-vector  

 public:
   //! empty constructor
   DITrack() :  DataItem() { } 
   //! main constructor
   DITrack(int uniqueid, const DITrackStub* stub, double kappa, double phi0, double theta, double d0, double z0);   
   //! copy constructor
   DITrack( const DITrack& di) : fHits(di.fHits), fMomentumAtPCA(di.fMomentumAtPCA), fKappa(di.fKappa), fPhi0(di.fPhi0), fTheta(di.fTheta), fD0(di.fD0), fZ0(di.fZ0){  }
   //! assignment operator
   DITrack& operator = ( const DITrack& di){ fHits = di.fHits; fMomentumAtPCA = di.fMomentumAtPCA; fKappa = di.fKappa; fPhi0 = di.fPhi0; fTheta = di.fTheta; fD0 = di.fD0; fZ0 = di.fZ0;}
   
   // Getter Functions
   //! The momentum at the PCA is usually the best estimate for the momentum before the track is bent 
   const TVector3& GetMomentumAtPCA() const {return fMomentumAtPCA;}
   const TVector3& GetPCA() const { return fPCA;}
   const vector <const DITrackerHit* >& GetHits() const { return fHits;}
   double GetEta() const {return fMomentumAtPCA.Eta();}
   double GetPhi() const {return fMomentumAtPCA.Phi();}
   double GetPt()  const {return fMomentumAtPCA.Perp();}
   double GetCharge() const {if (fKappa <0.) return -1.; else return 1.;}
   MyDef(DITrack) 
 protected:
   void Read();
   void Write() const; 
};

}
#endif
