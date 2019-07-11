#include "Framework.h"
#include "SiliconClusterer.h"
#include "PrintTrackerHits.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
#include "TrackFinder.h"
#include "CaloClusterer.h"

using namespace ExampleFramework;

int main()
{
   int runnumber=107;
   Framework* theFramework = new Framework("data/electron20GeV.dat", "simpletest.dat", runnumber);
   theFramework->AddProcessor(new TrackerRawHitProcessor(theFramework));
   theFramework->AddProcessor(new CaloRawHitProcessor(theFramework));
   SiliconClusterer* clus = new SiliconClusterer(theFramework);
   theFramework->AddProcessor(clus);
   
   TrackFinder* finder = new TrackFinder(theFramework);
   finder->SetProperty("CurvatureCorrection", 1.01);
   finder->SetProperty("InputCollection", "ClusteredTrackerHitCollection");
   theFramework->AddProcessor(finder);      
   PrintTrackerHits* printer = new PrintTrackerHits(theFramework);
   theFramework->AddProcessor(printer);      
   CaloClusterer* caloclus = new CaloClusterer(theFramework);
   theFramework->AddProcessor(caloclus);

   theFramework->ProcessAll();

   delete theFramework;
}
