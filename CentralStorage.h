
#ifndef CENTRALSTORAGE_H
#define CENTRALSTORAGE_H

#include "DataCollection.h"
#include "DataItem.h"
#include "Service.h"
#include "AnalysisFile.h"
#include "DataItemFactory.h"
#include <map>
#include <utility>

namespace ExampleFramework {

/*!
  Transient Data Storage. Stores DataCollections. Provides also partial support of persistent data items.
 (c) E. von Toerne, U. of Bonn 2009
 ToDo turn into a singleton object
 */
class CentralStorage : public Service {
   
   friend class Framework;
   friend class utCentralStorage;
   friend class utDITrack;
   friend class utDIMuon;
   friend class utDIElectron;
   friend class utDIClusteredTrackerHit;
   friend class utDICaloCluster;
private:
   vector < DataCollection* >   fStore; //!< Storage for event data
   //map    < int, DataItem* >    fIdRegister; //!< bool = true for persistent items
   map    < int, pair<DataItem*, bool> >    fIdRegister; //!< bool = true for persistent items
   
   vector < TString >           fStoreKeys;
   vector < TString >           fStoreTypeNames;
   TString                      fOutfileName;
   AnalysisFile                 fOutfile; 
   mutable istream*             fInstream;  //!< the mutable is important to maintain the constness of io operations  
   mutable ostream*             fOutstream; //!< the mutable is important to maintain the constness of write operations 
   int                          fRunNo;
   int                          fMaxUniqueID; //!< maximum unique ID found in event
   DataItemFactory              fCreator;   
   bool                         fDebug;
   ostream& out() const { return *fOutstream;}
   istream& in() const { return *fInstream;}
   void Clear(bool keepPersistent=true); 
   bool SetInputFile(AnalysisFile* infile);
   bool ReadNextEvent(); 
   bool ReadCollectionList(bool isRunHeader=false);
   bool ReadRunHeader();
   void WriteCollectionList(bool isRunHeader=false);
   void WriteEvent(); 
   bool WriteRunHeader();
   const DataItem* GetPointer(int id);
   int GetMaxUniqueID() const { return fMaxUniqueID;} 
   ~CentralStorage(){ Clear(false); }    // private destructor
   //! private constructor, used only by friend classes
   CentralStorage(TString outfile, int ID);
public:
   void StoreData(TString name, DataCollection* poi);
   const DataCollection* RetrieveData(TString name, TString typ) const;
   void PrintStorage() const; 
   void RegisterID(int id, DataItem* poi);
   void DeregisterID(int id);
   void SetDebug(bool deb){ fDebug = deb;}
   void Write(const double& d) const { out()<<setprecision(12)<<d<< " ";}
   void Write(const int& i) const { out() << i << " ";} 
   void Write(const DataItem* poi) const;
   void Write(const vector<const DataItem*>& vec) const;
   void Read(double& d) { in() >> d;}
   void Read(int& i) { in() >> i;}
   void Read(const DataItem *& poi);
   void Read(vector<const DataItem *>& vec);

   MyDef(CentralStorage)
};

} 
#endif
