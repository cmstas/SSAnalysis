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

// lepfilter
#include "lepfilter.cc"

using namespace std;
using namespace samesign;

float getPt(float pt, bool extrPtRel) {
  if(!extrPtRel && pt >= 70.) return 69.;
  if(extrPtRel && pt >= 150.) return 149.;
  if(pt < 10.)  return 11.;   //use this if lower FR histo bound is 10.
  return pt;
}

float getEta(float eta, float ht, bool extrPtRel) {
  if (extrPtRel) {
    if(ht >= 800) return 799;
    return ht;
  }
  if(fabs(eta) >= 2.4) return 2.3;
  return fabs(eta);
}

int ScanChain( TChain* chain, TString outfile, TString option="", bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  bool noSIP = false;
  if (option.Contains("noSIP")) noSIP = true;

  bool unIso = false;
  if (option.Contains("unIso")) unIso = true;

  bool usePtRel = false;
  if (option.Contains("ptRel")) usePtRel = true;

  bool doBonly = false;
  if (option.Contains("doBonly")) doBonly = true;

  bool doConly = false;
  if (option.Contains("doConly")) doConly = true;

  bool doLightonly = false;
  if (option.Contains("doLightonly")) doLightonly = true;

  bool lowPtRel = false;
  if (option.Contains("lowPtRel")) lowPtRel = true;

  bool extrPtRel = false;
  if (option.Contains("extrPtRel")) {
    extrPtRel = true;
    usePtRel = true;
    unIso = true;
    noSIP = true;
  }

  int nptbins = 5;
  int netabins = 3;
  float ptbins[6] = {10., 15., 25., 35., 50., 70.};
  float etabins[4] = {0., 1., 2., 2.4};

  if (extrPtRel) {
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

  TH2D *Nt_histo = new TH2D("Nt_histo", "Nt vs Pt, Eta", nptbins,ptbins,netabins,etabins);
  Nt_histo->SetDirectory(rootdir);
  Nt_histo->Sumw2();

  TH2D *Nl_histo = new TH2D("Nl_histo", "Nl vs Pt, Eta", nptbins,ptbins,netabins,etabins);
  Nl_histo->SetDirectory(rootdir);
  Nl_histo->Sumw2();

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

  TH2D *NBs_histo_e = new TH2D("NBs_histo_e", "Number of FO's from B's vs pT, Eta (els)", nptbins,ptbins,netabins,etabins);
  NBs_histo_e->SetDirectory(rootdir);
  NBs_histo_e->Sumw2();

  TH2D *NBs_histo_mu = new TH2D("NBs_histo_mu", "Number of FO's from B's vs pT, Eta (muons)", nptbins,ptbins,netabins,etabins);
  NBs_histo_mu->SetDirectory(rootdir);
  NBs_histo_mu->Sumw2();

  TH2D *NnotBs_histo_e = new TH2D("NnotBs_histo_e", "Number of FO's NOT from B's vs pT, Eta (els)", nptbins,ptbins,netabins,etabins);
  NnotBs_histo_e->SetDirectory(rootdir);
  NnotBs_histo_e->Sumw2();

  TH2D *NnotBs_histo_mu = new TH2D("NnotBs_histo_mu", "Number of FO's NOT from B's vs pT, Eta (muons)", nptbins,ptbins,netabins,etabins);
  NnotBs_histo_mu->SetDirectory(rootdir);
  NnotBs_histo_mu->Sumw2();

  TH2D *NBs_cone_histo_e = new TH2D("NBs_cone_histo_e", "Number of FO's from B's vs Cone Energy, Eta (els)", nptbins,ptbins,netabins,etabins);
  NBs_cone_histo_e->SetDirectory(rootdir);
  NBs_cone_histo_e->Sumw2();

  TH2D *NBs_cone_histo_mu = new TH2D("NBs_cone_histo_mu", "Number of FO's from B's vs Cone Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  NBs_cone_histo_mu->SetDirectory(rootdir);
  NBs_cone_histo_mu->Sumw2();

  TH2D *NnotBs_cone_histo_e = new TH2D("NnotBs_cone_histo_e", "Number of FO's NOT from B's vs Cone Energy, Eta (els)", nptbins,ptbins,netabins,etabins);
  NnotBs_cone_histo_e->SetDirectory(rootdir);
  NnotBs_cone_histo_e->Sumw2();

  TH2D *NnotBs_cone_histo_mu = new TH2D("NnotBs_cone_histo_mu", "Number of FO's NOT from B's vs Cone Energy, Eta (muons)", nptbins,ptbins,netabins,etabins);
  NnotBs_cone_histo_mu->SetDirectory(rootdir);
  NnotBs_cone_histo_mu->Sumw2();

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
  float Bs_e;
  float notBs_e;
  float Bs_mu;
  float notBs_mu;

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
	  
      // Analysis Code
	  float weight = ss.scale1fb()*10.0;
	  if(ss.scale1fb() > 100000.) continue;  //excludes 5to10 and 10to20 EM Enriched, 15to30 non-Enriched
	  
	  bool jetptcut = false;
	  float ht = 0.;
	  int njets40 = 0;
	  int nbtags = 0;
	  for(int i=0; i<ss.jets().size(); i++)  {
		if(ROOT::Math::VectorUtil::DeltaR(ss.jets()[i], ss.p4()) < 1.) continue; //0.4 in babymaker
		if(ss.jets_disc()[i] > 0.814) nbtags++;
		if(ss.jets()[i].pt() > 40.) {
		  ht += ss.jets()[i].pt();
		  njets40++;
		}
	  }
	  if(njets40 > 0) jetptcut = true;
	  
	  if( !(jetptcut && (extrPtRel || (ss.met() < 20. && ss.mt() < 20)) ) )//cheat to increase stats in ptrel case
	  	{continue;}

	  if(ss.nFOs() > 1) //if more than 1 FO in event
		{continue;}

	  // if(ht < 200.) continue;
	  // //if(ss.ht() < 200.) continue;  //have to recalc ht
   
	  // if(nbtags != 1) continue; 

	  //Ditch bounds here and just enforce correct reading of histo in getFakeRate() in app_region/ScanChain.C???
	  //If we dont want leptons w/ |eta|>2.4 in ttbar application, filling rate histos with leptons w/
	  // |eta|>2.4 will mess up the rate in the highest eta bins (2<|eta|<3)
	  //Don't think eta cut is anywhere else
	  if(ss.p4().pt() < 10. || fabs(ss.p4().eta()) > 2.4) //What do we want here? 
	  	{continue;}

	  if (doLightonly && abs(ss.id())==11 && ss.p4().pt() < 20.) continue;//because EMEnriched does not go below 20 GeV

          if (lowPtRel && ss.ptrelv1()>14.) continue;

	  if (unIso && ss.iso()<=0.1 ) continue;

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

		  bool passId = ( usePtRel ? ss.passes_id_ptrel() : ss.passes_id() );
		  bool passFO = ( usePtRel ? ss.FO_ptrel() : ss.FO() );

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

		  bool passId = ( usePtRel ? ss.passes_id_ptrel() : ss.passes_id() );
		  bool passFO = ( usePtRel ? ss.FO_ptrel() : ss.FO() );

		  if (extrPtRel) {
		    passId = (ss.FO_NoIso() && ss.ptrelv1()>14. && fabs(ss.ip3d()/ss.ip3derr())<4. && ss.iso()>0.1);
		    passFO = (ss.FO_NoIso() && ss.ptrelv1()>6.0 && fabs(ss.ip3d()/ss.ip3derr())<4. && ss.iso()>0.1);
		  }

		  if (passFO) {
		    histo_ht->Fill( std::min(ht,float(1000.)) );
		    histo_met->Fill( std::min(ss.met(),float(1000.)) );
		    histo_mt->Fill( std::min(ss.mt(),float(1000.)) );

		    if( abs( ss.id() ) == 11 ) pTrelvsIso_histo_el->Fill( std::min(ss.iso(),float(0.99)), std::min(ss.ptrelv1(),float(29.9)) );
		    if( abs( ss.id() ) == 13 ) pTrelvsIso_histo_mu->Fill( std::min(ss.iso(),float(0.99)), std::min(ss.ptrelv1(),float(29.9)) );
		    if( abs( ss.id() ) == 11 ) pTrel_histo_el->Fill( std::min(ss.ptrelv1(),float(29.9)) );
		    if( abs( ss.id() ) == 13 ) pTrel_histo_mu->Fill( std::min(ss.ptrelv1(),float(29.9)) );
		  }

		  if( abs( ss.id() ) == 11 ) // it's an el
			{
			  if( passId )  //if el is tight
				{ 
				  Nt_histo->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);     //fill histo with fake pt, eta
				  Nt_histo_e->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);   //
				}

			  if( passFO )  //if el is FO
			  //if( passId )  //ONLY USE FOR NUMERATOR ABUNDANCE PLOTS!
				{
				  if (noSIP && fabs(ss.ip3d()/ss.ip3derr())>4. ) continue;
				  Nl_histo->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);     //fill histo with fake pt, eta 
				  Nl_histo_e->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);   //  <-- loose (as opposed to l!t)			
				  if( ss.passes_id() ) Nl_cone_histo_e->Fill(ss.p4().pt(), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);   //  <-- loose (as opposed to l!t)			
				  else Nl_cone_histo_e->Fill(ss.p4().pt()+ss.p4().pt()*std::max(0.,ss.iso()-0.1), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
				  njets40_histo->Fill(njets40, weight);

				  if (doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
					{
					  if(ss.motherID()==-1){
						NBs_histo_e->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel),weight);
						if( ss.passes_id() ) NBs_cone_histo_e->Fill(ss.p4().pt(), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
						else NBs_cone_histo_e->Fill(ss.p4().pt()+ss.p4().pt()*std::max(0.,ss.iso()-0.1), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
						NBs_BR_histo_e ->Fill(nbtags, weight);
						Bs_e = Bs_e + weight;
					  }
					  else if(ss.motherID()==-2 || ss.motherID()==0){
						NnotBs_histo_e->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel),weight);
						if( ss.passes_id() ) NnotBs_cone_histo_e->Fill(ss.p4().pt(), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
						else NnotBs_cone_histo_e->Fill(ss.p4().pt()+ss.p4().pt()*std::max(0.,ss.iso()-0.1), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
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
				  Nt_histo->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);     //fill histo with fake pt, eta
				  Nt_histo_mu->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);   //
				}

			  if( passFO )  //if mu is FO
			  //if( passId )  //ONLY USE FOR NUMERATOR ABUNDANCE PLOTS!
				{
				  if (noSIP && fabs(ss.ip3d()/ss.ip3derr())>4. ) continue;
				  Nl_histo->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);     //fill histo with fake pt, eta 
				  Nl_histo_mu->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);   //  <-- loose (as opposed to l!t)			
				  if( ss.passes_id() ) Nl_cone_histo_mu->Fill(ss.p4().pt(), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);   //  <-- loose (as opposed to l!t)			
				  else Nl_cone_histo_mu->Fill(ss.p4().pt()+ss.p4().pt()*std::max(0.,ss.iso()-0.1), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
				  njets40_histo->Fill(njets40, weight);

				  if (doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
					{
					  if(ss.motherID()==-1){
						NBs_histo_mu->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel),weight);
						if( ss.passes_id() ) NBs_cone_histo_mu->Fill(ss.p4().pt(), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
						else NBs_cone_histo_mu->Fill(ss.p4().pt()+ss.p4().pt()*std::max(0.,ss.iso()-0.1), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
						NBs_BR_histo_mu ->Fill(nbtags, weight);
						Bs_mu = Bs_mu + weight;
					  }
					  else if(ss.motherID()==-2 || ss.motherID()==0){
						NnotBs_histo_mu->Fill(getPt(ss.p4().pt(),extrPtRel), getEta(fabs(ss.p4().eta()),ht,extrPtRel),weight);
						if( ss.passes_id() ) NnotBs_cone_histo_mu->Fill(ss.p4().pt(), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
						else NnotBs_cone_histo_mu->Fill(ss.p4().pt()+ss.p4().pt()*std::max(0.,ss.iso()-0.1), getEta(fabs(ss.p4().eta()),ht,extrPtRel), weight);
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
  TH2D *rate_histo = (TH2D*) Nt_histo->Clone("rate_histo");
  TH2D *rate_histo_e = (TH2D*) Nt_histo_e->Clone("rate_histo_e");
  TH2D *rate_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_histo_mu");
  TH2D *rate_cone_histo_e = (TH2D*) Nt_histo_e->Clone("rate_cone_histo_e");
  TH2D *rate_cone_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_cone_histo_mu");
  TH2D *total_histo_e = (TH2D*) NBs_histo_e->Clone("total_histo_e");
  TH2D *total_histo_mu = (TH2D*) NBs_histo_mu->Clone("total_histo_mu");
  TH2D *total_cone_histo_e = (TH2D*) NBs_cone_histo_e->Clone("total_cone_histo_e");
  TH2D *total_cone_histo_mu = (TH2D*) NBs_cone_histo_mu->Clone("total_cone_histo_mu");
  TH1F *total_BR_histo_e = (TH1F*) NBs_BR_histo_e->Clone("total_BR_histo_e");
  TH1F *total_BR_histo_mu = (TH1F*) NBs_BR_histo_mu->Clone("total_BR_histo_mu");

  rate_histo->Divide(rate_histo,Nl_histo,1,1,"B");
  rate_histo_e->Divide(rate_histo_e,Nl_histo_e,1,1,"B");
  rate_histo_mu->Divide(rate_histo_mu,Nl_histo_mu,1,1,"B");
  rate_cone_histo_e->Divide(rate_cone_histo_e,Nl_cone_histo_e,1,1,"B");
  rate_cone_histo_mu->Divide(rate_cone_histo_mu,Nl_cone_histo_mu,1,1,"B");
  total_histo_e->Add(NnotBs_histo_e);
  total_histo_mu->Add(NnotBs_histo_mu);
  total_cone_histo_e->Add(NnotBs_cone_histo_e);
  total_cone_histo_mu->Add(NnotBs_cone_histo_mu);
  total_BR_histo_e->Add(NnotBs_BR_histo_e);
  total_BR_histo_mu->Add(NnotBs_BR_histo_mu);
  NBs_histo_e->Divide(NBs_histo_e, total_histo_e,1,1,"B");
  NBs_histo_mu->Divide(NBs_histo_mu, total_histo_mu,1,1,"B");
  NBs_cone_histo_e->Divide(NBs_cone_histo_e, total_cone_histo_e,1,1,"B");
  NBs_cone_histo_mu->Divide(NBs_cone_histo_mu, total_cone_histo_mu,1,1,"B");
  NBs_BR_histo_e->Divide(NBs_BR_histo_e, total_BR_histo_e,1,1,"B");
  NBs_BR_histo_mu->Divide(NBs_BR_histo_mu, total_BR_histo_mu,1,1,"B");

  rate_histo->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_histo->GetYaxis()->SetTitle("eta");
  rate_histo->GetZaxis()->SetRangeUser(0,.5);
  rate_histo->SetTitle("Fake Rate vs Pt, Eta");
  rate_histo_e->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_histo_e->GetYaxis()->SetTitle("eta");
  rate_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_histo_e->SetTitle("Fake Rate vs Pt, Eta (electrons)");
  rate_histo_mu->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_histo_mu->GetYaxis()->SetTitle("eta");
  rate_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_histo_mu->SetTitle("Fake Rate vs Pt, Eta (muons)");
  rate_cone_histo_e->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_cone_histo_e->GetYaxis()->SetTitle("eta");
  rate_cone_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_cone_histo_e->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (electrons)");
  rate_cone_histo_mu->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_cone_histo_mu->GetYaxis()->SetTitle("eta");
  rate_cone_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_cone_histo_mu->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (muons)");
  NBs_histo_e->GetXaxis()->SetTitle("pT (GeV)"); 
  NBs_histo_e->GetYaxis()->SetTitle("|eta|");
  NBs_histo_e->SetTitle("B Abundance vs Pt, eta (electrons)");
  NBs_histo_mu->GetXaxis()->SetTitle("pT (GeV)"); 
  NBs_histo_mu->GetYaxis()->SetTitle("|eta|");
  NBs_histo_mu->SetTitle("B Abundance vs Pt, eta (muons)");
  NBs_cone_histo_e->GetXaxis()->SetTitle("Cone Energy (GeV)"); 
  NBs_cone_histo_e->GetYaxis()->SetTitle("|eta|");
  NBs_cone_histo_e->SetTitle("B Abundance vs Cone Energy, eta (electrons)");
  NBs_cone_histo_mu->GetXaxis()->SetTitle("Cone Energy (GeV)"); 
  NBs_cone_histo_mu->GetYaxis()->SetTitle("|eta|");
  NBs_cone_histo_mu->SetTitle("B Abundance vs Cone Energy, eta (muons)");
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

  // TCanvas *c0=new TCanvas("c0","Fake Rate vs Pt, eta",800,800);
  // rate_histo->Draw("colz,texte");
  // TCanvas *c1=new TCanvas("c1","Fake Rate vs Pt, eta (electron)",800,800);
  // rate_histo_e->Draw("colz,texte");
  // TCanvas *c2=new TCanvas("c2","Fake Rate vs Pt, eta (muon)",800,800);
  // rate_histo_mu->Draw("colz,texte");
  // TCanvas *c3=new TCanvas("c3","Fake Rate vs Pt + Cone Energy, eta (electron)",800,800);
  // rate_cone_histo_e->Draw("colz,texte");
  // TCanvas *c4=new TCanvas("c4","Fake Rate vs Pt + Cone Energy, eta (muon)",800,800);
  // rate_cone_histo_mu->Draw("colz,texte");
  // TCanvas *c5=new TCanvas("c5","B Abundance vs Pt, eta (el)",800,800);
  // NBs_histo_e->Draw("colz,texte");
  // TCanvas *c6=new TCanvas("c6","B Abundance vs Pt, eta (mu)",800,800);
  // NBs_histo_mu->Draw("colz,texte");
  // TCanvas *c7=new TCanvas("c7","B Abundance vs Cone Energy, eta (el)",800,800);
  // NBs_cone_histo_e->Draw("colz,texte");
  // TCanvas *c8=new TCanvas("c8","B Abundance vs Cone Energy, eta (mu)",800,800);
  // NBs_cone_histo_mu->Draw("colz,texte");
  // TCanvas *c9=new TCanvas("c9","pTrel vs Iso (el)",800,800);
  // pTrelvsIso_histo_el->Draw("colz,texte");
  // TCanvas *c10=new TCanvas("c10","pTrel vs Iso (mu)",800,800);
  // pTrelvsIso_histo_mu->Draw("colz,texte");
  TCanvas *c11=new TCanvas("c11","B Abundance vs Nbjets (electrons)",800,800);
  NBs_BR_histo_e->Draw("histE");
  TCanvas *c12=new TCanvas("c12","B Abundance vs Nbjets (muons)",800,800);
  NBs_BR_histo_mu->Draw("histE");
  TCanvas *c13=new TCanvas("c13","Njets with pT > 40 GeV",800,800);
  njets40_histo->Draw("histE");

  //---save histos-------//
  TFile *OutputFile = new TFile(outfile,"recreate");
  OutputFile->cd();
  Nl_histo->Write();
  Nl_histo_e->Write();
  Nl_histo_mu->Write();
  Nt_histo->Write();
  Nt_histo_e->Write();
  Nt_histo_mu->Write();
  rate_histo->Write();
  rate_histo_e->Write();
  rate_histo_mu->Write();
  rate_cone_histo_e->Write();
  rate_cone_histo_mu->Write();
  NBs_histo_e->Write();
  NBs_histo_mu->Write();
  NBs_cone_histo_e->Write();
  NBs_cone_histo_mu->Write();
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

  delete Nt_histo;
  delete Nl_histo;
  delete Nt_histo_e;
  delete Nl_histo_e;
  delete Nt_histo_mu;
  delete Nl_histo_mu;
  delete Nl_cone_histo_e;
  delete Nl_cone_histo_mu;

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
