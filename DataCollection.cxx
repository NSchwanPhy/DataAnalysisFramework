
#include "DataCollection.h"
#include "DataItem.h"

using namespace ExampleFramework;

DataCollection::DataCollection() :  vector< const DataItem* >(0), fIsPersistent(false) { } 
