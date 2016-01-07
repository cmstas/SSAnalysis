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

bool noTrigger = false;

void outreachPlots(){

  //Define Chain
  TChain *chain = new TChain("t"); 
  chain->Add("/home/users/cgeorge/SSAnalysis/outreach/babymaker/outreachbaby_ttbar.root"); 

  //Define output plots
  float xbins[] = { 10, 15, 20, 25, 30, 40, 50, 65, 80, 100, 120 }; 
  //float xbins[] = { 10, 25, 120 }; 
  TH1F *elec_numer = new TH1F("elec_numer", "elec_numer", 10, xbins); 
  TH1F *elec_denom = new TH1F("elec_denom", "elec_denom", 10, xbins); 
  TH1F *muon_numer = new TH1F("muon_numer", "muon_numer", 10, xbins); 
  TH1F *muon_denom = new TH1F("muon_denom", "muon_denom", 10, xbins); 
  TH1F *dr_plot    = new TH1F("dr_plot"   , "dr_plot"   , 102, 0, 0.102); 

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

    //Cases
    int case1 = 0;
    int case2 = 0;
    int case3 = 0;
    int case4 = 0;
    int casea = 0;
    int caseb = 0;
    int casec = 0;
    int cased = 0;
    if ( noTrigger) cout << "No trigger requirement!" << endl;
    if (!noTrigger) cout << "Trigger requirement!" << endl;

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
      if (out::ht() < 300) continue;

      //If we get here, we have something, fill the denominator
      if (abs(out::genLep1_id()) == 11){ elec_denom->Fill(out::genLep1_p4().pt()); casea++; } 
      if (abs(out::genLep1_id()) == 13){ muon_denom->Fill(out::genLep1_p4().pt()); caseb++; } 
      if (abs(out::genLep2_id()) == 11){ elec_denom->Fill(out::genLep2_p4().pt()); casec++; } 
      if (abs(out::genLep2_id()) == 13){ muon_denom->Fill(out::genLep2_p4().pt()); cased++; } 

      //Just for convenience, reject these
      if (abs(out::genLep1_id()) != 11 && abs(out::genLep1_id()) != 13) continue;
      if (abs(out::genLep2_id()) != 11 && abs(out::genLep2_id()) != 13) continue;

      //If it passes ID, should go in the numerator
      if ((noTrigger || out::fired_trigger_1()) && out::lep1_passID() && abs(out::genLep1_id()) == 11){ elec_numer->Fill(out::genLep1_p4().pt()); case1++; }
      if ((noTrigger || out::fired_trigger_1()) && out::lep1_passID() && abs(out::genLep1_id()) == 13){ muon_numer->Fill(out::genLep1_p4().pt()); case2++; }
      if ((noTrigger || out::fired_trigger_2()) && out::lep2_passID() && abs(out::genLep2_id()) == 11){ elec_numer->Fill(out::genLep2_p4().pt()); case3++; }
      if ((noTrigger || out::fired_trigger_2()) && out::lep2_passID() && abs(out::genLep2_id()) == 13){ muon_numer->Fill(out::genLep2_p4().pt()); case4++; }

      //delta-R between reco and gen
      dr_plot->Fill(DeltaR(out::genLep1_p4(), out::recoLep1_p4())); 
      dr_plot->Fill(DeltaR(out::genLep2_p4(), out::recoLep2_p4())); 
 
    }//event loop

    cout << 1.0*case1/casea << endl;
    cout << 1.0*case2/caseb << endl;
    cout << 1.0*case3/casec << endl;
    cout << 1.0*case4/cased << endl;

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

  //Now make the dr plot 
  bkgds = { dr_plot }; 
  titles = { "#Delta R" }; 
  dataMCplotMaker(null, bkgds, titles, "#Delta R(gen, reco)", "SS Baseline, T1tttt", "--outputName dr --yAxisLabel Number --xAxisLabel #Delta R --isLinear --drawDots --noOverflow --outOfFrame --noLegend"); 

}
