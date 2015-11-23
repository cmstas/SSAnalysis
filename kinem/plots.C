#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "../classFiles/v4.05/SS.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../CORE/SSSelections.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"

int mode = 1;  // 0-nominal, 1-upstream, 2-upstream+2jets

struct doublePlotHolder{ TH1F* OS[10][4][4][5]; TH1F* SS[10][4][4][5]; TH1F* SF[10][4][4][5]; TH1F* EZ[10][4][4][5]; TH1F* sr3[3]; TH1F* sr2[3]; };

doublePlotHolder plotMaker(TChain *chain, bool isData){

                     //MET,  HT, nj, nb, pt,  mt,  mll nvtx
  int      max[10] = { 700, 800, 10, 5, 300, 200 , 500, 24, 700, 700 };
  int nBins_OS[10] = {  50,  25, 10, 5,  30,  50 ,  40, 24,  50,  50 };
  int nBins_SS[10] = {  50,  20, 10, 5,  30,  25 ,  25, 24,  50,  50 };
  int nBins_SF[10] = {  50,  20, 10, 5,  30,  25 ,  40, 24,  50,  50 };
  int nBins_EZ[10] = {  50,  20, 10, 5,  30,  25 ,  40, 24,  50,  50 };

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

  //Also SR plots
  plots.sr3[0] = new TH1F("SRHH_ss", "SRHH_ss", 32, 1, 33); 
  plots.sr3[1] = new TH1F("SRHL_ss", "SRHL_ss", 26, 1, 27); 
  plots.sr3[2] = new TH1F("SRLL_ss", "SRLL_ss",  8, 1,  9); 
  plots.sr2[0] = new TH1F("SRHH_sf", "SRHH_sf", 32, 1, 33); 
  plots.sr2[1] = new TH1F("SRHL_sf", "SRHL_sf", 26, 1, 27); 
  plots.sr2[2] = new TH1F("SRLL_sf", "SRLL_sf",  8, 1,  9); 

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

      //if (ss::event() == (int)447808321) cout << "here0!" << endl;
    
      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //Get event weight
      float lumi = getLumi();
      float weight = isData ? 1 : ss::scale1fb()*lumi;

      //Reject duplicates
      if (isData){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)){ reject++; continue; }
      }

      //Apply PU reweighting
      if (!isData) {
      	//float puw = getPUwECO(ss::nGoodVertices());
      	float puw = getTruePUw(ss::trueNumInt()[0]);
      	weight*=puw;
      }
      //DY totWeight=285999/325590
      totWeight+=weight;

      //Throw away non OS-numerator events
      if (ss::hyp_class() != 4 && ss::hyp_class() != 3 && ss::hyp_class() != 2 && ss::hyp_class() != 6) continue;

      //Reject not triggered
      if (!ss::fired_trigger()) continue;
      if (!ss::passedFilterList()) continue;
      if (!ss::passes_met_filters()) continue;

      //electron FO is tighter for iso triggers, make sure it is passed
      if (ss::ht()<300.){
        if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA())) continue;
        if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA())) continue;
      } 

      //Determine analysis category
      int AC = -1;
      if (analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()) == HighHigh) AC = 0; 
      if (analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()) == HighLow ) AC = 1; 
      if (analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()) == LowLow  ) AC = 2; 
      if (AC < 0) continue;

      //Determine BR
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()); 
      if (mode == 0 && BR < 0) continue;
      if (mode == 2 && ss::njets() < 2) continue;
      BR = 0;

      //Determine ee, em, mm
      int type = -1;
      if      (ss::hyp_type() == 0) type = 0; 
      else if (ss::hyp_type() == 3) type = 2; 
      else                          type = 1; 

      //Determine SR
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()); 

      //Fill MET, HT
      if (ss::hyp_class() == 4){
        plots.OS[0][BR][AC][type]->Fill(ss::met()                           , weight);
        plots.OS[1][BR][AC][type]->Fill(ss::ht()                            , weight);
        plots.OS[2][BR][AC][type]->Fill(ss::njets()                         , weight);
        plots.OS[3][BR][AC][type]->Fill(ss::nbtags()                        , weight);
        plots.OS[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                  , weight);
        plots.OS[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                  , weight);
        plots.OS[5][BR][AC][type]->Fill(ss::mtmin()                        , weight);
        plots.OS[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M() , weight);
        plots.OS[7][BR][AC][type]->Fill(ss::nGoodVertices()                 , weight);
      }
      else if (ss::hyp_class() == 3){ 
        float unblindWeight = isData ? 1 : weight*getLumiUnblind()/getLumi();
        if (!isData || isUnblindRun(ss::run())) {
          plots.SS[0][BR][AC][type]->Fill(ss::met()                           , unblindWeight);
          plots.SS[1][BR][AC][type]->Fill(ss::ht()                            , unblindWeight);
          plots.SS[2][BR][AC][type]->Fill(ss::njets()                         , unblindWeight);
          plots.SS[3][BR][AC][type]->Fill(ss::nbtags()                        , unblindWeight);
          plots.SS[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                  , unblindWeight);
          plots.SS[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                  , unblindWeight);
          plots.SS[5][BR][AC][type]->Fill(ss::mtmin()                        , unblindWeight);
          plots.SS[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M() , unblindWeight);
          plots.SS[7][BR][AC][type]->Fill(ss::nGoodVertices()                 , unblindWeight);
          plots.sr3[AC]->Fill(SR, unblindWeight); 
        }
      }
      else if (ss::hyp_class() == 2){ 
        plots.SF[0][BR][AC][type]->Fill(ss::met()                           , weight);
        plots.SF[1][BR][AC][type]->Fill(ss::ht()                            , weight);
        plots.SF[2][BR][AC][type]->Fill(ss::njets()                         , weight);
        plots.SF[3][BR][AC][type]->Fill(ss::nbtags()                        , weight);
        plots.SF[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                  , weight);
        plots.SF[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                  , weight);
        plots.SF[5][BR][AC][type]->Fill(ss::mtmin()                        , weight);
        plots.SF[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M() , weight);
        plots.SF[7][BR][AC][type]->Fill(ss::nGoodVertices()                 , weight);
        plots.sr2[AC]->Fill(SR, weight); 
      }
      else if (ss::hyp_class() == 6 && ss::madeExtraZ() && ss::lep1_passes_id() && ss::lep2_passes_id() && ss::lep3_passes_id()){ 
        plots.EZ[0][BR][AC][type]->Fill(ss::met()                           , weight);
        plots.EZ[1][BR][AC][type]->Fill(ss::ht()                            , weight);
        plots.EZ[2][BR][AC][type]->Fill(ss::njets()                         , weight);
        plots.EZ[3][BR][AC][type]->Fill(ss::nbtags()                        , weight);
        plots.EZ[4][BR][AC][type]->Fill(ss::lep1_p4().pt()                  , weight);
        plots.EZ[4][BR][AC][type]->Fill(ss::lep2_p4().pt()                  , weight);
        plots.EZ[5][BR][AC][type]->Fill(ss::mtmin()                        , weight);
        plots.EZ[6][BR][AC][type]->Fill((ss::lep1_p4() + ss::lep2_p4()).M() , weight);
        plots.EZ[7][BR][AC][type]->Fill(ss::nGoodVertices()                 , weight);
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
  //cout << chain->GetTitle() << " totWeight=" << totWeight << endl;

  return plots;

}

void plots(){

  TString tag = getTag();
  
  //Set up chains
  TChain *chain_data = new TChain("t","data");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleEGC_05oct.root"  );
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleMuonC_05oct.root");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataMuonEGC_05oct.root"    );
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleEGD_05oct.root"  );
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleMuonD_05oct.root");
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataMuonEGD_05oct.root"    );
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleEGD_v4.root"     );
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleMuonD_v4.root"   );
  chain_data->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataMuonEGD_v4.root"       );

  TChain *chain_ttv = new TChain("t","ttv"); 
  chain_ttv->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTW.root");
  chain_ttv->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTWQQ.root");
  chain_ttv->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTZL.root");
  chain_ttv->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTZQ.root");

  TChain *chain_rares = new TChain("t","rares"); 
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WZZ.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WWZ.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/QQWW.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WWDPS.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/VHtoNonBB.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTHtoNonBB.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTG.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TZQ.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTTT.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/ZZ.root");
  chain_rares->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/GGHtoZZto4L.root");

  TChain *chain_ttbar = new TChain("t","ttbar"); 
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTBAR_PH.root");
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/SINGLETOP1.root");
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/SINGLETOP2.root");
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/SINGLETOP3.root");
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/SINGLETOP4.root");
  chain_ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/SINGLETOP5.root");

  TChain *chain_VZ = new TChain("t","VZ"); 
  chain_VZ->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WZ.root");
  chain_VZ->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/ZZ.root");

  TChain *chain_DY = new TChain("t","DY"); 
  chain_DY->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DY_high.root");
  chain_DY->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DY_low.root");

  TChain *chain_WJets = new TChain("t","WJets"); 
  chain_WJets->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WJets_LO.root");
  chain_WJets->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WGToLNuG.root");

  //Make individual plots
  doublePlotHolder data  = plotMaker(chain_data, 1);
  doublePlotHolder ttv   = plotMaker(chain_ttv, 0);
  doublePlotHolder rares = plotMaker(chain_rares, 0);
  doublePlotHolder vz    = plotMaker(chain_VZ, 0);
  doublePlotHolder ttbar = plotMaker(chain_ttbar, 0);
  doublePlotHolder dy    = plotMaker(chain_DY, 0);
  doublePlotHolder wjets = plotMaker(chain_WJets, 0);

  //Make vectors for plots
  vector < vector < TH1F*> > OS_plots_em; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttv.OS[i][0][3][1]);
    temp.push_back(rares.OS[i][0][3][1]);
    temp.push_back(vz.OS[i][0][3][1]);
    temp.push_back(ttbar.OS[i][0][3][1]);
    temp.push_back(dy.OS[i][0][3][1]);
    temp.push_back(wjets.OS[i][0][3][1]);
    OS_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_em; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttv.SS[i][0][3][1]);
    temp.push_back(rares.SS[i][0][3][1]);
    temp.push_back(vz.SS[i][0][3][1]);
    temp.push_back(ttbar.SS[i][0][3][1]);
    temp.push_back(dy.SS[i][0][3][1]);
    temp.push_back(wjets.SS[i][0][3][1]);
    SS_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SF_plots_em; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttv.SF[i][0][3][1]);
    temp.push_back(rares.SF[i][0][3][1]);
    temp.push_back(vz.SF[i][0][3][1]);
    temp.push_back(ttbar.SF[i][0][3][1]);
    temp.push_back(dy.SF[i][0][3][1]);
    temp.push_back(wjets.SF[i][0][3][1]);
    SF_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots_em; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttv.EZ[i][0][3][1]);
    temp.push_back(rares.EZ[i][0][3][1]);
    temp.push_back(vz.EZ[i][0][3][1]);
    temp.push_back(ttbar.EZ[i][0][3][1]);
    temp.push_back(dy.EZ[i][0][3][1]);
    temp.push_back(wjets.EZ[i][0][3][1]);
    EZ_plots_em.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots_mm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.OS[i][0][3][0]);
    temp.push_back(  rares.OS[i][0][3][0]);
    temp.push_back(   vz.OS[i][0][3][0]);
    temp.push_back(ttbar.OS[i][0][3][0]);
    temp.push_back(   dy.OS[i][0][3][0]);
    temp.push_back(wjets.OS[i][0][3][0]);
    OS_plots_mm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_mm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.SS[i][0][3][0]);
    temp.push_back(  rares.SS[i][0][3][0]);
    temp.push_back(   vz.SS[i][0][3][0]);
    temp.push_back(ttbar.SS[i][0][3][0]);
    temp.push_back(   dy.SS[i][0][3][0]);
    temp.push_back(wjets.SS[i][0][3][0]);
    SS_plots_mm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SF_plots_mm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.SF[i][0][3][0]);
    temp.push_back(  rares.SF[i][0][3][0]);
    temp.push_back(   vz.SF[i][0][3][0]);
    temp.push_back(ttbar.SF[i][0][3][0]);
    temp.push_back(   dy.SF[i][0][3][0]);
    temp.push_back(wjets.SF[i][0][3][0]);
    SF_plots_mm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots_mm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.EZ[i][0][3][0]);
    temp.push_back(  rares.EZ[i][0][3][0]);
    temp.push_back(   vz.EZ[i][0][3][0]);
    temp.push_back(ttbar.EZ[i][0][3][0]);
    temp.push_back(   dy.EZ[i][0][3][0]);
    temp.push_back(wjets.EZ[i][0][3][0]);
    EZ_plots_mm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots_ee; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.OS[i][0][3][2]);
    temp.push_back(  rares.OS[i][0][3][2]);
    temp.push_back(   vz.OS[i][0][3][2]);
    temp.push_back(ttbar.OS[i][0][3][2]);
    temp.push_back(   dy.OS[i][0][3][2]);
    temp.push_back(wjets.OS[i][0][3][2]);
    OS_plots_ee.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_ee; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.SS[i][0][3][2]);
    temp.push_back(  rares.SS[i][0][3][2]);
    temp.push_back(   vz.SS[i][0][3][2]);
    temp.push_back(ttbar.SS[i][0][3][2]);
    temp.push_back(   dy.SS[i][0][3][2]);
    temp.push_back(wjets.SS[i][0][3][2]);
    SS_plots_ee.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SF_plots_ee; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.SF[i][0][3][2]);
    temp.push_back(  rares.SF[i][0][3][2]);
    temp.push_back(   vz.SF[i][0][3][2]);
    temp.push_back(ttbar.SF[i][0][3][2]);
    temp.push_back(   dy.SF[i][0][3][2]);
    temp.push_back(wjets.SF[i][0][3][2]);
    SF_plots_ee.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots_ee; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.EZ[i][0][3][2]);
    temp.push_back(  rares.EZ[i][0][3][2]);
    temp.push_back(   vz.EZ[i][0][3][2]);
    temp.push_back(ttbar.EZ[i][0][3][2]);
    temp.push_back(   dy.EZ[i][0][3][2]);
    temp.push_back(wjets.EZ[i][0][3][2]);
    EZ_plots_ee.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots_eemm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.OS[i][0][3][3]);
    temp.push_back(  rares.OS[i][0][3][3]);
    temp.push_back(   vz.OS[i][0][3][3]);
    temp.push_back(ttbar.OS[i][0][3][3]);
    temp.push_back(   dy.OS[i][0][3][3]);
    temp.push_back(wjets.OS[i][0][3][3]);
    OS_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots_eemm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.SS[i][0][3][3]);
    temp.push_back(  rares.SS[i][0][3][3]);
    temp.push_back(   vz.SS[i][0][3][3]);
    temp.push_back(ttbar.SS[i][0][3][3]);
    temp.push_back(   dy.SS[i][0][3][3]);
    temp.push_back(wjets.SS[i][0][3][3]);
    SS_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SF_plots_eemm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.SF[i][0][3][3]);
    temp.push_back(  rares.SF[i][0][3][3]);
    temp.push_back(   vz.SF[i][0][3][3]);
    temp.push_back(ttbar.SF[i][0][3][3]);
    temp.push_back(   dy.SF[i][0][3][3]);
    temp.push_back(wjets.SF[i][0][3][3]);
    SF_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots_eemm; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(  ttv.EZ[i][0][3][3]);
    temp.push_back(  rares.EZ[i][0][3][3]);
    temp.push_back(   vz.EZ[i][0][3][3]);
    temp.push_back(ttbar.EZ[i][0][3][3]);
    temp.push_back(   dy.EZ[i][0][3][3]);
    temp.push_back(wjets.EZ[i][0][3][3]);
    EZ_plots_eemm.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > OS_plots; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttv.OS[i][0][3][4]);
    temp.push_back(rares.OS[i][0][3][4]);
    temp.push_back(vz.OS[i][0][3][4]);
    temp.push_back(ttbar.OS[i][0][3][4]);
    temp.push_back(dy.OS[i][0][3][4]);
    temp.push_back(wjets.OS[i][0][3][4]);
    OS_plots.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SS_plots; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttv.SS[i][0][3][4]);
    temp.push_back(rares.SS[i][0][3][4]);
    temp.push_back(vz.SS[i][0][3][4]);
    temp.push_back(ttbar.SS[i][0][3][4]);
    temp.push_back(dy.SS[i][0][3][4]);
    temp.push_back(wjets.SS[i][0][3][4]);
    SS_plots.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > SF_plots; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttv.SF[i][0][3][4]);
    temp.push_back(rares.SF[i][0][3][4]);
    temp.push_back(vz.SF[i][0][3][4]);
    temp.push_back(ttbar.SF[i][0][3][4]);
    temp.push_back(dy.SF[i][0][3][4]);
    temp.push_back(wjets.SF[i][0][3][4]);
    SF_plots.push_back(temp); 
    temp.clear();
  }

  vector < vector < TH1F*> > EZ_plots; 
  for (int i = 0; i < 10; i++){
    vector <TH1F*> temp; 
    temp.push_back(ttv.EZ[i][0][3][4]);
    temp.push_back(rares.EZ[i][0][3][4]);
    temp.push_back(vz.EZ[i][0][3][4]);
    temp.push_back(ttbar.EZ[i][0][3][4]);
    temp.push_back(dy.EZ[i][0][3][4]);
    temp.push_back(wjets.EZ[i][0][3][4]);
    EZ_plots.push_back(temp); 
    temp.clear();
  }

  //Titles for legend
  vector <string> titles;
  titles.push_back("ttV"); 
  titles.push_back("Rares"); 
  titles.push_back("VZ"); 
  titles.push_back("Top"); 
  titles.push_back("DY"); 
  titles.push_back("W+X"); 

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

  vector <double> max_upstream   = { 5e6, 5e6, 5e6, 5e6, 5e6, 5e6, 5e6, 5e6, 5e6, 5e6 };
  vector <double> max_downstream = { 5e4, 5e4, 5e5, 1e5, 1e5, 1e5, 1e5, 1e4, 5e4, 5e4 }; 
  vector <double> max;
  if (mode == 0) max = max_downstream;
  if (mode == 1) max = max_upstream; 
  if (mode == 2) max = max_upstream;
  double min[10] = {1, 1, 1e-2, 1e-1, 1, 1, 1, 1, 1, 1 };

  std::string title;
  if (mode == 0) title = Form("Run2, HH,HL,LL, BR0-3"); 
  if (mode == 1) title = Form("Run2, HH,HL,LL, upstream"); 
  if (mode == 2) title = Form("Run2, HH,HL,LL, njets #geq 2"); 

  //Make plots
  for (int i = 0; i < 8; i++){
    cout << i << endl;
    //mm
    dataMCplotMaker(data.OS[i][0][3][0], OS_plots_mm[i], titles, Form("%s %s", names[i].c_str(), "(OS #mu#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_mm_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][0], SS_plots_mm[i], titles, Form("%s %s", names[i].c_str(), "(SS #mu#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_mm_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumiUnblind(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][0], SF_plots_mm[i], titles, Form("%s %s", names[i].c_str(), "(SF #mu#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_mm_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][0], EZ_plots_mm[i], titles, Form("%s %s", names[i].c_str(), "(EZ #mu#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_mm_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 

    //ee
    dataMCplotMaker(data.OS[i][0][3][2], OS_plots_ee[i], titles, Form("%s %s", names[i].c_str(), "(OS ee)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_ee_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][2], SS_plots_ee[i], titles, Form("%s %s", names[i].c_str(), "(SS ee)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_ee_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumiUnblind(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][2], SF_plots_ee[i], titles, Form("%s %s", names[i].c_str(), "(SF ee)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_ee_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][2], EZ_plots_ee[i], titles, Form("%s %s", names[i].c_str(), "(EZ ee)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_ee_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 

    //ee+mm
    dataMCplotMaker(data.OS[i][0][3][3], OS_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(OS ee+#mu#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_eemm_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][3], SS_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(SS ee+#mu#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_eemm_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumiUnblind(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][3], SF_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(SF ee+#mu#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_eemm_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][3], EZ_plots_eemm[i], titles, Form("%s %s", names[i].c_str(), "(EZ ee+#mu#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_eemm_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
   //emu
    dataMCplotMaker(data.OS[i][0][3][1], OS_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(OS e#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_em_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][1], SS_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(SS e#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_em_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumiUnblind(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.SF[i][0][3][1], SF_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(SF e#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_em_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
    dataMCplotMaker(data.EZ[i][0][3][1], EZ_plots_em[i], titles, Form("%s %s", names[i].c_str(), "(EZ e#mu)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_em_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
   //all
    dataMCplotMaker(data.OS[i][0][3][4], OS_plots[i], titles, Form("%s %s", names[i].c_str(), "(OS)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_all_OS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i])); 
    dataMCplotMaker(data.SS[i][0][3][4], SS_plots[i], titles, Form("%s %s", names[i].c_str(), "(SS)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_all_SS.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumiUnblind(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.001)); 
    dataMCplotMaker(data.SF[i][0][3][4], SF_plots[i], titles, Form("%s %s", names[i].c_str(), "(SF)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_all_SF.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.01)); 
    dataMCplotMaker(data.EZ[i][0][3][4], EZ_plots[i], titles, Form("%s %s", names[i].c_str(), "(EZ)"), title.c_str(), Form("--lumi %.2f --outputName mode%i_%s_all_EZ.pdf --xAxisLabel %s --percentageInBox --setMinimum %f --setMaximum %f --legendUp -.05", getLumi(), mode, out[i].c_str(), names[i].c_str() , min[i], max[i]*.2)); 
  }

  //And the SR plot
  vector <TH1F*> SRSSHH = { ttv.sr3[0], rares.sr3[0], vz.sr3[0], ttbar.sr3[0], dy.sr3[0], wjets.sr3[0] }; 
  vector <TH1F*> SRSSHL = { ttv.sr3[1], rares.sr3[1], vz.sr3[1], ttbar.sr3[1], dy.sr3[1], wjets.sr3[1] }; 
  vector <TH1F*> SRSSLL = { ttv.sr3[2], rares.sr3[2], vz.sr3[2], ttbar.sr3[2], dy.sr3[2], wjets.sr3[2] }; 
  vector <TH1F*> SRSFHH = { ttv.sr2[0], rares.sr2[0], vz.sr2[0], ttbar.sr2[0], dy.sr2[0], wjets.sr2[0] }; 
  vector <TH1F*> SRSFHL = { ttv.sr2[1], rares.sr2[1], vz.sr2[1], ttbar.sr2[1], dy.sr2[1], wjets.sr2[1] }; 
  vector <TH1F*> SRSFLL = { ttv.sr2[2], rares.sr2[2], vz.sr2[2], ttbar.sr2[2], dy.sr2[2], wjets.sr2[2] }; 
  dataMCplotMaker(data.sr3[0], SRSSHH, titles, "SS H-H SR", "", "--outputName SR_hh_ss.pdf --isLinear --noDivisionLabel --noXaxisUnit --noOverflow"); 
  dataMCplotMaker(data.sr3[1], SRSSHL, titles, "SS H-L SR", "", "--outputName SR_hl_ss.pdf --isLinear --noDivisionLabel --noXaxisUnit --noOverflow"); 
  dataMCplotMaker(data.sr3[2], SRSSLL, titles, "SS L-L SR", "", "--outputName SR_ll_ss.pdf --isLinear --noDivisionLabel --noXaxisUnit --noOverflow"); 
  dataMCplotMaker(data.sr2[0], SRSFHH, titles, "SF H-H SR", "", "--outputName SR_hh_sf.pdf --isLinear --noDivisionLabel --noXaxisUnit --noOverflow"); 
  dataMCplotMaker(data.sr2[1], SRSFHL, titles, "SF H-L SR", "", "--outputName SR_hl_sf.pdf --isLinear --noDivisionLabel --noXaxisUnit --noOverflow"); 
  dataMCplotMaker(data.sr2[2], SRSFLL, titles, "SF L-L SR", "", "--outputName SR_ll_sf.pdf --isLinear --noDivisionLabel --noXaxisUnit --noOverflow"); 

}
