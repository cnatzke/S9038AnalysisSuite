#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TGaxis.h"

void Compare90SrToBG2(){

   // Inputs and Control
   /*
      TString PlotTitle("12hrs: 90Sr source vs Room BG, 14uA protons");
      TString SrRunName("/data2/S9038/Histograms/Runs_15664-15669-90Sr-with-14uA-protons.root");
      TString BgRunName("/data2/S9038/Histograms/Runs_15687-15692-BG-with-14uA-protons.root");
      TString SrHistoName("addbackESum_Supp");
      TString SrLegend("90Sr runs 15664-15669");
      TString BgHistoName("addbackESum_Supp");
      TString BgLegend("BG runs 15687-15692");
      int YMax=600;
      */
   /*
      TString PlotTitle("24hrs: 90Sr source vs Room BG, no protons");
      TString SrRunName("/data2/S9038/Histograms/Runs_15734-15737-90Sr-no-protons.root");
      TString BgRunName("/data2/S9038/Histograms/Runs_15720-15722-BG-no-protons.root");
      TString SrHistoName("addbackESum_Supp");
      TString SrLegend("90Sr runs 15734-15737");
      TString BgHistoName("addbackESum_Supp");
      TString BgLegend("BG runs 15720-15722");
      int YMax=1000;
      */
   /*
      TString PlotTitle("24hrs: 90Sr source vs Room BG, no protons");
      TString SrRunName("/data2/S9038/Histograms/Runs_15734-15737-90Sr-no-protons.root");
      TString BgRunName("/data2/S9038/Histograms/Runs_15720-15722-BG-no-protons.root");
      TString SrHistoName("addbackESum_PRL");
      TString SrLegend("90Sr runs 15734-15737");
      TString BgHistoName("addbackESum_PRL");
      TString BgLegend("BG runs 15720-15722");
      int YMax=55;
      */
   /*
      TString PlotTitle("48hrs: 90Sr source vs Room BG, no protons");
      TString SrRunName("/data2/S9038/Histograms/Runs_15734-15740-90Sr-no-protons.root");
      TString BgRunName("/data2/S9038/Histograms/Runs_15716-15722-BG-no-protons.root");
      TString SrHistoName("addbackESum_Supp");
      TString SrLegend("90Sr runs 15734-15740");
      TString BgHistoName("addbackESum_Supp");
      TString BgLegend("BG runs 15716-15722");
      int YMax=2000;
      */
   /*
      TString PlotTitle("99hrs 90Sr source vs 48hrs Room BG, no protons");
      TString SrRunName("/data2/S9038/Histograms/Runs_15733-15746-90Sr-no-protons.root");
      TString BgRunName("/data2/S9038/Histograms/Runs_15716-15722-BG-no-protons.root");
      TString SrHistoName("addbackESum_Supp");
      TString SrLegend("90Sr runs 15733-15746");
      TString BgHistoName("addbackESum_Supp");
      TString BgLegend("BG runs 15716-15722");
      int YMax=4000;
      */
   /*
      TString PlotTitle("99hrs 90Sr source vs 48hrs Room BG, no protons");
      TString SrRunName("/data2/S9038/Histograms/Runs_15733-15746-90Sr-no-protons.root");
      TString BgRunName("/data2/S9038/Histograms/Runs_15716-15722-BG-no-protons.root");
      TString SrHistoName("addbackESum_Aggr_Supp");
      TString SrLegend("90Sr runs 15733-15746");
      TString BgHistoName("addbackESum_Aggr_Supp");
      TString BgLegend("BG runs 15716-15722");
      int YMax=2000;
      */
   /*
      TString PlotTitle("99hrs 90Sr source vs 48hrs Room BG, no protons");
      TString SrRunName("/data2/S9038/Histograms/Runs_15733-15746-90Sr-no-protons.root");
      TString BgRunName("/data2/S9038/Histograms/Runs_15716-15722-BG-no-protons.root");
      TString SrHistoName("addbackESum_PRL");
      TString SrLegend("90Sr runs 15733-15746");
      TString BgHistoName("addbackESum_PRL");
      TString BgLegend("BG runs 15716-15722");
      int YMax=200;
      */
   /*
      TString PlotTitle("90Sr: 120hrs 30ns vs 99hrs 120ns, no protons");
      TString SrRunName("/data2/S9038/Histograms/Runs_15733-15746-90Sr-no-protons.root");
      TString BgRunName("/data2/S9038/Histograms/TwoPhotonGriffinAnglePlots15738-15752-90Sr-120hrs.root");
      TString SrHistoName("addbackESum_Supp");
      TString SrLegend("90Sr 15733-15746, 120ns");
      TString BgHistoName("addbackESum_Supp");
      TString BgLegend("90Sr 15738-15752, 30ns");
      int YMax=4000;
      */

   TString PlotTitle("200hrs 90Sr source vs 3.3x 48hrs Room BG, no protons");
   //TString SrRunName("/data2/S9038/Histograms/TwoPhotonGriffinAnglePlots15738-15759.root");
   TString SrRunName("/data4/S9038/Histograms/TwoPhotonGriffinAnglePlots15734-15759-90Sr-200hrs.root");
   TString BgRunName("/data4/S9038/Histograms/TwoPhotonGriffinAnglePlots15716-15722-BGNoProtons-48hrs.root");
   TString SrHistoName("addbackESum_EnergyAngleRestriction");
   TString SrLegend("90Sr runs 15734-15759");
   TString BgHistoName("addbackESum_EnergyAngleRestriction");
   TString BgLegend("BG runs 15716-15722 x3.3");
   TString BgSubLegend("Room Background Subtracted 90Sr");
   int YMax=1600;

   //Declare and set up the Canvas
   TCanvas *c1 = new TCanvas("c1",PlotTitle,600,400);
   c1->Divide(1,2);
   TPad* pada = (TPad*)c1->GetPad(1); 
   gStyle->SetOptStat(kFALSE);

   // Get 90Sr histogram
   TFile f1(SrRunName);
   TH1* h1 = 0;
   f1.GetObject(SrHistoName,h1);

   h1->GetXaxis()->SetRange(1700, 1850);
   h1->GetYaxis()->SetRangeUser(0, YMax);
   h1->Draw();
   c1->Update();

   // Get background histogram
   TFile f2(BgRunName);
   TH1* h2 = 0;
   f2.GetObject(BgHistoName,h2);
   h2->Add(h2,2.3); // now x3.3

   h2->SetLineColor(kRed);
   h2->GetXaxis()->SetRange(1700, 1850);
   h2->GetYaxis()->SetRangeUser(0, YMax);
   h2->Draw("same");

   // Add vertical line to indicate expected peak location
   TLine *l = new TLine(1761,0,1761,YMax);
   l->SetLineStyle(2);
   l->Draw("same");

   // Time normalized BG subtraction
   TH1D* h3 = (TH1D*)h1->Clone("h3");
   h3->Add(h2, -1);
   h3->SetLineColor(kBlack);
   //h3->SetLineStyle(kDashed);
   h3->GetXaxis()->SetRange(1700, 1850);
   h3->GetYaxis()->SetRangeUser(0, YMax);
   h3->Draw("same");

   // Add the Legend
   auto legend = new TLegend(0.65,0.65,0.95,0.9);
   legend->AddEntry(h1,SrLegend,"l");
   legend->AddEntry(h2,BgLegend,"l");
   legend->AddEntry(h3,BgSubLegend,"l");
   legend->Draw();

   /*
      h1->Add(h2,-1.9);
      h1->GetYaxis()->SetRangeUser(0, YMax);
      h1->SetLineColor(kBlue);
      h1->Draw("same");
      */

   c1->Update();

   // Wait for Press any key to continue...
   cin.get();

}
