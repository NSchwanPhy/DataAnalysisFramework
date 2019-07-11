// created November 2006, Eckhard von Toerne

#ifndef EVT_HISTOGRAMSERVICE_H
#define EVT_HISTOGRAMSERVICE_H

#include <map>
#include <string>
using std::map;
using std::string;

class TObject;
class TH1D;
class TH2D;
class TDirectory;
class TFile;

namespace ExampleFramework {

class HistogramService
{
   friend class Framework;
   friend class utHistogramService;
private:
   string fCurrentDir;
   map< string, TObject*> fMegaList;
   HistogramService();  //! private constructor to be invoked by friend classes
   ~HistogramService(); // destructor

public:

   void FastFillTH1D(TString key, double x, int nbin=100, double xmin=0., double xmax=0.);
   
   void FastFillTH2D(TString key, double x, double y, int nbinx, double xmin, double xmax, int nbiny, double ymin, double ymax);
   string CreateKey(TString tkey);   
   string GetDirOf(string key);
   void WriteRootFile(TString fileName);

   void SetCurrentDir(TString dirname){ fCurrentDir=dirname; }
   string GetCurrentDir(){ return fCurrentDir; }
   bool BookRootObject(TObject* hist);

};

}
#endif
