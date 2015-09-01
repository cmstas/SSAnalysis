#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "../software/dataMCplotMaker/dataMCplotMaker.h"

//Regular includes
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1.h"
#include "TChain.h"

//Class files
#include "SS.h"

//Namespace
using namespace std;

//Path
string path = "../v1.09";

//Global Definitions
vector <TH1F*> makePlots(TChain *chain);
void drawPlot(int which, string title = "", string subtitle = "", string xaxis = "quantity");
vector <TH1F*> ttz_plots;
vector <TH1F*> ttw_plots;   
vector <TH1F*> wz_plots;    
vector <TH1F*> ttbar_plots; 

//Main function
void plots(){

  //Declare chains
  TChain *ttz   = new TChain("t"); ttz  ->Add(Form("%s/TTZ_multiIso.root"  , path.c_str())); 
  TChain *ttw   = new TChain("t"); ttw  ->Add(Form("%s/TTW_multiIso.root"  , path.c_str())); 
  TChain *wz    = new TChain("t"); wz   ->Add(Form("%s/WZ_multiIso.root"   , path.c_str())); 
  TChain *ttbar = new TChain("t"); ttbar->Add(Form("%s/TTBAR_multiIso.root", path.c_str())); 

  //Make plots
  ttz_plots   = makePlots(ttz);
  ttw_plots   = makePlots(ttw);
  wz_plots    = makePlots(wz);
  ttbar_plots = makePlots(ttbar);

  //Draw plots
  drawPlot(0, "MET PHYS14 multiIso", "p_{T} 25/25, #geq 2 jets"          , "MET"   );
  drawPlot(1, "HT PHYS14 multiIso" , "p_{T} 25/25, MET > 50, #geq 2 jets", "HT"    );

}

//Fill plot function
vector <TH1F*> makePlots(TChain *chain){

  //Declare plots
  TH1F *met = new TH1F("met", "met",  80, 0, 400); 
  TH1F *ht  = new TH1F("ht" , "ht" , 100, 0, 800); 

  //Lumi
  float lumi = 10.0;
 
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

    //Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      samesign.GetEntry(event);
      nEventsTotal++;
    
      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //Essential Cuts
      if (ss::hyp_class() != 3) continue;

      //Make lepton plots here

      //Lepton cuts 
      if (ss::lep1_p4().pt() < 25) continue;
      if (ss::lep2_p4().pt() < 25) continue;

      //Make jet plots here

      //Jet cuts
      if (ss::njets() < 2) continue;

      //Make met plots here
      met->Fill(ss::met(), ss::scale1fb()*lumi);  

      //Met cuts
      if (ss::met() < 50) continue;
   
      //Make all other plots
      ht->Fill(ss::ht(), ss::scale1fb()*lumi);  

    }//event loop
  }//file loop

  vector <TH1F*> results;
  results.push_back(met);
  results.push_back(ht);

  return results;

}

void drawPlot(int which, string title, string subtitle, string xaxis){
  
  //Vector of backgrounds
  vector <TH1F*> backgrounds;
  backgrounds.push_back(ttz_plots.at(which));
  backgrounds.push_back(ttw_plots.at(which));   
  backgrounds.push_back(wz_plots.at(which));    
  backgrounds.push_back(ttbar_plots.at(which)); 

  //Vector of background titles
  vector <string> titles;
  titles.push_back("ttZ");
  titles.push_back("ttW");
  titles.push_back("WZ");
  titles.push_back("ttbar");

  //Null plot
  TH1F* null = new TH1F("","",1,0,1);

  //Make plots
  dataMCplotMaker(null, backgrounds, titles, title, subtitle, Form("--outputName %s --xAxisLabel %s", xaxis.c_str(), xaxis.c_str())); 
  
}

//Make plot cumulative function
TH1F* makePlotCumulative(TH1F* hist_bkgd){
  float integ = hist_bkgd->Integral(0,hist_bkgd->GetNbinsX()+1);
  float integ_no_overflow = hist_bkgd->Integral(1,hist_bkgd->GetNbinsX());
  Double_t *integral = hist_bkgd->GetIntegral(); 
  hist_bkgd->SetContent(integral);
  for (int i = 1; i <= hist_bkgd->GetNbinsX(); i++){
    float bin_content = hist_bkgd->GetBinContent(i);
    hist_bkgd->SetBinContent(i, integ-integ_no_overflow*bin_content);
  }
  hist_bkgd->SetBinContent(hist_bkgd->GetNbinsX()+1, 0); 
  return hist_bkgd;
}
