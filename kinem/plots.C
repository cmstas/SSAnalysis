#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "SS.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../CORE/SSSelections.h"
#include "../CORE/Tools/dorky/dorky.h"

int mode = 1;  // 0-nominal, 1-upstream, 2-upstream+2jets
bool corrected = false;

struct doublePlotHolder{ TH1F* OS[10][4][4][5]; TH1F* SS[10][4][4][5]; };

doublePlotHolder plotMaker(TChain *chain, bool isData, bool isPhys14){

  int      max[10] = { 700, 800, 10, 5, 300, 200, 400, 300, 700, 700 };
  int nBins_OS[10] = { 50, 25, 10, 5, 30, 50, 40, 50, 50, 50 };
  int nBins_SS[10] = { 50, 20, 10, 5, 30, 25, 40, 25, 50, 50 };

  doublePlotHolder plots;
  for (int m = 0; m < 10; m++){       //m - which plots
    for (int i = 0; i < 4; i++){     //i - b-tag region
      for (int j = 0; j < 4; j++){   //j - HH, HL, LL, all
        for (int k = 0; k < 5; k++){ //k - ee, em, mm, ee+mm, total
          plots.SS[m][i][j][k] = new TH1F(Form("hist_ss_%i_%i_%i_%i", m, i, j, k), "hist", nBins_SS[m], 0, max[m]); 
          plots.OS[m][i][j][k] = new TH1F(Form("hist_os_%i_%i_%i_%i", m, i, j, k), "hist", nBins_OS[m], 0, max[m]); 
        }
      }
    }
  }

  //Weight
  float weight = 0;

  //Number duplicates rejected
  int reject = 0;

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
      float lumi = 0.04003;
      float weight = isData ? 1 : ss::scale1fb()*lumi;

      //Reject duplicates
	  if (isData == true){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)){ reject++; continue; }
      }

      //Reject not triggered
      if (!isPhys14 && !ss::fired_trigger()) continue;

      //Throw away non OS-numerator events
      if (ss::hyp_class() != 4 && ss::hyp_class() != 3 && ss::hyp_class() != 2) continue;

      //Determine MET
      float metAG = (corrected && !isPhys14) ? ss::corrMET() : ss::met();
      float met30 = isPhys14 ? ss::met() : ss::met3p0(); 

      //Determine analysis category
      int AC = -1;
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == HighHigh) AC = 0; 
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == HighLow ) AC = 1; 
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == LowLow  ) AC = 2; 
      if (AC < 0) continue;

      //Determine BR
      int BR = baselineRegion(ss::njets(), ss::nbtags(), metAG, ss::ht(), ss::lep1_p4().pt(), ss::lep2_p4().pt()); 
      if (mode == 0 && BR < 0) continue;
      if (mode == 2 && ss::met() < 30) continue;
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
        plots.OS[0][BR][AC][type]->Fill(metAG                               , weight); 
        plots.OS[1][BR][AC][type]->Fill(ss::ht()                            , weight); 
        plots.OS[2][BR][AC][type]->Fill(ss::njets()                         , weight); 
        plots.OS[3][BR][AC][type]->Fill(ss::nbtags()                        , weight); 
        plots.OS[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                  , weight); 
        plots.OS[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                  , weight); 
        plots.OS[5][BR][AC][type]->Fill(ss::mtmin()                         , weight); 
        plots.OS[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M() , weight);
        plots.OS[7][BR][AC][type]->Fill(mht.pt()                            , weight);
        plots.OS[8][BR][AC][type]->Fill(met30                               , weight);
      }
      else { 
        plots.SS[0][BR][AC][type]->Fill(metAG                              , weight); 
        plots.SS[1][BR][AC][type]->Fill(ss::ht()                           , weight); 
        plots.SS[2][BR][AC][type]->Fill(ss::njets()                        , weight); 
        plots.SS[3][BR][AC][type]->Fill(ss::nbtags()                       , weight); 
        plots.SS[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                 , weight); 
        plots.SS[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                 , weight); 
        plots.SS[5][BR][AC][type]->Fill(ss::mtmin()                        , weight); 
        plots.SS[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M(), weight);
        plots.SS[7][BR][AC][type]->Fill(mht.pt()                           , weight);
        plots.SS[8][BR][AC][type]->Fill(met30                              , weight);
      }

    }//event loop
  }//file loop

  //Fill ee+mm, ee+em+mm plots
  for (int m = 0; m < 10; m++){
    for (int i = 0; i < 4; i++){     
      for (int j = 0; j < 3; j++){   
        plots.SS[m][i][j][3]->Add( plots.SS[m][i][j][0], plots.SS[m][i][j][2] );
        plots.OS[m][i][j][3]->Add( plots.OS[m][i][j][0], plots.OS[m][i][j][2] );
        plots.SS[m][i][j][4]->Add( plots.SS[m][i][j][3], plots.SS[m][i][j][1] );
        plots.OS[m][i][j][4]->Add( plots.OS[m][i][j][3], plots.OS[m][i][j][1] );
      }
    }
  }

  //Fill HH+HL+LL plots
  for (int m = 0; m < 10; m++){
    for (int i = 0; i < 4; i++){     
      for (int j = 0; j < 5; j++){   
        plots.SS[m][i][3][j]->Add( plots.SS[m][i][0][j], plots.SS[m][i][1][j]);
        plots.SS[m][i][3][j]->Add( plots.SS[m][i][2][j] );
        plots.OS[m][i][3][j]->Add( plots.OS[m][i][0][j], plots.OS[m][i][1][j]);
        plots.OS[m][i][3][j]->Add( plots.OS[m][i][2][j] );
      }
    }
  }

  //Fill ee+mm, ee+em+mm plots for HH+HL+LL
  for (int m = 0; m < 10; m++){
    for (int i = 0; i < 4; i++){     
        plots.SS[m][i][3][3]->Add( plots.SS[m][i][3][0], plots.SS[m][i][3][2] );
        plots.OS[m][i][3][3]->Add( plots.OS[m][i][3][0], plots.OS[m][i][3][2] );
        plots.SS[m][i][3][4]->Add( plots.SS[m][i][3][3], plots.SS[m][i][3][1] );
        plots.OS[m][i][3][4]->Add( plots.OS[m][i][3][3], plots.OS[m][i][3][1] );
    }
  }

  if (isData) cout << "number of duplicates removed: " << reject << endl;

  return plots;

}

void plots(){

  //Set up chains
  TChain *chain_data = new TChain("t");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/v2.09/DataDoubleEG_0.root");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/v2.09/DataDoubleMuon_0.root");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/v2.09/DataMuonEG_0.root");

  TChain *chain_ttW = new TChain("t"); 
  chain_ttW->Add("/nfs-7/userdata/ss2015/ssBabies/v1.27/TTW_0.root");

  TChain *chain_ttZ = new TChain("t"); 
  chain_ttZ->Add("/nfs-7/userdata/ss2015/ssBabies/v1.27/TTZ_0.root");

  TChain *chain_ttbar = new TChain("t"); 
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/v2.09/TTBAR_0.root");

  TChain *chain_WZ = new TChain("t"); 
  chain_WZ->Add("/nfs-7/userdata/ss2015/ssBabies/v2.09/WZ_0.root");

  TChain *chain_DY = new TChain("t"); 
  chain_DY->Add("/nfs-7/userdata/ss2015/ssBabies/v2.09/DY_high_0.root");
  chain_DY->Add("/nfs-7/userdata/ss2015/ssBabies/v2.09/DY_low_0.root");

  TChain *chain_WJets = new TChain("t"); 
  chain_WJets->Add("/nfs-7/userdata/ss2015/ssBabies/v2.09/WJets_0.root");

  //Make individual plots
  doublePlotHolder data = plotMaker(chain_data, 1, 0);   
  doublePlotHolder ttw = plotMaker(chain_ttW, 0, 1);   
  doublePlotHolder ttz = plotMaker(chain_ttZ, 0, 1);   
  doublePlotHolder wz = plotMaker(chain_WZ, 0, 0);
  doublePlotHolder ttbar = plotMaker(chain_ttbar, 0, 0);
  doublePlotHolder dy = plotMaker(chain_DY, 0, 0);
  doublePlotHolder wjets = plotMaker(chain_WJets, 0, 0);

  //Make vectors for plots
  vector < vector < TH1F*> > OS_plots_em; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.OS[i][0][3][1]);
    temp.push_back(ttz.OS[i][0][3][1]);
    temp.push_back(wz.OS[i][0][3][1]);
    temp.push_back(ttbar.OS[i][0][3][1]);
    temp.push_back(dy.OS[i][0][3][1]);
    temp.push_back(wjets.OS[i][0][3][1]);
    OS_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_em; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.SS[i][0][3][1]);
    temp.push_back(ttz.SS[i][0][3][1]);
    temp.push_back(wz.SS[i][0][3][1]);
    temp.push_back(ttbar.SS[i][0][3][1]);
    temp.push_back(dy.SS[i][0][3][1]);
    temp.push_back(wjets.SS[i][0][3][1]);
    SS_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots_eemm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.OS[i][0][3][3]);
    temp.push_back(  ttz.OS[i][0][3][3]);
    temp.push_back(   wz.OS[i][0][3][3]);
    temp.push_back(ttbar.OS[i][0][3][3]);
    temp.push_back(   dy.OS[i][0][3][3]);
    temp.push_back(wjets.OS[i][0][3][3]);
    OS_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_eemm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.SS[i][0][3][3]);
    temp.push_back(  ttz.SS[i][0][3][3]);
    temp.push_back(   wz.SS[i][0][3][3]);
    temp.push_back(ttbar.SS[i][0][3][3]);
    temp.push_back(   dy.SS[i][0][3][3]);
    temp.push_back(wjets.SS[i][0][3][3]);
    SS_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.OS[i][0][3][4]);
    temp.push_back(ttz.OS[i][0][3][4]);
    temp.push_back(wz.OS[i][0][3][4]);
    temp.push_back(ttbar.OS[i][0][3][4]);
    temp.push_back(dy.OS[i][0][3][4]);
    temp.push_back(wjets.OS[i][0][3][4]);
    OS_plots.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.SS[i][0][3][4]);
    temp.push_back(ttz.SS[i][0][3][4]);
    temp.push_back(wz.SS[i][0][3][4]);
    temp.push_back(ttbar.SS[i][0][3][4]);
    temp.push_back(dy.SS[i][0][3][4]);
    temp.push_back(wjets.SS[i][0][3][4]);
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
  titles.push_back("WJets"); 

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
  names.push_back("MET3p0");

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
  out.push_back("MET3p0");

  vector <double> max_upstream   = { 5e5, 5e5, 5e6, 1e6, 1e6, 1e6, 1e6, 1e5, 5e5, 5e5 };
  vector <double> max_downstream = { 5e4, 5e4, 5e5, 1e5, 1e5, 1e5, 1e5, 1e4, 5e4, 5e4 }; 
  vector <double> max;
  if (mode == 0) max = max_downstream;
  if (mode == 1) max = max_upstream; 
  if (mode == 2) max = max_upstream;
  double min[10] = {1e-3, 1e-3, 1e-2, 1e-1, 1e-3, 1e-3, 1e-3, 1e-3, 1e-3, 1e-3 };

  std::string title;
  if (mode == 0) title = Form("Run2, HH,HL,LL, BR0-3 %s"   , corrected ? "corr" : "" );
  if (mode == 1) title = Form("Run2, HH,HL,LL, upstream %s", corrected ? "corr" : "" );
  if (mode == 2) title = Form("Run2, HH,HL,LL, MET > 30 %s", corrected ? "corr" : "" );

  //Make plots
  for (int i = 0; i < 10; i++){
    //ee+mm
    dataMCplotMaker(data.OS[i][0][3][3], OS_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(OS ee+#mu#mu)"), title.c_str(), Form("--lumi 40.03 --lumiUnit pb --outputName %s_eemm_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][3], SS_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(SS ee+#mu#mu)"), title.c_str(), Form("--lumi 40.03 --lumiUnit pb --outputName %s_eemm.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
   //emu
    dataMCplotMaker(data.OS[i][0][3][1], OS_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(OS e#mu)"), title.c_str(), Form("--lumi 40.03 --lumiUnit pb --outputName %s_em_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][1], SS_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(SS e#mu)"), title.c_str(), Form("--lumi 40.03 --lumiUnit pb --outputName %s_em.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
   //all
    dataMCplotMaker(data.OS[i][0][3][4], OS_plots[i], titles, Form("%s %s", names[i].c_str(), "(OS)"), title.c_str(), Form("--lumi 40.03 --lumiUnit pb --outputName %s_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][4], SS_plots[i], titles, Form("%s %s", names[i].c_str(), "(SS)"), title.c_str(), Form("--lumi 40.03 --lumiUnit pb --outputName %s.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
  }

}