#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

#include "TCut.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

//Enums
enum sample_t { ALL, TTW, TTBAR, TTZ, WZ, TTBAR_8TEV, T1TTTT_1500_100, T1TTTT_1200_800, TEST };
enum selection_t { SS, OS };
enum weight_t { WEIGHTED, UNWEIGHTED };
enum lep_t { EL, MU, EM, ANY };
enum region_t { NEW, OLD };
enum mtmin_t { NONE, MT100, MT125, MT150 };
enum hyp_class_t { INC1, INC2, NORMAL };
enum pt_t { HIHI, HILO, LOLO, ALL3 };

//Switches
char* path = "v1.04";                  //where the ntuples are
char* output_dir = "mydir";            //dir where plots should go (in parent directory, will create if doesn't exist)
int nRegions = 1;                      //which regions to include (can be 1 for single region, > 10 will turn on b-tagging)
int firstSR = 0;                       //first SR you want to consider (usually 0 or 1)
bool scan = 0;                         //make printout of events
int scanRegion = 0;                    //only needed if scan == TRUE
mtmin_t mt_min = NONE;                 //add mt_min cut
sample_t sample = ALL;                  //Sample as defined above
selection_t selection = SS;            //select SS or OS
weight_t weighted = WEIGHTED;          //weight by cross-section or not
lep_t leps = ANY;                      //segregate by flavor composition (MU, EL, EM, ANY)
char* testfile = "ttbar.root";         //only needed if sample is TEST
hyp_class_t hyp_class_wanted = NORMAL; //INC1 is inclusive SS, INC2 is h-h or h-l SS, NORMAL is h-h SS only
region_t regions = NEW;                //new pT regions (25/10) or old (20/10)
pt_t which_pt = ALL3;                  //which pT regions should be handled?
int met_cut = 120;                     //120 for 8 TeV 
int ht_cut = 400;                      //400 for 8 TeV
int njets_cut = 4;                     //4 for 8 TeV
int btag_pt = 25;                      //40 for 8 TeV; 25 is default for 13 TeV
bool doBtagging = true;                //should SRs have b-tagging applied? (automatically true for nRegions > 10)
bool doTruthMatching = true;           //require truth-level leptons to be matched to correctly-signed (SS or OS) pair
bool makePlots = false;                //should plots for limit making be produced?
bool outputYields = true;              //should yields be printed to the screen?

//Declare output file
TFile *file;

vector<float> doIt(TTree *chain, TCut cuts, pt_t pt_){

  //Declare and create output histogram
  TH1F *hist; 
  if (makePlots){
    if (pt_ == HIHI) hist = new TH1F("hyp_hihi", "hyp_hihi", nRegions, 1, nRegions+1);
    if (pt_ == HILO) hist = new TH1F("hyp_hilow", "hyp_hilow", nRegions, 1, nRegions+1);
    if (pt_ == LOLO) hist = new TH1F("hyp_lowlow", "hyp_lowlow", nRegions, 1, nRegions+1);
  }

  //Create vector with yields to return
  vector <float> temp;

  //-------CUTS-------
  //Void, initialize baseline
  TCut none("1.0");
  TCut baseline = none;
  
  //Weight
  TCut weight_13("scale1fb*10.0");
  TCut notweighted("1.0");
  TCut weight = (weighted == WEIGHTED) ? weight_13 : notweighted;
  TCut weight8("1.95*sf_dilepTrig_hpt*sf_dilep_eff");
  if (sample == TTBAR_8TEV) weight *= weight8;

  //Hyp class (num-num, num-denom, etc.)
  TCut selection1("hyp_class == 1 || hyp_class == 2 || hyp_class == 3");
  TCut selection2("hyp_class == 2 || hyp_class == 3");
  TCut selectionSS("hyp_class == 3");
  TCut selectionOS("hyp_class == 4");
  baseline += (hyp_class_wanted == NORMAL ? (selection == OS ? selectionOS : selectionSS) : none);
  if (hyp_class_wanted == INC1) baseline += selection1; 
  if (hyp_class_wanted == INC2) baseline += selection2; 

  //Lep pT requirements
  TCut highLow("((lep1_p4.pt() > 10 && lep2_p4.pt() > 25) || (lep1_p4.pt() > 25 && lep2_p4.pt() > 10))"); 
  TCut lowLow("((lep1_p4.pt() > 10 && lep2_p4.pt() > 10) && (lep1_p4.pt() < 25 && lep2_p4.pt() < 25))"); 
  TCut high_new("lep1_p4.pt() > 25 && lep2_p4.pt() > 25"); 
  TCut high_old("lep1_p4.pt() > 20 && lep2_p4.pt() > 20"); 
  TCut high = regions == NEW ? high_new : high_old;
  if (pt_ == HIHI) baseline += high;
  if (pt_ == HILO) baseline += highLow;
  if (pt_ == LOLO) baseline += lowLow;
 
  //Truth matching
  TCut mc_ss("lep1_mc_id*lep2_mc_id > 0");
  TCut mc_os("lep1_mc_id*lep2_mc_id < 0");
  if (doTruthMatching) baseline += (selection == OS) ? mc_os : mc_ss;
 
  //nJets
  TCut njets2("njets >= 2");
  baseline += njets2;
  TCut njets_low(Form("njets >= 2 && njets < %i", njets_cut));
  TCut njets_high(Form("njets >= %i", njets_cut));

  //Lepton flavor
  TCut el("abs(lep1_id) == 11 && abs(lep2_id) == 11");
  TCut mu("abs(lep1_id) == 13 && abs(lep2_id) == 13");
  TCut emu("(abs(lep1_id) == 11 && abs(lep2_id) == 13) || (abs(lep1_id) == 13 && abs(lep2_id) == 11)");
  if (leps == EL) baseline += el;
  if (leps == MU) baseline += mu;
  if (leps == EM) baseline += emu;

  //MET, HT
  TCut met_low(Form("met > 50 && met < %i", met_cut));
  TCut met_high(Form("met > %i", met_cut));
  TCut ht_low(Form("ht > 200 && ht < %i", ht_cut));
  TCut ht_high(Form("ht > %i", ht_cut));
  TCut htmet("ht > 500 ? 1 : met > 30");

  //nBtags
  TCut nbtags0_slow(Form("Sum$(jets.pt()>%i && jets_disc>0.814 ? 1 : 0) == 0", btag_pt));
  TCut nbtags1_slow(Form("Sum$(jets.pt()>%i && jets_disc>0.814 ? 1 : 0) == 1", btag_pt));
  TCut nbtags2_slow(Form("Sum$(jets.pt()>%i && jets_disc>0.814 ? 1 : 0) == 2", btag_pt));
  TCut nbtags3p_slow(Form("Sum$(jets.pt()>%i && jets_disc>0.814 ? 1 : 0) >= 3", btag_pt));
  TCut nbtags0_quick("nbtags == 0");
  TCut nbtags1_quick("nbtags == 1");
  TCut nbtags2_quick("nbtags == 2");
  TCut nbtags3p_quick("nbtags >= 3");
  TCut nbtags0 =  btag_pt == 25 ? nbtags0_quick  : nbtags0_slow;
  TCut nbtags1 =  btag_pt == 25 ? nbtags1_quick  : nbtags1_slow;
  TCut nbtags2 =  btag_pt == 25 ? nbtags2_quick  : nbtags2_slow;
  TCut nbtags3p = btag_pt == 25 ? nbtags3p_quick : nbtags3p_slow;

  //MT MIN
  TCut mtmin100("mt > mt_l2 ? mt_l2 > 100 : mt > 100");
  TCut mtmin125("mt > mt_l2 ? mt_l2 > 125 : mt > 125");
  TCut mtmin150("mt > mt_l2 ? mt_l2 > 150 : mt > 150");
  if (mt_min == MT100) baseline += mtmin100;
  if (mt_min == MT125) baseline += mtmin125;
  if (mt_min == MT150) baseline += mtmin150;

  //Define each SR, make yield, plot
  for (int i = firstSR; i < firstSR+nRegions; i++){
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
    if (doBtagging || nRegions > 10){
      if (i-30 >= 0) cut *= nbtags3p;
      else if (i-20 >= 0) cut *= nbtags2;
      else if (i-10 >= 0) cut *= nbtags1;
      else if (i > 0) cut *= nbtags0; 
    }
    cut *= cuts;
    chain->Draw("0.5>>SR", weight*baseline*cut); 
    temp.push_back(SR->Integral());
    if (makePlots) hist->Fill(i, SR->Integral()); 
    if (scan){
      if (i == scanRegion) cout << weight*baseline*cut << endl;
      if (i == scanRegion) chain->SetScanField(0);
      if (i == scanRegion) chain->Scan("lumi:event:njets:nbtags:ht:met",baseline*cut);
    }
    delete SR;
  }

  if (makePlots) file->cd();
  if (makePlots) hist->Write();

  return temp;

}

int yields(){

  if (nRegions < 10 && !doBtagging) cout << "WARNING! Not doing any b-tag requirements!" << endl;
  cout << Form("SR regions based around HT cut of %i, MET cut of %i, njets cut of %i", ht_cut, met_cut, njets_cut) << endl;

  //If making plots, check if directory exists; create it if not; otherwise, ask permission to overwrite it
  if (makePlots){
    struct stat info;
    if (stat(output_dir, &info) != 0){ 
      string input;
      bool keepAsking = true;
      while (keepAsking == true){
        cout << Form("Directory %s already exists.  Really overwrite those files? (yes/no) ", output_dir);
        getline(cin, input); 
        if (input == "no"){ cout << "aborting" << endl; keepAsking = false; return 0; }
        if (input == "yes") keepAsking = false;
      }
    } 
    else system(Form("mkdir ../%s", output_dir)); 
  }

  //Warning if not running on all lepton flavors
  const char* ssoros = selection == SS ? "SS" : "OS";
  cout << "Running in " << ssoros << " mode" << endl;
  if (leps == EL) cout << "Running on dielectrons!" << endl; 
  if (leps == EM) cout << "Running on el-mus!" << endl; 
  if (leps == MU) cout << "Running on dimuons!" << endl; 

  //Declare null TCut
  TCut none("1.0");

  //Declare yield vectors
  vector <float> ttw_yields;
  vector <float> ttz_yields;
  vector <float> wz_yields;
  vector <float> ttbar_yields;
  vector <float> ttbar_8_yields;
  vector <float> test_yields;
  vector <float> t1tttt_1_yields;
  vector <float> t1tttt_2_yields;
  vector <float> t1tttt_3_yields;

  //---------------For each sample, make yields, plots-------------------
  if (sample == TEST){
    TFile *test_file = new TFile(Form("../%s", testfile));
    TTree *test_tree = (TTree*)test_file->Get("t");
    cout << endl << "TEST yields: " << endl;
    if (which_pt == HIHI || which_pt == ALL3){
      cout << "  High High" << endl;
      test_yields = doIt(test_tree, none, HIHI);
      for (unsigned int i = 0; i < test_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << test_yields[i] << endl;
    }
    if (which_pt == HILO || which_pt == ALL3){
      cout << endl << "  High Low" << endl;
      test_yields = doIt(test_tree, none, HILO);
      for (unsigned int i = 0; i < test_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << test_yields[i] << endl;
    }
    if (which_pt == LOLO || which_pt == ALL3){
      cout << endl << "  Low Low" << endl;
      test_yields = doIt(test_tree, none, LOLO);
      for (unsigned int i = 0; i < test_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << test_yields[i] << endl;
    }
  }

  if (sample == ALL || sample == TTW){
    if (makePlots) file = new TFile(Form("../%s/%s", output_dir, "TTWJets_histos.root"), "RECREATE");
    TFile *ttw_file = new TFile(Form("../%s/ttw_baby.root", path));
    TTree *ttw_tree = (TTree*)ttw_file->Get("t");
    cout << endl << "TTW yields: " << endl;
    if (which_pt == HIHI || which_pt == ALL3){
      cout << "  High High" << endl;
      ttw_yields = doIt(ttw_tree, none, HIHI);
      for (unsigned int i = 0; i < ttw_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttw_yields[i] << endl;
    }
    if (which_pt == HILO || which_pt == ALL3){
      cout << endl << "  High Low" << endl;
      ttw_yields = doIt(ttw_tree, none, HILO);
      for (unsigned int i = 0; i < ttw_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttw_yields[i] << endl;
    }
    if (which_pt == LOLO || which_pt == ALL3){
      cout << endl << "  Low Low" << endl;
      ttw_yields = doIt(ttw_tree, none, LOLO);
      for (unsigned int i = 0; i < ttw_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttw_yields[i] << endl;
    }
  }

  if (sample == ALL || sample == TTZ){
    if (makePlots) file = new TFile(Form("../%s/%s", output_dir, "TTZJets_histos.root"), "RECREATE");
    TFile *ttz_file = new TFile(Form("../%s/ttz_baby.root", path));
    TTree *ttz_tree = (TTree*)ttz_file->Get("t");
    cout << endl <<  "TTZ yields: " << endl;
    if (which_pt == HIHI || which_pt == ALL3){
      cout << "  High High" << endl;
      ttz_yields = doIt(ttz_tree, none, HIHI);
      for (unsigned int i = 0; i < ttz_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttz_yields[i] << endl;
    }
    if (which_pt == HILO || which_pt == ALL3){
      cout << endl << "  High Low" << endl;
      ttz_yields = doIt(ttz_tree, none, HILO);
      for (unsigned int i = 0; i < ttz_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttz_yields[i] << endl;
    }
    if (which_pt == LOLO || which_pt == ALL3){
      cout << endl << "  Low Low" << endl;
      ttz_yields = doIt(ttz_tree, none, LOLO);
      for (unsigned int i = 0; i < ttz_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttz_yields[i] << endl;
    }
  }
  
  if (sample == ALL || sample == TTBAR){
    if (makePlots) file = new TFile(Form("../%s/%s", output_dir, "ttbar_histos.root"), "RECREATE");
    TFile *ttbar_file = new TFile(Form("../%s/ttbar_baby.root", path));
    TTree *ttbar_tree = (TTree*)ttbar_file->Get("t");
    cout << endl << "TTBAR yields: " << endl;
    if (which_pt == HIHI || which_pt == ALL3){
      cout << "  High High" << endl;
      ttbar_yields = doIt(ttbar_tree, none, HIHI);
      for (unsigned int i = 0; i < ttbar_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttbar_yields[i] << endl;
    }
    if (which_pt == HILO || which_pt == ALL3){
      cout << endl << "  High Low" << endl;
      ttbar_yields = doIt(ttbar_tree, none, HILO);
      for (unsigned int i = 0; i < ttbar_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttbar_yields[i] << endl;
    }
    if (which_pt == LOLO || which_pt == ALL3){
      cout << endl << "  Low Low" << endl;
      ttbar_yields = doIt(ttbar_tree, none, LOLO);
      for (unsigned int i = 0; i < ttbar_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << ttbar_yields[i] << endl;
    }
  }

  if (sample == TTBAR_8TEV){
    TFile *ttbar_8_file = new TFile("../eightTeV/ttbar_8_baby.root");
    TTree *ttbar_8_tree = (TTree*)ttbar_8_file->Get("t");
    ttbar_8_yields = doIt(ttbar_8_tree, none, HIHI);
  }

  if (sample == T1TTTT_1500_100 || sample == ALL){
    if (makePlots) file = new TFile(Form("../%s/%s", output_dir, "T1TTTT_1500_100_histos.root"), "RECREATE");
    TFile *t1tttt_2_file = new TFile(Form("../%s/t1tttt_1500_100_baby.root", path));
    TTree *t1tttt_2_tree = (TTree*)t1tttt_2_file->Get("t");
    cout << endl << "T1TTTT_1500_100 yields: " << endl;
    if (which_pt == HIHI || which_pt == ALL3){
      cout << "  High High" << endl;
      t1tttt_2_yields = doIt(t1tttt_2_tree, none, HIHI);
      for (unsigned int i = 0; i < t1tttt_2_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << t1tttt_2_yields[i] << endl;
    }
    if (which_pt == HILO || which_pt == ALL3){
      cout << endl << "  High Low" << endl;
      t1tttt_2_yields = doIt(t1tttt_2_tree, none, HILO);
      for (unsigned int i = 0; i < t1tttt_2_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << t1tttt_2_yields[i] << endl;
    }
    if (which_pt == LOLO || which_pt == ALL3){
      cout << endl << "  Low Low" << endl;
      t1tttt_2_yields = doIt(t1tttt_2_tree, none, LOLO);
      for (unsigned int i = 0; i < t1tttt_2_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << t1tttt_2_yields[i] << endl;
    }
  }

  if (sample == T1TTTT_1200_800 || sample == ALL){
    if (makePlots) file = new TFile(Form("../%s/%s", output_dir, "T1TTTT_1200_800_histos.root"), "RECREATE");
    TFile *t1tttt_3_file = new TFile(Form("../%s/t1tttt_1200_800_baby.root", path));
    TTree *t1tttt_3_tree = (TTree*)t1tttt_3_file->Get("t");
    cout << endl << "T1TTTT_1200_800 yields: " << endl;
    if (which_pt == HIHI || which_pt == ALL3){
      cout << "  High High" << endl;
      t1tttt_3_yields = doIt(t1tttt_3_tree, none, HIHI);
      for (unsigned int i = 0; i < t1tttt_3_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << t1tttt_3_yields[i] << endl;
    }
    if (which_pt == HILO || which_pt == ALL3){
      cout << endl << "  High Low" << endl;
      t1tttt_3_yields = doIt(t1tttt_3_tree, none, HILO);
      for (unsigned int i = 0; i < t1tttt_3_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << t1tttt_3_yields[i] << endl;
    }
    if (which_pt == LOLO || which_pt == ALL3){
      cout << endl << "  Low Low" << endl;
      t1tttt_3_yields = doIt(t1tttt_3_tree, none, LOLO);
      for (unsigned int i = 0; i < t1tttt_3_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << t1tttt_3_yields[i] << endl;
    }
  }

  if (sample == WZ || sample == ALL){
    if (makePlots) file = new TFile(Form("../%s/%s", output_dir, "WZJets_histos.root"), "RECREATE");
    TFile *wz_file = new TFile(Form("../%s/wz_baby.root", path));
    TTree *wz_tree = (TTree*)wz_file->Get("t");
    cout << endl << "WZ yields: " << endl;
    if (which_pt == HIHI || which_pt == ALL3){
      cout << "  High High" << endl;
       wz_yields = doIt(wz_tree, none, HIHI);
       for (unsigned int i = 0; i < wz_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << wz_yields[i] << endl;
    }
    if (which_pt == HILO || which_pt == ALL3){
      wz_yields = doIt(wz_tree, none, HILO);
      cout << endl << "  High Low" << endl;
      for (unsigned int i = 0; i < wz_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << wz_yields[i] << endl;
    }
    if (which_pt == LOLO || which_pt == ALL3){
      wz_yields = doIt(wz_tree, none, LOLO);
      cout << endl << "  Low Low" << endl;
      for (unsigned int i = 0; i < wz_yields.size(); i++) cout << "   SR " << firstSR+i << ": " << wz_yields[i] << endl;
    }
  }
 
  if (makePlots) file->Close();

  return 0;

}
