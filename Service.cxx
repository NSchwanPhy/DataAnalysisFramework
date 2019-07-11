#include "Service.h"
#include "Framework.h"

using namespace ExampleFramework;


int Service::CreateUniqueObjectID()
{
   return fFramework->GetUniqueObjectID();
}

void Service::ReportError(const char* typ, const char* option)
{
   cout << "problem in Service::" << GetName() << " exiting ... ="
        << " problem is: " << typ <<" "<<option<< endl;
   //exit(1);
   throw int(1);
}




