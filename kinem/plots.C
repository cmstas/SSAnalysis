#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "SS.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../CORE/SSSelections.h"

struct plotHolder{ TH1F* met[4][5][5]; TH1F* ht[4][5][5]; TH1F* njets[4][5][5]; TH1F* nbtags[4][5][5]; TH1F* leppt[4][5][5]; TH1F* mtmin[4][5][5]; TH1F* mll[4][5][5]; TH1F* mht[4][5][5]; };

struct doublePlotHolder{ plotHolder OS; plotHolder SS; };

doublePlotHolder plotMaker(TChain *chain, bool isData, bool isPhys14){

  doublePlotHolder plots;
  for (int i = 0; i < 4; i++){     //i - b-tag region
    for (int j = 0; j < 3; j++){   //j - HH, HL, LL
      for (int k = 0; k < 5; k++){ //k - ee, em, mm, ee+mm, total
        plots.SS.met[i][j][k]    = new TH1F(Form("hist_ss_met_%i_%i_%i", i, j, k), "hist", 50, 0, 700); 
        plots.SS.ht[i][j][k]     = new TH1F(Form("hist_ss_ht_%i_%i_%i", i, j, k), "hist", 20, 0, 800); 
        plots.SS.njets[i][j][k]  = new TH1F(Form("hist_ss_njets_%i_%i_%i", i, j, k), "hist", 10, 0, 10); 
        plots.SS.nbtags[i][j][k] = new TH1F(Form("hist_ss_nbtags_%i_%i_%i", i, j, k), "hist", 5, 0, 5); 
        plots.SS.leppt[i][j][k]  = new TH1F(Form("hist_ss_leppt_%i_%i_%i", i, j, k), "hist", 30, 0, 300); 
        plots.SS.mtmin[i][j][k]  = new TH1F(Form("hist_ss_mtmin_%i_%i_%i", i, j, k), "hist", 25, 0, 200); 
        plots.SS.mll[i][j][k]    = new TH1F(Form("hist_ss_mll_%i_%i_%i", i, j, k), "hist", 40, 0, 400); 
        plots.SS.mht[i][j][k]    = new TH1F(Form("hist_ss_mht_%i_%i_%i", i, j, k), "hist", 25, 0, 300); 
        plots.OS.met[i][j][k]    = new TH1F(Form("hist_os_met_%i_%i_%i", i, j, k), "hist", 50, 0, 700); 
        plots.OS.ht[i][j][k]     = new TH1F(Form("hist_os_ht_%i_%i_%i", i, j, k), "hist", 25, 0, 800); 
        plots.OS.njets[i][j][k]  = new TH1F(Form("hist_os_njets_%i_%i_%i", i, j, k), "hist", 10, 0, 10); 
        plots.OS.nbtags[i][j][k] = new TH1F(Form("hist_os_nbtags_%i_%i_%i", i, j, k), "hist", 5, 0, 5); 
        plots.OS.leppt[i][j][k]  = new TH1F(Form("hist_os_leppt_%i_%i_%i", i, j, k), "hist", 30, 0, 300); 
        plots.OS.mtmin[i][j][k]  = new TH1F(Form("hist_os_mtmin_%i_%i_%i", i, j, k), "hist", 50, 0, 200); 
        plots.OS.mll[i][j][k]    = new TH1F(Form("hist_os_mll_%i_%i_%i", i, j, k), "hist", 40, 0, 400); 
        plots.OS.mht[i][j][k]    = new TH1F(Form("hist_os_mht_%i_%i_%i", i, j, k), "hist", 50, 0, 300); 
      }
    }
  }

  //Weight
  float weight = 0;

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

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

      //Get event weight
      weight = isData ? 1 : ss::scale1fb()*0.04024;

      //Reject not triggered
      if (!isPhys14 && !ss::fired_trigger()) continue;

      //Throw away non OS-numerator events
      if (ss::hyp_class() != 4 && ss::hyp_class() != 3 && ss::hyp_class() != 2) continue;

      //Determine analysis category
      int AC = -1;
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == HighHigh) AC = 0; 
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == HighLow ) AC = 1; 
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == LowLow  ) AC = 2; 
      if (AC < 0) continue;
      AC = 0; 

      //Determine BR
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_p4().pt(), ss::lep2_p4().pt()); 
      if (BR < 0) continue;
      BR = 0;

      //Determine ee, em, mm
      int type = -1;
      if (ss::hyp_type() == 0) type = 0; 
      else if (ss::hyp_type() == 3) type = 2; 
      else type = 1; 
      
      //Determine mht
      LorentzVector mht;
      for (unsigned int i = 0; i < ss::jets().size(); i++) mht += ss::jets().at(i);  

      //Fill MET, HT
      if (ss::hyp_class() == 4){
        plots.OS.met[BR][AC][type]   ->Fill(ss::met()                          , weight); 
        plots.OS.ht[BR][AC][type]    ->Fill(ss::ht()                           , weight); 
        plots.OS.njets[BR][AC][type] ->Fill(ss::njets()                        , weight); 
        plots.OS.nbtags[BR][AC][type]->Fill(ss::nbtags()                       , weight); 
        plots.OS.leppt[BR][AC][type] ->Fill(ss::lep1_p4().pt()                 , weight); 
        plots.OS.leppt[BR][AC][type] ->Fill(ss::lep2_p4().pt()                 , weight); 
        plots.OS.mtmin[BR][AC][type] ->Fill(ss::mtmin()                        , weight); 
        plots.OS.mll[BR][AC][type]   ->Fill((ss::lep1_p4() + ss::lep2_p4()).M(), weight);
        plots.OS.mht[BR][AC][type]   ->Fill(mht.pt()                           , weight);
      }
      else { 
        plots.SS.met[BR][AC][type]   ->Fill(ss::met()                          , weight); 
        plots.SS.ht[BR][AC][type]    ->Fill(ss::ht()                           , weight); 
        plots.SS.njets[BR][AC][type] ->Fill(ss::njets()                        , weight); 
        plots.SS.nbtags[BR][AC][type]->Fill(ss::nbtags()                       , weight); 
        plots.SS.leppt[BR][AC][type] ->Fill(ss::lep1_p4().pt()                 , weight); 
        plots.SS.leppt[BR][AC][type] ->Fill(ss::lep2_p4().pt()                 , weight); 
        plots.SS.mtmin[BR][AC][type] ->Fill(ss::mtmin()                        , weight); 
        plots.SS.mll[BR][AC][type]   ->Fill((ss::lep1_p4() + ss::lep2_p4()).M(), weight);
        plots.SS.mht[BR][AC][type]   ->Fill(mht.pt()                           , weight);
      }

    }//event loop
  }//file loop

  for (int i = 0; i < 4; i++){     
    for (int j = 0; j < 3; j++){   
      //k = 3 (ee+mm)
      plots.SS.met[i][j][3]    ->Add( plots.SS.met[i][j][0]    , plots.SS.met[i][j][2]   );
      plots.SS.ht[i][j][3]     ->Add( plots.SS.ht[i][j][0]     , plots.SS.ht[i][j][2]    );
      plots.SS.njets[i][j][3]  ->Add( plots.SS.njets[i][j][0]  , plots.SS.njets[i][j][2] );
      plots.SS.nbtags[i][j][3] ->Add( plots.SS.nbtags[i][j][0] , plots.SS.nbtags[i][j][2]);
      plots.SS.leppt[i][j][3]  ->Add( plots.SS.leppt[i][j][0]  , plots.SS.leppt[i][j][2] );
      plots.SS.mtmin[i][j][3]  ->Add( plots.SS.mtmin[i][j][0]  , plots.SS.mtmin[i][j][2] );
      plots.SS.mll[i][j][3]    ->Add( plots.SS.mll[i][j][0]    , plots.SS.mll[i][j][2]   );
      plots.SS.mht[i][j][3]    ->Add( plots.SS.mht[i][j][0]    , plots.SS.mht[i][j][2]   );
      plots.OS.met[i][j][3]    ->Add( plots.OS.met[i][j][0]    , plots.OS.met[i][j][2]   );
      plots.OS.ht[i][j][3]     ->Add( plots.OS.ht[i][j][0]     , plots.OS.ht[i][j][2]    );
      plots.OS.njets[i][j][3]  ->Add( plots.OS.njets[i][j][0]  , plots.OS.njets[i][j][2] );
      plots.OS.nbtags[i][j][3] ->Add( plots.OS.nbtags[i][j][0] , plots.OS.nbtags[i][j][2]);
      plots.OS.leppt[i][j][3]  ->Add( plots.OS.leppt[i][j][0]  , plots.OS.leppt[i][j][2] );
      plots.OS.mtmin[i][j][3]  ->Add( plots.OS.mtmin[i][j][0]  , plots.OS.mtmin[i][j][2] );
      plots.OS.mll[i][j][3]    ->Add( plots.OS.mll[i][j][0]    , plots.OS.mll[i][j][2]   );
      plots.OS.mht[i][j][3]    ->Add( plots.OS.mht[i][j][0]    , plots.OS.mht[i][j][2]   );
      //k = 5 (ee+em+mm)
      plots.SS.met[i][j][4]    ->Add( plots.SS.met[i][j][3]    , plots.SS.met[i][j][1]   );
      plots.SS.ht[i][j][4]     ->Add( plots.SS.ht[i][j][3]     , plots.SS.ht[i][j][1]    );
      plots.SS.njets[i][j][4]  ->Add( plots.SS.njets[i][j][3]  , plots.SS.njets[i][j][1] );
      plots.SS.nbtags[i][j][4] ->Add( plots.SS.nbtags[i][j][3] , plots.SS.nbtags[i][j][1]);
      plots.SS.leppt[i][j][4]  ->Add( plots.SS.leppt[i][j][3]  , plots.SS.leppt[i][j][1] );
      plots.SS.mtmin[i][j][4]  ->Add( plots.SS.mtmin[i][j][3]  , plots.SS.mtmin[i][j][1] );
      plots.SS.mll[i][j][4]    ->Add( plots.SS.mll[i][j][3]    , plots.SS.mll[i][j][1]   );
      plots.SS.mht[i][j][4]    ->Add( plots.SS.mht[i][j][3]    , plots.SS.mht[i][j][1]   );
      plots.OS.met[i][j][4]    ->Add( plots.OS.met[i][j][3]    , plots.OS.met[i][j][1]   );
      plots.OS.ht[i][j][4]     ->Add( plots.OS.ht[i][j][3]     , plots.OS.ht[i][j][1]    );
      plots.OS.njets[i][j][4]  ->Add( plots.OS.njets[i][j][3]  , plots.OS.njets[i][j][1] );
      plots.OS.nbtags[i][j][4] ->Add( plots.OS.nbtags[i][j][3] , plots.OS.nbtags[i][j][1]);
      plots.OS.leppt[i][j][4]  ->Add( plots.OS.leppt[i][j][3]  , plots.OS.leppt[i][j][1] );
      plots.OS.mtmin[i][j][4]  ->Add( plots.OS.mtmin[i][j][3]  , plots.OS.mtmin[i][j][1] );
      plots.OS.mll[i][j][4]    ->Add( plots.OS.mll[i][j][3]    , plots.OS.mll[i][j][1]   );
      plots.OS.mht[i][j][4]    ->Add( plots.OS.mht[i][j][3]    , plots.OS.mht[i][j][1]   );
    }
  }

  return plots;

}

void plots(){

  //Set up chains
  TChain *chain_data = new TChain("t");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/v2.05/DataDoubleEG_0.root");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/v2.05/DataDoubleMuon_0.root");

  TChain *chain_ttW = new TChain("t"); 
  chain_ttW->Add("/nfs-7/userdata/ss2015/ssBabies/v1.27/TTW_0.root");

  TChain *chain_ttZ = new TChain("t"); 
  chain_ttZ->Add("/nfs-7/userdata/ss2015/ssBabies/v1.27/TTZ_0.root");

  TChain *chain_ttbar = new TChain("t"); 
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/v2.06/TTBAR_0.root");

  TChain *chain_WZ = new TChain("t"); 
  chain_WZ->Add("/nfs-7/userdata/ss2015/ssBabies/v2.06/WZ_0.root");

  TChain *chain_DY = new TChain("t"); 
  chain_DY->Add("/nfs-7/userdata/ss2015/ssBabies/v2.06/DY_high_0.root");
  chain_DY->Add("/nfs-7/userdata/ss2015/ssBabies/v2.06/DY_low_0.root");

  //Make individual plots
  doublePlotHolder data = plotMaker(chain_data, 1, 0);   
  doublePlotHolder ttw = plotMaker(chain_ttW, 0, 1);   
  doublePlotHolder ttz = plotMaker(chain_ttZ, 0, 1);   
  doublePlotHolder wz = plotMaker(chain_WZ, 0, 0);
  doublePlotHolder ttbar = plotMaker(chain_ttbar, 0, 0);
  doublePlotHolder dy = plotMaker(chain_DY, 0, 0);

  //Make vectors for plots
  vector <TH1F*> bkgd_met_OS;
  bkgd_met_OS.push_back(ttw.OS.met[0][0][4]);
  bkgd_met_OS.push_back(ttz.OS.met[0][0][4]); 
  bkgd_met_OS.push_back(wz.OS.met[0][0][4]); 
  bkgd_met_OS.push_back(ttbar.OS.met[0][0][4]); 
  bkgd_met_OS.push_back(dy.OS.met[0][0][4]); 
  vector <TH1F*> bkgd_ht_OS;
  bkgd_ht_OS.push_back(ttw.OS.ht[0][0][4]); 
  bkgd_ht_OS.push_back(ttz.OS.ht[0][0][4]); 
  bkgd_ht_OS.push_back(wz.OS.ht[0][0][4]); 
  bkgd_ht_OS.push_back(ttbar.OS.ht[0][0][4]); 
  bkgd_ht_OS.push_back(dy.OS.ht[0][0][4]); 
  vector <TH1F*> bkgd_njets_OS;
  bkgd_njets_OS.push_back(ttw.OS.njets[0][0][4]); 
  bkgd_njets_OS.push_back(ttz.OS.njets[0][0][4]); 
  bkgd_njets_OS.push_back(wz.OS.njets[0][0][4]); 
  bkgd_njets_OS.push_back(ttbar.OS.njets[0][0][4]); 
  bkgd_njets_OS.push_back(dy.OS.njets[0][0][4]); 
  vector <TH1F*> bkgd_nbtags_OS;
  bkgd_nbtags_OS.push_back(ttw.OS.nbtags[0][0][4]); 
  bkgd_nbtags_OS.push_back(ttz.OS.nbtags[0][0][4]); 
  bkgd_nbtags_OS.push_back(wz.OS.nbtags[0][0][4]); 
  bkgd_nbtags_OS.push_back(ttbar.OS.nbtags[0][0][4]); 
  bkgd_nbtags_OS.push_back(dy.OS.nbtags[0][0][4]); 
  vector <TH1F*> bkgd_leppt_OS;
  bkgd_leppt_OS.push_back(ttw.OS.leppt[0][0][4]); 
  bkgd_leppt_OS.push_back(ttz.OS.leppt[0][0][4]); 
  bkgd_leppt_OS.push_back(wz.OS.leppt[0][0][4]); 
  bkgd_leppt_OS.push_back(ttbar.OS.leppt[0][0][4]); 
  bkgd_leppt_OS.push_back(dy.OS.leppt[0][0][4]); 
  vector <TH1F*> bkgd_mtmin_OS;
  bkgd_mtmin_OS.push_back(ttw.OS.mtmin[0][0][4]); 
  bkgd_mtmin_OS.push_back(ttz.OS.mtmin[0][0][4]); 
  bkgd_mtmin_OS.push_back(wz.OS.mtmin[0][0][4]); 
  bkgd_mtmin_OS.push_back(ttbar.OS.mtmin[0][0][4]); 
  bkgd_mtmin_OS.push_back(dy.OS.mtmin[0][0][4]); 
  vector <TH1F*> bkgd_mll_OS;
  bkgd_mll_OS.push_back(ttw.OS.mll[0][0][4]); 
  bkgd_mll_OS.push_back(ttz.OS.mll[0][0][4]); 
  bkgd_mll_OS.push_back(wz.OS.mll[0][0][4]); 
  bkgd_mll_OS.push_back(ttbar.OS.mll[0][0][4]); 
  bkgd_mll_OS.push_back(dy.OS.mll[0][0][4]); 
  vector <TH1F*> bkgd_mht_OS;
  bkgd_mht_OS.push_back(ttw.OS.mht[0][0][4]); 
  bkgd_mht_OS.push_back(ttz.OS.mht[0][0][4]); 
  bkgd_mht_OS.push_back(wz.OS.mht[0][0][4]); 
  bkgd_mht_OS.push_back(ttbar.OS.mht[0][0][4]); 
  bkgd_mht_OS.push_back(dy.OS.mht[0][0][4]); 

  //And ditto for SS plots
  vector <TH1F*> bkgd_met_SS;
  bkgd_met_SS.push_back(ttw.SS.met[0][0][4]); 
  bkgd_met_SS.push_back(ttz.SS.met[0][0][4]); 
  bkgd_met_SS.push_back(wz.SS.met[0][0][4]); 
  bkgd_met_SS.push_back(ttbar.SS.met[0][0][4]); 
  bkgd_met_SS.push_back(dy.SS.met[0][0][4]); 
  vector <TH1F*> bkgd_ht_SS;
  bkgd_ht_SS.push_back(ttw.SS.ht[0][0][4]); 
  bkgd_ht_SS.push_back(ttz.SS.ht[0][0][4]); 
  bkgd_ht_SS.push_back(wz.SS.ht[0][0][4]); 
  bkgd_ht_SS.push_back(ttbar.SS.ht[0][0][4]); 
  bkgd_ht_SS.push_back(dy.SS.ht[0][0][4]); 
  vector <TH1F*> bkgd_njets_SS;
  bkgd_njets_SS.push_back(ttw.SS.njets[0][0][4]); 
  bkgd_njets_SS.push_back(ttz.SS.njets[0][0][4]); 
  bkgd_njets_SS.push_back(wz.SS.njets[0][0][4]); 
  bkgd_njets_SS.push_back(ttbar.SS.njets[0][0][4]); 
  bkgd_njets_SS.push_back(dy.SS.njets[0][0][4]); 
  vector <TH1F*> bkgd_nbtags_SS;
  bkgd_nbtags_SS.push_back(ttw.SS.nbtags[0][0][4]); 
  bkgd_nbtags_SS.push_back(ttz.SS.nbtags[0][0][4]); 
  bkgd_nbtags_SS.push_back(wz.SS.nbtags[0][0][4]); 
  bkgd_nbtags_SS.push_back(ttbar.SS.nbtags[0][0][4]); 
  bkgd_nbtags_SS.push_back(dy.SS.nbtags[0][0][4]); 
  vector <TH1F*> bkgd_leppt_SS;
  bkgd_leppt_SS.push_back(ttw.SS.leppt[0][0][4]); 
  bkgd_leppt_SS.push_back(ttz.SS.leppt[0][0][4]); 
  bkgd_leppt_SS.push_back(wz.SS.leppt[0][0][4]); 
  bkgd_leppt_SS.push_back(ttbar.SS.leppt[0][0][4]); 
  bkgd_leppt_SS.push_back(dy.SS.leppt[0][0][4]); 
  vector <TH1F*> bkgd_mtmin_SS;
  bkgd_mtmin_SS.push_back(ttw.SS.mtmin[0][0][4]); 
  bkgd_mtmin_SS.push_back(ttz.SS.mtmin[0][0][4]); 
  bkgd_mtmin_SS.push_back(wz.SS.mtmin[0][0][4]); 
  bkgd_mtmin_SS.push_back(ttbar.SS.mtmin[0][0][4]); 
  bkgd_mtmin_SS.push_back(dy.SS.mtmin[0][0][4]); 
  vector <TH1F*> bkgd_mll_SS;
  bkgd_mll_SS.push_back(ttw.SS.mll[0][0][4]); 
  bkgd_mll_SS.push_back(ttz.SS.mll[0][0][4]); 
  bkgd_mll_SS.push_back(wz.SS.mll[0][0][4]); 
  bkgd_mll_SS.push_back(ttbar.SS.mll[0][0][4]); 
  bkgd_mll_SS.push_back(dy.SS.mll[0][0][4]); 
  vector <TH1F*> bkgd_mht_SS;
  bkgd_mht_SS.push_back(ttw.SS.mht[0][0][4]); 
  bkgd_mht_SS.push_back(ttz.SS.mht[0][0][4]); 
  bkgd_mht_SS.push_back(wz.SS.mht[0][0][4]); 
  bkgd_mht_SS.push_back(ttbar.SS.mht[0][0][4]); 
  bkgd_mht_SS.push_back(dy.SS.mht[0][0][4]); 

  //Titles for plots
  vector <string> titles;
  titles.push_back("ttW"); 
  titles.push_back("ttZ"); 
  titles.push_back("WZ"); 
  titles.push_back("ttbar"); 
  titles.push_back("DY"); 

  //Make plots
  dataMCplotMaker(data.OS.met[0][0][4], bkgd_met_OS, titles, "MET (OS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName MET_OS.pdf --xAxisLabel MET --percentageInBox --setMinimum .001 --legendUp -.05"); 
//  dataMCplotMaker(data.OS.ht[0][0], bkgd_ht_OS, titles, "HT (OS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName HT_OS.pdf --xAxisLabel HT --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 50000"); 
//  dataMCplotMaker(data.OS.njets[0][0], bkgd_njets_OS, titles, "nJets (OS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName nJets_OS.pdf --xAxisLabel nJets --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 50000"); 
//  dataMCplotMaker(data.OS.nbtags[0][0], bkgd_nbtags_OS, titles, "nBtags (OS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName nBtags_OS.pdf --xAxisLabel nBtags --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 500000 --nDivisions 5");
//  dataMCplotMaker(data.OS.mtmin[0][0], bkgd_mtmin_OS, titles, "mtmin (OS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName mtmin_OS.pdf --xAxisLabel mt min --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 100000 --nDivisions 5"); 
//  dataMCplotMaker(data.OS.leppt[0][0], bkgd_leppt_OS, titles, "lep p_{T} (OS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName leppt_OS.pdf --xAxisLabel lep p_{T} --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 100000 --nDivisions 5"); 
//  dataMCplotMaker(data.OS.mll[0][0], bkgd_mll_OS, titles, "lep m_{ll} (OS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName mll_OS.pdf --xAxisLabel m_{ll} --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 100000"); 
//  dataMCplotMaker(data.OS.mht[0][0], bkgd_mht_OS, titles, "MHT (OS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName mht_OS.pdf --xAxisLabel MHT --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 100000"); 
//  dataMCplotMaker(data.SS.met[0][0], bkgd_met_SS, titles, "MET (SS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName MET.pdf --xAxisLabel MET --percentageInBox --setMinimum .001 --legendUp -.05"); 
//  dataMCplotMaker(data.SS.ht[0][0], bkgd_ht_SS, titles, "HT (SS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName HT.pdf --xAxisLabel HT --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 50000"); 
//  dataMCplotMaker(data.SS.njets[0][0], bkgd_njets_SS, titles, "nJets (SS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName nJets.pdf --xAxisLabel nJets --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 50000"); 
//  dataMCplotMaker(data.SS.nbtags[0][0], bkgd_nbtags_SS, titles, "nBtags (SS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName nBtags.pdf --xAxisLabel nBtags --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 500000 --nDivisions 5");
//  dataMCplotMaker(data.SS.mtmin[0][0], bkgd_mtmin_SS, titles, "mtmin (SS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName mtmin.pdf --xAxisLabel mt min --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 100000 --nDivisions 5"); 
//  dataMCplotMaker(data.SS.leppt[0][0], bkgd_leppt_SS, titles, "lep p_{T} (SS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName leppt.pdf --xAxisLabel lep p_{T} --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 100000 --nDivisions 5"); 
//  dataMCplotMaker(data.SS.mll[0][0], bkgd_mll_SS, titles, "lep m_{ll} (SS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName mll.pdf --xAxisLabel m_{ll} --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 100000"); 
//  dataMCplotMaker(data.SS.mht[0][0], bkgd_mht_SS, titles, "MHT (SS)", "Run2, HH,HL,LL, BR0-3", "--lumi 40.24 --lumiUnit pb --outputName mht.pdf --xAxisLabel MHT --percentageInBox --setMinimum .001 --legendUp -.05 --setMaximum 100000"); 

}
