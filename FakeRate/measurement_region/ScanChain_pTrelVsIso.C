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

  bool doConly = false;
  if (option.Contains("doConly")) doConly = true;

  bool doLightonly = false;
  if (option.Contains("doLightonly")) doLightonly = true;

  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  TH2D *pTrelvsIso_histo_el = new TH2D("pTrelvsIso_histo_el", "pTrel vs Iso (Electrons)", 10, 0., 1., 15, 0., 30.);
  pTrelvsIso_histo_el->SetDirectory(rootdir);
  pTrelvsIso_histo_el->Sumw2();

  TH2D *pTrelvsIso_histo_mu = new TH2D("pTrelvsIso_histo_mu", "pTrel vs Iso (Muons)", 10, 0., 1., 15, 0., 30.);
  pTrelvsIso_histo_mu->SetDirectory(rootdir);
  pTrelvsIso_histo_mu->Sumw2();
  //----------------------

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
	  if(ss.scale1fb() > 100000.) continue;  //excludes 5to10 and 10to20 EM Enriched
	  
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

	  // if(ss.nFOs() != 1) //if more than 1 FO in event
	  // 	{continue;}
   
	  int nbtags = 0;
	  for(int i = 0; i < ss.jets().size(); i++){
		  if(ss.jets_disc()[i] > 0.814) nbtags++;
		}
	  if (nbtags > 2.) nbtags = 2;

	  if(ss.p4().pt() > 100. || ss.p4().pt() < 10. || fabs(ss.p4().eta()) > 2.4) //What do we want here? 
	  	{continue;}

	  //------------------------------------------------------------------------------------------
	  //---------------------------------Find e = f(Pt,eta)---------------------------------------
	  //------------------------------------------------------------------------------------------

	  //Find ratio of nonprompt leps passing tight to nonprompt leps passing at least loose.  This is the fake rate 
	  // Use lep_passes_id to see if num.  Use FO to see if Fakable Object (denom)
	  //Calculate e=Nt/(Nl) where l->loose  (as opposed to loose-not-tight).
	  
	  //Using gen level info to see if prompt -> no prompt contamination in measurement region
	  //everything else is RECO (p4, id, passes_id, FO, etc.)
	  

	  if( ss.motherID() != 1 && (doBonly==0 || ss.motherID() == -1) && (doConly==0 || ss.motherID() == -2) && (doLightonly==0 || ss.motherID() == 0) )  //if el is nonprompt (GEN info)
		{
		  if( abs( ss.id() ) == 11 ) // it's an el
			{
			  if(ss.FO_NoIso())  pTrelvsIso_histo_el->Fill( ss.iso(), ss.ptrelv1() );
			}
		  if( abs( ss.id() ) == 13 ) // it's a mu
			{
 			  if(ss.FO_NoIso())  pTrelvsIso_histo_mu->Fill( ss.iso(), ss.ptrelv1() );
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

  pTrelvsIso_histo_el->GetXaxis()->SetTitle("Iso");
  pTrelvsIso_histo_el->GetYaxis()->SetTitle("pTrel");
  pTrelvsIso_histo_mu->GetXaxis()->SetTitle("Iso");
  pTrelvsIso_histo_mu->GetYaxis()->SetTitle("pTrel");

  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("1.3f");

  TCanvas *c9=new TCanvas("c9","B pTrel vs Iso (el)",800,800);
  pTrelvsIso_histo_el->Draw("colz,texte");
  TCanvas *c10=new TCanvas("c10","B pTrel vs Iso (mu)",800,800);
  pTrelvsIso_histo_mu->Draw("colz,texte");

  //---save histos-------//
  TFile *OutputFile = new TFile(outfile,"recreate");
  pTrelvsIso_histo_el->Write();
  pTrelvsIso_histo_mu->Write();

  OutputFile->Close();

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
