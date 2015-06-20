#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "SS.h"

//Tables on/off
bool makeTables = 1;

//Root files on/off
bool makeRootFiles = 0;

//Lumi
float lumi = 10.0;

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

      //Reject non-SS
      if (ss::hyp_class() != 3) continue;

      //Flavor selection
      if (flavor == MM && ss::hyp_type() != 3) continue;
      if (flavor == EM && ss::hyp_type() != 1 && ss::hyp_type() != 2) continue;
      if (flavor == EE && ss::hyp_type() != 0) continue;
   
      //Calculate mtmin
      float mtmin = ss::mt() > ss::mt_l2() ? ss::mt_l2() : ss::mt(); 
 
      //Figure out region, fill plot
      anal_type_t categ = analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt());  
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_p4().pt(), ss::lep2_p4().pt());
      if (SR > 0 && categ == HighHigh) HighHighPlot->Fill(SR, ss::scale1fb()*lumi);
      if (SR > 0 && categ == HighLow)  HighLowPlot ->Fill(SR, ss::scale1fb()*lumi);
      if (SR > 0 && categ == LowLow)   LowLowPlot  ->Fill(SR, ss::scale1fb()*lumi);

    }//event loop
  }//file loop

  //Make root files
  if (makeRootFiles && flavor==UNASSIGNED) {
    TFile *fileOut = TFile::Open("../cards/v1.16/"+TString(name)+"_histos.root","RECREATE");
    TH1F* hyp_hihi   = (TH1F*) HighHighPlot->Clone("hyp_hihi");
    TH1F* hyp_hilow  = (TH1F*) HighLowPlot->Clone("hyp_hilow");
    TH1F* hyp_lowlow = (TH1F*) LowLowPlot->Clone("hyp_lowlow");
    hyp_hihi->Write();
    hyp_hilow->Write();
    hyp_lowlow->Write();
    fileOut->Close();
  }
 
  //Return hists
  results_t result; 
  result.hh = HighHighPlot;
  result.hl = HighLowPlot;
  result.ll = LowLowPlot;
  return result;

}

void yields(){

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
  ttbar      ->Add("/nfs-7/userdata/ss2015/ssBabies/v1.12/TTBAR_multiIso.root"                                 );
  ttw        ->Add("/nfs-7/userdata/ss2015/ssBabies/v1.12/TTW_multiIso.root"                                   );
  ttz        ->Add("/nfs-7/userdata/ss2015/ssBabies/v1.12/TTZ_multiIso.root"                                   );
  wz         ->Add("/nfs-7/userdata/ss2015/ssBabies/v1.12/WZ_multiIso.root"                                    );
  t1tttt_1200->Add("/nfs-7/userdata/ss2015/ssBabies/v1.12/T1TTTT_1200_multiIso.root"                           );
  t1tttt_1500->Add("/nfs-7/userdata/ss2015/ssBabies/v1.12/T1TTTT_1500_multiIso.root"                           );
  t5qqww_1200->Add("/nfs-7/userdata/ss2015/ssBabies/v1.12/private/t5qqqqWW_1200_1000_800_baby_multiIso.root"   );
  t5qqww_deg ->Add("/nfs-7/userdata/ss2015/ssBabies/v1.12/private/t5qqqqWW_deg_1000_315_300_baby_multiIso.root");

  //Chains for type
  TChain* all_bkgd = new TChain("t");
  all_bkgd->Add(ttbar);
  all_bkgd->Add(ttw);
  all_bkgd->Add(ttz);
  all_bkgd->Add(wz);

  //Make the signal histograms
  results_t ttbar_graphs       = run(ttbar      , "ttbar"      );
  results_t ttw_graphs         = run(ttw        , "ttw"        );
  results_t ttz_graphs         = run(ttz        , "ttz"        );
  results_t wz_graphs          = run(wz         , "wz"         );
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
  vector <TH1F*> background_hl; 
  background_hl.push_back(ttbar_graphs.hl);
  background_hl.push_back(ttw_graphs.hl);
  background_hl.push_back(ttz_graphs.hl);
  background_hl.push_back(wz_graphs.hl);
  vector <TH1F*> background_low; 
  background_low.push_back(ttbar_graphs.ll);
  background_low.push_back(ttw_graphs.ll);
  background_low.push_back(ttz_graphs.ll);
  background_low.push_back(wz_graphs.ll);

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
  signal_high.push_back(t5qqww_1200_graphs.hh);
  signal_high.push_back(t5qqww_deg_graphs.hh);
  vector <TH1F*> signal_hl; 
  signal_hl.push_back(t1tttt_1200_graphs.hl);
  signal_hl.push_back(t1tttt_1500_graphs.hl);
  signal_hl.push_back(t5qqww_1200_graphs.hl);
  signal_hl.push_back(t5qqww_deg_graphs.hl);
  vector <TH1F*> signal_low; 
  signal_low.push_back(t1tttt_1200_graphs.ll);
  signal_low.push_back(t1tttt_1500_graphs.ll);
  signal_low.push_back(t5qqww_1200_graphs.ll);
  signal_low.push_back(t5qqww_deg_graphs.ll);

  //Prepare for plots -- background titles
  vector <string> titles;
  titles.push_back("ttbar");
  titles.push_back("ttw");
  titles.push_back("ttz");
  titles.push_back("wz");

  //Prepare for plots -- background titles (type)
  vector <string> typetitles;
  typetitles.push_back("ee");
  typetitles.push_back("e#mu");
  typetitles.push_back("#mu#mu");

  //Prepare for plots -- signal titles
  vector <string> signal_titles;
  signal_titles.push_back("t1tttt (1.2, 0.8)");
  signal_titles.push_back("t1tttt (1.5, 0.1)");
  signal_titles.push_back("t5qqww (1.2, 1.0, 0.8)");
  signal_titles.push_back("t5qqww (1.0, 0.315, 0.3)");

  //Make plots -- sample
  dataMCplotMaker(null, background_high, titles, "H-H", "", "--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear"); 
  dataMCplotMaker(null, background_hl  , titles, "H-L", "multiIso", "--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName   hl_yields --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear"); 
  dataMCplotMaker(null, signal_high, signal_titles, "H-H", "", "--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields_s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --legendRight -0.12 --noXaxisUnit  --legendTextSize 0.0325 --noStack --nDivisions 210 --isLinear"); 
  dataMCplotMaker(null, signal_hl  , signal_titles, "H-L", "", "--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName   hl_yields_s --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --legendRight -0.12 --noXaxisUnit  --legendTextSize 0.03 --noStack  --nDivisions 210 --isLinear"); 

  //Make plots -- type
  dataMCplotMaker(null, background_type_high, typetitles, "H-H", "multiIso", "--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields_t --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear"); 
  dataMCplotMaker(null, background_type_hl  , typetitles, "H-L", "multiIso", "--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName hl_yields_t --noDivisionLabel --xAxisLabel SR --energy 13 --lumi 10 --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear"); 

  //Make tables
  if (!makeTables) return; 
  int nSRs[3] = { 32, 26, 8 }; 
  for (int j = 0; j < 3; j++){
    CTable table;
    table.setTable() (      "",    "",    "",   "",     "t1tttt",     "t1tttt",          "t5qqww",            "t5qqww" ) 
                     ("  ", "ttbar", "ttw", "ttz", "wz", "(1.2, 0.8)", "(1.5, 0.1)", "(1.2, 1.0, 0.8)", "(1.0, 0.315, 0.3)" ); 
    table.setPrecision(3); 
    if (j == 0) table.setTitle("H-H Yields");
    if (j == 1) table.setTitle("H-L Yields");
    if (j == 2) table.setTitle("L-L Yields");
    table.useTitle(); 
    for (int i = 1; i <= nSRs[j]; i++){
      table.setRowLabel(Form("SR%i", i), i);
      table.setCell(getYield(ttbar_graphs, i, j), i, 0); 
      table.setCell(getYield(ttw_graphs, i, j), i, 1); 
      table.setCell(getYield(ttz_graphs, i, j), i, 2); 
      table.setCell(getYield(wz_graphs, i, j), i, 3); 
      table.setCell(getYield(t1tttt_1200_graphs, i, j), i, 4); 
      table.setCell(getYield(t1tttt_1500_graphs, i, j), i, 5); 
      table.setCell(getYield(t5qqww_1200_graphs, i, j), i, 6); 
      table.setCell(getYield(t5qqww_deg_graphs, i, j), i, 7); 
    }
    table.print();
  }
}
