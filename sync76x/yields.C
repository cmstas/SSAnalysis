#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "../classFiles/v6.02/SS.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"
#include "../CORE/Tools/dorky/dorky.h"

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

void yields(){

  // Make chains, histograms
  TChain* chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/synch_ttw_1.root"); 

  // Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  // Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // Set up output text file
  ofstream textfile;
  textfile.open("unsorted.txt");

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    // Loop over Events in current file
    for(unsigned int ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    
      //Get Event Content
      samesign.GetEntry(ievent);
      nEventsTotal++;

      // Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      // Reject non-SS
      if (ss::hyp_class() != 3) continue;

      // Ignore events with <2 jets
      if (ss::njets() < 2) continue;

      // Figure out region
      anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());  
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::rawmet(), ss::ht(), ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::rawmet(), ss::ht(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());


      if (categ == HighHigh && BR >= 0) {
          textfile_global << Form("%1d%9d%12d\t%2d\t%+2d%5.1f\t%+2d%5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d", 
                  ss::run(), ss::lumi(), ss::event(), 
                  ss::nVetoElectrons7()+ss::nVetoMuons5(),
                  ss::lep1_id(), ss::lep1_p4().pt(), 
                  ss::lep2_id(), ss::lep2_p4().pt(), 
                  ss::njets(), ss::nbtags(), ss::rawmet(), ss::ht(), SR); 

          textfile_leptons << Form("%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t
                  %1d\t%1d\t%1d\t%1d\t%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t
                  %5.1f\t%1d\t%1d\t%1d\t%1d",
                  lep1_p4().pt(),
                  lep1_p4().eta(),
                  lep1_p4().phi(),
                  lep1_id(),
                  lep1_coneCorrPt(),
                  lep1_dxyPV(),
                  lep1_dZ(),
                  lep1_sip(),
                  lep1_miniIso(),
                  lep1_p4.pt()/lep1_closeJet.pt(), // pt ratio
                  lep1_ptrel_v1(),
                  (std::abs(lep1_id())==11)?lep1_MVA_value():0,
                  (std::abs(lep1_id())==11)?lep1_convVeto():0, // els_conv_vtx_flag().at(elIdx) in CMS3
                  (std::abs(lep1_id())==11)?lep1_nPixelMiss():0, // els_lost_pixelhits().at(elIdx) in CMS3
                  lep1_tightCharge(), // tightChargeEle() in CORE/ElectronSelections.cc
                  lep1_motherID() > 0, // is MC truth matched?
                  lep2_p4().pt(),
                  lep2_p4().eta(),
                  lep2_p4().phi(),
                  lep2_id(),
                  lep2_coneCorrPt(),
                  lep2_dxyPV(),
                  lep2_dZ(),
                  lep2_sip(),
                  lep2_miniIso(),
                  lep2_p4.pt()/lep2_closeJet.pt(),
                  lep2_ptrel_v1(),
                  (std::abs(lep2_id())==11)?lep2_MVA_value():0,
                  (std::abs(lep2_id())==11)?lep2_convVeto():0,
                  (std::abs(lep2_id())==11)?lep2_nPixelMiss():0,
                  lep2_tightCharge(),
                  lep2_motherID() > 0);

          textfile_jets << Form("%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d",
                  jets().at(0).pt(),
                  jets().at(0).eta(),
                  jets().at(0).phi(),
                  jets_disc_mva().at(0),
                  jets_disc().at(0),
                  jets().at(1).pt(),
                  jets().at(1).eta(),
                  jets().at(1).phi(),
                  jets_disc_mva().at(1),
                  jets_disc().at(1));
      }

    }//event loop
  }//file loop
  
  //Close output text file
  textfile.close();

  //Sort the files
  system("sort -n -k1 -k2 -k3 unsorted.txt > ucsx_TTW_76X_HighHigh_dump.txt");
  system("rm unsorted.txt"); 


}
