#include "../software/dataMCplotMaker/PlotMaker2D.h"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../commonUtils.h"
#include "../classFiles/v8.04/SS.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TChain.h"
#include <fstream>
#include "../CORE/IsolationTools.h"
#include "../CORE/SSSelections.h"
#include "../CORE/Tools/dorky/dorky.h"

bool doData = false; 
bool doWjetsOnly = false;
bool doSoup = false;
bool test_MVA = false;
bool test_Laurent = false;
bool doPromptsOnly = false;
bool doExtraElectronPtBin = false;

//Path
string tag = getTag().Data();


TH1D * evtCounter = new TH1D("counter","counter",1000,0,1000); 
std::vector<TString> counterNames;
map<TString, int> evtBinMap;
int evtBin = 0;
void initCounter() {
    for(int ibin = 0; ibin < evtCounter->GetNbinsX(); ibin++) {
        evtCounter->SetBinContent(ibin, 0);
    }
    evtCounter->Sumw2();
    evtBinMap.clear();
    counterNames.clear();
    evtBin = 0;
}
void addToCounter(TString name, double weight=1.0) {
    if(evtBinMap.find(name) == evtBinMap.end() ) {
        evtBinMap[name] = evtBin;
        counterNames.push_back(name);
        evtBin++;
    }
    evtCounter->Fill(evtBinMap[name], weight);
}
void printCounter(bool file = true) {
    ofstream outfile;
    if(file) outfile.open("counter.txt");
    cout << string(30, '-') << endl << "Counter totals: " << endl;
    for(map<TString,int>::iterator it = evtBinMap.begin(); it != evtBinMap.end(); it++) {
        int iBin = (it->second)+1;
        printf("%-15s %6.2f %6.2f\n",
                (it->first).Data(),
                evtCounter->GetBinContent(iBin),
                evtCounter->GetBinError(iBin) );
        if(file) outfile << (it->first).Data() << "  " << evtCounter->GetBinContent(iBin) << "  " << evtCounter->GetBinError(iBin) << endl;
    }
    cout << string(30, '-') << endl;
    if(file) outfile.close();
    if(file) cout << "Wrote counter to counter.txt" << endl;
}


void drawProfile(TH2F* h2, TString title, TString filename) {
    TCanvas *c0 = new TCanvas("c0","c0");
    TProfile *tp = h2->ProfileX();
    tp->SetTitle(title);
    tp->Draw();
    c0->SaveAs(filename);
    // std::cout << "Saved profile as " << filename << std::endl;
}

bool passesMVA(int level, int lep_id, float lep_etaSC, float lep_MVA){
    lep_etaSC = fabs(lep_etaSC);
    if (abs(lep_id) != 11) return true;

    // (0.87, 0.6, 0.17) is tight MVA for numerator/analysis leptons
    // (-0.155, -0.56, -0.76) is MVA for IsolatedFO
    // http://uaf-8.t2.ucsd.edu/~namin/dump/ssan.pdf#page=55
    //    - Isolated trigger denominator: (0.05, -0.26, -0.40)
    //    - Non-isolated trigger denominator: (-0.36,-0.58,-0.62)
    //       - will need to bypass the passIsolatedFO stuff in order to use this since this is looser than that
    if (level == 1) {
        if (lep_etaSC < 0.8) return lep_MVA > -0.155;
        if ((lep_etaSC >= 0.8 && lep_etaSC <= 1.479)) return lep_MVA > -0.56;
        if (lep_etaSC > 1.479) return lep_MVA > -0.76;
    } else if (level == 2) {
        if (lep_etaSC < 0.8) return lep_MVA > 0.87;
        if ((lep_etaSC >= 0.8 && lep_etaSC <= 1.479)) return lep_MVA > 0.60;
        if (lep_etaSC > 1.479) return lep_MVA > 0.17;
    }
    return false;
}

bool AN_MVA(int level, int lep_id, float lep_etaSC, float lep_MVA){
    // level == 1 for Iso and 2 for noniso
    lep_etaSC = fabs(lep_etaSC);
    if (abs(lep_id) != 11) return true;

    // http://uaf-8.t2.ucsd.edu/~namin/dump/ssan.pdf#page=55
    //    - Isolated trigger denominator: (0.05, -0.26, -0.40)
    //    - Non-isolated trigger denominator: (-0.36,-0.58,-0.62)
    //       - will need to bypass the passIsolatedFO stuff in order to use this since this is looser than that
    if (level == 1) {
        if (lep_etaSC < 0.8) return lep_MVA > 0.05;
        if ((lep_etaSC >= 0.8 && lep_etaSC <= 1.479)) return lep_MVA > -0.26;
        if (lep_etaSC > 1.479) return lep_MVA > -0.40;
    } else if (level == 2) {
        if (lep_etaSC < 0.8) return lep_MVA > -0.36;
        if ((lep_etaSC >= 0.8 && lep_etaSC <= 1.479)) return lep_MVA > -0.58;
        if (lep_etaSC > 1.479) return lep_MVA > -0.62;
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

void handleOverflow(TH2D* h2) {
    int nx = h2->GetNbinsX();
    for (unsigned int i = 1; i < 4; i++){ // <--- GRRR. ROOT starts bin #s at 1 (0 is underflow)
        h2->SetBinContent(nx, i, h2->GetBinContent(nx , i) + h2->GetBinContent(nx+1 , i)); 
        h2->SetBinError(nx, i, sqrt(pow(h2->GetBinError(nx,i),2) + pow(h2->GetBinError(nx+1,i),2))); 
    }
}

void draw1DNumerDenom(TH1F* numer, TH1F* denom, TString fname) {
  TCanvas *ctmp = new TCanvas("temp","temp");

  denom->SetLineColor(kBlack);
  denom->Draw("hist");
  numer->SetLineColor(kRed);
  numer->Draw("pesame");

  ctmp->SaveAs(fname);


}

void FR(int doHighHT=-1, TString opts=""){

    doData = opts.Contains("doData");
    doWjetsOnly = opts.Contains("doWjetsOnly");
    doSoup = opts.Contains("doSoup");
    test_MVA = opts.Contains("test_MVA");
    test_Laurent = opts.Contains("test_Laurent");
    doPromptsOnly = opts.Contains("doPromptsOnly");
    doExtraElectronPtBin = opts.Contains("doExtraElectronPtBin");

    initCounter();

    ofstream textfile;
    textfile.open("unsorted.txt");

  //Lumi
  float luminosity = doData ? getLumi() : getLumi();

  std::cout << "Using lumi = " << luminosity << "/fb" << std::endl;
  std::cout << "Using " << tag << " babies" << std::endl;

  //Declare hists
  TH2D *numer_elec_2D_sipgt4; 
  TH2D *denom_elec_2D_sipgt4; 
  TH2D *numer_elec_2D_siplt4; 
  TH2D *denom_elec_2D_siplt4; 
  TH2D *numer_muon_2D_sipgt4; 
  TH2D *denom_muon_2D_sipgt4; 
  TH2D *numer_muon_2D_siplt4; 
  TH2D *denom_muon_2D_siplt4; 

  //Define hists
  int nBinsX_el = 5; 
  int nBinsY_el = 3; 
  float xbins_el[] = { 10, 15, 25, 35, 50, 70 }; 
  float xbins_el_extr[] = { 10, 15, 25, 35, 50, 70, 90 }; 
  float ybins_el[] = {0., 0.8, 1.479, 2.5};
  int nBinsX_mu = 5; 
  int nBinsY_mu = 3; 
  float xbins_mu[] = { 10, 15, 25, 35, 50, 70 }; 
  float ybins_mu[] = {0., 1.2, 2.1, 2.4};

  if(doExtraElectronPtBin) {
      numer_elec_2D_sipgt4  = new TH2D("numer_elec_2D_sipgt4" , "numer_elec_2D_sipgt4" , nBinsX_el+1, xbins_el_extr, nBinsY_el, ybins_el);
      denom_elec_2D_sipgt4  = new TH2D("denom_elec_2D_sipgt4" , "denom_elec_2D_sipgt4" , nBinsX_el+1, xbins_el_extr, nBinsY_el, ybins_el);
      numer_elec_2D_siplt4 = new TH2D("numer_elec_2D_siplt4", "numer_elec_2D_siplt4",    nBinsX_el+1, xbins_el_extr, nBinsY_el, ybins_el);
      denom_elec_2D_siplt4 = new TH2D("denom_elec_2D_siplt4", "denom_elec_2D_siplt4",    nBinsX_el+1, xbins_el_extr, nBinsY_el, ybins_el);
  } else {
      numer_elec_2D_sipgt4  = new TH2D("numer_elec_2D_sipgt4" , "numer_elec_2D_sipgt4" , nBinsX_el, xbins_el, nBinsY_el, ybins_el);
      denom_elec_2D_sipgt4  = new TH2D("denom_elec_2D_sipgt4" , "denom_elec_2D_sipgt4" , nBinsX_el, xbins_el, nBinsY_el, ybins_el);
      numer_elec_2D_siplt4 = new TH2D("numer_elec_2D_siplt4", "numer_elec_2D_siplt4",    nBinsX_el, xbins_el, nBinsY_el, ybins_el);
      denom_elec_2D_siplt4 = new TH2D("denom_elec_2D_siplt4", "denom_elec_2D_siplt4",    nBinsX_el, xbins_el, nBinsY_el, ybins_el);
  }
  numer_muon_2D_sipgt4 = new TH2D("numer_muon_2D_sipgt4", "numer_muon_2D_sipgt4",    nBinsX_mu, xbins_mu, nBinsY_mu, ybins_mu);
  denom_muon_2D_sipgt4 = new TH2D("denom_muon_2D_sipgt4", "denom_muon_2D_sipgt4",    nBinsX_mu, xbins_mu, nBinsY_mu, ybins_mu);
  numer_muon_2D_siplt4 = new TH2D("numer_muon_2D_siplt4", "numer_muon_2D_siplt4",    nBinsX_mu, xbins_mu, nBinsY_mu, ybins_mu);
  denom_muon_2D_siplt4 = new TH2D("denom_muon_2D_siplt4", "denom_muon_2D_siplt4",    nBinsX_mu, xbins_mu, nBinsY_mu, ybins_mu);

  TH1F *numer_vs_sip_el  = new TH1F("numer_vs_sip_el" , "numer_vs_sip_el" , 16,0,12);
  TH1F *denom_vs_sip_el  = new TH1F("denom_vs_sip_el" , "denom_vs_sip_el" , 16,0,12);
  TH1F *numer_vs_sip_mu  = new TH1F("numer_vs_sip_mu" , "numer_vs_sip_mu" , 16,0,12);
  TH1F *denom_vs_sip_mu  = new TH1F("denom_vs_sip_mu" , "denom_vs_sip_mu" , 16,0,12);

  TH1F *numer_vs_pt_el  = new TH1F("numer_vs_pt_el" , "numer, denom vs pt el (sip>4)" , 20,0,200);
  TH1F *denom_vs_pt_el  = new TH1F("denom_vs_pt_el" , "numer, denom vs pt el (sip>4)" , 20,0,200);
  TH1F *numer_vs_pt_mu  = new TH1F("numer_vs_pt_mu" , "numer, denom vs pt mu (sip>4)" , 20,0,200);
  TH1F *denom_vs_pt_mu  = new TH1F("denom_vs_pt_mu" , "numer, denom vs pt mu (sip>4)" , 20,0,200);
  TH1F *numer_vs_eta_el  = new TH1F("numer_vs_eta_el" , "numer, denom vs eta el (sip>4)" , 20,0,2.5);
  TH1F *denom_vs_eta_el  = new TH1F("denom_vs_eta_el" , "numer, denom vs eta el (sip>4)" , 20,0,2.5);
  TH1F *numer_vs_eta_mu  = new TH1F("numer_vs_eta_mu" , "numer, denom vs eta mu (sip>4)" , 20,0,2.5);
  TH1F *denom_vs_eta_mu  = new TH1F("denom_vs_eta_mu" , "numer, denom vs eta mu (sip>4)" , 20,0,2.5);

  TH2F *miniiso_vs_sip_el  = new TH2F("miniiso_vs_sip_el" , "miniiso_vs_sip_el" , 16,0,8, 20,0,0.5);
  TH2F *ptrel_vs_sip_el  = new TH2F("ptrel_vs_sip_el" , "ptrel_vs_sip_el" , 16,0,8, 30,0,30);
  TH2F *ptratio_vs_sip_el  = new TH2F("ptratio_vs_sip_el" , "ptratio_vs_sip_el" , 16,0,8, 20,0.6,1.2);
  TH2F *multiiso_vs_sip_el  = new TH2F("multiiso_vs_sip_el" , "multiiso_vs_sip_el" , 16,0,8, 2,-0.5,1.5);

  TH2F *miniiso_vs_sip_mu  = new TH2F("miniiso_vs_sip_mu" , "miniiso_vs_sip_mu" , 16,0,8, 20,0,0.5);
  TH2F *ptrel_vs_sip_mu  = new TH2F("ptrel_vs_sip_mu" , "ptrel_vs_sip_mu" , 16,0,8, 30,0,30);
  TH2F *ptratio_vs_sip_mu  = new TH2F("ptratio_vs_sip_mu" , "ptratio_vs_sip_mu" , 16,0,8, 20,0.6,1.2);
  TH2F *multiiso_vs_sip_mu  = new TH2F("multiiso_vs_sip_mu" , "multiiso_vs_sip_mu" , 16,0,8, 2,-0.5,1.5);

  TH2F *ptratio_vs_miniiso_mu  = new TH2F("ptratio_vs_miniiso_mu" , "ptratio_vs_miniiso_mu" , 40,0,0.4,20,1.0,3.0);
  TH2F *ptratio_vs_miniiso_el  = new TH2F("ptratio_vs_miniiso_el" , "ptratio_vs_miniiso_el" , 40,0,0.4,20,1.0,3.0);

  TH2F *mva_vs_aeta_true  = new TH2F("mva_vs_aeta_true" , "mva_vs_aeta_true" , 10,0,2.5, 20,-1,1);
  TH2F *mva_vs_aeta_fake  = new TH2F("mva_vs_aeta_fake" , "mva_vs_aeta_fake" , 10,0,2.5, 20,-1,1);

  //Set hist errors
  numer_elec_2D_sipgt4->Sumw2();
  denom_elec_2D_sipgt4->Sumw2();
  numer_elec_2D_siplt4->Sumw2();
  denom_elec_2D_siplt4->Sumw2();
  numer_muon_2D_sipgt4->Sumw2();
  denom_muon_2D_sipgt4->Sumw2();
  numer_muon_2D_siplt4->Sumw2();
  denom_muon_2D_siplt4->Sumw2();

  // TString base="./skims_Dec2/";
  TString base="./skims_Jan25/";

  //Declare chain
  TChain *chain = new TChain("t");
  if (doData){
    duplicate_removal::clear_list();
    // chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/Data*.root"  , tag.c_str()));
    // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04_trigsafe_v4/Data*.root");
    // chain->Add("Data_skim.root");
    // chain->Add(base+"Data_22p0ifb_skim.root");
    chain->Add(base+"Data_skim.root");
  }
  else if (!doData){

    // chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR_PH.root", tag.c_str()));
    // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04_trigsafe/TTBAR_PH*.root");
    // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04_trigsafe_v2/TTBAR_PH*.root");

      if(doWjetsOnly) {
          // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04_trigsafe_v4/WJets*To*.root");
          chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/WJets*.root");
      } else if(doSoup) {
          // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04_trigsafe_v4/WJets*To*.root");
          // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04_trigsafe_v3/TTBAR_PH*.root");
          // chain->Add(base+"WJets_skim.root");
          chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/WJets*.root");
          chain->Add(base+"TTBAR_PH_skim.root");
      } else {
          // chain->Add("/nfs-7/userdata/ss2015/ssBabies/v8.04_trigsafe_v3/TTBAR_PH*.root");
          chain->Add(base+"TTBAR_PH_skim.root");
      }

    // chain->Add("/hadoop/cms/store/user/namin/condor/ss_babies_Aug25/ttbar_ph_1282.root");
    // chain->Add("/home/users/namin/2016/ss/master/SSAnalysis/batch/ttbar_ph_1282.root");

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
    TString filename = currentFile->GetTitle();
    TFile *file = new TFile(filename);
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

      float pt1 = ss::lep1_coneCorrPt(); 
      float pt2 = ss::lep2_coneCorrPt(); 

      float weight = 1;
      if(!ss::is_real_data()) weight *= ss::scale1fb()*luminosity;

      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //Event Selection
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), pt1, pt2);
      if (BR < 0) continue;

      bool verbose = false;

      // if (ss::lumi() != 144401 || ss::event() != 23132887) continue;
      // if (ss::lumi() != 204117 || ss::event() != 32699566) continue;
      // if (ss::lumi() != 228769 || ss::event() != 36648858) continue;
      // if (ss::lumi() != 247804 || ss::event() != 39698163) continue;
      // if (ss::lumi() != 319965 || ss::event() != 51258393) continue;
      // if (ss::lumi() != 402409 || ss::event() != 64465899) continue;
      // if (ss::lumi() != 447023 || ss::event() != 71613202) continue;
      // if (ss::lumi() != 477223 || ss::event() != 76451090) continue;


      if (doWjetsOnly || (doSoup && filename.Contains("WJets"))) {
          if (ss::hyp_class() == 6) continue;
      } else {
          if (ss::hyp_class() == 4 || ss::hyp_class() == 6) continue;
          // lep1_pt lep1_id lep2_pt lep2_id lep1_eta lep2_eta lep1_passes_id lep2_passes_id zmass
          if (ss::hyp_class() == 7) continue;
      }

      // recompute these since I will need to now modify one of the cuts in the denominator
      bool inSituFR_id_lep1 = true;
      bool inSituFR_id_lep2 = true;


      if(abs(ss::lep1_id()) == 11) inSituFR_id_lep1 = fabs(ss::lep1_el_etaSC())<2.5 &&  ss::lep1_el_conv_vtx_flag() == 1 &&  ss::lep1_el_threeChargeAgree() == 1 &&  ss::lep1_el_exp_innerlayers() == 1 &&  ss::lep1_dZ() < 0.1;
      else inSituFR_id_lep1 =  ss::lep1_dZ() < 0.1 && ss::lep1_mu_ptErr() && ss::lep1_mediumMuonPOG() &&  fabs(ss::lep1_p4().eta()) < 2.4;
      if(abs(ss::lep2_id()) == 11) inSituFR_id_lep2 = fabs(ss::lep2_el_etaSC())<2.5 &&  ss::lep2_el_conv_vtx_flag() == 1 &&  ss::lep2_el_threeChargeAgree() == 1 &&  ss::lep2_el_exp_innerlayers() == 1 &&  ss::lep2_dZ() < 0.1;
      else inSituFR_id_lep2 =  ss::lep2_dZ() < 0.1 && ss::lep2_mu_ptErr() && ss::lep2_mediumMuonPOG() &&  fabs(ss::lep2_p4().eta()) < 2.4;

      if (verbose) std::cout << " ss::hyp_class(): " << ss::hyp_class() << std::endl;
      if (verbose) std::cout << " inSituFR_id_lep1: " << inSituFR_id_lep1 << " inSituFR_id_lep2: " << inSituFR_id_lep2 << std::endl;

      // If either one of them fails the inSituFR id, then bye bye. If we aren't hyp_class=3, then this forces us to be 5
      // if (!ss::passed_id_inSituFR_lep1() || !ss::passed_id_inSituFR_lep2()) continue;
      if (!inSituFR_id_lep1 || !inSituFR_id_lep2) continue; // recomputed versions of variables above




      //Non-Isolated trigger requirements
      // if (ss::is_real_data() && !ss::fired_trigger()) continue; 
      if (!ss::fired_trigger()) continue; 

      if(abs(ss::lep1_id()) == 11) {
          if (isGoodLeg(1)) mva_vs_aeta_true->Fill(abs(ss::lep1_el_etaSC()), ss::lep1_MVA());
          if (isGoodLeg(2)) mva_vs_aeta_fake->Fill(abs(ss::lep1_el_etaSC()), ss::lep1_MVA());
      }
      if(abs(ss::lep2_id()) == 11) {
          if (isGoodLeg(2)) mva_vs_aeta_true->Fill(abs(ss::lep2_el_etaSC()), ss::lep2_MVA());
          if (isGoodLeg(1)) mva_vs_aeta_fake->Fill(abs(ss::lep2_el_etaSC()), ss::lep2_MVA());
      }


      if(!test_MVA) {
          if (ss::ht()<300.) {
              if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA(), ss::lep1_p4().pt())) continue;
              if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA(), ss::lep2_p4().pt())) continue;
          } 

          // if (!passesMVA(2, ss::lep1_id(), ss::lep1_el_etaSC(), ss::lep1_MVA())) continue;
          // if (!passesMVA(2, ss::lep2_id(), ss::lep2_el_etaSC(), ss::lep2_MVA())) continue;
          if (!passesNumeratorMVA(ss::lep1_id(), ss::lep1_el_etaSC(), ss::lep1_MVA(), ss::lep1_p4().pt())) continue;
          if (!passesNumeratorMVA(ss::lep2_id(), ss::lep2_el_etaSC(), ss::lep2_MVA(), ss::lep2_p4().pt())) continue;

      } else {
          if (ss::ht()<300.) { // isolated
              if(!AN_MVA(1, ss::lep1_id(), ss::lep1_el_etaSC(), ss::lep1_MVA())) continue;
              if(!AN_MVA(1, ss::lep2_id(), ss::lep2_el_etaSC(), ss::lep2_MVA())) continue;
          } else { // non isolated
              if(!AN_MVA(2, ss::lep1_id(), ss::lep1_el_etaSC(), ss::lep1_MVA())) continue;
              if(!AN_MVA(2, ss::lep2_id(), ss::lep2_el_etaSC(), ss::lep2_MVA())) continue;
          }
      }

      // if doHighHT is -1 (default), then don't do anything -- inclusive
      if(doHighHT == 0 && ss::ht() > 300.) continue;
      if(doHighHT == 1 && ss::ht() < 300.) continue;

      // //SS Z veto
      // if (fabs((ss::lep1_p4() + ss::lep2_p4()).M() - 91) < 15) continue;


      float miniiso_1 = ss::lep1_miniIso();
      float miniiso_2 = ss::lep2_miniIso();
      float ptrel_1   = ss::lep1_ptrel_v1();
      float ptrel_2   = ss::lep2_ptrel_v1();
      float ptratio_1 = ss::lep1_closeJet().pt() > 0 ? ss::lep1_p4().pt()/ss::lep1_closeJet().pt() : 1.0; 
      float ptratio_2 = ss::lep2_closeJet().pt() > 0 ? ss::lep2_p4().pt()/ss::lep2_closeJet().pt() : 1.0; 

      //Various variables
      float ptrel_cut_1    = (abs(ss::lep1_id()) == 11 ? 7.20 : 7.20);
      float ptrel_cut_2    = (abs(ss::lep2_id()) == 11 ? 7.20 : 7.20);
      float ptratio_cut_1  = (abs(ss::lep1_id()) == 11 ? 0.80 : 0.76);
      float ptratio_cut_2  = (abs(ss::lep2_id()) == 11 ? 0.80 : 0.76);
      if(test_Laurent) {
          ptratio_cut_1  = 0.7;
          ptratio_cut_2  = 0.7;
      }
      bool lep1_denom_iso  = (miniiso_1 < 0.4) && ((ptrel_1 > ptrel_cut_1) || ((1.0/ptratio_1) < (1.0/ptratio_cut_1 + miniiso_1)));
      bool lep2_denom_iso  = (miniiso_2 < 0.4) && ((ptrel_2 > ptrel_cut_2) || ((1.0/ptratio_2) < (1.0/ptratio_cut_2 + miniiso_2)));
      // bool lep1_denom_iso  = (miniiso_1 < 0.4); // && ((ptrel_1 > ptrel_cut_1) || ((1.0/ptratio_1) < (1.0/ptratio_cut_1 + miniiso_1)));
      // bool lep2_denom_iso  = (miniiso_2 < 0.4); // && ((ptrel_2 > ptrel_cut_2) || ((1.0/ptratio_2) < (1.0/ptratio_cut_2 + miniiso_2)));

      // The if statements are getting kind of long now, so shorten some things
      bool lep1_prompt = doPromptsOnly ? (isGoodLeg(1) && isGoodLeg(2)) : (isGoodLeg(1) && isFakeLeg(2));
      bool lep2_prompt = doPromptsOnly ? (isGoodLeg(2) && isGoodLeg(1)) : (isGoodLeg(2) && isFakeLeg(1));
      bool lep1_elec = abs(ss::lep1_id()) == 11;
      bool lep2_elec = abs(ss::lep2_id()) == 11;
      bool lep1_passes_id = ss::lep1_passes_id();
      bool lep2_passes_id = ss::lep2_passes_id();
      float lep1_sip = ss::lep1_sip();
      float lep2_sip = ss::lep2_sip();
      bool lep1_sip_good = lep1_sip < 4;
      bool lep2_sip_good = lep2_sip < 4;

      // Finally we can combine things together
      bool denom1 = lep2_prompt && lep2_passes_id && lep1_denom_iso;
      bool denom2 = lep1_prompt && lep1_passes_id && lep2_denom_iso; 

      bool numer1 = ss::lep1_multiIso() && (fabs(ss::lep1_dxyPV()) < 0.05);
      bool numer2 = ss::lep2_multiIso() && (fabs(ss::lep2_dxyPV()) < 0.05);

      if (verbose) std::cout << " lep1_elec: " << lep1_elec << " lep2_elec: " << lep2_elec << " lep1_prompt: " << lep1_prompt << " lep2_prompt: " << lep2_prompt << " denom1: " << denom1 << " denom2: " << denom2 << " numer1: " << numer1 << " numer2: " << numer2 << std::endl;
      if (verbose) std::cout << " lep1_sip: " << lep1_sip << " lep2_sip: " << lep2_sip << " lep1_passes_id: " << lep1_passes_id << " lep2_passes_id: " << lep2_passes_id << std::endl;
      if (verbose) std::cout << " pt1: " << pt1 << " pt2: " << pt2 << " fabs(ss::lep1_p4().eta()): " << fabs(ss::lep1_p4().eta()) << " fabs(ss::lep2_p4().eta()): " << fabs(ss::lep2_p4().eta()) << std::endl;


      // // Check how many events are charge flips
      // bool isCF1 = ss::lep1_id() == -ss::lep1_mc_id();
      // bool isCF2 = ss::lep2_id() == -ss::lep2_mc_id();

    if ((denom1 && !lep1_sip_good) || (denom2 && !lep2_sip_good)) {
      textfile << Form("%1d %9d %llu %1d %1d %1d %1d\n", 
          ss::run(), ss::lumi(), ss::event(), 
          (denom1 && !lep1_sip_good),
          (denom1 && !lep1_sip_good && numer1),
          (denom2 && !lep2_sip_good),
          (denom2 && !lep2_sip_good && numer2)
          );
    }

      //histo1 is for electrons with SIP > 4
      if (lep1_elec && denom1 && !lep1_sip_good){

        addToCounter("DENOM EL");
        if (numer1) {
            addToCounter("NUMER EL");
            // if (isCF1) addToCounter("CF: NUMER EL");
            numer_elec_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_el->Fill(lep1_sip, weight);
            numer_vs_pt_el->Fill(pt1, weight);
            numer_vs_eta_el->Fill(fabs(ss::lep1_p4().eta()), weight);
        }
        ptratio_vs_miniiso_el->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_elec_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_el->Fill(lep1_sip, weight);
        denom_vs_pt_el->Fill(pt1, weight);
        denom_vs_eta_el->Fill(fabs(ss::lep1_p4().eta()), weight);

        miniiso_vs_sip_el->Fill(lep1_sip, miniiso_1, weight);
        ptrel_vs_sip_el->Fill(lep1_sip, ptrel_1, weight);
        ptratio_vs_sip_el->Fill(lep1_sip, ptratio_1, weight);
        multiiso_vs_sip_el->Fill(lep1_sip, ss::lep1_multiIso(), weight);

      }
      if (lep2_elec && denom2 && !lep2_sip_good){
        addToCounter("DENOM EL");
        if (numer2) {
            addToCounter("NUMER EL");
            // if (isCF2) addToCounter("CF: NUMER EL");
            numer_elec_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_el->Fill(lep2_sip, weight);
            numer_vs_pt_el->Fill(pt2, weight);
            numer_vs_eta_el->Fill(fabs(ss::lep2_p4().eta()), weight);
        }
        ptratio_vs_miniiso_el->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_elec_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_el->Fill(lep2_sip, weight);
        denom_vs_pt_el->Fill(pt2, weight);
        denom_vs_eta_el->Fill(fabs(ss::lep2_p4().eta()), weight);

        miniiso_vs_sip_el->Fill(lep2_sip, miniiso_2, weight);
        ptrel_vs_sip_el->Fill(lep2_sip, ptrel_2, weight);
        ptratio_vs_sip_el->Fill(lep2_sip, ptratio_2, weight);
        multiiso_vs_sip_el->Fill(lep2_sip, ss::lep2_multiIso(), weight);

      }

      //histo2 is for electrons with SIP < 4
      if (lep1_elec && denom1 && lep1_sip_good){
        if (numer1) {
            numer_elec_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_el->Fill(lep1_sip, weight);
            if (fabs(ss::lep1_p4().eta()) < 0.8 && pt1>70) {
                addToCounter("pteta1 NUMER EL");
                // addToCounter(Form("%i:%llu",ss::lumi(), ss::event()));
            }
        }
        if (fabs(ss::lep1_p4().eta()) < 0.8 && pt1>70) addToCounter("pteta1 DENOM EL");
        ptratio_vs_miniiso_el->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_elec_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_el->Fill(lep1_sip, weight);

        miniiso_vs_sip_el->Fill(lep1_sip, miniiso_1, weight);
        ptrel_vs_sip_el->Fill(lep1_sip, ptrel_1, weight);
        ptratio_vs_sip_el->Fill(lep1_sip, ptratio_1, weight);
        multiiso_vs_sip_el->Fill(lep1_sip, ss::lep1_multiIso(), weight);

      }
      if (lep2_elec && denom2 && lep2_sip_good){
        if (numer2) {
            numer_elec_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_el->Fill(lep2_sip, weight);
            if (fabs(ss::lep2_p4().eta()) < 0.8 && pt2>70) addToCounter("pteta2 NUMER EL");
        }
        if (fabs(ss::lep2_p4().eta()) < 0.8 && pt2>70) addToCounter("pteta2 DENOM EL");
        ptratio_vs_miniiso_el->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_elec_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_el->Fill(lep2_sip, weight);

        miniiso_vs_sip_el->Fill(lep2_sip, miniiso_2, weight);
        ptrel_vs_sip_el->Fill(lep2_sip, ptrel_2, weight);
        ptratio_vs_sip_el->Fill(lep2_sip, ptratio_2, weight);
        multiiso_vs_sip_el->Fill(lep2_sip, ss::lep2_multiIso(), weight);

      }

      //histo3 is for muons with SIP > 4 
      if (!lep1_elec && denom1 && !lep1_sip_good){
        addToCounter("DENOM MU");
        if (numer1) {
            addToCounter("NUMER MU");
            // if (isCF1) addToCounter("CF: NUMER MU");
            numer_muon_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(lep1_sip, weight);
            numer_vs_pt_mu->Fill(pt1, weight);
            numer_vs_eta_mu->Fill(fabs(ss::lep1_p4().eta()), weight);
        }
        ptratio_vs_miniiso_mu->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_muon_2D_sipgt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_mu->Fill(lep1_sip, weight);
        denom_vs_pt_mu->Fill(pt1, weight);
        denom_vs_eta_mu->Fill(fabs(ss::lep1_p4().eta()), weight);

        miniiso_vs_sip_mu->Fill(lep1_sip, miniiso_1, weight);
        ptrel_vs_sip_mu->Fill(lep1_sip, ptrel_1, weight);
        ptratio_vs_sip_mu->Fill(lep1_sip, ptratio_1, weight);
        multiiso_vs_sip_mu->Fill(lep1_sip, ss::lep1_multiIso(), weight);

      }
      if (!lep2_elec && denom2 && !lep2_sip_good){
        addToCounter("DENOM MU");
        if (numer2) {
            addToCounter("NUMER MU");
            // if (isCF2) addToCounter("CF: NUMER MU");
            numer_muon_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(lep2_sip, weight);
            numer_vs_pt_mu->Fill(pt2, weight);
            numer_vs_eta_mu->Fill(fabs(ss::lep2_p4().eta()), weight);
        }
        ptratio_vs_miniiso_mu->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_muon_2D_sipgt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_mu->Fill(lep2_sip, weight);
        denom_vs_pt_mu->Fill(pt2, weight);
        denom_vs_eta_mu->Fill(fabs(ss::lep2_p4().eta()), weight);

        miniiso_vs_sip_mu->Fill(lep2_sip, miniiso_2, weight);
        ptrel_vs_sip_mu->Fill(lep2_sip, ptrel_2, weight);
        ptratio_vs_sip_mu->Fill(lep2_sip, ptratio_2, weight);
        multiiso_vs_sip_mu->Fill(lep2_sip, ss::lep2_multiIso(), weight);

      }

      //histo4 is for muons with SIP < 4 
      if (!lep1_elec && denom1 && lep1_sip_good){
        if (numer1) {
            numer_muon_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(lep1_sip, weight);
            if (fabs(ss::lep1_p4().eta()) < 1.2 && pt1>50) addToCounter("pteta1 NUMER MU");
        }
        if (fabs(ss::lep1_p4().eta()) < 1.2 && pt1>50) addToCounter("pteta1 DENOM MU");
        ptratio_vs_miniiso_mu->Fill(miniiso_1, 1.0/ptratio_1);  
        denom_muon_2D_siplt4->Fill(pt1, fabs(ss::lep1_p4().eta()), weight);
        denom_vs_sip_mu->Fill(lep1_sip, weight);

        miniiso_vs_sip_mu->Fill(lep1_sip, miniiso_1, weight);
        ptrel_vs_sip_mu->Fill(lep1_sip, ptrel_1, weight);
        ptratio_vs_sip_mu->Fill(lep1_sip, ptratio_1, weight);
        multiiso_vs_sip_mu->Fill(lep1_sip, ss::lep1_multiIso(), weight);

      }
      if (!lep2_elec && denom2 && lep2_sip_good){
        if (numer2) {
            numer_muon_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);  
            numer_vs_sip_mu->Fill(lep2_sip, weight);
            if (fabs(ss::lep2_p4().eta()) < 1.2 && pt2>50) {
                addToCounter("pteta2 NUMER MU");
                // addToCounter(Form("%i:%llu",ss::lumi(), ss::event()));
            }
        }
        if (fabs(ss::lep2_p4().eta()) < 1.2 && pt2>50) addToCounter("pteta2 DENOM MU");
        ptratio_vs_miniiso_mu->Fill(miniiso_2, 1.0/ptratio_2);  
        denom_muon_2D_siplt4->Fill(pt2, fabs(ss::lep2_p4().eta()), weight);
        denom_vs_sip_mu->Fill(lep2_sip, weight);
        
        miniiso_vs_sip_mu->Fill(lep2_sip, miniiso_2, weight);
        ptrel_vs_sip_mu->Fill(lep2_sip, ptrel_2, weight);
        ptratio_vs_sip_mu->Fill(lep2_sip, ptratio_2, weight);
        multiiso_vs_sip_mu->Fill(lep2_sip, ss::lep2_multiIso(), weight);

      }

    }//event loop

    // samesign.DumpActivatedBranches(tree);
  }//file loop


    handleOverflow(denom_elec_2D_sipgt4);
    handleOverflow(numer_elec_2D_sipgt4);
    handleOverflow(denom_elec_2D_siplt4);
    handleOverflow(numer_elec_2D_siplt4);
    handleOverflow(denom_muon_2D_sipgt4);
    handleOverflow(numer_muon_2D_sipgt4);
    handleOverflow(denom_muon_2D_siplt4);
    handleOverflow(numer_muon_2D_siplt4);

  string pfx = "all";
  string desc = "All H_{T}";
  if(doHighHT == 0) { pfx = "lowht"; desc = "Low H_{T}"; }
  if(doHighHT == 1) { pfx = "highht"; desc = "High H_{T}"; }

  // plot numer and denom before dividing
  PlotMaker2D(numer_elec_2D_sipgt4 , Form("--outputName plots/%s_numer_FR_elec_sipgt4.pdf --noOverflow --setTitle elec %s numer (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(numer_muon_2D_sipgt4, Form("--outputName plots/%s_numer_FR_muon_sipgt4.pdf --noOverflow --setTitle muon %s numer (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(denom_elec_2D_sipgt4 , Form("--outputName plots/%s_denom_FR_elec_sipgt4.pdf --noOverflow --setTitle elec %s denom (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(denom_muon_2D_sipgt4, Form("--outputName plots/%s_denom_FR_muon_sipgt4.pdf --noOverflow --setTitle muon %s denom (sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 

  // plot numer and denom before dividing
  PlotMaker2D(numer_elec_2D_siplt4 , Form("--outputName plots/%s_numer_FR_elec_siplt4.pdf --noOverflow --setTitle elec %s numer (sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(numer_muon_2D_siplt4, Form("--outputName plots/%s_numer_FR_muon_siplt4.pdf --noOverflow --setTitle muon %s numer (sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(denom_elec_2D_siplt4 , Form("--outputName plots/%s_denom_FR_elec_siplt4.pdf --noOverflow --setTitle elec %s denom (sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  PlotMaker2D(denom_muon_2D_siplt4, Form("--outputName plots/%s_denom_FR_muon_siplt4.pdf --noOverflow --setTitle muon %s denom (sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 


  string pfx2 = "";
  if (doPromptsOnly) pfx2 = "prompt_";
  else if (doData) pfx2 = "data_";
  TFile *file = new TFile(Form("%s%s_inSituFR_cone_FR_histos.root", pfx2.c_str(), pfx.c_str()), "RECREATE");
  file->Write(); 

  // write out numer and denom before dividing
  numer_elec_2D_sipgt4->Write("numer_elec_sipgt4");
  numer_muon_2D_sipgt4->Write("numer_muon_sipgt4");
  numer_elec_2D_siplt4->Write("numer_elec_siplt4");
  numer_muon_2D_siplt4->Write("numer_muon_siplt4");
  denom_elec_2D_sipgt4->Write("denom_elec_sipgt4");
  denom_muon_2D_sipgt4->Write("denom_muon_sipgt4");
  denom_elec_2D_siplt4->Write("denom_elec_siplt4");
  denom_muon_2D_siplt4->Write("denom_muon_siplt4");

  //Divide numer/denom
  numer_elec_2D_sipgt4->Divide(numer_elec_2D_sipgt4, denom_elec_2D_sipgt4, 1, 1, "b"); 
  numer_elec_2D_siplt4->Divide(numer_elec_2D_siplt4, denom_elec_2D_siplt4, 1, 1, "b"); 
  numer_muon_2D_sipgt4->Divide(numer_muon_2D_sipgt4, denom_muon_2D_sipgt4, 1, 1, "b"); 
  numer_muon_2D_siplt4->Divide(numer_muon_2D_siplt4, denom_muon_2D_siplt4, 1, 1, "b"); 


  numer_elec_2D_sipgt4->Write("elec");
  numer_muon_2D_sipgt4->Write("muon");
  numer_elec_2D_siplt4->Write("elec_siplt4");
  numer_muon_2D_siplt4->Write("muon_siplt4");
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
  if(doWjetsOnly) {
      PlotMaker2D(numer_elec_2D_siplt4, Form("--outputName plots/%s_FR_elec_sipratio.pdf --noOverflow --zMax 1.5 --zMin 0.0 --setTitle elec %s FR(sip3D < 4)/FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
      PlotMaker2D(numer_muon_2D_siplt4, Form("--outputName plots/%s_FR_muon_sipratio.pdf --noOverflow --zMax 1.5 --zMin 0.0 --setTitle muon %s FR(sip3D < 4)/FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  } else {
      PlotMaker2D(numer_elec_2D_siplt4, Form("--outputName plots/%s_FR_elec_sipratio.pdf --noOverflow --zMax 1.4 --zMin 0.75 --setTitle elec %s FR(sip3D < 4)/FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
      PlotMaker2D(numer_muon_2D_siplt4, Form("--outputName plots/%s_FR_muon_sipratio.pdf --noOverflow --zMax 1.4 --zMin 0.75 --setTitle muon %s FR(sip3D < 4)/FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", pfx.c_str(), desc.c_str())); 
  }

  // Isolation vs vs sip3D
  PlotMaker2D(miniiso_vs_sip_el, Form("  --outputName plots/%s_miniiso_vs_sip_el.pdf  --noOverflow --setTitle %s miniiso vs sip (elec)  --Xaxis sip3D --Yaxis miniiso  --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptrel_vs_sip_el, Form("    --outputName plots/%s_ptrel_vs_sip_el.pdf    --noOverflow --setTitle %s ptrel vs sip (elec)    --Xaxis sip3D --Yaxis ptrel    --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptratio_vs_sip_el, Form("  --outputName plots/%s_ptratio_vs_sip_el.pdf  --noOverflow --setTitle %s ptratio vs sip (elec)  --Xaxis sip3D --Yaxis ptratio  --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(multiiso_vs_sip_el, Form(" --outputName plots/%s_multiiso_vs_sip_el.pdf --noOverflow --setTitle %s multiiso vs sip (elec) --Xaxis sip3D --Yaxis multiiso --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(miniiso_vs_sip_mu, Form("  --outputName plots/%s_miniiso_vs_sip_mu.pdf  --noOverflow --setTitle %s miniiso vs sip (muon)  --Xaxis sip3D --Yaxis miniiso  --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptrel_vs_sip_mu, Form("    --outputName plots/%s_ptrel_vs_sip_mu.pdf    --noOverflow --setTitle %s ptrel vs sip (muon)    --Xaxis sip3D --Yaxis ptrel    --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptratio_vs_sip_mu, Form("  --outputName plots/%s_ptratio_vs_sip_mu.pdf  --noOverflow --setTitle %s ptratio vs sip (muon)  --Xaxis sip3D --Yaxis ptratio  --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(multiiso_vs_sip_mu, Form(" --outputName plots/%s_multiiso_vs_sip_mu.pdf --noOverflow --setTitle %s multiiso vs sip (muon) --Xaxis sip3D --Yaxis multiiso --colors", pfx.c_str(), desc.c_str()));

  drawProfile(miniiso_vs_sip_el  , Form("%s miniiso vs sip (elec) ", desc.c_str()), Form("plots/%s_miniiso_vs_sip_el_prof.pdf"  , pfx.c_str()));
  drawProfile(ptrel_vs_sip_el    , Form("%s ptrel vs sip (elec)   ", desc.c_str()), Form("plots/%s_ptrel_vs_sip_el_prof.pdf"    , pfx.c_str()));
  drawProfile(ptratio_vs_sip_el  , Form("%s ptratio vs sip (elec) ", desc.c_str()), Form("plots/%s_ptratio_vs_sip_el_prof.pdf"  , pfx.c_str()));
  drawProfile(multiiso_vs_sip_el , Form("%s multiiso vs sip (elec)", desc.c_str()), Form("plots/%s_multiiso_vs_sip_el_prof.pdf" , pfx.c_str()));
  drawProfile(miniiso_vs_sip_mu  , Form("%s miniiso vs sip (muon) ", desc.c_str()), Form("plots/%s_miniiso_vs_sip_mu_prof.pdf"  , pfx.c_str()));
  drawProfile(ptrel_vs_sip_mu    , Form("%s ptrel vs sip (muon)   ", desc.c_str()), Form("plots/%s_ptrel_vs_sip_mu_prof.pdf"    , pfx.c_str()));
  drawProfile(ptratio_vs_sip_mu  , Form("%s ptratio vs sip (muon) ", desc.c_str()), Form("plots/%s_ptratio_vs_sip_mu_prof.pdf"  , pfx.c_str()));
  drawProfile(multiiso_vs_sip_mu , Form("%s multiiso vs sip (muon)", desc.c_str()), Form("plots/%s_multiiso_vs_sip_mu_prof.pdf" , pfx.c_str()));

  PlotMaker2D(ptratio_vs_miniiso_el, Form("--outputName plots/%s_ptratio_vs_miniiso_el.pdf --noOverflow --setTitle %s 1/ptratio vs miniIso (elec) --Xaxis miniIso --Yaxis 1/p_{T}^{ratio} --colors", pfx.c_str(), desc.c_str()));
  PlotMaker2D(ptratio_vs_miniiso_mu, Form("--outputName plots/%s_ptratio_vs_miniiso_mu.pdf --noOverflow --setTitle %s 1/ptratio vs miniIso (muon) --Xaxis miniIso --Yaxis 1/p_{T}^{ratio} --colors", pfx.c_str(), desc.c_str()));

  PlotMaker2D(mva_vs_aeta_true, Form("--outputName plots/%s_mva_vs_aeta_true.pdf --noOverflow --setTitle %s MVA vs abs(etaSC) for prompt leptons --Xaxis abs(etaSC) --Yaxis MVA --colors --isLogz", pfx.c_str(), desc.c_str()));
  PlotMaker2D(mva_vs_aeta_fake, Form("--outputName plots/%s_mva_vs_aeta_fake.pdf --noOverflow --setTitle %s MVA vs abs(etaSC) for fake leptons --Xaxis abs(etaSC) --Yaxis MVA --colors --isLogz", pfx.c_str(), desc.c_str()));

  singleComparisonMaker(numer_vs_sip_el,denom_vs_sip_el,"denom",Form("FR vs sip3D (elec, %s)",desc.c_str()), Form("--dataName numer --xAxisLabel sip3D --outputName plots/%s_numer_vs_sip_el.pdf --ratioUpperBound 0.6 --errHistAtBottom --legendCounts --topYaxisTitle FR --isLinear --lumi %.1f",pfx.c_str(), luminosity));
  singleComparisonMaker(numer_vs_sip_mu,denom_vs_sip_mu,"denom",Form("FR vs sip3D (muon, %s)",desc.c_str()), Form("--dataName numer --xAxisLabel sip3D --outputName plots/%s_numer_vs_sip_mu.pdf --ratioUpperBound 0.6 --errHistAtBottom --legendCounts --topYaxisTitle FR --isLinear --lumi %.1f",pfx.c_str(), luminosity));

  singleComparisonMaker(numer_vs_pt_el,denom_vs_pt_el,"denom",Form("FR vs p_{T} (elec, sip3D>4, %s)",desc.c_str()), Form("--dataName numer --xAxisLabel p_{T} --outputName plots/%s_numer_vs_pt_el.pdf --ratioUpperBound 1.0 --errHistAtBottom --legendCounts --topYaxisTitle FR --isLinear --lumi %.1f",pfx.c_str(), luminosity));
  singleComparisonMaker(numer_vs_pt_mu,denom_vs_pt_mu,"denom",Form("FR vs p_{T} (muon, sip3D>4, %s)",desc.c_str()), Form("--dataName numer --xAxisLabel p_{T} --outputName plots/%s_numer_vs_pt_mu.pdf --ratioUpperBound 1.0 --errHistAtBottom --legendCounts --topYaxisTitle FR --isLinear --lumi %.1f",pfx.c_str(), luminosity));

  singleComparisonMaker(numer_vs_eta_el,denom_vs_eta_el,"denom",Form("FR vs abs(#eta) (elec, sip3D>4, %s)",desc.c_str()), Form("--dataName numer --xAxisLabel |#eta| --outputName plots/%s_numer_vs_eta_el.pdf --ratioUpperBound 1.0 --errHistAtBottom --legendCounts --topYaxisTitle FR --isLinear --lumi %.1f",pfx.c_str(), luminosity));
  singleComparisonMaker(numer_vs_eta_mu,denom_vs_eta_mu,"denom",Form("FR vs abs(#eta) (muon, sip3D>4, %s)",desc.c_str()), Form("--dataName numer --xAxisLabel |#eta| --outputName plots/%s_numer_vs_eta_mu.pdf --ratioUpperBound 1.0 --errHistAtBottom --legendCounts --topYaxisTitle FR --isLinear --lumi %.1f",pfx.c_str(), luminosity));

  printCounter();

  textfile.close();
  system("sort -n -k1 -k2 -k3 unsorted.txt > ucsx_ISFR_dump.txt");

}

void corrected_division(TH2D* hnum1, TH2D* hnum2, TH2D* hden1, TH2D* hden2) {
    hnum1->Add(hnum2, -1.0);
    hden1->Add(hden2, -1.0);
    hnum1->Divide(hnum1, hden1, 1, 1, "b");
}

void prompt_corrected_FR(int doHighHT=-1){

    TString htstr = "";
    TString desc = "";
    if(doHighHT == -1) { htstr = "all"; desc = "All H_{T}"; }
    if(doHighHT == 0)  { htstr = "lowht"; desc = "Low H_{T}"; }
    if(doHighHT == 1)  { htstr = "highht"; desc = "High H_{T}"; }
    TFile* f_data = TFile::Open(Form("data_%s_inSituFR_cone_FR_histos.root", htstr.Data()));
    TFile* f_mc = TFile::Open(Form("prompt_%s_inSituFR_cone_FR_histos.root", htstr.Data()));


    TH2D* data_numer_elec_2D_sipgt4 = (TH2D*) f_data->Get("numer_elec_sipgt4");
    TH2D* data_numer_muon_2D_sipgt4 = (TH2D*) f_data->Get("numer_muon_sipgt4");
    TH2D* data_numer_elec_2D_siplt4 = (TH2D*) f_data->Get("numer_elec_siplt4");
    TH2D* data_numer_muon_2D_siplt4 = (TH2D*) f_data->Get("numer_muon_siplt4");
    TH2D* prompt_numer_elec_2D_sipgt4 = (TH2D*) f_mc->Get("numer_elec_sipgt4");
    TH2D* prompt_numer_muon_2D_sipgt4 = (TH2D*) f_mc->Get("numer_muon_sipgt4");
    TH2D* prompt_numer_elec_2D_siplt4 = (TH2D*) f_mc->Get("numer_elec_siplt4");
    TH2D* prompt_numer_muon_2D_siplt4 = (TH2D*) f_mc->Get("numer_muon_siplt4");

    TH2D* data_denom_elec_2D_sipgt4 = (TH2D*) f_data->Get("denom_elec_sipgt4");
    TH2D* data_denom_muon_2D_sipgt4 = (TH2D*) f_data->Get("denom_muon_sipgt4");
    TH2D* data_denom_elec_2D_siplt4 = (TH2D*) f_data->Get("denom_elec_siplt4");
    TH2D* data_denom_muon_2D_siplt4 = (TH2D*) f_data->Get("denom_muon_siplt4");
    TH2D* prompt_denom_elec_2D_sipgt4 = (TH2D*) f_mc->Get("denom_elec_sipgt4");
    TH2D* prompt_denom_muon_2D_sipgt4 = (TH2D*) f_mc->Get("denom_muon_sipgt4");
    TH2D* prompt_denom_elec_2D_siplt4 = (TH2D*) f_mc->Get("denom_elec_siplt4");
    TH2D* prompt_denom_muon_2D_siplt4 = (TH2D*) f_mc->Get("denom_muon_siplt4");

    corrected_division(data_numer_elec_2D_sipgt4, prompt_numer_elec_2D_sipgt4, data_denom_elec_2D_sipgt4, prompt_denom_elec_2D_sipgt4);
    corrected_division(data_numer_muon_2D_sipgt4, prompt_numer_muon_2D_sipgt4, data_denom_muon_2D_sipgt4, prompt_denom_muon_2D_sipgt4);
    corrected_division(data_numer_elec_2D_siplt4, prompt_numer_elec_2D_siplt4, data_denom_elec_2D_siplt4, prompt_denom_elec_2D_siplt4);
    corrected_division(data_numer_muon_2D_siplt4, prompt_numer_muon_2D_siplt4, data_denom_muon_2D_siplt4, prompt_denom_muon_2D_siplt4);

    PlotMaker2D(data_numer_elec_2D_siplt4, Form("--outputName plots/%s_promptCorr_FR_elec_siplt4.pdf --noOverflow --setTitle [Prompt-corr] elec %s FR(sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", htstr.Data(), desc.Data())); 
    PlotMaker2D(data_numer_muon_2D_siplt4, Form("--outputName plots/%s_promptCorr_FR_muon_siplt4.pdf --noOverflow --setTitle [Prompt-corr] muon %s FR(sip3D < 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", htstr.Data(), desc.Data())); 
    PlotMaker2D(data_numer_elec_2D_sipgt4, Form("--outputName plots/%s_promptCorr_FR_elec_sipgt4.pdf --noOverflow --setTitle [Prompt-corr] elec %s FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", htstr.Data(), desc.Data())); 
    PlotMaker2D(data_numer_muon_2D_sipgt4, Form("--outputName plots/%s_promptCorr_FR_muon_sipgt4.pdf --noOverflow --setTitle [Prompt-corr] muon %s FR(sip3D > 4) --Xaxis fake p_{T} --Yaxis |#eta| --text --colors", htstr.Data(), desc.Data())); 

  TFile *file = new TFile(Form("corr_%s_inSituFR_cone_FR_histos.root", htstr.Data()), "RECREATE");
  file->Write(); 

  data_numer_elec_2D_sipgt4->Write("elec");
  data_numer_muon_2D_sipgt4->Write("muon");
  data_numer_elec_2D_siplt4->Write("elec_siplt4");
  data_numer_muon_2D_siplt4->Write("muon_siplt4");
  file->Close(); 

}

