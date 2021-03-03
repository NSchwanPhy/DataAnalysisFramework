#ifndef UTJetFinder_H
#define UTJetFinder_H

#include "UnitTest.h"
#include "JetFinder.h"

#include "TString.h"

#include "TFile.h"
#include "TMath.h"

#include "Framework.h"
#include "Processor.h"
#include "DICalorimeterHit.h"
#include "DICaloCluster.h"
#include "DICaloJet.h"
#include "FastKtAlgorithm.h"
// JetFinder unit tests

namespace ExampleFramework {

class utJetFinder : public UnitTesting::UnitTest
{
private:
    TString fOutFileName;
    TString fROOTName;
    TFile* fROOTFile;

public:
    utJetFinder()
       : UnitTest("JetFinder"), fROOTName("out.root"), fOutFileName("out.dat"), fROOTFile(0) 
    { }

    
    DICaloJet* initJet(vector<double> &x, vector<double> &y, vector<double> &z, vector<double> &Energy){

      vector <TString> inlist; // empty input
      inlist.push_back("../data/testdata.dat"); 
      Framework* dummyFramework = new Framework(inlist, fOutFileName, 100);
      DICaloCluster* cluster = new DICaloCluster();
      DICaloJet* jet = new DICaloJet();

      //first creating two clusterCol including two hits each
      int id=10;
      DataCollection* hitCol = new DataCollection();             //Collection of hit objects
      DataCollection* clusterCol = new DataCollection();    //Collection of cluster objects
      DataCollection* jetCol = new DataCollection();   //Collection of jet objects
      int module=112; // fdummy value
      const int layer=1;
      DICalorimeterHit* thit = new DICalorimeterHit(x[0], y[0], z[0], Energy[0], module, layer, id++);
      hitCol->push_back(thit);
      cluster->Add(thit);
      thit = new DICalorimeterHit(x[1], y[1], z[1], Energy[1], module+1, layer+1, id++);
      hitCol->push_back(thit);
      cluster->Add(thit);
      jet->Add(cluster);
      clusterCol->push_back(cluster);

      cluster = new DICaloCluster();
      hitCol = new DataCollection();
      module = 23;
      thit = new DICalorimeterHit(x[2], y[2], z[2], Energy[2], module, layer, id++);
      hitCol->push_back(thit);
      cluster->Add(thit);
      thit = new DICalorimeterHit(x[3], y[3], z[3], Energy[0], module+1, layer+1, id++);
      hitCol->push_back(thit);
      cluster->Add(thit);
      jet->Add(cluster);
      clusterCol->push_back(cluster);
      jetCol->push_back(jet);

      delete dummyFramework;

      return jet;
    }

    void run(){

        vector <TString> inlist; // empty input
        inlist.push_back("../data/testdata.dat"); 
        Framework* theFramework = new Framework(inlist, fOutFileName, 100);

        vector<double> x = {0.1, 0.1001, 0.3, 0.301};
        vector<double> y = {0.2, 0.21, 0.15, 0.14};
        vector<double> z = {0.3, 0.3, 0.2, 0.21};
        vector<double> Energy = {1.5, 2.5, 5.5, 4.5};
      
        DICaloJet* Jet1 = initJet(x,y,z,Energy);

        x = {0.3, 0.305, 0.5, 0.52};
        y = {0.1, 0.11, 0.35, 0.34};
        z = {0.2, 0.2, 0.4, 0.39};
        Energy = {2.5, 3.5, 2.3, 2.9};

        DICaloJet* Jet2 = initJet(x,y,z,Energy);

        double newEnergy = Jet1->GetEnergy() + Jet2->GetEnergy();
        double newx      = Jet2->GetX() * (Jet2->GetEnergy() / newEnergy)  + Jet1->GetX() * (Jet1->GetEnergy() / newEnergy);
        double newy      = Jet2->GetY() * (Jet2->GetEnergy() / newEnergy)  + Jet1->GetY() * (Jet1->GetEnergy() / newEnergy);
        double newz      = Jet2->GetZ() * (Jet2->GetEnergy() / newEnergy)  + Jet1->GetZ() * (Jet1->GetEnergy() / newEnergy);
        int newsize =  Jet1->GetCaloClusters().size() + Jet2->GetCaloClusters().size();

        // copied from JetFinder since it can not be accessed here directly
        DICaloJet* MergedJet = Merge(Jet1, Jet2);

        test_(newsize - MergedJet->GetCaloClusters().size() == 0);
        test_(fabs(newEnergy - MergedJet->GetEnergy()) < 0.01);
        test_(fabs(newx - MergedJet->GetX()) < 0.01);
        test_(fabs(newy - MergedJet->GetY()) < 0.01);
        test_(fabs(newz - MergedJet->GetZ()) < 0.01);



        delete theFramework;
    }



DICaloJet* Merge(const DICaloJet* di1,const DICaloJet* di2){
   DICaloJet* res = new DICaloJet();           

   res->SetEnergy( di2->GetEnergy() + di1->GetEnergy() );
   double x = di2->GetX() * (di2->GetEnergy() / res->GetEnergy())  + di1->GetX() * (di1->GetEnergy() / res->GetEnergy());
   double y = di2->GetY() * (di2->GetEnergy() / res->GetEnergy())  + di1->GetY() * (di1->GetEnergy() / res->GetEnergy());
   double z = di2->GetZ() * (di2->GetEnergy() / res->GetEnergy())  + di1->GetZ() * (di1->GetEnergy() / res->GetEnergy());
   res->SetCmsX(x);
   res->SetCmsY(y);
   res->SetCmsZ(z);

   vector <const DataItem* > MergedCaloClusters = di1->GetCaloClusters();
   vector <const DataItem* > CaloClusters = di2->GetCaloClusters();
   MergedCaloClusters.insert(MergedCaloClusters.end(), CaloClusters.begin(), CaloClusters.end());
   res->SetCaloClusters(MergedCaloClusters);  
   return res;
}


};

}
#endif