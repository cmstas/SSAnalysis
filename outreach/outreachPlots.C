#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TColor.h"
#include "TH1F.h"
#include "OUT.h"
#include "../CORE/Tools/utils.h"
#include "../CORE/SSSelections.h"
#include "../CORE/MCSelections.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"

void outreachPlots(){

  //Define Chain
  TChain *chain = new TChain("t"); 
  chain->Add("/home/users/cgeorge/SSAnalysis/outreach/babymaker/smallbaby.root"); 

  //Define output plots
  float xbins[] = { 10, 15, 20, 25, 30, 40, 50, 65, 80, 100, 120 }; 
  TH1F *elec_numer = new TH1F("elec_numer", "elec_numer", 10, xbins); 
  TH1F *elec_denom = new TH1F("elec_denom", "elec_denom", 10, xbins); 
  TH1F *muon_numer = new TH1F("muon_numer", "muon_numer", 10, xbins); 
  TH1F *muon_denom = new TH1F("muon_denom", "muon_denom", 10, xbins); 

  //Do errors properly
  elec_numer->Sumw2();
  muon_numer->Sumw2();
  elec_denom->Sumw2();
  muon_denom->Sumw2();

  //MVA function
  createAndInitMVA("../CORE", true);

  //nEvents in chain
  unsigned int nEventsTotal = 0; 
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0; 

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    outreach.Init(tree);

    //Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntries(); event++){

      //Get Event Content
      outreach.GetEntry(event);
      nEventsTotal++;

      //Progress
      OUT::progress(nEventsTotal, nEventsChain);

      //If we fail genMET cut, done
      if (out::genmet() < 50) continue;

      //If we fail genHT, nJets cuts, done
      if (out::nGenJets() < 2) continue;
      if (out::htGen() < 200) continue;

      //If we get here, we have something, fill the denominator
      if (abs(out::genLep1_id()) == 11) elec_denom->Fill(out::genLep1_p4().pt()); 
      if (abs(out::genLep1_id()) == 13) muon_denom->Fill(out::genLep1_p4().pt()); 
      if (abs(out::genLep2_id()) == 11) elec_denom->Fill(out::genLep2_p4().pt()); 
      if (abs(out::genLep2_id()) == 13) muon_denom->Fill(out::genLep2_p4().pt()); 

      //If it passes ID, should go in the numerator
      if (out::lep1_passID() && abs(out::genLep1_id()) == 11) elec_numer->Fill(out::genLep1_p4().pt()); 
      if (out::lep1_passID() && abs(out::genLep1_id()) == 13) muon_numer->Fill(out::genLep1_p4().pt()); 
      if (out::lep2_passID() && abs(out::genLep2_id()) == 11) elec_numer->Fill(out::genLep2_p4().pt()); 
      if (out::lep2_passID() && abs(out::genLep2_id()) == 13) muon_numer->Fill(out::genLep2_p4().pt()); 
 
    }//event loop

  }//file loop

  //Now take the ratio
  elec_numer->Divide(elec_denom); 
  muon_numer->Divide(muon_denom); 

  //Now make the plot 
  vector <TH1F*> bkgds = { elec_numer, muon_numer }; 
  vector <string> titles = { "elec", "muon" }; 
  vector <Color_t> colors = { kBlue, kRed }; 
  TH1F* null = new TH1F("","",1,0,1);
  dataMCplotMaker(null, bkgds, titles, "Lepton Efficiency", "SS Baseline", "--outputName lepeff --yAxisLabel Efficiency --xAxisLabel p_{T}^{gen} --isLinear --noStack --noLumi --drawDots --noOverflow --setMaximum 1.0 --outOfFrame --legendUp -0.4", std::vector<TH1F*>(), std::vector<string>(), colors); 

}
