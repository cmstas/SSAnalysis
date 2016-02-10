#ifndef ScanChain_h
#define ScanChain_h

// C++ includes
#include <string>
#include <vector>

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

class babyMaker {

 public:

  babyMaker() {
  };
  ~babyMaker() {
    delete BabyFile_;
    delete BabyTree_;
  };

  void ScanChain( TChain*, char* name = "test", int = -1);
  void MakeBabyNtuple(const char *);
  void InitBabyNtuple();
  void FillBabyNtuple();
  void CloseBabyNtuple();

 private:

  // baby ntuple variables
  TFile *BabyFile_;
  TTree *BabyTree_;
  
  //Gen variables
  float met_gen;
  float ht_gen;
  float ht_jets_gen;
  int njets_gen;
  int njets_jets_gen;
  int nbtags_gen; 
  int nbtags_jets_gen; 
  float pt1_gen;
  float pt2_gen;
  float eta1_gen;
  float eta2_gen;
  int id1_gen;
  int id2_gen;
  vector <LorentzVector> jets_gen;
  vector <LorentzVector> jets_direct_gen;
  vector <LorentzVector> btags_gen;
  vector <LorentzVector> btags_direct_gen;

  //Reco variables
  int id1_reco;
  int id2_reco;
  float pt1_reco;
  float pt2_reco;
  float eta1_reco;
  float eta2_reco;
  float met_reco;
  float ht_reco;
  float ht_direct_reco;
  bool isGoodLepton_1;
  bool isIsolatedLepton_1;
  bool isNumeratorLepton_1;
  bool isGoodLepton_2;
  bool isIsolatedLepton_2;
  bool isNumeratorLepton_2;
  bool choseRightHyp;
  vector <LorentzVector> jets_reco;
  vector <LorentzVector> jets_reco_direct;
  vector <LorentzVector> btags_reco;
  vector <LorentzVector> btags_jets_reco;
  vector <LorentzVector> jets_reco_cms2;
  int njets_reco;
  int njets_reco_direct;
  int njets_reco_cms2;
  int nbtags_reco;
  int nbtags_jets_reco;
  vector <bool> jet_is_matched;
  vector <bool> jet_is_matched_direct;
  vector <bool> gen_btag_is_matched; 
  vector <bool> gen_btag_is_matched_direct; 

  //Sparm
  vector <float> sparm_value;
  vector <TString> sparm_name; 
};


void babyMaker::MakeBabyNtuple(const char *BabyFilename){

  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  rootdir->cd();
  BabyFile_ = new TFile(Form("%s", BabyFilename), "RECREATE");
  BabyFile_->cd();
  BabyTree_ = new TTree("tree", "A Baby Ntuple");

  //Gen variables
  BabyTree_->Branch("met_gen", &met_gen);
  BabyTree_->Branch("ht_gen", &ht_gen);
  BabyTree_->Branch("ht_jets_gen", &ht_jets_gen);
  BabyTree_->Branch("njets_gen", &njets_gen);
  BabyTree_->Branch("njets_jets_gen", &njets_jets_gen);
  BabyTree_->Branch("nbtags_gen", &nbtags_gen);
  BabyTree_->Branch("nbtags_jets_gen", &nbtags_jets_gen);
  BabyTree_->Branch("pt1_gen", &pt1_gen);
  BabyTree_->Branch("pt2_gen", &pt2_gen);
  BabyTree_->Branch("eta1_gen", &eta1_gen);
  BabyTree_->Branch("eta2_gen", &eta2_gen);
  BabyTree_->Branch("id1_gen", &id1_gen);
  BabyTree_->Branch("id2_gen", &id2_gen);
  BabyTree_->Branch("jets_gen", &jets_gen);
  BabyTree_->Branch("jets_direct_gen", &jets_direct_gen);
  BabyTree_->Branch("btags_gen", &btags_gen);
  BabyTree_->Branch("btags_direct_gen", &btags_direct_gen);

  //Reco variables
  BabyTree_->Branch("choseRightHyp", &choseRightHyp);
  BabyTree_->Branch("id1_reco", &id1_reco);
  BabyTree_->Branch("id2_reco", &id2_reco);
  BabyTree_->Branch("pt1_reco", &pt1_reco);
  BabyTree_->Branch("pt2_reco", &pt2_reco);
  BabyTree_->Branch("eta1_reco", &eta1_reco);
  BabyTree_->Branch("eta2_reco", &eta2_reco);
  BabyTree_->Branch("met_reco", &met_reco);
  BabyTree_->Branch("ht_reco", &ht_reco);
  BabyTree_->Branch("ht_direct_reco", &ht_direct_reco);
  BabyTree_->Branch("isGoodLepton_1", &isGoodLepton_1);
  BabyTree_->Branch("isIsolatedLepton_1", &isIsolatedLepton_1);
  BabyTree_->Branch("isNumeratorLepton_1", &isNumeratorLepton_1);
  BabyTree_->Branch("isGoodLepton_2", &isGoodLepton_2);
  BabyTree_->Branch("isIsolatedLepton_2", &isIsolatedLepton_2);
  BabyTree_->Branch("isNumeratorLepton_2", &isNumeratorLepton_2);
  BabyTree_->Branch("jets_reco", &jets_reco);
  BabyTree_->Branch("jets_reco_direct", &jets_reco_direct);
  BabyTree_->Branch("jets_reco_cms2", &jets_reco_cms2);
  BabyTree_->Branch("btags_reco", &btags_reco);
  BabyTree_->Branch("btags_jets_reco", &btags_reco);
  BabyTree_->Branch("njets_reco", &njets_reco);
  BabyTree_->Branch("njets_reco_direct", &njets_reco_direct);
  BabyTree_->Branch("njets_reco_cms2", &njets_reco_cms2);
  BabyTree_->Branch("nbtags_reco", &nbtags_reco);
  BabyTree_->Branch("nbtags_jets_reco", &nbtags_jets_reco);
  BabyTree_->Branch("gen_btag_is_matched", &gen_btag_is_matched);
  BabyTree_->Branch("gen_btag_is_matched_direct", &gen_btag_is_matched_direct);
  BabyTree_->Branch("jet_is_matched", &jet_is_matched);
  BabyTree_->Branch("jet_is_matched_direct", &jet_is_matched_direct);

  //Sparms
  BabyTree_->Branch("sparm_value", &sparm_value);
  BabyTree_->Branch("sparm_name", &sparm_name);

  return;
}

void babyMaker::InitBabyNtuple () {

    //Gen variables
    met_gen = -10;
    ht_gen = -10;
    ht_jets_gen = -10;
    njets_gen = -10;
    njets_jets_gen = -10;
    nbtags_gen = -10;
    nbtags_jets_gen = -10;
    pt1_gen = -1;
    pt2_gen = -1;
    eta1_gen = -1;
    eta2_gen = -1;
    id1_gen = -1;
    id2_gen = -1;
    jets_gen.clear();
    jets_direct_gen.clear();
    btags_gen.clear();
    btags_direct_gen.clear();

    //Reco variables
    id1_reco = -1;
    id2_reco = -1;
    pt1_reco = -1;
    pt2_reco = -1;
    eta1_reco = -1;
    eta2_reco = -1;
    met_reco = -1;  
    ht_reco = -1;
    ht_direct_reco = -1;
    isGoodLepton_1 = false;
    isIsolatedLepton_1 = false;
    isNumeratorLepton_1 = false;
    isGoodLepton_2 = false;
    isIsolatedLepton_2 = false;
    isNumeratorLepton_2 = false;
    jets_reco.clear();
    jets_reco_direct.clear();
    btags_reco.clear();
    btags_jets_reco.clear();
    gen_btag_is_matched.clear();
    gen_btag_is_matched_direct.clear();
    jet_is_matched.clear();
    jet_is_matched_direct.clear();
    jets_reco_cms2.clear();
    njets_reco = -1;
    njets_reco_direct = -1;
    njets_reco_cms2 = -1;
    nbtags_reco = -1;
    nbtags_jets_reco = -1;
    choseRightHyp = false;

    //Sparms
    sparm_value.clear();
    sparm_name.clear();

    return;
}

void babyMaker::FillBabyNtuple(){
  BabyTree_->Fill();
  return;
}

void babyMaker::CloseBabyNtuple(){
  BabyFile_->cd();
  BabyTree_->Write();
  BabyFile_->Close();
  return;
}

#endif
