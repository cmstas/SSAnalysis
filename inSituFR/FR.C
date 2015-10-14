#include "../software/dataMCplotMaker/PlotMaker2D.h"
#include "../software/tableMaker/CTable.h"
#include "../commonUtils.h"
#include "SS.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TChain.h"
#include <fstream>

//Lumi
float luminosity = getLumi();

//Errors on MC or data?
bool dataErrors = true;

//testPC -- test the prompt contamination, ie allow numer-numer events
bool testPC = false;

//Include DY and W+Jets
bool others = false;

//SS Z-veto
bool ssZveto = false;

//Path
string tag = "v4.00";

bool passesNumeratorMVA(int which){
  if (abs(ss::lep1_id()) != 11) return true;
  float aeta = (which == 1) ? fabs(ss::lep1_p4().eta()) : fabs(ss::lep2_p4().eta());
  float disc = (which == 1) ? ss::lep1_MVA() : ss::lep2_MVA();
  if (aeta < 0.8) return disc > 0.87;
  if ((aeta >= 0.8 && aeta <= 1.479)) return disc > 0.60;
  if (aeta > 1.479) return disc > 0.17;
  return false;
}

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

  //Define hists
  nBinsX = 5; 
  int nBinsY = 3; 
  float xbins[] = { 10, 15, 25, 35, 50, 70 }; 
  float ybins[] = { 0, 1, 2, 2.4 }; 
  numer  = new TH2D("numer" , "numer" , nBinsX, xbins, nBinsY, ybins);
  denom  = new TH2D("denom" , "denom" , nBinsX, xbins, nBinsY, ybins);
  numer2 = new TH2D("numer2", "numer2", nBinsX, xbins, nBinsY, ybins);
  denom2 = new TH2D("denom2", "denom2", nBinsX, xbins, nBinsY, ybins);
  numer3 = new TH2D("numer3", "numer3", nBinsX, xbins, nBinsY, ybins);
  denom3 = new TH2D("denom3", "denom3", nBinsX, xbins, nBinsY, ybins);
  numer4 = new TH2D("numer4", "numer4", nBinsX, xbins, nBinsY, ybins);
  denom4 = new TH2D("denom4", "denom4", nBinsX, xbins, nBinsY, ybins);

  //Set hist errors
  if (!dataErrors){
    numer->Sumw2();
    denom->Sumw2();
    numer2->Sumw2();
    denom2->Sumw2();
    numer3->Sumw2();
    denom3->Sumw2();
    numer4->Sumw2();
    denom4->Sumw2();
  }

  //Declare chain
  TChain *chain = new TChain("t");
  int fo2_suffix = 0;
  chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR.root", tag.c_str()));
  if (others){
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY.root", tag.c_str()));
    chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets.root", tag.c_str()));
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

      //Pt
      float pt1 = ss::lep1_coneCorrPt(); 
      float pt2 = ss::lep2_coneCorrPt(); 

      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //Event Selection
      if (ss::njets() < 2) continue;
      if (ss::ht() < 80) continue;
      if (ss::met() < 30 && ss::ht() < 500) continue;
      if (ss::hyp_class() == 4 || ss::hyp_class() == 6) continue;

      //SS Z veto
      if (ssZveto && fabs((ss::lep1_p4() + ss::lep2_p4()).M() - 91) < 15) continue;

      //Must pass tight MVA
      if (!passesNumeratorMVA(1) && !passesNumeratorMVA(2)) continue;
 
      //Various variables
      float ptrel_cut_1    = (abs(ss::lep1_id()) == 11 ? 7.20 : 7.2);
      float ptrel_cut_2    = (abs(ss::lep2_id()) == 11 ? 7.20 : 7.2);
      float ptratio_cut_1  = (abs(ss::lep1_id()) == 11 ? 0.80 : 0.76);
      float ptratio_cut_2  = (abs(ss::lep2_id()) == 11 ? 0.80 : 0.76);
      float mini_cut_1     = (abs(ss::lep1_id()) == 11 ? 0.12 : 0.16);
      float mini_cut_2     = (abs(ss::lep2_id()) == 11 ? 0.12 : 0.16);
      bool lep1_denom_iso  = ((ss::lep1_miniIso() < mini_cut_1) && ((ss::lep1_ptrel_v1() > ptrel_cut_1) || ((ss::lep1_closeJet().pt()/ss::lep1_p4().pt()) < (1.0/ptratio_cut_1 + ss::lep1_miniIso()))));
      bool lep2_denom_iso  = ((ss::lep2_miniIso() < mini_cut_2) && ((ss::lep2_ptrel_v1() > ptrel_cut_2) || ((ss::lep2_closeJet().pt()/ss::lep2_p4().pt()) < (1.0/ptratio_cut_2 + ss::lep2_miniIso()))));

      //histo1 is for electrons with SIP > 4 
      if (abs(ss::lep1_id()) == 11 && (testPC || isGoodLeg(2)) && (testPC || isFakeLeg(1)) && ss::lep1_sip() > 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (passesNumeratorMVA(1) && ss::lep1_multiIso()) numer->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);  
        denom->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);
        counter++; 
      }
      else if (abs(ss::lep2_id()) == 11 && (testPC || isGoodLeg(1)) && (testPC || isFakeLeg(2)) && ss::lep2_sip() > 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (passesNumeratorMVA(2) && ss::lep2_multiIso()) numer->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);  
        denom->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);
      }

      //histo2 is for electrons with SIP < 4 
      else if (abs(ss::lep1_id()) == 11 && (testPC || isGoodLeg(2)) && (isFakeLeg(1) || testPC) && ss::lep1_sip() < 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (ss::lep1_multiIso()) numer2->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);  
        denom2->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);
      }
      else if (abs(ss::lep2_id()) == 11 && (testPC || isGoodLeg(1)) && (testPC || isFakeLeg(2)) && ss::lep2_sip() < 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (ss::lep2_multiIso()) numer2->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);  
        denom2->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);
      }

      //histo3 is for muons with SIP > 4 
      else if (abs(ss::lep1_id()) == 13 && (testPC || isGoodLeg(2)) && (isFakeLeg(1) || testPC) && ss::lep1_sip() > 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (passesNumeratorMVA(1) && ss::lep1_multiIso()) numer3->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);  
        denom3->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);
        counter++; 
      }
      else if (abs(ss::lep2_id()) == 13 && (testPC || isGoodLeg(1)) && (testPC || isFakeLeg(2)) && ss::lep2_sip() > 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (passesNumeratorMVA(2) && ss::lep2_multiIso()) numer3->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);  
        denom3->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);
      }

      //histo4 is for muons with SIP < 4 
      else if (abs(ss::lep1_id()) == 13 && (testPC || isGoodLeg(2)) && (testPC || isFakeLeg(1)) && ss::lep1_sip() < 4 && ss::lep2_passes_id() && lep1_denom_iso){
        if (ss::lep1_multiIso()) numer4->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);  
        denom4->Fill(pt1, fabs(ss::lep1_p4().eta()), ss::scale1fb()*luminosity);
      }
      else if (abs(ss::lep2_id()) == 13 && (testPC || isGoodLeg(1)) && (testPC || isFakeLeg(2)) && ss::lep2_sip() < 4 && ss::lep1_passes_id() && lep2_denom_iso){
        if (ss::lep2_multiIso()) numer4->Fill(pt2, fabs(ss::lep2_p4().eta()), ss::scale1fb()*luminosity);  
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

  //Set hist errors
  if (dataErrors){
    numer->Sumw2();
    denom->Sumw2();
    numer2->Sumw2();
    denom2->Sumw2();
    numer3->Sumw2();
    denom3->Sumw2();
    numer4->Sumw2();
    denom4->Sumw2();
  }

  //Divide numer/denom
  cout << numer->GetBinContent(22) << "/" << denom->GetBinContent(22) << " = " << 1.0*numer->GetBinContent(22)/denom->GetBinContent(22)<< endl;
  numer ->Divide(numer , denom , 1, 1, "b"); 
  numer2->Divide(numer2, denom2, 1, 1, "b"); 
  numer3->Divide(numer3, denom3, 1, 1, "b"); 
  numer4->Divide(numer4, denom4, 1, 1, "b"); 

  ofstream myfile;
  string name; 
  if (testPC == false && others == false){
    myfile.open("FO.cc"); 
    name = "FO";
  }
  else if (testPC == true && others == true){
    myfile.open("PC.cc"); 
    name = "PC";
  }
  else{
    myfile.open("EXPT.cc"); 
    name = "EXPT";
  }

  //Function Output
  myfile << "float functionAG_" << name << "(float pt, float eta, int id){" << endl;
  myfile << "  if (abs(id) == 11){ " << endl;
  myfile << "    if (fabs(eta) < 1){ " << endl;
  myfile << "      if (fabs(pt) < 15) return " << numer->GetBinContent(1,1) << ";" << endl;
  myfile << "      if (fabs(pt) < 25) return " << numer->GetBinContent(2,1) << ";" << endl;
  myfile << "      if (fabs(pt) < 35) return " << numer->GetBinContent(3,1) << ";" << endl;
  myfile << "      if (fabs(pt) < 50) return " << numer->GetBinContent(4,1) << ";" << endl;
  myfile << "      return " << numer->GetBinContent(5,1) << ";" << endl;
  myfile << "    } " << endl;
  myfile << "    if (fabs(eta) < 2){ " << endl;
  myfile << "      if (fabs(pt) < 15) return " << numer->GetBinContent(1,2) << ";" << endl;
  myfile << "      if (fabs(pt) < 25) return " << numer->GetBinContent(2,2) << ";" << endl;
  myfile << "      if (fabs(pt) < 35) return " << numer->GetBinContent(3,2) << ";" << endl;
  myfile << "      if (fabs(pt) < 50) return " << numer->GetBinContent(4,2) << ";" << endl;
  myfile << "      return " << numer->GetBinContent(5,2) << ";" << endl;
  myfile << "    } " << endl;
  myfile << "    if (fabs(eta) < 4){ " << endl;
  myfile << "      if (fabs(pt) < 15) return " << numer->GetBinContent(1,3) << ";" << endl;
  myfile << "      if (fabs(pt) < 25) return " << numer->GetBinContent(2,3) << ";" << endl;
  myfile << "      if (fabs(pt) < 35) return " << numer->GetBinContent(3,3) << ";" << endl;
  myfile << "      if (fabs(pt) < 50) return " << numer->GetBinContent(4,3) << ";" << endl;
  myfile << "      return " << numer->GetBinContent(5,3) << ";" << endl;
  myfile << "    } " << endl;
  myfile << "  } " << endl;
  myfile << "  if (abs(id) == 13){ " << endl;
  myfile << "    if (fabs(eta) < 1){ " << endl;
  myfile << "      if (fabs(pt) < 15) return " << numer3->GetBinContent(1,1) << ";" << endl;
  myfile << "      if (fabs(pt) < 25) return " << numer3->GetBinContent(2,1) << ";" << endl;
  myfile << "      if (fabs(pt) < 35) return " << numer3->GetBinContent(3,1) << ";" << endl;
  myfile << "      if (fabs(pt) < 50) return " << numer3->GetBinContent(4,1) << ";" << endl;
  myfile << "      return " << numer3->GetBinContent(5,1) << ";" << endl;
  myfile << "    } " << endl;
  myfile << "    if (fabs(eta) < 2){ " << endl;
  myfile << "      if (fabs(pt) < 15) return " << numer3->GetBinContent(1,2) << ";" << endl;
  myfile << "      if (fabs(pt) < 25) return " << numer3->GetBinContent(2,2) << ";" << endl;
  myfile << "      if (fabs(pt) < 35) return " << numer3->GetBinContent(3,2) << ";" << endl;
  myfile << "      if (fabs(pt) < 50) return " << numer3->GetBinContent(4,2) << ";" << endl;
  myfile << "      return " << numer3->GetBinContent(5,2) << ";" << endl;
  myfile << "    } " << endl;
  myfile << "    if (fabs(eta) < 2.4){ " << endl;
  myfile << "      if (fabs(pt) < 15) return " << numer3->GetBinContent(1,3) << ";" << endl;
  myfile << "      if (fabs(pt) < 25) return " << numer3->GetBinContent(2,3) << ";" << endl;
  myfile << "      if (fabs(pt) < 35) return " << numer3->GetBinContent(3,3) << ";" << endl;
  myfile << "      if (fabs(pt) < 50) return " << numer3->GetBinContent(4,3) << ";" << endl;
  myfile << "      return " << numer3->GetBinContent(5,3) << ";" << endl;
  myfile << "    } " << endl;
  myfile << "  } " << endl;
  myfile << "  return 0; " << endl;
  myfile << "} " << endl;


  //Save output
  string name2 = "blah";
  if (!others  && !testPC && !ssZveto) name2 = "normal"; 
  if ( others  &&  testPC && !ssZveto) name2 = "PC"; 
  if ( others  && !testPC && !ssZveto) name2 = "soup";
  if (!others  && !testPC &&  ssZveto) name2 = "ssZ"; 
  if ( others  &&  testPC &&  ssZveto) name2 = "PCssZ"; 

  TFile *file = new TFile(Form("inSituFR_cone_FR_histos_%s.root", name2.c_str()), "RECREATE");
  file->Write(); 
  numer->Write("elec");
  numer3->Write("muon");
  file->Close(); 

  //Plots
  PlotMaker2D(numer , Form("--outputName FR_elec_%s.pdf --noOverflow --setTitle elec %s --Xaxis fake p_{T} --Yaxis |#eta|", name2.c_str(), name2.c_str())); 
  PlotMaker2D(numer3, Form("--outputName FR_muon_%s.pdf --noOverflow --setTitle muon %s --Xaxis fake p_{T} --Yaxis |#eta|", name2.c_str(), name2.c_str())); 
 
  cout << counter << endl;
}
