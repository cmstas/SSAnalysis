#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "../classFiles/v4.05/SS.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"
#include "../CORE/Tools/dorky/dorky.h"

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

void yields(){

  //Make chains, histograms
  TChain* chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v4.06/Data*.root"); 

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
  ofstream textfile2;
  textfile2.open("unsorted2.txt");

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    // Loop over Events in current file
    for(unsigned int eventAG = 0; eventAG < tree->GetEntriesFast(); eventAG++){
    
      //Get Event Content
      samesign.GetEntry(eventAG);
      nEventsTotal++;

      //Duplicates
      duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
      if (duplicate_removal::is_duplicate(id)) continue; 

      //Blinding
      if (!isUnblindRun(ss::run())) continue;
    
      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //Reject non-SS
      if (ss::hyp_class() != 3) continue;

      //Reject not triggered
      if (!ss::fired_trigger()) continue;

      //Calculate mtmin
      float mt1 = MT(ss::lep1_p4().pt(), ss::lep1_p4().phi(), ss::rawmet(), ss::rawmetPhi());
      float mt2 = MT(ss::lep2_p4().pt(), ss::lep2_p4().phi(), ss::rawmet(), ss::rawmetPhi());
      float mtmin = mt1 > mt2 ? mt2 : mt1; 

      //Determine type
      int type = -1;
      if (ss::hyp_type() == 3) type = 2;
      if (ss::hyp_type() == 2 || ss::hyp_type() == 1) type = 1;
      if (ss::hyp_type() == 0) type = 0;
   
      //Figure out region
      anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());  
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::rawmet(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::rawmet(), ss::ht(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());

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
      if (categ == HighHigh && BR >=  0) textfile << Form("%1d %9d %12llu\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d\n", ss::run(), ss::lumi(), ss::event(), ss::nVetoElectrons7()+ss::nVetoMuons5(), ss::lep1_id(), ss::lep1_p4().pt(), ss::lep2_id(), ss::lep2_p4().pt(), ss::njets(), ss::nbtags(), ss::rawmet(), ss::ht(), SR); 
      if (categ == HighLow && BR >=  0) textfile2 << Form("%1d %9d %12llu\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d\n", ss::run(), ss::lumi(), ss::event(), ss::nVetoElectrons7()+ss::nVetoMuons5(), ss::lep1_id(), ss::lep1_p4().pt(), ss::lep2_id(), ss::lep2_p4().pt(), ss::njets(), ss::nbtags(), ss::rawmet(), ss::ht(), SR); 

    }//event loop
  }//file loop
  
  //Close output text file
  textfile.close();
  textfile2.close();

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

  //Sort the files
  system("sort -n -k1 -k2 -k3 unsorted.txt > ucsx_SR0_HH_yields.txt");
  system("rm unsorted.txt"); 
  system("sort -n -k1 -k2 -k3 unsorted2.txt > ucsx_SR0_HL_yields.txt");
  system("rm unsorted2.txt"); 


}
