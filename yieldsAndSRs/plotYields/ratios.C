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

    TH1F* base_njets_raw;
    TH1F* base_jetpt;
    TH1F* base_nbtags_raw;
    TH1F* base_rawmet;
    TH1F* base_ht_raw;

    TH1F* njets;
    TH1F* njets_raw;
    TH1F* nbtags;
    TH1F* nbtags_raw;
    TH1F* mtmin;
    TH1F* met;
    TH1F* rawmet;
    TH1F* ht;
    TH1F* ht_raw;
    TH1F* lep1pt;
    TH1F* lep2pt;
}; 


results_t run(TChain* chain, string name, hyp_type_t flavor = UNASSIGNED){

  //Declare hists
  TH1F *HighHighPlot = new TH1F(Form("%s_hh", name.c_str()), Form("%s_hh", name.c_str()), 33, 0.5, 33.5); 
  TH1F *HighLowPlot  = new TH1F(Form("%s_hl", name.c_str()), Form("%s_hl", name.c_str()), 27, 0.5, 27.5); 
  TH1F *LowLowPlot   = new TH1F(Form("%s_ll", name.c_str()), Form("%s_ll", name.c_str()),  8, 0.5,  8.5); 

  TH1F *base_Njets_raw   = new TH1F(Form("%s_base_njets_raw", name.c_str()), Form("%s_base_njets_raw", name.c_str()),  8, 0,  8); 
  TH1F *base_jetPt   = new TH1F(Form("%s_base_jetPt", name.c_str()), Form("%s_base_jetPt", name.c_str()),  15, 0,  300); 
  TH1F *base_Nbtags_raw   = new TH1F(Form("%s_base_nbtags_raw", name.c_str()), Form("%s_base_nbtags_raw", name.c_str()),  8, 0,  8); 
  TH1F *base_rawMET   = new TH1F(Form("%s_base_rawmet", name.c_str()), Form("%s_base_rawmet", name.c_str()),  15, 0,  300); 
  TH1F *base_HT_raw   = new TH1F(Form("%s_bas_basee_ht_raw", name.c_str()), Form("%s_base_ht_raw", name.c_str()),  15, 0,  1200); 

  TH1F *Njets   = new TH1F(Form("%s_njets", name.c_str()), Form("%s_njets", name.c_str()),  8, 0,  8); 
  TH1F *Njets_raw   = new TH1F(Form("%s_njets_raw", name.c_str()), Form("%s_njets_raw", name.c_str()),  8, 0,  8); 
  TH1F *Nbtags   = new TH1F(Form("%s_nbtags", name.c_str()), Form("%s_nbtags", name.c_str()),  8, 0,  8); 
  TH1F *Nbtags_raw   = new TH1F(Form("%s_nbtags_raw", name.c_str()), Form("%s_nbtags_raw", name.c_str()),  8, 0,  8); 
  TH1F *Mtmin   = new TH1F(Form("%s_mtmin", name.c_str()), Form("%s_mtmin", name.c_str()),  20, 0,  200); 
  TH1F *MET   = new TH1F(Form("%s_met", name.c_str()), Form("%s_met", name.c_str()),  12, 0,  300); 
  TH1F *rawMET   = new TH1F(Form("%s_rawmet", name.c_str()), Form("%s_rawmet", name.c_str()),  12, 0,  300); 
  TH1F *HT   = new TH1F(Form("%s_ht", name.c_str()), Form("%s_ht", name.c_str()),  15, 0,  1200); 
  TH1F *HT_raw   = new TH1F(Form("%s_ht_raw", name.c_str()), Form("%s_ht_raw", name.c_str()),  15, 0,  1200); 
  TH1F *Lep1pt   = new TH1F(Form("%s_lep1pt", name.c_str()), Form("%s_lep1pt", name.c_str()),  20, 0,  150); 
  TH1F *Lep2pt   = new TH1F(Form("%s_lep2pt", name.c_str()), Form("%s_lep2pt", name.c_str()),  20, 0,  150); 

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
      // float weight = isData ? 1 : ss::scale1fb()*getLumiUnblind();
      float weight = isData ? 1 : ss::scale1fb()*theLumi; // FIXME

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
        if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA())) continue;
        if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA())) continue;
      } 
      //drop electrons below 15 GeV
      if (abs(ss::lep1_id())==11 && lep1_pt<15) continue;
      if (abs(ss::lep2_id())==11 && lep2_pt<15) continue;	


      //Require baseline selections
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);
      if (BR < 0) continue;
   
      //Calculate mtmin

      base_Njets_raw->Fill(ss::njets_raw(),weight);
      base_jetPt->Fill(ss::jet_pt().at(0),weight);
      base_Nbtags_raw->Fill(ss::nbtags_raw(),weight);
      base_rawMET->Fill(ss::rawmet(),weight);
      base_HT_raw->Fill(ss::ht_raw(),weight);
       
      //Figure out region, fill plot
      anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);  
      int SR = signalRegion2016(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);
      if (SR > 0 && categ == HighHigh) HighHighPlot->Fill(SR, weight);
      if (SR > 0 && categ == HighLow)  HighLowPlot ->Fill(SR, weight);
      if (SR > 0 && categ == LowLow)   LowLowPlot  ->Fill(SR, weight);

      if (SR > 0) {
          Njets->Fill(ss::njets(),weight);
          Njets_raw->Fill(ss::njets_raw(),weight);
          Nbtags->Fill(ss::nbtags(),weight);
          Nbtags_raw->Fill(ss::nbtags_raw(),weight);
          Mtmin->Fill(mtmin,weight);
          MET->Fill(ss::met(),weight);
          rawMET->Fill(ss::rawmet(),weight);
          HT->Fill(ss::ht(),weight);
          HT_raw->Fill(ss::ht_raw(),weight);
          Lep1pt->Fill(lep1_pt,weight);
          Lep2pt->Fill(lep2_pt,weight);
      }

    }//event loop
  }//file loop

  if (isData) cout << "number of duplicates removed: " << reject << endl;
 
  //Return hists
  results_t result; 
  result.base_njets_raw = base_Njets_raw;
  result.base_jetpt = base_jetPt;
  result.base_nbtags_raw = base_Nbtags_raw;
  result.base_rawmet = base_rawMET;
  result.base_ht_raw = base_HT_raw;
  result.hh = HighHighPlot;
  result.hl = HighLowPlot;
  result.ll = LowLowPlot;
  result.njets = Njets;
  result.njets_raw = Njets_raw;
  result.nbtags = Nbtags;
  result.nbtags_raw = Nbtags_raw;
  result.mtmin = Mtmin;
  result.met = MET;
  result.rawmet = rawMET;
  result.ht = HT;
  result.ht_raw = HT_raw;
  result.lep1pt = Lep1pt;
  result.lep2pt = Lep2pt;

  return result;

}

void ratios(bool data, TString opts=""){

    

  //Make chains
  // TChain* data        = new TChain("t");

  TChain* ch1          = new TChain("t");
  TChain* ch2          = new TChain("t");

  bool doTTZ = opts.Contains("doTTZ");
  // Compare new NLO with old NLO unless bool set, then compare new NLO with high stats LO
  bool doCompareNLOwithLO = opts.Contains("doCompareNLOwithLO");


  TString tag = getTag();

  if(data) {
      ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataDoubleEG*.root"    );
      ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataDoubleMuon*.root"  );
      ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataMuonEG*.root"      );

      ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/DataDoubleEG*.root"    );
      ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/DataDoubleMuon*.root"  );
      ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/DataMuonEG*.root"      );
  } else {

      if(doTTZ) ch2->Add("/home/users/namin/2016/ss/master/SSAnalysis/batch/TTZnlo_new.root");
      else ch2->Add("/home/users/namin/2016/ss/master/SSAnalysis/batch/TTWnlo_new.root");

      if (doCompareNLOwithLO) {
          if(doTTZ) ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04/TTZ*.root");
          // if(doTTZ) ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04/TTZ.root");
          else ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04/TTW.root");
      } else {
          if(doTTZ) ch1->Add("/home/users/namin/2016/ss/master/SSAnalysis/batch/TTZnlo_old.root");
          else ch1->Add("/home/users/namin/2016/ss/master/SSAnalysis/batch/TTWnlo_old.root");
      }

      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTBAR_PH.root");
      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/SINGLETOP*.root");
      
      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTHtoNonBB.root");
      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/QQWW.root");


      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WZ.root");
      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WJets.root");
      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WGToLNuG.root");
      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DY_high.root");
      // ch1->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DY_low.root");

      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTBAR_PH.root");
      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/SINGLETOP*.root");

      
      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTHtoNonBB.root");
      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/QQWW.root");

      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WZ.root");
      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WJets.root");
      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WGToLNuG.root");
      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/DY_high.root");
      // ch2->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/DY_low.root");
  }
  std::vector<Color_t> colors;
  if(doTTZ) colors.push_back(kGreen-6);  // ttzh
  else colors.push_back(kGreen+3);  // tw

  results_t ch1_graphs       = run(ch1      , "ch1"      );
  results_t ch2_graphs       = run(ch2      , "ch2"      );


  //Prepare for plots -- null
  TH1F* null = new TH1F("","",1,0,1);

  //Prepare for plots -- backgrounds, sample
  vector <TH1F*> background_high; background_high.push_back(ch1_graphs.hh);
  vector <TH1F*> background_hl; background_hl.push_back(ch1_graphs.hl);
  vector <TH1F*> background_low; background_low.push_back(ch1_graphs.ll);

  //Kinematic distributions
  vector <TH1F*> background_base_njets_raw  ; background_base_njets_raw.push_back(ch1_graphs.base_njets_raw)   ;
  vector <TH1F*> background_base_jetpt  ; background_base_jetpt.push_back(ch1_graphs.base_jetpt)   ;
  vector <TH1F*> background_base_nbtags_raw ; background_base_nbtags_raw.push_back(ch1_graphs.base_nbtags_raw) ;
  vector <TH1F*> background_base_rawmet     ; background_base_rawmet.push_back(ch1_graphs.base_rawmet)         ;
  vector <TH1F*> background_base_ht_raw     ; background_base_ht_raw.push_back(ch1_graphs.base_ht_raw)         ;

  vector <TH1F*> background_njets; background_njets.push_back(ch1_graphs.njets);
  vector <TH1F*> background_njets_raw; background_njets_raw.push_back(ch1_graphs.njets_raw);
  vector <TH1F*> background_nbtags; background_nbtags.push_back(ch1_graphs.nbtags);
  vector <TH1F*> background_nbtags_raw; background_nbtags_raw.push_back(ch1_graphs.nbtags_raw);
  vector <TH1F*> background_mtmin; background_mtmin.push_back(ch1_graphs.mtmin);
  vector <TH1F*> background_ht; background_ht.push_back(ch1_graphs.ht);
  vector <TH1F*> background_ht_raw; background_ht_raw.push_back(ch1_graphs.ht_raw);
  vector <TH1F*> background_met; background_met.push_back(ch1_graphs.met);
  vector <TH1F*> background_rawmet; background_rawmet.push_back(ch1_graphs.rawmet);
  vector <TH1F*> background_lep1pt; background_lep1pt.push_back(ch1_graphs.lep1pt);
  vector <TH1F*> background_lep2pt; background_lep2pt.push_back(ch1_graphs.lep2pt);

  //Prepare for plots -- background titles
  vector <string> titles;

  string postfix = "";
  string type="Old NLO";

  if (doCompareNLOwithLO) {
      type = "LO";
      titles.push_back("LO");
      postfix = "NLOvsLO";
  } else {
      type = "Old NLO";
      titles.push_back("Old NLO");
      postfix = "NLOvsNLO";
  }
  
  // if (data) {
  //     type = "Data";
  //     postfix = "_comparison_data";
  // } 
  // // string extra = " --setMaximum 57.0 ";
  string extra = " --legendCounts --histoErrors ";




 if(doTTZ) {
     dataMCplotMaker(ch2_graphs.hh      , background_high      , titles , "[ttZ] HH"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttz_HH_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.hl      , background_hl        , titles , "[ttZ] HL"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttz_HL_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.ll      , background_low       , titles , "[ttZ] LL"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttz_LL_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.njets      , background_njets      , titles , "[ttZ] Njets"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttz_kinem_njets_%s --noDivisionLabel --xAxisLabel Njets --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.nbtags     , background_nbtags     , titles , "[ttZ] Nbtags"     , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttz_kinem_nbtags_%s --noDivisionLabel --xAxisLabel Nbtags --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                 , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.mtmin      , background_mtmin      , titles , "[ttZ] mtmin"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttz_kinem_mtmin_%s --xAxisLabel mtmin --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                            , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.met        , background_met        , titles , "[ttZ] met"        , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttz_kinem_met_%s --xAxisLabel MET --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.ht         , background_ht         , titles , "[ttZ] ht"         , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttz_kinem_ht_%s --xAxisLabel HT --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 } else {
     dataMCplotMaker(ch2_graphs.hh      , background_high      , titles , "[ttW] HH"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttw_HH_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.hl      , background_hl        , titles , "[ttW] HL"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttw_HL_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.ll      , background_low       , titles , "[ttW] LL"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttw_LL_%s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.njets      , background_njets      , titles , "[ttW] Njets"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttw_kinem_njets_%s --noDivisionLabel --xAxisLabel Njets --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.nbtags     , background_nbtags     , titles , "[ttW] Nbtags"     , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttw_kinem_nbtags_%s --noDivisionLabel --xAxisLabel Nbtags --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                 , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.mtmin      , background_mtmin      , titles , "[ttW] mtmin"      , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttw_kinem_mtmin_%s --xAxisLabel mtmin --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                            , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.met        , background_met        , titles , "[ttW] met"        , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttw_kinem_met_%s --xAxisLabel MET --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(ch2_graphs.ht         , background_ht         , titles , "[ttW] ht"         , "" , Form("--topYaxisTitle New NLO/%s --dataName New NLO --outputName yield_plots/ttw_kinem_ht_%s --xAxisLabel HT  --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 }

 // dataMCplotMaker(ch2_graphs.lep1pt     , background_lep1pt     , titles , "lep1pt"     , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_lep1pt_%s --xAxisLabel lep1pt --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                          , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 // dataMCplotMaker(ch2_graphs.lep2pt     , background_lep2pt     , titles , "lep2pt"     , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_lep2pt_%s --xAxisLabel lep2pt --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                          , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);


}
