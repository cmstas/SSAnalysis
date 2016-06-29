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
float theLumi = 2.3;

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
  TH1F *HighHighPlot = new TH1F(Form("%s_hh", name.c_str()), Form("%s_hh", name.c_str()), 32, 1, 33); 
  TH1F *HighLowPlot  = new TH1F(Form("%s_hl", name.c_str()), Form("%s_hl", name.c_str()), 26, 1, 27); 
  TH1F *LowLowPlot   = new TH1F(Form("%s_ll", name.c_str()), Form("%s_ll", name.c_str()),  8, 1,  9); 

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
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);
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

void ratios(bool data){

    

  //Make chains
  // TChain* data        = new TChain("t");

  TChain* c74          = new TChain("t");
  TChain* c80          = new TChain("t");

  bool doTTZ = true;
  bool do74NLO = false;

  TString tag = getTag();

  if(data) {
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataDoubleEG*.root"    );
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataDoubleMuon*.root"  );
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataMuonEG*.root"      );

      c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/DataDoubleEG*.root"    );
      c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/DataDoubleMuon*.root"  );
      c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/DataMuonEG*.root"      );
  } else {

      // 80X NLO:
      if(!do74NLO) {
          if(doTTZ) c74->Add("/home/users/namin/2016/ss/master/SSAnalysis/batch/TTZnlo.root");
          else c74->Add("/home/users/namin/2016/ss/master/SSAnalysis/batch/TTWnlo.root");
      }

      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTBAR_PH.root");
      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/SINGLETOP*.root");
      
      if(do74NLO) {
          if(doTTZ) c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTZL*.root");
          else c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTW.root");
      }

      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTHtoNonBB.root");
      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/QQWW.root");


      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WZ.root");
      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WJets.root");
      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WGToLNuG.root");
      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DY_high.root");
      // c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DY_low.root");

      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTBAR_PH.root");
      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/SINGLETOP*.root");

      if(doTTZ) c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTZ*.root");
      else c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTW.root");
      
      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTHtoNonBB.root");
      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/QQWW.root");

      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WZ.root");
      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WJets.root");
      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WGToLNuG.root");
      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/DY_high.root");
      // c80->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/DY_low.root");
  }
  std::vector<Color_t> colors;
  if(doTTZ) colors.push_back(kGreen-6);  // ttzh
  else colors.push_back(kGreen+3);  // tw

  results_t c74_graphs       = run(c74      , "c74"      );
  results_t c80_graphs       = run(c80      , "c80"      );


  //Prepare for plots -- null
  TH1F* null = new TH1F("","",1,0,1);

  //Prepare for plots -- backgrounds, sample
  vector <TH1F*> background_high; background_high.push_back(c74_graphs.hh);
  vector <TH1F*> background_hl; background_hl.push_back(c74_graphs.hl);
  vector <TH1F*> background_low; background_low.push_back(c74_graphs.ll);

  //Kinematic distributions
  vector <TH1F*> background_base_njets_raw  ; background_base_njets_raw.push_back(c74_graphs.base_njets_raw)   ;
  vector <TH1F*> background_base_jetpt  ; background_base_jetpt.push_back(c74_graphs.base_jetpt)   ;
  vector <TH1F*> background_base_nbtags_raw ; background_base_nbtags_raw.push_back(c74_graphs.base_nbtags_raw) ;
  vector <TH1F*> background_base_rawmet     ; background_base_rawmet.push_back(c74_graphs.base_rawmet)         ;
  vector <TH1F*> background_base_ht_raw     ; background_base_ht_raw.push_back(c74_graphs.base_ht_raw)         ;

  vector <TH1F*> background_njets; background_njets.push_back(c74_graphs.njets);
  vector <TH1F*> background_njets_raw; background_njets_raw.push_back(c74_graphs.njets_raw);
  vector <TH1F*> background_nbtags; background_nbtags.push_back(c74_graphs.nbtags);
  vector <TH1F*> background_nbtags_raw; background_nbtags_raw.push_back(c74_graphs.nbtags_raw);
  vector <TH1F*> background_mtmin; background_mtmin.push_back(c74_graphs.mtmin);
  vector <TH1F*> background_ht; background_ht.push_back(c74_graphs.ht);
  vector <TH1F*> background_ht_raw; background_ht_raw.push_back(c74_graphs.ht_raw);
  vector <TH1F*> background_met; background_met.push_back(c74_graphs.met);
  vector <TH1F*> background_rawmet; background_rawmet.push_back(c74_graphs.rawmet);
  vector <TH1F*> background_lep1pt; background_lep1pt.push_back(c74_graphs.lep1pt);
  vector <TH1F*> background_lep2pt; background_lep2pt.push_back(c74_graphs.lep2pt);

  //Prepare for plots -- background titles
  vector <string> titles;
  if(data) titles.push_back("74X Data");
  else titles.push_back("80X NLO");

  string postfix = "_comparison_mc";
  string type="";
  
  if (data) {
      type = "Data";
      postfix = "_comparison_data";
  } 
  // string extra = " --setMaximum 57.0 ";
  string extra = " ";



  //Make plots -- sample
 dataMCplotMaker(c80_graphs.base_njets_raw  , background_base_njets_raw  , titles , "base Njets_raw"  , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_base_njets_raw_%s --noDivisionLabel --xAxisLabel --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                              , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 dataMCplotMaker(c80_graphs.base_jetpt  , background_base_jetpt  , titles , "base jetpt"  , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_base_jetpt_%s --noDivisionLabel --xAxisLabel --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                              , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 dataMCplotMaker(c80_graphs.base_nbtags_raw , background_base_nbtags_raw , titles , "base Nbtags_raw" , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_base_nbtags_raw%s --noDivisionLabel --xAxisLabel --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                              , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 dataMCplotMaker(c80_graphs.base_ht_raw     , background_base_ht_raw     , titles , "base ht_raw"     , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_base_ht_raw_%s --xAxisLabel ht_raw --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                          , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 dataMCplotMaker(c80_graphs.base_rawmet     , background_base_rawmet     , titles , "base rawmet"     , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_base_rawmet_%s --xAxisLabel rawmet --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                          , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);

 dataMCplotMaker(c80_graphs.hh         , background_high       , titles , "H-H"        , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName yield_plots/high_yields%s --noDivisionLabel --xAxisLabel --energy 13 --lumi %.2f --lumiUnit fb --nDivisions 210 --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s --xAxisVerticalBinLabels --xAxisBinLabels 1HH,2HH,3HH,4HH,5HH,6HH,7HH,8HH,9HH,10HH,11HH,12HH,13HH,14HH,15HH,16HH,17HH,18HH,19HH,20HH,21HH,22HH,23HH,24HH,25HH,26HH,27HH,28HH,29HH,30HH,31HH,32HH" , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 dataMCplotMaker(c80_graphs.hl         , background_hl         , titles , "H-L"        , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName yield_plots//hl_yields%s --noDivisionLabel --xAxisLabel --energy 13 --lumi %.2f --lumiUnit fb --nDivisions 210 --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s --xAxisVerticalBinLabels --xAxisBinLabels 1HL,2HL,3HL,4HL,5HL,6HL,7HL,8HL,9HL,10HL,11HL,12HL,13HL,14HL,15HL,16HL,17HL,18HL,19HL,20HL,21HL,22HL,23HL,24HL,25HL,26HL" , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);

 if(doTTZ) {
     dataMCplotMaker(c80_graphs.njets      , background_njets      , titles , "Njets"      , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_njets_%s --noDivisionLabel --setMaximum 3 --xAxisLabel Njets --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(c80_graphs.nbtags     , background_nbtags     , titles , "Nbtags"     , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_nbtags_%s --noDivisionLabel --setMaximum 4 --xAxisLabel Nbtags --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                 , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(c80_graphs.mtmin      , background_mtmin      , titles , "mtmin"      , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_mtmin_%s --xAxisLabel mtmin --setMaximum 1.2 --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                            , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(c80_graphs.met        , background_met        , titles , "met"        , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_met_%s --xAxisLabel MET --setMaximum 3 --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(c80_graphs.ht         , background_ht         , titles , "ht"         , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_ht_%s --xAxisLabel HT --setMaximum 2 --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 } else {
     dataMCplotMaker(c80_graphs.njets      , background_njets      , titles , "Njets"      , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_njets_%s --noDivisionLabel --setMaximum 9 --xAxisLabel Njets --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(c80_graphs.nbtags     , background_nbtags     , titles , "Nbtags"     , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_nbtags_%s --noDivisionLabel --setMaximum 12 --xAxisLabel Nbtags --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit --legendTextSize 0.0325 --isLinear %s "                                                                                                                 , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(c80_graphs.mtmin      , background_mtmin      , titles , "mtmin"      , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_mtmin_%s --xAxisLabel mtmin --setMaximum 2.5 --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                            , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(c80_graphs.met        , background_met        , titles , "met"        , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_met_%s --xAxisLabel MET --setMaximum 6.2 --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
     dataMCplotMaker(c80_graphs.ht         , background_ht         , titles , "ht"         , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_ht_%s --xAxisLabel HT --setMaximum 5.3 --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                  , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 }

 dataMCplotMaker(c80_graphs.lep1pt     , background_lep1pt     , titles , "lep1pt"     , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_lep1pt_%s --xAxisLabel lep1pt --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                          , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);
 dataMCplotMaker(c80_graphs.lep2pt     , background_lep2pt     , titles , "lep2pt"     , "" , Form("--topYaxisTitle LO/NLO --dataName 80X LO %s --outputName yield_plots/kinem_lep2pt_%s --xAxisLabel lep2pt --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                          , type.c_str() , postfix.c_str() , theLumi , extra.c_str() ), vector<TH1F*>(), vector<string>(), colors);


}
