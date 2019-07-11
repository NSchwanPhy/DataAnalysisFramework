#ifndef CaloRawHitProcessor_H
#define CaloRawHitProcessor_H

#include "Processor.h"
#include "ModuleID.h"

namespace ExampleFramework {
   
class CaloRawHitProcessor : public Processor {
 private:
   ModuleID fModuleTranslator;
 public:
   
   // constructor 
   CaloRawHitProcessor(Framework* fram);

   // destructor
   ~CaloRawHitProcessor(){ }

   bool EventLoop();   //!< is executed in every event

   bool InitRun();     //!< is executed at begin of job

   bool FinalizeRun(); //!< is executed at end of job

   TString GetName(){ return TString("CaloRawHitProcessor");}

};  // end class CaloRawHitProcessor


}   // end namespace ExampleFramework

#endif // end CaloRawHitProcessor_H
