#ifndef SERVICE_H
#define SERVICE_H

#include "Riostream.h"
#include <vector>
#include <cstdlib>
#include "PropertyHandler.h"
using namespace std;

 
namespace ExampleFramework {
   
class Framework; // forward declaration

/*! 
  Base class for all services. Still under construction.
  (c) Eckhard von Toerne, University of Bonn, April 2011
*/
class Service : public PropertyHandler {
private:
   Framework* fFramework;
protected:
   int  CreateUniqueObjectID(); 
   void ReportError(const char* typ, const char* option="");

public:   
   //! default constructor 
   Service() : PropertyHandler(), fFramework(0) {} 
   Service(Framework* f) : PropertyHandler(), fFramework(f) {} 

   virtual ~Service(){ }  //!< virtual destructor of BaseClass

   virtual const std::string& GetName() const = 0;

};

} 
#endif
