#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "SS.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../CORE/SSSelections.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"

int mode = 1;  // 0-nominal, 1-upstream, 2-upstream+2jets
bool corrected = true;

struct doublePlotHolder{ TH1F* OS[10][4][4][5]; TH1F* SS[10][4][4][5]; TH1F* SF[10][4][4][5]; TH1F* EZ[10][4][4][5]; };

doublePlotHolder plotMaker(TChain *chain, bool isData){

  int      max[10] = { 700, 800, 10, 5, 300, 200, 400, 50, 700, 700 };
  int nBins_OS[10] = { 50, 25, 10, 5, 30, 50, 40, 50, 50, 50 };
  int nBins_SS[10] = { 50, 20, 10, 5, 30, 25, 40, 25, 50, 50 };
  int nBins_SF[10] = { 50, 20, 10, 5, 30, 25, 40, 25, 50, 50 };
  int nBins_EZ[10] = { 50, 20, 10, 5, 30, 25, 40, 25, 50, 50 };

  doublePlotHolder plots;
  for (int m = 0; m < 10; m++){       //m - which plots
    for (int i = 0; i < 4; i++){     //i - b-tag region
      for (int j = 0; j < 4; j++){   //j - HH, HL, LL, all
        for (int k = 0; k < 5; k++){ //k - ee, em, mm, ee+mm, total
          plots.SS[m][i][j][k] = new TH1F(Form("hist_ss_%i_%i_%i_%i", m, i, j, k), "hist", nBins_SS[m], 0, max[m]); 
          plots.SF[m][i][j][k] = new TH1F(Form("hist_sf_%i_%i_%i_%i", m, i, j, k), "hist", nBins_SF[m], 0, max[m]); 
          plots.EZ[m][i][j][k] = new TH1F(Form("hist_ez_%i_%i_%i_%i", m, i, j, k), "hist", nBins_EZ[m], 0, max[m]); 
          plots.OS[m][i][j][k] = new TH1F(Form("hist_os_%i_%i_%i_%i", m, i, j, k), "hist", nBins_OS[m], 0, max[m]); 
        }
      }
    }
  }

  //Weight
  float totWeight = 0.;
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
      float lumi = getLumi();
      float weight = isData ? 1 : ss::scale1fb()*lumi;

      //Reject duplicates
      if (isData == true){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)){ reject++; continue; }
      }

      //Apply PU reweighting
      if (!isData) {
      	float puw = getPUw(ss::nGoodVertices());
      	weight*=puw;
      }
      //DY totWeight=285999/325590
      totWeight+=weight;

      //Throw away non OS-numerator events
      if (ss::hyp_class() != 4 && ss::hyp_class() != 3 && ss::hyp_class() != 2 && ss::hyp_class() != 6) continue;

      //Reject not triggered
      if (!ss::fired_trigger()) continue;

      //Determine MET
      float metAG = (corrected ? ss::corrMET() : ss::met() );
      float metPhiAG = (corrected ? ss::corrMETphi() : ss::metPhi());

      //Determine HT
      float htAG = (corrected ? ss::ht_corr() : ss::ht());

      //Determine njets
      float njetsAG =  (corrected ? ss::njets_corr() : ss::njets());

      //Determine nbtags
      float nbtagsAG = (corrected ? ss::nbtags_corr() : ss::nbtags());

      //Calculate mtmin
      float mt1 = MT(ss::lep1_p4().pt(), ss::lep1_p4().phi(), metAG, metPhiAG);
      float mt2 = MT(ss::lep2_p4().pt(), ss::lep2_p4().phi(), metAG, metPhiAG);
      float mtmin = mt1 > mt2 ? mt2 : mt1; 

      //Determine analysis category
      int AC = -1;
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == HighHigh) AC = 0; 
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == HighLow ) AC = 1; 
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) == LowLow  ) AC = 2; 
      if (AC < 0) continue;

      //Determine BR
      int BR = baselineRegion(njetsAG, nbtagsAG, metAG, htAG, ss::lep1_p4().pt(), ss::lep2_p4().pt()); 
      if (mode == 0 && BR < 0) continue;
      if (mode == 2 && njetsAG < 2) continue;
      BR = 0;

      //Determine ee, em, mm
      int type = -1;
      if (ss::hyp_type() == 0) type = 0; 
      else if (ss::hyp_type() == 3) type = 2; 
      else type = 1; 

      //Fill MET, HT
      if (ss::hyp_class() == 4){
        plots.OS[0][BR][AC][type]->Fill(metAG                               , weight); 
        plots.OS[1][BR][AC][type]->Fill(htAG                                , weight); 
        plots.OS[2][BR][AC][type]->Fill(njetsAG                             , weight); 
        plots.OS[3][BR][AC][type]->Fill(nbtagsAG                            , weight); 
        plots.OS[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                  , weight); 
        plots.OS[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                  , weight); 
        plots.OS[5][BR][AC][type]->Fill(mtmin                               , weight); 
        plots.OS[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M() , weight);
        plots.OS[7][BR][AC][type]->Fill(ss::nGoodVertices()                 , weight);
      }
      else if (ss::hyp_class() == 3){ 
        plots.SS[0][BR][AC][type]->Fill(metAG                              , weight); 
        plots.SS[1][BR][AC][type]->Fill(htAG                               , weight); 
        plots.SS[2][BR][AC][type]->Fill(njetsAG                            , weight); 
        plots.SS[3][BR][AC][type]->Fill(nbtagsAG                           , weight); 
        plots.SS[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                 , weight); 
        plots.SS[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                 , weight); 
        plots.SS[5][BR][AC][type]->Fill(mtmin                              , weight); 
        plots.SS[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M(), weight);
        plots.SS[7][BR][AC][type]->Fill(ss::nGoodVertices()                , weight);
      }
      else if (ss::hyp_class() == 2){ 
        plots.SF[0][BR][AC][type]->Fill(metAG                              , weight); 
        plots.SF[1][BR][AC][type]->Fill(htAG                               , weight); 
        plots.SF[2][BR][AC][type]->Fill(njetsAG                            , weight); 
        plots.SF[3][BR][AC][type]->Fill(nbtagsAG                           , weight); 
        plots.SF[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                 , weight); 
        plots.SF[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                 , weight); 
        plots.SF[5][BR][AC][type]->Fill(mtmin                              , weight); 
        plots.SF[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M(), weight);
        plots.SF[7][BR][AC][type]->Fill(ss::nGoodVertices()                , weight);
      }
      else if (ss::hyp_class() == 6 && ss::madeExtraZ() && ss::lep1_passes_id() && ss::lep2_passes_id() && ss::lep3_passes_id()){ 
        plots.EZ[0][BR][AC][type]->Fill(metAG                              , weight); 
        plots.EZ[1][BR][AC][type]->Fill(htAG                               , weight); 
        plots.EZ[2][BR][AC][type]->Fill(njetsAG                            , weight); 
        plots.EZ[3][BR][AC][type]->Fill(nbtagsAG                           , weight); 
        plots.EZ[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                 , weight); 
        plots.EZ[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                 , weight); 
        plots.EZ[5][BR][AC][type]->Fill(mtmin                              , weight); 
        plots.EZ[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M(), weight);
        plots.EZ[7][BR][AC][type]->Fill(ss::nGoodVertices()                , weight);
      }

    }//event loop
  }//file loop

  //Fill ee+mm, ee+em+mm plots
  for (int m = 0; m < 10; m++){
    for (int i = 0; i < 4; i++){     
      for (int j = 0; j < 3; j++){   
        plots.SS[m][i][j][3]->Add( plots.SS[m][i][j][0], plots.SS[m][i][j][2] );
        plots.SF[m][i][j][3]->Add( plots.SF[m][i][j][0], plots.SF[m][i][j][2] );
        plots.EZ[m][i][j][3]->Add( plots.EZ[m][i][j][0], plots.EZ[m][i][j][2] );
        plots.OS[m][i][j][3]->Add( plots.OS[m][i][j][0], plots.OS[m][i][j][2] );
        plots.SS[m][i][j][4]->Add( plots.SS[m][i][j][3], plots.SS[m][i][j][1] );
        plots.SF[m][i][j][4]->Add( plots.SF[m][i][j][3], plots.SF[m][i][j][1] );
        plots.EZ[m][i][j][4]->Add( plots.EZ[m][i][j][3], plots.EZ[m][i][j][1] );
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
        plots.SF[m][i][3][j]->Add( plots.SF[m][i][0][j], plots.SF[m][i][1][j]);
        plots.SF[m][i][3][j]->Add( plots.SF[m][i][2][j] );
        plots.EZ[m][i][3][j]->Add( plots.EZ[m][i][0][j], plots.EZ[m][i][1][j]);
        plots.EZ[m][i][3][j]->Add( plots.EZ[m][i][2][j] );
        plots.OS[m][i][3][j]->Add( plots.OS[m][i][0][j], plots.OS[m][i][1][j]);
        plots.OS[m][i][3][j]->Add( plots.OS[m][i][2][j] );
      }
    }
  }

  //Fill ee+mm, ee+em+mm plots for HH+HL+LL
  for (int m = 0; m < 10; m++){
    for (int i = 0; i < 4; i++){     
        plots.SS[m][i][3][3]->Add( plots.SS[m][i][3][0], plots.SS[m][i][3][2] );
        plots.SF[m][i][3][3]->Add( plots.SF[m][i][3][0], plots.SF[m][i][3][2] );
        plots.EZ[m][i][3][3]->Add( plots.EZ[m][i][3][0], plots.EZ[m][i][3][2] );
        plots.OS[m][i][3][3]->Add( plots.OS[m][i][3][0], plots.OS[m][i][3][2] );
        plots.SS[m][i][3][4]->Add( plots.SS[m][i][3][3], plots.SS[m][i][3][1] );
        plots.SF[m][i][3][4]->Add( plots.SF[m][i][3][3], plots.SF[m][i][3][1] );
        plots.EZ[m][i][3][4]->Add( plots.EZ[m][i][3][3], plots.EZ[m][i][3][1] );
        plots.OS[m][i][3][4]->Add( plots.OS[m][i][3][3], plots.OS[m][i][3][1] );
    }
  }

  if (isData) cout << "number of duplicates removed: " << reject << endl;
  cout << chain->GetTitle() << " totWeight=" << totWeight << endl;

  return plots;

}

void plots(){

  TString tag = getTag();
  
  //Set up chains
  TChain *chain_data = new TChain("t","data");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataMuonEGD.root");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleEGD.root");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleMuonD.root");

  TChain *chain_ttW = new TChain("t","ttW"); 
  chain_ttW->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTW.root");

  TChain *chain_ttZ = new TChain("t","ttZ"); 
  chain_ttZ->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTZL.root");

  TChain *chain_ttbar = new TChain("t","ttbar"); 
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTBAR.root");
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/SINGLETOP.root");

  TChain *chain_WZ = new TChain("t","WZ"); 
  chain_WZ->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WZ3LNU.root");

  TChain *chain_DY = new TChain("t","DY"); 
  chain_DY->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DY_high.root");
  chain_DY->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DY_low.root");

  TChain *chain_WJets = new TChain("t","WJets"); 
  chain_WJets->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WJets.root");
  chain_WJets->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WGToLNuG.root");

  //Make individual plots
  doublePlotHolder data = plotMaker(chain_data, 1);   
  doublePlotHolder ttw = plotMaker(chain_ttW, 0);   
  doublePlotHolder ttz = plotMaker(chain_ttZ, 0);   
  doublePlotHolder wz = plotMaker(chain_WZ, 0);
  doublePlotHolder ttbar = plotMaker(chain_ttbar, 0);
  doublePlotHolder dy = plotMaker(chain_DY, 0);
  doublePlotHolder wjets = plotMaker(chain_WJets, 0);

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

  vector < vector < TH1F*> > SF_plots_em; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.SF[i][0][3][1]);
    temp.push_back(ttz.SF[i][0][3][1]);
    temp.push_back(wz.SF[i][0][3][1]);
    temp.push_back(ttbar.SF[i][0][3][1]);
    temp.push_back(dy.SF[i][0][3][1]);
    temp.push_back(wjets.SF[i][0][3][1]);
    SF_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots_em; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.EZ[i][0][3][1]);
    temp.push_back(ttz.EZ[i][0][3][1]);
    temp.push_back(wz.EZ[i][0][3][1]);
    temp.push_back(ttbar.EZ[i][0][3][1]);
    temp.push_back(dy.EZ[i][0][3][1]);
    temp.push_back(wjets.EZ[i][0][3][1]);
    EZ_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots_mm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.OS[i][0][3][0]);
    temp.push_back(  ttz.OS[i][0][3][0]);
    temp.push_back(   wz.OS[i][0][3][0]);
    temp.push_back(ttbar.OS[i][0][3][0]);
    temp.push_back(   dy.OS[i][0][3][0]);
    temp.push_back(wjets.OS[i][0][3][0]);
    OS_plots_mm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_mm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.SS[i][0][3][0]);
    temp.push_back(  ttz.SS[i][0][3][0]);
    temp.push_back(   wz.SS[i][0][3][0]);
    temp.push_back(ttbar.SS[i][0][3][0]);
    temp.push_back(   dy.SS[i][0][3][0]);
    temp.push_back(wjets.SS[i][0][3][0]);
    SS_plots_mm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SF_plots_mm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.SF[i][0][3][0]);
    temp.push_back(  ttz.SF[i][0][3][0]);
    temp.push_back(   wz.SF[i][0][3][0]);
    temp.push_back(ttbar.SF[i][0][3][0]);
    temp.push_back(   dy.SF[i][0][3][0]);
    temp.push_back(wjets.SF[i][0][3][0]);
    SF_plots_mm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots_mm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.EZ[i][0][3][0]);
    temp.push_back(  ttz.EZ[i][0][3][0]);
    temp.push_back(   wz.EZ[i][0][3][0]);
    temp.push_back(ttbar.EZ[i][0][3][0]);
    temp.push_back(   dy.EZ[i][0][3][0]);
    temp.push_back(wjets.EZ[i][0][3][0]);
    EZ_plots_mm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots_ee; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.OS[i][0][3][2]);
    temp.push_back(  ttz.OS[i][0][3][2]);
    temp.push_back(   wz.OS[i][0][3][2]);
    temp.push_back(ttbar.OS[i][0][3][2]);
    temp.push_back(   dy.OS[i][0][3][2]);
    temp.push_back(wjets.OS[i][0][3][2]);
    OS_plots_ee.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_ee; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.SS[i][0][3][2]);
    temp.push_back(  ttz.SS[i][0][3][2]);
    temp.push_back(   wz.SS[i][0][3][2]);
    temp.push_back(ttbar.SS[i][0][3][2]);
    temp.push_back(   dy.SS[i][0][3][2]);
    temp.push_back(wjets.SS[i][0][3][2]);
    SS_plots_ee.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SF_plots_ee; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.SF[i][0][3][2]);
    temp.push_back(  ttz.SF[i][0][3][2]);
    temp.push_back(   wz.SF[i][0][3][2]);
    temp.push_back(ttbar.SF[i][0][3][2]);
    temp.push_back(   dy.SF[i][0][3][2]);
    temp.push_back(wjets.SF[i][0][3][2]);
    SF_plots_ee.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots_ee; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.EZ[i][0][3][2]);
    temp.push_back(  ttz.EZ[i][0][3][2]);
    temp.push_back(   wz.EZ[i][0][3][2]);
    temp.push_back(ttbar.EZ[i][0][3][2]);
    temp.push_back(   dy.EZ[i][0][3][2]);
    temp.push_back(wjets.EZ[i][0][3][2]);
    EZ_plots_ee.push_back(temp); 
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

  vector < vector < TH1F*> > SF_plots_eemm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.SF[i][0][3][3]);
    temp.push_back(  ttz.SF[i][0][3][3]);
    temp.push_back(   wz.SF[i][0][3][3]);
    temp.push_back(ttbar.SF[i][0][3][3]);
    temp.push_back(   dy.SF[i][0][3][3]);
    temp.push_back(wjets.SF[i][0][3][3]);
    SF_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots_eemm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttw.EZ[i][0][3][3]);
    temp.push_back(  ttz.EZ[i][0][3][3]);
    temp.push_back(   wz.EZ[i][0][3][3]);
    temp.push_back(ttbar.EZ[i][0][3][3]);
    temp.push_back(   dy.EZ[i][0][3][3]);
    temp.push_back(wjets.EZ[i][0][3][3]);
    EZ_plots_eemm.push_back(temp); 
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

  vector < vector < TH1F*> > SF_plots; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.SF[i][0][3][4]);
    temp.push_back(ttz.SF[i][0][3][4]);
    temp.push_back(wz.SF[i][0][3][4]);
    temp.push_back(ttbar.SF[i][0][3][4]);
    temp.push_back(dy.SF[i][0][3][4]);
    temp.push_back(wjets.SF[i][0][3][4]);
    SF_plots.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttw.EZ[i][0][3][4]);
    temp.push_back(ttz.EZ[i][0][3][4]);
    temp.push_back(wz.EZ[i][0][3][4]);
    temp.push_back(ttbar.EZ[i][0][3][4]);
    temp.push_back(dy.EZ[i][0][3][4]);
    temp.push_back(wjets.EZ[i][0][3][4]);
    EZ_plots.push_back(temp); 
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
  names.push_back("nvtx");

  //Titles for output
  vector <string> out;
  out.push_back("MET");
  out.push_back("HT");
  out.push_back("nJets");
  out.push_back("nBtags");
  out.push_back("leppt");
  out.push_back("mtmin");
  out.push_back("mll");
  out.push_back("nvtx");

  vector <double> max_upstream   = { 5e5, 5e5, 5e6, 1e6, 1e6, 1e6, 1e6, 1e5, 5e5, 5e5 };
  vector <double> max_downstream = { 5e4, 5e4, 5e5, 1e5, 1e5, 1e5, 1e5, 1e4, 5e4, 5e4 }; 
  vector <double> max;
  if (mode == 0) max = max_downstream;
  if (mode == 1) max = max_upstream; 
  if (mode == 2) max = max_upstream;
  double min[10] = {1e-3, 1e-3, 1e-2, 1e-1, 1e-3, 1e-3, 1e-3, 1e-3, 1e-3, 1e-3 };

  std::string title;
  if (mode == 0) title = Form("Run2, HH,HL,LL, BR0-3 %s"   , corrected ? "" : "raw" );
  if (mode == 1) title = Form("Run2, HH,HL,LL, upstream %s", corrected ? "" : "raw" );
  if (mode == 2) title = Form("Run2, HH,HL,LL, njets > 1 %s", corrected ? "" : "raw" );

  //Make plots
  for (int i = 0; i < 10; i++){
    //mm
    dataMCplotMaker(data.OS[i][0][3][0], OS_plots_mm[i], titles, Form("%s %s", names[i].c_str(), "(OS #mu#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_mm_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][0], SS_plots_mm[i], titles, Form("%s %s", names[i].c_str(), "(SS #mu#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_mm_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][0], SF_plots_mm[i], titles, Form("%s %s", names[i].c_str(), "(SF #mu#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_mm_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][0], EZ_plots_mm[i], titles, Form("%s %s", names[i].c_str(), "(EZ #mu#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_mm_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 

    //ee
    dataMCplotMaker(data.OS[i][0][3][2], OS_plots_ee[i], titles, Form("%s %s", names[i].c_str(), "(OS ee)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_ee_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][2], SS_plots_ee[i], titles, Form("%s %s", names[i].c_str(), "(SS ee)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_ee_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][2], SF_plots_ee[i], titles, Form("%s %s", names[i].c_str(), "(SF ee)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_ee_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][2], EZ_plots_ee[i], titles, Form("%s %s", names[i].c_str(), "(EZ ee)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_ee_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 

    //ee+mm
    dataMCplotMaker(data.OS[i][0][3][3], OS_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(OS ee+#mu#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_eemm_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][3], SS_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(SS ee+#mu#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_eemm_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][3], SF_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(SF ee+#mu#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_eemm_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][3], EZ_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(EZ ee+#mu#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_eemm_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
   //emu
    dataMCplotMaker(data.OS[i][0][3][1], OS_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(OS e#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_em_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][1], SS_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(SS e#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_em_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][1], SF_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(SF e#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_em_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][1], EZ_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(EZ e#mu)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_em_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
   //all
    dataMCplotMaker(data.OS[i][0][3][4], OS_plots[i], titles, Form("%s %s", names[i].c_str(), "(OS)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][4], SS_plots[i], titles, Form("%s %s", names[i].c_str(), "(SS)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][4], SF_plots[i], titles, Form("%s %s", names[i].c_str(), "(SF)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][4], EZ_plots[i], titles, Form("%s %s", names[i].c_str(), "(EZ)"), title.c_str(), Form("--lumi 209.5 --lumiUnit pb --outputName %s_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
  }

}
