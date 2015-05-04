#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

#include "TCut.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include "/home/users/cgeorge/software/dataMCplotMaker/dataMCplotMaker.h"

using namespace std;

enum pt_region_t { HH, HL, LL }; 

//Switches
int njets_cut = 5;   //njets high will be >= than this
int met_cut = 200;   //met high will be >= than this
int ht_cut_1 = 300;  //ht med will be >= than this
int ht_cut_2 = 500;  //ht high will be >= than this
int mtmin_cut = 120; //mtmin high will be >= than this
int ht_ultra_value = 1600; 
int met_ultra_value = 500; 

//Cuts
TCut weight("scale1fb*10.0");
TCut ht_low(Form("ht > 200 && ht < %i", ht_cut_1));
TCut ht_med(Form("ht > %i && ht < %i", ht_cut_1, ht_cut_2));
TCut nbtags0("nbtags == 0");
TCut nbtags1("nbtags == 1");
TCut nbtags2("nbtags == 2");
TCut nbtags3p("nbtags >= 3");
TCut mtmin_low(Form("mt > mt_l2 ? mt_l2 < %i : mt < %i", mtmin_cut, mtmin_cut));
TCut mtmin_high(Form("mt > mt_l2 ? mt_l2 > %i : mt > %i", mtmin_cut, mtmin_cut));
TCut njets_low(Form("njets >= 2 && njets < %i", njets_cut));
TCut njets_high(Form("njets >= %i", njets_cut));
TCut selectionSS("hyp_class == 3");
TCut highHigh("lep1_p4.pt() > 25 && lep2_p4.pt() > 25"); 
TCut highLow("((lep1_p4.pt() > 10 && lep1_p4.pt() < 25 && lep2_p4.pt() > 25) || (lep1_p4.pt() > 25 && lep2_p4.pt() > 10 && lep2_p4.pt() < 25))"); 
TCut lowLow("((lep1_p4.pt() > 10 && lep2_p4.pt() > 10) && (lep1_p4.pt() < 25 && lep2_p4.pt() < 25))"); 
TCut njets2("njets >= 2");
TCut met50("met > 50");
TCut none("1.0");

//Vectors
vector <float> ttbar_high;
vector <float> ttw_high;
vector <float> ttz_high;
vector <float> wz_high;
vector <float> t1tttt_1200_high;
vector <float> t1tttt_1500_high;
vector <float> t5qqww_1200_high;
vector <float> t5qqww_deg_high;
vector <float> ttbar_hl;
vector <float> ttw_hl;
vector <float> ttz_hl;
vector <float> wz_hl;
vector <float> t1tttt_1200_hl;
vector <float> t1tttt_1500_hl;
vector <float> t5qqww_1200_hl;
vector <float> t5qqww_deg_hl;
vector <float> ttbar_low;
vector <float> ttw_low;
vector <float> ttz_low;
vector <float> wz_low;
vector <float> t1tttt_1200_low;
vector <float> t1tttt_1500_low;
vector <float> t5qqww_1200_low;
vector <float> t5qqww_deg_low;

//Names
vector <char*> titles;
vector <char*> signal_titles;
vector <Color_t> Colors;
vector <Color_t> Colors2;

float sum( vector <float> it ){
  float result = 0;
  for (unsigned int i = 0; i < it.size(); i++) result += it[i];
  return result;
}

void printResults(int nRegions, bool isHighLow){
  if (isHighLow){
    cout << "--------------- Printing high-high results ----------------- " << endl;
    cout << "     ttbar       ttw       ttz      wz      t1tttt (1.2,0.8)   t1tttt(1.5,0.1)    t5qqww(1.2,1.0,0.8)" << endl;
    for (int number = 1; number <= nRegions; number++){
      cout << Form("%3i% 5.2f     % 4.2f     % 4.2f   % 4.2f         % 4.2f              % 4.2f           % 4.2f", number, ttbar_high[number-1], ttw_high[number-1], ttz_high[number-1], wz_high[number-1], t1tttt_1200_high[number-1], t1tttt_1500_high[number-1], t5qqww_1200_high[number-1]) << endl; 
    }
      cout << Form("  % 5.2f     % 4.2f     % 4.2f   % 4.2f         % 4.2f              % 4.2f           % 4.2f", sum(ttbar_high), sum(ttw_high), sum(ttz_high), sum(wz_high), sum(t1tttt_1200_high), sum(t1tttt_1500_high), sum(t5qqww_1200_high)) << endl; 
  
    cout << "--------------- Printing high-low results ----------------- " << endl;
    cout << "     ttbar       ttw       ttz      wz      t1tttt (1.2,0.8)   t1tttt(1.5,0.1)    t5qqww(1.2,1.0,0.8)" << endl;
    for (int number = 1; number <= nRegions; number++){
      cout << Form("%3i% 5.2f     % 4.2f     % 4.2f   % 4.2f         % 4.2f              % 4.2f           % 4.2f", number, ttbar_hl[number-1], ttw_hl[number-1], ttz_hl[number-1], wz_hl[number-1], t1tttt_1200_hl[number-1], t1tttt_1500_hl[number-1], t5qqww_1200_hl[number-1]) << endl; 
    }
    cout << Form("  % 5.2f     % 4.2f     % 4.2f   % 4.2f         % 4.2f              % 4.2f           % 4.2f", sum(ttbar_hl), sum(ttw_hl), sum(ttz_hl), sum(wz_hl), sum(t1tttt_1200_hl), sum(t1tttt_1500_hl), sum(t5qqww_1200_hl)) << endl; 

  }
  else { 
    cout << "--------------- Printing low-low results ----------------- " << endl;
    cout << "     ttbar       ttw       ttz      wz      t1tttt (1.2,0.8)   t1tttt(1.5,0.1)    t5qqww(1.2,1.0,0.8)" << endl;
    for (int number = 1; number <= nRegions; number++){
      cout << Form("%3i% 5.2f     % 4.2f     % 4.2f   % 4.2f         % 4.2f              % 4.2f           % 4.2f", number, ttbar_low[number-1], ttw_low[number-1], ttz_low[number-1], wz_low[number-1], t1tttt_1200_low[number-1], t1tttt_1500_low[number-1], t5qqww_1200_low[number-1]) << endl; 
    }
      cout << Form("  % 5.2f     % 4.2f     % 4.2f   % 4.2f         % 4.2f              % 4.2f           % 4.2f", sum(ttbar_low), sum(ttw_low), sum(ttz_low), sum(wz_low), sum(t1tttt_1200_low), sum(t1tttt_1500_low), sum(t5qqww_1200_low)) << endl; 
  } 
}

vector<float> fillA(TTree *chain){

  //Vector for results;
  vector <float> result;

  //Void, initialize baseline
  TCut baseline = selectionSS + njets2 + met50 + highHigh;
  
  //ht_ultra and met_ultra
  TCut ht_ultra(Form("ht > %i", ht_ultra_value)); 
  TCut met_low(Form("met > 50 && met < %i", met_cut));
  TCut met_high(Form("met >= %i && met < %i", met_cut, met_ultra_value));
  TCut ht_high(Form("ht >= %i && ht < %i", ht_cut_2, ht_ultra_value));
  TCut met_ultra(Form("met > %i", met_ultra_value));
 
  //Define each SR, make yield, plot
  TCut SR1  = nbtags0*ht_low *njets_low *met_low*mtmin_low;
  TCut SR2  = nbtags0*(ht_med || ht_high) *njets_low *met_low*mtmin_low;
  TCut SR3  = nbtags0*ht_low *((njets_high || (njets_low && met_high))*mtmin_low || njets_low*met_low*mtmin_high || (njets_high || (njets_low && met_high))*mtmin_high); 
  TCut SR4  = nbtags0*(ht_med || ht_high)*njets_high*met_low*mtmin_low;
  TCut SR5  = nbtags0*(ht_med || ht_high)*njets_low*met_high*mtmin_low;
  TCut SR6  = nbtags0*(ht_med || ht_high)*njets_high*met_high*mtmin_low;
  TCut SR7 = nbtags0*(ht_med || ht_high)*njets_low*met_low*mtmin_high;
  TCut SR8 = nbtags0*(ht_med || ht_high)*(njets_high || (njets_low && met_high))*mtmin_high; 
  TCut SR9 = nbtags1*ht_low *njets_low *met_low*mtmin_low;
  TCut SR10 = nbtags1*(ht_med || ht_high)*njets_low *met_low*mtmin_low;
  TCut SR11 = nbtags1*ht_low *((njets_high || (njets_low && met_high))*mtmin_low || njets_low*met_low*mtmin_high || (njets_high || (njets_low && met_high))*mtmin_high); 
  TCut SR12 = nbtags1*(ht_med || ht_high) *njets_high*met_low*mtmin_low;
  TCut SR13 = nbtags1*(ht_med || ht_high)*njets_low*met_high*mtmin_low;
  TCut SR14 = nbtags1*(ht_med || ht_high)*njets_high*met_high*mtmin_low;
  TCut SR15 = nbtags1*(ht_med || ht_high)*njets_low*met_low*mtmin_high;
  TCut SR16 = nbtags1*(ht_med || ht_high)*(njets_high || (njets_low && met_high))*mtmin_high; 
  TCut SR17 = nbtags2*ht_low *njets_low *met_low*mtmin_low;
  TCut SR18 = nbtags2*(ht_med || ht_high) *njets_low *met_low*mtmin_low;
  TCut SR19 = nbtags2*ht_low *((njets_high || (njets_low && met_high))*mtmin_low || njets_low*met_low*mtmin_high || (njets_high || (njets_low && met_high))*mtmin_high); 
  TCut SR20 = nbtags2*(ht_med || ht_high)*njets_high*met_low*mtmin_low;
  TCut SR21 = nbtags2*(ht_med || ht_high)*njets_low*met_high*mtmin_low;
  TCut SR22 = nbtags2*(ht_med || ht_high)*njets_high*met_high*mtmin_low;
  TCut SR23 = nbtags2*(ht_med || ht_high)*njets_low*met_low*mtmin_high;
  TCut SR24 = nbtags2*(ht_med || ht_high)*(njets_high || (njets_low && met_high))*mtmin_high; 
  TCut SR25 = nbtags3p*ht_low*met_low*mtmin_low;
  TCut SR26 = nbtags3p*(ht_med || ht_high)*met_low*mtmin_low;
  TCut SR27 = nbtags3p*ht_low*met_high*mtmin_low;
  TCut SR28 = nbtags3p*(ht_med || ht_high)*met_high*mtmin_low;
  TCut SR29 = nbtags3p*ht_low*(met_low || met_high)*mtmin_high; 
  TCut SR30 = nbtags3p*(ht_med || ht_high)*njets2*(met_low || met_high)*mtmin_high; 
  TCut SR31 = met_ultra; 
  TCut SR32 = ht_ultra;

  vector <TCut> cuts;
  cuts.push_back(SR1); 
  cuts.push_back(SR2); 
  cuts.push_back(SR3); 
  cuts.push_back(SR4); 
  cuts.push_back(SR5); 
  cuts.push_back(SR6); 
  cuts.push_back(SR7); 
  cuts.push_back(SR8); 
  cuts.push_back(SR9); 
  cuts.push_back(SR10); 
  cuts.push_back(SR11); 
  cuts.push_back(SR12); 
  cuts.push_back(SR13); 
  cuts.push_back(SR14); 
  cuts.push_back(SR15); 
  cuts.push_back(SR16); 
  cuts.push_back(SR17); 
  cuts.push_back(SR18); 
  cuts.push_back(SR19); 
  cuts.push_back(SR20); 
  cuts.push_back(SR21); 
  cuts.push_back(SR22); 
  cuts.push_back(SR23); 
  cuts.push_back(SR24); 
  cuts.push_back(SR25); 
  cuts.push_back(SR26); 
  cuts.push_back(SR27); 
  cuts.push_back(SR28); 
  cuts.push_back(SR29); 
  cuts.push_back(SR30); 
  cuts.push_back(SR31); 
  cuts.push_back(SR32); 

  for (unsigned int i = 0; i < 32; i++){
    TH1F *hist = new TH1F("hist", "hist", 1, 0, 1);
    chain->Draw("0.5>>hist", baseline*weight*cuts[i]);
    result.push_back(hist->Integral());  
    delete hist;
  }
  return result;
}

vector<float> fillB(TTree *chain){

  //Vector for results;
  vector <float> result;

  //Void, initialize baseline
  TCut baseline = highLow + selectionSS + njets2 + met50;

  //ht_ultra and met_ultra
  TCut ht_ultra(Form("ht > %i", ht_ultra_value)); 
  TCut met_low(Form("met > 50 && met < %i", met_cut));
  TCut met_high(Form("met >= %i && met < %i", met_cut, met_ultra_value));
  TCut ht_high(Form("ht >= %i && ht < %i", ht_cut_2, ht_ultra_value));
  TCut met_ultra(Form("met > %i", met_ultra_value));
 
  //Define each SR, make yield, plot
  TCut SR1  = nbtags0*ht_low *njets_low *met_low*mtmin_low;
  TCut SR2  = nbtags0*(ht_med || ht_high)*njets_low *met_low*mtmin_low;
  TCut SR3  = nbtags0*ht_low *(njets_high || (njets_low && met_high))*mtmin_low; 
  TCut SR4  = nbtags0*(ht_med || ht_high) *njets_high*met_low*mtmin_low;
  TCut SR5  = nbtags0*(ht_med || ht_high)*njets_low*met_high*mtmin_low;
  TCut SR6  = nbtags0*(ht_med || ht_high)*njets_high*met_high*mtmin_low;
  TCut SR7 = nbtags1*ht_low *njets_low *met_low*mtmin_low;
  TCut SR8 = nbtags1*(ht_med || ht_high)*njets_low *met_low*mtmin_low;
  TCut SR9 = nbtags1*ht_low *(njets_high || (njets_low && met_high))*mtmin_low; 
  TCut SR10 = nbtags1*(ht_med || ht_high)*njets_high*met_low*mtmin_low;
  TCut SR11 = nbtags1*(ht_med || ht_high)*njets_low*met_high*mtmin_low;
  TCut SR12 = nbtags1*(ht_med || ht_high)*njets_high*met_high*mtmin_low;
  TCut SR13 = nbtags2*ht_low *njets_low *met_low*mtmin_low;
  TCut SR14 = nbtags2*(ht_med || ht_high)*njets_low *met_low*mtmin_low;
  TCut SR15 = nbtags2*ht_low *(njets_high || (njets_low && met_high))*mtmin_low; 
  TCut SR16 = nbtags2*(ht_med || ht_high)*njets_high*met_low*mtmin_low;
  TCut SR17 = nbtags2*(ht_med || ht_high)*njets_low*met_high*mtmin_low;
  TCut SR18 = nbtags2*(ht_med || ht_high)*njets_high*met_high*mtmin_low;
  TCut SR19 = nbtags3p*ht_low*met_low*mtmin_low;
  TCut SR20 = nbtags3p*(ht_med || ht_high)*met_low*mtmin_low;
  TCut SR21 = nbtags3p*ht_low*met_high*mtmin_low;
  TCut SR22 = nbtags3p*(ht_med || ht_high)*met_high*mtmin_low;
  TCut SR23 = ht_low*mtmin_high*(!met_ultra);
  TCut SR24 = (ht_med || ht_high)*(!met_ultra)*mtmin_high;
  TCut SR25 = met_ultra; 
  TCut SR26 = ht_ultra;

  vector <TCut> cuts;
  cuts.push_back(SR1); 
  cuts.push_back(SR2); 
  cuts.push_back(SR3); 
  cuts.push_back(SR4); 
  cuts.push_back(SR5); 
  cuts.push_back(SR6); 
  cuts.push_back(SR7); 
  cuts.push_back(SR8); 
  cuts.push_back(SR9); 
  cuts.push_back(SR10); 
  cuts.push_back(SR11); 
  cuts.push_back(SR12); 
  cuts.push_back(SR13); 
  cuts.push_back(SR14); 
  cuts.push_back(SR15); 
  cuts.push_back(SR16); 
  cuts.push_back(SR17); 
  cuts.push_back(SR18); 
  cuts.push_back(SR19); 
  cuts.push_back(SR20); 
  cuts.push_back(SR21); 
  cuts.push_back(SR22); 
  cuts.push_back(SR23); 
  cuts.push_back(SR24); 
  cuts.push_back(SR25); 
  cuts.push_back(SR26); 

  for (unsigned int i = 0; i < 26; i++){
    TH1F *hist = new TH1F("hist", "hist", 1, 0, 1);
    chain->Draw("0.5>>hist", baseline*weight*cuts[i]);
    result.push_back(hist->Integral());  
    delete hist;
  }
  return result;
}

vector<float> fillC(TTree *chain){

  //Vector for results;
  vector <float> result;

  //-------CUTS-------
  //Void, initialize baseline
  TCut baseline = selectionSS + njets2 + met50 + lowLow;
  TCut met_low(Form("met < %i", met_cut));
  TCut met_high(Form("met >= %i", met_cut));
  TCut ht_cut(Form("ht > %i", ht_cut_1));
  
  //HT everywhere
  baseline += ht_cut;

  //Define each SR, make yield, plot
  TCut SR1 = nbtags0*met_low*mtmin_low; 
  TCut SR2 = nbtags0*met_high*mtmin_low; 
  TCut SR3 = nbtags1*met_low*mtmin_low; 
  TCut SR4 = nbtags1*met_high*mtmin_low; 
  TCut SR5 = nbtags2*met_low*mtmin_low; 
  TCut SR6 = nbtags2*met_high*mtmin_low; 
  TCut SR7 = nbtags3p*mtmin_low;
  TCut SR8 = mtmin_high;
  vector <TCut> cuts;
  cuts.push_back(SR1); 
  cuts.push_back(SR2); 
  cuts.push_back(SR3); 
  cuts.push_back(SR4); 
  cuts.push_back(SR5); 
  cuts.push_back(SR6); 
  cuts.push_back(SR7); 
  cuts.push_back(SR8); 
  for (unsigned int i = 0; i < 8; i++){
    TH1F *hist = new TH1F("hist", "hist", 1, 0, 1);
    chain->Draw("0.5>>hist", baseline*weight*cuts[i]);
    result.push_back(hist->Integral());  
    delete hist;
  }
  return result;
}

int yields(){

  //Make chains
  TChain* ttbar       = new TChain("t");
  TChain* ttw         = new TChain("t");
  TChain* ttz         = new TChain("t");
  TChain* wz          = new TChain("t");
  TChain* t1tttt_1200 = new TChain("t");
  TChain* t1tttt_1500 = new TChain("t");
  TChain* t5qqww_1200 = new TChain("t");
  TChain* t5qqww_deg  = new TChain("t");
 
  //Fill chains
  ttbar      ->Add("../v1.08/TTBAR_multiIso.root"                                 );
  ttw        ->Add("../v1.08/TTW_multiIso.root"                                   );
  ttz        ->Add("../v1.08/TTZ_multiIso.root"                                   );
  wz         ->Add("../v1.08/WZ_multiIso.root"                                    );
  t1tttt_1200->Add("../v1.08/T1TTTT_1200_multiIso.root"                           );
  t1tttt_1500->Add("../v1.08/T1TTTT_1500_multiIso.root"                           );
  t5qqww_1200->Add("../v1.08/private/t5qqqqWW_1200_1000_800_baby_multiIso.root"   );
  t5qqww_deg ->Add("../v1.08/private/t5qqqqWW_deg_1000_315_300_baby_multiIso.root");

  //Get results
  ttbar_high        = fillA(ttbar      ); 
  ttw_high          = fillA(ttw        ); 
  ttz_high          = fillA(ttz        ); 
  wz_high           = fillA(wz         ); 
  t1tttt_1200_high  = fillA(t1tttt_1200); 
  t1tttt_1500_high  = fillA(t1tttt_1500); 
  t5qqww_1200_high  = fillA(t5qqww_1200); 
  t5qqww_deg_high   = fillA(t5qqww_deg ); 
  ttbar_hl          = fillB(ttbar      ); 
  ttw_hl            = fillB(ttw        ); 
  ttz_hl            = fillB(ttz        ); 
  wz_hl             = fillB(wz         ); 
  t1tttt_1200_hl    = fillB(t1tttt_1200); 
  t1tttt_1500_hl    = fillB(t1tttt_1500); 
  t5qqww_1200_hl    = fillB(t5qqww_1200); 
  t5qqww_deg_hl     = fillB(t5qqww_deg ); 

  //Print results
  //printResults(47, 0);
  //printResults(32, 1);

  //Now make histogram of results
  TH1F *high_ttbar       = new TH1F("high_ttbar", "high_ttbar", 32, 1, 33);
  TH1F *high_ttw         = new TH1F("high_ttw", "high_ttw", 32, 1, 33);
  TH1F *high_ttz         = new TH1F("high_ttz", "high_ttz", 32, 1, 33);
  TH1F *high_wz          = new TH1F("high_wz", "high_wz", 32, 1, 33);
  TH1F *high_t1tttt_1200 = new TH1F("high_t1tttt_1200", "high_t1tttt_1200", 32, 1, 33);
  TH1F *high_t1tttt_1500 = new TH1F("high_t1tttt_1500", "high_t1tttt_1500", 32, 1, 33);
  TH1F *high_t5qqww_1200 = new TH1F("high_t5qqww_1200", "high_t5qqww_1200", 32, 1, 33);
  TH1F *high_t5qqww_deg = new TH1F("high_t5qqww_deg", "high_t5qqww_deg", 32, 1, 33);
  TH1F *hl_ttbar       = new TH1F("hl_ttbar", "hl_ttbar", 26, 1, 27);
  TH1F *hl_ttw         = new TH1F("hl_ttw", "hl_ttw", 26, 1, 27);
  TH1F *hl_ttz         = new TH1F("hl_ttz", "hl_ttz", 26, 1, 27);
  TH1F *hl_wz          = new TH1F("hl_wz", "hl_wz", 26, 1, 27);
  TH1F *hl_t1tttt_1200 = new TH1F("hl_t1tttt_1200", "hl_t1tttt_1200", 26, 1, 27);
  TH1F *hl_t1tttt_1500 = new TH1F("hl_t1tttt_1500", "hl_t1tttt_1500", 26, 1, 27);
  TH1F *hl_t5qqww_1200 = new TH1F("hl_t5qqww_1200", "hl_t5qqww_1200", 26, 1, 27);
  TH1F *hl_t5qqww_deg = new TH1F("hl_t5qqww_deg", "hl_t5qqww_deg", 26, 1, 27);

  //Fill the histograms
  for (unsigned int i = 0; i < 32; i++){
    high_ttbar->SetBinContent(i+1, ttbar_high[i]); 
    high_ttw->SetBinContent(i+1, ttw_high[i]); 
    high_ttz->SetBinContent(i+1, ttz_high[i]); 
    high_wz->SetBinContent(i+1, wz_high[i]); 
    high_t1tttt_1200->SetBinContent(i+1, t1tttt_1200_high[i]); 
    high_t1tttt_1500->SetBinContent(i+1, t1tttt_1500_high[i]); 
    high_t5qqww_1200->SetBinContent(i+1, t5qqww_1200_high[i]); 
    high_t5qqww_deg->SetBinContent(i+1, t5qqww_deg_high[i]); 
  }

  for (unsigned int i = 0; i < 26; i++){
    hl_ttbar->SetBinContent(i+1, ttbar_hl[i]); 
    hl_ttw->SetBinContent(i+1, ttw_hl[i]); 
    hl_ttz->SetBinContent(i+1, ttz_hl[i]); 
    hl_wz->SetBinContent(i+1, wz_hl[i]); 
    hl_t1tttt_1200->SetBinContent(i+1, t1tttt_1200_hl[i]); 
    hl_t1tttt_1500->SetBinContent(i+1, t1tttt_1500_hl[i]); 
    hl_t5qqww_1200->SetBinContent(i+1, t5qqww_1200_hl[i]); 
    hl_t5qqww_deg->SetBinContent(i+1, t5qqww_deg_hl[i]); 
  }

  vector <TH1F*> background_high;
  background_high.push_back(high_ttbar);
  background_high.push_back(high_ttw);
  background_high.push_back(high_ttz);
  background_high.push_back(high_wz);

  vector <TH1F*> background_hl;
  background_hl.push_back(hl_ttbar);
  background_hl.push_back(hl_ttw);
  background_hl.push_back(hl_ttz);
  background_hl.push_back(hl_wz);

  vector <TH1F*> signal_high;
  signal_high.push_back(high_t1tttt_1200);
  signal_high.push_back(high_t1tttt_1500);
  signal_high.push_back(high_t5qqww_1200);
  signal_high.push_back(high_t5qqww_deg);

  vector <TH1F*> signal_hl;
  signal_hl.push_back(hl_t1tttt_1200);
  signal_hl.push_back(hl_t1tttt_1500);
  signal_hl.push_back(hl_t5qqww_1200);
  signal_hl.push_back(hl_t5qqww_deg);

  titles.push_back("ttbar");
  titles.push_back("ttw");
  titles.push_back("ttz");
  titles.push_back("wz");

  signal_titles.push_back("t1tttt (1.2, 0.8)");
  signal_titles.push_back("t1tttt (1.5, 0.1)");
  signal_titles.push_back("t5qqww (1.2, 1.0, 0.8)");
  signal_titles.push_back("t5qqww (1.0, 0.315, 0.3) test test test test");

  TH1F* null = new TH1F("","",1,0,1);
  vector <TH1F*> null_vector;
  vector <char*> null_strings;

  //Colors.push_back(kGreen-3);
  //Colors.push_back(kCyan);
  //Colors.push_back(kOrange-4);
  //Colors.push_back(kMagenta-8);
  //Colors2.push_back(kRed);
  //Colors2.push_back(kBlue);
  //Colors2.push_back(kGreen+3);
  //Colors2.push_back(kMagenta+1);

  dataMCplotMaker(null, background_high, titles, "H-H", "", "--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325"); 
  dataMCplotMaker(null, background_hl  , titles, "H-L", "multiIso", "--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName   hl_yields --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325"); 
  dataMCplotMaker(null, signal_high, signal_titles, "H-H", "", "--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields_s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --legendRight -0.12 --noXaxisUnit  --legendTextSize 0.0325 --noStack --nDivisions 210 "); 
  dataMCplotMaker(null, signal_hl  , signal_titles, "H-L", "", "--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName   hl_yields_s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --legendRight -0.12 --noXaxisUnit  --legendTextSize 0.03 --noStack  --nDivisions 210"); 

  //Get results
  ttbar_low         = fillC(ttbar      ); 
  ttw_low           = fillC(ttw        ); 
  ttz_low           = fillC(ttz        ); 
  wz_low            = fillC(wz         ); 
  t1tttt_1200_low   = fillC(t1tttt_1200); 
  t1tttt_1500_low   = fillC(t1tttt_1500); 
  t5qqww_1200_low   = fillC(t5qqww_1200); 
  t5qqww_deg_low    = fillC(t5qqww_deg ); 

  //Print results
  //printResults(8, 0);

  //Now make histogram of results
  TH1F *low_ttbar       = new TH1F("low_ttbar", "low_ttbar", 8, 1, 9);
  TH1F *low_ttw         = new TH1F("low_ttw", "low_ttw", 8, 1, 9);
  TH1F *low_ttz         = new TH1F("low_ttz", "low_ttz", 8, 1, 9);
  TH1F *low_wz          = new TH1F("low_wz", "low_wz", 8, 1, 9);
  TH1F *low_t1tttt_1200 = new TH1F("low_t1tttt_1200", "low_t1tttt_1200", 8, 1, 9);
  TH1F *low_t1tttt_1500 = new TH1F("low_t1tttt_1500", "low_t1tttt_1500", 8, 1, 9);
  TH1F *low_t5qqww_1200 = new TH1F("low_t5qqww_1200", "low_t5qqww_1200", 8, 1, 9);
  TH1F *low_t5qqww_deg = new TH1F("low_t5qqww_deg", "low_t5qqww_deg", 8, 1, 9);

  //Fill the histograms
  for (unsigned int i = 0; i < 8; i++){
    low_ttbar->SetBinContent(i+1, ttbar_low[i]); 
    low_ttw->SetBinContent(i+1, ttw_low[i]); 
    low_ttz->SetBinContent(i+1, ttz_low[i]); 
    low_wz->SetBinContent(i+1, wz_low[i]); 
    low_t1tttt_1200->SetBinContent(i+1, t1tttt_1200_low[i]); 
    low_t1tttt_1500->SetBinContent(i+1, t1tttt_1500_low[i]); 
    low_t5qqww_1200->SetBinContent(i+1, t5qqww_1200_low[i]); 
    low_t5qqww_deg->SetBinContent(i+1, t5qqww_deg_low[i]); 
  }

  vector <TH1F*> background_low;
  background_low.push_back(low_ttbar);
  background_low.push_back(low_ttw);
  background_low.push_back(low_ttz);
  background_low.push_back(low_wz);

  vector <TH1F*> signal_low;
  signal_low.push_back(low_t1tttt_1200);
  signal_low.push_back(low_t1tttt_1500);
  signal_low.push_back(low_t5qqww_1200);
  signal_low.push_back(low_t5qqww_deg);

  dataMCplotMaker(null, background_low, titles, "L-L", "", "--vLine 3 --vLine 5 --vLine 7 --vLine 8 --nDivisions 107  --outputName low_yields --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --noXaxisUnit  --legendTextSize 0.0325 --isLinear --legendRight 0.12"); 
  dataMCplotMaker(null, signal_low, signal_titles, "L-L", "", "--vLine 3 --vLine 5 --vLine 7 --vLine 8 --nDivisions 107 --outputName low_yields_s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --noXaxisUnit  --legendTextSize 0.0325 --isLinear --legendRight -0.18 --noStack");

  return 0;

}
