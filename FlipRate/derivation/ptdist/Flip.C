#include "TChain.h"
#include "TH2D.h"
#include "../../../classFiles/v5.04/SS.h"
#include "../../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../../commonUtils.h"

using namespace ss;

//Set lumi
float theLumi = 1.0; 

//DY only
bool DYonly = false;

TH1F* makePlot(TChain *chain);

void flip(){

  //Set up chains
  TChain *dy_chain    = new TChain("t");
  TChain *ttbar_chain = new TChain("t");
  ttbar_chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"/TTBAR.root"); 
  dy_chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"/DY_low.root"); 
  dy_chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"/DY_high.root"); 

  //Make plots
  TH1F* ttbar = makePlot(ttbar_chain); 
  TH1F* dy    = makePlot(dy_chain); 

  //Plotting stuff
  TH1F* null = new TH1F("","",1,0,1);
  vector <TH1F*> backgrounds = {ttbar, dy}; 
  vector <string> names = {"ttbar", "dy"}; 

  //Colors
  vector<Color_t> colors = { kRed, kBlue }; 

  //Make the plot
  dataMCplotMaker(null, backgrounds, names, "p_{T} distribution", "", "--outputName ptPlot.pdf --noStack --noLumi --xAxisLabel p_{T} --bkgd_width 3", vector<TH1F*>(), vector<std::string>(), colors); 

}

TH1F* makePlot(TChain *chain){

  //Pt Histograms
  TH1F* hist  = new TH1F("hist" , "hist" , 100, 0, 500);
  hist->Sumw2();

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

      //Consider only numerator events of either charge
      if (hyp_class() != 3 && hyp_class() != 4) continue;

      //Only care about electrons
      if (abs(lep1_id()) == 13 && abs(lep2_id()) == 13) continue;

      //If they make it this far, good
      if (abs(lep1_id()) == 11) hist->Fill(lep1_p4().pt()); 
      if (abs(lep2_id()) == 11) hist->Fill(lep2_p4().pt()); 

    }//event loop
  }//file loop

  return hist;

}
