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

void thesisPlots(){

  //Define Chain
  TChain *chain = new TChain("t"); 
  chain->Add("/home/users/cgeorge/SSAnalysis/outreach/babymaker/outreachbaby.root"); 

  //Define output plots
  float xbins[]  = { 10, 15, 20, 25, 30, 40, 50, 65, 80, 100, 120 }; 
  float xbins2[] = { 40, 50, 60, 80, 100, 150, 200, 300, 450, 700 }; 
  TH1F *elec_eff_numer   = new TH1F("elec_eff_numer"  , "elec_eff_numer"  , 10, xbins); 
  TH1F *elec_eff_numer_l = new TH1F("elec_eff_numer_l", "elec_eff_numer_l", 10, xbins); 
  TH1F *elec_eff_denom   = new TH1F("elec_eff_denom"  , "elec_eff_denom"  , 10, xbins); 
  TH1F *muon_eff_numer   = new TH1F("muon_eff_numer"  , "muon_eff_numer"  , 10, xbins); 
  TH1F *muon_eff_numer_l = new TH1F("muon_eff_numer_l", "muon_eff_numer_l", 10, xbins); 
  TH1F *muon_eff_denom   = new TH1F("muon_eff_denom"  , "muon_eff_denom"  , 10, xbins); 
  TH1F *jets_eff_numer   = new TH1F("jets_eff_numer"  , "jets_eff_numer"  , 9, xbins2); 
  TH1F *jets_eff_numer_l = new TH1F("jets_eff_numer_l", "jets_eff_numer_l", 9, xbins2); 
  TH1F *jets_eff_denom   = new TH1F("jets_eff_denom"  , "jets_eff_denom"  , 9, xbins2); 
  TH1F *elec_pt_accur    = new TH1F("elec_pt_accur"   ,  "elec_pt_accur"  , 50, -0.25, 0.25); 
  TH1F *muon_pt_accur    = new TH1F("muon_pt_accur"   ,  "muon_pt_accur"  , 50, -0.25, 0.25); 
  TH1F *jets_pt_accur    = new TH1F("jets_pt_accur"   ,  "jets_pt_accur"  , 50, -0.25, 0.25); 

  //Do errors properly
  elec_eff_numer->Sumw2();
  elec_eff_denom->Sumw2();
  muon_eff_numer->Sumw2();
  muon_eff_denom->Sumw2();
  jets_eff_numer->Sumw2();
  jets_eff_denom->Sumw2();
  elec_pt_accur->Sumw2();
  muon_pt_accur->Sumw2();
  jets_pt_accur->Sumw2();

  //MVA function
  createAndInitMVA("../CORE", true);

  //Electron fraction
  int elec_0p05_numer = 0;
  int elec_0p05_denom = 0;
  int muon_0p05_numer = 0;
  int muon_0p05_denom = 0;
  int jets_0p05_numer = 0;
  int jets_0p05_denom = 0;
  int jets_0p10_numer = 0;
  int jets_0p10_denom = 0;
  int jets_0p15_numer = 0;
  int jets_0p15_denom = 0;

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

      //If we get here, we have something, fill the denominator
      for (unsigned int i = 0; i < out::genLep_id().size(); i++){
        if (abs(out::genLep_id().at(i)) == 11) elec_eff_denom->Fill(out::genLep_p4().at(i).pt()); 
        if (abs(out::genLep_id().at(i)) == 13) muon_eff_denom->Fill(out::genLep_p4().at(i).pt()); 
      }
      for (unsigned int i = 0; i < out::genJets().size(); i++) jets_eff_denom->Fill(out::genJets().at(i).pt()); 

      //Loose numerator if we reconstruct it at all
      for (unsigned int i = 0; i < out::genLep_id().size(); i++){
        if (out::idx_reco().at(i) >= 0 && abs(out::genLep_id().at(i)) == 11) elec_eff_numer_l->Fill(out::genLep_p4().at(i).pt()); 
        if (out::idx_reco().at(i) >= 0 && abs(out::genLep_id().at(i)) == 13) muon_eff_numer_l->Fill(out::genLep_p4().at(i).pt()); 
      }
      for (unsigned int i = 0; i < out::genJets().size(); i++){
        if (out::genJets_matched().at(i) >= 0) jets_eff_numer_l->Fill(out::genJets().at(i).pt()); 
      }

      //Regular numerator if it passes ID
      for (unsigned int i = 0; i < out::genLep_id().size(); i++){
        if (out::lep_passID().at(i) && abs(out::genLep_id().at(i)) == 11) elec_eff_numer->Fill(out::genLep_p4().at(i).pt()); 
        if (out::lep_passID().at(i) && abs(out::genLep_id().at(i)) == 13) muon_eff_numer->Fill(out::genLep_p4().at(i).pt()); 
      }
      for (unsigned int i = 0; i < out::genJets().size(); i++){
        if (out::genJets_matchedID().at(i) >= 0) jets_eff_numer->Fill(out::genJets().at(i).pt()); 
      }

      //Also get error on reco pT
      for (unsigned int i = 0; i < out::genLep_id().size(); i++){
        if (out::idx_reco().at(i) < 0) continue;
        if (abs(out::genLep_id().at(i)) == 11){
          float reco_pt_accur = (out::recoLep_p4().at(i).pt() - out::genLep_p4().at(i).pt())/out::genLep_p4().at(i).pt();
          if (out::idx_reco().at(i) >= 0) elec_pt_accur->Fill(reco_pt_accur); 
          if (out::idx_reco().at(i) >= 0 && abs(reco_pt_accur) < 0.05) elec_0p05_numer++; 
          if (out::idx_reco().at(i) >= 0) elec_0p05_denom++;
        }
        if (abs(out::genLep_id().at(i)) == 13){
          float reco_pt_accur = (out::recoLep_p4().at(i).pt() - out::genLep_p4().at(i).pt())/out::genLep_p4().at(i).pt();
          if (out::idx_reco().at(i) >= 0) muon_pt_accur->Fill(reco_pt_accur); 
          if (out::idx_reco().at(i) >= 0 && abs(reco_pt_accur) < 0.05) muon_0p05_numer++; 
          if (out::idx_reco().at(i) >= 0) muon_0p05_denom++;
        }
      }
      for (unsigned int i = 0; i < out::genJets().size(); i++){
        if (out::genJets_matched().at(i) < 0) continue;
        float reco_pt_accur = (out::pfjets_p4().at(out::genJets_matched().at(i)).pt() - out::genJets().at(i).pt())/out::genJets().at(i).pt();
        jets_pt_accur->Fill(reco_pt_accur); 
        if (abs(reco_pt_accur) < 0.05) jets_0p05_numer++;
        jets_0p05_denom++;
        if (abs(reco_pt_accur) < 0.10) jets_0p10_numer++;
        jets_0p10_denom++;
        if (abs(reco_pt_accur) < 0.15) jets_0p15_numer++;
        jets_0p15_denom++;
      }
 
    }//event loop

  }//file loop

  //Now take the ratio
  elec_eff_numer->Divide(elec_eff_denom); 
  elec_eff_numer_l->Divide(elec_eff_denom); 
  muon_eff_numer->Divide(muon_eff_denom); 
  muon_eff_numer_l->Divide(muon_eff_denom); 
  jets_eff_numer->Divide(jets_eff_denom); 
  jets_eff_numer_l->Divide(jets_eff_denom); 

  //Elec Eff Plot
  vector <TH1F*> elec_eff_bkgds = { elec_eff_numer, elec_eff_numer_l }; 
  vector <string> titles = { "analysis ID", "no ID" }; 
  vector <Color_t> colors = { kBlue, kRed, kGreen+3 }; 
  TH1F* null = new TH1F("","",1,0,1);
  dataMCplotMaker(null, elec_eff_bkgds, titles, "Electron Signal Efficiency", "", "--outputName elec_eff --yAxisLabel Efficiency --xAxisLabel p_{T}^{gen} --isLinear --noStack --noLumi --drawDots --noOverflow --setMaximum 1.2 --hLine 1.0 --outOfFrame --legendUp -0.4 --noDivisionLabel", std::vector<TH1F*>(), std::vector<string>(), colors); 

  //Muon Eff Plot
  vector <TH1F*> muon_eff_bkgds = { muon_eff_numer, muon_eff_numer_l }; 
  dataMCplotMaker(null, muon_eff_bkgds, titles, "Muon Signal Efficiency", "", "--outputName muon_eff --yAxisLabel Efficiency --xAxisLabel p_{T}^{gen} --isLinear --noStack --noLumi --drawDots --noOverflow --setMaximum 1.2 --hLine 1.0 --outOfFrame --legendUp -0.4 --noDivisionLabel", std::vector<TH1F*>(), std::vector<string>(), colors); 

  //Jets Eff Plot
  vector <TH1F*> jets_eff_bkgds = { jets_eff_numer, jets_eff_numer_l }; 
  dataMCplotMaker(null, jets_eff_bkgds, titles, "Jets Signal Efficiency", "", "--outputName jets_eff --yAxisLabel Efficiency --xAxisLabel p_{T}^{gen} --isLinear --noStack --noLumi --drawDots --noOverflow --setMaximum 1.2 --hLine 1.0 --outOfFrame --legendUp -0.4 --noDivisionLabel", std::vector<TH1F*>(), std::vector<string>(), colors); 

  //Lepton pt accuracy plot
  vector <TH1F*> pt_acc_bkgds = { elec_pt_accur, muon_pt_accur, jets_pt_accur }; 
  vector <string> titles_pt_acc = { "elec", "muon", "jets" }; 
  dataMCplotMaker(null, pt_acc_bkgds, titles_pt_acc, "p_{T} reconstruction accuracy", "", "--outputName pt_acc --yAxisLabel Fraction --xAxisLabel p_{T}^{gen} --isLinear --noStack --normalize --noLumi --setMaximum 0.3 --outOfFrame  --noDivisionLabel --bkgd_width 2", std::vector<TH1F*>(), std::vector<string>(), colors); 
  cout << "Electrons, fraction within 5\%: " << 1.0*elec_0p05_numer/elec_0p05_denom << endl;
  cout << "Muons, fraction within 5\%: " << 1.0*muon_0p05_numer/muon_0p05_denom << endl;
  cout << "Jets, fraction within 5\%: " << 1.0*jets_0p05_numer/jets_0p05_denom << endl;
  cout << "Jets, fraction within 10\%: " << 1.0*jets_0p10_numer/jets_0p10_denom << endl;
  cout << "Jets, fraction within 15\%: " << 1.0*jets_0p15_numer/jets_0p15_denom << endl;

}
