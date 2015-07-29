#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "SS.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../CORE/SSSelections.h"

struct doublePlotHolder{ TH1F* OS[8][4][3][5]; TH1F* SS[8][4][3][5]; };

doublePlotHolder plotMaker(TChain *chain, bool isData, bool isPhys14){

  int      max[8] = { 700, 800, 10, 5, 300, 200, 400, 300 };
  int nBins_OS[8] = { 50, 25, 10, 5, 30, 50, 40, 50 };
  int nBins_SS[8] = { 50, 20, 10, 5, 30, 25, 40, 25 };

  doublePlotHolder plots;
  for (int m = 0; m < 8; m++){       //m - which plots
    for (int i = 0; i < 4; i++){     //i - b-tag region
      for (int j = 0; j < 3; j++){   //j - HH, HL, LL
        for (int k = 0; k < 5; k++){ //k - ee, em, mm, ee+mm, total
          plots.SS[m][i][j][k] = new TH1F(Form("hist_ss_%i_%i_%i_%i", m, i, j, k), "hist", nBins_SS[m], 0, max[m]); 
          plots.OS[m][i][j][k] = new TH1F(Form("hist_os_%i_%i_%i_%i", m, i, j, k), "hist", nBins_OS[m], 0, max[m]); 
        }
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
        plots.OS[0][BR][AC][type]   ->Fill(ss::met()                          , weight); 
        plots.OS[1][BR][AC][type]    ->Fill(ss::ht()                           , weight); 
        plots.OS[2][BR][AC][type] ->Fill(ss::njets()                        , weight); 
        plots.OS[3][BR][AC][type]->Fill(ss::nbtags()                       , weight); 
        plots.OS[4][BR][AC][type] ->Fill(ss::lep1_p4().pt()                 , weight); 
        plots.OS[4][BR][AC][type] ->Fill(ss::lep2_p4().pt()                 , weight); 
        plots.OS[5][BR][AC][type] ->Fill(ss::mtmin()                        , weight); 
        plots.OS[6][BR][AC][type]   ->Fill((ss::lep1_p4() + ss::lep2_p4()).M(), weight);
        plots.OS[7][BR][AC][type]   ->Fill(mht.pt()                           , weight);
      }
      else { 
        plots.SS[0][BR][AC][type]   ->Fill(ss::met()                          , weight); 
        plots.SS[1][BR][AC][type]    ->Fill(ss::ht()                           , weight); 
        plots.SS[2][BR][AC][type] ->Fill(ss::njets()                        , weight); 
        plots.SS[3][BR][AC][type]->Fill(ss::nbtags()                       , weight); 
        plots.SS[4][BR][AC][type] ->Fill(ss::lep1_p4().pt()                 , weight); 
        plots.SS[4][BR][AC][type] ->Fill(ss::lep2_p4().pt()                 , weight); 
        plots.SS[5][BR][AC][type] ->Fill(ss::mtmin()                        , weight); 
        plots.SS[6][BR][AC][type]   ->Fill((ss::lep1_p4() + ss::lep2_p4()).M(), weight);
        plots.SS[7][BR][AC][type]   ->Fill(mht.pt()                           , weight);
      }

    }//event loop
  }//file loop

  for (int m = 0; m < 8; m++){
    for (int i = 0; i < 4; i++){     
      for (int j = 0; j < 3; j++){   
        plots.SS[m][i][j][3]->Add( plots.SS[m][i][j][0], plots.SS[m][i][j][2] );
        plots.OS[m][i][j][3]->Add( plots.OS[m][i][j][0], plots.OS[m][i][j][2] );
        plots.SS[m][i][j][4]->Add( plots.SS[m][i][j][3], plots.SS[m][i][j][1] );
        plots.OS[m][i][j][4]->Add( plots.OS[m][i][j][3], plots.OS[m][i][j][1] );
      }
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
  vector < vector < TH1F*> > OS_plots_em; 
  for (int i = 0; i < 8; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.OS[i][0][0][1]);
    temp.push_back(ttz.OS[i][0][0][1]);
    temp.push_back(wz.OS[i][0][0][1]);
    temp.push_back(ttbar.OS[i][0][0][1]);
    temp.push_back(dy.OS[i][0][0][1]);
    OS_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_em; 
  for (int i = 0; i < 8; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.SS[i][0][0][1]);
    temp.push_back(ttz.SS[i][0][0][1]);
    temp.push_back(wz.SS[i][0][0][1]);
    temp.push_back(ttbar.SS[i][0][0][1]);
    temp.push_back(dy.SS[i][0][0][1]);
    SS_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots_eemm; 
  for (int i = 0; i < 8; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.OS[i][0][0][3]);
    temp.push_back(ttz.OS[i][0][0][3]);
    temp.push_back(wz.OS[i][0][0][3]);
    temp.push_back(ttbar.OS[i][0][0][3]);
    temp.push_back(dy.OS[i][0][0][3]);
    OS_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_eemm; 
  for (int i = 0; i < 8; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.SS[i][0][0][3]);
    temp.push_back(ttz.SS[i][0][0][3]);
    temp.push_back(wz.SS[i][0][0][3]);
    temp.push_back(ttbar.SS[i][0][0][3]);
    temp.push_back(dy.SS[i][0][0][3]);
    SS_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots; 
  for (int i = 0; i < 8; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.OS[i][0][0][4]);
    temp.push_back(ttz.OS[i][0][0][4]);
    temp.push_back(wz.OS[i][0][0][4]);
    temp.push_back(ttbar.OS[i][0][0][4]);
    temp.push_back(dy.OS[i][0][0][4]);
    OS_plots.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots; 
  for (int i = 0; i < 8; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.SS[i][0][0][4]);
    temp.push_back(ttz.SS[i][0][0][4]);
    temp.push_back(wz.SS[i][0][0][4]);
    temp.push_back(ttbar.SS[i][0][0][4]);
    temp.push_back(dy.SS[i][0][0][4]);
    SS_plots.push_back(temp); 
    temp.clear();
  }

  //Titles for legend
  vector <string> titles;
  titles.push_back("ttW"); 
  titles.push_back("ttZ"); 
  titles.push_back("WZ"); 
  titles.push_back("ttbar"); 
  titles.push_back("DY"); 

  //Titles for plots
  vector <string> names;
  names.push_back("met");
  names.push_back("ht");
  names.push_back("nJets");
  names.push_back("nBtags");
  names.push_back("lep p_{T}");
  names.push_back("mt min");
  names.push_back("m_{ll}");
  names.push_back("mht");

  //Titles for output
  vector <string> out;
  out.push_back("MET");
  out.push_back("HT");
  out.push_back("nJets");
  out.push_back("nBtags");
  out.push_back("leppt");
  out.push_back("mtmin");
  out.push_back("mll");
  out.push_back("mht");

  double max[8] = { 5e4, 5e4, 5e5, 1e5, 1e5, 1e5, 1e5, 1e4 }; 
  double min[8] = {1e-3, 1e-3, 1e-2, 1e-1, 1e-3, 1e-3, 1e-3, 1e-3 };

  std::string title = "Run2, HH,HL,LL, BR0-3";
  //std::string title = "Run2, HH,HL,LL, no kinem cuts";

  //Make plots
  for (int i = 0; i < 8; i++){
    //ee+mm
    dataMCplotMaker(data.OS[i][0][0][3], OS_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(OS ee+#mu#mu)"), title.c_str(), Form("--lumi 40.24 --lumiUnit pb --outputName %s_eemm_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][0][3], SS_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(SS ee+#mu#mu)"), title.c_str(), Form("--lumi 40.24 --lumiUnit pb --outputName %s_eemm.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
   //emu
    dataMCplotMaker(data.OS[i][0][0][1], OS_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(OS e#mu)"), title.c_str(), Form("--lumi 40.24 --lumiUnit pb --outputName %s_em_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][0][1], SS_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(SS e#mu)"), title.c_str(), Form("--lumi 40.24 --lumiUnit pb --outputName %s_em.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
   //all
    dataMCplotMaker(data.OS[i][0][0][4], OS_plots[i], titles, Form("%s %s", names[i].c_str(), "(OS)"), title.c_str(), Form("--lumi 40.24 --lumiUnit pb --outputName %s_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][0][4], SS_plots[i], titles, Form("%s %s", names[i].c_str(), "(SS)"), title.c_str(), Form("--lumi 40.24 --lumiUnit pb --outputName %s.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
  }

}
