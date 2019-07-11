#ifndef DITrackStub_H
#define DITrackStub_H

#include "DataItem.h"
#include "TString.h"
#include "DITrackerHit.h"

namespace ExampleFramework {

   //! Helper object for track finding algorithms. Hits should be added hits from outside to inside.
class DITrackStub : public DataItem
{
 friend class DITrack;
 friend class TrackFinder; // !< friend declaration allows trackfinder to delete trackstubs not handed to centralstorage

 private:
   // data 
   vector <const DITrackerHit* > fHits;
   bool fDone;
   ~DITrackStub(){ } //!< private destructor
   
 public:
   // empty constructor
   DITrackStub() :  DataItem() { } 
 
   //! main constructor
   DITrackStub(const DITrackerHit* hit, int uniqueid) 
      : DataItem(uniqueid), fHits(), fDone(false) 
   { fHits.push_back(hit);}
   
   //! dysfunctional copy constructor
   DITrackStub( const DITrackStub& di) { ReportError("no copy constructor defined"); }
   //! dysfunctional assignment operator
   DITrackStub& operator = ( const DITrackStub& di){ ReportError("no assignment operator defined");}
   
   // Getter Functions
   int GetNHits() const {return fHits.size();}
   int GetNextLayer(){ return ((DITrackerHit*) fHits[fHits.size()-1])->GetLayer()-1;}
   double GetDistance(const DITrackerHit* hit) const {return (hit->GetV()-fHits[fHits.size()-1]->GetV()).Mag();}
   bool AddHit(const DITrackerHit* hit){fHits.push_back(hit);return true;}
   void ForceToBeDone(){fDone=true;}
   bool IsDone() const {return fDone || (fHits[fHits.size()-1]->GetLayer()==0);}

   MyDef(DITrackStub) 
protected:
   void Read();
   void Write() const;
};

}
#endif
