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
#include "lepfilter.cc"

using namespace std;
using namespace samesign;

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

  bool noPtRel = false;
  if (option.Contains("noPtRel")) noPtRel = true;

  bool useLooseEMVA = false;
  if (option.Contains("useLooseEMVA")) useLooseEMVA = true;

  bool usePtRatioCor = false;
  if (option.Contains("usePtRatioCor")) usePtRatioCor = true;

  bool useInvPtRatio = false;
  if (option.Contains("useInvPtRatio")) useInvPtRatio = true;

  bool doBonly = false;
  if (option.Contains("doBonly")) doBonly = true;

  bool doConly = false;
  if (option.Contains("doConly")) doConly = true;

  bool doLightonly = false;
  if (option.Contains("doLightonly")) doLightonly = true;

  bool onlyMuPt15 = false;
  if (option.Contains("onlyMuPt15")) onlyMuPt15 = true;

  bool onlyEM80to170 = false;
  if (option.Contains("onlyEM80to170")) onlyEM80to170 = true;

  bool onlyEM30to80 = false;
  if (option.Contains("onlyEM30to80")) onlyEM30to80 = true;

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
    ss.Init(tree);
    
    // Loop over Events in current file   //ACTUALLY A LEPTON "EVENT" LOOP
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
    
      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      ss.GetEntry(event);
      ++nEventsTotal;
    
      // Progress
      lepfilter::progress( nEventsTotal, nEventsChain );

      int evtToDebug = -999999;
      if (ss.evt_event()==evtToDebug) {
	cout << __FILE__ << " " << __LINE__ << endl;
	cout << "pt=" << ss.p4().pt() << " eta=" << ss.p4().eta() << " sip=" << fabs(ss.ip3d()/ss.ip3derr()) << " mva=" << ss.mva() << endl;
	cout << "lepp4=" << ss.p4() << " jetp4=" << ss.jet_close_lep() << endl;
      }

      // Analysis Code
      float weight = ss.scale1fb()*10.0/1000.;//use 10./pb for sync

	  if(ss.scale1fb() > 100000.) continue;  //excludes 5to10 and 10to20 EM Enriched, 15to30 non-Enriched
	  if(abs(ss.id())==13 && ss.p4().pt()<15. && ss.scale1fb() > 79. && ss.scale1fb() < 80.) continue;  //take only Mu5 below pT=15
	  if(abs(ss.id())==13 && ss.p4().pt()>15. && (ss.scale1fb() < 79. || ss.scale1fb() > 80.)) continue;  //take only Mu15 above pT=15
	  if (onlyMuPt15 && (ss.scale1fb() < 79. || ss.scale1fb() > 80.)) continue;
	  if (onlyEM80to170 && (ss.scale1fb() < 284.5521 || ss.scale1fb() > 284.5522)) continue;
	  if (onlyEM30to80  && (ss.scale1fb() < 5203.01  || ss.scale1fb() > 5203.03 )) continue;

	  //make sure we use mu from MuEnrich and el from EG+BCtoE
	  if (abs(ss.id())==13 && fabs(ss.scale1fb()-20.94)>0.1 && fabs(ss.scale1fb()-79.81)>0.1 && fabs(ss.scale1fb()-85.19)>0.1 && fabs(ss.scale1fb()-357.93)>0.1) continue;
	  if (abs(ss.id())==11 && !(fabs(ss.scale1fb()-20.94)>0.1 && fabs(ss.scale1fb()-79.81)>0.1 && fabs(ss.scale1fb()-85.19)>0.1 && fabs(ss.scale1fb()-357.93)>0.1)) continue;

	  bool jetptcut = false;
	  float ht = 0.;
	  int njets40 = 0;
	  int nbtags = 0;
	  for(unsigned int i=0; i<ss.jets().size(); i++)  {
	        if(fabs(ss.jets()[i].eta()) > 2.4) continue;
		if(ROOT::Math::VectorUtil::DeltaR(ss.jets()[i], ss.p4()) < 1.) continue; //0.4 in babymaker
		if(ss.jets_disc()[i] > 0.814) nbtags++;
		if(ss.jets()[i].pt() > 40.) {
		  ht += ss.jets()[i].pt();
		  njets40++;
		}
	  }
	  if(njets40 > 0) jetptcut = true;

	  float ptrel = ss.ptrelv1();
	  assert(fabs(ptrel - computePtRel(ss.p4(),ss.jet_close_lep(),true))<0.0001);
	  if (noPtRel && ptrel > 7.0) continue;
	  float closejetpt = ss.jet_close_lep().pt();
	  if (closejetpt<ss.p4().pt()) closejetpt=ss.p4().pt();
	  //float miniIso = ss.miniiso();
	  float relIso = ss.RelIso03EA();
	  float ptratio = closejetpt>0. ? ss.p4().pt()/closejetpt : 1.;

	  if( !(jetptcut && ss.evt_pfmet() < 20. && ss.mt() < 20) )
	  	{continue;}

	  if(ss.nFOs_SS() > 1) //if more than 1 FO in event
		{continue;}

	  // if(nbtags != 1) continue; 

	  //Ditch bounds here and just enforce correct reading of histo in getFakeRate() in app_region/ScanChain.C???
	  //If we dont want leptons w/ |eta|>2.4 in ttbar application, filling rate histos with leptons w/
	  // |eta|>2.4 will mess up the rate in the highest eta bins (2<|eta|<3)
	  //Don't think eta cut is anywhere else
	  if(ss.p4().pt() < 10. || fabs(ss.p4().eta()) > 2.5 || (abs(ss.id()==13 && fabs(ss.p4().eta()) > 2.4)) )
	  	{continue;}

	  if (doLightonly && abs(ss.id())==11 && ss.p4().pt() < 20.) continue;//because EMEnriched does not go below 20 GeV

	  if (fabs(ss.ip3d()/ss.ip3derr())>4. ) continue;

	  bool passId = ss.passes_SS_tight_v3();
	  bool passFO = ss.passes_SS_fo_v3();
	  bool passId_noiso = ss.passes_SS_tight_noiso_v3();
	  bool passFO_noiso = ss.passes_SS_fo_noiso_v3();
	  if (abs(ss.id())==11 && useLooseEMVA) {
	    passFO = ss.passes_SS_fo_looseMVA_v3();
	    passFO_noiso = ss.passes_SS_fo_looseMVA_noiso_v3();
	  }

	  if (usePtRatioCor) {
	    if (abs(ss.id())==11) {
	      float ptratiocor = closejetpt>0. ? ss.p4().pt()*(1+std::max(0.,ss.miniiso()-0.10))/closejetpt : 1.;
	      passFO = ss.passes_SS_fo_v3() && (ptratiocor > 0.70 || ptrel > 7.0);
	    } else {
	      float ptratiocor = closejetpt>0. ? ss.p4().pt()*(1+std::max(0.,ss.miniiso()-0.14))/closejetpt : 1.;
	      passFO = ss.passes_SS_fo_v3() && (ptratiocor > 0.68 || ptrel > 6.7);	      
	    }
	  } else if (useInvPtRatio) {
	    if (abs(ss.id())==11) {
	      passFO = ss.passes_SS_fo_v3() && ( (1./ptratio < (1./0.70+ss.miniiso())) /*|| ptrel > 7.0*/);
	    } else {
	      passFO = ss.passes_SS_fo_v3() && ( (1./ptratio < (1./0.68+ss.miniiso())) /*|| ptrel > 6.7*/);
	    }
	  }

	  float coneptcorr = 0.;
	  if (abs(ss.id())==11) {
	    if (ptrel>7.0) {
	      coneptcorr = std::max(0.,ss.miniiso()-0.10);
	    } else {
	      coneptcorr = max(double(0.),(ss.jet_close_lep().pt()*0.70/ss.p4().pt()-1.));
	    }
	  } else {
	    if (ptrel>6.7) {
	      coneptcorr = std::max(0.,ss.miniiso()-0.14);
	    } else {
	      coneptcorr = max(double(0.),(ss.jet_close_lep().pt()*0.68/ss.p4().pt()-1.));
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

	  if( ss.motherID() <= 0 && (doBonly==0 || ss.motherID() == -1) && (doConly==0 || ss.motherID() == -2) && (doLightonly==0 || ss.motherID() == 0) )  //if lep is nonprompt
		{

		  if( abs( ss.id() ) == 11 ) //it's an el
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

		  if( abs( ss.id() ) == 13 ) //it's a mu
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

	  if( ss.motherID() <= 0 && (doBonly==0 || ss.motherID() == -1) && (doConly==0 || ss.motherID() == -2) && (doLightonly==0 || ss.motherID() == 0) )  //if el is nonprompt (GEN info)
		{

		  if (passFO) {
		    histo_ht->Fill( std::min(ht,float(1000.)) );
		    histo_met->Fill( std::min(ss.evt_pfmet(),float(1000.)) );
		    histo_mt->Fill( std::min(ss.mt(),float(1000.)) );

		    if( abs( ss.id() ) == 11 ) pTrelvsIso_histo_el->Fill( std::min(ss.RelIso03EA(),float(0.99)), std::min(ptrel,float(29.9)) );
		    if( abs( ss.id() ) == 13 ) pTrelvsIso_histo_mu->Fill( std::min(ss.RelIso03EA(),float(0.99)), std::min(ptrel,float(29.9)) );
		    if( abs( ss.id() ) == 11 ) pTrel_histo_el->Fill( std::min(ptrel,float(29.9)) );
		    if( abs( ss.id() ) == 13 ) pTrel_histo_mu->Fill( std::min(ptrel,float(29.9)) );

		    /*
		    //TURN ON TO DEBUG LEPTONS IN SPECIFIC BINS
		    //if (ss.p4().pt()>50. && ss.p4().pt()<70. && fabs(ss.p4().eta())>2.) {
		    if (ss.p4().pt()>35. && ss.p4().pt()<50. && fabs(ss.p4().eta())>2.) {
		      char type = 'n';
		      if (ss.motherID() == -1) type = 'b'; 
		      if (ss.motherID() == -2) type = 'c'; 
		      if (ss.motherID() ==  0) type = 'l'; 
		      cout << Form("%11d %3i %6.2f %5.2f %5.2f %5.2f %5.2f %c %i %5.2f %i %i %i",
				   ss.evt_event(),ss.id(),ss.p4().pt(),ss.p4().eta(),
				   ss.miniiso(),ptrel,ptratio,type,passId,ss.mva(),
				   ss.exp_innerlayers(),ss.conv_vtx_flag(),ss.threeChargeAgree()) << endl;
		      if (ss.evt_event()==1137407433 || ss.evt_event()==1658883658) {
			cout << "close jet pt=" << ss.jet_close_lep().pt() << " eta=" << ss.jet_close_lep().eta() << " phi=" << ss.jet_close_lep().phi() << endl;
			for(unsigned int i=0; i<ss.jets().size(); i++)  {
			  if(ROOT::Math::VectorUtil::DeltaR(ss.jets()[i], ss.p4()) < 1.) continue; //0.4 in babymaker
			  if(ss.jets()[i].pt() > 40.) {
			    cout << "jet pt=" << ss.jets()[i].pt() << " eta=" <<  ss.jets()[i].eta() << " phi=" <<  ss.jets()[i].phi() << " dR(j,l)=" << ROOT::Math::VectorUtil::DeltaR(ss.jets()[i], ss.p4()) << endl;
			  }
			}
			cout << endl;
		      }
		    }
		    */

		  }

		  if( abs( ss.id() ) == 11 ) // it's an el
			{
			  if( passId )  //if el is tight
				{ 
				  //uncorrected and cone corrected FR
				  Nt_histo_e->Fill(getPt(ss.p4().pt(),false), getEta(fabs(ss.p4().eta()),ht,false), weight);   //
				  //jet corrected FR
				  Nt_jet_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  if (ss.p4().pt()>25.) Nt_jet_highpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  else Nt_jet_lowpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				}

			  if( passFO )  //if el is FO
				{
				  //not corrected FR
				  Nl_histo_e->Fill(getPt(ss.p4().pt(),false), getEta(fabs(ss.p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
				  //cone corrected FR
				  if( passId ) Nl_cone_histo_e->Fill(getPt(ss.p4().pt(),false), getEta(fabs(ss.p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
				  else Nl_cone_histo_e->Fill(getPt(ss.p4().pt()*(1+coneptcorr),false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  //jet corrected FR
				  Nl_jet_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  if (ss.p4().pt()>25.) Nl_jet_highpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  else Nl_jet_lowpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
 
				  njets40_histo->Fill(njets40, weight);

				  if (doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
					{
					  if(ss.motherID()==-1){
						NBs_BR_histo_e ->Fill(nbtags, weight);
						Bs_e = Bs_e + weight;
					  }
					  else if(ss.motherID()==-2 || ss.motherID()==0){
						NnotBs_BR_histo_e ->Fill(nbtags, weight);
						notBs_e = notBs_e + weight;
					  }
					}
				}
			}
		  if( abs( ss.id() ) == 13 ) // it's a mu
			{
			  if( passId )  //if mu is tight
				{ 
				  //uncorrected and cone corrected FR
				  Nt_histo_mu->Fill(getPt(ss.p4().pt(),false), getEta(fabs(ss.p4().eta()),ht,false), weight);   //
				  //jet corrected FR
				  Nt_jet_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  if (ss.p4().pt()>25.) Nt_jet_highpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  else Nt_jet_lowpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				}

			  if( passFO )  //if mu is FO
				{
				  //not corrected FR
				  Nl_histo_mu->Fill(getPt(ss.p4().pt(),false), getEta(fabs(ss.p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
				  //cone corrected FR
				  if( passId ) Nl_cone_histo_mu->Fill(getPt(ss.p4().pt(),false), getEta(fabs(ss.p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
				  else Nl_cone_histo_mu->Fill(getPt(ss.p4().pt()*(1+coneptcorr),false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  //jet corrected FR
				  Nl_jet_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  if (ss.p4().pt()>25.) Nl_jet_highpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);
				  else Nl_jet_lowpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(ss.p4().eta()),ht,false), weight);

				  njets40_histo->Fill(njets40, weight);

				  if (doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
					{
					  if(ss.motherID()==-1){
						NBs_BR_histo_mu ->Fill(nbtags, weight);
						Bs_mu = Bs_mu + weight;
					  }
					  else if(ss.motherID()==-2 || ss.motherID()==0){
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
