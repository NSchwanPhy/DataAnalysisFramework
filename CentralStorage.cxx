#include "DataItem.h"
#include "CentralStorage.h"
#include <algorithm>

using namespace ExampleFramework; 
MyImp(CentralStorage);

CentralStorage::CentralStorage(TString outfile, int ID) 
      : Service(), 
        fOutfileName(outfile), fOutfile(outfile,"write"), 
        fOutstream(&(fOutfile.GetOutStream())),        
        fRunNo(0), fMaxUniqueID(-1000), fDebug(false) 
{ 
   ExampleFramework::DataItem::theStorage = this;
}

void CentralStorage::StoreData(TString name, DataCollection* poi)
{
   // Check for Consistency
   bool found = (find(fStoreKeys.begin(),fStoreKeys.end(),name) != fStoreKeys.end());
   if (found) ReportError(TString("key already used ")+name); // check if key already exists
   if (name.Contains(" ")) ReportError(TString("key has a space character=")+name);    // check for blanks in key

   int ndat = poi->size();
   if (fDebug && ndat>0) cout << "this is centralstorage::StoreData name="<<name<<" poi="<<poi<<" with ndata="<<ndat<< "pointer to first obj"<< (*poi)[0] <<endl; 
   for (int i=0; i<ndat;i++) 
      if (GetPointer((*poi)[i]->GetUniqueID()) != 0)
         ReportError(TString("dataitem was already entered, name=")+name+TString::Format(" i=%d",i));
   fStore.push_back(poi);
   fStoreKeys.push_back(name);
   if (poi->size()>0) fStoreTypeNames.push_back((*poi)[0]->GetName());
   else fStoreTypeNames.push_back("empty");
}

const DataCollection* CentralStorage::RetrieveData(TString name, TString typ) const {
   for (int i=0;i<fStore.size();i++){
      if (name==fStoreKeys[i]){
         if (typ == "donotcare" || typ == fStoreTypeNames[i] || fStoreTypeNames[i]=="empty") return fStore[i];
         else{
            cout << "ERROR in CentralStorage, trying to access data with correct key and wrong type, requested=" << name <<" of type "<<typ<<" actual type is="<<fStoreTypeNames[i];
            return NULL;
         }
      }
   }
   cout << "ERROR in CentralStorage, did not find key="<<name<<" Type="<<typ<<endl;
   PrintStorage();
   return NULL;
}

void CentralStorage::Clear(bool keepPersistent){
   if (fDebug) cout << "CentralStorage::Clear()"<<endl;
   vector < DataCollection* >::iterator ist;
   int iii=0;
   ist = fStore.begin();
   while (ist != fStore.end()){
      DataCollection* col= *ist;
      if (keepPersistent && col->GetPersistent()){
         for (int j=0;j<col->size();j++){
            fIdRegister[(*col)[j]->GetUniqueID()].second=true;
         }
         ist++;
         iii++;
      }
      else{
         for (int j=0;j<col->size();j++){
            fIdRegister.erase((*col)[j]->GetUniqueID());
            delete (*col)[j];
         }
         col->clear();  
         delete col;
         fStoreKeys.erase(fStoreKeys.begin()+iii);
         fStoreTypeNames.erase(fStoreTypeNames.begin()+iii);
         ist=fStore.erase(ist);
      }
   }
   // now clear the remaining items in fIdRegister (Garbage Collection)
   int maxid = -999999;
   map<int, pair <DataItem*, bool> >::iterator it;
   vector < int > itemsToDelete;
   if (fDebug){
      cout << "fIdRegister contains "<<fIdRegister.size()<<" elements"<<endl;
      for (it = fIdRegister.begin();it != fIdRegister.end(); it++ )
         cout << "id="<<it->first <<" ";
      cout << endl;
   }
   for  (it =fIdRegister.begin(); it != fIdRegister.end(); it++){
      if (!(it->second.second)){
         cout << "Garbage collector, deleting id="<<it->first << endl;      
         itemsToDelete.push_back(it->first);
      } 
      else{
         if (fDebug) cout << "stepping over id="<<it->first << endl; 
         if (it->first > maxid) maxid = it->first;
      }
   }
   // cleanup fIdRegister
   for (int i=0;i<itemsToDelete.size();i++){
         delete (fIdRegister[itemsToDelete[i]].first);
         fIdRegister.erase(i); 
   }
   //
   if (keepPersistent==false){
      fStore.clear();
      fStoreKeys.clear();
      fStoreTypeNames.clear();
      fMaxUniqueID=-1000;
   }
   else {
      int i=0;
      while (i<fStore.size()){
         if (fStore[i]->GetPersistent()){
            if (fDebug) cout << "keeping collection "<<fStoreKeys[i]<<endl;
            i++;
         }
         else {
            fStore.erase(fStore.begin()+i);
            fStoreKeys.erase(fStoreKeys.begin()+i);
            fStoreTypeNames.erase(fStoreTypeNames.begin()+i);
         }
      }
      fMaxUniqueID=maxid;
   }
   if (fDebug){
      cout << "fMaxUniqueID="<<fMaxUniqueID<<" fIdRegister.size="<<fIdRegister.size()<<endl;
      PrintStorage();
   }
}

bool CentralStorage::SetInputFile(AnalysisFile* infile){
   fInstream = &(infile->GetInStream());
   return true;
}

const DataItem* CentralStorage::GetPointer(int id) {
   if (fDebug && id<20) cout << "CentralStorage::GetPointer id=" <<id<<endl;
   for (int istore=0;istore<fStore.size();istore++){
      DataCollection* col = fStore[istore];
      for (int idat=0;idat<col->size();idat++){
         int id2 = ((*col)[idat])->GetUniqueID(); 
         if (id==id2) return ((*col)[idat]);
      }
   }
   if (fDebug && id<20) cout << "CentralStorage::GetPointer error could not find pointer for ID="<<id<<endl;
   return 0;
}

bool CentralStorage::ReadRunHeader(){
   if (fDebug) cout << "CentralStorage::ReadRunHeader"<<endl;
   bool readcol = ReadCollectionList(true);
   return readcol;
}

bool CentralStorage::ReadNextEvent(){
   if (fDebug) cout << "CentralStorage::ReadNextEvent"<<endl;
   bool readcol = ReadCollectionList();
   return readcol;
}

bool CentralStorage::ReadCollectionList(bool isRunHeader)
{
   TString lim(" "), header;
   istream& ifs =in();
   int storesize;
   ifs >> header; // read header
   if (fDebug) cout << "Header="<<header <<endl;
   if (!ifs) return false;
   ifs >> storesize;
   if (fDebug) cout << "storesize=" << storesize << endl;
   // loop over all collections
   bool isPersistent;
   for (int istore=0;istore<storesize;istore++){
      DataCollection* col = new DataCollection();
      fStore.push_back(col);
      TString storekey,typname;
      ifs >> header >> storekey >> typname >> isPersistent;
      col->SetPersistent(isPersistent); 
      if (isRunHeader && !isPersistent) ReportError("non-persistent entry");
      if (fDebug) cout << "Collection header read " << header << lim 
           << storekey <<lim<< typname<<endl;
      fStoreKeys.push_back(storekey);
      fStoreTypeNames.push_back(typname);
      int colsize;
      ifs >> colsize >> header;
      // loop over all items in collection
      for (int idat=0;idat<colsize;idat++){
         // read each item
         DataItem* dat = fCreator.Create(typname);
         dat->PRead(); // prereading of base class properties
         dat->Read();
         if (fDebug) cout << " Centralstorage reading object with unique ID="<< dat->GetUniqueID() <<endl; 
         fMaxUniqueID = TMath::Max(dat->GetUniqueID(), fMaxUniqueID);
         col->push_back(dat); // now our data is securely stored in a container
      }
      ifs >> header; //Enddata
      if (fDebug) cout << "read " << colsize << "elements, header currently is "<< header<<endl;
      ifs >> header; //end collection
      if (fDebug) cout << "header currently is "<< header<<endl;
   }
   // read collection footer
   ifs >> header;
   if (fDebug) cout << "read at end of collection loop, header currently is "<< header<<endl;
   if (fDebug) PrintStorage();
   return true;
}

void CentralStorage::WriteCollectionList(bool isRunHeader)
{
   TString lim(" ");
   ostream& ofs = out();

   int nitems=0;
   for (int istore=0;istore<fStore.size();istore++){
      DataCollection* col = fStore[istore];
      if (isRunHeader) col->SetPersistent(true); 
      else if (col->GetPersistent()) continue;
      nitems++;
   }
   ofs << nitems<<endl;

   // loop over all collections
   for (int istore=0;istore<fStore.size();istore++){
      DataCollection* col = fStore[istore];
      if (!isRunHeader && col->GetPersistent()) continue; // Do not write persisitent items into event collections 
      // write collection header
      ofs << "--BEGINCOLLECTION"<<lim
          << fStoreKeys[istore] << lim 
          << fStoreTypeNames[istore] << lim
          << col->GetPersistent() <<endl;
      ofs << col->size() << endl;
      ofs << "--BEGINDATA"<<endl;
      // loop over all items in collection and write each item
      for (int idat=0;idat<col->size();idat++){
         ((*col)[idat])->PWrite(); // prewriting of base class properties 
         ((*col)[idat])->Write(); 
         ofs << endl;
      } // end loop collection
      ofs << "--ENDDATA"<<endl;
      ofs << "--ENDCOLLECTION"<<endl; // write collection footer
   } // end loop storage
}

bool CentralStorage::WriteRunHeader()
{
   TString lim(" ");
   if (fDebug) cout << "CentralStorage::WriteRunHeader"<<endl;
   if (fDebug) PrintStorage();
   // get ostream
   ostream& ofs = out();
   // write event header
   ofs << "--BEGINRUNHEADER"<<endl; 
   WriteCollectionList(true);
   ofs << "--ENDRUNHEADER"<<endl;      // write event footer
}

void CentralStorage::WriteEvent(){
   TString lim(" ");
   if (fDebug) cout << "CentralStorage::WriteEvent"<<endl;
   if (fDebug) PrintStorage();
   // get ostream
   ostream& ofs = out();
   // write event header
   ofs << "--BEGINEVENT"<<endl; 
   WriteCollectionList();
   ofs << "--ENDEVENT"<<endl;      // write event footer
}

void CentralStorage::PrintStorage() const
{
   cout << "List of stored items"<<endl;
   for (int i=0;i<fStore.size();i++){
      std::cout << "Name="<<fStoreKeys[i] << " Type=" << fStoreTypeNames[i]<< " size="<<fStore[i]->size()<< ( fStore[i]->GetPersistent() ? "persistent" : "")<<endl;
   }
}

void CentralStorage::Write(const vector<const DataItem*>& vec) const 
{
   out() << endl;
   out() << vec.size() << " ";
   for (int i =0; i<vec.size(); i++) Write(vec[i]);
} 
void CentralStorage::Write(const DataItem* poi) const 
{
   if (poi) out() << (poi->GetUniqueID()) << " "; 
   else out() << 0 << " ";
}

void CentralStorage::Read(const DataItem *& poi) 
{
   int iD;
   in() >> iD;
   if (fDebug) std::cout << "Read pointer id="<<iD<<endl;
   if (iD == 0) poi=0;
   else{
      poi = dynamic_cast < const DataItem * > ( GetPointer(iD) ); // see Deitel&Deitel, section 25.2
      if (poi==0) cout <<"Read dataitem zero pointer"<<endl;   
   }
}

void CentralStorage::Read(vector<const DataItem *>& vec) 
{
   int n;
   const DataItem * poi;
   in() >> n;
   for (int i =0; i<n; i++){ 
      Read(poi);
      vec.push_back(poi);
   }
}

void CentralStorage::DeregisterID(int id){   
   if (fDebug && id<10) cout << "Deregistering id= "<<id<<endl;
   fIdRegister.erase(id);
}

void CentralStorage::RegisterID(int id, DataItem* poi)
{
   if (fIdRegister[id].first==0) fIdRegister[id]=pair< DataItem* , bool >(poi,false);
   else {
      cout << "error found an id again "<<id<<" poi="<<poi<<endl;
      PrintStorage();
      exit(1);
   }
}
