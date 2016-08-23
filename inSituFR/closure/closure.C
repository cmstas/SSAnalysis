#include "TChain.h"
#include "TFile.h"
#include "../../classFiles/v6.02/SS.h"
#include "../../commonUtils.h"
#include "../../software/tableMaker/CTable.h"
#include "../../CORE/SSSelections.h"

//Lumi
float lumiNA = 20.0;

//FO4
bool FO4 = true;

//others
bool others = false;

//cont
bool cont = false;

bool isFakeLeg(int lep){
  if (lep == 1) return (ss::lep1_motherID() <= 0); 
  if (lep == 2) return (ss::lep2_motherID() <= 0); 
  return 0;
}

bool isGoodLeg(int lep){
  if (lep == 1) return (ss::lep1_motherID() == 1); 
  if (lep == 2) return (ss::lep2_motherID() == 1); 
  return 0;
}

float functionAG_FO4(float pt, float eta, int id);
float functionAG_PC(float pt, float eta, int id);
float functionAG_threesig_nocont(float pt, float eta, int id);
float FR(float pt, float eta, int id){
    return fakeRateInSitu(id, pt, eta);

  // if (FO4 && !cont) return functionAG_FO4(pt, eta, id);
  // if (FO4 && cont) return functionAG_PC(pt, eta, id);           //this has contaminiton
  //if (FO4 && cont) return functionAG_threesig_nocont(pt, eta, id);  //this has noe but all 3 signals
  // return 0; 
}

bool passesMVA(int which){
  if (which == 1 && abs(ss::lep1_id()) != 11) return true;
  if (which == 2 && abs(ss::lep1_id()) != 11) return true;
  // FIXME - remake babies to have SCeta
  float aeta = (which == 1) ? fabs(ss::lep1_p4().eta()) : fabs(ss::lep2_p4().eta());//uhm this should be SCeta
  float disc = (which == 1) ? ss::lep1_MVA() : ss::lep2_MVA();
  if (aeta < 0.8) return disc > 0.87;
  if ((aeta >= 0.8 && aeta <= 1.479)) return disc > 0.60;
  if (aeta > 1.479) return disc > 0.17;
  return false;
}

void closure(){

  //Declare chain
  string tag = getTag().Data();
  TChain *chain = new TChain("t");
  chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR_PH.root", tag.c_str()));
  if (others){
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_*.root", tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets.root", tag.c_str()));
  }

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

      //Non-Isolated trigger requirements
      if (ss::is_real_data() && !ss::fired_trigger()) continue; 
      if (ss::ht() < 300){
          if (!passesMVA(1) || !passesMVA(2)) continue;
      }

      //Event Selection
      //if (ss::event() != 308) continue;
      if (baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt()) < 0) continue;
      if (ss::hyp_class() == 4 || ss::hyp_class() == 6) continue;

      float ptrel_cut_1    = (abs(ss::lep1_id()) == 11 ? 7.20 : 7.20);
      float ptrel_cut_2    = (abs(ss::lep2_id()) == 11 ? 7.20 : 7.20);
      float ptratio_cut_1  = (abs(ss::lep1_id()) == 11 ? 0.80 : 0.76);
      float ptratio_cut_2  = (abs(ss::lep2_id()) == 11 ? 0.80 : 0.76);
      float mini_cut_1     = (abs(ss::lep1_id()) == 11 ? 0.12 : 0.16);
      float mini_cut_2     = (abs(ss::lep2_id()) == 11 ? 0.12 : 0.16);
      bool lep1_denom_iso  = ((ss::lep1_miniIso() < 0.4) && ((ss::lep1_ptrel_v1() > ptrel_cut_1) || ((ss::lep1_closeJet().pt()/ss::lep1_p4().pt()) < (1.0/ptratio_cut_1 + ss::lep1_miniIso()))));
      bool lep2_denom_iso  = ((ss::lep2_miniIso() < 0.4) && ((ss::lep2_ptrel_v1() > ptrel_cut_2) || ((ss::lep2_closeJet().pt()/ss::lep2_p4().pt()) < (1.0/ptratio_cut_2 + ss::lep2_miniIso()))));

      //Temporarily recalculate multiIso (this won't be necessary after v4.00)
      float miniiso_1 = ss::lep1_miniIso();
      float miniiso_2 = ss::lep2_miniIso();
      float ptrel_1   = ss::lep1_ptrel_v1();
      float ptrel_2   = ss::lep2_ptrel_v1();
      float ptratio_1 = ss::lep1_closeJet().pt() > 0 ? ss::lep1_p4().pt()/ss::lep1_closeJet().pt() : 1.0; 
      float ptratio_2 = ss::lep2_closeJet().pt() > 0 ? ss::lep2_p4().pt()/ss::lep2_closeJet().pt() : 1.0; 
      bool lep1_multiIso = passMultiIsoCuts(mini_cut_1, ptratio_cut_1, ptrel_cut_1, miniiso_1, ptratio_1, ptrel_1); 
      bool lep2_multiIso = passMultiIsoCuts(mini_cut_2, ptratio_cut_2, ptrel_cut_2, miniiso_2, ptratio_2, ptrel_2); 

      //Calculate yields
      for (int j = 0; j < 3; j++){
        if (j == 0 && analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()) != HighHigh) continue;
        if (j == 1 && analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()) != HighLow ) continue;
        if (j == 2 && analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt()) != LowLow  ) continue;
        for (int i = 0; i < 4; i++){
          if (ss::nbtags() != i && i < 3) continue;
          if (ss::nbtags() < 3 && i == 3) continue;
          //Actual yield -- events that are fake, SIP3D < 4 and isolated
          if (ss::hyp_class() == 3){
            if ((isFakeLeg(1) && isGoodLeg(2) && abs(ss::lep1_id()) == 11) || (isFakeLeg(2) && isGoodLeg(1) && abs(ss::lep2_id()) == 11)) nFake_e[j][i] += ss::scale1fb()*lumiNA; 
            if ((isFakeLeg(1) && isGoodLeg(2) && abs(ss::lep1_id()) == 13) || (isFakeLeg(2) && isGoodLeg(1) && abs(ss::lep2_id()) == 13)) nFake_m[j][i] += ss::scale1fb()*lumiNA; 
          }
          //Predicted yield -- events that are real, SIP3D < 4, and not isolated
          float fr1_el = FR(ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), 11);
          float fr2_el = FR(ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), 11);
          float fr1_mu = FR(ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), 13);
          float fr2_mu = FR(ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), 13);
          if (isFakeLeg(1) && isGoodLeg(2) && ss::lep2_passes_id() && abs(ss::lep1_id()) == 11 && abs(ss::lep1_sip()) < 4 && !lep1_multiIso && lep1_denom_iso) nPred_e[j][i] += ss::scale1fb()*lumiNA*fr1_el/(1.0-fr1_el);
          if (isFakeLeg(1) && isGoodLeg(2) && ss::lep2_passes_id() && abs(ss::lep1_id()) == 13 && abs(ss::lep1_sip()) < 4 && !lep1_multiIso && lep1_denom_iso) nPred_m[j][i] += ss::scale1fb()*lumiNA*fr1_mu/(1.0-fr1_mu);
          if (isFakeLeg(2) && isGoodLeg(1) && ss::lep1_passes_id() && abs(ss::lep2_id()) == 11 && abs(ss::lep2_sip()) < 4 && !lep2_multiIso && lep2_denom_iso) nPred_e[j][i] += ss::scale1fb()*lumiNA*fr2_el/(1.0-fr2_el);
          if (isFakeLeg(2) && isGoodLeg(1) && ss::lep1_passes_id() && abs(ss::lep2_id()) == 13 && abs(ss::lep2_sip()) < 4 && !lep2_multiIso && lep2_denom_iso) nPred_m[j][i] += ss::scale1fb()*lumiNA*fr2_mu/(1.0-fr2_mu);
        }
      }

    }//event loop
  }//file loop
  
  for (int j = 0; j < 3; j++){
    CTable table; 
    if (j == 0) table.setTitle("H-H");
    if (j == 1) table.setTitle("H-L");
    if (j == 2) table.setTitle("L-L");
    table.useTitle(); 
    table.setTable() ( "pred", "obs", "pred/obs", "(p-o)/p") 
         ("0 b-tags elec" , nPred_e[j][0], nFake_e[j][0], nPred_e[j][0]/nFake_e[j][0], fabs(nPred_e[j][0]-nFake_e[j][0])/nPred_e[j][0]) 
         ("1 b-tags elec" , nPred_e[j][1], nFake_e[j][1], nPred_e[j][1]/nFake_e[j][1], fabs(nPred_e[j][1]-nFake_e[j][1])/nPred_e[j][1]) 
         ("2 b-tags elec" , nPred_e[j][2], nFake_e[j][2], nPred_e[j][2]/nFake_e[j][2], fabs(nPred_e[j][2]-nFake_e[j][2])/nPred_e[j][2]) 
         ("3+ b-tags elec", nPred_e[j][3], nFake_e[j][3], nPred_e[j][3]/nFake_e[j][3], fabs(nPred_e[j][3]-nFake_e[j][3])/nPred_e[j][3])
         ("0 b-tags muon" , nPred_m[j][0], nFake_m[j][0], nPred_m[j][0]/nFake_m[j][0], fabs(nPred_m[j][0]-nFake_m[j][0])/nPred_m[j][0]) 
         ("1 b-tags muon" , nPred_m[j][1], nFake_m[j][1], nPred_m[j][1]/nFake_m[j][1], fabs(nPred_m[j][1]-nFake_m[j][1])/nPred_m[j][1]) 
         ("2 b-tags muon" , nPred_m[j][2], nFake_m[j][2], nPred_m[j][2]/nFake_m[j][2], fabs(nPred_m[j][2]-nFake_m[j][2])/nPred_m[j][2]) 
         ("3+ b-tags muon", nPred_m[j][3], nFake_m[j][3], nPred_m[j][3]/nFake_m[j][3], fabs(nPred_m[j][3]-nFake_m[j][3])/nPred_m[j][3]);
    table.print();
    if (j == 0) table.forSlideMaker("tableHH.tex"); 
    if (j == 1) table.forSlideMaker("tableHL.tex"); 
    if (j == 2) table.forSlideMaker("tableLL.tex"); 

  }

  float overall_p[3] = { 0 };
  float overall_o[3] = { 0 };
  float overall[3]   = { 0 };
  float el_overall_p[3] = { 0 };
  float el_overall_o[3] = { 0 };
  float el_overall[3]   = { 0 };
  float mu_overall_p[3] = { 0 };
  float mu_overall_o[3] = { 0 };
  float mu_overall[3]   = { 0 };
  for (int j = 0; j < 3; j++){
    for (int i = 0; i < 4; i++){
      overall_p[j] += nPred_e[j][i] + nPred_m[j][i]; 
      overall_o[j] += nFake_e[j][i] + nFake_m[j][i]; 
      el_overall_p[j] += nPred_e[j][i];
      el_overall_o[j] += nFake_e[j][i];
      mu_overall_p[j] += nPred_m[j][i];
      mu_overall_o[j] += nFake_m[j][i];
    }
  }
  for (int i = 0; i < 3; i++) {
      overall[i] = overall_p[i]/overall_o[i]; 
      el_overall[i] = el_overall_p[i]/el_overall_o[i]; 
      mu_overall[i] = mu_overall_p[i]/mu_overall_o[i]; 
  }

  cout << "__ electrons __" << endl;
  cout << "Overall closure H-H (100\% best): " << 100*el_overall[0] << "%" << endl;
  cout << "Overall closure H-L (100\% best): " << 100*el_overall[1] << "%" << endl;
  cout << "Overall closure L-L (100\% best): " << 100*el_overall[2] << "%" << endl;
  cout << endl;

  cout << "__ muons __" << endl;
  cout << "Overall closure H-H (100\% best): " << 100*mu_overall[0] << "%" << endl;
  cout << "Overall closure H-L (100\% best): " << 100*mu_overall[1] << "%" << endl;
  cout << "Overall closure L-L (100\% best): " << 100*mu_overall[2] << "%" << endl;
  cout << endl;

  cout << "__ electrons + muons __" << endl;
  cout << "Overall closure H-H (100\% best): " << 100*overall[0] << "%" << endl;
  cout << "Overall closure H-L (100\% best): " << 100*overall[1] << "%" << endl;
  cout << "Overall closure L-L (100\% best): " << 100*overall[2] << "%" << endl;
  cout << endl;

}
