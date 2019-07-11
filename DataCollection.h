#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

#include <vector>
using namespace std;
#include <cstdio>
#include <iostream>

namespace ExampleFramework {
      
class DataItem;

/*!
  Container for storage of DataItems. Uitlizes polymorphism to allow generic handling of data items. This used to be just a vector of DataItem*. Now it inerits from this vector
 (c) E. von Toerne, U. of Bonn 2011
 */
class DataCollection : public  vector< const DataItem* > 
{
private:
   bool fIsPersistent; //!< =false for event data, =true for data that remain in storage for more than one event
public:
   void SetPersistent(bool isPersistent){ fIsPersistent = isPersistent;}
   bool GetPersistent(){ return fIsPersistent;}
   DataCollection();
};

   // useful function for access to elements using a template function
   template<typename T> const T* Get(const DataCollection* col, int index){
      const T* poi = dynamic_cast< const T* >((*col)[index]);
      if (!poi) std::cout <<"error converting DataCollection entry to derived class pointer, check data type!" <<std::endl;
      return poi;      
   }
   // old definition
   // typedef vector< const DataItem* > DataCollection;

} // end namespace

#endif
