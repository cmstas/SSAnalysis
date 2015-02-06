#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "TCut.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include <iostream>

using namespace std;

enum sample_t { ALL, TTW, TTBAR, TTZ, TTBAR_8TEV, TEST };
enum selection_t { SS, OS };
enum weight_t { WEIGHTED, UNWEIGHTED };
enum lep_t { EL, MU, EM, ANY };
enum region_t { NEW, OLD };
enum mtmin_t { NONE, MT100, MT125, MT150 };

//switches
char* path = "v1.03";
int nRegions = 28;
bool scan = 0; 
int scanRegion = 0;
bool doMC = 0;
mtmin_t mt_min = MT150;
sample_t sample = ALL;
selection_t selection = SS;
weight_t weighted = WEIGHTED;
region_t regions = NEW;
//char* testfile = "test/ttbar_reverted_both.root";
char* testfile = "v1.03/ttbar.root";
//char* testfile = "test/ttbar_reverted_sel.root";
//char* testfile = "test/ttbar_ii_updated.root";

vector<float> doIt(TTree *chain, TCut cuts, lep_t leps = ANY){

  vector <float> temp;

  TCut weight_13("scale1fb*10.0");
  TCut notweighted("1.0");
  TCut weight = (weighted == WEIGHTED) ? weight_13 : notweighted;
  TCut weight8("1.95*sf_dilepTrig_hpt*sf_dilep_eff");
  if (sample == TTBAR_8TEV) weight *= weight8;
  TCut selectionSS("hyp_class == 3");
  TCut selectionOS("hyp_class == 4");
  TCut highLow("((lep1_p4.pt() > 10 && lep2_p4.pt() > 25) || (lep1_p4.pt() > 25 && lep2_p4.pt() > 10))"); 
  TCut high_new("lep1_p4.pt() > 25 && lep2_p4.pt() > 25"); 
  TCut high_old("lep1_p4.pt() > 20 && lep2_p4.pt() > 20"); 
  TCut mc_ss("lep1_mc_id*lep2_mc_id > 0");
  TCut mc_os("lep1_mc_id*lep2_mc_id > 0");
  TCut njets2("njets >= 2");
  TCut high = regions == NEW ? high_new : high_old;
  TCut baseline = high + njets2;
  baseline += selection == OS ? selectionOS+mc_os : selectionSS+mc_ss;
  TCut el("abs(lep1_id) == 11 && abs(lep2_id) == 11");
  TCut mu("abs(lep1_id) == 13 && abs(lep2_id) == 13");
  TCut emu("(abs(lep1_id) == 11 && abs(lep2_id) == 13) || (abs(lep1_id) == 13 && abs(lep2_id) == 11)");
  if (leps == EL) baseline += el;
  if (leps == MU) baseline += mu;
  if (leps == EM) baseline += emu;
  TCut njets_low("njets == 2 || njets == 3");
  TCut njets_high("njets >= 4");
  TCut met_low("met > 50 && met < 120");
  TCut met_high("met > 120");
  TCut ht_low("ht > 200 && ht < 400");
  TCut ht_high("ht > 400");
  TCut nbtagsi("nbtags >= 0");
  TCut nbtags0("nbtags == 0");
  TCut nbtags1("nbtags == 1");
  TCut nbtags2("nbtags == 2");
  TCut nbtags3p("nbtags >= 3");
  TCut htmet("ht > 500 ? 1 : met > 30");
  TCut none("1.0");
  TCut mtmin100("mt > mt_l2 ? mt_l2 > 100 : mt > 100");
  TCut mtmin125("mt > mt_l2 ? mt_l2 > 125 : mt > 125");
  TCut mtmin150("mt > mt_l2 ? mt_l2 > 150 : mt > 150");
  if (mt_min == MT100) baseline += mtmin100;
  if (mt_min == MT125) baseline += mtmin125;
  if (mt_min == MT150) baseline += mtmin150;

  for (int i = 0; i < nRegions+1; i++){
    if (i%10 == 9){
      temp.push_back(-1);
      continue;
    }
    TH1F* SR = new TH1F("SR", "SR", 1, 0, 1);
    TCut cut;
    if (i%10 == 0) cut = htmet;
    if (i%10 == 1) cut = met_low*ht_low*njets_low; 
    if (i%10 == 2) cut = met_low*ht_high*njets_low; 
    if (i%10 == 3) cut = met_low*ht_low*njets_high; 
    if (i%10 == 4) cut = met_low*ht_high*njets_high; 
    if (i%10 == 5) cut = met_high*ht_low*njets_low; 
    if (i%10 == 6) cut = met_high*ht_high*njets_low; 
    if (i%10 == 7) cut = met_high*ht_low*njets_high; 
    if (i%10 == 8) cut = met_high*ht_high*njets_high; 
    if (i-30 >= 0) cut *= nbtags3p;
    else if (i-20 >= 0) cut *= nbtags2;
    else if (i-10 >= 0) cut *= nbtags1;
    else if (i > 0) cut *= nbtags0;
    else if (i == 0) cut *= nbtagsi;
    cut *= cuts;
    chain->Draw("0.5>>SR", weight*baseline*cut); 
    temp.push_back(SR->Integral());
    if (scan){
      if (i == scanRegion) cout << weight*baseline*cut << endl;
      if (i == scanRegion) chain->SetScanField(0);
      if (i == scanRegion) chain->Scan("lumi:event:njets:nbtags:ht:met",baseline*cut);
    }
    delete SR;
  }

  return temp;

}

int yields(){

  cout << "Printing high-pT yields for SS Analysis!" << endl;
  const char* ssoros = selection == SS ? "SS" : "OS";
  cout << "Running in " << ssoros << " mode" << endl;

  TCut none("1.0");

  vector <float> ttw_yields;
  vector <float> ttz_yields;
  vector <float> ttbar_yields;
  vector <float> ttbar_8_yields;
  vector <float> test_yields;

  if (sample == TEST){
    TFile *test_file = new TFile(Form("../%s", testfile));
    TTree *test_tree = (TTree*)test_file->Get("t");
    test_yields = doIt(test_tree, none);
    cout << "SR0: " << test_yields[0] << endl;
    cout << "SR10: " << test_yields[10] << endl;
    cout << "SR20: " << test_yields[20] << endl;
    cout << "SR30: " << test_yields[30] << endl;
  }

  for (unsigned int i = 0; i < test_yields.size(); i++){
    cout << i << " " << test_yields[i] << endl;
  }

  if (sample == ALL || sample == TTW){
    TFile *ttw_file = new TFile(Form("../%s/ttw.root", path));
    TTree *ttw_tree = (TTree*)ttw_file->Get("t");
    ttw_yields = doIt(ttw_tree, none);
    if (sample == TTW) cout << ttw_yields[0] << endl;
  }

  if (sample == ALL || sample == TTZ){
    TFile *ttz_file = new TFile(Form("../%s/ttz.root", path));
    TTree *ttz_tree = (TTree*)ttz_file->Get("t");
    ttz_yields = doIt(ttz_tree, none);
    if (sample == TTZ) cout << ttz_yields[0] << endl;
  }
  
  if (sample == ALL || sample == TTBAR){
    TFile *ttbar_file = new TFile(Form("../%s/ttbar.root", path));
    TTree *ttbar_tree = (TTree*)ttbar_file->Get("t");
    ttbar_yields = doIt(ttbar_tree, none);
    if (sample == TTBAR) cout << "ttbar SR0 yield: " << ttbar_yields[0] << endl;
  }

  if (sample == TTBAR_8TEV){
    TFile *ttbar_8_file = new TFile(Form("../%s/ttbar_8.root", path));
    TTree *ttbar_8_tree = (TTree*)ttbar_8_file->Get("t");
    ttbar_8_yields = doIt(ttbar_8_tree, none);
    if (sample == TTBAR_8TEV) cout << ttbar_8_yields[0] << endl;
  }

  if (sample == ALL){
    TFile *wz_file = new TFile(Form("../%s/wz.root", path));
    TTree *wz_tree = (TTree*)wz_file->Get("t");
    vector <float> wz_yields = doIt(wz_tree, none);

    TFile *t1tttt_1_file = new TFile(Form("../%s/t1tttt_1200_800.root", path));
    TTree *t1tttt_1_tree = (TTree*)t1tttt_1_file->Get("t");
    vector <float> t1tttt_1_yields = doIt(t1tttt_1_tree, none);

    TFile *t1tttt_2_file = new TFile(Form("../%s/t1tttt_1500_100.root", path));
    TTree *t1tttt_2_tree = (TTree*)t1tttt_2_file->Get("t");
    vector <float> t1tttt_2_yields = doIt(t1tttt_2_tree, none);

    cout << "ttz: " << ttz_yields[0] << endl;
    cout << "ttw: " << ttw_yields[0] << endl;
    cout << "tt: " << ttbar_yields[0] << endl;
    cout << "wz: " << wz_yields[0] << endl;
    cout << "t1tttt_1: " << t1tttt_1_yields[0] << endl;
    cout << "t1tttt_2: " << t1tttt_2_yields[0] << endl;
    cout << " " << endl;
    cout << "ttz: " << ttz_yields[28] << endl;
    cout << "ttw: " << ttw_yields[28] << endl;
    cout << "tt: " << ttbar_yields[28] << endl;
    cout << "wz: " << wz_yields[28] << endl;
    cout << "t1tttt_1: " << t1tttt_1_yields[28] << endl;
    cout << "t1tttt_2: " << t1tttt_2_yields[28] << endl;

    //cout << "\\documentclass{article}" << endl;
    //cout << "\\usepackage[landscape]{geometry}" << endl;
    //cout << "\\usepackage{fullpage}" << endl;
    //cout << "\\begin{document}" << endl;
    //cout << "\\begin{table}" << endl;
    //cout << "\\centering" << endl;
    //cout << "\\begin{tabular}{|c|ccccc|c|ccc|c|c|}" << endl;
    //cout << "\\hline" << endl;
    //cout << " & ttbar &  ttW & ttZ & WZ & WH & bkgd & s1 & s2 & s3 & s1/bkgd \\\\ " << endl;
    //cout << "\\hline" << endl;
    //for (int i = 0; i < nRegions+1; i++){
      //float backgrounds = ttbar_yields.at(2*i) + ttw_yields.at(2*i) + ttz_yields.at(2*i) + wh_yields.at(2*i) + wz_yields.at(2*i); 
      //float backgrounds_error = ttbar_yields.at(2*i+1) + ttw_yields.at(2*i+1) + ttz_yields.at(2*i+1) + wh_yields.at(2*i+1) + wz_yields.at(2*i+1); 
      //if (i%10 == 9) continue;
      //cout << Form("%i & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f & %.2f $\\pm$ %.2f", i, ttbar_yields.at(2*i), ttw_yields.at(2*i), ttz_yields.at(2*i), wz_yields.at(2*i), wh_yields.at(2*i), backgrounds, t1tttt_1_yields.at(2*i), t1tttt_2_yields.at(2*i), t5ww_1_yields.at(2*i), t1tttt_1_yields.at(2*i)/backgrounds, (t1tttt_1_yields.at(2*i)/backgrounds)*sqrt( pow((t1tttt_1_yields.at(2*i+1)/t1tttt_1_yields.at(2*i)), 2) + pow((backgrounds_error/backgrounds), 2)  )) << " \\\\" << endl;
    //  cout << "\\hline" << endl;
    //}
    //cout << "\\end{tabular}" << endl;
    //cout << "\\caption{s1 t1tttt is (1200, 800), s2 is t1tttt (1500, 100), s3 is t5ww (1200, 1000, 800)}" << endl;
    //cout << "\\end{table}" << endl;
    //cout << "\\end{document}" << endl;
  }
 
  return 0;

}


//Other files
//TFile *wh_file = new TFile(Form("../%s/wh.root", path));
//TTree *wh_tree = (TTree*)wh_file->Get("t");
//vector <float> wh_yields = doIt(wh_tree, none);
//
//TFile *ww_file = new TFile(Form("../%s/ww.root", path));
//TTree *ww_tree = (TTree*)ww_file->Get("t");
//vector <float> ww_yields = doIt(ww_tree, none);
//
//TFile *t5ww_1_file = new TFile(Form("../%s/t5ww_1200_1000_800.root", path));
//TTree *t5ww_1_tree = (TTree*)t5ww_1_file->Get("t");
//vector <float> t5ww_1_yields = doIt(t5ww_1_tree, none);
//
//TFile *t5ww_2_file = new TFile(Form("../%s/t5ww_1500_800_100.root", path));
//TTree *t5ww_2_tree = (TTree*)t5ww_2_file->Get("t");
//vector <float> t5ww_2_yields = doIt(t5ww_2_tree, none);
//
