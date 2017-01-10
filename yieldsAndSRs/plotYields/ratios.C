#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
//include "../software/tableMaker/CTable.h"
#include "../../CORE/SSSelections.h"
#include "../../CORE/Tools/dorky/dorky.h"
#include "../../CORE/Tools/utils.h"
#include "../../classFiles/v6.02/SS.h"
#include "../../commonUtils.h"

//Tables on/off
bool makeTables = 0;
bool testClassSF = false;
float theLumi = 12.9;

struct results_t {
    TH1F* hh;
    TH1F* hl;
    TH1F* ll; 
    TH1F* njets;
    TH1F* nbtags;
    TH1F* mtmin;
    TH1F* met;
    TH1F* ht;
    TH1F* lep1pt;
    TH1F* lep2pt;

    TH1F* hh_up;
    TH1F* hl_up;
    TH1F* ll_up; 
    TH1F* njets_up;
    TH1F* nbtags_up;
    TH1F* mtmin_up;
    TH1F* met_up;
    TH1F* ht_up;
    TH1F* lep1pt_up;
    TH1F* lep2pt_up;

    TH1F* hh_dn;
    TH1F* hl_dn;
    TH1F* ll_dn; 
    TH1F* njets_dn;
    TH1F* nbtags_dn;
    TH1F* mtmin_dn;
    TH1F* met_dn;
    TH1F* ht_dn;
    TH1F* lep1pt_dn;
    TH1F* lep2pt_dn;
}_dn; 


results_t run(TChain* chain, string name, TFile* f1, hyp_type_t flavor = UNASSIGNED){

  //Declare hists
  TH1F *HighHighPlot = new TH1F(Form("%s_hh", name.c_str()), Form("%s_hh", name.c_str()), 33, 0.5, 33.5); 
  TH1F *HighLowPlot  = new TH1F(Form("%s_hl", name.c_str()), Form("%s_hl", name.c_str()), 27, 0.5, 27.5); 
  TH1F *LowLowPlot   = new TH1F(Form("%s_ll", name.c_str()), Form("%s_ll", name.c_str()),  8, 0.5,  8.5); 
  TH1F *Njets   = new TH1F(Form("%s_njets", name.c_str()), Form("%s_njets", name.c_str()),  8, 0,  8); 
  TH1F *Nbtags   = new TH1F(Form("%s_nbtags", name.c_str()), Form("%s_nbtags", name.c_str()),  8, 0,  8); 
  TH1F *Mtmin   = new TH1F(Form("%s_mtmin", name.c_str()), Form("%s_mtmin", name.c_str()),  20, 0,  200); 
  TH1F *MET   = new TH1F(Form("%s_met", name.c_str()), Form("%s_met", name.c_str()),  15, 0,  600); 
  TH1F *HT   = new TH1F(Form("%s_ht", name.c_str()), Form("%s_ht", name.c_str()),  18, 0,  1800); 
  TH1F *Lep1pt   = new TH1F(Form("%s_lep1pt", name.c_str()), Form("%s_lep1pt", name.c_str()),  20, 0,  300); 
  TH1F *Lep2pt   = new TH1F(Form("%s_lep2pt", name.c_str()), Form("%s_lep2pt", name.c_str()),  20, 0,  150); 

  string name_up = name+"_up";
  TH1F *HighHighPlot_up = new TH1F(Form("%s_hh", name_up.c_str()), Form("%s_hh", name_up.c_str()), 33, 0.5, 33.5); 
  TH1F *HighLowPlot_up  = new TH1F(Form("%s_hl", name_up.c_str()), Form("%s_hl", name_up.c_str()), 27, 0.5, 27.5); 
  TH1F *LowLowPlot_up   = new TH1F(Form("%s_ll", name_up.c_str()), Form("%s_ll", name_up.c_str()),  8, 0.5,  8.5); 
  TH1F *Njets_up   = new TH1F(Form("%s_njets", name_up.c_str()), Form("%s_njets", name_up.c_str()),  8, 0,  8); 
  TH1F *Nbtags_up   = new TH1F(Form("%s_nbtags", name_up.c_str()), Form("%s_nbtags", name_up.c_str()),  8, 0,  8); 
  TH1F *Mtmin_up   = new TH1F(Form("%s_mtmin", name_up.c_str()), Form("%s_mtmin", name_up.c_str()),  20, 0,  200); 
  TH1F *MET_up   = new TH1F(Form("%s_met", name_up.c_str()), Form("%s_met", name_up.c_str()),  15, 0,  600); 
  TH1F *HT_up   = new TH1F(Form("%s_ht", name_up.c_str()), Form("%s_ht", name_up.c_str()),  18, 0,  1800); 
  TH1F *Lep1pt_up   = new TH1F(Form("%s_lep1pt", name_up.c_str()), Form("%s_lep1pt", name_up.c_str()),  20, 0,  300); 
  TH1F *Lep2pt_up   = new TH1F(Form("%s_lep2pt", name_up.c_str()), Form("%s_lep2pt", name_up.c_str()),  20, 0,  150); 

  string name_dn = name+"_dn";
  TH1F *HighHighPlot_dn = new TH1F(Form("%s_hh", name_dn.c_str()), Form("%s_hh", name_dn.c_str()), 33, 0.5, 33.5); 
  TH1F *HighLowPlot_dn  = new TH1F(Form("%s_hl", name_dn.c_str()), Form("%s_hl", name_dn.c_str()), 27, 0.5, 27.5); 
  TH1F *LowLowPlot_dn   = new TH1F(Form("%s_ll", name_dn.c_str()), Form("%s_ll", name_dn.c_str()),  8, 0.5,  8.5); 
  TH1F *Njets_dn   = new TH1F(Form("%s_njets", name_dn.c_str()), Form("%s_njets", name_dn.c_str()),  8, 0,  8); 
  TH1F *Nbtags_dn   = new TH1F(Form("%s_nbtags", name_dn.c_str()), Form("%s_nbtags", name_dn.c_str()),  8, 0,  8); 
  TH1F *Mtmin_dn   = new TH1F(Form("%s_mtmin", name_dn.c_str()), Form("%s_mtmin", name_dn.c_str()),  20, 0,  200); 
  TH1F *MET_dn   = new TH1F(Form("%s_met", name_dn.c_str()), Form("%s_met", name_dn.c_str()),  15, 0,  600); 
  TH1F *HT_dn   = new TH1F(Form("%s_ht", name_dn.c_str()), Form("%s_ht", name_dn.c_str()),  18, 0,  1800); 
  TH1F *Lep1pt_dn   = new TH1F(Form("%s_lep1pt", name_dn.c_str()), Form("%s_lep1pt", name_dn.c_str()),  20, 0,  300); 
  TH1F *Lep2pt_dn   = new TH1F(Form("%s_lep2pt", name_dn.c_str()), Form("%s_lep2pt", name_dn.c_str()),  20, 0,  150); 

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  bool isData = (name=="data");


  //Number duplicates rejected
  int reject = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TString filename = currentFile->GetTitle();
    TFile *file = new TFile(filename);
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    std::cout << " filename: " << filename << std::endl;

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      samesign.GetEntry(event);
      nEventsTotal++;
    
      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //Reject duplicates
      if (isData == true){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)){ reject++; continue; }
      }

      //Blinding
      if (!isUnblindRun(ss::run())) continue; 

      //Weight
      float weight = isData ? 1 : ss::scale1fb()*getLumiUnblind();
      float weight_up = 1.0; 
      float weight_dn = 1.0; 
      if (ss::genweights().size() >= 9) weight_up *= weight*ss::genweights().at(4)/ss::genweights().at(0);
      if (ss::genweights().size() >= 9) weight_dn *= weight*ss::genweights().at(8)/ss::genweights().at(0);

      // float weight = isData ? 1 : ss::scale1fb()*theLumi;

      //Reject non-SS
      if (ss::hyp_class() != 3 && testClassSF==false) continue;
      if (ss::hyp_class() != 2 && testClassSF==true) continue;

      //Flavor selection
      if (flavor == MM && ss::hyp_type() != 3) continue;
      if (flavor == EM && ss::hyp_type() != 1 && ss::hyp_type() != 2) continue;
      if (flavor == EE && ss::hyp_type() != 0) continue;

      float lep1_pt = ss::lep1_coneCorrPt();
      float lep2_pt = ss::lep2_coneCorrPt();
      float mtl1 = MT(lep1_pt, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
      float mtl2 = MT(lep2_pt, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
      float mtmin = mtl1 > mtl2 ? mtl2 : mtl1;

      if (ss::ht()<300.) {
        if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA(), ss::lep1_p4().pt())) continue;
        if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA(), ss::lep2_p4().pt())) continue;
      } 
      //drop electrons below 15 GeV
      if (abs(ss::lep1_id())==11 && lep1_pt<15) continue;
      if (abs(ss::lep2_id())==11 && lep2_pt<15) continue;	


      //Require baseline selections
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, true);
      if (BR < 0) continue;
   
      //Calculate mtmin

       
      //Figure out region, fill plot
      anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);  
      int SR = signalRegion2016(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);
      if (SR > 0 && categ == HighHigh) HighHighPlot->Fill(SR, weight);
      if (SR > 0 && categ == HighLow)  HighLowPlot ->Fill(SR, weight);
      if (SR > 0 && categ == LowLow)   LowLowPlot  ->Fill(SR, weight);
      if (SR > 0 && categ == HighHigh) HighHighPlot_up->Fill(SR, weight_up);
      if (SR > 0 && categ == HighLow)  HighLowPlot_up ->Fill(SR, weight_up);
      if (SR > 0 && categ == LowLow)   LowLowPlot_up  ->Fill(SR, weight_up);
      if (SR > 0 && categ == HighHigh) HighHighPlot_dn->Fill(SR, weight_dn);
      if (SR > 0 && categ == HighLow)  HighLowPlot_dn ->Fill(SR, weight_dn);
      if (SR > 0 && categ == LowLow)   LowLowPlot_dn  ->Fill(SR, weight_dn);

      if (SR > 0) {
          Njets->Fill(ss::njets(),weight);
          Nbtags->Fill(ss::nbtags(),weight);
          Mtmin->Fill(mtmin,weight);
          MET->Fill(ss::met(),weight);
          HT->Fill(ss::ht(),weight);
          Lep1pt->Fill(lep1_pt,weight);
          Lep2pt->Fill(lep2_pt,weight);

          Njets_up->Fill(ss::njets(),weight_up);
          Nbtags_up->Fill(ss::nbtags(),weight_up);
          Mtmin_up->Fill(mtmin,weight_up);
          MET_up->Fill(ss::met(),weight_up);
          HT_up->Fill(ss::ht(),weight_up);
          Lep1pt_up->Fill(lep1_pt,weight_up);
          Lep2pt_up->Fill(lep2_pt,weight_up);

          Njets_dn->Fill(ss::njets(),weight_dn);
          Nbtags_dn->Fill(ss::nbtags(),weight_dn);
          Mtmin_dn->Fill(mtmin,weight_dn);
          MET_dn->Fill(ss::met(),weight_dn);
          HT_dn->Fill(ss::ht(),weight_dn);
          Lep1pt_dn->Fill(lep1_pt,weight_dn);
          Lep2pt_dn->Fill(lep2_pt,weight_dn);

      }

    }//event loop
  }//file loop

  if (isData) cout << "number of duplicates removed: " << reject << endl;
 
  //Return hists
  results_t result; 

  result.hh = HighHighPlot;
  result.hl = HighLowPlot;
  result.ll = LowLowPlot;
  result.njets = Njets;
  result.nbtags = Nbtags;
  result.mtmin = Mtmin;
  result.met = MET;
  result.ht = HT;
  result.lep1pt = Lep1pt;
  result.lep2pt = Lep2pt;

  result.hh_up = HighHighPlot_up;
  result.hl_up = HighLowPlot_up;
  result.ll_up = LowLowPlot_up;
  result.njets_up = Njets_up;
  result.nbtags_up = Nbtags_up;
  result.mtmin_up = Mtmin_up;
  result.met_up = MET_up;
  result.ht_up = HT_up;
  result.lep1pt_up = Lep1pt_up;
  result.lep2pt_up = Lep2pt_up;

  result.hh_dn = HighHighPlot_dn;
  result.hl_dn = HighLowPlot_dn;
  result.ll_dn = LowLowPlot_dn;
  result.njets_dn = Njets_dn;
  result.nbtags_dn = Nbtags_dn;
  result.mtmin_dn = Mtmin_dn;
  result.met_dn = MET_dn;
  result.ht_dn = HT_dn;
  result.lep1pt_dn = Lep1pt_dn;
  result.lep2pt_dn = Lep2pt_dn;

  f1->cd();

  result.hh->Write();
  result.hl->Write();
  result.ll->Write();
  result.njets->Write();
  result.nbtags->Write();
  result.mtmin->Write();
  result.met->Write();
  result.ht->Write();
  result.lep1pt->Write();
  result.lep2pt->Write();
  result.hh_up->Write();
  result.hl_up->Write();
  result.ll_up->Write();
  result.njets_up->Write();
  result.nbtags_up->Write();
  result.mtmin_up->Write();
  result.met_up->Write();
  result.ht_up->Write();
  result.lep1pt_up->Write();
  result.lep2pt_up->Write();
  result.hh_dn->Write();
  result.hl_dn->Write();
  result.ll_dn->Write();
  result.njets_dn->Write();
  result.nbtags_dn->Write();
  result.mtmin_dn->Write();
  result.met_dn->Write();
  result.ht_dn->Write();
  result.lep1pt_dn->Write();
  result.lep2pt_dn->Write();

  return result;

}

void ratios(bool data, TString opts=""){

    

  //Make chains
  // TChain* data        = new TChain("t");

  TChain* ch1          = new TChain("t");
  TChain* ch2          = new TChain("t");
  TChain* ch3          = new TChain("t");

  bool doTTZ = opts.Contains("doTTZ");
  // Compare new NLO with old NLO unless bool set, then compare new NLO with high stats LO
  bool doCompareNLOwithLO = opts.Contains("doCompareNLOwithLO");

  // ch1 is NLO
  // ch2 is bugged LO
  // ch3 is fixed LO

  TString tag = getTag();
  if(doTTZ) {
      ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v8.07/TTZnlo.root");
      ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.07/TTZ.root");
      ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.07/TTZLOW.root");
      ch3->Add("/nfs-7/userdata/ss2015/ssBabies/v8.07/TTZlofix.root");
      // ch3->Add("/nfs-7/userdata/ss2015/ssBabies/v8.07/TTZnlo.root");
  }

  std::vector<Color_t> colors;
  if(doTTZ) {
      colors.push_back(kGreen-6);  // ttzh
      colors.push_back(kGreen-2);  // ttzh
  } else {
      colors.push_back(kGreen+3);  // tw
      colors.push_back(kGreen+1);  // tw
  }

TFile *fout = new TFile("hists.root","RECREATE");

  results_t ch1_graphs       = run(ch1      , "ch1"    , fout  );
  results_t ch2_graphs       = run(ch2      , "ch2"    , fout  );
  results_t ch3_graphs       = run(ch3      , "ch3"    , fout  );

fout->Write();
fout->Close();

  //Prepare for plots -- null
  TH1F* null = new TH1F("","",1,0,1);

  //Prepare for plots -- backgrounds, sample
  vector <TH1F*> background_high; background_high.push_back(ch2_graphs.hh); background_high.push_back(ch3_graphs.hh);
  vector <TH1F*> background_hl; background_hl.push_back(ch2_graphs.hl); background_hl.push_back(ch3_graphs.hl);
  vector <TH1F*> background_low; background_low.push_back(ch2_graphs.ll); background_low.push_back(ch3_graphs.ll);

  //Kinematic distributions

  vector <TH1F*> background_njets; background_njets.push_back(ch2_graphs.njets); background_njets.push_back(ch3_graphs.njets);
  vector <TH1F*> background_nbtags; background_nbtags.push_back(ch2_graphs.nbtags); background_nbtags.push_back(ch3_graphs.nbtags);
  vector <TH1F*> background_mtmin; background_mtmin.push_back(ch2_graphs.mtmin); background_mtmin.push_back(ch3_graphs.mtmin);
  vector <TH1F*> background_ht; background_ht.push_back(ch2_graphs.ht); background_ht.push_back(ch3_graphs.ht);
  vector <TH1F*> background_met; background_met.push_back(ch2_graphs.met); background_met.push_back(ch3_graphs.met);
  vector <TH1F*> background_lep1pt; background_lep1pt.push_back(ch2_graphs.lep1pt); background_lep1pt.push_back(ch3_graphs.lep1pt);
  vector <TH1F*> background_lep2pt; background_lep2pt.push_back(ch2_graphs.lep2pt); background_lep2pt.push_back(ch3_graphs.lep2pt);

  //Prepare for plots -- background titles
  vector <string> titles;

  string postfix = "";
  string type="LO";

  titles.push_back("LO [bugged]");
  titles.push_back("LO [fixed]");
  postfix = "NLOvsLO";
  
  // if (data) {
  //     type = "Data";
  //     postfix = "_comparison_data";
  // } 
  // // string extra = " --setMaximum 57.0 ";
  string extra = " --legendCounts --histoErrors --noStack ";



 if(doTTZ) {
     dataMCplotMaker(ch1_graphs.hh      , background_high      , titles , "[ttZ] HH"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttz_HH_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.hl      , background_hl        , titles , "[ttZ] HL"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttz_HL_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.ll      , background_low       , titles , "[ttZ] LL"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttz_LL_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.njets      , background_njets      , titles , "[ttZ] Njets"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttz_kinem_njets_%s --noDivisionLabel --xAxisLabel Njets --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.nbtags     , background_nbtags     , titles , "[ttZ] Nbtags"     , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttz_kinem_nbtags_%s --noDivisionLabel --xAxisLabel Nbtags --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                 , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.mtmin      , background_mtmin      , titles , "[ttZ] mtmin"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttz_kinem_mtmin_%s --xAxisLabel mtmin --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                            , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.met        , background_met        , titles , "[ttZ] met"        , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttz_kinem_met_%s --xAxisLabel MET --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.ht         , background_ht         , titles , "[ttZ] ht"         , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttz_kinem_ht_%s --xAxisLabel HT --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);

       // dataMCplotMaker(p_data.SRHH.TOTAL, SRHH_plots, titles, "HH SRs", "", Form("--lumi %.1f --outputName plots/HHSR.pdf --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08 --noOverflow --systInclStat  --outOfFrame --legendTaller 0.15 --yTitleOffset -0.5 --dataName Data %s --type Preliminary --makeTable  ", lumiAG, extra.Data()), vector <TH1F*>(), vector <string>(), colors);
       // dataMCplotMaker(p_data.SRHH.TOTAL, SRHH_plots, titles, "HH SRs", "", Form("--lumi %.1f --outputName plots/HHSR.pdf --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08 --noOverflow --systInclStat  --outOfFrame --legendTaller 0.15 --yTitleOffset -0.5 --dataName Data %s --type Preliminary --makeTable  ", lumiAG, extra.Data()), vector <TH1F*>(), vector <string>(), colors);
     // dataMCplotMaker(ch1_graphs.ht, background_ht, titles , "[ttZ] ht", "" , Form("--topYaxisTitle NLO/%s --compareMultiple --dataName NLO --outputName yield_plots_bugcomp/ttz_kinem_ht_%s --xAxisLabel HT --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s ", type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), background_ht, titles, colors);

 } else {
     dataMCplotMaker(ch1_graphs.hh      , background_high      , titles , "[ttW] HH"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttw_HH_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.hl      , background_hl        , titles , "[ttW] HL"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttw_HL_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.ll      , background_low       , titles , "[ttW] LL"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttw_LL_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.njets      , background_njets      , titles , "[ttW] Njets"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttw_kinem_njets_%s --noDivisionLabel --xAxisLabel Njets --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.nbtags     , background_nbtags     , titles , "[ttW] Nbtags"     , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttw_kinem_nbtags_%s --noDivisionLabel --xAxisLabel Nbtags --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                 , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.mtmin      , background_mtmin      , titles , "[ttW] mtmin"      , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttw_kinem_mtmin_%s --xAxisLabel mtmin --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                            , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.met        , background_met        , titles , "[ttW] met"        , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttw_kinem_met_%s --xAxisLabel MET --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch1_graphs.ht         , background_ht         , titles , "[ttW] ht"         , "" , Form("--topYaxisTitle NLO/%s --dataName NLO --outputName yield_plots_bugcomp/ttw_kinem_ht_%s --xAxisLabel HT  --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 }



}
