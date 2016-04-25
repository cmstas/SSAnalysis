#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "../classFiles/v6.02/SS.h"
#include "../CORE/Tools/utils.h"
#include "../commonUtils.h"
#include "../CORE/Tools/dorky/dorky.h"

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

void yields(){

  //Make chains, histograms
  TChain* chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.00-fix/synch_ttw_1.root"); 

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

      // Determine type
      int type = -1;
      if (ss::hyp_type() == 3) type = 2;
      if (ss::hyp_type() == 2 || ss::hyp_type() == 1) type = 1;
      if (ss::hyp_type() == 0) type = 0;

      if (type == -1) continue;
   
      //Figure out region
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
                  lep1_pt(),lep1_eta(),lep1_phi(),lep1_pdgId(),lep1_conePt(), // FIXME
                  lep1_dxy(),lep1_dz(),lep1_SIP()lep1_miniIso(),lep1_ptRatio(),lep1_ptRel(), // FIXME
                  (std::abs(lep1_pdgId()==11)?(lep1_mvaId()):0),(std::abs(lep1_pdgId()==11)?(lep1_convVeto()):0), // FIXME
                  (std::abs(lep1_pdgId()==11)?(lep1_nPixelMiss()):0),lep1_tightCharge(),lep1_MCTruthMatched(), // FIXME
                  lep2_pt(),lep2_eta(),lep2_phi(),lep2_pdgId(),lep2_conePt(),lep2_dxy(),lep2_dz(),lep2_SIP(), // FIXME
                  lep2_miniIso(),lep2_ptRatio(),lep2_ptRel(),(std::abs(lep2_pdgId()==11)?(lep2_mvaId()):0), // FIXME
                  (std::abs(lep2_pdgId()==11)?(lep2_convVeto()):0),(std::abs(lep2_pdgId()==11)?(lep2_nPixelMiss()):0), // FIXME
                  lep2_tightCharge(),lep2_MCTruthMatched()); // FIXME

          textfile_jets << Form("%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d%5.1f\t%5.1f\t%5.1f\t%5.1f\t%1d",
                  jet1_pt(),jet1_eta(),jet1_phi(),jet1_btagCSVmva(),jet1_btagCSVMedium(), // FIXME
                  jet2_pt(),jet2_eta(), jet2_phi(),jet2_btagCSVmva(),jet2_btagCSVMedium()); // FIXME
      }

    }//event loop
  }//file loop
  
  //Close output text file
  textfile.close();

  //Sort the files
  system("sort -n -k1 -k2 -k3 unsorted.txt > ucsx_TTW_76X_HighHigh_dump.txt");
  system("rm unsorted.txt"); 


}
