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
}; 


results_t run(TChain* chain, string name, hyp_type_t flavor = UNASSIGNED){

  //Declare hists
  TH1F *HighHighPlot = new TH1F(Form("%s_hh", name.c_str()), Form("%s_hh", name.c_str()), 32, 1, 33); 
  TH1F *HighLowPlot  = new TH1F(Form("%s_hl", name.c_str()), Form("%s_hl", name.c_str()), 26, 1, 27); 
  TH1F *LowLowPlot   = new TH1F(Form("%s_ll", name.c_str()), Form("%s_ll", name.c_str()),  8, 1,  9); 

  TH1F *Njets   = new TH1F(Form("%s_njets", name.c_str()), Form("%s_njets", name.c_str()),  8, 0,  8); 
  TH1F *Nbtags   = new TH1F(Form("%s_nbtags", name.c_str()), Form("%s_nbtags", name.c_str()),  8, 0,  8); 
  TH1F *Mtmin   = new TH1F(Form("%s_mtmin", name.c_str()), Form("%s_mtmin", name.c_str()),  20, 0,  200); 
  TH1F *MET   = new TH1F(Form("%s_met", name.c_str()), Form("%s_met", name.c_str()),  20, 0,  400); 
  TH1F *HT   = new TH1F(Form("%s_ht", name.c_str()), Form("%s_ht", name.c_str()),  20, 0,  600); 
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
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

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

      //Reject non-SS
      if (ss::hyp_class() != 3 && testClassSF==false) continue;
      if (ss::hyp_class() != 2 && testClassSF==true) continue;

      //Flavor selection
      if (flavor == MM && ss::hyp_type() != 3) continue;
      if (flavor == EM && ss::hyp_type() != 1 && ss::hyp_type() != 2) continue;
      if (flavor == EE && ss::hyp_type() != 0) continue;
   
      //Calculate mtmin
      float mt1 = MT(ss::lep1_p4().pt(), ss::lep1_p4().phi(), ss::met(), ss::metPhi());
      float mt2 = MT(ss::lep2_p4().pt(), ss::lep2_p4().phi(), ss::met(), ss::metPhi());
      float mtmin = mt1 > mt2 ? mt2 : mt1; 

       
      //Figure out region, fill plot
      anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());  
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      if (SR > 0 && categ == HighHigh) HighHighPlot->Fill(SR, weight);
      if (SR > 0 && categ == HighLow)  HighLowPlot ->Fill(SR, weight);
      if (SR > 0 && categ == LowLow)   LowLowPlot  ->Fill(SR, weight);

      if (SR > 0) {
          Njets->Fill(ss::njets(),weight);
          Nbtags->Fill(ss::nbtags(),weight);
          Mtmin->Fill(ss::mtmin(),weight);
          MET->Fill(ss::met(),weight);
          HT->Fill(ss::ht(),weight);
          Lep1pt->Fill(ss::lep1_p4().pt(),weight);
          Lep2pt->Fill(ss::lep2_p4().pt(),weight);
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

  return result;

}

void ratios(bool data){

    

  //Make chains
  // TChain* data        = new TChain("t");

  TChain* c74          = new TChain("t");
  TChain* c76          = new TChain("t");

  TString tag = getTag();

  if(data) {
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataDoubleEG*.root"    );
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataDoubleMuon*.root"  );
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DataMuonEG*.root"      );

      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/DataDoubleEG*.root"    );
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/DataDoubleMuon*.root"  );
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/DataMuonEG*.root"      );
  } else {
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTBAR.root");
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/SINGLETOP*.root");
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTW.root");
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/TTZL.root");
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WZ.root");
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WJets.root");
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/WGToLNuG.root");
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DY_high.root");
      c74->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02/DY_low.root");

      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/TTBAR.root");
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/SINGLETOP*.root");
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/TTW.root");
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/TTZL.root");
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/WZ.root");
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/WJets.root");
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/WGToLNuG.root");
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/DY_high.root");
      c76->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00/DY_low.root");
  }

  results_t c74_graphs       = run(c74      , "c74"      );
  results_t c76_graphs       = run(c76      , "c76"      );


  //Prepare for plots -- null
  TH1F* null = new TH1F("","",1,0,1);

  //Prepare for plots -- backgrounds, sample
  vector <TH1F*> background_high; background_high.push_back(c74_graphs.hh);
  vector <TH1F*> background_hl; background_hl.push_back(c74_graphs.hl);
  vector <TH1F*> background_low; background_low.push_back(c74_graphs.ll);

  //Kinematic distributions
  vector <TH1F*> background_njets; background_njets.push_back(c74_graphs.njets);
  vector <TH1F*> background_nbtags; background_nbtags.push_back(c74_graphs.nbtags);
  vector <TH1F*> background_mtmin; background_mtmin.push_back(c74_graphs.mtmin);
  vector <TH1F*> background_ht; background_ht.push_back(c74_graphs.ht);
  vector <TH1F*> background_met; background_met.push_back(c74_graphs.met);
  vector <TH1F*> background_lep1pt; background_lep1pt.push_back(c74_graphs.lep1pt);
  vector <TH1F*> background_lep2pt; background_lep2pt.push_back(c74_graphs.lep2pt);

  //Prepare for plots -- background titles
  vector <string> titles;
  if(data) titles.push_back("74X Data");
  else titles.push_back("74X MC");

  string postfix = "_comparison_mc";
  string type="MC";
  
  if (data) {
      type = "Data";
      postfix = "_comparison_data";
  } 
  // string extra = " --setMaximum 57.0 ";
  string extra = "";


  //Make plots -- sample
  dataMCplotMaker(c76_graphs.hh     , background_high   , titles , "H-H"    , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName yield_plots/high_yields%s --noDivisionLabel --xAxisLabel   --energy 13 --lumi %.2f --lumiUnit fb --nDivisions 210 --legendUp -0.1 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear %s --xAxisVerticalBinLabels --xAxisBinLabels 1HH         , 2HH             , 3HH       , 4HH               , 5HH , 6HH , 7HH , 8HH , 9HH , 10HH , 11HH , 12HH , 13HH , 14HH , 15HH , 16HH , 17HH , 18HH , 19HH , 20HH , 21HH , 22HH , 23HH , 24HH , 25HH , 26HH  , 27HH            , 28HH      , 29HH              , 30HH , 31HH , 32HH" , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));
  dataMCplotMaker(c76_graphs.hl     , background_hl     , titles , "H-L"    , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName yield_plots//hl_yields%s --noDivisionLabel --xAxisLabel --energy 13 --lumi %.2f --lumiUnit fb --nDivisions 210 --legendUp -0.1 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear %s --xAxisVerticalBinLabels --xAxisBinLabels 1HL , 2HL             , 3HL       , 4HL               , 5HL , 6HL , 7HL , 8HL , 9HL , 10HL , 11HL , 12HL , 13HL , 14HL , 15HL , 16HL , 17HL , 18HL , 19HL , 20HL , 21HL , 22HL , 23HL , 24HL , 25HL , 26HL" , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));
  dataMCplotMaker(c76_graphs.njets  , background_njets  , titles , "Njets"  , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --outputName yield_plots/kinem_njets_%s --noDivisionLabel --xAxisLabel   --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear %s "                                                                                                                , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));
  dataMCplotMaker(c76_graphs.nbtags , background_nbtags , titles , "Nbtags" , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --outputName yield_plots/kinem_nbtags_%s --noDivisionLabel --xAxisLabel   --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear %s "                                                                                                               , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));
  dataMCplotMaker(c76_graphs.mtmin  , background_mtmin  , titles , "mtmin"  , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --outputName yield_plots/kinem_mtmin_%s --xAxisLabel mtmin  --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                            , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));
  dataMCplotMaker(c76_graphs.met    , background_met    , titles , "met"    , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --outputName yield_plots/kinem_met_%s --xAxisLabel MET  --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));
  dataMCplotMaker(c76_graphs.ht     , background_ht     , titles , "ht"     , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --outputName yield_plots/kinem_ht_%s --xAxisLabel HT  --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                                  , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));
  dataMCplotMaker(c76_graphs.lep1pt , background_lep1pt , titles , "lep1pt" , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --outputName yield_plots/kinem_lep1pt_%s --xAxisLabel lep1pt --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                           , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));
  dataMCplotMaker(c76_graphs.lep2pt , background_lep2pt , titles , "lep2pt" , "" , Form("--topYaxisTitle 76X/74X --dataName 76X %s --outputName yield_plots/kinem_lep2pt_%s --xAxisLabel lep2pt --energy 13 --lumi %.2f --lumiUnit fb --legendUp -0.1 --legendRight -0.00 --legendTextSize 0.0325 --isLinear %s "                                                                                                                                           , type.c_str(), postfix.c_str() , getLumi() , extra.c_str() ));

}
