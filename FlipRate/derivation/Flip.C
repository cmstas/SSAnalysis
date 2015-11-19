#include "TChain.h"
#include "TH2D.h"
#include "../../classFiles/v4.05/SS.h"
#include "../../software/dataMCplotMaker/PlotMaker2D.h"
#include "../../commonUtils.h"

using namespace ss;

//Set lumi
float theLumi = 1.0; 

//DY only
bool DYonly = false;

void flip(){

  //Flip Rate Histograms
  int nBinsX = 6; 
  int nBinsY = 3; 
  float xbins[] = { 15, 40, 60, 80, 100, 200, 300 }; 
  float ybins[] = { 0, 0.8, 1.479, 2.5 }; 
  // int nBinsX = 1; 
  // int nBinsY = 1; 
  // float xbins[] = { 0, 100 }; 
  // float ybins[] = { 0, 3 }; 
  TH2D* numer  = new TH2D("numer" , "numer" , nBinsX, xbins, nBinsY, ybins);
  TH2D* denom  = new TH2D("denom" , "denom" , nBinsX, xbins, nBinsY, ybins);
  numer->Sumw2();
  denom->Sumw2();

  //Set up chains
  TChain *chain = new TChain("t");
  if (!DYonly) chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"/TTBAR.root"); 
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"/DY_low.root"); 
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"/DY_high.root"); 

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

      //Reject not triggered
      if (!ss::fired_trigger()) continue;

      //Consider only numerator events of either charge, without extra Z veto
      if (hyp_class() != 3 && hyp_class() != 4) continue;// && hyp_class() != 6

      //Hyp-6 events don't necessarily pass ID (in v1.26, to be fixed); impose that here
      if (!lep1_passes_id() || !lep2_passes_id()) continue;

      //Only care about electrons
      if (abs(lep1_id()) == 13 && abs(lep2_id()) == 13) continue;
      // if (abs(lep1_id()) == 11 && abs(lep2_id()) == 11) continue;

      //Weight
      float weight = getPUw(ss::nGoodVertices());//scale1fb()*theLumi; 

      //If they make it this far, they are denominator events
      if (abs(lep1_id()) == 11) denom->Fill(min(lep1_p4().pt(), float(299.)), fabs(lep1_p4().eta()), weight); 
      if (abs(lep2_id()) == 11) denom->Fill(min(lep2_p4().pt(), float(299.)), fabs(lep2_p4().eta()), weight); 
      // if (abs(lep1_id()) == 13) denom->Fill(lep1_p4().pt(), fabs(lep1_p4().eta()), weight); 
      // if (abs(lep2_id()) == 13) denom->Fill(lep2_p4().pt(), fabs(lep2_p4().eta()), weight); 

      //Now require exactly one charge flip
      bool isCF1 = 0;
      bool isCF2 = 0;
      if (abs(lep1_id()) == 11 && abs(lep1_mc_id()) == 11 && lep1_id() != lep1_mc_id()) isCF1 = 1;
      if (abs(lep2_id()) == 11 && abs(lep2_mc_id()) == 11 && lep2_id() != lep2_mc_id()) isCF2 = 1;
      // if (abs(lep1_id()) == 13 && abs(lep1_mc_id()) == 13 && lep1_id() != lep1_mc_id()) isCF1 = 1;
      // if (abs(lep2_id()) == 13 && abs(lep2_mc_id()) == 13 && lep2_id() != lep2_mc_id()) isCF2 = 1;
      if (!isCF1 && !isCF2) continue;
      if ( isCF1 &&  isCF2) continue;

      //The one that flipped is in the numerator
      if (isCF1) numer->Fill(lep1_p4().pt(), fabs(lep1_p4().eta()), weight); 
      if (isCF2) numer->Fill(lep2_p4().pt(), fabs(lep2_p4().eta()), weight); 

    }//event loop
  }//file loop

  PlotMaker2D(numer, "--outputName flips_numer.pdf --noOverflow --setTitle flip rate numer --Xaxis elec p_{T} --Yaxis |#eta|"); 
  PlotMaker2D(denom, "--outputName flips_denom.pdf --noOverflow --setTitle flip rate denom --Xaxis elec p_{T} --Yaxis |#eta|"); 

  //Now make ratio plot
  numer->Divide(numer, denom, 1, 1, "b"); 
  if (DYonly) PlotMaker2D(numer, "--outputName flips_rate_dy.pdf --noOverflow --setTitle flip rate DY only --Xaxis elec p_{T} --Yaxis |#eta| --sciNot .2 --color --text"); 
  else PlotMaker2D(numer, "--outputName flips_rate.pdf --noOverflow --setTitle flip rate --Xaxis elec p_{T} --Yaxis |#eta| --color --text"); //--sciNot .6 

  //Save the histogram
  TFile *file = new TFile("flip_rate.root", "RECREATE");
  file->Write(); 
  numer->Write("flips");
  file->Close(); 

}
