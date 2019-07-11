#ifndef EXAMPLEPROCESSOR_H
#define EXAMPLEPROCESSOR_H

#include "Processor.h"

namespace ExampleFramework {
/*! 
  Example Processor, please add decription of processor here
*/   
class ExampleProcessor : public Processor {
 private:

 public:
   
   // constructor 
   ExampleProcessor(Framework* fram);

   // destructor
   ~ExampleProcessor(){ }

   bool EventLoop();   //!< is executed in every event

   bool InitRun();     //!< is executed at begin of job

   bool FinalizeRun(); //!< is executed at end of job

   TString GetName(){ return TString("ExampleProcessor");}
};  // end class ExampleProcessor


}   // end namespace ExampleFramework

#endif // end EXAMPLEPROCESSOR_H
