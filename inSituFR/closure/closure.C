#include "TChain.h"
#include "TFile.h"
#include "SS.h"
#include "../../software/tableMaker/CTable.h"
#include "../../CORE/SSSelections.h"

//Lumi
float lumi = 10.0;

//FO4
bool FO4 = true;

bool isFakeLeg(int lep){
  if (lep == 1) return (ss::lep1_motherID() <= 0); 
  if (lep == 2) return (ss::lep2_motherID() <= 0); 
  return 0;
}

bool isGoodLeg(int lep){
  if (lep == 1) return (ss::lep1_motherID() > 0); 
  if (lep == 2) return (ss::lep2_motherID() > 0); 
  return 0;
}

float functionAG_FO4(float pt, float eta, int id);

float functionAG(float pt, float eta, int id){
  return functionAG_FO4(pt, eta, id);
}


void closure(){

  //Declare chain
  TChain *chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.21/TTBAR.root");

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
      if (FO4)  lep1_denom_iso = ((ss::lep1_miniIso() < 0.4) && ((ss::lep1_ptrel_v1() > ptrel_cut_1) || ((ss::lep1_closeJet().pt()/ss::lep1_p4().pt()) < (1/ptratio_cut_1 + ss::lep1_miniIso())))); 
      if (FO4)  lep2_denom_iso = ((ss::lep2_miniIso() < 0.4) && ((ss::lep2_ptrel_v1() > ptrel_cut_2) || ((ss::lep2_closeJet().pt()/ss::lep2_p4().pt()) < (1/ptratio_cut_2 + ss::lep2_miniIso())))); 

      //Calculate yields
      for (int j = 0; j < 3; j++){
        if (j == 0 && analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) != HighHigh) continue;
        if (j == 1 && analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) != HighLow ) continue;
        if (j == 2 && analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt()) != LowLow  ) continue;
        for (int i = 0; i < 4; i++){
          if (ss::nbtags() != i && i < 3) continue;
          if (ss::nbtags() < 3 && i == 3) continue;
          //Actual yield -- events that are fake, SIP3D < 4 and isolated
          if (isFakeLeg(1) && isGoodLeg(2) && ss::lep2_passes_id() && abs(ss::lep1_id()) == 11 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso() && lep1_denom_iso) nFake_e[j][i] += ss::scale1fb()*lumi; 
          if (isFakeLeg(1) && isGoodLeg(2) && ss::lep2_passes_id() && abs(ss::lep1_id()) == 13 && abs(ss::lep1_sip()) < 4 && ss::lep1_multiIso() && lep1_denom_iso) nFake_m[j][i] += ss::scale1fb()*lumi; 
          if (isFakeLeg(2) && isGoodLeg(1) && ss::lep1_passes_id() && abs(ss::lep2_id()) == 11 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso() && lep2_denom_iso) nFake_e[j][i] += ss::scale1fb()*lumi; 
          if (isFakeLeg(2) && isGoodLeg(1) && ss::lep1_passes_id() && abs(ss::lep2_id()) == 13 && abs(ss::lep2_sip()) < 4 && ss::lep2_multiIso() && lep2_denom_iso) nFake_m[j][i] += ss::scale1fb()*lumi; 
          //Predicted yield -- events that are real, SIP3D < 4, and not isolated
          if (isFakeLeg(1) && isGoodLeg(2) && ss::lep2_passes_id() && abs(ss::lep1_id()) == 11 && abs(ss::lep1_sip()) < 4 && !ss::lep1_multiIso() && lep1_denom_iso) nPred_e[j][i] += ss::scale1fb()*lumi*functionAG(ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), 11)/(1.0-functionAG(ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), 11)); 
          if (isFakeLeg(1) && isGoodLeg(2) && ss::lep2_passes_id() && abs(ss::lep1_id()) == 13 && abs(ss::lep1_sip()) < 4 && !ss::lep1_multiIso() && lep1_denom_iso) nPred_m[j][i] += ss::scale1fb()*lumi*functionAG(ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), 13)/(1.0-functionAG(ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), 13)); 
          if (isFakeLeg(2) && isGoodLeg(1) && ss::lep1_passes_id() && abs(ss::lep2_id()) == 11 && abs(ss::lep2_sip()) < 4 && !ss::lep2_multiIso() && lep2_denom_iso) nPred_e[j][i] += ss::scale1fb()*lumi*functionAG(ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), 11)/(1.0-functionAG(ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), 11)); 
          if (isFakeLeg(2) && isGoodLeg(1) && ss::lep1_passes_id() && abs(ss::lep2_id()) == 13 && abs(ss::lep2_sip()) < 4 && !ss::lep2_multiIso() && lep2_denom_iso) nPred_m[j][i] += ss::scale1fb()*lumi*functionAG(ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), 13)/(1.0-functionAG(ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), 13)); 
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
    table.forSlideMaker("table.tex"); 

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

  float overall_p[3] = { 0 };
  float overall_o[3] = { 0 };
  float overall[3]   = { 0 };
  for (int j = 0; j < 3; j++){
    for (int i = 0; i < 4; i++){
      overall_p[j] += nPred_e[j][i] + nPred_m[j][i]; 
      overall_o[j] += nFake_e[j][i] + nFake_m[j][i]; 
    }
  }
  for (int i = 0; i < 3; i++) overall[i] = overall_p[i]/overall_o[i]; 


  cout << "Overall closure H-H (100\% best): " << 100*overall[0] << "%" << endl;
  cout << "Overall closure H-L (100\% best): " << 100*overall[1] << "%" << endl;
  cout << "Overall closure L-L (100\% best): " << 100*overall[2] << "%" << endl;

}





