#include "../software/dataMCplotMaker/PlotMaker2D.h"
#include "../software/tableMaker/CTable.h"
#include "../commonUtils.h"
#include "../classFiles/v6.02/SS.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TChain.h"
#include <fstream>
#include "../CORE/IsolationTools.h"
#include "../CORE/Tools/dorky/dorky.h"

//do the data!!
bool doData = true; 

//testPC -- test the prompt contamination, ie allow numer-numer events
bool testPC = false;

//Include DY and W+Jets
bool others = false;

//SS Z-veto
bool ssZveto = true;

//Path
string tag = getTag().Data();

//Lumi
float luminosity = doData ? 1.0 : getLumi();

bool passesMVA(int which, int level){
  float eta1[3]={0.87, 0.05,-0.36}; 
  float eta2[3]={0.60,-0.26,-0.58}; 
  float eta3[3]={0.17,-0.40,-0.62}; 
  if (abs(ss::lep1_id()) != 11) return true;
  float aeta = (which == 1) ? fabs(ss::lep1_p4().eta()) : fabs(ss::lep2_p4().eta());//uhm this should be SCeta
  float disc = (which == 1) ? ss::lep1_MVA() : ss::lep2_MVA();
  if (aeta < 0.8) return disc > eta1[level];
  if ((aeta >= 0.8 && aeta <= 1.479)) return disc > eta2[level];
  if (aeta > 1.479) return disc > eta3[level];
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

void FR(){

  std::cout << "Using lumi = " << luminosity << "/fb" << std::endl;
  std::cout << "Using " << tag << " babies" << std::endl;

  int counter = 0;

  //Declare hists
  int nBinsX = 20;
  TH2D *numer ; 
  TH2D *denom ; 
  TH2D *numer2; 
  TH2D *denom2; 
  TH2D *numer3; 
  TH2D *denom3; 
  TH2D *numer4; 
  TH2D *denom4; 

  //Reproduce Lesya's plot of 1/ptratio vs. miniiso 
  TH2D *test; 

  //Define hists
  nBinsX = 5; 
  int nBinsY = 1; 
  float xbins[] = { 10, 15, 25, 35, 50, 70 }; 
  //float ybins[] = { 0, 0.8, 1.479, 2.5 }; 
  float ybins[] = { 0, 2.4 }; 
  numer  = new TH2D("numer" , "numer" , nBinsX, xbins, nBinsY, ybins);
  denom  = new TH2D("denom" , "denom" , nBinsX, xbins, nBinsY, ybins);
  numer2 = new TH2D("numer2", "numer2", nBinsX, xbins, nBinsY, ybins);
  denom2 = new TH2D("denom2", "denom2", nBinsX, xbins, nBinsY, ybins);
  numer3 = new TH2D("numer3", "numer3", nBinsX, xbins, nBinsY, ybins);
  denom3 = new TH2D("denom3", "denom3", nBinsX, xbins, nBinsY, ybins);
  numer4 = new TH2D("numer4", "numer4", nBinsX, xbins, nBinsY, ybins);
  denom4 = new TH2D("denom4", "denom4", nBinsX, xbins, nBinsY, ybins);
  test = new TH2D("test", "test", 40, 0, 0.4, 20, 1.0, 3.0); 

  //Set hist errors
  numer->Sumw2();
  denom->Sumw2();
  numer2->Sumw2();
  denom2->Sumw2();
  numer3->Sumw2();
  denom3->Sumw2();
  numer4->Sumw2();
  denom4->Sumw2();

  //Declare chain
  TChain *chain = new TChain("t");
  int fo2_suffix = 0;
  if (doData){
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGC_05oct.root"  , tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_05oct.root"  , tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_v4.root"     , tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonC_05oct.root", tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_05oct.root", tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_v4.root"   , tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGC_05oct.root"    , tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_05oct.root"    , tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_v4.root"       , tag.c_str()));
  }
  else if (!doData){
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR.root", tag.c_str()));
    if (others){
      chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY.root", tag.c_str()));
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
      // relax cuts for low lumi
      //if (ss::njets() < 2) continue;
      //if (ss::ht() < 80) continue;
      //if (ss::met() < 30 && ss::ht() < 500) continue;
      if (ss::hyp_class() == 4 || ss::hyp_class() == 6) continue;

      //Non-Isolated trigger requirements
      if (ss::ht() > 300){
        if (!ss::fired_trigger()) continue; 
        if (!passesMVA(1,1) || !passesMVA(2,1)) continue;
      }
      if (ss::ht() < 300){
        if (!passesMVA(1,2) || !passesMVA(2,2)) continue;
      }

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

      //histo1 is for electrons with SIP > 4 
      if (abs(ss::lep1_id()) == 11 && (testPC || isGoodLeg(2)) && (testPC || isFakeLeg(1)) && ss::lep1_sip() > 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (passesMVA(1,0) && lep1_multiIso) numer->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);  
        denom->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);
        counter++; 
      }
      if (abs(ss::lep2_id()) == 11 && (testPC || isGoodLeg(1)) && (testPC || isFakeLeg(2)) && ss::lep2_sip() > 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (passesMVA(2,0) && lep2_multiIso) numer->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);  
        denom->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);
      }

      //histo2 is for electrons with SIP < 4 
      if (abs(ss::lep1_id()) == 11 && (testPC || isGoodLeg(2)) && (isFakeLeg(1) || testPC) && ss::lep1_sip() < 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (lep1_multiIso) numer2->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);  
        denom2->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);
      }
      if (abs(ss::lep2_id()) == 11 && (testPC || isGoodLeg(1)) && (testPC || isFakeLeg(2)) && ss::lep2_sip() < 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (lep2_multiIso) numer2->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);  
        denom2->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);
      }

      //histo3 is for muons with SIP > 4 
      if (abs(ss::lep1_id()) == 13 && ss::lep1_sip() > 4 && (testPC || isGoodLeg(2)) && (isFakeLeg(1) || testPC) && ss::lep2_passes_id() && lep1_denom_iso){
        if (passesMVA(1,0) && lep1_multiIso) numer3->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);  
        if (ss::lep1_p4().pt() >= 25) test->Fill(miniiso_1, 1.0/ptratio_1);  
        denom3->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);

        counter++; 
      }
      if (abs(ss::lep2_id()) == 13 && ss::lep2_sip() > 4 && (testPC || isGoodLeg(1)) && (testPC || isFakeLeg(2)) && ss::lep1_passes_id() && lep2_denom_iso){
        if (passesMVA(2,0) && lep2_multiIso) numer3->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);  
        if (ss::lep2_p4().pt() >= 25) test->Fill(miniiso_2, 1.0/ptratio_2);  
        denom3->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);
      }

      //histo4 is for muons with SIP < 4 
      if (abs(ss::lep1_id()) == 13 && (testPC || isGoodLeg(2)) && (testPC || isFakeLeg(1)) && ss::lep1_sip() < 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (lep1_multiIso) numer4->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);  
        denom4->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);
      }
      if (abs(ss::lep2_id()) == 13 && (testPC || isGoodLeg(1)) && (testPC || isFakeLeg(2)) && ss::lep2_sip() < 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (lep2_multiIso) numer4->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);  
        denom4->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);
      }

    }//event loop
  }//file loop

  //Overflow
  for (unsigned int i = 0; i < 3; i++){
    denom ->SetBinContent(nBinsX, i, denom->GetBinContent(nBinsX , i) + denom->GetBinContent(nBinsX+1 , i)); 
    numer ->SetBinContent(nBinsX, i, numer->GetBinContent(nBinsX , i) + numer->GetBinContent(nBinsX+1 , i)); 
    denom2->SetBinContent(nBinsX, i, denom2->GetBinContent(nBinsX, i) + denom2->GetBinContent(nBinsX+1, i)); 
    numer2->SetBinContent(nBinsX, i, numer2->GetBinContent(nBinsX, i) + numer2->GetBinContent(nBinsX+1, i)); 
    denom3->SetBinContent(nBinsX, i, denom3->GetBinContent(nBinsX, i) + denom3->GetBinContent(nBinsX+1, i)); 
    numer3->SetBinContent(nBinsX, i, numer3->GetBinContent(nBinsX, i) + numer3->GetBinContent(nBinsX+1, i)); 
    denom4->SetBinContent(nBinsX, i, denom4->GetBinContent(nBinsX, i) + denom4->GetBinContent(nBinsX+1, i)); 
    numer4->SetBinContent(nBinsX, i, numer4->GetBinContent(nBinsX, i) + numer4->GetBinContent(nBinsX+1, i)); 
  }

  //Divide numer/denom
  numer ->Divide(numer , denom , 1, 1, "b"); 
  numer2->Divide(numer2, denom2, 1, 1, "b"); 
  numer3->Divide(numer3, denom3, 1, 1, "b"); 
  numer4->Divide(numer4, denom4, 1, 1, "b"); 

  //Save output
  string name2 = "blah";
  if (!doData && !others  && !testPC && !ssZveto) name2 = "normal"; 
  if (!doData &&  others  &&  testPC && !ssZveto) name2 = "PC"; 
  if (!doData &&  others  && !testPC && !ssZveto) name2 = "soup";
  if (!doData && !others  && !testPC &&  ssZveto) name2 = "ssZ"; 
  if (!doData &&  others  &&  testPC &&  ssZveto) name2 = "PCssZ"; 
  if ( doData && !others  && !testPC && !ssZveto) name2 = "data_normal"; 
  if ( doData &&  others  &&  testPC && !ssZveto) name2 = "data_PC"; 
  if ( doData &&  others  && !testPC && !ssZveto) name2 = "data_soup";
  if ( doData && !others  && !testPC &&  ssZveto) name2 = "data_ssZ"; 
  if ( doData &&  others  &&  testPC &&  ssZveto) name2 = "data_PCssZ"; 

  TFile *file = new TFile(Form("inSituFR_cone_FR_histos_%s.root", name2.c_str()), "RECREATE");
  file->Write(); 
  numer->Write("elec");
  numer3->Write("muon");
  file->Close(); 

  //Plots
  PlotMaker2D(numer , Form("--outputName FR_elec_%s.pdf --noOverflow --setTitle elec %s --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", name2.c_str(), name2.c_str())); 
  PlotMaker2D(numer3, Form("--outputName FR_muon_%s.pdf --noOverflow --setTitle muon %s --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", name2.c_str(), name2.c_str())); 
  PlotMaker2D(test, "--outputName test.pdf --noOverflow --setTitle muons, pT > 25, SIP3D > 4 --Xaxis miniisolation --Yaxis 1/p_{T}^{ratio} --colors"); 
}
