#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "SS.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"

bool corrected = true;

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

string version = "v1.26";

void yields(){

  //Make chains, histograms
  TChain* chain = new TChain("t");
  chain->Add("./babies/sync_TTW_baby_0.root");

  //Declare counters
  int y0hh[3] = { 0 }; 
  int y0hl[3] = { 0 }; 
  int y1hh[3] = { 0 }; 
  int y1hl[3] = { 0 }; 
  int y2hh[3] = { 0 }; 
  int y2hl[3] = { 0 }; 
  int y3hh[3] = { 0 }; 
  int y3hl[3] = { 0 }; 

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //Set up output text file
  ofstream textfile;
  textfile.open("unsorted.txt");

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

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

      //Reject non-SS
      if (ss::hyp_class() != 3) continue;

      //Reject not triggered
      if (!ss::fired_trigger()) continue;

      //Determine MET
      float metAG = (corrected ? ss::corrMET() : ss::met() );
      float metPhiAG = (corrected ? ss::corrMETphi() : ss::metPhi());

      //Determine HT
      float htAG = (corrected ? ss::ht_corr() : ss::ht());

      //Determine njets
      float njetsAG =  (corrected ? ss::njets_corr() : ss::njets());

      //Determine nbtags
      float nbtagsAG = (corrected ? ss::nbtags_corr() : ss::nbtags());

      //Calculate mtmin
      float mt1 = MT(ss::lep1_p4().pt(), ss::lep1_p4().phi(), metAG, metPhiAG);
      float mt2 = MT(ss::lep2_p4().pt(), ss::lep2_p4().phi(), metAG, metPhiAG);
      float mtmin = mt1 > mt2 ? mt2 : mt1; 

      //Determine type
      int type = -1;
      if (ss::hyp_type() == 3) type = 2;
      if (ss::hyp_type() == 2 || ss::hyp_type() == 1) type = 1;
      if (ss::hyp_type() == 0) type = 0;
   
      //Figure out region
      anal_type_t categ = analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt());  
      int SR = signalRegion(njetsAG, nbtagsAG, metAG, htAG, mtmin, ss::lep1_p4().pt(), ss::lep2_p4().pt());
      int BR = baselineRegion(njetsAG, nbtagsAG, metAG, htAG, ss::lep1_p4().pt(), ss::lep2_p4().pt());

      //Counters
      if (categ == HighHigh && BR >=  0) y0hh[type]++; 
      if (categ == HighLow  && BR >=  0) y0hl[type]++; 
      if (categ == HighHigh && BR == 1) y1hh[type]++; 
      if (categ == HighLow  && BR == 1) y1hl[type]++; 
      if (categ == HighHigh && BR == 2) y2hh[type]++; 
      if (categ == HighLow  && BR == 2) y2hl[type]++; 
      if (categ == HighHigh && BR == 3) y3hh[type]++; 
      if (categ == HighLow  && BR == 3) y3hl[type]++; 

      //Print sync script for 0-0 HH    
      if (categ == HighHigh && BR >=  0) textfile << Form("%1d %9d %12d\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d\n", ss::run(), ss::lumi(), ss::event(), ss::nVetoElectrons7()+ss::nVetoMuons5(), ss::lep1_id(), ss::lep1_p4().pt(), ss::lep2_id(), ss::lep2_p4().pt(), njetsAG, nbtagsAG, metAG, htAG, SR); 

    }//event loop
  }//file loop

  //Close output text file
  textfile.close();

  //Print counter results
  CTable table;
  table.setTitle("Sync Results");
  table.useTitle(); 
  table.setTable() (             "ee",    "em",    "mm")
                   (" 0 H-H", y0hh[2], y0hh[1], y0hh[0]) 
                   (" 0 H-L", y0hl[2], y0hl[1], y0hl[0]) 
                   ("10 H-H", y1hh[2], y1hh[1], y1hh[0]) 
                   ("10 H-L", y1hl[2], y1hl[1], y1hl[0]) 
                   ("20 H-H", y2hh[2], y2hh[1], y2hh[0]) 
                   ("20 H-L", y2hl[2], y2hl[1], y2hl[0]) 
                   ("30 H-H", y3hh[2], y3hh[1], y3hh[0]) 
                   ("30 H-L", y3hl[2], y3hl[1], y3hl[0]);
  table.print(); 

  //Sort the file
  system("sort -n -k1 -k2 -k3 unsorted.txt > ucsx_SR0_HH_yields.txt");
  system("rm unsorted.txt"); 

}
