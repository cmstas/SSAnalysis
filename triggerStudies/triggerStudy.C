#include "TTree.h"
#include "TFile.h"

#include "../software/dataMCplotMaker/dataMCplotMaker.h" 
#include "SL.h"

using namespace std;

int whichFile = 2; //0-data, 1-DY, 2-DY truth

void triggerStudy(){

  //Input file and tree
  TFile *file = 0;
  if (whichFile == 0) file = new TFile("/nfs-7/userdata/leptonTree/v0.14-16p1pb/2015CDoubleMuon.root");
  if (whichFile != 0) file = new TFile("/hadoop/cms/store/user/cgeorge/condor/frbabies/merged_files/DY_madgraph.root"); 
  TTree *tree = (TTree*)file->Get("t"); 
  singlelepton.Init(tree);

  //Histograms of numer and denom
  TH1F *denom = new TH1F("denom", "denom", 50, 0, 100); 
  TH1F *numer_leading = new TH1F("numer_leading", "numer_leading", 50, 0, 100); 
  TH1F *numer_trailing = new TH1F("numer_trailing", "numer_trailing", 50, 0, 100); 
  TH1F *numer_truth = new TH1F("numer_truth", "numer_truth", 50, 0, 100); 
  TH1F *denom_truth = new TH1F("denom_truth", "denom_truth", 50, 0, 100); 
  
  //Set sumw2
  denom->Sumw2();
  denom_truth->Sumw2();
  numer_leading->Sumw2();
  numer_trailing->Sumw2();
  numer_truth->Sumw2();

  //Number of events
  int nEvents = tree->GetEntries();

  //Event loop -- T&P
  for (int event = 0; event < nEvents; event++){
 
    //Progress
    SL::progress(event, nEvents*2);

    //Read in event
    singlelepton.GetEntry(event);
    
    //If tag does not pass single lepton trigger, we're done
    if (sl::tag_HLT_IsoMu20() <= 0) continue;

    //If probe is not a good muon, we're done
	if (sl::passes_SS_tight_v3() == 0) continue;

    //Tag + Probe must form a Z, or we're done
    if (abs((sl::p4() + sl::tag_p4()).M() - 91) > 15) continue;

    //Also done if tag is not at least 30 GeV
    if (sl::tag_p4().pt() < 30) continue;

    //If we get this far, it's denom
    denom->Fill(sl::p4().pt()); 

    //Now if probe passes the trigger, put it in numer
    if (sl::probe_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_LeadingLeg()  > 0) numer_leading->Fill(sl::p4().pt()); 
    if (sl::probe_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_TrailingLeg() > 0) numer_trailing->Fill(sl::p4().pt()); 

  }

  //Event loop -- truth
  for (unsigned int event = 0; event < tree->GetEntries(); event++){

    //Progress
    SL::progress(nEvents + event, nEvents*2);

    //Read in event
    singlelepton.GetEntry(event);

    //If probe is not truly a muon, we're done
    if (abs(sl::mc_id()) != 13) continue;
    
    //If probe does not pass our selection, we're done
	if (sl::passes_SS_tight_v3() == 0) continue;

    //If we get this far, it's denom
    denom_truth->Fill(sl::p4().pt()); 

    //Now if probe passes the trigger, put it in numer
    if (sl::HLT_IsoMu20() > 0) numer_truth->Fill(sl::p4().pt()); 

  }

  //Now divide numerator and denom
  numer_trailing->Divide(denom);
  numer_leading->Divide(denom);
  numer_truth->Divide(denom_truth);

  //The result is a plot that we can plot 
  TH1F* null = new TH1F("","",1,0,1); 
  vector <TH1F*> ratios;
  string name = "data";
  if (whichFile == 1) name = "DYMC"; 
  if (whichFile == 2) name = "DYMC_truth"; 
  if (whichFile != 2) ratios.push_back(numer_leading);
  if (whichFile != 2) ratios.push_back(numer_trailing);
  if (whichFile == 2) ratios.push_back(numer_truth);
  vector <string> titles; 
  if (whichFile != 2) titles.push_back("leading leg"); 
  if (whichFile != 2) titles.push_back("trailing leg"); 
  if (whichFile == 2) titles.push_back("truth"); 
  if (whichFile != 2) dataMCplotMaker(null, ratios, titles, Form("trigger eff in %s, SS numer id", name.c_str()), "Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ", Form("--outputName diMuonTriggEff_%s --xAxisLabel probe p_{T} --isLinear --drawDots --noStack --noOverflow --setMaximum 1.7 --lumi 16.1 --lumiUnit pb", name.c_str()));
  if (whichFile == 2) dataMCplotMaker(null, ratios, titles, Form("trigger eff in %s, SS numer id", name.c_str()), "IsoMu20", Form("--outputName diMuonTriggEff_%s --xAxisLabel probe p_{T} --isLinear --drawDots --noStack --noOverflow --setMaximum 1.7 --lumi 16.1 --lumiUnit pb", name.c_str()));

}
