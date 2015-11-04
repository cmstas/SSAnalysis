#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
//include "../software/tableMaker/CTable.h"
#include "../../CORE/SSSelections.h"
#include "../../CORE/Tools/dorky/dorky.h"
#include "../../CORE/Tools/utils.h"
#include "../../classFiles/v4.04/SS.h"
#include "../../commonUtils.h"

//Tables on/off
bool makeTables = 0;
bool testClassSF = false;

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

float getYield(results_t results, int bin, int which){
  if (which == 0) return results.hh->Integral(bin, bin); 
  if (which == 1) return results.hl->Integral(bin, bin); 
  if (which == 2) return results.ll->Integral(bin, bin); 
  return -1;
}

results_t run(TChain* chain, string name, hyp_type_t flavor = UNASSIGNED){

  //Declare hists
  TH1F *HighHighPlot = new TH1F(Form("%s_hh", name.c_str()), Form("%s_hh", name.c_str()), 32, 1, 33); 
  TH1F *HighLowPlot  = new TH1F(Form("%s_hl", name.c_str()), Form("%s_hl", name.c_str()), 26, 1, 27); 
  TH1F *LowLowPlot   = new TH1F(Form("%s_ll", name.c_str()), Form("%s_ll", name.c_str()),  8, 1,  9); 

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

    }//event loop
  }//file loop

  if (isData) cout << "number of duplicates removed: " << reject << endl;
 
  //Return hists
  results_t result; 
  result.hh = HighHighPlot;
  result.hl = HighLowPlot;
  result.ll = LowLowPlot;
  return result;

}

void yields(){

  //Make chains
  TChain* data        = new TChain("t");
  TChain* ttbar       = new TChain("t");
  TChain* ttw         = new TChain("t");
  TChain* ttz         = new TChain("t");
  TChain* wz          = new TChain("t");
  TChain* wjets       = new TChain("t");
  TChain* dy          = new TChain("t");
  TChain* t1tttt_1200 = new TChain("t");
  TChain* t1tttt_1500 = new TChain("t");
  TChain* t5qqww_1200 = new TChain("t");
  TChain* t5qqww_deg  = new TChain("t");
 
  TString tag = getTag();

  //Fill chains
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGC_05oct.root"    , tag.Data()));
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonC_05oct.root"  , tag.Data()));
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGC_05oct.root"      , tag.Data()));
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_05oct.root"    , tag.Data()));
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_05oct.root"  , tag.Data()));
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_05oct.root"      , tag.Data()));
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_v4.root"       , tag.Data()));
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_v4.root"     , tag.Data()));
  data->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_v4.root"         , tag.Data()));

  ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTBAR.root");
  ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/SINGLETOP*.root");

  ttw->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTW.root");
  ttz->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTZL.root");
  wz->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WZ3LNU.root");

  wjets->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WJets.root");
  wjets->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WGToLNuG.root");

  dy->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DY_high.root");
  dy->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DY_low.root");

  t1tttt_1200->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/T1TTTT_1200.root");
  t1tttt_1500->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/T1TTTT_1500.root");

  //Chains for type
  TChain* all_bkgd = new TChain("t");
  all_bkgd->Add(ttbar);
  all_bkgd->Add(ttw);
  all_bkgd->Add(ttz);
  all_bkgd->Add(wz);
  all_bkgd->Add(dy);

  //Make the signal histograms
  results_t data_graphs        = run(data       , "data"      );
  results_t ttbar_graphs       = run(ttbar      , "ttbar"      );
  results_t ttw_graphs         = run(ttw        , "ttw"        );
  results_t ttz_graphs         = run(ttz        , "ttz"        );
  results_t wz_graphs          = run(wz         , "wz"         );
  results_t wjets_graphs       = run(wjets      , "wjets"      );
  results_t dy_graphs          = run(dy         , "dy"      );
  results_t t1tttt_1200_graphs = run(t1tttt_1200, "t1tttt_1200");
  results_t t1tttt_1500_graphs = run(t1tttt_1500, "t1tttt_1500");
  results_t t5qqww_1200_graphs = run(t5qqww_1200, "t5qqww_1200");
  results_t t5qqww_deg_graphs  = run(t5qqww_deg , "t5qqww_deg" );

  //Make the type histograms
  results_t bkgd_graphs_ee  = run(all_bkgd, "bkgd_ee", EE);
  results_t bkgd_graphs_em  = run(all_bkgd, "bkgd_em", EM);
  results_t bkgd_graphs_mm  = run(all_bkgd, "bkgd_mm", MM);

  //Prepare for plots -- null
  TH1F* null = new TH1F("","",1,0,1);

  //Prepare for plots -- backgrounds, sample
  vector <TH1F*> background_high; 
  background_high.push_back(ttbar_graphs.hh);
  background_high.push_back(ttw_graphs.hh);
  background_high.push_back(ttz_graphs.hh);
  background_high.push_back(wz_graphs.hh);
  background_high.push_back(dy_graphs.hh);
  vector <TH1F*> background_hl; 
  background_hl.push_back(ttbar_graphs.hl);
  background_hl.push_back(ttw_graphs.hl);
  background_hl.push_back(ttz_graphs.hl);
  background_hl.push_back(wz_graphs.hl);
  background_hl.push_back(dy_graphs.hl);
  vector <TH1F*> background_low; 
  background_low.push_back(ttbar_graphs.ll);
  background_low.push_back(ttw_graphs.ll);
  background_low.push_back(ttz_graphs.ll);
  background_low.push_back(wz_graphs.ll);
  background_low.push_back(dy_graphs.ll);

  //Prepare for plots -- backgrounds, type
  vector <TH1F*> background_type_high; 
  background_type_high.push_back(bkgd_graphs_ee.hh);
  background_type_high.push_back(bkgd_graphs_em.hh);
  background_type_high.push_back(bkgd_graphs_mm.hh);
  vector <TH1F*> background_type_hl; 
  background_type_hl.push_back(bkgd_graphs_ee.hl);
  background_type_hl.push_back(bkgd_graphs_em.hl);
  background_type_hl.push_back(bkgd_graphs_mm.hl);
  vector <TH1F*> background_type_low; 
  background_type_low.push_back(bkgd_graphs_ee.ll);
  background_type_low.push_back(bkgd_graphs_em.ll);
  background_type_low.push_back(bkgd_graphs_mm.ll);

  //Prepare for plots -- signals, sample
  vector <TH1F*> signal_high; 
  signal_high.push_back(t1tttt_1200_graphs.hh);
  signal_high.push_back(t1tttt_1500_graphs.hh);
  //signal_high.push_back(t5qqww_1200_graphs.hh);
  //signal_high.push_back(t5qqww_deg_graphs.hh);
  vector <TH1F*> signal_hl; 
  signal_hl.push_back(t1tttt_1200_graphs.hl);
  signal_hl.push_back(t1tttt_1500_graphs.hl);
  //signal_hl.push_back(t5qqww_1200_graphs.hl);
  //signal_hl.push_back(t5qqww_deg_graphs.hl);
  vector <TH1F*> signal_low; 
  signal_low.push_back(t1tttt_1200_graphs.ll);
  signal_low.push_back(t1tttt_1500_graphs.ll);
  //signal_low.push_back(t5qqww_1200_graphs.ll);
  //signal_low.push_back(t5qqww_deg_graphs.ll);

  //Prepare for plots -- background titles
  vector <string> titles;
  titles.push_back("ttbar");
  titles.push_back("ttw");
  titles.push_back("ttz");
  titles.push_back("wz");
  titles.push_back("dy");

  //Prepare for plots -- background titles (type)
  vector <string> typetitles;
  typetitles.push_back("ee");
  typetitles.push_back("e#mu");
  typetitles.push_back("#mu#mu");

  //Prepare for plots -- signal titles
  vector <string> signal_titles;
  signal_titles.push_back("T1tttt (1.2, 0.8)");
  signal_titles.push_back("T1tttt (1.5, 0.1)");
  //signal_titles.push_back("t5qqww (1.2, 1.0, 0.8)");
  //signal_titles.push_back("t5qqww (1.0, 0.315, 0.3)");

  string postfix = "";
  if (testClassSF) postfix = "_sf";


  //Make plots -- sample
  dataMCplotMaker(data_graphs.hh, background_high, titles, "H-H", "", Form("--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName yield_plots/high_yields%s --noDivisionLabel --xAxisLabel   --energy 13 --lumi 209.5 --lumiUnit pb --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear --xAxisVerticalBinLabels --xAxisBinLabels 1HH,2HH,3HH,4HH,5HH,6HH,7HH,8HH,9HH,10HH,11HH,12HH,13HH,14HH,15HH,16HH,17HH,18HH,19HH,20HH,21HH,22HH,23HH,24HH,25HH,26HH,27HH,28HH,29HH,30HH,31HH,32HH", postfix.c_str())); 
  dataMCplotMaker(data_graphs.hl, background_hl  , titles, "H-L", "", Form("--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName yield_plots//hl_yields%s --noDivisionLabel --xAxisLabel --energy 13 --lumi 209.5 --lumiUnit pb --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear --xAxisVerticalBinLabels --xAxisBinLabels 1HL,2HL,3HL,4HL,5HL,6HL,7HL,8HL,9HL,10HL,11HL,12HL,13HL,14HL,15HL,16HL,17HL,18HL,19HL,20HL,21HL,22HL,23HL,24HL,25HL,26HL", postfix.c_str())); 
  dataMCplotMaker(null, signal_high, signal_titles, "H-H", "", Form("--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName yield_plots//high_yields_s%s --noDivisionLabel --xAxisLabel    --energy 13 --lumi 209.5 --lumiUnit pb --legendRight -0.12 --noXaxisUnit  --legendTextSize 0.0325 --noStack --nDivisions 210 --isLinear --xAxisVerticalBinLabels --xAxisBinLabels 1HH,2HH,3HH,4HH,5HH,6HH,7HH,8HH,9HH,10HH,11HH,12HH,13HH,14HH,15HH,16HH,17HH,18HH,19HH,20HH,21HH,22HH,23HH,24HH,25HH,26HH,27HH,28HH,29HH,30HH,31HH,32HH", postfix.c_str())); 
  dataMCplotMaker(null, signal_hl  , signal_titles, "H-L", "", Form("--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName yield_plots//hl_yields_s%s --noDivisionLabel --xAxisLabel  --energy 13 --lumi 209.5 --lumiUnit pb --legendRight -0.12 --noXaxisUnit  --legendTextSize 0.03 --noStack  --nDivisions 210 --isLinear --xAxisVerticalBinLabels --xAxisBinLabels 1HL,2HL,3HL,4HL,5HL,6HL,7HL,8HL,9HL,10HL,11HL,12HL,13HL,14HL,15HL,16HL,17HL,18HL,19HL,20HL,21HL,22HL,23HL,24HL,25HL,26HL", postfix.c_str())); 

  //Make plots -- background
  dataMCplotMaker(null, background_type_high, typetitles, "H-H", "", Form("--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName yield_plots//high_yields_t%s --noDivisionLabel --xAxisLabel    --energy 13 --lumi 209.5 --lumiUnit pb --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear --xAxisVerticalBinLabels --xAxisBinLabels 1HH,2HH,3HH,4HH,5HH,6HH,7HH,8HH,9HH,10HH,11HH,12HH,13HH,14HH,15HH,16HH,17HH,18HH,19HH,20HH,21HH,22HH,23HH,24HH,25HH,26HH,27HH,28HH,29HH,30HH,31HH,32HH", postfix.c_str())); 
  dataMCplotMaker(null, background_type_hl  , typetitles, "H-L", "", Form("--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName yield_plots//hl_yields_t%s --noDivisionLabel --xAxisLabel  --energy 13 --lumi 209.5 --lumiUnit pb --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear --xAxisVerticalBinLabels --xAxisBinLabels 1HL,2HL,3HL,4HL,5HL,6HL,7HL,8HL,9HL,10HL,11HL,12HL,13HL,14HL,15HL,16HL,17HL,18HL,19HL,20HL,21HL,22HL,23HL,24HL,25HL,26HL", postfix.c_str())); 

//  //Make tables
//  if (!makeTables) return; 
//  int nSRs[3] = { 32, 26, 8 }; 
//  for (int j = 0; j < 3; j++){
//    CTable table;
//    table.setTable() (      "",    "",    "",   "",     "t1tttt",     "t1tttt",          "t5qqww",            "t5qqww" ) 
//                     ("  ", "ttbar", "ttw", "ttz", "wz", "(1.2, 0.8)", "(1.5, 0.1)", "(1.2, 1.0, 0.8)", "(1.0, 0.315, 0.3)" ); 
//    table.setPrecision(3); 
//    if (j == 0) table.setTitle("H-H Yields");
//    if (j == 1) table.setTitle("H-L Yields");
//    if (j == 2) table.setTitle("L-L Yields");
//    table.useTitle(); 
//    for (int i = 1; i <= nSRs[j]; i++){
//      table.setRowLabel(Form("SR%i", i), i);
//      table.setCell(getYield(ttbar_graphs, i, j), i, 0); 
//      table.setCell(getYield(ttw_graphs, i, j), i, 1); 
//      table.setCell(getYield(ttz_graphs, i, j), i, 2); 
//      table.setCell(getYield(wz_graphs, i, j), i, 3); 
//      table.setCell(getYield(t1tttt_1200_graphs, i, j), i, 4); 
//      table.setCell(getYield(t1tttt_1500_graphs, i, j), i, 5); 
//      table.setCell(getYield(t5qqww_1200_graphs, i, j), i, 6); 
//      table.setCell(getYield(t5qqww_deg_graphs, i, j), i, 7); 
//    }
//    table.print();
//  }
}
