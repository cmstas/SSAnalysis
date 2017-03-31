#include "TChain.h"
#include "TH2D.h"
#include "../../classFiles/v4.05/SS.h"
#include "../../software/dataMCplotMaker/PlotMaker2D.h"
#include "../../commonUtils.h"

using namespace ss;

//Set lumi
float theLumi = 1.0; 

//DY only
// bool DYonly = false;
// bool TTonly = false;

void flip(bool doMu=false, bool DYonly=false, bool TTonly=false){

  //Flip Rate Histograms
  TH2D* numer;
  TH2D* denom;


  if(doMu) {
      // int nBinsX = 1; 
      // int nBinsY = 1; 
      // float xbins[] = { 0, 100 }; 
      // float ybins[] = { 0, 3 }; 

      int nBinsX = 2; 
      int nBinsY = 2; 
      float xbins[] = { 0, 100, 200 }; 
      float ybins[] = { 0, 2, 3 }; 
      numer  = new TH2D("numer" , "numer" , nBinsX, xbins, nBinsY, ybins);
      denom  = new TH2D("denom" , "denom" , nBinsX, xbins, nBinsY, ybins);
  } else {
      int nBinsX = 6; 
      int nBinsY = 3; 
      float xbins[] = { 15, 40, 60, 80, 100, 200, 300 }; 
      float ybins[] = { 0, 0.8, 1.479, 2.5 }; 
      numer  = new TH2D("numer" , "numer" , nBinsX, xbins, nBinsY, ybins);
      denom  = new TH2D("denom" , "denom" , nBinsX, xbins, nBinsY, ybins);
  }

  numer->Sumw2();
  denom->Sumw2();

  //Set up chains
  TChain *chain = new TChain("t");


  // TString pfx = "/nfs-7/userdata/ss2015/ssBabies/"+getTag();
  TString pfx_moriond  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.03/output/";
  TString pfx = pfx_moriond;


  if (!DYonly) chain->Add(pfx+"/TTBAR_PH*.root"); 
  if (!TTonly) chain->Add(pfx+"/DY_low*.root"); 
  if (!TTonly) chain->Add(pfx+"/DY_high*.root"); 

  // if (!DYonly) chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04/TTBAR_PH.root"); 
  // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04/DY_low.root"); 
  // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04/DY_high.root"); 

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
      // if (!ss::fired_trigger() && ss::is_real_data()) continue;
      if (!ss::fired_trigger()) continue;

      //Consider only numerator events of either charge, without extra Z veto
      if (hyp_class() != 3 && hyp_class() != 4) continue;// && hyp_class() != 6

      //Hyp-6 events don't necessarily pass ID (in v1.26, to be fixed); impose that here
      if (!lep1_passes_id() || !lep2_passes_id()) continue;

      if(doMu) {
          if (abs(lep1_id()) == 11 && abs(lep2_id()) == 11) continue;
      } else {
          if (abs(lep1_id()) == 13 && abs(lep2_id()) == 13) continue;
      }
      int absLepId = doMu ? 13 : 11;

      //Weight
      // float weight = getTruePUw(ss::trueNumInt()[0])*scale1fb()*theLumi; 
      float weight = getTruePUw_Moriond(ss::trueNumInt()[0])*scale1fb()*theLumi; 
      // float weight = 1.0;

      //If they make it this far, they are denominator events
      if (abs(lep1_id()) == absLepId) denom->Fill(min(lep1_p4().pt(), float(299.)), fabs(lep1_p4().eta()), weight); 
      if (abs(lep2_id()) == absLepId) denom->Fill(min(lep2_p4().pt(), float(299.)), fabs(lep2_p4().eta()), weight); 

      //Now require exactly one charge flip
      bool isCF1 = 0;
      bool isCF2 = 0;
      if (abs(lep1_id()) == absLepId && abs(lep1_mc_id()) == absLepId && lep1_id() != lep1_mc_id()) isCF1 = 1;
      if (abs(lep2_id()) == absLepId && abs(lep2_mc_id()) == absLepId && lep2_id() != lep2_mc_id()) isCF2 = 1;
      if (!isCF1 && !isCF2) continue;
      if ( isCF1 &&  isCF2) continue;

      //The one that flipped is in the numerator
      if (isCF1) numer->Fill(lep1_p4().pt(), fabs(lep1_p4().eta()), weight); 
      if (isCF2) numer->Fill(lep2_p4().pt(), fabs(lep2_p4().eta()), weight); 

    }//event loop
  }//file loop

  std::string lepStr = doMu ? "mu" : "el";

  if (DYonly) lepStr += "_dy";
  if (TTonly) lepStr += "_tt";

  std::cout << "numer: " << numer->GetEntries() << " denom: " << denom->GetEntries() << std::endl;
  std::cout << "Total flip rate for " << lepStr << ": " << numer->GetEntries()/denom->GetEntries() << std::endl;

  PlotMaker2D(numer, "--outputName plots/flips_numer_"+lepStr+".pdf --noOverflow --setTitle flip rate numer ("+lepStr+") --Xaxis p_{T} --Yaxis |#eta| --color --text"); 
  PlotMaker2D(denom, "--outputName plots/flips_denom_"+lepStr+".pdf --noOverflow --setTitle flip rate denom ("+lepStr+") --Xaxis p_{T} --Yaxis |#eta| --color --text"); 

  //Now make ratio plot
  numer->Divide(numer, denom, 1, 1, "b"); 
  // if (DYonly) PlotMaker2D(numer, "--outputName plots/flips_rate_dy_"+lepStr+".pdf --noOverflow --setTitle flip rate DY only ("+lepStr+") --Xaxis p_{T} --Yaxis |#eta| --sciNot .2 --color --text90"); 
  // else PlotMaker2D(numer, "--outputName plots/flips_rate_"+lepStr+".pdf --noOverflow --setTitle flip rate ("+lepStr+") --Xaxis p_{T} --Yaxis |#eta| --color --text90  --sciNot .2"); //--sciNot .6 
  PlotMaker2D(numer, "--outputName plots/flips_rate_"+lepStr+".pdf --noOverflow --setTitle flip rate ("+lepStr+") --Xaxis p_{T} --Yaxis |#eta| --color --text90  --sciNot .2"); //--sciNot .6 

  if(DYonly) {
      TFile *file = new TFile("flip_rate_dy.root", "RECREATE");
      file->Write(); 
      numer->Write("flips");
      file->Close(); 
  }
  if(TTonly) {
      TFile *file = new TFile("flip_rate_tt.root", "RECREATE");
      file->Write(); 
      numer->Write("flips");
      file->Close(); 
  }

  //Save the histogram
  if(!doMu && !DYonly && !TTonly) {
      TFile *file = new TFile("flip_rate.root", "RECREATE");
      // TFile *file = new TFile("flip_rate_ICHEP.root", "RECREATE");
      file->Write(); 
      numer->Write("flips");
      file->Close(); 
  }

}
