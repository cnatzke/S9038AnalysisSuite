#define TwoPhotonGriffinTimingSelector_cxx
// The class definition in TwoPhotonGriffinSelector.h has been generated automatically
#include "TwoPhotonGriffinTimingSelector.h"

int gamma_bin = 8000;
int gamma_min = 0;
int gamma_max = 8000;
float g_energy_thres_aggr = 300.; //AggressiveEnergyThreshold
float gate = 3.; //Energy gate for angle plots (keV)

// prompt coincidences
bool PromptCoincidence(TGriffinHit *h1, TGriffinHit *h2){  //Check if GRIFFIN hits are less then 120 ns apart.
   return std::fabs(h1->GetTime() - h2->GetTime()) < 30.;
}
// time random
bool TimeRandom(TGriffinHit *h1, TGriffinHit *h2){  //Check if hits are less then 500 ns apart.
   return 1000. < std::fabs(h1->GetTime() - h2->GetTime()) && std::fabs(h1->GetTime() - h2->GetTime()) < 2000.;
}
// low energy threshold
bool LowEnergyThreshold(TGriffinHit *h1, TGriffinHit *h2){ // checks if one hit is less than 15 keV
   return h1->GetEnergy() > 15. && h2->GetEnergy() >15.; // 15 keV
}
// Higher energy cut
bool AggressiveEnergyThreshold(TGriffinHit *h1, TGriffinHit *h2){ // checks if hits are above a threshold energy
   return h1->GetEnergy() > g_energy_thres_aggr && h2->GetEnergy() > g_energy_thres_aggr;
}
// energy difference between photons
bool EnergyDiffThreshold(TGriffinHit *h1, TGriffinHit *h2){ // checks if energy difference between photons is less than 500 keV
   return std::fabs(h1->GetEnergy() - h2->GetEnergy()) < 500; // 500 keV
} 
bool AngleRestriction(TGriffinHit* hit_one, TGriffinHit* hit_two){ //Checks for angle between hits
   double angle = hit_one->GetPosition().Angle(hit_two->GetPosition()) * 180. / TMath::Pi();
   return angle > 120.0 && angle < 180.0;
}
bool EnergyGate(TGriffinHit *h1, TGriffinHit *h2, float energy){ // checks for hit within energy range
   double gate_upper = energy + gate;
   double gate_lower = energy - gate;
   return ((h1->GetEnergy() + h2->GetEnergy()) > gate_lower) && ((h1->GetEnergy() + h2->GetEnergy()) < gate_upper);
}

bool GriffinBgoEnergyGate(TGriffinHit *h1, TBgoHit *h2, float energy){ // checks for hit within energy range
   double gate_upper = energy + gate;
   double gate_lower = energy - gate;
   return ((h1->GetEnergy() + h2->GetEnergy()) > gate_lower) && ((h1->GetEnergy() + h2->GetEnergy()) < gate_upper);
}

void TwoPhotonGriffinTimingSelector::CreateHistograms() {
   // get cycle length from ODB - this is in nanoseconds!
   fCycleLength = fPpg->OdbCycleLength();
   std::cout<<"got ODB cycle length "<<fCycleLength<<" ns, "<<fCycleLength/1e6<<" s"<<std::endl;

   //Define Energy Histograms
   fH2["griffinE"] = new TH2D("griffinE","#gamma singles vs. crystal #", 65, 0., 65., 16000, 0., 8000.); //griffin energy (all channels)
   fH2["griffinESupp"] = new TH2D("griffinESupp","#gamma suppressed singles vs. crystal #", 65, 0., 65., 16000, 0., 8000.); //griffin energy (all channels)
   fH2["addbackE"] = new TH2D("addbackE","Addback #gamma vs. detector #", 17, 0, 17, 16000, 0., 8000.); //griffin energy (all channels)
   fH2["griffinGriffinE"] = new TH2D("griffinGriffinE","#gamma-#gamma matrix", 4000, 0., 4000., 4000, 0., 4000.); //griffin energy (all channels)
   fH2["griffinGriffinEBg"] = new TH2D("griffinGriffinEBg","#gamma-#gamma matrix, time random BG", 4000, 0., 4000., 4000, 0., 4000.); //griffin energy (all channels)
   fH2["addbackAddbackE"] = new TH2D("addbackAddbackE","addback-addback matrix", 4000, 0., 4000., 4000, 0., 4000.); //griffin energy (all channels)
   fH2["addbackAddbackEBg"] = new TH2D("addbackAddbackEBg","addback-addback matrix, time random BG", 4000, 0., 4000., 4000, 0., 4000.); //griffin energy (all channels)
   fH1["griffinESingles"] = new TH1D("griffinESingles","Griffin Singles", 16000, 0., 4000.); 

   fH2["griffinCfd"] = new TH2D("griffinCfd","energy vs. Time-TS Griffin", 1000, -500., 500., 1000, 0., 5000.); //griffin energy (all channels)

   //Timing between griffin and other detectors including itself
   fH1["griffinGriffinTime"] = new TH1D("griffinGriffinTime", "GRIFFIN-GRIFFIN timing", 2500, -500., 2000.);
   fH1["griffinGriffinTs"] = new TH1D("griffinGriffinTs", "GRIFFIN-GRIFFIN timing using timestamps only", 250, -500., 2000.);
   fH2["griffinGriffinTsGamma1"] = new TH2D("griffinGriffinTsGamma1", "GRIFFIN-GRIFFIN energy vs timestamp in cycle of #gamma_1;Timestamps #gamma_1 in cycle [ns];Energy #gamma_1 [keV]", fCycleLength/1e4, 0., fCycleLength/1e6, 4000., 0., 4000.);
   fH2["griffinGriffinTsGamma2"] = new TH2D("griffinGriffinTsGamma2", "GRIFFIN-GRIFFIN energy vs timestamp in cycle of #gamma_2;Timestamps #gamma_2 in cycle [ns];Energy #gamma_2 [keV];",  fCycleLength/1e4, 0., fCycleLength/1e6, 4000., 0., 4000.);

   //Cycle histograms
   fH2["griffinCycle"] = new TH2D("griffinCycle", "#gamma energy vs. cycle time", fCycleLength/1e4, 0., fCycleLength/1e6, 4000, 0., 4000.);
   fH1["griftimestamp"] = new TH1D("griftimestamp", "grif time", 4500., 0., 4500.);
   fH1["griftimestampbig"] = new TH1D("griftimestampbig", "grif time big", 450., 0., 450.);

   //time histograms
   fH2["griffinTime"] = new TH2D("griffinTime", "#gamma energy vs. time", 3600, 0., 3600., 4000, 0., 4000.);
   fH2["griffinBgoTimeDiff"] = new TH2D("griffinBgoTimeDiff", "Bgo-Ge #Deltat;#Deltat [ns];Energy [keV]", gamma_bin, gamma_min, gamma_max, 2000, 0., 2000.);
   fH1["griffinGriffinTimeDiff"] = new TH1D("griffinGriffinTimeDiff","Ge-Ge delta time", 2000, 0., 2000.); 
   fH1["addbackBgoTimeDiff"] = new TH1D("addbackBgoTimeDiff", "Bgo-Ge #Deltat;#Deltat [ns];Counts [1 ns/bin]", 2000, 0., 2000.);

   //Sum Histograms
   fH1["griffinESum"] = new TH1D("griffinESum","Griffin Energy Sum; Energy [keV]; Counts per 1 keV", gamma_bin, gamma_min, gamma_max);
   fH1["griffinESum_Aggr"] = new TH1D("griffinESum_Aggr","Griffin Energy Sum (#gamma_{1,2} > 300 keV); Energy [keV]; Counts per 1 keV", gamma_bin, gamma_min, gamma_max);
   fH1["griffinESum_Supp"] = new TH1D("griffinESum_Supp","Griffin Suppressed Energy Sum; Energy [keV]; Counts per 1 keV", gamma_bin, gamma_min, gamma_max);
   fH1["griffinESum_PRL"] = new TH1D("griffinESum_PRL","Griffin Energy Sum (120<#theta_{1,2}<180, #DeltaE_{#gamma}<500keV); Energy [keV]; Counts per 1 keV", gamma_bin, gamma_min, gamma_max);

   fH1["addbackESum_Supp"] = new TH1D("addbackESum_Supp","Griffin Addback Suppressed Energy Sum; Energy [keV]; Counts per 1 keV", gamma_bin, gamma_min, gamma_max);
   fH1["addbackESum_PRL"] = new TH1D("addbackESum_PRL","Addback Suppressed Energy Sum (120<#theta_{1,2}<180, #DeltaE_{#gamma}<500keV); Energy [keV]; Counts per 1 keV", gamma_bin, gamma_min, gamma_max);
   fH1["addbackESum_Aggr_Supp"] = new TH1D("addbackESum_Aggr_Supp","Griffin Addback Suppressed Energy Sum (#gamma_{1,2} > 300 keV); Energy [keV]; Counts per 1 keV", gamma_bin, gamma_min, gamma_max);
   fH2["addbackComptonHitPattern_Supp"] = new TH2D("addbackComptonHitPattern_Supp","#gamma Sum Energy vs. detector # of highest energy deposit", 17, 0, 17, gamma_bin, gamma_min, gamma_max);
    
   // Compton Gated Histograms
   fH2["griffinGriffinE_1764Compton"] = new TH2D("griffinGriffinE_1764Compton","#gamma-#gamma matrix 1764 keV Compton Gated; #gamma_1 [keV]; #gamma_2 [keV]", 4000, 0., 4000., 4000, 0., 4000.);
   fH2["griffinBgoE_1764Compton"] = new TH2D("griffinBgoE_1764Compton","#gamma-bgo matrix 1764 keV Compton Gated; Ge [keV]; bgo [keV]", 4000, 0., 4000., 4000, 0., 4000.);
   fH2["griffinBgoE"] = new TH2D("griffinBgoE","#gamma-bgo matrix; Ge [keV]; bgo [keV]", 4000, 0., 4000., 4000, 0., 4000.);

   //Send histograms to Output list to be added and written.
   for(auto it : fH1) {
      GetOutputList()->Add(it.second);
   }
   for(auto it : fH2) {
      GetOutputList()->Add(it.second);
   }
   for(auto it : fHSparse) {
      GetOutputList()->Add(it.second);
   }
}

void TwoPhotonGriffinTimingSelector::FillHistograms() {
   //GRIFFIN non-addback
   for(auto g = 0; g < fGrif->GetMultiplicity(); ++g) {
      auto grif = fGrif->GetGriffinHit(g);
      fH2.at("griffinE")->Fill(grif->GetArrayNumber(), grif->GetEnergy()); 
      fH1.at("griffinESingles")->Fill(grif->GetEnergy());
      fH2.at("griffinCfd")->Fill(grif->GetTime()-grif->GetTimeStampNs(), grif->GetEnergy());
      fH2.at("griffinCycle")->Fill(fmod(grif->GetTime()/1e3, fCycleLength)/1e6, grif->GetEnergy());
      fH2.at("griffinTime")->Fill(grif->GetTimeStampNs()/1e9, grif->GetEnergy());
      fH1.at("griftimestamp")->Fill(grif->GetTime()/1e7);
      fH1.at("griftimestampbig")->Fill(grif->GetTime()/1e9);

      //GRIFFIN-GRIFFIN
      for(auto g2 = g+1; g2 < fGrif->GetMultiplicity(); ++g2) {
         auto grif2 = fGrif->GetGriffinHit(g2);
         fH1.at("griffinGriffinTime")->Fill(grif2->GetTime() - grif->GetTime());
         fH1.at("griffinGriffinTs")->Fill(grif2->GetTimeStampNs() - grif->GetTimeStampNs());
         if(PromptCoincidence(grif, grif2)) {
            fH2.at("griffinGriffinE")->Fill(grif->GetEnergy(), grif2->GetEnergy());
            fH2.at("griffinGriffinE")->Fill(grif2->GetEnergy(), grif->GetEnergy());
            fH2.at("griffinGriffinTsGamma1")->Fill(fmod(grif->GetTimeStampNs()/1e3, fCycleLength)/1e6, grif->GetEnergy());
            //fH2.at("griffinGriffinTsGamma1")->Fill(grif2->GetEnergy(), fmod(grif->GetTimeStampNs()/1e3, fCycleLength)/1e6);
            fH2.at("griffinGriffinTsGamma2")->Fill(fmod(grif2->GetTimeStampNs()/1e3, fCycleLength)/1e6, grif2->GetEnergy());
            //fH2.at("griffinGriffinTsGamma2")->Fill(grif->GetEnergy(), fmod(grif2->GetTimeStampNs()/1e3, fCycleLength)/1e6);
         } else if(TimeRandom(grif, grif2)) {
            fH2.at("griffinGriffinEBg")->Fill(grif->GetEnergy(), grif2->GetEnergy());
            fH2.at("griffinGriffinEBg")->Fill(grif2->GetEnergy(), grif->GetEnergy());
         }
      }


      bool prompt = false;
      bool bg = false;

   }

   //GRIFFIN Suppressed
   for (auto g1 = 0; g1 < fGrif->GetSuppressedMultiplicity(fBgo); ++g1){
      auto grif1 = fGrif->GetSuppressedHit(g1);
      // Bgo
      for (auto b1 = 0; b1 < fBgo->GetMultiplicity(); ++b1){
         auto grif_bgo = fBgo->GetBgoHit(b1);
         fH2.at("griffinBgoTimeDiff")->Fill(grif1->GetEnergy(), std::fabs(grif_bgo->GetTime() - grif1->GetTime()));
         fH2.at("griffinBgoE")->Fill(grif1->GetEnergy(), grif_bgo->GetEnergy());
         if (GriffinBgoEnergyGate(grif1, grif_bgo, 1764)){
            fH2.at("griffinBgoE_1764Compton")->Fill(grif1->GetEnergy(), grif_bgo->GetEnergy());
         }
      } // end BGO-timing

      // GRIFFIN Second hit
      for(auto g2 = g1+1; g2 < fGrif->GetMultiplicity(); ++g2) {
         auto grif2 = fGrif->GetGriffinHit(g2);
         if(PromptCoincidence(grif1, grif2) && EnergyGate(grif1, grif2, 1764)) {
            fH2.at("griffinGriffinE_1764Compton")->Fill(grif1->GetEnergy(), grif2->GetEnergy());
            fH2.at("griffinGriffinE_1764Compton")->Fill(grif2->GetEnergy(), grif1->GetEnergy());
         }
      } // end GRIFFIN second hit
   } // end GRIFFIN Suppressed

   //GRIFFIN Addback Sum
   if (fGrif->GetSuppressedAddbackMultiplicity(fBgo) == 2){
      for(auto g1 = 0; g1 < fGrif->GetSuppressedAddbackMultiplicity(fBgo); ++g1) {
         auto grif1 = fGrif->GetSuppressedAddbackHit(g1);

         //BGO-timing
         for (auto b1 = 0; b1 < fBgo->GetMultiplicity(); ++b1){
            auto grif_bgo = fBgo->GetBgoHit(b1);
            fH1.at("addbackBgoTimeDiff")->Fill(std::fabs(grif_bgo->GetTime() - grif1->GetTime()));
         } // end BGO-timing

         //Second hit
         for (auto g2=g1+1; g2 < fGrif->GetSuppressedAddbackMultiplicity(fBgo); ++g2){
            auto grif2 = fGrif->GetSuppressedAddbackHit(g2);

            if (EnergyGate(grif1, grif2, 1764)){
               fH1.at("griffinGriffinTimeDiff")->Fill(std::fabs(grif1->GetTime() - grif2->GetTime()));
            }

            if (PromptCoincidence(grif1, grif2)){
               if (LowEnergyThreshold(grif1,grif2)){
                  fH1.at("addbackESum_Supp")->Fill(grif1->GetEnergy()+grif2->GetEnergy());
                  if (grif1->GetEnergy() > grif2->GetEnergy()){
                     fH2.at("addbackComptonHitPattern_Supp")->Fill(grif1->GetDetector(), grif1->GetEnergy() + grif2->GetEnergy());
                  }
                  else {
                     fH2.at("addbackComptonHitPattern_Supp")->Fill(grif2->GetDetector(), grif1->GetEnergy() + grif2->GetEnergy());
                  }
               }
               if (AggressiveEnergyThreshold(grif1,grif2)){
                  fH1.at("addbackESum_Aggr_Supp")->Fill(grif1->GetEnergy()+grif2->GetEnergy());
               }
               if (EnergyDiffThreshold(grif1, grif2) && AngleRestriction(grif1, grif2)){
                  fH1.at("addbackESum_PRL")->Fill(grif1->GetEnergy()+grif2->GetEnergy());
               }
            } 
         } //end second hit
      }
   } // end GRIFFIN Addback Sum

}
