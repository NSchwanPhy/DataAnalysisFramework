// Example Analysis Framework unit tests

#include <iostream>
#include "UnitTestSuite.h"


#include "utCentralStorage.h"
#include "utHistogramService.h"
#include "utPropertyHandler.h"
#include "utTestRun.h"
#include "utTrackFinder.h" 
#include "utSiliconClusterer.h" 
#include "utDIClusteredTrackerHit.h"
#include "utDITrack.h"
//#include "utDIElectron.h"
#include "utDICaloCluster.h"
#include "utCaloClusterer.h"
#include "utDICaloJet.h"
#include "utJetFinder.h"
#include "utkt_algorithm.h"
#include "utFastKtAlgorithm.h"
#include "utConeAlgorithm.h"
//#include "utElectronFinder.h"

using namespace UnitTesting;
using namespace std;

int main()
{
   UnitTestSuite test_DataCreator("DataCreator unit testing");
   
   test_DataCreator.intro();
   
   // test_DataCreator.addTest(new utDIElectron);
   // test_DataCreator.addTest(new utElectronFinder);

   test_DataCreator.addTest(new utCentralStorage);
   test_DataCreator.addTest(new utHistogramService);
   test_DataCreator.addTest(new utPropertyHandler);
   test_DataCreator.addTest(new utTestRun);  
   test_DataCreator.addTest(new utTrackFinder);
   test_DataCreator.addTest(new utSiliconClusterer);
   test_DataCreator.addTest(new utDITrack);
   test_DataCreator.addTest(new utDIClusteredTrackerHit);
   test_DataCreator.addTest(new utCaloClusterer);
   test_DataCreator.addTest(new utDICaloCluster);
   test_DataCreator.addTest(new utDICaloJet);
   test_DataCreator.addTest(new utJetFinder);
   test_DataCreator.addTest(new utkt_algorithm);
   test_DataCreator.addTest(new utFastKtAlgorithm);
   test_DataCreator.addTest(new utConeAlgorithm);
   test_DataCreator.run();
   long int nFail = test_DataCreator.report();
   cout << "Total number of failures: " << nFail << endl;
   cout << "*********************************************" << endl;
}
