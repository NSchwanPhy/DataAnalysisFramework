
#include "ExampleProcessor.h"

using namespace ExampleFramework;
   

// constructor 
ExampleProcessor::ExampleProcessor(Framework* fram) 
  : Processor("ExampleProcessor", fram)
{
   DefineProperty("InputCollection", &fInputCollectionName, "TrackerHitCollection");
   DefineProperty("OutputCollection", &fOutputCollectionName, "TrackCollection");
}

bool ExampleProcessor::EventLoop(){
   const DataCollection* inputs = fStorage->RetrieveData(fInputCollectionName,"donotcare");
   // create output
   DataCollection* myOutCollection = new DataCollection();

   // loop over inputs  first version 
   //int nhits = inputs->size();
   // use HistogramService 
   // fHisto->FastFillTH1D("Ninputs", nhits, 100, -0.5, 99.5);

   //for (int ihit=0;ihit<nhits;ihit++){ 
   //  const DICalorimeterHit* hit = dynamic_cast< const DICalorimeterHit* >((*inputs)[ihit]);
   //}

   // loop over inputs  interator version
   // for (DataCollection::const_iterator it = inputs->begin(); it != inputs->end(); it++){
   //   const DICalorimeterHit* hit = dynamic_cast< const DICalorimeterHit* >(*it);
   //}
   // store output collection 
   // fStorage->StoreData(fOutputCollectionName,myOutCollection);
   return true;
}

bool ExampleProcessor::InitRun(){ 
   return true;
}

bool ExampleProcessor::FinalizeRun(){ 
   return true;
}

