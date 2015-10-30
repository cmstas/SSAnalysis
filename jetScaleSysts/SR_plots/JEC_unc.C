#include <iostream>
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "../../classFiles/v4.03/SS.h"
#include "../../commonUtils.h"
#include "../../CORE/SSSelections.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"

using namespace std;

float lumiAG = getLumi();

vector<vector<TH1F*> > run(TChain* chain);

void JEC_unc(){

  TChain *ttw = new TChain("t"); 
  TChain *wz  = new TChain("t"); 

  ttw->Add("/nfs-7/userdata/ss2015/ssBabies/v4.03/TTW.root"); 
  wz ->Add("/nfs-7/userdata/ss2015/ssBabies/v4.03/WZ3LNU.root"); 

  vector <vector<TH1F*> > plots_ttw = run(ttw); 
  vector <vector<TH1F*> > plots_wz  = run(wz); 

  //Make plots
  TH1F* null = new TH1F("","",1,0,1);
  vector <TH1F*> hh_normal = { plots_ttw[0][0], plots_wz[0][0] }; 
  vector <TH1F*> hh_up     = { plots_ttw[0][1], plots_wz[0][1] }; 
  vector <TH1F*> hh_dn     = { plots_ttw[0][2], plots_wz[0][2] }; 
  vector <TH1F*> hl_normal = { plots_ttw[1][0], plots_wz[1][0] }; 
  vector <TH1F*> hl_up     = { plots_ttw[1][1], plots_wz[1][1] }; 
  vector <TH1F*> hl_dn     = { plots_ttw[1][2], plots_wz[1][2] }; 
  vector <TH1F*> ll_normal = { plots_ttw[2][0], plots_wz[2][0] }; 
  vector <TH1F*> ll_up     = { plots_ttw[2][1], plots_wz[2][1] }; 
  vector <TH1F*> ll_dn     = { plots_ttw[2][2], plots_wz[2][2] }; 
  vector <string> titles   = { "TTW", "WZ" }; 
  dataMCplotMaker(null, hh_normal, titles, "JEC unc", "", Form("--outputName hh_no --isLinear --lumi %f --xAxisOverride SR --setMaximum 60 --noDivisionLabel", lumiAG)); 
  dataMCplotMaker(null, hh_up    , titles, "JEC unc", "", Form("--outputName hh_up --isLinear --lumi %f --xAxisOverride SR --setMaximum 60 --noDivisionLabel", lumiAG)); 
  dataMCplotMaker(null, hh_dn    , titles, "JEC unc", "", Form("--outputName hh_dn --isLinear --lumi %f --xAxisOverride SR --setMaximum 60 --noDivisionLabel", lumiAG)); 
  dataMCplotMaker(null, hl_normal, titles, "JEC unc", "", Form("--outputName hl_no --isLinear --lumi %f --xAxisOverride SR --setMaximum 25 --noDivisionLabel", lumiAG)); 
  dataMCplotMaker(null, hl_up    , titles, "JEC unc", "", Form("--outputName hl_up --isLinear --lumi %f --xAxisOverride SR --setMaximum 25 --noDivisionLabel", lumiAG)); 
  dataMCplotMaker(null, hl_dn    , titles, "JEC unc", "", Form("--outputName hl_dn --isLinear --lumi %f --xAxisOverride SR --setMaximum 25 --noDivisionLabel", lumiAG)); 
  dataMCplotMaker(null, ll_normal, titles, "JEC unc", "", Form("--outputName ll_no --isLinear --lumi %f --xAxisOverride SR --setMaximum 0.8 --noDivisionLabel", lumiAG)); 
  dataMCplotMaker(null, ll_up    , titles, "JEC unc", "", Form("--outputName ll_up --isLinear --lumi %f --xAxisOverride SR --setMaximum 0.8 --noDivisionLabel", lumiAG)); 
  dataMCplotMaker(null, ll_dn    , titles, "JEC unc", "", Form("--outputName ll_dn --isLinear --lumi %f --xAxisOverride SR --setMaximum 0.8 --noDivisionLabel", lumiAG)); 

  //More plots
  vector <TH1F*> ttw_hh = { plots_ttw[0][0], plots_ttw[0][1], plots_ttw[0][2] }; 
  vector <TH1F*> ttw_hl = { plots_ttw[1][0], plots_ttw[1][1], plots_ttw[1][2] }; 
  vector <TH1F*> ttw_ll = { plots_ttw[2][0], plots_ttw[2][1], plots_ttw[2][2] }; 
  vector <TH1F*>  wz_hh = {  plots_wz[0][0],  plots_wz[0][1],  plots_wz[0][2] }; 
  vector <TH1F*>  wz_hl = {  plots_wz[1][0],  plots_wz[1][1],  plots_wz[1][2] }; 
  vector <TH1F*>  wz_ll = {  plots_wz[2][0],  plots_wz[2][1],  plots_wz[2][2] }; 
  vector <string> titles2 = { "normal", "up", "down" }; 
  dataMCplotMaker(plots_ttw[0][1], ttw_hh, titles2, "JEC unc", "ttW HH", Form("--outputName ttw_hh --isLinear --lumi %f --xAxisOverride SR --setMaximum 5   --noDivisionLabel --noStack --ratio 0 --ratioOnly --legendUp -0.05 --ratioColor kRed --topYaxisTitle up/normal --noErrBars --ratioLine", lumiAG));
  dataMCplotMaker(plots_ttw[1][1], ttw_hl, titles2, "JEC unc", "ttW HL", Form("--outputName ttw_hl --isLinear --lumi %f --xAxisOverride SR --setMaximum 4  --noDivisionLabel --noStack --ratio 0 --ratioOnly --legendUp -0.05 --ratioColor kRed --topYaxisTitle up/normal  --noErrBars --ratioLine", lumiAG));
  dataMCplotMaker(plots_ttw[2][1], ttw_ll, titles2, "JEC unc", "ttW LL", Form("--outputName ttw_ll --isLinear --lumi %f --xAxisOverride SR --setMaximum 0.6 --noDivisionLabel --noStack --ratio 0 --ratioOnly --legendUp -0.05 --ratioColor kRed --topYaxisTitle up/normal --noErrBars --ratioLine", lumiAG));
  dataMCplotMaker(plots_wz[0][1],  wz_hh, titles2, "JEC unc",  "WZ HH", Form("--outputName  wz_hh --isLinear --lumi %f --xAxisOverride SR --setMaximum 60  --noDivisionLabel  --noStack --ratio 0 --ratioOnly --legendUp -0.05 --ratioColor kRed --topYaxisTitle up/normal --noErrBars --ratioLine", lumiAG));
  dataMCplotMaker(plots_wz[1][1],  wz_hl, titles2, "JEC unc",  "WZ HL", Form("--outputName  wz_hl --isLinear --lumi %f --xAxisOverride SR --setMaximum 22  --noDivisionLabel  --noStack --ratio 0 --ratioOnly --legendUp -0.05 --ratioColor kRed --topYaxisTitle up/normal --noErrBars --ratioLine", lumiAG));
  dataMCplotMaker(plots_wz[2][1],  wz_ll, titles2, "JEC unc",  "WZ LL", Form("--outputName  wz_ll --isLinear --lumi %f --xAxisOverride SR --setMaximum 0.6 --noDivisionLabel  --noStack --ratio 0 --ratioOnly --legendUp -0.05 --ratioColor kRed --topYaxisTitle up/normal --noErrBars --ratioLine", lumiAG));

}

vector <vector<TH1F*> > run(TChain* chain){

  //Declare empty histograms
  vector <TH1F*> HH, HL, LL;
  vector <vector<TH1F*> > result = {HH, HL, LL}; 
  int nBins[3] = {32, 26, 8}; 
  for (unsigned int i = 0; i < result.size(); i++){
    for (int j = 0; j < 3; j++){
      TH1F* temp = new TH1F(Form("hist_%i_%i", i, j), Form("hist_%i_%i", i, j), nBins[i], 1, nBins[i]+1); 
      result[i].push_back(temp); 
    } 
  }

  //nEvents in chain
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    //Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      samesign.GetEntry(event);
      nEventsTotal++;

      //Category
      anal_type_t AC_name = analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      int AC = -2;
      if      (AC_name == HighHigh) AC = 0;
      else if (AC_name == HighLow)  AC = 1;
      else if (AC_name == LowLow)   AC = 2;
      else                          AC = -1; 
      if (AC < 0) continue; 

      //Signal Region
      int SR    = signalRegion(ss::njets()       , ss::nbtags()       , ss::met()       , ss::ht()       , ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      int SR_up = signalRegion(ss::njets_unc_up(), ss::nbtags_unc_up(), ss::met_unc_up(), ss::ht_unc_up(), ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      int SR_dn = signalRegion(ss::njets_unc_dn(), ss::nbtags_unc_dn(), ss::met_unc_dn(), ss::ht_unc_dn(), ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      if (SR < 1 && SR_up < 1 && SR_dn < 1) continue; 

      //Fill
      if (SR    > 0) result[AC][0]->Fill(SR   , ss::scale1fb()*lumiAG); 
      if (SR_up > 0) result[AC][1]->Fill(SR_up, ss::scale1fb()*lumiAG); 
      if (SR_dn > 0) result[AC][2]->Fill(SR_dn, ss::scale1fb()*lumiAG); 

    }//event loop
  }//file loop

  //Return results
  return result;
}
