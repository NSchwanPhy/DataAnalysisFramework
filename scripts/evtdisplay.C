
// How to run the event display macro
// 
//  root -l outputfile.root
//  .L scripts/evtdisplay.C
//  evtdisplay(0) 
//
// Author: Eckhard v. Toerne
//
void evtdisplay(int i){
   TH2D *h1, *h2, *r1, *r2; 
   TDirectory* myDir = (TDirectory*) (_file0->Get("PrintTrackerHits"));
   
   if (i>=0){
      h1 = (TH2D*) (myDir->Get(Form("Calo2D%d",i)));
      h2 = (TH2D*) (myDir->Get(Form("Tracker2D%d",i)));
      r1 = (TH2D*) (myDir->Get(Form("Calo2D_yz%d",i)));
      r2 = (TH2D*) (myDir->Get(Form("Tracker2D_yz%d",i)));
   }
   else{
      h1 = (TH2D*) (myDir->Get("Calo2D"));
      h2 = (TH2D*) (myDir->Get("Tracker2D"));
      r1 = (TH2D*) (myDir->Get("Calo2D_yz"));
      r2 = (TH2D*) (myDir->Get("Tracker2D_yz"));
   }
   std::cout << "h1="<<h1<<std::endl;
   h1->SetMarkerSize(0.2);
   h2->SetMarkerSize(0.2);
   r1->SetMarkerSize(0.2);
   r2->SetMarkerSize(0.2);
   h1->SetMarkerStyle(20);
   h2->SetMarkerStyle(20);
   r1->SetMarkerStyle(20);
   r2->SetMarkerStyle(20);
   h1->SetMarkerColor(2);
   r1->SetMarkerColor(2);
   h2->SetMarkerColor(4);
   r2->SetMarkerColor(4);
   TCanvas* c1=new TCanvas();
   c1->Divide(1,2);
   c1->cd(1);
   h1->Draw();
   h2->Draw("same");
   c1->cd(2);

   gStyle->SetPalette(1);
   //r1->Draw();

   r1->Draw();
   r2->Draw("same");
}
