// Usage:
// > root -b doAll.C

// C++
#include <iostream>
#include <vector>
#include <iostream>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TList.h"
#include "TIterator.h"
#include "TString.h"
#include "TRegexp.h"
#include "TObject.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TDatabasePDG.h"
#include "TH2F.h"

// CMS2
#include "/home/users/cgeorge/CORE/CMS2.h"
#include "/home/users/cgeorge/CORE/electronSelections.h"
#include "/home/users/cgeorge/CORE/muonSelections.h"
#include "/home/users/cgeorge/CORE/ssSelections.h"
#include "/home/users/cgeorge/CORE/jetSelections.h"
#include "/home/users/cgeorge/CORE/mcSelections.h"
#include "/home/users/cgeorge/CORE/eventSelections.h"
#include "/home/users/cgeorge/CORE/susySelections.h"
#include "/home/users/cgeorge/CORE/trackSelections.h"

#include "Math/VectorUtil.h"

// .h file
#include "looper.h"
#include "gen_functions.h"

//error inform
#include <stdexcept>

using namespace std;
using namespace tas;
using namespace gen_tools;
using namespace samesign;

int min_muon_pt = 0;
int min_elec_pt = 0;

int isGoodHyp(int iHyp, bool isData, int analType){
  //Bunch o' variables
  float pt_ll = hyp_ll_p4().at(iHyp).pt();
  float pt_lt = hyp_lt_p4().at(iHyp).pt();
  float eta_ll = hyp_ll_p4().at(iHyp).eta();
  float eta_lt = hyp_lt_p4().at(iHyp).eta();
  bool isss = false;
  if (hyp_ll_charge().at(iHyp) == hyp_lt_charge().at(iHyp)) isss = true;  
  int id_ll = hyp_ll_id().at(iHyp);
  int id_lt = hyp_lt_id().at(iHyp);
  bool passed_id_numer_ll = isNumeratorLepton(id_ll, hyp_ll_index().at(iHyp));
  bool passed_id_numer_lt = isNumeratorLepton(id_lt, hyp_lt_index().at(iHyp));
  bool passed_id_denom_ll = isDenominatorLepton(id_ll, hyp_ll_index().at(iHyp));
  bool passed_id_denom_lt = isDenominatorLepton(id_lt, hyp_lt_index().at(iHyp));
  int njets = 0;
  if (isData == 0) njets = samesign::nJets(iHyp, JETS_TYPE_PF_FAST_CORR, 0.4, 40.0, 2.4, min_muon_pt, min_elec_pt);
  if (isData == 1) njets = samesign::nJets(iHyp, JETS_TYPE_PF_FAST_CORR_RESIDUAL, 0.4, 40.0, 2.4, min_muon_pt, min_elec_pt);
  bool extraZ = samesign::makesExtraZ(iHyp);
  bool extraGammaStar = samesign::makesExtraGammaStar(iHyp);

  //Cuts:
  if (analType == 0 && pt_ll < 20) return 0;
  else if (analType == 1 && pt_ll < 10) return 0;
  else if (analType == 2 && abs(id_ll) == 11 && pt_ll < 10) return 0;
  else if (analType == 2 && abs(id_ll) == 13 && pt_ll < 5) return 0;
  if (analType == 0 && pt_lt < 20) return 0;
  else if (analType == 1 && pt_lt < 10) return 0;
  else if (analType == 2 && abs(id_lt) == 11 && pt_lt < 10) return 0;
  else if (analType == 2 && abs(id_lt) == 13 && pt_lt < 5) return 0;
  if (fabs(eta_ll) > 2.4) return 0;
  if (fabs(eta_lt) > 2.4) return 0;
  if (extraZ) return 0;
  if (extraGammaStar) return 0;
  if (njets < 2) return 0;
  if ((hyp_ll_p4().at(iHyp) + hyp_lt_p4().at(iHyp)).M() < 8) return 0; 
  if (!hypsFromFirstGoodVertex(iHyp)) return 0;
  if (isData == true && samesign::passesTrigger(hyp_type().at(iHyp), analType) == 0) return 0;

  //Results
  if (passed_id_numer_ll == 0 && passed_id_denom_ll == 0) return 0;      // 0 if ll fails
  if (passed_id_numer_lt == 0 && passed_id_denom_lt == 0) return 0; // 0 if lt fails
  else if (passed_id_numer_lt == 1 && passed_id_numer_ll == 1 && isss == true) return 3;  // 3 if both high pass, SS
  else if (passed_id_numer_lt == 1 && passed_id_numer_ll == 1 && isss == false) return 4;  // 4 if both high pass, OS
  else if (passed_id_numer_lt == 0 && passed_id_numer_ll == 0 && passed_id_denom_lt == 1 && passed_id_denom_ll == 1 && isss == true) return 1; // 1 if both low pass
  else if (isss == true) return 2; //2 for lowpass/highpass
  else return 0; //non-highpass OS
}

vector <bool> btagIsMatched( vector <LorentzVector> gen_btags ){
  vector <bool> tmp_vector;
  for (size_t i = 0; i < gen_btags.size(); i++){
    bool matched = false;
    float closest = .4;
    int reco_j = -1;
    for (size_t j = 0; j < pfjets_p4().size(); j++){
       if (pfjets_p4().at(j).pt() > 40 && fabs(pfjets_p4().at(j).eta()) < 2.4 && ROOT::Math::VectorUtil::DeltaR(pfjets_p4().at(j), gen_btags.at(i)) < closest){
         matched = true;
         closest = ROOT::Math::VectorUtil::DeltaR(pfjets_p4().at(j), gen_btags.at(i));
         reco_j = j; 
       } 
    }//for loop, reco
    if (matched == true && pfjets_combinedSecondaryVertexBJetTag().at(reco_j) > .679) tmp_vector.push_back(true);  
    else tmp_vector.push_back(false);
  }//for-loop, gen
  return tmp_vector;
}

vector <bool> jetIsMatched( vector <LorentzVector> gen_jets ){
  vector <bool> tmp_vector;
  for (size_t i = 0; i < gen_jets.size(); i++){
    bool matched = false;
    float closest = .4;
    int reco_j = -1;
    for (size_t j = 0; j < pfjets_p4().size(); j++){
       if (pfjets_p4().at(j).pt() > 40 && fabs(pfjets_p4().at(j).eta()) < 2.4 && ROOT::Math::VectorUtil::DeltaR(pfjets_p4().at(j), gen_jets.at(i)) < closest){
         matched = true;
         closest = ROOT::Math::VectorUtil::DeltaR(pfjets_p4().at(j), gen_jets.at(i));
         reco_j = j; 
       } 
    }//for loop, reco
    if (matched == true) tmp_vector.push_back(true);  
    else tmp_vector.push_back(false);
  }//for-loop, gen
  return tmp_vector;
}

float jetRecoHT( vector <LorentzVector> gen_jets ){
  float recoHT = 0;
  for (size_t i = 0; i < gen_jets.size(); i++){
    bool matched = false;
    float closest = .4;
    int reco_j = -1;
    for (size_t j = 0; j < pfjets_p4().size(); j++){
       if (pfjets_p4().at(j).pt() > 40 && fabs(pfjets_p4().at(j).eta()) < 2.4 && ROOT::Math::VectorUtil::DeltaR(pfjets_p4().at(j), gen_jets.at(i)) < closest){
         matched = true;
         closest = ROOT::Math::VectorUtil::DeltaR(pfjets_p4().at(j), gen_jets.at(i));
         reco_j = j; 
       } 
    }//for loop, reco
    if (matched == true) recoHT += pfjets_p4().at(reco_j).pt();  
  }//for-loop, gen
  return recoHT;
}

void babyMaker::ScanChain(TChain* chain, char* name, int nEvents) {

  //Make Baby Ntuple  
  MakeBabyNtuple( Form("%s.root", name) );
  
  //Initialize Baby Ntuple
  InitBabyNtuple();

  //Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile( currentFile->GetTitle() );
    TTree *tree = (TTree*)file->Get("Events");
    cms2.Init(tree);
  
    // Loop over Events in current file
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {

      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      cms2.GetEntry(event);
      nEventsTotal++;
    
      // Progress
      CMS2::progress( nEventsTotal, nEventsChain );

      //********************************** Begin Analysis Code *************************************

      //Initialize Baby Ntuple
      InitBabyNtuple();

      //Determine best hyp for gen
      pair<GenParticleStruct, GenParticleStruct> bestGenHyp = getGenHyp(10.0, 5.0, BOTH);
      GenParticleStruct GenParticle1 = bestGenHyp.first;
      GenParticleStruct GenParticle2 = bestGenHyp.second;

      //Throw away any events that do not have two good leptons at gen level
      if (GenParticle1.pt == 0) continue;

      //Determine generated jets & b-tags
      jets_gen = getGenJets(0.0, 2.4, 5.0, 10.0);
      jets_direct_gen = getGenJetsDirectly(0.0, 2.4, 5.0, 10.0);
      btags_gen = getGenBjets(40.0, 2.4, 5.0, 10.0);
      btags_direct_gen = getGenBjetsDirectly(40.0, 2.4, 5.0, 10.0);

      //Determine if generated jets & btags are matched at reco level
      jet_is_matched = jetIsMatched(jets_gen);
      jet_is_matched_direct = jetIsMatched(jets_direct_gen);
      gen_btag_is_matched = btagIsMatched(btags_gen);
      gen_btag_is_matched_direct = btagIsMatched(btags_direct_gen);

      //Record reco ht
      ht_reco = jetRecoHT(jets_gen);
      ht_direct_reco = jetRecoHT(jets_direct_gen);

      //Record gen information
      bool from_tau1 = (abs(GenParticle1.id) == 15);
      bool from_tau2 = (abs(GenParticle2.id) == 15);
      met_gen = gen_met();
      ht_gen = getGenHT(40.0, 2.4, 5.0, 10.0);
      ht_jets_gen = getGenHTdirectly(40.0, 2.4, 5.0, 10.0);
      njets_gen = jets_gen.size();
      njets_jets_gen = jets_direct_gen.size();
      nbtags_gen = btags_gen.size();
      nbtags_jets_gen = btags_direct_gen.size();
      pt1_gen = from_tau1 ? GenParticle1.dpt : GenParticle1.pt;
      pt2_gen = from_tau2 ? GenParticle2.dpt : GenParticle2.pt;
      eta1_gen = from_tau1 ? GenParticle1.deta : GenParticle1.eta;
      eta2_gen = from_tau2 ? GenParticle2.deta : GenParticle2.eta;
      id1_gen = from_tau1 ? GenParticle1.did : GenParticle1.id;
      id2_gen = from_tau2 ? GenParticle2.did : GenParticle2.id;
      float idx1_gen = from_tau1 ? GenParticle1.didx : GenParticle1.idx;
      float idx2_gen = from_tau2 ? GenParticle2.didx : GenParticle2.idx;


      //Determine whether leptons are reconstructed.  Loop over all reco particles to see if any of them correspond to the gen leptons.
      int idx1_reco = -1;
      int idx2_reco = -1;     

      for (size_t ilep = 0; ilep < els_p4().size(); ilep++){
        if (els_mc3idx().at(ilep) == idx1_gen){ idx1_reco = ilep; id1_reco = (els_charge().at(ilep) > 0) ? 11 : -11; }
        if (els_mc3idx().at(ilep) == idx2_gen){ idx2_reco = ilep; id2_reco = (els_charge().at(ilep) > 0) ? 11 : -11; }
      }
      for (size_t ilep = 0; ilep < mus_p4().size(); ilep++){
        if (mus_mc3idx().at(ilep) == idx1_gen){ idx1_reco = ilep; id1_reco = (mus_charge().at(ilep) > 0) ? 13 : -13; }
        if (mus_mc3idx().at(ilep) == idx2_gen){ idx2_reco = ilep; id2_reco = (mus_charge().at(ilep) > 0) ? 13 : -13; }
      }

      //Record reco pTs
      if (abs(id1_reco) == 11) pt1_reco = els_p4().at(idx1_reco).pt();
      if (abs(id1_reco) == 13) pt1_reco = mus_p4().at(idx1_reco).pt();
      if (abs(id2_reco) == 11) pt2_reco = els_p4().at(idx2_reco).pt();
      if (abs(id2_reco) == 13) pt2_reco = mus_p4().at(idx2_reco).pt();
      if (abs(id1_reco) == 11) eta1_reco = els_p4().at(idx1_reco).eta();
      if (abs(id1_reco) == 13) eta1_reco = mus_p4().at(idx1_reco).eta();
      if (abs(id2_reco) == 11) eta2_reco = els_p4().at(idx2_reco).eta();
      if (abs(id2_reco) == 13) eta2_reco = mus_p4().at(idx2_reco).eta();

      //Determine which reco hyp the analysis would actually pick
      vector <int> goodhyps;
      goodhyps.clear();
      for (size_t iHyp = 0; iHyp != hyp_type().size(); iHyp++){ 
        if (isGoodHyp(iHyp, false, 0) == 3) goodhyps.push_back(iHyp);
      }

     //Now we choose our best hyp, should there be more than one
     int besthyp = -1;
     if (goodhyps.size() == 1) besthyp = goodhyps[0];
     else if (goodhyps.size() > 1) {
        //First, choose first goodhyp as besthyp
        besthyp = goodhyps[0];
        //Now loop over the rest and see if any one of them is better. 
        for (size_t i = 1; i < goodhyps.size(); i++){
          //First, see if type is better.  If it is, take over.
          if (hyp_type()[goodhyps[i]] < hyp_type()[besthyp]) besthyp = goodhyps[i];
          //Then, see if type is same.  If it is, compare pT.  
          else if (hyp_type()[goodhyps[i]] == hyp_type()[besthyp] && hyp_ll_p4().at(goodhyps[i]).pt()+hyp_lt_p4().at(goodhyps[i]).pt() > hyp_ll_p4().at(besthyp).pt()+hyp_lt_p4().at(besthyp).pt()) besthyp = goodhyps[i]; 
        }//for loop
     }//else loop
 
  //Now besthyp really is the best hypothesis.  Is this hyp the same as the generated event?
  if (besthyp == -1) choseRightHyp = false;
  int ll_gen_idx = -1;
  int lt_gen_idx = -1;
  if (besthyp >= 0){
    if (abs(hyp_ll_id().at(besthyp)) == 11) ll_gen_idx = els_mc3idx().at(hyp_ll_index().at(besthyp));
    if (abs(hyp_lt_id().at(besthyp)) == 11) lt_gen_idx = els_mc3idx().at(hyp_lt_index().at(besthyp));
    if (abs(hyp_ll_id().at(besthyp)) == 13) ll_gen_idx = mus_mc3idx().at(hyp_ll_index().at(besthyp));
    if (abs(hyp_lt_id().at(besthyp)) == 13) lt_gen_idx = mus_mc3idx().at(hyp_lt_index().at(besthyp));
    if ((ll_gen_idx == GenParticle1.idx || ll_gen_idx == GenParticle2.idx) && (lt_gen_idx == GenParticle1.idx || lt_gen_idx == GenParticle2.idx)) choseRightHyp = true;  
    else choseRightHyp = false;
  }

      //Record reco information
      met_reco = evt_pfmet_type1cor();
      if (idx1_reco >= 0){
        isGoodLepton_1 = isGoodLepton(id1_reco, idx1_reco);
        isIsolatedLepton_1 = isIsolatedLepton(id1_reco, idx1_reco);
        isNumeratorLepton_1 = isNumeratorLepton(id1_reco, idx1_reco);
      }
      if (idx2_reco >= 0){
        isGoodLepton_2 = isGoodLepton(id2_reco, idx2_reco);
        isIsolatedLepton_2 = isIsolatedLepton(id2_reco, idx2_reco);
        isNumeratorLepton_2 = isNumeratorLepton(id2_reco, idx2_reco);
      }
    
      njets_reco = 0;
      //number of reco jets
      for (size_t i = 0; i < jets_gen.size(); i++){
        if (jet_is_matched.at(i)){ jets_reco.push_back(jets_gen.at(i)); njets_reco++;}
      }

      njets_reco_direct = 0;
      //number of reco jets, direct (ie match to pfjets rather than partons)
      for (size_t i = 0; i < jets_direct_gen.size(); i++){
        if (jet_is_matched_direct.at(i)){ jets_reco_direct.push_back(jets_direct_gen.at(i)); njets_reco_direct++;}
      }

      nbtags_reco = 0;
      //number of reco b-tags
      for (size_t i = 0; i < btags_gen.size(); i++){
        if (gen_btag_is_matched.at(i)){ btags_reco.push_back(btags_gen.at(i)); nbtags_reco++;}
      }

      nbtags_jets_reco = 0;
      //number of reco b-tags, direct
      for (size_t i = 0; i < btags_direct_gen.size(); i++){
        if (gen_btag_is_matched_direct.at(i)){ 
          btags_jets_reco.push_back(btags_direct_gen.at(i)); 
          nbtags_jets_reco++;
        }
      }

     //Njets reco without truth matching
     njets_reco_cms2 = 0;
     for (size_t i = 0; i < pfjets_p4().size(); i++){
       if (pfjets_p4().at(i).pt() < 40) continue;
       if (fabs(pfjets_p4().at(i).eta()) > 2.4) continue;
       njets_reco_cms2++;
       jets_reco_cms2.push_back(pfjets_p4().at(i));
     }

      //Susy Sparms
      sparm_value = sparm_values();
      sparm_name = sparm_names();

      //Fill the baby histo with the values we've found
      FillBabyNtuple();

      //********************************** End Analysis Code *************************************
}//end loop over events

    // Clean Up
    delete tree;
    file->Close();
    delete file;
  }


  CloseBabyNtuple();

  return;
}
