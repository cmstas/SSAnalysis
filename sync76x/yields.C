#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "../classFiles/v8.01/SS.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"
#include "../CORE/Tools/dorky/dorky.h"

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

void yields(){

  // Make chains, histograms
  TChain* chain = new TChain("t");
  // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/synch_ttw_1.root"); 
  chain->Add("synch_ttw_1.root"); 

  // Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  // Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // Set up output text file
  ofstream textfile_global, textfile_leptons, textfile_jets;
  textfile_global.open("ucsx_TTW_76X_HighHigh_dump_global.txt");
  textfile_leptons.open("ucsx_TTW_76X_HighHigh_dump_leptons.txt");
  textfile_jets.open("ucsx_TTW_76X_HighHigh_dump_jets.txt");

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
          textfile_global << Form("%1d%9d%12d\t%2d\t%+2d %5.1f\t%+2d%5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d\n", 
                  ss::run(), ss::lumi(), ss::event(), 
                  ss::nVetoElectrons7()+ss::nVetoMuons5(),
                  ss::lep1_id(), ss::lep1_p4().pt(), 
                  ss::lep2_id(), ss::lep2_p4().pt(), 
                  ss::njets(), ss::nbtags(), ss::rawmet(), ss::ht(), SR); 

          textfile_leptons << Form("%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\t%1d\t%1d\t%1d\t%5.1f\t%5.1f\t%5.1f\t%+2d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\t%1d\t%1d\t%1d\n",
                  ss::lep1_p4().pt(),
                  ss::lep1_p4().eta(),
                  ss::lep1_p4().phi(),
                  ss::lep1_id(),
                  ss::lep1_coneCorrPt(),
                  ss::lep1_dxyPV(),
                  ss::lep1_dZ(),
                  ss::lep1_sip(),
                  ss::lep1_miniIso(),
                  ss::lep1_p4().pt()/ss::lep1_closeJet().pt(), // pt ratio
                  ss::lep1_ptrel_v1(),
                  (std::abs(ss::lep1_id())==11)?ss::lep1_MVA():0,
                  (std::abs(ss::lep1_id())==11)?ss::lep1_el_conv_vtx_flag():0, // els_conv_vtx_flag().at(elIdx) in CMS3
                  (std::abs(ss::lep1_id())==11)?ss::lep1_el_exp_innerlayers():0,
                  ss::lep1_tightCharge(), // tightChargeEle() in CORE/ElectronSelections.cc
                  ss::lep1_motherID() == 1, // "lep1_MCTruthMatched"  (function returns 0 if fails matching)
                  ss::lep2_p4().pt(),
                  ss::lep2_p4().eta(),
                  ss::lep2_p4().phi(),
                  ss::lep2_id(),
                  ss::lep2_coneCorrPt(),
                  ss::lep2_dxyPV(),
                  ss::lep2_dZ(),
                  ss::lep2_sip(),
                  ss::lep2_miniIso(),
                  ss::lep2_p4().pt()/ss::lep2_closeJet().pt(),
                  ss::lep2_ptrel_v1(),
                  (std::abs(ss::lep2_id())==11)?ss::lep2_MVA():0,
                  (std::abs(ss::lep2_id())==11)?ss::lep2_el_conv_vtx_flag():0,
                  (std::abs(ss::lep2_id())==11)?ss::lep2_el_exp_innerlayers():0,
                  ss::lep2_tightCharge(),
                  ss::lep2_motherID() == 1);

          textfile_jets << Form("%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\t%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d\n",
                  ss::jets().at(0).pt(),
                  ss::jets().at(0).eta(),
                  ss::jets().at(0).phi(),
                  ss::jets_disc_mva().at(0), // "jet1_btagCSVmva"
                  ss::jets_disc().at(0) > 0.800, // "jet1_btagCSVMedium". medium WP
                  ss::jets().at(1).pt(),
                  ss::jets().at(1).eta(),
                  ss::jets().at(1).phi(),
                  ss::jets_disc_mva().at(1),
                  ss::jets_disc().at(1) > 0.800);
      }

    }//event loop
  }//file loop
  
  //Close output text file
  textfile_global.close();
  textfile_leptons.close();
  textfile_jets.close();

  //Sort the files
  // system("sort -n -k1 -k2 -k3 unsorted_leptons.txt > ucsx_TTW_76X_HighHigh_dump_leptons.txt");
  // system("sort -n -k1 -k2 -k3 unsorted_jets.txt > ucsx_TTW_76X_HighHigh_dump_jets.txt");
  // system("sort -n -k1 -k2 -k3 unsorted_global.txt > ucsx_TTW_76X_HighHigh_dump_global.txt");
  // system("rm unsorted*.txt"); 


}
