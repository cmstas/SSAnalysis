#include "TChain.h"
#include "TFile.h"
#include "SS.h"
#include "../../software/tableMaker/CTable.h"
#include "../../CORE/SSSelections.h"

//Lumi
float lumi = 10.0;

//FO4
bool FO4 = true;

float functionAG_FO1(LorentzVector lep, int id){
  if (abs(id) == 11){ 
    if (fabs(lep.eta()) < 1){ 
      if (fabs(lep.pt()) < 15) return 0.0596617;
      if (fabs(lep.pt()) < 25) return 0.0560961;
      if (fabs(lep.pt()) < 35) return 0.0591798;
      if (fabs(lep.pt()) < 50) return 0.0804231;
      return 0.163637;
    } 
    if (fabs(lep.eta()) < 2){ 
      if (fabs(lep.pt()) < 15) return 0.0620429;
      if (fabs(lep.pt()) < 25) return 0.0541576;
      if (fabs(lep.pt()) < 35) return 0.0835479;
      if (fabs(lep.pt()) < 50) return 0.083994;
      return 0.227872;
    } 
    if (fabs(lep.eta()) < 4){ 
      if (fabs(lep.pt()) < 15) return 0.0689655;
      if (fabs(lep.pt()) < 25) return 0.0799999;
      if (fabs(lep.pt()) < 35) return 0.114754;
      if (fabs(lep.pt()) < 50) return 0.146341;
      return 0.176471;
    } 
  } 
  if (abs(id) == 13){ 
    if (fabs(lep.eta()) < 1){ 
      if (fabs(lep.pt()) < 15) return 0.0724891;
      if (fabs(lep.pt()) < 25) return 0.0667128;
      if (fabs(lep.pt()) < 35) return 0.0721167;
      if (fabs(lep.pt()) < 50) return 0.0864296;
      return 0.0785428;
    } 
    if (fabs(lep.eta()) < 2){ 
      if (fabs(lep.pt()) < 15) return 0.0696282;
      if (fabs(lep.pt()) < 25) return 0.0742888;
      if (fabs(lep.pt()) < 35) return 0.0806561;
      if (fabs(lep.pt()) < 50) return 0.0863229;
      return 0.111888;
    } 
    if (fabs(lep.eta()) < 4){ 
      if (fabs(lep.pt()) < 15) return 0.116402;
      if (fabs(lep.pt()) < 25) return 0.0930738;
      if (fabs(lep.pt()) < 35) return 0.0773479;
      if (fabs(lep.pt()) < 50) return 0.0833334;
      return 0.115385;
    } 
  } 
  return 0;
}  

float functionAG_FO4(LorentzVector lep, int id){
  if (abs(id) == 11){ 
    if (fabs(lep.eta()) < 1){ 
      if (fabs(lep.pt()) < 15) return 0.405474;
      if (fabs(lep.pt()) < 25) return 0.199506;
      if (fabs(lep.pt()) < 35) return 0.10685;
      if (fabs(lep.pt()) < 50) return 0.10411;
      return 0.181361;
    } 
    if (fabs(lep.eta()) < 2){ 
      if (fabs(lep.pt()) < 15) return 0.438143;
      if (fabs(lep.pt()) < 25) return 0.242775;
      if (fabs(lep.pt()) < 35) return 0.181564;
      if (fabs(lep.pt()) < 50) return 0.124122;
      return 0.286729;
    } 
    if (fabs(lep.eta()) < 4){ 
      if (fabs(lep.pt()) < 15) return 0.363636;
      if (fabs(lep.pt()) < 25) return 0.48;
      if (fabs(lep.pt()) < 35) return 0.25;
      if (fabs(lep.pt()) < 50) return 0.214286;
      return 0.230769;
    } 
  } 
  if (abs(id) == 13){ 
    if (fabs(lep.eta()) < 1){ 
      if (fabs(lep.pt()) < 15) return 0.439522;
      if (fabs(lep.pt()) < 25) return 0.23404;
      if (fabs(lep.pt()) < 35) return 0.134676;
      if (fabs(lep.pt()) < 50) return 0.123153;
      return 0.0992838;
    } 
    if (fabs(lep.eta()) < 2){ 
      if (fabs(lep.pt()) < 15) return 0.47399;
      if (fabs(lep.pt()) < 25) return 0.30977;
      if (fabs(lep.pt()) < 35) return 0.176471;
      if (fabs(lep.pt()) < 50) return 0.144195;
      return 0.154839;
    } 
    if (fabs(lep.eta()) < 4){ 
      if (fabs(lep.pt()) < 15) return 0.594594;
      if (fabs(lep.pt()) < 25) return 0.328244;
      if (fabs(lep.pt()) < 35) return 0.177215;
      if (fabs(lep.pt()) < 50) return 0.142857;
      return 0.153846;
    } 
  } 
  return 0; 
} 

float functionAG(LorentzVector lep, int id){
  if (FO4)  return functionAG_FO4(lep, id);
  if (!FO4) return functionAG_FO1(lep, id);
}


void closure(){

  //Declare chain
  TChain *chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.19/TTBAR.root");

  //Arrays to store results
  float nFake_e[3][4] = { { 0 } }; 
  float nFake_m[3][4] = { { 0 } }; 
  float nPred_e[3][4] = { { 0 } }; 
  float nPred_m[3][4] = { { 0 } }; 

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

      //Event Selection
      if (baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_p4().pt(), ss::lep2_p4().pt()) < 0) continue;
      if (ss::hyp_class() == 4) continue;

      //FO1 vs. FO4 selection
      bool lep1_denom_iso = false;
      bool lep2_denom_iso = false;
      float ptrel_cut_1 = (abs(ss::lep1_id()) == 11 ? 7.0 : 6.7); 
      float ptrel_cut_2 = (abs(ss::lep2_id()) == 11 ? 7.0 : 6.7); 
      float ptratio_cut_1 = (abs(ss::lep1_id()) == 11 ? 0.7 : 0.68); 
      float ptratio_cut_2 = (abs(ss::lep2_id()) == 11 ? 0.7 : 0.68); 
      if (!FO4) lep1_denom_iso = (ss::lep1_miniIso() < 0.4);
      if (!FO4) lep2_denom_iso = (ss::lep2_miniIso() < 0.4);
      if (FO4)  lep1_denom_iso = ((ss::lep1_miniIso() < 0.4) || (ss::lep1_ptrel_v1() > ptrel_cut_1) || ((ss::lep1_closeJet().pt()/ss::lep1_p4().pt()) < (1/ptratio_cut_1 + ss::lep1_miniIso()))); 
      if (FO4)  lep2_denom_iso = ((ss::lep2_miniIso() < 0.4) || (ss::lep2_ptrel_v1() > ptrel_cut_2) || ((ss::lep2_closeJet().pt()/ss::lep2_p4().pt()) < (1/ptratio_cut_2 + ss::lep2_miniIso()))); 

      //Calculate yields
      for (int j = 0; j < 3; j++){
        if (j == 0 && analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) != HighHigh) continue;
        if (j == 1 && analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) != HighLow ) continue;
        if (j == 2 && analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) != LowLow  ) continue;
        for (int i = 0; i < 4; i++){
          if (ss::nbtags() != i && i < 3) continue;
          if (ss::nbtags() < 3 && i == 3) continue;
          //Actual yield -- events that are fake, SIP3D < 4 and isolated
          if (ss::lep1_isFakeLeg() && ss::lep2_isGoodLeg() && ss::lep2_passes_id() && abs(ss::lep1_id()) == 11 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso() && lep1_denom_iso) nFake_e[j][i] += ss::scale1fb()*lumi; 
          if (ss::lep1_isFakeLeg() && ss::lep2_isGoodLeg() && ss::lep2_passes_id() && abs(ss::lep1_id()) == 13 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso() && lep1_denom_iso) nFake_m[j][i] += ss::scale1fb()*lumi; 
          if (ss::lep2_isFakeLeg() && ss::lep1_isGoodLeg() && ss::lep1_passes_id() && abs(ss::lep2_id()) == 11 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso() && lep2_denom_iso) nFake_e[j][i] += ss::scale1fb()*lumi; 
          if (ss::lep2_isFakeLeg() && ss::lep1_isGoodLeg() && ss::lep1_passes_id() && abs(ss::lep2_id()) == 13 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso() && lep2_denom_iso) nFake_m[j][i] += ss::scale1fb()*lumi; 
          //Predicted yield -- events that are real, SIP3D < 4, and not isolated
          if (ss::lep1_isFakeLeg() && ss::lep2_isGoodLeg() && ss::lep2_passes_id() && abs(ss::lep1_id()) == 11 && abs(ss::lep1_sip()) < 4 && !ss::lep1_multiIso() && lep1_denom_iso) nPred_e[j][i] += ss::scale1fb()*lumi*functionAG(ss::lep1_p4(), 11)/(1.0-functionAG(ss::lep1_p4(), 11)); 
          if (ss::lep1_isFakeLeg() && ss::lep2_isGoodLeg() && ss::lep2_passes_id() && abs(ss::lep1_id()) == 13 && abs(ss::lep1_sip()) < 4 && !ss::lep1_multiIso() && lep1_denom_iso) nPred_m[j][i] += ss::scale1fb()*lumi*functionAG(ss::lep1_p4(), 13)/(1.0-functionAG(ss::lep1_p4(), 13)); 
          if (ss::lep2_isFakeLeg() && ss::lep1_isGoodLeg() && ss::lep1_passes_id() && abs(ss::lep2_id()) == 11 && abs(ss::lep2_sip()) < 4 && !ss::lep2_multiIso() && lep2_denom_iso) nPred_e[j][i] += ss::scale1fb()*lumi*functionAG(ss::lep2_p4(), 11)/(1.0-functionAG(ss::lep2_p4(), 11)); 
          if (ss::lep2_isFakeLeg() && ss::lep1_isGoodLeg() && ss::lep1_passes_id() && abs(ss::lep2_id()) == 13 && abs(ss::lep2_sip()) < 4 && !ss::lep2_multiIso() && lep2_denom_iso) nPred_m[j][i] += ss::scale1fb()*lumi*functionAG(ss::lep2_p4(), 13)/(1.0-functionAG(ss::lep2_p4(), 13)); 
        }
      }

    }//event loop
  }//file loop
  
  for (int j = 0; j < 3; j++){
    CTable table; 
    if (j == 0) table.setTitle("electrons H-H");
    if (j == 1) table.setTitle("electrons H-L");
    if (j == 2) table.setTitle("electrons L-L");
    table.useTitle(); 
    table.setTable() ( "pred", "obs", "pred/obs", "(p-o)/p") 
         ("0 b-tags" , nPred_e[j][0], nFake_e[j][0], nPred_e[j][0]/nFake_e[j][0], fabs(nPred_e[j][0]-nFake_e[j][0])/nPred_e[j][0]) 
         ("1 b-tags" , nPred_e[j][1], nFake_e[j][1], nPred_e[j][1]/nFake_e[j][1], fabs(nPred_e[j][1]-nFake_e[j][1])/nPred_e[j][1]) 
         ("2 b-tags" , nPred_e[j][2], nFake_e[j][2], nPred_e[j][2]/nFake_e[j][2], fabs(nPred_e[j][2]-nFake_e[j][2])/nPred_e[j][2]) 
         ("3+ b-tags", nPred_e[j][3], nFake_e[j][3], nPred_e[j][3]/nFake_e[j][3], fabs(nPred_e[j][3]-nFake_e[j][3])/nPred_e[j][3]);
    table.print();

    CTable table2; 
    if (j == 0) table2.setTitle("muons H-H");
    if (j == 1) table2.setTitle("muons H-L");
    if (j == 2) table2.setTitle("muons L-L");
    table2.useTitle(); 
    table2.setTable() ( "pred", "obs", "pred/obs", "(p-o)/p") 
         ("0 b-tags" , nPred_m[j][0], nFake_m[j][0], nPred_m[j][0]/nFake_m[j][0], fabs(nPred_m[j][0]-nFake_m[j][0])/nPred_m[j][0]) 
         ("1 b-tags" , nPred_m[j][1], nFake_m[j][1], nPred_m[j][1]/nFake_m[j][1], fabs(nPred_m[j][1]-nFake_m[j][1])/nPred_m[j][1]) 
         ("2 b-tags" , nPred_m[j][2], nFake_m[j][2], nPred_m[j][2]/nFake_m[j][2], fabs(nPred_m[j][2]-nFake_m[j][2])/nPred_m[j][2]) 
         ("3+ b-tags", nPred_m[j][3], nFake_m[j][3], nPred_m[j][3]/nFake_m[j][3], fabs(nPred_m[j][3]-nFake_m[j][3])/nPred_m[j][3]);
    table2.print();
  }

}





