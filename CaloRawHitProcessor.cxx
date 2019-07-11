
#include "CaloRawHitProcessor.h"
#include "DICalorimeterHit.h"
#include "DIRawHit.h"

using namespace ExampleFramework;
   

// constructor 
CaloRawHitProcessor::CaloRawHitProcessor(Framework* fram) 
  : Processor("CaloRawHitProcessor", fram)
{
   DefineProperty("InputCollection",
                  &fInputCollectionName, TString("CalorimeterRawHitCollection"));
   DefineProperty("OutputCollection",
                  &fOutputCollectionName, TString("CalorimeterHitCollection"));
}

bool CaloRawHitProcessor::EventLoop(){
   const DataCollection* inputs = fStorage->RetrieveData(fInputCollectionName,"donotcare");
   // create output
   DataCollection* myOutCollection = new DataCollection();

   // loop over inputs  iterator version
   double x,y,z,energy;
   int layer, id;
   bool isCalo;
   for (DataCollection::const_iterator it = inputs->begin(); it != inputs->end(); it++){
     const DIRawHit* hit = dynamic_cast< const DIRawHit* >(*it);
     id = hit->GetCellID();
     energy = hit->GetPulseHeight();
     fModuleTranslator.GetPositionFromID(id, layer, x, y, z, isCalo);
     myOutCollection->push_back(new DICalorimeterHit(
                                x,y,z,energy, 
                                id, 
                                layer - fModuleTranslator.GetNTrackerLayers(),
                                CreateUniqueObjectID()));
     fHistoService->FastFillTH1D("CaloPulseheight",energy,200,0.,10000.);
     fHistoService->FastFillTH1D("CaloPulseheightZoom",energy,100,0.,2000.);
   }

   // store output collection 
   fStorage->StoreData(fOutputCollectionName,myOutCollection);
   return true;
}

bool CaloRawHitProcessor::InitRun(){ 
   return true;
}

bool CaloRawHitProcessor::FinalizeRun(){ 
   return true;
}

