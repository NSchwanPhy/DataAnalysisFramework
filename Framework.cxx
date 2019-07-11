#include "Framework.h"
#include "DataItem.h"
#include <vector>
#include "DIEventNumber.h"
#include "DIRunNumber.h"
#include "TStopwatch.h"

using namespace ExampleFramework;

//! default constructor
Framework::Framework(vector<TString>& inlist, TString outfile, int jobID) 
   : fStorage(0), fProcessors(), fAnalysisID(jobID), 
     fInputList(inlist), fCurrentInput(0), fUniqueIDCounter(0), 
     fSuppressWriteOut(false), fVerbose(true), fCurrentEventNumber(0)
{
   if (inlist.size()==0) {
      cout << "no input files defined" << endl; 
      exit(1);
   }
   fOutputname = outfile;
   fStorage = new CentralStorage(outfile, fAnalysisID);
   fHistogramService = new HistogramService();
}

//! default constructor with one inputfile
Framework::Framework(TString infile, TString outfile, int jobID) 
   : fStorage(0), fProcessors(), fAnalysisID(jobID), 
     fInputList(0), fCurrentInput(0), fUniqueIDCounter(0), 
     fSuppressWriteOut(false), fVerbose(true), fCurrentEventNumber(0)
{
   fInputList.push_back(infile);
   fOutputname = outfile;
   fStorage = new CentralStorage(outfile, fAnalysisID);
   fHistogramService = new HistogramService();
}

// destructor
Framework::~Framework(){
   for (int i=0;i<fProcessors.size();i++) delete fProcessors[i];
   fProcessors.clear();
   fInputList.clear();
   delete fHistogramService;
   delete fStorage;
}

bool Framework::AddProcessor(Processor* pro)
{
   pro->SetUp(fProcessors.size(),fStorage, fHistogramService); 
   fProcessors.push_back(pro);
   return true;
}

void Framework::GenerateEvents(int nevt)
{
   Init();
   TStopwatch tstop;

   DataCollection* RunNumberContainer = new DataCollection();
   RunNumberContainer->push_back(new DIRunNumber(fAnalysisID));
   fStorage->StoreData("RunNumber",RunNumberContainer);
   if (!fSuppressWriteOut) fStorage->WriteRunHeader();

   for (int i = 0; i <nevt; i++){
      if (fVerbose && i >0 && i%200==0) cout << "generating event="<<i <<endl;;
      ResetIDCounter();
      // create event number object
      DataCollection* EventNumberContainer = new DataCollection();
      EventNumberContainer->push_back(new DIEventNumber(i));
      fStorage->StoreData("EventNumber",EventNumberContainer);
      // process all
      ProcessOneEvent();
   }
   fHistogramService->FastFillTH1D("Framework_time",tstop.CpuTime(),100,0,0);
   Finalize();
}

void Framework::ProcessAll(){
   Init();
   int icurrent=0;
   TStopwatch tstop;
   if (fVerbose) cout <<"this is Framework::ProcessAll nfiles="<<fInputList.size()<<endl;
   // loop over input files
   for (int ifile = 0; ifile <fInputList.size(); ifile++){
      // open each file
      fCurrentInput = new AnalysisFile(fInputList[ifile], "read");
      if (!(fCurrentInput->IsValid())){
         cout << "ERROR, input not valid, file="<<fInputList[ifile]<<endl; 
         continue;
      }
      fStorage->SetInputFile(fCurrentInput);

      fStorage->Clear(false);
      fStorage->ReadRunHeader();
      if (!fSuppressWriteOut && ifile==0) fStorage->WriteRunHeader();

      while (ReadNextEvent()){
         if (fVerbose && icurrent >0 && icurrent%200==0) cout << "processing event="<<icurrent <<endl;;
         icurrent++;
         ProcessOneEvent(); // event loop
      }    
      delete fCurrentInput;
   } // end file loop
   fHistogramService->FastFillTH1D("Framework_time",tstop.CpuTime(),100,0,0);
   Finalize();
   if (fVerbose) cout << "Framework::ProcessAll is done"<<endl;
}

bool Framework::ReadNextEvent()
{
   ResetIDCounter(); 
   bool retval = fStorage->ReadNextEvent();
   ResetIDCounter(fStorage->GetMaxUniqueID()+1);
   return retval;
}

void Framework::Init()
{ 
   for (int i = 0; i <fProcessors.size(); i++)
      (fProcessors[i])->InitRun();
   if (fVerbose){
      cout <<"Framework:: Printing properties of all processors"<<endl;
      for (int i = 0; i <fProcessors.size(); i++){
         cout << fProcessors[i]->GetName()<< ", Title="<< fProcessors[i]->GetTitle()<<": "<<endl;
         (fProcessors[i])->PrintProperties();     
      } 
   }
}

void Framework::ProcessOneEvent()
{
   // loop over processor list and let them process it
   for (int i = 0; i <fProcessors.size(); i++){
      fHistogramService->SetCurrentDir((fProcessors[i])->GetTitle());
      (fProcessors[i])->EventLoop();
   }
   // tell storage to write event 
   if (!fSuppressWriteOut) fStorage->WriteEvent();
   // tell storage to clean up
   fStorage->Clear();
   fHistogramService->SetCurrentDir("");
}

void Framework::Finalize()
{ 
   for (int i = 0; i <fProcessors.size(); i++)
      (fProcessors[i])->FinalizeRun();

   // histo service
   TString outname = fOutputname;
   outname.ReplaceAll(".dat",".root");
   if (!outname.Contains(".root")) outname = outname + ".root";
   fHistogramService->WriteRootFile(outname); 

}

Service* Framework::GetService(TString& name){
   return NULL; //ToDo implement me!!!!!!!!!!
}
