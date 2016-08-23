#include "../software/dataMCplotMaker/PlotMaker2D.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../commonUtils.h"
#include "../classFiles/v6.02/SS.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TChain.h"
#include <fstream>
#include "../CORE/IsolationTools.h"
#include "../CORE/SSSelections.h"
#include "../CORE/Tools/dorky/dorky.h"

//do the data!!
bool doData = false; 

//testPC -- test the prompt contamination, ie allow numer-numer events
bool testPC = false;

//Include DY and W+Jets
bool others = false;

//SS Z-veto
bool ssZveto = true;

//Path
string tag = getTag().Data();

//Lumi
float luminosity = doData ? 20.0 : getLumi();

// bool passesMVA(int which, int level){
//   float eta1[3]={0.87, 0.05,-0.36}; 
//   float eta2[3]={0.60,-0.26,-0.58}; 
//   float eta3[3]={0.17,-0.40,-0.62}; 
//   if (abs(ss::lep1_id()) != 11) return true;
//   float aeta = (which == 1) ? fabs(ss::lep1_p4().eta()) : fabs(ss::lep2_p4().eta());//uhm this should be SCeta
//   float disc = (which == 1) ? ss::lep1_MVA() : ss::lep2_MVA();
//   if (aeta < 0.8) return disc > eta1[level];
//   if ((aeta >= 0.8 && aeta <= 1.479)) return disc > eta2[level];
//   if (aeta > 1.479) return disc > eta3[level];
//   return false;
// }

bool passesMVA(int whichlep, int level){
  if (whichlep == 1 && abs(ss::lep1_id()) != 11) return true;
  if (whichlep == 2 && abs(ss::lep1_id()) != 11) return true;
  // FIXME - remake babies to have SCeta
  float aeta = (whichlep == 1) ? fabs(ss::lep1_p4().eta()) : fabs(ss::lep2_p4().eta());//uhm this should be SCeta
  float disc = (whichlep == 1) ? ss::lep1_MVA() : ss::lep2_MVA();
  if (level == 1) {
      if (aeta < 0.8) return disc > -0.155;
      if ((aeta >= 0.8 && aeta <= 1.479)) return disc > -0.56;
      if (aeta > 1.479) return disc > -0.76;
  } else if (level == 2) {
      if (aeta < 0.8) return disc > 0.87;
      if ((aeta >= 0.8 && aeta <= 1.479)) return disc > 0.60;
      if (aeta > 1.479) return disc > 0.17;
  }
  return false;
}

bool isFakeLeg(int lep){
  if (doData) return 1; 
  if (lep == 1) return (ss::lep1_motherID() <= 0); 
  if (lep == 2) return (ss::lep2_motherID() <= 0); 
  return 0;
}

bool isGoodLeg(int lep){
  if (doData) return 1; 
  if (lep == 1) return (ss::lep1_motherID() > 0); 
  if (lep == 2) return (ss::lep2_motherID() > 0); 
  return 0;
}

void FR(int doHighHT=-1){

  std::cout << "Using lumi = " << luminosity << "/fb" << std::endl;
  std::cout << "Using " << tag << " babies" << std::endl;

  int counter = 0;

  //Declare hists
  int nBinsX = 20;
  TH2D *numer_elec_2D_sipgt4 ; 
  TH2D *denom_elec_2D_sipgt4 ; 
  TH2D *numer_elec_2D_siplt4; 
  TH2D *denom_elec_2D_siplt4; 
  TH2D *numer_muon_2D_sipgt4; 
  TH2D *denom_muon_2D_sipgt4; 
  TH2D *numer_muon_2D_siplt4; 
  TH2D *denom_muon_2D_siplt4; 

  //Reproduce Lesya's plot of 1/ptratio vs. miniiso 
  TH2D *test; 

  //Define hists
  nBinsX = 5; 
  // int nBinsY = 1; 
  int nBinsY = 3; 
  float xbins[] = { 10, 15, 25, 35, 50, 70 }; 
  // //float ybins[] = { 0, 0.8, 1.479, 2.5 }; 
  // float ybins[] = { 0, 2.4 }; 
  float ybins_mu[] = {0., 1.2, 2.1, 2.4};
  float ybins_el[] = {0., 0.8, 1.479, 2.5};

  numer_elec_2D_sipgt4  = new TH2D("numer_elec_2D_sipgt4" , "numer_elec_2D_sipgt4" , nBinsX, xbins, nBinsY, ybins_el);
  denom_elec_2D_sipgt4  = new TH2D("denom_elec_2D_sipgt4" , "denom_elec_2D_sipgt4" , nBinsX, xbins, nBinsY, ybins_el);
  numer_elec_2D_siplt4 = new TH2D("numer_elec_2D_siplt4", "numer_elec_2D_siplt4", nBinsX, xbins, nBinsY, ybins_el);
  denom_elec_2D_siplt4 = new TH2D("denom_elec_2D_siplt4", "denom_elec_2D_siplt4", nBinsX, xbins, nBinsY, ybins_el);
  numer_muon_2D_sipgt4 = new TH2D("numer_muon_2D_sipgt4", "numer_muon_2D_sipgt4", nBinsX, xbins, nBinsY, ybins_mu);
  denom_muon_2D_sipgt4 = new TH2D("denom_muon_2D_sipgt4", "denom_muon_2D_sipgt4", nBinsX, xbins, nBinsY, ybins_mu);
  numer_muon_2D_siplt4 = new TH2D("numer_muon_2D_siplt4", "numer_muon_2D_siplt4", nBinsX, xbins, nBinsY, ybins_mu);
  denom_muon_2D_siplt4 = new TH2D("denom_muon_2D_siplt4", "denom_muon_2D_siplt4", nBinsX, xbins, nBinsY, ybins_mu);
  test = new TH2D("test", "test", 40, 0, 0.4, 20, 1.0, 3.0); 

  TH1F *numer_vs_sip_el  = new TH1F("numer_vs_sip_el" , "numer_vs_sip_el" , 16,0,8);
  TH1F *denom_vs_sip_el  = new TH1F("denom_vs_sip_el" , "denom_vs_sip_el" , 16,0,8);
  TH1F *numer_vs_sip_mu  = new TH1F("numer_vs_sip_mu" , "numer_vs_sip_mu" , 16,0,8);
  TH1F *denom_vs_sip_mu  = new TH1F("denom_vs_sip_mu" , "denom_vs_sip_mu" , 16,0,8);

  TH2F *miniiso_vs_sip_el  = new TH2F("miniiso_vs_sip_el" , "miniiso_vs_sip_el" , 16,0,8, 20,0,0.5);
  TH2F *ptrel_vs_sip_el  = new TH2F("ptrel_vs_sip_el" , "ptrel_vs_sip_el" , 16,0,8, 30,0,30);
  TH2F *ptratio_vs_sip_el  = new TH2F("ptratio_vs_sip_el" , "ptratio_vs_sip_el" , 16,0,8, 20,0.6,1.2);
  TH2F *multiiso_vs_sip_el  = new TH2F("multiiso_vs_sip_el" , "multiiso_vs_sip_el" , 16,0,8, 2,0,2);

  TH2F *miniiso_vs_sip_mu  = new TH2F("miniiso_vs_sip_mu" , "miniiso_vs_sip_mu" , 16,0,8, 20,0,0.5);
  TH2F *ptrel_vs_sip_mu  = new TH2F("ptrel_vs_sip_mu" , "ptrel_vs_sip_mu" , 16,0,8, 30,0,30);
  TH2F *ptratio_vs_sip_mu  = new TH2F("ptratio_vs_sip_mu" , "ptratio_vs_sip_mu" , 16,0,8, 20,0.6,1.2);
  TH2F *multiiso_vs_sip_mu  = new TH2F("multiiso_vs_sip_mu" , "multiiso_vs_sip_mu" , 16,0,8, 2,0,2);

  //Set hist errors
  numer_elec_2D_sipgt4->Sumw2();
  denom_elec_2D_sipgt4->Sumw2();
  numer_elec_2D_siplt4->Sumw2();
  denom_elec_2D_siplt4->Sumw2();
  numer_muon_2D_sipgt4->Sumw2();
  denom_muon_2D_sipgt4->Sumw2();
  numer_muon_2D_siplt4->Sumw2();
  denom_muon_2D_siplt4->Sumw2();

  //Declare chain
  TChain *chain = new TChain("t");
  int fo2_suffix = 0;
  if (doData){
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/Data*.root"  , tag.c_str()));
  }
  else if (!doData){
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR_PH.root", tag.c_str()));
    if (others){
      chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_*.root", tag.c_str()));
      chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets.root", tag.c_str()));
    }
  }

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

      //Data duplicate removal
      if (doData){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)) continue; 
      }

      //Pt
      float pt1 = ss::lep1_coneCorrPt(); 
      float pt2 = ss::lep2_coneCorrPt(); 

      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //if (ss::run() > 258750) continue;

      //Event Selection
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), pt1, pt2);
      if (BR < 0) continue;

      if (ss::hyp_class() == 4 || ss::hyp_class() == 6) continue;

      //Non-Isolated trigger requirements
      if (ss::is_real_data() && !ss::fired_trigger()) continue; 
      if (ss::ht() < 300){
          if (!passesMVA(1,1) || !passesMVA(2,1)) continue;
      }

      // if doHighHT is -1 (default), then don't do anything -- inclusive
      if(doHighHT == 0 && ss::ht() > 300.) continue;
      if(doHighHT == 1 && ss::ht() < 300.) continue;

      //SS Z veto
      if (ssZveto && fabs((ss::lep1_p4() + ss::lep2_p4()).M() - 91) < 15) continue;

      //Various variables
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

      float weight = ss::scale1fb()*luminosity;
      bool lep1_prompt = (testPC || isGoodLeg(1)) && (isFakeLeg(2) || testPC);
      bool lep2_prompt = (testPC || isGoodLeg(2)) && (isFakeLeg(1) || testPC);
      bool lep1_elec = abs(ss::lep1_id()) == 11;
      bool lep2_elec = abs(ss::lep2_id()) == 11;

      // // speed up by factor of 10
      // if(event%10 != 0) continue;
      // weight *= 10;

      //histo1 is for electrons with SIP > 4 
      if (lep1_elec && lep2_prompt && ss::lep1_sip() > 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (passesMVA(1,2) && lep1_multiIso) {
            numer_elec_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_el->Fill(ss::lep1_sip(), weight);
        }
        denom_elec_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_el->Fill(ss::lep1_sip(), weight);

        miniiso_vs_sip_el->Fill(ss::lep1_sip(), miniiso_1, weight);
        ptrel_vs_sip_el->Fill(ss::lep1_sip(), ptrel_1, weight);
        ptratio_vs_sip_el->Fill(ss::lep1_sip(), ptratio_1, weight);
        multiiso_vs_sip_el->Fill(ss::lep1_sip(), lep1_multiIso, weight);

        counter++; 
      }
      if (lep2_elec && lep1_prompt && ss::lep2_sip() > 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (passesMVA(2,2) && lep2_multiIso) {
            numer_elec_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_el->Fill(ss::lep2_sip(), weight);
        }
        denom_elec_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_el->Fill(ss::lep2_sip(), weight);

        miniiso_vs_sip_el->Fill(ss::lep2_sip(), miniiso_2, weight);
        ptrel_vs_sip_el->Fill(ss::lep2_sip(), ptrel_2, weight);
        ptratio_vs_sip_el->Fill(ss::lep2_sip(), ptratio_2, weight);
        multiiso_vs_sip_el->Fill(ss::lep2_sip(), lep2_multiIso, weight);

      }

      //histo2 is for electrons with SIP < 4 
      if (lep1_elec && lep2_prompt && ss::lep1_sip() < 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (lep1_multiIso) {
            numer_elec_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_el->Fill(ss::lep1_sip(), weight);
        }
        denom_elec_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_el->Fill(ss::lep1_sip(), weight);

        miniiso_vs_sip_el->Fill(ss::lep1_sip(), miniiso_1, weight);
        ptrel_vs_sip_el->Fill(ss::lep1_sip(), ptrel_1, weight);
        ptratio_vs_sip_el->Fill(ss::lep1_sip(), ptratio_1, weight);
        multiiso_vs_sip_el->Fill(ss::lep1_sip(), lep1_multiIso, weight);

      }
      if (lep2_elec && lep1_prompt && ss::lep2_sip() < 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (lep2_multiIso) {
            numer_elec_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_el->Fill(ss::lep2_sip(), weight);
        }
        denom_elec_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_el->Fill(ss::lep2_sip(), weight);

        miniiso_vs_sip_el->Fill(ss::lep2_sip(), miniiso_2, weight);
        ptrel_vs_sip_el->Fill(ss::lep2_sip(), ptrel_2, weight);
        ptratio_vs_sip_el->Fill(ss::lep2_sip(), ptratio_2, weight);
        multiiso_vs_sip_el->Fill(ss::lep2_sip(), lep2_multiIso, weight);

      }

      //histo3 is for muons with SIP > 4 
      if (!lep1_elec && ss::lep1_sip() > 4 && lep2_prompt && ss::lep2_passes_id() && lep1_denom_iso){
        if (passesMVA(1,2) && lep1_multiIso) {
            numer_muon_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(ss::lep1_sip(), weight);
        }
        if (ss::lep1_p4().pt() >= 25) test->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_muon_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_mu->Fill(ss::lep1_sip(), weight);

        miniiso_vs_sip_mu->Fill(ss::lep1_sip(), miniiso_1, weight);
        ptrel_vs_sip_mu->Fill(ss::lep1_sip(), ptrel_1, weight);
        ptratio_vs_sip_mu->Fill(ss::lep1_sip(), ptratio_1, weight);
        multiiso_vs_sip_mu->Fill(ss::lep1_sip(), lep1_multiIso, weight);

        counter++; 
      }
      if (!lep2_elec && ss::lep2_sip() > 4 && lep1_prompt && ss::lep1_passes_id() && lep2_denom_iso){
        if (passesMVA(2,2) && lep2_multiIso) {
            numer_muon_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(ss::lep2_sip(), weight);
        }
        if (ss::lep2_p4().pt() >= 25) test->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_muon_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_mu->Fill(ss::lep2_sip(), weight);

        miniiso_vs_sip_mu->Fill(ss::lep2_sip(), miniiso_2, weight);
        ptrel_vs_sip_mu->Fill(ss::lep2_sip(), ptrel_2, weight);
        ptratio_vs_sip_mu->Fill(ss::lep2_sip(), ptratio_2, weight);
        multiiso_vs_sip_mu->Fill(ss::lep2_sip(), lep2_multiIso, weight);

      }

      //histo4 is for muons with SIP < 4 
      if (!lep1_elec && lep2_prompt && ss::lep1_sip() < 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (lep1_multiIso) {
            numer_muon_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(ss::lep1_sip(), weight);
        }
        denom_muon_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_mu->Fill(ss::lep1_sip(), weight);

        miniiso_vs_sip_mu->Fill(ss::lep1_sip(), miniiso_1, weight);
        ptrel_vs_sip_mu->Fill(ss::lep1_sip(), ptrel_1, weight);
        ptratio_vs_sip_mu->Fill(ss::lep1_sip(), ptratio_1, weight);
        multiiso_vs_sip_mu->Fill(ss::lep1_sip(), lep1_multiIso, weight);

      }
      if (!lep2_elec && lep1_prompt && ss::lep2_sip() < 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (lep2_multiIso) {
            numer_muon_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(ss::lep2_sip(), weight);
        }
        denom_muon_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_mu->Fill(ss::lep2_sip(), weight);
        
        miniiso_vs_sip_mu->Fill(ss::lep2_sip(), miniiso_2, weight);
        ptrel_vs_sip_mu->Fill(ss::lep2_sip(), ptrel_2, weight);
        ptratio_vs_sip_mu->Fill(ss::lep2_sip(), ptratio_2, weight);
        multiiso_vs_sip_mu->Fill(ss::lep2_sip(), lep2_multiIso, weight);

      }

    }//event loop
  }//file loop

  //Overflow
  for (unsigned int i = 0; i < 3; i++){
    denom_elec_2D_sipgt4 ->SetBinContent(nBinsX, i, denom_elec_2D_sipgt4->GetBinContent(nBinsX , i) + denom_elec_2D_sipgt4->GetBinContent(nBinsX+1 , i)); 
    numer_elec_2D_sipgt4 ->SetBinContent(nBinsX, i, numer_elec_2D_sipgt4->GetBinContent(nBinsX , i) + numer_elec_2D_sipgt4->GetBinContent(nBinsX+1 , i)); 
    denom_elec_2D_siplt4->SetBinContent(nBinsX, i, denom_elec_2D_siplt4->GetBinContent(nBinsX, i) + denom_elec_2D_siplt4->GetBinContent(nBinsX+1, i)); 
    numer_elec_2D_siplt4->SetBinContent(nBinsX, i, numer_elec_2D_siplt4->GetBinContent(nBinsX, i) + numer_elec_2D_siplt4->GetBinContent(nBinsX+1, i)); 
    denom_muon_2D_sipgt4->SetBinContent(nBinsX, i, denom_muon_2D_sipgt4->GetBinContent(nBinsX, i) + denom_muon_2D_sipgt4->GetBinContent(nBinsX+1, i)); 
    numer_muon_2D_sipgt4->SetBinContent(nBinsX, i, numer_muon_2D_sipgt4->GetBinContent(nBinsX, i) + numer_muon_2D_sipgt4->GetBinContent(nBinsX+1, i)); 
    denom_muon_2D_siplt4->SetBinContent(nBinsX, i, denom_muon_2D_siplt4->GetBinContent(nBinsX, i) + denom_muon_2D_siplt4->GetBinContent(nBinsX+1, i)); 
    numer_muon_2D_siplt4->SetBinContent(nBinsX, i, numer_muon_2D_siplt4->GetBinContent(nBinsX, i) + numer_muon_2D_siplt4->GetBinContent(nBinsX+1, i)); 
  }

  //Divide numer/denom
  numer_elec_2D_sipgt4 ->Divide(numer_elec_2D_sipgt4 , denom_elec_2D_sipgt4 , 1, 1, "b"); 
  numer_elec_2D_siplt4->Divide(numer_elec_2D_siplt4, denom_elec_2D_siplt4, 1, 1, "b"); 
  numer_muon_2D_sipgt4->Divide(numer_muon_2D_sipgt4, denom_muon_2D_sipgt4, 1, 1, "b"); 
  numer_muon_2D_siplt4->Divide(numer_muon_2D_siplt4, denom_muon_2D_siplt4, 1, 1, "b"); 

  // //Save output
  // string name2 = "blah";
  // if (!doData && !others  && !testPC && !ssZveto) name2 = "normal"; 
  // if (!doData &&  others  &&  testPC && !ssZveto) name2 = "PC"; 
  // if (!doData &&  others  && !testPC && !ssZveto) name2 = "soup";
  // if (!doData && !others  && !testPC &&  ssZveto) name2 = "ssZ"; 
  // if (!doData &&  others  &&  testPC &&  ssZveto) name2 = "PCssZ"; 
  // if ( doData && !others  && !testPC && !ssZveto) name2 = "data_normal"; 
  // if ( doData &&  others  &&  testPC && !ssZveto) name2 = "data_PC"; 
  // if ( doData &&  others  && !testPC && !ssZveto) name2 = "data_soup";
  // if ( doData && !others  && !testPC &&  ssZveto) name2 = "data_ssZ"; 
  // if ( doData &&  others  &&  testPC &&  ssZveto) name2 = "data_PCssZ"; 
  string name2 = "all";
  if(doHighHT == 0) name2 = "lowht";
  if(doHighHT == 1) name2 = "highht";

  TFile *file = new TFile(Form("%s_inSituFR_cone_FR_histos.root", name2.c_str()), "RECREATE");
  file->Write(); 
  numer_elec_2D_sipgt4->Write("elec");
  numer_muon_2D_sipgt4->Write("muon");
  file->Close(); 

  //Plots

  // FR sip3D > 4
  PlotMaker2D(numer_elec_2D_sipgt4 , Form("--outputName plots/%s_FR_elec.pdf --noOverflow --setTitle elec %s (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", name2.c_str(), name2.c_str())); 
  PlotMaker2D(numer_muon_2D_sipgt4, Form("--outputName plots/%s_FR_muon.pdf --noOverflow --setTitle muon %s (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", name2.c_str(), name2.c_str())); 

  // FR sip3D < 4
  PlotMaker2D(numer_elec_2D_siplt4, Form("--outputName plots/%s_FR_elec_siplt4.pdf --noOverflow --setTitle elec %s (sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", name2.c_str(), name2.c_str())); 
  PlotMaker2D(numer_muon_2D_siplt4, Form("--outputName plots/%s_FR_muon_siplt4.pdf --noOverflow --setTitle muon %s (sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", name2.c_str(), name2.c_str())); 

  // FR(sip3D<4)/FR(sip3D>4)
  numer_elec_2D_siplt4->Divide(numer_elec_2D_sipgt4 );
  numer_muon_2D_siplt4->Divide(numer_muon_2D_sipgt4);
  PlotMaker2D(numer_elec_2D_siplt4, Form("--outputName plots/%s_FR_elec_sipratio.pdf --noOverflow --setTitle elec FR(sip3D < 4)/FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", name2.c_str())); 
  PlotMaker2D(numer_muon_2D_siplt4, Form("--outputName plots/%s_FR_muon_sipratio.pdf --noOverflow --setTitle muon FR(sip3D < 4)/FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", name2.c_str())); 

  // Isolation vs vs sip3D
  PlotMaker2D(miniiso_vs_sip_el, Form("--outputName plots/%s_miniiso_vs_sip_el.pdf --noOverflow --setTitle miniiso vs sip (elec) --Xaxis sip3D --Yaxis miniiso --colors", name2.c_str()));
  PlotMaker2D(ptrel_vs_sip_el, Form("--outputName plots/%s_ptrel_vs_sip_el.pdf --noOverflow --setTitle ptrel vs sip (elec) --Xaxis sip3D --Yaxis ptrel --colors", name2.c_str()));
  PlotMaker2D(ptratio_vs_sip_el, Form("--outputName plots/%s_ptratio_vs_sip_el.pdf --noOverflow --setTitle ptratio vs sip (elec) --Xaxis sip3D --Yaxis ptratio --colors", name2.c_str()));
  PlotMaker2D(multiiso_vs_sip_el, Form("--outputName plots/%s_multiiso_vs_sip_el.pdf --noOverflow --setTitle multiiso vs sip (elec) --Xaxis sip3D  --Yaxis multiiso --colors", name2.c_str()));
  PlotMaker2D(miniiso_vs_sip_mu, Form("--outputName plots/%s_miniiso_vs_sip_mu.pdf --noOverflow --setTitle miniiso vs sip (muon) --Xaxis sip3D --Yaxis miniiso --colors", name2.c_str()));
  PlotMaker2D(ptrel_vs_sip_mu, Form("--outputName plots/%s_ptrel_vs_sip_mu.pdf --noOverflow --setTitle ptrel vs sip (muon) --Xaxis sip3D --Yaxis ptrel --colors", name2.c_str()));
  PlotMaker2D(ptratio_vs_sip_mu, Form("--outputName plots/%s_ptratio_vs_sip_mu.pdf --noOverflow --setTitle ptratio vs sip (muon) --Xaxis sip3D --Yaxis ptratio --colors", name2.c_str()));
  PlotMaker2D(multiiso_vs_sip_mu, Form("--outputName plots/%s_multiiso_vs_sip_mu.pdf --noOverflow --setTitle multiiso vs sip (muon) --Xaxis sip3D  --Yaxis multiiso --colors", name2.c_str()));

  PlotMaker2D(test, "--outputName plots/test.pdf --noOverflow --setTitle muons, pT > 25, SIP3D > 4 --Xaxis miniisolation --Yaxis 1/p_{T}^{ratio} --colors"); 

  // numer_vs_sip_el->Divide(numer_vs_sip_el, denom_vs_sip_el, 1, 1, "b");
  // numer_vs_sip_mu->Divide(numer_vs_sip_mu, denom_vs_sip_mu, 1, 1, "b");

  TCanvas *c1 = new TCanvas("c1","c1");
  // numer_vs_sip_el->Draw("pe");
  denom_vs_sip_el->Draw("pe");
  c1->SaveAs(Form("plots/%s_FR_vs_sip3d_el.pdf", name2.c_str()));
  // numer_vs_sip_mu->Draw("pe");
  denom_vs_sip_mu->Draw("pe");
  c1->SaveAs(Form("plots/%s_FR_vs_sip3d_mu.pdf", name2.c_str()));
  // singlePlotMaker(numer_vs_sip_el, "FR vs sip3D (el)", "--outputName plots/FR_vs_sip3d_el.pdf --noOverflow");
  // singlePlotMaker(numer_vs_sip_mu, "FR vs sip3D (mu)", "--outputName plots/FR_vs_sip3d_mu.pdf --noOverflow");
}
