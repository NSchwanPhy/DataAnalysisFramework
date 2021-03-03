#ifndef JetFinder_H
#define JetFinder_H

#include "Processor.h"
#include <map>
#include "DICaloJet.h"
//#include "FastKtAlgorithm.h"


namespace ExampleFramework {
/*! 
  Mother class for all Jetfinder algorithms
*/   
class JetFinder : public Processor {
 private:

 public:
   
    JetFinder(Framework* fram);

    ~JetFinder() {}

    virtual bool EventLoop()=0;
    virtual bool InitRun()=0;
    virtual bool FinalizeRun()=0;

    //usefull functions
   static bool valueComparer(map<pair<const DICaloJet*,const DICaloJet*>, double>::value_type &i1, map<pair<const DICaloJet*,const DICaloJet*>, double>::value_type &i2)
   { return i1.second < i2.second; }
   DICaloJet* Merge(const DICaloJet* di1,const DICaloJet* di2);



   TString GetName(){ return TString("JetFinder");}
};  // end class JetFinder


}   // end namespace ExampleFramework

#endif // end JetFinder_H
