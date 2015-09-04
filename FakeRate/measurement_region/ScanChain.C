// Usage:
// > root -b doAll.C

// C++
#include <iostream>
#include <iomanip>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "Math/VectorUtil.h"
#include <unistd.h> //isatty

// lepfilter
#include "LeptonTree.cc"

using namespace std;
using namespace lepton_tree;

#ifdef __MAKECINT__ 
#pragma link C++ class ROOT::Math::PxPyPzE4D<float>+; 
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >+;
#endif 

float computePtRel(LorentzVector lepp4, LorentzVector jetp4, bool subtractLep) {
  if (jetp4.pt()==0) return 0.;
  if (subtractLep) jetp4-=lepp4;
  float dot = lepp4.Vect().Dot( jetp4.Vect() );
  float ptrel = lepp4.P2() - dot*dot/jetp4.P2();
  ptrel = ptrel>0 ? sqrt(ptrel) : 0.0;
  return ptrel;
}

float getPt(float pt, bool extrPtRel = false) {
  if(!extrPtRel && pt >= 70.) return 69.;
  if(extrPtRel && pt >= 150.) return 149.;
  if(pt < 10.)  return 11.;   //use this if lower FR histo bound is 10.
  return pt;
}

float getEta(float eta, float ht, bool extrPtRel = false) {
  if (extrPtRel) {
    if(ht >= 800) return 799;
    return ht;
  }
  if(fabs(eta) >= 2.4) return 2.3;
  return fabs(eta);
}

double calculateMt(const LorentzVector p4, double met, double met_phi){
  float phi1 = p4.Phi();
  float phi2 = met_phi;
  float Et1  = p4.Et();
  float Et2  = met;
  return sqrt(2*Et1*Et2*(1.0 - cos(phi1-phi2)));
}

int ScanChain( TChain* chain, TString outfile, TString option="", bool fast = true, int nEvents = -1) {//, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  //default is MultiIso no SIP

  bool useRelIso = false;
  if (option.Contains("useRelIso")) useRelIso = true;

  bool useLooseEMVA = false;
  if (option.Contains("useLooseEMVA")) useLooseEMVA = true;

  bool usePtRatioCor = false;
  if (option.Contains("usePtRatioCor")) usePtRatioCor = true;

  bool doBonly = false;
  if (option.Contains("doBonly")) doBonly = true;

  bool doConly = false;
  if (option.Contains("doConly")) doConly = true;

  bool doLightonly = false;
  if (option.Contains("doLightonly")) doLightonly = true;

  int nptbins = 5;
  int netabins = 3;
  float ptbins[6] = {10., 15., 25., 35., 50., 70.};
  float etabins[4] = {0., 1., 2., 2.4};

  if (false) {
    //this should be ok as long as there are less bins in the extrPtRel case
    ptbins[0] = 10.;
    ptbins[1] = 50.;
    ptbins[2] = 100.;
    ptbins[3] = 150.;
    nptbins = 3;
    etabins[0] = 0.;
    etabins[1] = 400.;
    etabins[2] = 800.;
    netabins = 2;
  } 


  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  TH2D *Nt_histo_e = new TH2D("Nt_histo_e", "Nt vs Pt, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nt_histo_e->SetDirectory(rootdir);
  Nt_histo_e->Sumw2();

  TH2D *Nl_histo_e = new TH2D("Nl_histo_e", "Nl vs Pt, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nl_histo_e->SetDirectory(rootdir);
  Nl_histo_e->Sumw2();

  TH2D *Nt_histo_mu = new TH2D("Nt_histo_mu", "Nt vs Pt, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nt_histo_mu->SetDirectory(rootdir);
  Nt_histo_mu->Sumw2();

  TH2D *Nl_histo_mu = new TH2D("Nl_histo_mu", "Nl vs Pt, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nl_histo_mu->SetDirectory(rootdir);
  Nl_histo_mu->Sumw2();

  TH2D *Nl_cone_histo_e = new TH2D("Nl_cone_histo_e", "Nl vs Cone Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nl_cone_histo_e->SetDirectory(rootdir);
  Nl_cone_histo_e->Sumw2();

  TH2D *Nl_cone_histo_mu = new TH2D("Nl_cone_histo_mu", "Nl vs Cone Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nl_cone_histo_mu->SetDirectory(rootdir);
  Nl_cone_histo_mu->Sumw2();

  TH2D *Nt_jet_histo_e = new TH2D("Nt_jet_histo_e", "Nt vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nt_jet_histo_e->SetDirectory(rootdir);
  Nt_jet_histo_e->Sumw2();

  TH2D *Nt_jet_histo_mu = new TH2D("Nt_jet_histo_mu", "Nt vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nt_jet_histo_mu->SetDirectory(rootdir);
  Nt_jet_histo_mu->Sumw2();

  TH2D *Nl_jet_histo_e = new TH2D("Nl_jet_histo_e", "Nl vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nl_jet_histo_e->SetDirectory(rootdir);
  Nl_jet_histo_e->Sumw2();

  TH2D *Nl_jet_histo_mu = new TH2D("Nl_jet_histo_mu", "Nl vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nl_jet_histo_mu->SetDirectory(rootdir);
  Nl_jet_histo_mu->Sumw2();

  TH2D *Nt_jet_highpt_histo_e = new TH2D("Nt_jet_highpt_histo_e", "Nt vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nt_jet_highpt_histo_e->SetDirectory(rootdir);
  Nt_jet_highpt_histo_e->Sumw2();

  TH2D *Nt_jet_highpt_histo_mu = new TH2D("Nt_jet_highpt_histo_mu", "Nt vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nt_jet_highpt_histo_mu->SetDirectory(rootdir);
  Nt_jet_highpt_histo_mu->Sumw2();

  TH2D *Nl_jet_highpt_histo_e = new TH2D("Nl_jet_highpt_histo_e", "Nl vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nl_jet_highpt_histo_e->SetDirectory(rootdir);
  Nl_jet_highpt_histo_e->Sumw2();

  TH2D *Nl_jet_highpt_histo_mu = new TH2D("Nl_jet_highpt_histo_mu", "Nl vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nl_jet_highpt_histo_mu->SetDirectory(rootdir);
  Nl_jet_highpt_histo_mu->Sumw2();

  TH2D *Nt_jet_lowpt_histo_e = new TH2D("Nt_jet_lowpt_histo_e", "Nt vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nt_jet_lowpt_histo_e->SetDirectory(rootdir);
  Nt_jet_lowpt_histo_e->Sumw2();

  TH2D *Nt_jet_lowpt_histo_mu = new TH2D("Nt_jet_lowpt_histo_mu", "Nt vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nt_jet_lowpt_histo_mu->SetDirectory(rootdir);
  Nt_jet_lowpt_histo_mu->Sumw2();

  TH2D *Nl_jet_lowpt_histo_e = new TH2D("Nl_jet_lowpt_histo_e", "Nl vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins);
  Nl_jet_lowpt_histo_e->SetDirectory(rootdir);
  Nl_jet_lowpt_histo_e->Sumw2();

  TH2D *Nl_jet_lowpt_histo_mu = new TH2D("Nl_jet_lowpt_histo_mu", "Nl vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  Nl_jet_lowpt_histo_mu->SetDirectory(rootdir);
  Nl_jet_lowpt_histo_mu->Sumw2();

  TH1F *NBs_BR_histo_e = new TH1F("NBs_BR_histo_e", "Number of FO's from B's vs Nbtags (els)", 5,0,5);
  NBs_BR_histo_e->SetDirectory(rootdir);
  NBs_BR_histo_e->Sumw2();

  TH1F *NBs_BR_histo_mu = new TH1F("NBs_BR_histo_mu", "Number of FO's from B's vs Nbtags (muons)", 5,0,5);
  NBs_BR_histo_mu->SetDirectory(rootdir);
  NBs_BR_histo_mu->Sumw2();

  TH1F *NnotBs_BR_histo_e = new TH1F("NnotBs_BR_histo_e", "Number of FO's NOT from B's vs Nbtags (els)", 5,0,5);
  NnotBs_BR_histo_e->SetDirectory(rootdir);
  NnotBs_BR_histo_e->Sumw2();

  TH1F *NnotBs_BR_histo_mu = new TH1F("NnotBs_BR_histo_mu", "Number of FO's NOT from B's vs Nbtags (muons)", 5,0,5);
  NnotBs_BR_histo_mu->SetDirectory(rootdir);
  NnotBs_BR_histo_mu->Sumw2();

  TH2D *pTrelvsIso_histo_el = new TH2D("pTrelvsIso_histo_el", "pTrel vs Iso (Electrons)", 10, 0., 1., 15, 0., 30.);
  pTrelvsIso_histo_el->SetDirectory(rootdir);
  pTrelvsIso_histo_el->Sumw2();

  TH2D *pTrelvsIso_histo_mu = new TH2D("pTrelvsIso_histo_mu", "pTrel vs Iso (Muons)", 10, 0., 1., 15, 0., 30.);
  pTrelvsIso_histo_mu->SetDirectory(rootdir);
  pTrelvsIso_histo_mu->Sumw2();

  TH1D *pTrel_histo_el = new TH1D("pTrel_histo_el", "pTrel (Electrons)", 15, 0., 30.);
  pTrel_histo_el->SetDirectory(rootdir);
  pTrel_histo_el->Sumw2();

  TH1D *pTrel_histo_mu = new TH1D("pTrel_histo_mu", "pTrel (Muons)", 15, 0., 30.);
  pTrel_histo_mu->SetDirectory(rootdir);
  pTrel_histo_mu->Sumw2();

  TH1F *histo_ht = new TH1F("histo_ht", "HT", 20,0,1000);
  histo_ht->SetDirectory(rootdir);
  histo_ht->Sumw2();

  TH1F *histo_met = new TH1F("histo_met", "MET", 20,0,1000);
  histo_met->SetDirectory(rootdir);
  histo_met->Sumw2();

  TH1F *histo_met_all = new TH1F("histo_met_all", "MET", 20,0,200);
  histo_met_all->SetDirectory(rootdir);
  histo_met_all->Sumw2();

  TH1F *histo_met_all_el = new TH1F("histo_met_all_el", "MET", 20,0,200);
  histo_met_all_el->SetDirectory(rootdir);
  histo_met_all_el->Sumw2();

  TH1F *histo_met_all_mu = new TH1F("histo_met_all_mu", "MET", 20,0,200);
  histo_met_all_mu->SetDirectory(rootdir);
  histo_met_all_mu->Sumw2();

  TH1F *histo_met_lm = new TH1F("histo_met_lm", "MET", 20,0,200);
  histo_met_lm->SetDirectory(rootdir);
  histo_met_lm->Sumw2();

  TH1F *histo_met_lm_el = new TH1F("histo_met_lm_el", "MET", 20,0,200);
  histo_met_lm_el->SetDirectory(rootdir);
  histo_met_lm_el->Sumw2();

  TH1F *histo_met_lm_mu = new TH1F("histo_met_lm_mu", "MET", 20,0,200);
  histo_met_lm_mu->SetDirectory(rootdir);
  histo_met_lm_mu->Sumw2();

  TH1F *histo_met_cr = new TH1F("histo_met_cr", "MET", 20,0,200);
  histo_met_cr->SetDirectory(rootdir);
  histo_met_cr->Sumw2();

  TH1F *histo_met_cr_el = new TH1F("histo_met_cr_el", "MET", 20,0,200);
  histo_met_cr_el->SetDirectory(rootdir);
  histo_met_cr_el->Sumw2();

  TH1F *histo_met_cr_mu = new TH1F("histo_met_cr_mu", "MET", 20,0,200);
  histo_met_cr_mu->SetDirectory(rootdir);
  histo_met_cr_mu->Sumw2();

  TH1F *histo_mt = new TH1F("histo_mt", "MT", 20,0,1000);
  histo_mt->SetDirectory(rootdir);
  histo_mt->Sumw2();

  TH1F *histo_mt_all = new TH1F("histo_mt_all", "MT", 20,0,200);
  histo_mt_all->SetDirectory(rootdir);
  histo_mt_all->Sumw2();

  TH1F *histo_mt_all_el = new TH1F("histo_mt_all_el", "MT", 20,0,200);
  histo_mt_all_el->SetDirectory(rootdir);
  histo_mt_all_el->Sumw2();

  TH1F *histo_mt_all_mu = new TH1F("histo_mt_all_mu", "MT", 20,0,200);
  histo_mt_all_mu->SetDirectory(rootdir);
  histo_mt_all_mu->Sumw2();

  TH1F *histo_mt_lm = new TH1F("histo_mt_lm", "MT", 20,0,200);
  histo_mt_lm->SetDirectory(rootdir);
  histo_mt_lm->Sumw2();

  TH1F *histo_mt_lm_el = new TH1F("histo_mt_lm_el", "MT", 20,0,200);
  histo_mt_lm_el->SetDirectory(rootdir);
  histo_mt_lm_el->Sumw2();

  TH1F *histo_mt_lm_mu = new TH1F("histo_mt_lm_mu", "MT", 20,0,200);
  histo_mt_lm_mu->SetDirectory(rootdir);
  histo_mt_lm_mu->Sumw2();

  TH1F *histo_mt_cr = new TH1F("histo_mt_cr", "MT", 20,0,200);
  histo_mt_cr->SetDirectory(rootdir);
  histo_mt_cr->Sumw2();

  TH1F *histo_mt_cr_el = new TH1F("histo_mt_cr_el", "MT", 20,0,200);
  histo_mt_cr_el->SetDirectory(rootdir);
  histo_mt_cr_el->Sumw2();

  TH1F *histo_mt_cr_mu = new TH1F("histo_mt_cr_mu", "MT", 20,0,200);
  histo_mt_cr_mu->SetDirectory(rootdir);
  histo_mt_cr_mu->Sumw2();

  TH1F *histo_pt_mu = new TH1F("histo_pt_mu", "pt mu", 100,0,200);
  histo_pt_mu->SetDirectory(rootdir);
  histo_pt_mu->Sumw2();

  TH1F *histo_pt_mu8 = new TH1F("histo_pt_mu8", "pt mu8", 100,0,200);
  histo_pt_mu8->SetDirectory(rootdir);
  histo_pt_mu8->Sumw2();

  TH1F *histo_pt_mu17 = new TH1F("histo_pt_mu17", "pt mu17", 100,0,200);
  histo_pt_mu17->SetDirectory(rootdir);
  histo_pt_mu17->Sumw2();

  TH1F *histo_pt_mu24 = new TH1F("histo_pt_mu24", "pt mu24", 100,0,200);
  histo_pt_mu24->SetDirectory(rootdir);
  histo_pt_mu24->Sumw2();

  TH1F *histo_pt_mu34 = new TH1F("histo_pt_mu34", "pt mu34", 100,0,200);
  histo_pt_mu34->SetDirectory(rootdir);
  histo_pt_mu34->Sumw2();

  TH1F *histo_pt_el = new TH1F("histo_pt_el", "pt el", 100,0,200);
  histo_pt_el->SetDirectory(rootdir);
  histo_pt_el->Sumw2();

  TH1F *histo_pt_el8 = new TH1F("histo_pt_el8", "pt el8", 100,0,200);
  histo_pt_el8->SetDirectory(rootdir);
  histo_pt_el8->Sumw2();

  TH1F *histo_pt_el12 = new TH1F("histo_pt_el12", "pt el12", 100,0,200);
  histo_pt_el12->SetDirectory(rootdir);
  histo_pt_el12->Sumw2();

  TH1F *histo_pt_el17 = new TH1F("histo_pt_el17", "pt el17", 100,0,200);
  histo_pt_el17->SetDirectory(rootdir);
  histo_pt_el17->Sumw2();

  TH1F *histo_pt_el24 = new TH1F("histo_pt_el24", "pt el24", 100,0,200);
  histo_pt_el24->SetDirectory(rootdir);
  histo_pt_el24->Sumw2();

  TH1F *histo_pt_el34 = new TH1F("histo_pt_el34", "pt el34", 100,0,200);
  histo_pt_el34->SetDirectory(rootdir);
  histo_pt_el34->Sumw2();

  TH1F *njets40_histo = new TH1F("njets40_histo", "Njets with pT > 40 GeV", 5,0,5);
  njets40_histo->SetDirectory(rootdir);
  njets40_histo->Sumw2();
  //----------------------

  //e determination
  float Nt = 0.;  //# of tight leptons
  float Nl = 0.;  //# of loose leptons
  float e = 0.;   //rate = Nt/(Nl)
  float Nt_e = 0.;
  float Nl_e = 0.; 
  float e_e = 0.;
  float Nt_mu = 0.;  
  float Nl_mu = 0.; 
  float e_mu = 0.;
  //----------------
  float Bs_e = 0.;
  float notBs_e = 0.;
  float Bs_mu = 0.;
  float notBs_mu = 0.;

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile( currentFile->GetTitle() );
    TTree *tree = (TTree*)file->Get("t");
    if(fast) TTreeCache::SetLearnEntries(10);
    if(fast) tree->SetCacheSize(128*1024*1024);
    lepton_tree_obj.Init(tree);
    
    // Loop over Events in current file   //ACTUALLY A LEPTON "EVENT" LOOP
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
    
      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      lepton_tree_obj.GetEntry(event);
      ++nEventsTotal;
    
      // Progress
      LeptonTree::progress( nEventsTotal, nEventsChain );

      //cout << "pt=" << p4().pt() << " iso=" << RelIso03EA() << endl;
      //cout << "lepp4=" << p4() << " jetp4=" << jet_close_lep() << endl;

      bool isData = evt_isRealData();
      bool noMCMatch = false;
      if (isData) noMCMatch = true;

      bool isEWK = false;
      if (TString(currentFile->GetTitle()).Contains("WJets") || TString(currentFile->GetTitle()).Contains("DY")) isEWK = true;

      // Analysis Code
      float lumi = 0.009;//in /fb
      float weight = scale1fb()*lumi;
      if (isData) weight = 1.;

      if (isData==0 && isEWK==0) {

	if (abs(id())==13) {
	  /*
	  //Map of samples and correspongding scale1fb
	  /QCD_Pt-15to20_MuEnrichedPt5    1618.8612
	  /QCD_Pt-20to30_MuEnrichedPt5    564.34503
	  /QCD_Pt-30to50_MuEnrichedPt5    334.38748
	  /QCD_Pt-50to80_MuEnrichedPt5    86.584976
	  /QCD_Pt-80to120_MuEnrichedPt5   27.320997
	  /QCD_Pt-120to170_MuEnrichedPt5  6.2567968
	  /QCD_Pt-170to300_MuEnrichedPt5  2.1951010
	  /QCD_Pt-300to470_MuEnrichedPt5  0.2039125
	  /QCD_Pt-470to600_MuEnrichedPt5  0.0409794
	  /QCD_Pt-600to800_MuEnrichedPt5  0.0126527
	  /QCD_Pt-800to1000_MuEnrichedPt5 0.0023746
	  /QCD_Pt-1000toInf_MuEnrichedPt5 0.0008180
	  /QCD_Pt-20toInf_MuEnrichedPt15 22.926769
	  */
	  if (p4().pt()<15. &&  scale1fb() > 22.9 && scale1fb() < 23.0 ) continue;  //take only Mu15 above pT=15
	  if (p4().pt()>15. && (scale1fb() < 22.9 || scale1fb() > 23.0)) continue;  //take only Mu5 below pT=15
	  if (scale1fb() < 5.0 || scale1fb() > 600.) continue; //avoid extreme ranges and weights
	}	
	
	if (abs(id())==11) {
	  /*
	  //Map of samples and correspongding scale1fb
	  /QCD_Pt_15to20_bcToE   280.49151
	  /QCD_Pt_20to30_bcToE   101.22869
	  /QCD_Pt_30to80_bcToE   79.645675
	  /QCD_Pt_80to170_bcToE  7.4107956
	  /QCD_Pt_170to250_bcToE 0.9143287
	  /QCD_Pt_250toInf_bcToE 0.2412396
	  /QCD_Pt-15to20_EMEnriched   1085.0196
	  /QCD_Pt-20to30_EMEnriched   583.37152
	  /QCD_Pt-30to50_EMEnriched   2114.1918
	  /QCD_Pt-50to80_EMEnriched   553.06378
	  /QCD_Pt-80to120_EMEnriched  43.048187
	  /QCD_Pt-120to170_EMEnriched 7.4082317
	  /QCD_Pt-170to300_EMEnriched 3.2795264
	  /QCD_Pt-300toInf_EMEnriched 0.3651889
	  */
	  //if(isData==0 && scale1fb() > 100000.) continue;  //excludes 5to10 and 10to20 EM Enriched, 15to30 non-Enriched
	  if (scale1fb() < 5.0) continue; //avoid extreme ranges and weights
	  if (scale1fb() > 280 && scale1fb() < 281) continue; 
	  if (scale1fb() > 1085 && scale1fb() < 1086) continue; 
	}

	//fixme
	//make sure we use mu from MuEnrich and el from EG+BCtoE
	// if (isData==0 && abs(id())==13 && fabs(scale1fb()-20.94)>0.1 && fabs(scale1fb()-79.81)>0.1 && fabs(scale1fb()-85.19)>0.1 && fabs(scale1fb()-357.93)>0.1) continue;
	// if (isData==0 && abs(id())==11 && !(fabs(scale1fb()-20.94)>0.1 && fabs(scale1fb()-79.81)>0.1 && fabs(scale1fb()-85.19)>0.1 && fabs(scale1fb()-357.93)>0.1)) continue;

      }

      bool jetptcut = false;
      float ht = 0.;
      int njets40 = 0;
      int nbtags = 0;
      for(unsigned int i=0; i<jets().size(); i++)  {
	if(ROOT::Math::VectorUtil::DeltaR(jets()[i], p4()) < 1.) continue; //0.4 in babymaker
	if(jets_disc()[i] > 0.814) nbtags++;
	if(jets()[i].pt() > 40.) {
	  ht += jets()[i].pt();
	  njets40++;
	}
      }
      if(njets40 > 0) jetptcut = true;
      
      float ptrel = ptrelv1();
      //cout << ptrel << " " << computePtRel(p4(),jet_close_lep(),true) << endl;
      assert(ptrel<0 || fabs(ptrel - computePtRel(p4(),jet_close_lep(),true))<0.0001);
      float closejetpt = jet_close_lep().pt();
      //float miniIso = miniiso();
      float relIso = RelIso03EA();

      if( !jetptcut )
	{continue;}

      //trigger selection (fixme add also iso triggers)
      if (abs(id())==11) {
	//fixme
	if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30()==0  && 
	    HLT_Ele12_CaloIdM_TrackIdM_PFJet30()==0 && 
	    HLT_Ele18_CaloIdM_TrackIdM_PFJet30()==0 && 
	    HLT_Ele23_CaloIdM_TrackIdM_PFJet30()==0 && 
	    HLT_Ele33_CaloIdM_TrackIdM_PFJet30()==0 ) continue;
	
    // ele12 for 2015c 25ns 
	if (HLT_Ele12_CaloIdM_TrackIdM_PFJet30()==0 ) continue;

      }
      if (abs(id())==13) {
	if (HLT_Mu8()<=0  && 
	    HLT_Mu17()<=0 && 
	    HLT_Mu24()<=0 && 
	    HLT_Mu34()<=0 ) continue;

    // mu17 for 2015c 25ns 
	if (HLT_Mu17()<=0) continue;

      }	
      //check prescales for data
      int prescale = -1;
      if (isData) {
	if (abs(id())==11) {

	  // if (p4().pt()>35 && HLT_Ele33_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele33_CaloIdM_TrackIdM_PFJet30();
	  // else if (p4().pt()<=35 && p4().pt()>25 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele23_CaloIdM_TrackIdM_PFJet30();
	  // else if (p4().pt()<=25 && p4().pt()>20 && HLT_Ele18_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele18_CaloIdM_TrackIdM_PFJet30();
	  // else if (p4().pt()<=20 && p4().pt()>15 && HLT_Ele12_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele12_CaloIdM_TrackIdM_PFJet30();
	  // else if (p4().pt()<=15 && p4().pt()>10 && HLT_Ele8_CaloIdM_TrackIdM_PFJet30()>0 ) prescale = HLT_Ele8_CaloIdM_TrackIdM_PFJet30() ;
	  // if (prescale>0) weight *= prescale;
	  // else continue;

	  // if (HLT_Ele12_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele12_CaloIdM_TrackIdM_PFJet30();
	  // if (HLT_Ele18_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele18_CaloIdM_TrackIdM_PFJet30();

	  // if (p4().pt()>25 && prescale!=1) continue;

	  // if (p4().pt()>25) continue;
	  // int prescale = 999999;
	  // if (HLT_Ele33_CaloIdM_TrackIdM_PFJet30()>0 && HLT_Ele33_CaloIdM_TrackIdM_PFJet30()<prescale) prescale = HLT_Ele33_CaloIdM_TrackIdM_PFJet30();
	  // if (HLT_Ele23_CaloIdM_TrackIdM_PFJet30()>0 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30()<prescale) prescale = HLT_Ele23_CaloIdM_TrackIdM_PFJet30();
	  // if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30()>0 && HLT_Ele8_CaloIdM_TrackIdM_PFJet30()<prescale) prescale = HLT_Ele8_CaloIdM_TrackIdM_PFJet30();

        // use ele12 FIXME
	  if (p4().pt() > 10 && HLT_Ele12_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele12_CaloIdM_TrackIdM_PFJet30();

        // // use non-(ele12 or ele18) FIXME
	  // if (p4().pt()>35 && HLT_Ele33_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele33_CaloIdM_TrackIdM_PFJet30();
	  // else if (p4().pt()<=35 && p4().pt()>25 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30()>0) prescale = HLT_Ele23_CaloIdM_TrackIdM_PFJet30();
	  // else if (p4().pt()<=25 && p4().pt()>10 && HLT_Ele8_CaloIdM_TrackIdM_PFJet30()>0 ) prescale = HLT_Ele8_CaloIdM_TrackIdM_PFJet30() ;


	  if (prescale>0) weight *= prescale;
	  else continue;
	  
	  // if ((HLT_Ele23_CaloIdM_TrackIdM_PFJet30() || HLT_Ele33_CaloIdM_TrackIdM_PFJet30()) && minPrescale>1000) {
	    // cout << evt_run() << " " << 
	    //   minPrescale << " " <<
	    //   HLT_Ele8_CaloIdM_TrackIdM_PFJet30() << " " << 
	    //   HLT_Ele12_CaloIdM_TrackIdM_PFJet30() << " " << 
	    //   HLT_Ele18_CaloIdM_TrackIdM_PFJet30() << " " << 
	    //   HLT_Ele23_CaloIdM_TrackIdM_PFJet30() << " " << 
	    //   HLT_Ele33_CaloIdM_TrackIdM_PFJet30() << " " << 
	    //   endl;
	  // }

	}
	if (abs(id())==13) {

	  // if (p4().pt()>38 && HLT_Mu34()>0) prescale = HLT_Mu34();
	  // else if (p4().pt()<=38 && p4().pt()>28 && HLT_Mu24()>0) prescale = HLT_Mu24();
	  // else if (p4().pt()<=28 && p4().pt()>20 && HLT_Mu17()>0) prescale = HLT_Mu17();
	  // else if (p4().pt()<=20 && p4().pt()>10 && HLT_Mu8() >0) prescale = HLT_Mu8();

        // use mu17 FIXME
	  if (p4().pt()>10 && HLT_Mu17()>0) prescale = HLT_Mu17();
      
        // // use non-mu17 FIXME
	  // if (p4().pt()>38 && HLT_Mu34()>0) prescale = HLT_Mu34();
	  // else if (p4().pt()<=38 && p4().pt()>28 && HLT_Mu24()>0) prescale = HLT_Mu24();
	  // else if (p4().pt()<=28 && p4().pt()>10 && HLT_Mu8() >0) prescale = HLT_Mu8();

	  if (prescale>0) weight *= prescale;
	  else continue;
	  
	}
      } 

      if(nFOs_SS() > 1) //if more than 1 FO in event
	{continue;}

      // if(nbtags != 1) continue; 

      //Ditch bounds here and just enforce correct reading of histo in getFakeRate() in app_region/ScanChain.C???
      //If we dont want leptons w/ |eta|>2.4 in ttbar application, filling rate histos with leptons w/
      // |eta|>2.4 will mess up the rate in the highest eta bins (2<|eta|<3)
      //Don't think eta cut is anywhere else
      if(p4().pt() < 10. || fabs(p4().eta()) > 2.4) //What do we want here? 
	{continue;}

      if (doLightonly && abs(id())==11 && p4().pt() < 20.) continue;//because EMEnriched does not go below 20 GeV

      if (fabs(ip3d()/ip3derr())>4. ) continue;

      bool passId = passes_SS_tight_v3();
      bool passFO = passes_SS_fo_v3();
      bool passId_noiso = passes_SS_tight_noiso_v3();
      bool passFO_noiso = passes_SS_fo_noiso_v3();
      if (useLooseEMVA && abs(id())==11) {
	bool isEB = true;
	if ( fabs(etaSC())>1.479 ) isEB = false;
	float sIeIe = sigmaIEtaIEta_full5x5();
	float hoe = hOverE();
	float deta = fabs(dEtaIn());
	float dphi = fabs(dPhiIn());
	float invep = fabs(1./ecalEnergy() - 1./p4().P());
	float cut_sIeIe = isEB ? 0.011 : 0.031;
	float cut_hoe   = 0.08;
	float cut_deta  = 0.01;
	float cut_dphi  = isEB ? 0.04 : 0.08;
	float cut_invep = 0.01;
	bool passHltCuts = ( sIeIe<cut_sIeIe && hoe<cut_hoe && deta<cut_deta && dphi<cut_dphi && invep<cut_invep );
	passFO = passHltCuts && passes_SS_fo_looseMVA_v3();
	passFO_noiso = passHltCuts && passes_SS_fo_looseMVA_noiso_v3();
      }

      float evt_met = evt_met3p0();
      float evt_metPhi = evt_met3p0Phi();
      float evt_mt = calculateMt(p4(),evt_met,evt_metPhi);

      if (passId) {
	//mt control region
	if (evt_met > 30. && p4().pt()>30) {
	  histo_mt_all->Fill( std::min(evt_mt,float(200.)), weight );
	  if (abs(id())==11) histo_mt_all_el->Fill( std::min(evt_mt,float(200.)), weight );
	  if (abs(id())==13) histo_mt_all_mu->Fill( std::min(evt_mt,float(200.)), weight );
	}
	if (evt_met < 20.) {
	  histo_mt_lm->Fill( std::min(evt_mt,float(200.)), weight );
	  if (abs(id())==11) histo_mt_lm_el->Fill( std::min(evt_mt,float(200.)), weight );
	  if (abs(id())==13) histo_mt_lm_mu->Fill( std::min(evt_mt,float(200.)), weight );
	}
	if (evt_met > 30.) {
	  histo_mt_cr->Fill( std::min(evt_mt,float(200.)), weight );
	  if (abs(id())==11) histo_mt_cr_el->Fill( std::min(evt_mt,float(200.)), weight );
	  if (abs(id())==13) histo_mt_cr_mu->Fill( std::min(evt_mt,float(200.)), weight );
	}
	//test if bad data/MC ratio in mt control region is due to met
	if (p4().pt()>30) {
	  histo_met_all->Fill( std::min(evt_met,float(200.)), weight );
	  if (abs(id())==11) histo_met_all_el->Fill( std::min(evt_met,float(200.)), weight );
	  if (abs(id())==13) histo_met_all_mu->Fill( std::min(evt_met,float(200.)), weight );
	}
	if (evt_mt < 20.) {
	  histo_met_lm->Fill( std::min(evt_met,float(200.)), weight );
	  if (abs(id())==11) histo_met_lm_el->Fill( std::min(evt_met,float(200.)), weight );
	  if (abs(id())==13) histo_met_lm_mu->Fill( std::min(evt_met,float(200.)), weight );
	}
	if (evt_mt > 30.) {
	  histo_met_cr->Fill( std::min(evt_met,float(200.)), weight );
	  if (abs(id())==11) histo_met_cr_el->Fill( std::min(evt_met,float(200.)), weight );
	  if (abs(id())==13) histo_met_cr_mu->Fill( std::min(evt_met,float(200.)), weight );
	}
      }
      if( !(evt_met < 20. && evt_mt < 20) ) {
	continue;
      }

      if (isData && passFO) {
	if (abs(id())==11) {
	  if (HLT_Ele33_CaloIdM_TrackIdM_PFJet30()>0) histo_pt_el34->Fill(p4().pt(),HLT_Ele33_CaloIdM_TrackIdM_PFJet30());
	  if (HLT_Ele23_CaloIdM_TrackIdM_PFJet30()>0) histo_pt_el24->Fill(p4().pt(),HLT_Ele23_CaloIdM_TrackIdM_PFJet30());
	  if (HLT_Ele18_CaloIdM_TrackIdM_PFJet30()>0) histo_pt_el17->Fill(p4().pt(),HLT_Ele18_CaloIdM_TrackIdM_PFJet30());
	  if (HLT_Ele12_CaloIdM_TrackIdM_PFJet30()>0) histo_pt_el12->Fill(p4().pt(),HLT_Ele12_CaloIdM_TrackIdM_PFJet30());
	  if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30()>0 ) histo_pt_el8->Fill(p4().pt() ,HLT_Ele8_CaloIdM_TrackIdM_PFJet30() );
	  histo_pt_el->Fill(p4().pt(), prescale );
	}
	if (abs(id())==13) {
	  if (HLT_Mu34()>0) histo_pt_mu34->Fill(p4().pt(),HLT_Mu34());
	  if (HLT_Mu24()>0) histo_pt_mu24->Fill(p4().pt(),HLT_Mu24());
	  if (HLT_Mu17()>0) histo_pt_mu17->Fill(p4().pt(),HLT_Mu17());
	  if (HLT_Mu8()>0 ) histo_pt_mu8->Fill(p4().pt() ,HLT_Mu8() );
	  histo_pt_mu->Fill(p4().pt(), prescale );	  
	}
      } 

      if (usePtRatioCor) {
	if (abs(id())==11) {
	  float ptratiocor = closejetpt>0. ? p4().pt()*(1+std::max(0.,miniiso()-0.10))/closejetpt : 1.;
	  passFO = passes_SS_fo_v3() && (ptratiocor > 0.70 || ptrel > 7.0);
	} else {
	  float ptratiocor = closejetpt>0. ? p4().pt()*(1+std::max(0.,miniiso()-0.14))/closejetpt : 1.;
	  passFO = passes_SS_fo_v3() && (ptratiocor > 0.68 || ptrel > 6.7);	      
	}
      }

      float coneptcorr = 0.;
      if (abs(id())==11) {
	if (ptrel>7.0) {
	  coneptcorr = std::max(0.,miniiso()-0.10);
	} else {
	  coneptcorr = max(double(0.),(jet_close_lep().pt()*0.70/p4().pt()-1.));
	}
      } else {
	if (ptrel>6.7) {
	  coneptcorr = std::max(0.,miniiso()-0.14);
	} else {
	  coneptcorr = max(double(0.),(jet_close_lep().pt()*0.68/p4().pt()-1.));
	}
      }
      if (useRelIso) {
	passId = passId_noiso && relIso<0.1;
	passFO = passFO_noiso && relIso<0.5;
	coneptcorr = std::max(0.,relIso-0.1);
      } 

      //------------------------------------------------------------------------------------------
      //---------------------------------Find e = f(const)---------------------------------------
      //------------------------------------------------------------------------------------------

      //Find ratio of nonprompt leps passing tight to nonprompt leps passing at least loose.  This is the fake rate 
      // Use lep_passes_id to see if num.  Use FO to see if Fakable Object (denom)
      //Calculate e=Nt/(Nl) where l->loose  (as opposed to loose-not-tight).
	  
      //Using gen level info to see if prompt -> no prompt contamination in measurement region
      //everything else is RECO (p4, id, passes_id, FO, etc.)

      if( noMCMatch || (isEWK && motherID() == 1) || (motherID() <= 0 && (doBonly==0 || motherID() == -1) && (doConly==0 || motherID() == -2) && (doLightonly==0 || motherID() == 0) ) )  //if lep is nonprompt
	{

	  if( abs( id() ) == 11 ) //it's an el
	    {
	      if( passId )  //if el is tight
		{ 
		  Nt = Nt + weight;
		  Nt_e = Nt_e + weight;
		}
	      if( passFO )
		{
		  Nl = Nl + weight;     //l now means loose, as opposed to loose-not-tight
		  Nl_e = Nl_e + weight;
		}
	    }

	  if( abs( id() ) == 13 ) //it's a mu
	    {
	      if( passId )  //if mu is tight
		{ 
		  Nt = Nt + weight;
		  Nt_mu = Nt_mu + weight;
		}
	      if( passFO )
		{
		  Nl = Nl + weight;     //l now means loose, as opposed to loose-not-tight
		  Nl_mu = Nl_mu + weight;
		}
	    }
	} 

      //------------------------------------------------------------------------------------------
      //---------------------------------Find e = f(Pt,eta)---------------------------------------
      //------------------------------------------------------------------------------------------

      //Find ratio of nonprompt leps passing tight to nonprompt leps passing at least loose.  This is the fake rate 
      // Use lep_passes_id to see if num.  Use FO to see if Fakable Object (denom)
      //Calculate e=Nt/(Nl) where l->loose  (as opposed to loose-not-tight).
	  
      //Using gen level info to see if prompt -> no prompt contamination in measurement region
      //everything else is RECO (p4, id, passes_id, FO, etc.)
	  

      if( noMCMatch || (isEWK && motherID() == 1) || (motherID() <= 0 && (doBonly==0 || motherID() == -1) && (doConly==0 || motherID() == -2) && (doLightonly==0 || motherID() == 0) ) )  //if el is nonprompt (GEN info)
	{

	  if (passFO) {
	    histo_ht->Fill( std::min(ht,float(1000.)) );
	    histo_met->Fill( std::min(evt_met,float(1000.)) );
	    histo_mt->Fill( std::min(evt_mt,float(1000.)) );

	    if( abs( id() ) == 11 ) pTrelvsIso_histo_el->Fill( std::min(RelIso03EA(),float(0.99)), std::min(ptrel,float(29.9)) );
	    if( abs( id() ) == 13 ) pTrelvsIso_histo_mu->Fill( std::min(RelIso03EA(),float(0.99)), std::min(ptrel,float(29.9)) );
	    if( abs( id() ) == 11 ) pTrel_histo_el->Fill( std::min(ptrel,float(29.9)) );
	    if( abs( id() ) == 13 ) pTrel_histo_mu->Fill( std::min(ptrel,float(29.9)) );
	  }

	  if( abs( id() ) == 11 ) // it's an el
	    {
	      if( passId )  //if el is tight
		{ 
		  //uncorrected and cone corrected FR
		  Nt_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //
		  //jet corrected FR
		  Nt_jet_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		  if (p4().pt()>25.) Nt_jet_highpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		  else Nt_jet_lowpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		}

	      if( passFO )  //if el is FO
		{
		  //not corrected FR
		  Nl_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
		  //cone corrected FR
		  if( passId ) Nl_cone_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
		  else Nl_cone_histo_e->Fill(getPt(p4().pt()*(1+coneptcorr),false), getEta(fabs(p4().eta()),ht,false), weight);
		  //jet corrected FR
		  Nl_jet_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		  if (p4().pt()>25.) Nl_jet_highpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		  else Nl_jet_lowpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
 
		  njets40_histo->Fill(njets40, weight);

		  if (noMCMatch==0 && doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
		    {
		      if(motherID()==-1){
			NBs_BR_histo_e ->Fill(nbtags, weight);
			Bs_e = Bs_e + weight;
		      }
		      else if(motherID()==-2 || motherID()==0){
			NnotBs_BR_histo_e ->Fill(nbtags, weight);
			notBs_e = notBs_e + weight;
		      }
		    }
		}
	    }
	  if( abs( id() ) == 13 ) // it's a mu
	    {
	      if( passId )  //if mu is tight
		{ 
		  //uncorrected and cone corrected FR
		  Nt_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //
		  //jet corrected FR
		  Nt_jet_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		  if (p4().pt()>25.) Nt_jet_highpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		  else Nt_jet_lowpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		}

	      if( passFO )  //if mu is FO
		{
		  //not corrected FR
		  Nl_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
		  //cone corrected FR
		  if( passId ) Nl_cone_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
		  else Nl_cone_histo_mu->Fill(getPt(p4().pt()*(1+coneptcorr),false), getEta(fabs(p4().eta()),ht,false), weight);
		  //jet corrected FR
		  Nl_jet_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		  if (p4().pt()>25.) Nl_jet_highpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
		  else Nl_jet_lowpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);

		  njets40_histo->Fill(njets40, weight);

		  if (noMCMatch==0 && doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
		    {
		      if(motherID()==-1){
			NBs_BR_histo_mu ->Fill(nbtags, weight);
			Bs_mu = Bs_mu + weight;
		      }
		      else if(motherID()==-2 || motherID()==0){
			NnotBs_BR_histo_mu ->Fill(nbtags, weight);
			notBs_mu = notBs_mu + weight;
		      }
		    }
		}
	    }
	} 

      //---------------------------------------------------------------------------------------------------------------------------

    }//end event loop
  
    // Clean Up
    delete tree;
    file->Close();
    delete file;
  }
  if ( nEventsChain != nEventsTotal ) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }

  e = Nt/(Nl);
  e_e = Nt_e/(Nl_e);
  e_mu = Nt_mu/(Nl_mu);

  cout<<"\nReco: "<<"Nt = "<<Nt<<", Nl = "<<Nl<<", e ="<<e<<endl;
  cout<<"\nReco (el): "<<"Nt = "<<Nt_e<<", Nl = "<<Nl_e<<", e ="<<e_e<<endl;
  cout<<"\nReco (mu): "<<"Nt = "<<Nt_mu<<", Nl = "<<Nl_mu<<", e ="<<e_mu<<endl<<endl;
  cout<<"\nAve B abundance (els)= "<<Bs_e/(Bs_e + notBs_e)<<endl;
  cout<<"Ave B abundance (mus)= "<<Bs_mu/(Bs_mu + notBs_mu)<<endl;

  //Histograms
  // TH2D *rate_histo = (TH2D*) Nt_histo->Clone("rate_histo");
  TH2D *rate_histo_e = (TH2D*) Nt_histo_e->Clone("rate_histo_e");
  TH2D *rate_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_histo_mu");
  TH2D *rate_cone_histo_e = (TH2D*) Nt_histo_e->Clone("rate_cone_histo_e");
  TH2D *rate_cone_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_cone_histo_mu");
  TH2D *rate_jet_histo_e = (TH2D*) Nt_jet_histo_e->Clone("rate_jet_histo_e");
  TH2D *rate_jet_histo_mu = (TH2D*) Nt_jet_histo_mu->Clone("rate_jet_histo_mu");
  TH2D *rate_jet_highpt_histo_e = (TH2D*) Nt_jet_highpt_histo_e->Clone("rate_jet_highpt_histo_e");
  TH2D *rate_jet_highpt_histo_mu = (TH2D*) Nt_jet_highpt_histo_mu->Clone("rate_jet_highpt_histo_mu");
  TH2D *rate_jet_lowpt_histo_e = (TH2D*) Nt_jet_lowpt_histo_e->Clone("rate_jet_lowpt_histo_e");
  TH2D *rate_jet_lowpt_histo_mu = (TH2D*) Nt_jet_lowpt_histo_mu->Clone("rate_jet_lowpt_histo_mu");
  TH1F *total_BR_histo_e = (TH1F*) NBs_BR_histo_e->Clone("total_BR_histo_e");
  TH1F *total_BR_histo_mu = (TH1F*) NBs_BR_histo_mu->Clone("total_BR_histo_mu");

  rate_histo_e->Divide(rate_histo_e,Nl_histo_e,1,1,"B");
  rate_histo_mu->Divide(rate_histo_mu,Nl_histo_mu,1,1,"B");
  rate_cone_histo_e->Divide(rate_cone_histo_e,Nl_cone_histo_e,1,1,"B");
  rate_cone_histo_mu->Divide(rate_cone_histo_mu,Nl_cone_histo_mu,1,1,"B");
  rate_jet_histo_e->Divide(rate_jet_histo_e,Nl_jet_histo_e,1,1,"B");
  rate_jet_histo_mu->Divide(rate_jet_histo_mu,Nl_jet_histo_mu,1,1,"B");
  rate_jet_highpt_histo_e->Divide(rate_jet_highpt_histo_e,Nl_jet_highpt_histo_e,1,1,"B");
  rate_jet_highpt_histo_mu->Divide(rate_jet_highpt_histo_mu,Nl_jet_highpt_histo_mu,1,1,"B");
  rate_jet_lowpt_histo_e->Divide(rate_jet_lowpt_histo_e,Nl_jet_lowpt_histo_e,1,1,"B");
  rate_jet_lowpt_histo_mu->Divide(rate_jet_lowpt_histo_mu,Nl_jet_lowpt_histo_mu,1,1,"B");
  total_BR_histo_e->Add(NnotBs_BR_histo_e);
  total_BR_histo_mu->Add(NnotBs_BR_histo_mu);
  NBs_BR_histo_e->Divide(NBs_BR_histo_e, total_BR_histo_e,1,1,"B");
  NBs_BR_histo_mu->Divide(NBs_BR_histo_mu, total_BR_histo_mu,1,1,"B");

  rate_histo_e->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_histo_e->GetYaxis()->SetTitle("eta");
  rate_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_histo_e->SetTitle("Fake Rate vs Pt, Eta (electrons)");
  rate_histo_mu->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_histo_mu->GetYaxis()->SetTitle("eta");
  rate_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_histo_mu->SetTitle("Fake Rate vs Pt, Eta (muons)");
  rate_cone_histo_e->GetXaxis()->SetTitle("corrected pT (GeV)"); 
  rate_cone_histo_e->GetYaxis()->SetTitle("eta");
  rate_cone_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_cone_histo_e->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (electrons)");
  rate_cone_histo_mu->GetXaxis()->SetTitle("corrected pT (GeV)"); 
  rate_cone_histo_mu->GetYaxis()->SetTitle("eta");
  rate_cone_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_cone_histo_mu->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (muons)");
  rate_jet_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)"); 
  rate_jet_histo_e->GetYaxis()->SetTitle("eta");
  rate_jet_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
  rate_jet_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)"); 
  rate_jet_histo_mu->GetYaxis()->SetTitle("eta");
  rate_jet_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
  rate_jet_highpt_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)"); 
  rate_jet_highpt_histo_e->GetYaxis()->SetTitle("eta");
  rate_jet_highpt_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_highpt_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
  rate_jet_highpt_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)"); 
  rate_jet_highpt_histo_mu->GetYaxis()->SetTitle("eta");
  rate_jet_highpt_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_highpt_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
  rate_jet_lowpt_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)"); 
  rate_jet_lowpt_histo_e->GetYaxis()->SetTitle("eta");
  rate_jet_lowpt_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_lowpt_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
  rate_jet_lowpt_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)"); 
  rate_jet_lowpt_histo_mu->GetYaxis()->SetTitle("eta");
  rate_jet_lowpt_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_lowpt_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
  NBs_BR_histo_e->GetXaxis()->SetTitle("Nbjets"); 
  NBs_BR_histo_e->GetYaxis()->SetTitle("Abundance");
  NBs_BR_histo_e->GetYaxis()->SetRangeUser(0., 1.);
  NBs_BR_histo_e->SetTitle("B Abundance vs Nbtags (electrons)");
  NBs_BR_histo_mu->GetXaxis()->SetTitle("Nbjets"); 
  NBs_BR_histo_mu->GetYaxis()->SetTitle("Abundance");
  NBs_BR_histo_mu->GetYaxis()->SetRangeUser(0., 1.);
  NBs_BR_histo_mu->SetTitle("B Abundance vs Nbtags (muons)");
  pTrelvsIso_histo_el->GetXaxis()->SetTitle("Iso");
  pTrelvsIso_histo_el->GetYaxis()->SetTitle("pTrel");
  pTrelvsIso_histo_mu->GetXaxis()->SetTitle("Iso");
  pTrelvsIso_histo_mu->GetYaxis()->SetTitle("pTrel");
  njets40_histo->GetXaxis()->SetTitle("Njets");
  njets40_histo->GetYaxis()->SetTitle("Events");
  njets40_histo->SetTitle("Njets with pT > 40 GeV");

  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("1.3f");

  TCanvas *c11=new TCanvas("c11","B Abundance vs Nbjets (electrons)",800,800);
  c11->cd();
  NBs_BR_histo_e->Draw("histE");
  TCanvas *c12=new TCanvas("c12","B Abundance vs Nbjets (muons)",800,800);
  c12->cd();
  NBs_BR_histo_mu->Draw("histE");
  TCanvas *c13=new TCanvas("c13","Njets with pT > 40 GeV",800,800);
  c13->cd();
  njets40_histo->Draw("histE");

  //---save histos-------//
  TFile *OutputFile = new TFile(outfile,"recreate");
  OutputFile->cd();
  Nl_histo_e->Write();
  Nl_histo_mu->Write();
  Nt_histo_e->Write();
  Nt_histo_mu->Write();
  rate_histo_e->Write();
  rate_histo_mu->Write();
  Nl_cone_histo_e->Write();
  Nl_cone_histo_mu->Write();
  rate_cone_histo_e->Write();
  rate_cone_histo_mu->Write();
  Nl_jet_histo_e->Write();
  Nl_jet_histo_mu->Write();
  Nt_jet_histo_e->Write();
  Nt_jet_histo_mu->Write();
  rate_jet_histo_e->Write();
  rate_jet_histo_mu->Write();
  Nl_jet_highpt_histo_e->Write();
  Nl_jet_highpt_histo_mu->Write();
  Nt_jet_highpt_histo_e->Write();
  Nt_jet_highpt_histo_mu->Write();
  rate_jet_highpt_histo_e->Write();
  rate_jet_highpt_histo_mu->Write();
  Nl_jet_lowpt_histo_e->Write();
  Nl_jet_lowpt_histo_mu->Write();
  Nt_jet_lowpt_histo_e->Write();
  Nt_jet_lowpt_histo_mu->Write();
  rate_jet_lowpt_histo_e->Write();
  rate_jet_lowpt_histo_mu->Write();
  NBs_BR_histo_e->Write();
  NBs_BR_histo_mu->Write();  
  pTrelvsIso_histo_el->Write();
  pTrelvsIso_histo_mu->Write();
  pTrel_histo_el->Write();
  pTrel_histo_mu->Write();
  histo_ht->Write();
  histo_met->Write();
  histo_met_all->Write();
  histo_met_all_el->Write();
  histo_met_all_mu->Write();
  histo_met_lm->Write();
  histo_met_lm_el->Write();
  histo_met_lm_mu->Write();
  histo_met_cr->Write();
  histo_met_cr_el->Write();
  histo_met_cr_mu->Write();
  histo_mt->Write();
  histo_mt_all->Write();
  histo_mt_all_el->Write();
  histo_mt_all_mu->Write();
  histo_mt_lm->Write();
  histo_mt_lm_el->Write();
  histo_mt_lm_mu->Write();
  histo_mt_cr->Write();
  histo_mt_cr_el->Write();
  histo_mt_cr_mu->Write();
  histo_pt_mu34->Write();
  histo_pt_mu24->Write();
  histo_pt_mu17->Write();
  histo_pt_mu8->Write();
  histo_pt_mu->Write();
  histo_pt_el34->Write();
  histo_pt_el24->Write();
  histo_pt_el17->Write();
  histo_pt_el12->Write();
  histo_pt_el8->Write();
  histo_pt_el->Write();
  njets40_histo->Write();
  OutputFile->Close();

  delete Nt_histo_e;
  delete Nl_histo_e;
  delete Nt_histo_mu;
  delete Nl_histo_mu;
  delete rate_histo_e;
  delete rate_histo_mu;
  delete Nl_cone_histo_e;
  delete Nl_cone_histo_mu;
  delete rate_cone_histo_e;
  delete rate_cone_histo_mu;
  delete Nl_jet_histo_e;
  delete Nl_jet_histo_mu;
  delete Nt_jet_histo_e;
  delete Nt_jet_histo_mu;
  delete rate_jet_histo_e;
  delete rate_jet_histo_mu;
  delete Nl_jet_highpt_histo_e;
  delete Nl_jet_highpt_histo_mu;
  delete Nt_jet_highpt_histo_e;
  delete Nt_jet_highpt_histo_mu;
  delete rate_jet_highpt_histo_e;
  delete rate_jet_highpt_histo_mu;
  delete Nl_jet_lowpt_histo_e;
  delete Nl_jet_lowpt_histo_mu;
  delete Nt_jet_lowpt_histo_e;
  delete Nt_jet_lowpt_histo_mu;
  delete rate_jet_lowpt_histo_e;
  delete rate_jet_lowpt_histo_mu;
  delete NBs_BR_histo_e;
  delete NBs_BR_histo_mu;
  delete NnotBs_BR_histo_e;
  delete NnotBs_BR_histo_mu;
  delete pTrelvsIso_histo_el;
  delete pTrelvsIso_histo_mu;
  delete pTrel_histo_el;
  delete pTrel_histo_mu;
  delete histo_ht;
  delete histo_met;
  delete histo_met_all;
  delete histo_met_all_el;
  delete histo_met_all_mu;
  delete histo_met_lm;
  delete histo_met_lm_el;
  delete histo_met_lm_mu;
  delete histo_met_cr;
  delete histo_met_cr_el;
  delete histo_met_cr_mu;
  delete histo_mt;
  delete histo_mt_all;
  delete histo_mt_all_el;
  delete histo_mt_all_mu;
  delete histo_mt_lm;
  delete histo_mt_lm_el;
  delete histo_mt_lm_mu;
  delete histo_mt_cr;
  delete histo_mt_cr_el;
  delete histo_mt_cr_mu;
  delete histo_pt_mu34;
  delete histo_pt_mu24;
  delete histo_pt_mu17;
  delete histo_pt_mu8 ;
  delete histo_pt_mu  ;
  delete histo_pt_el34;
  delete histo_pt_el24;
  delete histo_pt_el17;
  delete histo_pt_el12;
  delete histo_pt_el8 ;
  delete histo_pt_el  ;
  delete njets40_histo;

  // return
  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}
