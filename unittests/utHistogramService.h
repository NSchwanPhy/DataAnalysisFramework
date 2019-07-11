#ifndef UTHISTOGRAMSERVICE_H_
#define UTHISTOGRAMSERVICE_H_

#include "UnitTest.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "HistogramService.h"
/*
 *
 */

namespace ExampleFramework{

class utHistogramService : public UnitTesting::UnitTest
{
private:
   HistogramService* fH;

public:
   utHistogramService() : UnitTest("HistogramService"), fH(new HistogramService()) {};
   virtual ~utHistogramService() {};

   void run() {
      createFile();
      checkFile();
   }

   void createFile() {  
      int var1;
      float var2;
      TTree* tree0=new TTree("tree","histogramService test");
      fH->BookRootObject(tree0);
      fH->FastFillTH1D("test2",0.5,100,0.,1.);
      fH->FastFillTH1D("test1",0.1,100,0.,1.);
      fH->FastFillTH1D("test2",0.3,100,0.,1.);
      fH->SetCurrentDir("sub1");
      fH->FastFillTH1D("test1",0.7,100,0.,1.);
      fH->FastFillTH1D("test3",0.4,100,0.,1.);
      fH->SetCurrentDir("sub2");
      fH->FastFillTH1D("test4",0.2); // autobinning feature
      fH->FastFillTH2D("test2D",0.2,0.4,10,0.,12.,10,0.,1.); // 2D histo
      TTree* tree=new TTree("tree","histogramService test2");
      fH->BookRootObject(tree);
      tree->Branch("var1",&var1,"var1/I");
      tree->Branch("var2",&var2,"var1/F");
      var1=2;var2=0.5;
      tree->Fill();
      var1=3;var2=0.7;
      tree->Fill();
      fH->WriteRootFile("utHistService.root");
   }
   void checkFile() {
      TFile file("utHistService.root", "read");
      test_(!file.IsZombie());
      test_(file.IsOpen());
      TH1D *h1, *h2, *hs1, *hs3, *hs4;
      file.GetObject("test1",h1);
      file.GetObject("test2",h2);
      test_(h1);
      test_(h2);
      test_(h1->GetEntries()==1);
      test_(h1->GetXaxis()->GetXmin()==0.);
      test_(h1->GetXaxis()->GetXmax()==1.);
      test_(h1->GetXaxis()->GetNbins()==100);
      test_(h2->GetEntries()==2);
      delete h1;
      delete h2;
      file.GetObject("sub1/test1",hs1);
      file.GetObject("sub1/test3",hs3);
      file.GetObject("sub2/test4",hs4);
      test_(hs1);
      test_(hs3);
      test_(hs4);
      test_(hs1->GetEntries()==1);
      test_(hs3->GetEntries()==1);
      test_(hs4->GetEntries()==1);
      TH2D* h2D;
      file.GetObject("sub2/test2D",h2D);
      test_(h2D);
      test_(h2D->GetEntries()==1);
      TTree* tree;
      file.GetObject("tree",tree);
      test_(tree);
      test_(tree->GetEntries()==0);
      int v1;
      float v2;
      file.GetObject("sub2/tree",tree);
      test_(tree);
      test_(tree->GetEntries()==2);
      tree->SetBranchAddress("var1",&v1);
      tree->GetEntry(0);
      test_(v1==2);
      tree->GetEntry(1);
      test_(v1==3);
   }

};

}

#endif /* UTHISTOGRAMSERVICE_H_ */
