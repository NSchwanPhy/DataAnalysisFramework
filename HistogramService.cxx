// 
// created October 2006, Eckhard von Toerne

#include <string>
#include <iostream>
#include "TVector.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"
#include "TList.h"
#include "TDirectory.h"
#include "HistogramService.h"
using std::cout;using std::endl;
using namespace ExampleFramework;

HistogramService::HistogramService() : fCurrentDir("") { }

HistogramService::~HistogramService()
{ 
   map<string, TObject* >::iterator it;
   for (it = fMegaList.begin();it != fMegaList.end(); it++){ 
      delete it->second;
   }
   fMegaList.clear(); 
}

string HistogramService::CreateKey(TString tkey){
   return string(fCurrentDir + "/" + tkey.Data());
}

void HistogramService::FastFillTH1D(TString tkey, double x, int nbin, double xmin, double xmax)
{
   string key = CreateKey(tkey);
   if (fMegaList.find(key)==fMegaList.end()){  // book histogram
      fMegaList[key] = new TH1D(tkey, tkey, nbin, xmin, xmax);
   }
   ((TH1D*) fMegaList[key])->Fill(x);
}

void HistogramService::FastFillTH2D(TString tkey, double x, double y, int nbinx, double xmin, double xmax, int nbiny, double ymin, double ymax )
{
   string key = CreateKey(tkey);
   if (fMegaList[key]==0){  // book histogram
      fMegaList[key] = new TH2D(tkey, tkey, 
                                nbinx, xmin, xmax, 
                                nbiny, ymin, ymax);
   }
   ((TH2D*) fMegaList[key])->Fill(x,y);
}

string HistogramService::GetDirOf(string key){
   TString s=key;
   if (!s.Contains("/")) return string("");
   if (s.Last('/')==0) return string("");
   s.Remove(s.Last('/'),s.Length()-1);
   return string(s.Data());
}
 
void HistogramService::WriteRootFile(TString fileName)
{  
   std::map< string , TDirectory* > mapOfCreatedDirs;    
   TFile *f = new TFile(fileName,"RECREATE");  
   f->cd();
   // using a map::iterator
   map<string, TObject* >::iterator it;
   map<string, TDirectory* >::iterator dirit;
   for (it = fMegaList.begin();it != fMegaList.end(); it++){ 
      string key = it->first;
      string dirName = GetDirOf(key); 
      dirit = mapOfCreatedDirs.find(dirName);

      if (dirName=="") f->cd();
      else if ( dirit == mapOfCreatedDirs.end()){
         //  create dir
         TDirectory *lstDir = f->mkdir(dirName.data(), dirName.data());
         lstDir->cd();
         mapOfCreatedDirs[dirName] = lstDir;
      }
      else dirit->second->cd(); // cd to dir 
      //cout << "writing histo "<<it->first<< " to dir "<<dirName<<endl;
      it->second->Write();
   }
   //cout << "wrote histo file=" << fileName << endl;
   f->Close();
   mapOfCreatedDirs.clear();
}

bool HistogramService::BookRootObject(TObject* obj){
   string key = CreateKey(obj->GetName());
   if (fMegaList.find(key)==fMegaList.end()){
      fMegaList[key]=obj;
      return true;
   } 
   return false;
}
