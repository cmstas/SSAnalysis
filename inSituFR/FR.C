#include "../software/dataMCplotMaker/PlotMaker2D.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../commonUtils.h"
#include "../classFiles/v8.04/SS.h"
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

TH1D * evtCounter = new TH1D("","",1000,0,1000); 
std::vector<TString> counterNames;
map<TString, int> evtBinMap;
int evtBin = 0;
void initCounter() {
    evtCounter = new TH1D("","",1000,0,1000); 
    evtCounter->Sumw2();
    evtBinMap.clear();
}
void addToCounter(TString name, double weight=1.0) {
    if(evtBinMap.find(name) == evtBinMap.end() ) {
        evtBinMap[name] = evtBin;
        counterNames.push_back(name);
        evtBin++;
    }
    evtCounter->Fill(evtBinMap[name], weight);
}
void printCounter(bool file = false) {
    cout << string(30, '-') << endl << "Counter totals: " << endl;
    for(int iBin = 0; iBin < evtBin; iBin++) {
        printf("%-25s %7.2f %7.2f\n",
                counterNames.at(iBin).Data(),
                evtCounter->GetBinContent(iBin+1),
                evtCounter->GetBinError(iBin+1) );
    }
    cout << string(30, '-') << endl;
}

bool passesMVA(int whichlep, int level){
    if (whichlep == 1 && abs(ss::lep1_id()) != 11) return true;
    if (whichlep == 2 && abs(ss::lep2_id()) != 11) return true;
    float aeta = (whichlep == 1) ? fabs(ss::lep1_el_etaSC()) : fabs(ss::lep2_el_etaSC());
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

    initCounter();

  std::cout << "Using lumi = " << luminosity << "/fb" << std::endl;
  std::cout << "Using " << tag << " babies" << std::endl;

  //Declare hists
  TH2D *numer_elec_2D_sipgt4 ; 
  TH2D *denom_elec_2D_sipgt4 ; 
  TH2D *numer_elec_2D_siplt4; 
  TH2D *denom_elec_2D_siplt4; 
  TH2D *numer_muon_2D_sipgt4; 
  TH2D *denom_muon_2D_sipgt4; 
  TH2D *numer_muon_2D_siplt4; 
  TH2D *denom_muon_2D_siplt4; 

  //Define hists
  int nBinsX = 5; 
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

  TH2F *ptratio_vs_miniiso_mu  = new TH2F("ptratio_vs_miniiso_mu" , "ptratio_vs_miniiso_mu" , 40,0,0.4,20,1.0,3.0);
  TH2F *ptratio_vs_miniiso_el  = new TH2F("ptratio_vs_miniiso_el" , "ptratio_vs_miniiso_el" , 40,0,0.4,20,1.0,3.0);

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
    // chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR_PH.root", tag.c_str()));
    chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04_trigsafe/TTBAR_PH*.root");
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

      float weight = ss::scale1fb()*luminosity;

      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //if (ss::run() > 258750) continue;

      //Event Selection
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), pt1, pt2);
      if (BR < 0) continue;

      if (ss::hyp_class() == 4 || ss::hyp_class() == 6) continue;

      // If either one of them fails the inSituFR id, then bye bye. If we aren't hyp_class=3, then this forces us to be 5
      if (!ss::passed_id_inSituFR_lep1() || !ss::passed_id_inSituFR_lep2()) continue;
      if (!ss::lep1_isTrigSafeNoIsov1() || !ss::lep2_isTrigSafeNoIsov1()) continue;

      //Non-Isolated trigger requirements
      if (ss::is_real_data() && !ss::fired_trigger()) continue; 
      if (ss::ht()<300.) {
          if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA())) continue;
          if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA())) continue;
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

      float miniiso_1 = ss::lep1_miniIso();
      float miniiso_2 = ss::lep2_miniIso();
      float ptrel_1   = ss::lep1_ptrel_v1();
      float ptrel_2   = ss::lep2_ptrel_v1();
      float ptratio_1 = ss::lep1_closeJet().pt() > 0 ? ss::lep1_p4().pt()/ss::lep1_closeJet().pt() : 1.0; 
      float ptratio_2 = ss::lep2_closeJet().pt() > 0 ? ss::lep2_p4().pt()/ss::lep2_closeJet().pt() : 1.0; 

      // The if statements are getting kind of long now, so shorten some things
      bool lep1_prompt = (testPC || isGoodLeg(1)) && (isFakeLeg(2) || testPC);
      bool lep2_prompt = (testPC || isGoodLeg(2)) && (isFakeLeg(1) || testPC);
      bool lep1_elec = abs(ss::lep1_id()) == 11;
      bool lep2_elec = abs(ss::lep2_id()) == 11;
      float lep1_sip = ss::lep1_sip();
      float lep2_sip = ss::lep2_sip();
      bool lep1_passes_id = ss::lep1_passes_id();
      bool lep2_passes_id = ss::lep2_passes_id();
      bool lep1_sip_good = lep1_sip < 4; // FIXME make sure inequality is correct when doing this for real
      bool lep2_sip_good = lep2_sip < 4;


      bool denom1 = lep2_prompt && !lep1_sip_good && lep2_passes_id && lep1_denom_iso;
      bool denom2 = lep1_prompt && !lep2_sip_good && lep1_passes_id && lep2_denom_iso;

      bool numer1 = passesMVA(1,2) && ss::lep1_multiIso() && (fabs(ss::lep1_dxyPV()) < 0.05);
      bool numer2 = passesMVA(2,2) && ss::lep2_multiIso() && (fabs(ss::lep2_dxyPV()) < 0.05);


      //histo1 is for electrons with SIP > 4
      if (lep1_elec && denom1){
          if(pt1>50 && abs(ss::lep1_p4().eta()) > 1.479) addToCounter("denom pt>50");
        if (numer1) {
          if(pt1>50 && abs(ss::lep1_p4().eta()) > 1.479) addToCounter("numer pt>50");
          if(pt1>50 && abs(ss::lep1_p4().eta()) > 1.479 && ss::hyp_class() == 3) addToCounter("numer pt>50 && class3");
            numer_elec_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_el->Fill(lep1_sip, weight);
        }
        ptratio_vs_miniiso_el->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_elec_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_el->Fill(lep1_sip, weight);

        miniiso_vs_sip_el->Fill(lep1_sip, miniiso_1, weight);
        ptrel_vs_sip_el->Fill(lep1_sip, ptrel_1, weight);
        ptratio_vs_sip_el->Fill(lep1_sip, ptratio_1, weight);
        multiiso_vs_sip_el->Fill(lep1_sip, ss::lep1_multiIso(), weight);

      }
      if (lep2_elec && denom2){
        if (numer2) {
            numer_elec_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_el->Fill(lep2_sip, weight);
        }
        ptratio_vs_miniiso_el->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_elec_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_el->Fill(lep2_sip, weight);

        miniiso_vs_sip_el->Fill(lep2_sip, miniiso_2, weight);
        ptrel_vs_sip_el->Fill(lep2_sip, ptrel_2, weight);
        ptratio_vs_sip_el->Fill(lep2_sip, ptratio_2, weight);
        multiiso_vs_sip_el->Fill(lep2_sip, ss::lep2_multiIso(), weight);

      }

      //histo2 is for electrons with SIP < 4
      if (lep1_elec && denom1){
        if (numer1) {
            numer_elec_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_el->Fill(lep1_sip, weight);
        }
        ptratio_vs_miniiso_el->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_elec_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_el->Fill(lep1_sip, weight);

        miniiso_vs_sip_el->Fill(lep1_sip, miniiso_1, weight);
        ptrel_vs_sip_el->Fill(lep1_sip, ptrel_1, weight);
        ptratio_vs_sip_el->Fill(lep1_sip, ptratio_1, weight);
        multiiso_vs_sip_el->Fill(lep1_sip, ss::lep1_multiIso(), weight);

      }
      if (lep2_elec && denom2){
        if (numer2) {
            numer_elec_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_el->Fill(lep2_sip, weight);
        }
        ptratio_vs_miniiso_el->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_elec_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_el->Fill(lep2_sip, weight);

        miniiso_vs_sip_el->Fill(lep2_sip, miniiso_2, weight);
        ptrel_vs_sip_el->Fill(lep2_sip, ptrel_2, weight);
        ptratio_vs_sip_el->Fill(lep2_sip, ptratio_2, weight);
        multiiso_vs_sip_el->Fill(lep2_sip, ss::lep2_multiIso(), weight);

      }

      //histo3 is for muons with SIP > 4 
      if (!lep1_elec && denom1){
        if (numer1) {
            numer_muon_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(lep1_sip, weight);
        }
        ptratio_vs_miniiso_mu->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_muon_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_mu->Fill(lep1_sip, weight);

        miniiso_vs_sip_mu->Fill(lep1_sip, miniiso_1, weight);
        ptrel_vs_sip_mu->Fill(lep1_sip, ptrel_1, weight);
        ptratio_vs_sip_mu->Fill(lep1_sip, ptratio_1, weight);
        multiiso_vs_sip_mu->Fill(lep1_sip, ss::lep1_multiIso(), weight);

      }
      if (!lep2_elec && denom2){
        if (numer2) {
            numer_muon_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(lep2_sip, weight);
        }
        ptratio_vs_miniiso_mu->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_muon_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_mu->Fill(lep2_sip, weight);

        miniiso_vs_sip_mu->Fill(lep2_sip, miniiso_2, weight);
        ptrel_vs_sip_mu->Fill(lep2_sip, ptrel_2, weight);
        ptratio_vs_sip_mu->Fill(lep2_sip, ptratio_2, weight);
        multiiso_vs_sip_mu->Fill(lep2_sip, ss::lep2_multiIso(), weight);

      }

      //histo4 is for muons with SIP < 4 
      if (!lep1_elec && denom1){
        if (numer1) {
            numer_muon_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(lep1_sip, weight);
        }
        ptratio_vs_miniiso_mu->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_muon_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_mu->Fill(lep1_sip, weight);

        miniiso_vs_sip_mu->Fill(lep1_sip, miniiso_1, weight);
        ptrel_vs_sip_mu->Fill(lep1_sip, ptrel_1, weight);
        ptratio_vs_sip_mu->Fill(lep1_sip, ptratio_1, weight);
        multiiso_vs_sip_mu->Fill(lep1_sip, ss::lep1_multiIso(), weight);

      }
      if (!lep2_elec && denom2){
        if (numer2) {
            numer_muon_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(lep2_sip, weight);
        }
        ptratio_vs_miniiso_mu->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_muon_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_mu->Fill(lep2_sip, weight);
        
        miniiso_vs_sip_mu->Fill(lep2_sip, miniiso_2, weight);
        ptrel_vs_sip_mu->Fill(lep2_sip, ptrel_2, weight);
        ptratio_vs_sip_mu->Fill(lep2_sip, ptratio_2, weight);
        multiiso_vs_sip_mu->Fill(lep2_sip, ss::lep2_multiIso(), weight);

      }

    }//event loop
  }//file loop

  //Overflow
  for (unsigned int i = 1; i < 4; i++){ // <--- GRRR. ROOT starts bin #s at 1 (0 is underflow)

    denom_elec_2D_sipgt4 ->SetBinContent(nBinsX, i, denom_elec_2D_sipgt4->GetBinContent(nBinsX , i) + denom_elec_2D_sipgt4->GetBinContent(nBinsX+1 , i)); 
    numer_elec_2D_sipgt4 ->SetBinContent(nBinsX, i, numer_elec_2D_sipgt4->GetBinContent(nBinsX , i) + numer_elec_2D_sipgt4->GetBinContent(nBinsX+1 , i)); 
    denom_elec_2D_siplt4->SetBinContent(nBinsX, i, denom_elec_2D_siplt4->GetBinContent(nBinsX, i) + denom_elec_2D_siplt4->GetBinContent(nBinsX+1, i)); 
    numer_elec_2D_siplt4->SetBinContent(nBinsX, i, numer_elec_2D_siplt4->GetBinContent(nBinsX, i) + numer_elec_2D_siplt4->GetBinContent(nBinsX+1, i)); 

    denom_muon_2D_sipgt4->SetBinContent(nBinsX, i, denom_muon_2D_sipgt4->GetBinContent(nBinsX, i) + denom_muon_2D_sipgt4->GetBinContent(nBinsX+1, i)); 
    numer_muon_2D_sipgt4->SetBinContent(nBinsX, i, numer_muon_2D_sipgt4->GetBinContent(nBinsX, i) + numer_muon_2D_sipgt4->GetBinContent(nBinsX+1, i)); 
    denom_muon_2D_siplt4->SetBinContent(nBinsX, i, denom_muon_2D_siplt4->GetBinContent(nBinsX, i) + denom_muon_2D_siplt4->GetBinContent(nBinsX+1, i)); 
    numer_muon_2D_siplt4->SetBinContent(nBinsX, i, numer_muon_2D_siplt4->GetBinContent(nBinsX, i) + numer_muon_2D_siplt4->GetBinContent(nBinsX+1, i)); 
  }

  string pfx = "all";
  string desc = "All H_{T}";
  if(doHighHT == 0) { pfx = "lowht"; desc = "Low H_{T}"; }
  if(doHighHT == 1) { pfx = "highht"; desc = "High H_{T}"; }

  // plot numer and denom before dividing
  PlotMaker2D(numer_elec_2D_sipgt4 , Form("--outputName plots/%s_numer_FR_elec.pdf --noOverflow --setTitle elec %s numer (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(numer_muon_2D_sipgt4, Form("--outputName plots/%s_numer_FR_muon.pdf --noOverflow --setTitle muon %s numer (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(denom_elec_2D_sipgt4 , Form("--outputName plots/%s_denom_FR_elec.pdf --noOverflow --setTitle elec %s denom (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(denom_muon_2D_sipgt4, Form("--outputName plots/%s_denom_FR_muon.pdf --noOverflow --setTitle muon %s denom (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 

  //Divide numer/denom
  numer_elec_2D_sipgt4 ->Divide(numer_elec_2D_sipgt4 , denom_elec_2D_sipgt4 , 1, 1, "b"); 
  numer_elec_2D_siplt4->Divide(numer_elec_2D_siplt4, denom_elec_2D_siplt4, 1, 1, "b"); 
  numer_muon_2D_sipgt4->Divide(numer_muon_2D_sipgt4, denom_muon_2D_sipgt4, 1, 1, "b"); 
  numer_muon_2D_siplt4->Divide(numer_muon_2D_siplt4, denom_muon_2D_siplt4, 1, 1, "b"); 

  TFile *file = new TFile(Form("%s_inSituFR_cone_FR_histos.root", pfx.c_str()), "RECREATE");
  file->Write(); 
  numer_elec_2D_sipgt4->Write("elec");
  numer_muon_2D_sipgt4->Write("muon");
  file->Close(); 

  //Plots

  // FR sip3D > 4
  PlotMaker2D(numer_elec_2D_sipgt4 , Form("--outputName plots/%s_FR_elec.pdf --noOverflow --setTitle elec %s (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(numer_muon_2D_sipgt4, Form("--outputName plots/%s_FR_muon.pdf --noOverflow --setTitle muon %s (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 

  // FR sip3D < 4
  PlotMaker2D(numer_elec_2D_siplt4, Form("--outputName plots/%s_FR_elec_siplt4.pdf --noOverflow --setTitle elec %s (sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(numer_muon_2D_siplt4, Form("--outputName plots/%s_FR_muon_siplt4.pdf --noOverflow --setTitle muon %s (sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 

  // FR(sip3D<4)/FR(sip3D>4)
  numer_elec_2D_siplt4->Divide(numer_elec_2D_sipgt4 );
  numer_muon_2D_siplt4->Divide(numer_muon_2D_sipgt4);
  PlotMaker2D(numer_elec_2D_siplt4, Form("--outputName plots/%s_FR_elec_sipratio.pdf --noOverflow --zMax 1.4 --zMin 0.75 --setTitle elec %s FR(sip3D < 4)/FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(numer_muon_2D_siplt4, Form("--outputName plots/%s_FR_muon_sipratio.pdf --noOverflow --zMax 1.4 --zMin 0.75 --setTitle muon %s FR(sip3D < 4)/FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 

  // Isolation vs vs sip3D
  PlotMaker2D(miniiso_vs_sip_el, Form("  --outputName plots/%s_miniiso_vs_sip_el.pdf  --noOverflow --setTitle %s miniiso vs sip (elec)  --Xaxis sip3D --Yaxis miniiso  --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptrel_vs_sip_el, Form("    --outputName plots/%s_ptrel_vs_sip_el.pdf    --noOverflow --setTitle %s ptrel vs sip (elec)    --Xaxis sip3D --Yaxis ptrel    --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptratio_vs_sip_el, Form("  --outputName plots/%s_ptratio_vs_sip_el.pdf  --noOverflow --setTitle %s ptratio vs sip (elec)  --Xaxis sip3D --Yaxis ptratio  --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(multiiso_vs_sip_el, Form(" --outputName plots/%s_multiiso_vs_sip_el.pdf --noOverflow --setTitle %s multiiso vs sip (elec) --Xaxis sip3D --Yaxis multiiso --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(miniiso_vs_sip_mu, Form("  --outputName plots/%s_miniiso_vs_sip_mu.pdf  --noOverflow --setTitle %s miniiso vs sip (muon)  --Xaxis sip3D --Yaxis miniiso  --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptrel_vs_sip_mu, Form("    --outputName plots/%s_ptrel_vs_sip_mu.pdf    --noOverflow --setTitle %s ptrel vs sip (muon)    --Xaxis sip3D --Yaxis ptrel    --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptratio_vs_sip_mu, Form("  --outputName plots/%s_ptratio_vs_sip_mu.pdf  --noOverflow --setTitle %s ptratio vs sip (muon)  --Xaxis sip3D --Yaxis ptratio  --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(multiiso_vs_sip_mu, Form(" --outputName plots/%s_multiiso_vs_sip_mu.pdf --noOverflow --setTitle %s multiiso vs sip (muon) --Xaxis sip3D --Yaxis multiiso --colors", pfx.c_str(), desc.c_str()));

  PlotMaker2D(ptratio_vs_miniiso_el, Form("--outputName plots/%s_ptratio_vs_miniiso_el.pdf --noOverflow --setTitle %s 1/ptratio vs miniIso (elec) --Xaxis miniIso --Yaxis 1/p_{T}^{ratio} --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptratio_vs_miniiso_mu, Form("--outputName plots/%s_ptratio_vs_miniiso_mu.pdf --noOverflow --setTitle %s 1/ptratio vs miniIso (muon) --Xaxis miniIso --Yaxis 1/p_{T}^{ratio} --colors", pfx.c_str(), desc.c_str()));

  numer_vs_sip_el->Divide(numer_vs_sip_el, denom_vs_sip_el, 1, 1, "b");
  numer_vs_sip_mu->Divide(numer_vs_sip_mu, denom_vs_sip_mu, 1, 1, "b");

  TCanvas *c1 = new TCanvas("c1","c1");
  numer_vs_sip_el->Draw("pe");
  c1->SaveAs(Form("plots/%s_FR_vs_sip3d_el.pdf", pfx.c_str()));
  numer_vs_sip_mu->Draw("pe");
  c1->SaveAs(Form("plots/%s_FR_vs_sip3d_mu.pdf", pfx.c_str()));

  printCounter();

}

