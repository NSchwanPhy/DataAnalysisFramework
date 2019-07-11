#ifndef TrackFinder_H
#define TrackFinder_H

#include "Processor.h"
#include "DITrackerHit.h"
#include "DITrackStub.h"

namespace ExampleFramework {

 /*!
   A rather crude trackfinder which is about 1000 times shorter and thus simpler than normal track finding algorithms. This serves just as an example on how a trackfinder works in the general framework. 
   Momentum resolution is about 20% at 10 GeV. This finder combines hits 
   in different layers if they match in eta-phi space. 
   Finder is not capable to do real pattern recognition.
   Please not that the TrackFinder runs by default on the TrackerHits. 
   Better performance is achieved if run on ClusteredTrackerHits. 
   See class SiliconClusterer for details.
   (c) E. von Toerne, U. of Bonn 2010
  */    
class TrackFinder : public Processor {
 private:
   vector<const DITrackerHit*> fHits;
   vector<DITrackStub*> fStubs;
   int fOutermostTrackerLayer;
   bool fHitsUsedJustOnce;
   double fCurvatureCorrection;
 public:
   
   // constructor 
   TrackFinder(Framework* fram);

   // destructor
   ~TrackFinder(){ }

   bool EventLoop();   //!< is executed in every event

   bool CreateTrackStubs();
   bool AddHitToTrackStub(DITrackStub* tr, DITrackerHit* h);
   bool CreateTracks();

   bool InitRun();     //!< is executed at begin of job

   bool FinalizeRun(); //!< is executed at end of job

   TString GetName(){ return TString("TrackFinder");}

};  // end class TrackFinder


}   // end namespace ExampleFramework

#endif // end TrackFinder_H
