#include <iostream> 
#include "TH1F.h"
#include "THStack.h"
#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "SYST.h"
#include "../../../Tools/utils.h"
#include "../../../CORE/SSSelections.h"

using namespace std;

//Switches
bool doOld = true;

int signalRegionAG(int njets, int nbtags, float met, float ht, float mt_min, float lep1_pt, float lep2_pt){
  if (ht != ht) return -1; //this is to protect against NANs
  if (!doOld) return signalRegion(njets, nbtags, met, ht, mt_min, lep1_pt, lep2_pt);
  else {
    if (met < 50 || njets < 2 || lep1_pt < 20 || lep2_pt < 20 || ht < 200) return 0;
    if (met >  50 && met < 120 && ht >= 200 && ht <= 400 && njets >= 2 && njets < 4) return 1;
    if (met >  50 && met < 120 && ht >  400 &&              njets >= 2 && njets < 4) return 2;
    if (met >  50 && met < 120 && ht >= 200 && ht <= 400 && njets >= 4             ) return 3;
    if (met >  50 && met < 120 && ht >  400 &&              njets >= 4             ) return 4;
    if (met > 120 &&              ht >= 200 && ht <= 400 && njets >= 2 && njets < 4) return 5;
    if (met > 120 &&              ht >  400 &&              njets >= 2 && njets < 4) return 6;
    if (met > 120 &&              ht >= 200 && ht <= 400 && njets >= 4             ) return 7;
    if (met > 120 &&              ht >  400 &&              njets >= 4             ) return 8;
  }
  cout << "WARNING!  No old SR found! Should never get here!" << endl;
  return -1;
}

void run(TChain* chain){

  //Constants
  float lumi = 10.0;
  float scale1fb = 1000.0/1000000.0; //weight provided must be provided by nEvents (denom).  Numer is to convert to fb.

  //Histograms for cross-section calculation
  TH1F* cs = new TH1F("cs","cs", 1, 0, 1);
  cs->Sumw2();

  //Histograms for cross-section scale errors
  float cs_scale_up = 0;
  float cs_scale_dn = 0;

  //Histograms for total number of events
  TH1F *total = new TH1F("total", "total", 1, 0, 1);
  total->Sumw2();
 
  //Floats for total nEvents scale errors
  float nTotal_scale_up = 0;
  float nTotal_scale_dn = 0;

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //PDF floats for nTotal
  float cs_pdf[40] = { 0 }; 
  float nTotalPDF[40] = { 0 }; 

  //SR Histos
  TH1F* HHSR[(doOld ? 8 : 32)];
  TH1F* HLSR[26];
  TH1F* LLSR[8];
  for (unsigned int i = 0; i < (doOld ? 8 : 32); i++) HHSR[i] = new TH1F(Form("histo3%i", i), "hist", 1, 0, 1);
  for (unsigned int i = 0; i < 26; i++) HLSR[i] = new TH1F(Form("histo4%i", i), "hist", 1, 0, 1);
  for (unsigned int i = 0; i <  8; i++) LLSR[i] = new TH1F(Form("histo5%i", i), "hist", 1, 0, 1);
  for (unsigned int i = 0; i < (doOld ? 8 : 32); i++) HHSR[i]->Sumw2();
  for (unsigned int i = 0; i < 26; i++) HLSR[i]->Sumw2();
  for (unsigned int i = 0; i <  8; i++) LLSR[i]->Sumw2();

  //Scale Histos for SR
  float HHSR_scale_up[(doOld ? 8 : 32)];
  float HLSR_scale_up[26];
  float LLSR_scale_up[8];
  float HHSR_scale_dn[(doOld ? 8 : 32)];
  float HLSR_scale_dn[26];
  float LLSR_scale_dn[8];
 
  //PDF errors for SR
  float sr_pdf_hh[(doOld ? 8 : 32)][40];
  float sr_pdf_hl[26][40];
  float sr_pdf_ll[8][40];
  for (int i = 0; i < (doOld ? 8 : 32); i++){ for (int j = 0; j < 40; j++) sr_pdf_hh[i][j] = 0; }
  for (int i = 0; i < 26;               i++){ for (int j = 0; j < 40; j++) sr_pdf_hl[i][j] = 0; }
  for (int i = 0; i < 8;                i++){ for (int j = 0; j < 40; j++) sr_pdf_ll[i][j] = 0; }

  //Initialize arrays
  for (unsigned int i = 0; i < sizeof(HHSR_scale_up)/sizeof(*HHSR_scale_up); i++) HHSR_scale_up[i] = 0; 
  for (unsigned int i = 0; i < sizeof(HLSR_scale_up)/sizeof(*HHSR_scale_up); i++) HLSR_scale_up[i] = 0; 
  for (unsigned int i = 0; i < sizeof(LLSR_scale_up)/sizeof(*HHSR_scale_up); i++) LLSR_scale_up[i] = 0; 
  for (unsigned int i = 0; i < sizeof(HHSR_scale_dn)/sizeof(*HHSR_scale_up); i++) HHSR_scale_dn[i] = 0; 
  for (unsigned int i = 0; i < sizeof(HLSR_scale_dn)/sizeof(*HHSR_scale_up); i++) HLSR_scale_dn[i] = 0; 
  for (unsigned int i = 0; i < sizeof(LLSR_scale_dn)/sizeof(*HHSR_scale_up); i++) LLSR_scale_dn[i] = 0; 

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("Events");
    systematics.Init(tree);

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      systematics.GetEntry(event);
      nEventsTotal++;
    
      //Progress
      SYST::progress(nEventsTotal, nEventsChain);

      //Fill c-s histo
      cs->Fill(0.5, syst::weight());  
 
      //Fill c-s scale error histos
      cs_scale_up += syst::weights().at(4);
      cs_scale_dn += syst::weights().at(8);
  
      //Fill c-s PDF error histos
      for (int i = 0; i < 40; i++) cs_pdf[i] += syst::weights().at(i+9);  

      //Fill total nEvents histos
      total->Fill(0.5, syst::weight()*scale1fb*lumi);
      nTotal_scale_up += syst::weights().at(4)*scale1fb*lumi;
      nTotal_scale_dn += syst::weights().at(8)*scale1fb*lumi;
      for (int i = 0; i < 40; i++) nTotalPDF[i] += syst::weights().at(i+9)*scale1fb*lumi;  

      //Reject all events that are not "true SS"
      if (!syst::keep()) continue;
      if (syst::id1()*syst::id2() <= 0) continue;
    
      //Determine SR for the event
      float mt_l1 = MT(syst::lep1().pt(), syst::lep1().phi(), syst::met(), syst::metphi());
      float mt_l2 = MT(syst::lep2().pt(), syst::lep2().phi(), syst::met(), syst::metphi());
      float mt_min = (mt_l1 > mt_l2 ? mt_l2 : mt_l1); 
      int SR = signalRegionAG(syst::nJets40(), syst::nBtags(), syst::met(), syst::ht(), mt_min, syst::lep1().pt(), syst::lep2().pt());
      anal_type_t hyp_type = doOld ? HighHigh : analysisCategory(syst::lep1().pt(), syst::lep2().pt());
      if (SR <= 0) continue;

      //Fill SR histograms -- yields
      if (hyp_type == HighHigh) HHSR[SR-1]->Fill(0.5, syst::weight()*scale1fb*lumi);
      if (hyp_type == HighLow)  HLSR[SR-1]->Fill(0.5, syst::weight()*scale1fb*lumi);
      if (hyp_type == LowLow)   LLSR[SR-1]->Fill(0.5, syst::weight()*scale1fb*lumi);

      //Determine SR -- scale errors
      if (hyp_type == HighHigh) HHSR_scale_up[SR-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == HighLow)  HLSR_scale_up[SR-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == LowLow)   LLSR_scale_up[SR-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == HighHigh) HHSR_scale_dn[SR-1] += syst::weights().at(8)*scale1fb*lumi;
      if (hyp_type == HighLow)  HLSR_scale_dn[SR-1] += syst::weights().at(8)*scale1fb*lumi;
      if (hyp_type == LowLow)   LLSR_scale_dn[SR-1] += syst::weights().at(8)*scale1fb*lumi;

      //Fill nEvents PDF error histos
      if (hyp_type == HighHigh) for (int i = 0; i < 40; i++) sr_pdf_hh[SR-1][i] += syst::weights().at(i+9)*scale1fb*lumi;  
      if (hyp_type == HighLow)  for (int i = 0; i < 40; i++) sr_pdf_hl[SR-1][i] += syst::weights().at(i+9)*scale1fb*lumi;  
      if (hyp_type == LowLow)   for (int i = 0; i < 40; i++) sr_pdf_ll[SR-1][i] += syst::weights().at(i+9)*scale1fb*lumi;  

    }//event loop
  }//file loop

  //Calculate c-s PDF errors
  float cs_pdf_up = 0;
  float cs_pdf_dn = 0;
  for (int i = 1; i <= 20; i++){
    float pdf_up_temp = max(cs_pdf[2*i-2]- cs->Integral(), cs_pdf[2*i-1] - cs->Integral());
    float pdf_dn_temp = max(cs->Integral() - cs_pdf[2*i-2], cs->Integral() - cs_pdf[2*i-1]);
    cs_pdf_up += (pdf_up_temp > 0) ? pow(pdf_up_temp, 2) : 0;
    cs_pdf_dn += (pdf_dn_temp > 0) ? pow(pdf_dn_temp, 2) : 0;
  }

  //Print results 
  cout << "Cross-Section: " << 1000*cs->Integral()/cs->GetEntries() << endl;
  cout << "  --> Stat err (%): " << 100*cs->GetBinError(1)/cs->Integral() << endl;
  cout << "  --> Scale up (%): " <<  100*(cs_scale_dn - cs->Integral())/cs->Integral() << endl;
  cout << "  --> Scale dn (%): " << -100*(cs_scale_up - cs->Integral())/cs->Integral() << endl;
  cout << "  --> PDF up (%):   " << 100*sqrt(cs_pdf_up)/cs->Integral() << endl;
  cout << "  --> PDF dn (%):   " << 100*sqrt(cs_pdf_dn)/cs->Integral() << endl;

  //Delete c-s histos
  delete cs; 

  //Calculate nTotal with statistical error
  float nTotal      = total->Integral();
  float nTotal_stat = total->GetBinError(1)/nTotal;

  //Calculate nTotal PDF errors
  float nTotal_pdf_up = 0;
  float nTotal_pdf_dn = 0;
  for (int i = 1; i <= 20; i++){
    float pdf_up_temp = max(nTotalPDF[2*i-2] - nTotal, nTotalPDF[2*i-1] - nTotal);
    float pdf_dn_temp = max(nTotal - nTotalPDF[2*i-2], nTotal - nTotalPDF[2*i-1]);
    nTotal_pdf_up += (pdf_up_temp > 0) ? pow(pdf_up_temp, 2) : 0;
    nTotal_pdf_dn += (pdf_dn_temp > 0) ? pow(pdf_dn_temp, 2) : 0;
  }

  //Print results
  cout << " " << endl;
  cout << "nTotal: " << nTotal << endl;
  cout << "  --> Stat err (%): " <<  100*nTotal_stat                       << endl;
  cout << "  --> Scale up (%): " <<  100*(nTotal_scale_dn - nTotal)/nTotal << endl;
  cout << "  --> Scale dn (%): " << -100*(nTotal_scale_up - nTotal)/nTotal << endl;
  cout << "  --> PDF up (%):   " <<  100*sqrt(nTotal_pdf_up)/nTotal        << endl;
  cout << "  --> PDF dn (%):   " <<  100*sqrt(nTotal_pdf_dn)/nTotal        << endl;

  //Calculate SR PDF errors (HH)
  float sr_pdf_up_hh[doOld ? 8 : 32];
  float sr_pdf_dn_hh[doOld ? 8 : 32];
  for (unsigned int k = 0; k < sizeof(sr_pdf_up_hh)/sizeof(*sr_pdf_up_hh); k++) sr_pdf_up_hh[k] = 0; 
  for (unsigned int k = 0; k < sizeof(sr_pdf_dn_hh)/sizeof(*sr_pdf_dn_hh); k++) sr_pdf_dn_hh[k] = 0; 
  for (unsigned int i = 0; i < (doOld ? 8 : 32); i++){
    float value = HHSR[i]->Integral();
    for (int j = 1; j <= 20; j++){
      float pdf_up_temp = max(sr_pdf_hh[i][2*j-2] - value, sr_pdf_hh[i][2*j-1] - value);
      float pdf_dn_temp = doOld ? -min(sr_pdf_hh[i][2*j-2] - value, sr_pdf_hh[i][2*j-1] - value) : max(value - sr_pdf_hh[i][2*j-2], value - sr_pdf_hh[i][2*j-1]);
      sr_pdf_up_hh[i] += (pdf_up_temp > 0) ? pow(pdf_up_temp, 2) : 0;
      sr_pdf_dn_hh[i] += (pdf_dn_temp > 0) ? pow(pdf_dn_temp, 2) : 0;
    }
  }


  //Print SR results (HH)
  for (unsigned int i = 0; i < (doOld ? 8 : 32); i++){
    float value = HHSR[i]->Integral();
    float stat = HHSR[i]->GetBinError(1);
    float eff   = value/nTotal;
    float eff_stat = sqrt(((1-2*eff)*pow(stat, 2) + pow(eff*nTotal_stat, 2)) / pow(nTotal, 2));
    float eff_scale_1 = HHSR_scale_dn[i]/nTotal_scale_dn - eff;
    float eff_scale_2 = HHSR_scale_up[i]/nTotal_scale_up - eff;
    float eff_pdf_1 = (value + sqrt(sr_pdf_up_hh[i]))/(nTotal + sqrt(nTotal_pdf_up)) - eff;
    float eff_pdf_2 = (value - sqrt(sr_pdf_dn_hh[i]))/(nTotal - sqrt(nTotal_pdf_dn)) - eff;
    if (doOld){ //bugs intentionally reproduced
      eff_pdf_1 = (value + sqrt(sr_pdf_up_hh[i]))/(nTotal + sqrt(nTotal_pdf_up)) - eff;
      eff_pdf_2 = (value + sqrt(sr_pdf_dn_hh[i]))/(nTotal + sqrt(nTotal_pdf_up)) - eff;
    }
    cout << " " << endl;
    cout << "SR " << i+1 << ": "       << Form("% 9.5f",  value                                          ) << endl;
    cout << "  --> Stat err (%):     " << Form("% 9.5f",  100*stat/value                                 ) << endl;
    cout << "  --> Scale up (%):     " << Form("% 9.5f",  100*(HHSR_scale_dn[i] - value)/value           ) << endl;
    cout << "  --> Scale dn (%):     " << Form("% 9.5f", -100*(HHSR_scale_up[i] - value)/value           ) << endl;
    cout << "  --> PDF up (%):       " << Form("% 9.5f",  100*sqrt(sr_pdf_up_hh[i])/value                ) << endl;
    cout << "  --> PDF dn (%):       " << Form("% 9.5f",  100*sqrt(sr_pdf_dn_hh[i])/value                ) << endl;
    cout << "  --> Efficiency:       " << Form("% 9.5f",  eff                                            ) << endl;
    cout << "  --> Eff stat (%):     " << Form("% 9.5f",  100*eff_stat/eff                               ) << endl;
    cout << "  --> Eff scale up (%): " << Form("% 9.5f",  100*max(eff_scale_1, eff_scale_2)/eff          ) << endl;
    cout << "  --> Eff scale dn (%): " << Form("% 9.5f", -100*min(eff_scale_1, eff_scale_2)/eff          ) << endl;
    cout << "  --> Eff PDF up (%):   " << Form("% 9.5f",  100*max(eff_pdf_1, eff_pdf_2)/eff              ) << endl;
    cout << "  --> Eff PDF dn (%):   " << Form("% 9.5f", -100*min(eff_pdf_1, eff_pdf_2)/eff              ) << endl;

  }

}

int analysis(){

  //Declare Chains
  TChain *fixed = new TChain("Events");
  TChain *dynam = new TChain("Events");

  //Fill Chains
  string infix = "fall2014";
  string suffix = "_noFilt";
  fixed->Add(Form("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/%s/myTTW_fixed%s.root", doOld ? infix.c_str() : "", doOld ? "" : suffix.c_str()));
  dynam->Add(Form("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/%s/myTTW_dynam%s.root", doOld ? infix.c_str() : "", doOld ? "" : suffix.c_str()));

  //Do everything
  run(fixed);
  //run(dynam);

  return 0;

}
