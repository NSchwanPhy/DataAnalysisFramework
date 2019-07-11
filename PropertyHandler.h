#ifndef PROPERTYHANLDER_H
#define PROPERTYHANLDER_H

//  E. von Toerne, U. of Bonn 2010

#include <iostream>
#include <map>
#include <string>
#include "TString.h"
#include <cstdlib>
using std::map;
using std::string;
using std::pair;

class Property{
public:
   Property() : poi(0), type("null") { } // empty property
   Property(void* p, string t) : poi(p), type(t) { }
   void* poi;
   string type;   
};

/*! Light-weight implementation of a propertyhandler with strong type-checking. Much simpler than a full-scale property handler. Uses function templates;

 (c) E. von Toerne, U. of Bonn 2010, reworked 2015
 */
class PropertyHandler {

private:
   static const bool fExitOnError=true;
   bool fIsConsistent; // //!< false if DeclarePropertyCommands are inconsistent
   map<string,Property> fMap;         //!< map of all keys, name and type
   bool KeyAlreadyDefinedError(const char* name);
   bool KeyNotFoundError(const char* name);
   bool WrongKeyTypeError(const char* name);
protected:
   template <typename VAR>
   bool DefineProperty(const char* name, VAR * thepoi, VAR defValue){
      string key(name);
      if (fMap.find(key)==fMap.end()) {
         *thepoi=defValue;
         fMap[key] = Property(thepoi,PointerType(thepoi));
         return true;
      }
      return KeyAlreadyDefinedError(name);
   }
public:
   template <typename VAR>
   bool SetProperty(const char* name, VAR value){
      VAR* pointer;
      string key(name);
      if (fMap.find(key)==fMap.end()) return KeyNotFoundError(name);
      if (PointerType(pointer)!=fMap[key].type) return WrongKeyTypeError(name);      
      void* voidpointer= fMap[key].poi;         
      pointer = (VAR*)(voidpointer);
      *pointer=value;
      return true;      
   } 
   void PrintProperties();
   bool IsConsistent(){ return fIsConsistent;}

   PropertyHandler() : fIsConsistent(true) { }
   virtual ~PropertyHandler() { }

   template <typename VAR>
   VAR GetProperty(const char* name){
      string key(name);
      VAR* pointer;
      void* voidpointer= fMap[key].poi; 
      if (PointerType(pointer)!=fMap[key].type) return WrongKeyTypeError(key.data());       if (fMap.find(key)==fMap.end()) KeyNotFoundError(key.data());      
      return *((VAR*)(voidpointer));
   }

   // stuff that is explicitly type-dependent
   string PointerType(int* i) {return "int";} 
   string PointerType(double* i) {return "double";} 
   string PointerType(bool* i) {return "bool";} 
   string PointerType(TString* i) {return "TString";} 

   string GetValueString(const char* name){
      string key(name);
      if (fMap.find(key)==fMap.end()) KeyNotFoundError(key.data());      
      string poitype = fMap[key].type;
      if (poitype=="int"){int i=GetProperty<int>(name); return string(TString::Format("%d",i).Data());}
      if (poitype=="double"){double i=GetProperty<double>(name); return string(TString::Format("%f",i).Data());}
      if (poitype=="TString"){TString i=GetProperty<TString>(name); return string(i.Data());}
      if (poitype=="bool"){bool i=GetProperty<bool>(name); return string(TString::Format("%d",i).Data());}
      return string("error");
   }
   
   // comfort functions to define strings via char* arguments
   bool SetProperty(const char* name, const char* value){ return SetProperty(name, TString(value)); }
   bool DefineProperty(const char* name, TString* var, const char* value){ return DefineProperty(name, var, TString(value)); }

};

#endif
