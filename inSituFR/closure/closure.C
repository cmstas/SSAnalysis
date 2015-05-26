#include "TChain.h"
#include "TFile.h"
#include "SS.h"
#include "../../software/tableMaker/CTable.h"
#include "../../CORE/SSSelections.h"

//Lumi
float lumi = 10.0;

float functionFR(LorentzVector lep, int id){
  if (abs(id) == 11){ 
    if (fabs(lep.eta()) < 1){ 
      if (fabs(lep.pt()) < 15) return 0.0608462;
      if (fabs(lep.pt()) < 25) return 0.0562874;
      if (fabs(lep.pt()) < 35) return 0.0595631;
      if (fabs(lep.pt()) < 50) return 0.0771002;
      return 0.163017;
    } 
    if (fabs(lep.eta()) < 2){ 
      if (fabs(lep.pt()) < 15) return 0.0642792;
      if (fabs(lep.pt()) < 25) return 0.0516121;
      if (fabs(lep.pt()) < 35) return 0.0809662;
      if (fabs(lep.pt()) < 50) return 0.0907507;
      return 0.221545;
    } 
    if (fabs(lep.eta()) < 4){ 
      if (fabs(lep.pt()) < 15) return 0.0652174;
      if (fabs(lep.pt()) < 25) return 0.090909;
      if (fabs(lep.pt()) < 35) return 0.0545455;
      if (fabs(lep.pt()) < 50) return 0.135135;
      return 0.153846;
    } 
  } 
  if (abs(id) == 13){ 
    if (fabs(lep.eta()) < 1){ 
      if (fabs(lep.pt()) < 15) return 0.0775365;
      if (fabs(lep.pt()) < 25) return 0.0712675;
      if (fabs(lep.pt()) < 35) return 0.0719187;
      if (fabs(lep.pt()) < 50) return 0.0865916;
      return 0.0802095;
    } 
    if (fabs(lep.eta()) < 2){ 
      if (fabs(lep.pt()) < 15) return 0.0720359;
      if (fabs(lep.pt()) < 25) return 0.0775695;
      if (fabs(lep.pt()) < 35) return 0.0847586;
      if (fabs(lep.pt()) < 50) return 0.0875302;
      return 0.110974;
    } 
    if (fabs(lep.eta()) < 4){ 
      if (fabs(lep.pt()) < 15) return 0.121331;
      if (fabs(lep.pt()) < 25) return 0.0983609;
      if (fabs(lep.pt()) < 35) return 0.0701753;
      if (fabs(lep.pt()) < 50) return 0.0873787;
      return 0.117647;
    } 
  } 
  return 0; 
} 

void closure(){

  //Declare chain
  TChain *chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.16/TTBAR_multiIso.root");

  //nfakes
  float nFake_e0 = 0; 
  float nFake_e1 = 0; 
  float nFake_e2 = 0; 
  float nFake_e3 = 0; 
  float nFake_m0 = 0; 
  float nFake_m1 = 0; 
  float nFake_m2 = 0; 
  float nFake_m3 = 0; 

  //npred
  float nPred_e0 = 0; 
  float nPred_e1 = 0; 
  float nPred_e2 = 0; 
  float nPred_e3 = 0; 
  float nPred_m0 = 0; 
  float nPred_m1 = 0; 
  float nPred_m2 = 0; 
  float nPred_m3 = 0; 

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

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

      //Reject events that don't pass selection (inclusive for SIP and multiIso)
      if (ss::hyp_class() == 1 || ss::hyp_class() == 4) continue;
      if (!ss::truth_inSituFR()) continue;

      //Event Selection
      if (baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_p4().pt(), ss::lep2_p4().pt()) < 0) continue;
      if (analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) != HighHigh) continue;

      //Actual yield -- events that are fake, SIP3D < 4 and isolated
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 0 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso()) nFake_e0 += ss::scale1fb()*10.0; 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 0 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso()) nFake_e0 += ss::scale1fb()*10.0; 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 1 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso()) nFake_e1 += ss::scale1fb()*10.0; 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 1 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso()) nFake_e1 += ss::scale1fb()*10.0; 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 2 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso()) nFake_e2 += ss::scale1fb()*10.0; 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 2 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso()) nFake_e2 += ss::scale1fb()*10.0; 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() >= 3 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso()) nFake_e3 += ss::scale1fb()*10.0; 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() >= 3 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso()) nFake_e3 += ss::scale1fb()*10.0; 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 0 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso()) nFake_m0 += ss::scale1fb()*10.0; 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 0 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso()) nFake_m0 += ss::scale1fb()*10.0; 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 1 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso()) nFake_m1 += ss::scale1fb()*10.0; 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 1 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso()) nFake_m1 += ss::scale1fb()*10.0; 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 2 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso()) nFake_m2 += ss::scale1fb()*10.0; 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 2 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso()) nFake_m2 += ss::scale1fb()*10.0; 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() >= 3 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso()) nFake_m3 += ss::scale1fb()*10.0; 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() >= 3 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso()) nFake_m3 += ss::scale1fb()*10.0; 

      //Predicted yield -- events that are real, SIP3D < 4, and not isolated
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 0 && abs(ss::lep1_sip()) < 4) nPred_e0 += ss::scale1fb()*10.0*functionFR(ss::lep1_p4(), 11); 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 0 && abs(ss::lep2_sip()) < 4) nPred_e0 += ss::scale1fb()*10.0*functionFR(ss::lep2_p4(), 11); 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 1 && abs(ss::lep1_sip()) < 4) nPred_e1 += ss::scale1fb()*10.0*functionFR(ss::lep1_p4(), 11); 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 1 && abs(ss::lep2_sip()) < 4) nPred_e1 += ss::scale1fb()*10.0*functionFR(ss::lep2_p4(), 11); 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 2 && abs(ss::lep1_sip()) < 4) nPred_e2 += ss::scale1fb()*10.0*functionFR(ss::lep1_p4(), 11); 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() == 2 && abs(ss::lep2_sip()) < 4) nPred_e2 += ss::scale1fb()*10.0*functionFR(ss::lep2_p4(), 11); 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() >= 3 && abs(ss::lep1_sip()) < 4) nPred_e3 += ss::scale1fb()*10.0*functionFR(ss::lep1_p4(), 11); 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 11 && ss::nbtags() >= 3 && abs(ss::lep2_sip()) < 4) nPred_e3 += ss::scale1fb()*10.0*functionFR(ss::lep2_p4(), 11); 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 0 && abs(ss::lep1_sip()) < 4) nPred_m0 += ss::scale1fb()*10.0*functionFR(ss::lep1_p4(), 13); 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 0 && abs(ss::lep2_sip()) < 4) nPred_m0 += ss::scale1fb()*10.0*functionFR(ss::lep2_p4(), 13); 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 1 && abs(ss::lep1_sip()) < 4) nPred_m1 += ss::scale1fb()*10.0*functionFR(ss::lep1_p4(), 13); 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 1 && abs(ss::lep2_sip()) < 4) nPred_m1 += ss::scale1fb()*10.0*functionFR(ss::lep2_p4(), 13); 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 2 && abs(ss::lep1_sip()) < 4) nPred_m2 += ss::scale1fb()*10.0*functionFR(ss::lep1_p4(), 13); 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() == 2 && abs(ss::lep2_sip()) < 4) nPred_m2 += ss::scale1fb()*10.0*functionFR(ss::lep2_p4(), 13); 
      if (ss::lep1_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() >= 3 && abs(ss::lep1_sip()) < 4) nPred_m3 += ss::scale1fb()*10.0*functionFR(ss::lep1_p4(), 13); 
      if (ss::lep2_isFakeLeg() && abs(ss::lep1_id()) == 13 && ss::nbtags() >= 3 && abs(ss::lep2_sip()) < 4) nPred_m3 += ss::scale1fb()*10.0*functionFR(ss::lep2_p4(), 13); 

    }//event loop
  }//file loop
  
  CTable table; 
  table.setTitle("electrons");
  table.useTitle(); 

  table.setTable() ( "pred", "obs", "pred/obs", "(p-o)/p") 
       ("0 b-tags" , nPred_e0, nFake_e0, nPred_e0/nFake_e0, fabs(nPred_e0-nFake_e0)/nPred_e0) 
       ("1 b-tags" , nPred_e1, nFake_e1, nPred_e1/nFake_e1, fabs(nPred_e1-nFake_e1)/nPred_e1) 
       ("2 b-tags" , nPred_e2, nFake_e2, nPred_e2/nFake_e2, fabs(nPred_e2-nFake_e2)/nPred_e2) 
       ("3+ b-tags", nPred_e3, nFake_e3, nPred_e3/nFake_e3, fabs(nPred_e3-nFake_e3)/nPred_e3);
  table.print();

  CTable table2; 
  table2.setTitle("muons");
  table2.useTitle(); 

  table2.setTable() ( "pred", "obs", "pred/obs", "(p-o)/p") 
       ("0 b-tags" , nPred_m0, nFake_m0, nPred_m0/nFake_m0, fabs(nPred_m0-nFake_m0)/nPred_m0) 
       ("1 b-tags" , nPred_m1, nFake_m1, nPred_m1/nFake_m1, fabs(nPred_m1-nFake_m1)/nPred_m1) 
       ("2 b-tags" , nPred_m2, nFake_m2, nPred_m2/nFake_m2, fabs(nPred_m2-nFake_m2)/nPred_m2) 
       ("3+ b-tags", nPred_m3, nFake_m3, nPred_m3/nFake_m3, fabs(nPred_m3-nFake_m3)/nPred_m3);
  table2.print();

}
