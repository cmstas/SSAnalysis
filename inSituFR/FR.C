#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/dataMCplotMaker/PlotMaker2D.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "SS.h"
#include "TH2F.h"

//Lumi
float lumi = 10.0;

//Errors on MC or data?
bool dataErrors = true;

//Bin fine or actual
int binFine = false; 

//Eta or pT plot
bool withEta = true;

void FR1D(){

  //Declare hists
  int nBinsX = 20;
  TH1F *numer ; 
  TH1F *denom ; 
  TH1F *numer2; 
  TH1F *denom2; 
  TH1F *numer3; 
  TH1F *denom3; 
  TH1F *numer4; 
  TH1F *denom4; 

  //Define hists
  if (binFine){
    numer  = new TH1F("numer" , "numer" , nBinsX, 0, 100);
    denom  = new TH1F("denom" , "denom" , nBinsX, 0, 100);
    numer2 = new TH1F("numer2", "numer2", nBinsX, 0, 100);
    denom2 = new TH1F("denom2", "denom2", nBinsX, 0, 100);
    numer3 = new TH1F("numer3", "numer3", nBinsX, 0, 100);
    denom3 = new TH1F("denom3", "denom3", nBinsX, 0, 100);
    numer4 = new TH1F("numer4", "numer4", nBinsX, 0, 100);
    denom4 = new TH1F("denom4", "denom4", nBinsX, 0, 100);
  }
  if (!binFine){ 
    nBinsX = 5; 
    float xbins[] = { 10, 15, 25, 35, 50, 70 }; 
    numer  = new TH1F("numer" , "numer" , nBinsX, xbins);
    denom  = new TH1F("denom" , "denom" , nBinsX, xbins);
    numer2 = new TH1F("numer2", "numer2", nBinsX, xbins);
    denom2 = new TH1F("denom2", "denom2", nBinsX, xbins);
    numer3 = new TH1F("numer3", "numer3", nBinsX, xbins);
    denom3 = new TH1F("denom3", "denom3", nBinsX, xbins);
    numer4 = new TH1F("numer4", "numer4", nBinsX, xbins);
    denom4 = new TH1F("denom4", "denom4", nBinsX, xbins);
  }

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
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.16/TTBAR_multiIso.root");

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
      if (ss::njets() < 2) continue;
      if (ss::ht() < 80) continue;
      if (ss::met() < 30 && ss::ht() < 500) continue;

      //If lep is the fake one and it passes SIPID > 4, it goes in plot
      if (abs(ss::lep1_id()) == 11 && ss::lep1_isFakeLeg() && ss::lep1_sip() > 4){
        if (ss::lep1_multiIso()) numer->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumi);  
        denom->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumi);
      }
      if (abs(ss::lep2_id()) == 11 && ss::lep2_isFakeLeg() && ss::lep2_sip() > 4){
        if (ss::lep2_multiIso()) numer->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumi);  
        denom->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumi);
      }

      //If lep is the fake one and it passes SIPID < 4, it goes in plot
      if (abs(ss::lep1_id()) == 11 && ss::lep1_isFakeLeg() && ss::lep1_sip() < 4){
        if (ss::lep1_multiIso()) numer2->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumi);  
        denom2->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumi);
      }
      if (abs(ss::lep2_id()) == 11 && ss::lep2_isFakeLeg() && ss::lep2_sip() < 4){
        if (ss::lep2_multiIso()) numer2->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumi);  
        denom2->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumi);
      }

      //If lep is the fake one and it passes SIPID > 4, it goes in plot
      if (abs(ss::lep1_id()) == 11 && ss::lep1_isFakeLeg() && ss::lep1_sip() > 4){
        if (ss::lep1_multiIso()) numer3->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumi);  
        denom3->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumi);
      }
      if (abs(ss::lep2_id()) == 11 && ss::lep2_isFakeLeg() && ss::lep2_sip() > 4){
        if (ss::lep2_multiIso()) numer3->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumi);  
        denom3->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumi);
      }

      //If lep is the fake one and it passes SIPID > 4, it goes in plot
      if (abs(ss::lep1_id()) == 11 && ss::lep1_isFakeLeg() && ss::lep1_sip() > 4){
        if (ss::lep1_multiIso()) numer4->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumi);  
        denom4->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumi);
      }
      if (abs(ss::lep2_id()) == 11 && ss::lep2_isFakeLeg() && ss::lep2_sip() > 4){
        if (ss::lep2_multiIso()) numer4->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumi);  
        denom4->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumi);
      }


    }//event loop
  }//file loop

  //Overflow
  denom ->SetBinContent(nBinsX, denom->GetBinContent(nBinsX) + denom->GetBinContent(nBinsX+1)); 
  numer ->SetBinContent(nBinsX, numer->GetBinContent(nBinsX) + numer->GetBinContent(nBinsX+1)); 
  denom2->SetBinContent(nBinsX, denom2->GetBinContent(nBinsX) + denom2->GetBinContent(nBinsX+1)); 
  numer2->SetBinContent(nBinsX, numer2->GetBinContent(nBinsX) + numer2->GetBinContent(nBinsX+1)); 
  denom3->SetBinContent(nBinsX, denom3->GetBinContent(nBinsX) + denom3->GetBinContent(nBinsX+1)); 
  numer3->SetBinContent(nBinsX, numer3->GetBinContent(nBinsX) + numer3->GetBinContent(nBinsX+1)); 
  denom4->SetBinContent(nBinsX, denom4->GetBinContent(nBinsX) + denom4->GetBinContent(nBinsX+1)); 
  numer4->SetBinContent(nBinsX, numer4->GetBinContent(nBinsX) + numer4->GetBinContent(nBinsX+1)); 

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
  numer ->Divide(numer , denom , 1, 1, "b"); 
  numer2->Divide(numer2, denom2, 1, 1, "b"); 
  numer3->Divide(numer3, denom3, 1, 1, "b"); 
  numer4->Divide(numer4, denom4, 1, 1, "b"); 

  TH1F *null = new TH1F("","", 1, 0, 1); 
  vector <TH1F*> backgrounds;
  backgrounds.push_back(numer); 
  vector <TH1F*> backgrounds_muon;
  backgrounds_muon.push_back(numer3); 
  vector <string> titles;
  titles.push_back("SIP3D > 4"); 
  vector <TH1F*> null_vector;
  null_vector.push_back(null);
  vector <string> null_titles;
  null_titles.push_back("");
  vector <Color_t> colors;
  colors.push_back(kRed); 

  dataMCplotMaker(numer2, backgrounds, titles, "FR", "elec", "--outputName result_elec --xAxisLabel fake p_{T} --isLinear --noOverflow --setMaximum 1.0 --drawDots --dataName SIP3D < 4 --noOverflow", null_vector, null_titles, colors);
  dataMCplotMaker(numer4, backgrounds_muon, titles, "FR", "muon", "--outputName result_muon --xAxisLabel fake p_{T} --isLinear --noOverflow --setMaximum 1.0 --drawDots --dataName SIP3D < 4 --noOverflow", null_vector, null_titles, colors);

}

void FR2D(){

  //Declare hists
  int nBinsX = 20;
  TH2F *numer ; 
  TH2F *denom ; 
  TH2F *numer2; 
  TH2F *denom2; 
  TH2F *numer3; 
  TH2F *denom3; 
  TH2F *numer4; 
  TH2F *denom4; 

  //Define hists
  nBinsX = 5; 
  int nBinsY = 3; 
  float xbins[] = { 10, 15, 25, 35, 50, 70 }; 
  float ybins[] = { 0, 1, 2, 4 }; 
  numer  = new TH2F("numer" , "numer" , nBinsX, xbins, nBinsY, ybins);
  denom  = new TH2F("denom" , "denom" , nBinsX, xbins, nBinsY, ybins);
  numer2 = new TH2F("numer2", "numer2", nBinsX, xbins, nBinsY, ybins);
  denom2 = new TH2F("denom2", "denom2", nBinsX, xbins, nBinsY, ybins);
  numer3 = new TH2F("numer3", "numer3", nBinsX, xbins, nBinsY, ybins);
  denom3 = new TH2F("denom3", "denom3", nBinsX, xbins, nBinsY, ybins);
  numer4 = new TH2F("numer4", "numer4", nBinsX, xbins, nBinsY, ybins);
  denom4 = new TH2F("denom4", "denom4", nBinsX, xbins, nBinsY, ybins);

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
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.16/TTBAR_multiIso.root");

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
      if (ss::njets() < 2) continue;
      if (ss::ht() < 80) continue;
      if (ss::met() < 30 && ss::ht() < 500) continue;

      //If lep is the fake one and it passes SIPID > 4, it goes in plot
      if (abs(ss::lep1_id()) == 11 && ss::lep1_isFakeLeg() && ss::lep1_sip() > 4){
        if (ss::lep1_multiIso()) numer->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::scale1fb()*lumi);  
         denom->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::scale1fb()*lumi);
      }
      if (abs(ss::lep2_id()) == 11 && ss::lep2_isFakeLeg() && ss::lep2_sip() > 4){
        if (ss::lep2_multiIso()) numer->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::scale1fb()*lumi);  
         denom->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::scale1fb()*lumi);
      }

      //If lep is the fake one and it passes SIPID < 4, it goes in plot
      if (abs(ss::lep1_id()) == 11 && ss::lep1_isFakeLeg() && ss::lep1_sip() < 4){
        if (ss::lep1_multiIso()) numer2->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::scale1fb()*lumi);  
        denom2->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::scale1fb()*lumi);
      }
      if (abs(ss::lep2_id()) == 11 && ss::lep2_isFakeLeg() && ss::lep2_sip() < 4){
        if (ss::lep2_multiIso()) numer2->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::scale1fb()*lumi);  
        denom2->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::scale1fb()*lumi);
      }

      //If lep is the fake one and it passes SIPID > 4, it goes in plot
      if (abs(ss::lep1_id()) == 13 && ss::lep1_isFakeLeg() && ss::lep1_sip() > 4){
        if (ss::lep1_multiIso()) numer3->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::scale1fb()*lumi);  
         denom3->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::scale1fb()*lumi);
      }
      if (abs(ss::lep2_id()) == 13 && ss::lep2_isFakeLeg() && ss::lep2_sip() > 4){
        if (ss::lep2_multiIso()) numer3->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::scale1fb()*lumi);  
         denom3->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::scale1fb()*lumi);
      }

      //If lep is the fake one and it passes SIPID < 4, it goes in plot
      if (abs(ss::lep1_id()) == 13 && ss::lep1_isFakeLeg() && ss::lep1_sip() < 4){
        if (ss::lep1_multiIso()) numer4->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::scale1fb()*lumi);  
        denom4->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::scale1fb()*lumi);
      }
      if (abs(ss::lep2_id()) == 13 && ss::lep2_isFakeLeg() && ss::lep2_sip() < 4){
        if (ss::lep2_multiIso()) numer4->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::scale1fb()*lumi);  
        denom4->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::scale1fb()*lumi);
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
  numer ->Divide(numer , denom , 1, 1, "b"); 
  numer2->Divide(numer2, denom2, 1, 1, "b"); 
  numer3->Divide(numer3, denom3, 1, 1, "b"); 
  numer4->Divide(numer4, denom4, 1, 1, "b"); 

  //Function Output
  cout << "float function(LorentzVector lep, int id){" << endl;
  cout << "  if (abs(id) == 11){ " << endl;
  cout << "    if (fabs(lep.eta()) < 1){ " << endl;
  cout << "      if (fabs(lep.pt()) < 15) return " << numer->GetBinContent(1,1) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 25) return " << numer->GetBinContent(2,1) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 35) return " << numer->GetBinContent(3,1) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 50) return " << numer->GetBinContent(4,1) << ";" << endl;
  cout << "      return " << numer->GetBinContent(5,1) << ";" << endl;
  cout << "    } " << endl;
  cout << "    if (fabs(lep.eta()) < 2){ " << endl;
  cout << "      if (fabs(lep.pt()) < 15) return " << numer->GetBinContent(1,2) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 25) return " << numer->GetBinContent(2,2) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 35) return " << numer->GetBinContent(3,2) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 50) return " << numer->GetBinContent(4,2) << ";" << endl;
  cout << "      return " << numer->GetBinContent(5,2) << ";" << endl;
  cout << "    } " << endl;
  cout << "    if (fabs(lep.eta()) < 4){ " << endl;
  cout << "      if (fabs(lep.pt()) < 15) return " << numer->GetBinContent(1,3) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 25) return " << numer->GetBinContent(2,3) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 35) return " << numer->GetBinContent(3,3) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 50) return " << numer->GetBinContent(4,3) << ";" << endl;
  cout << "      return " << numer->GetBinContent(5,3) << ";" << endl;
  cout << "    } " << endl;
  cout << "  } " << endl;
  cout << "  if (abs(id) == 13){ " << endl;
  cout << "    if (fabs(lep.eta()) < 1){ " << endl;
  cout << "      if (fabs(lep.pt()) < 15) return " << numer3->GetBinContent(1,1) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 25) return " << numer3->GetBinContent(2,1) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 35) return " << numer3->GetBinContent(3,1) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 50) return " << numer3->GetBinContent(4,1) << ";" << endl;
  cout << "      return " << numer3->GetBinContent(5,1) << ";" << endl;
  cout << "    } " << endl;
  cout << "    if (fabs(lep.eta()) < 2){ " << endl;
  cout << "      if (fabs(lep.pt()) < 15) return " << numer3->GetBinContent(1,2) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 25) return " << numer3->GetBinContent(2,2) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 35) return " << numer3->GetBinContent(3,2) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 50) return " << numer3->GetBinContent(4,2) << ";" << endl;
  cout << "      return " << numer3->GetBinContent(5,2) << ";" << endl;
  cout << "    } " << endl;
  cout << "    if (fabs(lep.eta()) < 4){ " << endl;
  cout << "      if (fabs(lep.pt()) < 15) return " << numer3->GetBinContent(1,3) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 25) return " << numer3->GetBinContent(2,3) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 35) return " << numer3->GetBinContent(3,3) << ";" << endl;
  cout << "      if (fabs(lep.pt()) < 50) return " << numer3->GetBinContent(4,3) << ";" << endl;
  cout << "      return " << numer3->GetBinContent(5,3) << ";" << endl;
  cout << "    } " << endl;
  cout << "  } " << endl;
  cout << "} " << endl;

  //Print output
  PlotMaker2D(numer , "--outputName gt_elec.pdf --noOverflow --setTitle elec, SIP3D > 4 --Xaxis fake p_{T} --Yaxis |#eta|"); 
  PlotMaker2D(numer2, "--outputName lt_elec.pdf --noOverflow --setTitle elec, SIP3D < 4 --Xaxis fake p_{T} --Yaxis |#eta|"); 
  PlotMaker2D(numer3, "--outputName gt_muon.pdf --noOverflow --setTitle muon, SIP3D > 4 --Xaxis fake p_{T} --Yaxis |#eta|"); 
  PlotMaker2D(numer4, "--outputName lt_muon.pdf --noOverflow --setTitle muon, SIP3D < 4 --Xaxis fake p_{T} --Yaxis |#eta|"); 
}

void FR(){
  if ( withEta) FR2D(); 
  if (!withEta) FR1D(); 
}
