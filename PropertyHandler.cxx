#include "PropertyHandler.h"

void PropertyHandler::PrintProperties()
{
   std::cout <<"Properties of object:"<<std::endl; 
   map<string, Property>::iterator it;
   for (it = fMap.begin();it != fMap.end(); it++)      
      std::cout <<it->first <<"="
                << GetValueString(it->first.data())
                <<" type="<<it->second.type
                <<std::endl;
}

bool PropertyHandler::KeyAlreadyDefinedError(const char* name)
{
   std::string key(name);
   std::cout << "PropertyHandler ERROR key="<<name<<" of type="<<fMap[key].type<<" is already defined, exiting..."<<std::endl;
   fIsConsistent = false;
   if (fExitOnError) throw int(1);
   return false;
}

bool PropertyHandler::KeyNotFoundError(const char* name)
{
   std::cout << "PropertyHandler could not find key="<<name<<", exiting..."<<std::endl;
   PrintProperties();
   fIsConsistent = false;
   if (fExitOnError) throw int(2);
   return false;
} 

bool PropertyHandler::WrongKeyTypeError(const char* name)
{
   std::string key(name);
   std::cout << "PropertyHandler wrong key type, key="<<name<<", type="<<fMap[key].type<<" exiting..."<<std::endl;
   fIsConsistent = false;
   if (fExitOnError) throw int(2);
   return false;
}
