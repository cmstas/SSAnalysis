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

// lepfilter
#include "lepfilter.cc"

using namespace std;
using namespace samesign;

int ScanChain( TChain* chain, TString outfile, TString option="", bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  bool noSIP = false;
  if (option.Contains("noSIP")) noSIP = true;

  bool usePtRel = false;
  if (option.Contains("ptRel")) usePtRel = true;

  bool doBonly = false;
  if (option.Contains("doBonly")) doBonly = true;

  float ptbins[5] = {10., 20., 30., 50., 70.};
  float etabins[4] = {0., 1., 2., 2.4};

  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  TH2D *Nt_histo = new TH2D("Nt_histo", "Nt vs Pt, Eta", 4,ptbins,3,etabins);
  Nt_histo->SetDirectory(rootdir);
  Nt_histo->Sumw2();

  TH2D *Nl_histo = new TH2D("Nl_histo", "Nl vs Pt, Eta", 4,ptbins,3,etabins);
  Nl_histo->SetDirectory(rootdir);
  Nl_histo->Sumw2();

  TH2D *Nt_histo_e = new TH2D("Nt_histo_e", "Nt vs Pt, Eta (electrons)", 4,ptbins,3,etabins);
  Nt_histo_e->SetDirectory(rootdir);
  Nt_histo_e->Sumw2();

  TH2D *Nl_histo_e = new TH2D("Nl_histo_e", "Nl vs Pt, Eta (electrons)", 4,ptbins,3,etabins);
  Nl_histo_e->SetDirectory(rootdir);
  Nl_histo_e->Sumw2();

  TH2D *Nt_histo_mu = new TH2D("Nt_histo_mu", "Nt vs Pt, Eta (muons)", 4,ptbins,3,etabins);
  Nt_histo_mu->SetDirectory(rootdir);
  Nt_histo_mu->Sumw2();

  TH2D *Nl_histo_mu = new TH2D("Nl_histo_mu", "Nl vs Pt, Eta (muons)", 4,ptbins,3,etabins);
  Nl_histo_mu->SetDirectory(rootdir);
  Nl_histo_mu->Sumw2();

  TH2D *Nl_cone_histo_e = new TH2D("Nl_cone_histo_e", "Nl vs Cone Energy, Eta (electrons)", 4,ptbins,3,etabins);
  Nl_cone_histo_e->SetDirectory(rootdir);
  Nl_cone_histo_e->Sumw2();

  TH2D *Nl_cone_histo_mu = new TH2D("Nl_cone_histo_mu", "Nl vs Cone Energy, Eta (muons)", 4,ptbins,3,etabins);
  Nl_cone_histo_mu->SetDirectory(rootdir);
  Nl_cone_histo_mu->Sumw2();

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

	  bool jetptcut = false;
	  int jetidx = 0;

	  while( (jetidx < ss.jets().size()) && !jetptcut) //check to see if at least one jet w/ pt > 40
	  	{
	  	  if( ss.jets()[jetidx].pt() > 40. )
	  		{jetptcut = true;}
	  	  jetidx++;
	  	}
	  
	  if( !(jetptcut && ss.met() < 20. && ss.mt() < 20) )
	  	{continue;}

	  if(ss.nFOs() != 1) //if more than 1 FO in event
		{continue;}

	  //Ditch bounds here and just enforce correct reading of histo in getFakeRate() in app_region/ScanChain.C???
	  //If we dont want leptons w/ |eta|>2.4 in ttbar application, filling rate histos with leptons w/
	  // |eta|>2.4 will mess up the rate in the highest eta bins (2<|eta|<3)
	  //Don't think eta cut is anywhere else
	  if(ss.p4().pt() > 100. || ss.p4().pt() < 10. || fabs(ss.p4().eta()) > 2.4) //What do we want here? 
	  //if(ss.p4().pt() > 100. || ss.p4().pt() < 25. || fabs(ss.p4().eta()) > 2.4) //What do we want here? 
	  	{continue;}

	  //------------------------------------------------------------------------------------------
	  //---------------------------------Find e = f(const)---------------------------------------
	  //------------------------------------------------------------------------------------------

 	  //Find ratio of nonprompt leps passing tight to nonprompt leps passing at least loose.  This is the fake rate 
	  // Use lep_passes_id to see if num.  Use FO to see if Fakable Object (denom)
	  //Calculate e=Nt/(Nl) where l->loose  (as opposed to loose-not-tight).
	  
	  //Using gen level info to see if prompt -> no prompt contamination in measurement region
	  //everything else is RECO (p4, id, passes_id, FO, etc.)

	  if( ss.motherID() != 1 && (doBonly==0 || ss.motherID() == -1) )  //if lep is nonprompt
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
	  

	  if( ss.motherID() != 1 && (doBonly==0 || ss.motherID() == -1) )  //if el is nonprompt (GEN info)
		{

		  bool passId = ( usePtRel ? ss.passes_id_ptrel() : ss.passes_id() );
		  bool passFO = ( usePtRel ? ss.FO_ptrel() : ss.FO() );

		  if( abs( ss.id() ) == 11 ) // it's an el
			{
			  if( passId )  //if el is tight
				{ 
				  Nt_histo->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);     //fill histo with fake pt, eta
				  Nt_histo_e->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);   //
				}
			  if( passFO )  //if el is FO
				{
				  if (noSIP && fabs(ss.ip3d()/ss.ip3derr())>4. ) continue;
				  Nl_histo->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);     //fill histo with fake pt, eta 
				  Nl_histo_e->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);   //  <-- loose (as opposed to l!t)			
				  if( ss.passes_id() ) Nl_cone_histo_e->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);   //  <-- loose (as opposed to l!t)			
				  else Nl_cone_histo_e->Fill(ss.p4().pt()+ss.p4().pt()*std::max(0.,ss.iso()-0.1), fabs(ss.p4().eta()), weight);
				}
			}
		  if( abs( ss.id() ) == 13 ) // it's a mu
			{
			  if( passId )  //if mu is tight
				{ 
				  Nt_histo->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);     //fill histo with fake pt, eta
				  Nt_histo_mu->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);   //
				}
			  if( passFO )  //if el is FO
				{
				  if (noSIP && fabs(ss.ip3d()/ss.ip3derr())>4. ) continue;
				  Nl_histo->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);     //fill histo with fake pt, eta 
				  Nl_histo_mu->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);   //  <-- loose (as opposed to l!t)			
				  if( ss.passes_id() ) Nl_cone_histo_mu->Fill(ss.p4().pt(), fabs(ss.p4().eta()), weight);   //  <-- loose (as opposed to l!t)			
				  else Nl_cone_histo_mu->Fill(ss.p4().pt()+ss.p4().pt()*std::max(0.,ss.iso()-0.1), fabs(ss.p4().eta()), weight);
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

  //Histograms
  TH2D *rate_histo = (TH2D*) Nt_histo->Clone("rate_histo");
  TH2D *rate_histo_e = (TH2D*) Nt_histo_e->Clone("rate_histo_e");
  TH2D *rate_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_histo_mu");
  TH2D *rate_cone_histo_e = (TH2D*) Nt_histo_e->Clone("rate_cone_histo_e");
  TH2D *rate_cone_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_cone_histo_mu");
  
  rate_histo->Divide(rate_histo,Nl_histo,1,1,"B");
  rate_histo_e->Divide(rate_histo_e,Nl_histo_e,1,1,"B");
  rate_histo_mu->Divide(rate_histo_mu,Nl_histo_mu,1,1,"B");
  rate_cone_histo_e->Divide(rate_cone_histo_e,Nl_cone_histo_e,1,1,"B");
  rate_cone_histo_mu->Divide(rate_cone_histo_mu,Nl_cone_histo_mu,1,1,"B");

  rate_histo->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_histo->GetYaxis()->SetTitle("eta");
  rate_histo->GetZaxis()->SetRangeUser(0,1.);
  rate_histo->SetTitle("Fake Rate vs Pt, Eta");
  rate_histo_e->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_histo_e->GetYaxis()->SetTitle("eta");
  rate_histo_e->GetZaxis()->SetRangeUser(0,1.);
  rate_histo_e->SetTitle("Fake Rate vs Pt, Eta (electrons)");
  rate_histo_mu->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_histo_mu->GetYaxis()->SetTitle("eta");
  rate_histo_mu->GetZaxis()->SetRangeUser(0,1.);
  rate_histo_mu->SetTitle("Fake Rate vs Pt, Eta (muons)");
  rate_cone_histo_e->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_cone_histo_e->GetYaxis()->SetTitle("eta");
  rate_cone_histo_e->GetZaxis()->SetRangeUser(0,1.);
  rate_cone_histo_e->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (electrons)");
  rate_cone_histo_mu->GetXaxis()->SetTitle("pT (GeV)"); 
  rate_cone_histo_mu->GetYaxis()->SetTitle("eta");
  rate_cone_histo_mu->GetZaxis()->SetRangeUser(0,1.);
  rate_cone_histo_mu->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (muons)");

  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("1.3f");

  TCanvas *c0=new TCanvas("c0","Fake Rate vs Pt, eta",800,800);
  rate_histo->Draw("colz,texte");
  TCanvas *c1=new TCanvas("c1","Fake Rate vs Pt, eta (electron)",800,800);
  rate_histo_e->Draw("colz,texte");
  TCanvas *c2=new TCanvas("c2","Fake Rate vs Pt, eta (muon)",800,800);
  rate_histo_mu->Draw("colz,texte");
  TCanvas *c3=new TCanvas("c3","Fake Rate vs Pt + Cone Energy, eta (electron)",800,800);
  rate_cone_histo_e->Draw("colz,texte");
  TCanvas *c4=new TCanvas("c4","Fake Rate vs Pt + Cone Energy, eta (muon)",800,800);
  rate_cone_histo_mu->Draw("colz,texte");

  //---save histos-------//
  TFile *OutputFile = new TFile(outfile,"recreate");
  OutputFile->cd();
  rate_histo->Write();
  rate_histo_e->Write();
  rate_histo_mu->Write();
  rate_cone_histo_e->Write();
  rate_cone_histo_mu->Write();

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
