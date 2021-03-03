#include "Framework.h"
#include "SiliconClusterer.h"
#include "PrintTrackerHits.h"
#include "TrackerRawHitProcessor.h"
#include "CaloRawHitProcessor.h"
#include "TrackFinder.h"
#include "CaloClusterer.h"
#include "kt_algorithm.h"
#include "FastKtAlgorithm.h"
#include "ConeAlgorithm.h"
#include "JetFinder.h"

using namespace ExampleFramework;
JetFinder* SetProperty(Framework* theFramework);

int main()
{
   int runnumber=107;
   Framework* theFramework = new Framework("data/testdata.dat", "simpletest.dat", runnumber);
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
   JetFinder* JetAlg =  SetProperty(theFramework);
   theFramework->AddProcessor(JetAlg);

   //kt_algorithm* kt = new kt_algorithm(theFramework);
   //theFramework->AddProcessor(kt);
   //FastKtAlgorithm* fastKt = new FastKtAlgorithm(theFramework);
   //theFramework->AddProcessor(fastKt);
   //ConeAlgorithm* Cone = new ConeAlgorithm(theFramework);
   //theFramework->AddProcessor(Cone);

   
   

   theFramework->ProcessAll();

   delete theFramework;
}

JetFinder* SetProperty(Framework* theFramework){

   repeat:
   string Alg;
   cout << "Please chose a jet clustering Algorithm" << "\n";
   cout << "Enter Kt(for the kt-algorithm), FastKt(for the Fast-kt-algorithm) or Cone(for a simple Cone algorithm)" << endl;
   cin >> Alg;

   if(Alg == "FastKt"){
      cout << "Running FastKtAlgorithm" << endl;
      FastKtAlgorithm* fastKt = new FastKtAlgorithm(theFramework);
      JetFinder* JetAlg = fastKt;

      return JetAlg;
   }
   else if(Alg == "Kt"){
      cout << "Running kt_algorithm" << endl;
      kt_algorithm* fastKt = new kt_algorithm(theFramework);
      JetFinder* JetAlg = fastKt;

      return JetAlg;
   }
   else if(Alg == "Cone"){
      again:
      string userParameter;
      cout << "Running ConeAlgorithm" << "\n";
      cout << "Do you wish to set the user parameter or run the default? (y/n)" << endl;
      cin >> userParameter;

      if(userParameter == "n"){
         ConeAlgorithm* fastKt = new ConeAlgorithm(theFramework);
         JetFinder* JetAlg = fastKt;
         return JetAlg;
      }
      else if(userParameter == "y"){
         double R, pTcut, f, N;
         cout << "Please enter the parameter in the following order:" << "\n";
         cout << "N(amount of iterations for the proto jets), R (Cone radius)," << "\n";
         cout << "pTcut (only proto jets with pT higher then pTcut will be final jets)," << "\n";
         cout << "f (Fraction of pT that can be shared between to jets)" << endl;
         cin >> N >> R >> pTcut >> f;
         N = N + 0.5 - (N<0);
         int Num = (int) N;
         ConeAlgorithm* fastKt = new ConeAlgorithm(theFramework, Num, R, pTcut, f);
         JetFinder* JetAlg = fastKt;
         return JetAlg;
      }
      else{
         goto again;
      }
   }
   else{
      cout << "Algorithm not found please chose a supported algorithm" << endl;
      goto repeat;
   }


}
