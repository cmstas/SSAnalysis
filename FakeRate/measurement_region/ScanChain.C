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

  TH1F *histo_mt = new TH1F("histo_mt", "MT", 20,0,1000);
  histo_mt->SetDirectory(rootdir);
  histo_mt->Sumw2();

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
      bool noMCMatch = true;
      if (isData) noMCMatch = true;

      // Analysis Code
      float weight = scale1fb()*0.0057;//10.0/1000.;//use 10./pb for sync
      if (isData) weight = 1.;

      /*
      if(isData==0 && scale1fb() > 100000.) continue;  //excludes 5to10 and 10to20 EM Enriched, 15to30 non-Enriched
      if(isData==0 && abs(id())==13 && p4().pt()<15. && scale1fb() > 79. && scale1fb() < 80.) continue;  //take only Mu15 above pT=15
      if(isData==0 && abs(id())==13 && p4().pt()>15. && (scale1fb() < 79. || scale1fb() > 80.)) continue;  //take only Mu5 below pT=15

      //make sure we use mu from MuEnrich and el from EG+BCtoE
      if (isData==0 && abs(id())==13 && fabs(scale1fb()-20.94)>0.1 && fabs(scale1fb()-79.81)>0.1 && fabs(scale1fb()-85.19)>0.1 && fabs(scale1fb()-357.93)>0.1) continue;
      if (isData==0 && abs(id())==11 && !(fabs(scale1fb()-20.94)>0.1 && fabs(scale1fb()-79.81)>0.1 && fabs(scale1fb()-85.19)>0.1 && fabs(scale1fb()-357.93)>0.1)) continue;
      */

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
      if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30()==0  && 
	  HLT_Ele12_CaloIdM_TrackIdM_PFJet30()==0 && 
	  HLT_Ele18_CaloIdM_TrackIdM_PFJet30()==0 && 
	  HLT_Ele23_CaloIdM_TrackIdM_PFJet30()==0 && 
	  HLT_Ele33_CaloIdM_TrackIdM_PFJet30()==0 &&
	  abs(id())==11) continue;
      if (HLT_Mu8()<=0  && 
	  HLT_Mu17()<=0 && 
	  HLT_Mu24()<=0 && 
	  HLT_Mu34()<=0 &&
	  abs(id())==13) continue;

      //fixme hacks for first data! add prescales when available...
      if (abs(id())==11) {
	if (isData && evt_run()!=251244 && evt_run()!=251251 && evt_run()!=251252 && evt_run()!=251561 && evt_run()!=251562) continue;
	if (HLT_Ele23_CaloIdM_TrackIdM_PFJet30()==0 && HLT_Ele33_CaloIdM_TrackIdM_PFJet30()==0) continue;
      }
      if (isData && abs(id())==13) {
	if (evt_run()==251244 || evt_run()==251251 || evt_run()==251252) {
	  if (HLT_Mu34())      weight*=37.;
	  // else if (HLT_Mu24()) weight*=140.;
	  // else if (HLT_Mu17()) weight*=1000.;
	  // else if (HLT_Mu8())  weight*=680.;
	  else continue;
	} else if (evt_run()==251559 || evt_run()==251560 || evt_run()==251561 || evt_run()==251562 || evt_run()==251883) {
	  if (HLT_Mu34())      weight*=75.;
	  // else if (HLT_Mu24()) weight*=280.;
	  else if (HLT_Mu17()) weight*=11.;
	  else if (HLT_Mu8())  weight*=520.;
	  else continue;
	} else if (evt_run()==251638 || evt_run()==251640 || evt_run()==251643 || evt_run()==251721) {
	  // if (HLT_Mu34())      weight*=112.;
	  // else if (HLT_Mu24()) weight*=420.;
	  // else 
	  if (HLT_Mu17()) weight*=15.;
	  // else if (HLT_Mu8())  weight*=800.;
	  else continue;
	} else continue; 
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
	bool passCuts = ( sIeIe<cut_sIeIe && hoe<cut_hoe && deta<cut_deta && dphi<cut_dphi && invep<cut_invep );
	passFO = passCuts && passes_SS_fo_looseMVA_v3();
	passFO_noiso = passCuts && passes_SS_fo_looseMVA_noiso_v3();
      }


      if (evt_pfmet() < 30. && passId) {
	histo_mt_lm->Fill( std::min(mt(),float(200.)), weight );
	if (abs(id()==11)) histo_mt_lm_el->Fill( std::min(mt(),float(200.)), weight );
	if (abs(id()==13)) histo_mt_lm_mu->Fill( std::min(mt(),float(200.)), weight );
      }
      if (evt_pfmet() > 30. && passId) {
	histo_mt_cr->Fill( std::min(mt(),float(200.)), weight );
	if (abs(id()==11)) histo_mt_cr_el->Fill( std::min(mt(),float(200.)), weight );
	if (abs(id()==13)) histo_mt_cr_mu->Fill( std::min(mt(),float(200.)), weight );
      }
      if( !(evt_pfmet() < 20. && mt() < 20) ) {
	continue;
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

      if( noMCMatch || (motherID() <= 0 && (doBonly==0 || motherID() == -1) && (doConly==0 || motherID() == -2) && (doLightonly==0 || motherID() == 0) ) )  //if lep is nonprompt
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
	  

      if( noMCMatch || (motherID() <= 0 && (doBonly==0 || motherID() == -1) && (doConly==0 || motherID() == -2) && (doLightonly==0 || motherID() == 0) ) )  //if el is nonprompt (GEN info)
	{

	  if (passFO) {
	    histo_ht->Fill( std::min(ht,float(1000.)) );
	    histo_met->Fill( std::min(evt_pfmet(),float(1000.)) );
	    histo_mt->Fill( std::min(mt(),float(1000.)) );

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
  histo_mt->Write();
  histo_mt_lm->Write();
  histo_mt_lm_el->Write();
  histo_mt_lm_mu->Write();
  histo_mt_cr->Write();
  histo_mt_cr_el->Write();
  histo_mt_cr_mu->Write();
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
  delete histo_mt;
  delete histo_mt_lm;
  delete histo_mt_lm_el;
  delete histo_mt_lm_mu;
  delete histo_mt_cr;
  delete histo_mt_cr_el;
  delete histo_mt_cr_mu;
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
