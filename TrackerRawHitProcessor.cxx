
#include "TrackerRawHitProcessor.h"
#include "DIRawHit.h"
#include "DITrackerHit.h"

using namespace ExampleFramework;
   

// constructor 
TrackerRawHitProcessor::TrackerRawHitProcessor(Framework* fram) 
  : Processor("TrackerRawHitProcessor", fram)
{
   DefineProperty(TString("InputCollection"), 
                  &fInputCollectionName, TString("TrackerRawHitCollection"));
   DefineProperty(TString("OutputCollection"), 
                  &fOutputCollectionName, TString("TrackerHitCollection"));
}

bool TrackerRawHitProcessor::EventLoop(){
   const DataCollection* inputs = fStorage->RetrieveData(fInputCollectionName,"donotcare");
   // create output
   DataCollection* myOutCollection = new DataCollection();

   double x,y,z,energy;
   int layer, id;
   bool isCalo;
   for (DataCollection::const_iterator it = inputs->begin(); it != inputs->end(); it++){
      const DIRawHit* hit = dynamic_cast< const DIRawHit* >(*it);
      id = hit->GetCellID();
      energy = hit->GetPulseHeight();
      fModuleTranslator.GetPositionFromID(id, layer, x, y, z, isCalo);
      myOutCollection->push_back(new DITrackerHit(x,y,z,energy, 
                                                 id, layer,
                                                 CreateUniqueObjectID()));
      fHistoService->FastFillTH1D("TrackerHitPulseheight",energy, 100,0.,500.);
      fHistoService->FastFillTH1D("TrackerHitPulseheightZoom",energy, 500,0.,50.);
   }
   // store output collection 
   fStorage->StoreData(fOutputCollectionName,myOutCollection);

   return true;
}

bool TrackerRawHitProcessor::InitRun(){ 
   return true;
}

bool TrackerRawHitProcessor::FinalizeRun(){ 
   return true;
}

