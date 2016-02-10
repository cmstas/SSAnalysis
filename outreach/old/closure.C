#include "TTree.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TStyle.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLine.h"

#include "ScaleFactors.h"
//#include "eff.h"
#include "fits.h"
#include "SS.h"

using namespace ss;
using namespace std;

enum GenReco { gen, reco };

float prob_0_jet(){
  float prob = 1.0;
  for (size_t k = 0; k < jets_direct_gen().size(); k++){
    prob *= 1.0-jet_prob_jets(jets_direct_gen().at(k).pt());
  }
  return prob;
}

float scale_factor(GenReco type){
  float temp = 1.0;
  if (type == gen){
    //temp *= DileptonTriggerScaleFactor( abs(id1_gen()), pt1_gen(), eta1_gen() );
    //temp *= DileptonTriggerScaleFactor( abs(id2_gen()), pt2_gen(), eta2_gen() );
    //temp *= TagAndProbeScaleFactor( id1_gen(), pt1_gen(), eta1_gen() );
    //temp *= TagAndProbeScaleFactor( id2_gen(), pt2_gen(), eta2_gen() );
  }
  if (type == reco){
    temp *= DileptonTriggerScaleFactor( abs(id1_reco()), pt1_reco(), eta1_reco() );
    temp *= DileptonTriggerScaleFactor( abs(id2_reco()), pt2_reco(), eta2_reco() );
    temp *= TagAndProbeScaleFactor( id1_reco(), pt1_reco(), eta1_reco() );
    temp *= TagAndProbeScaleFactor( id2_reco(), pt2_reco(), eta2_reco() );
  }
  return temp;
}

float prob_1_jet(){
  float prob = 0;
  for (size_t i = 0; i < jets_direct_gen().size()-1; i++){
    float current_prob = 1.0;
    for (size_t k = 0; k < jets_direct_gen().size(); k++){
      if (k == i) current_prob *= jet_prob_jets(jets_direct_gen().at(k).pt());
      else current_prob *= 1.0-jet_prob_jets(jets_direct_gen().at(k).pt());
    }
    prob += current_prob;
  }
  return prob;
}

float prob_2_jets(){
  if (jets_direct_gen().size() < 2) return 0;
  float prob = 0;
  for (size_t i = 0; i < jets_direct_gen().size()-1; i++){
    for (size_t j = i+1; j < jets_direct_gen().size(); j++){
      float current_prob = 1.0;
      for (size_t k = 0; k < jets_direct_gen().size(); k++){
        if (k == i) current_prob *= jet_prob_jets(jets_direct_gen().at(k).pt());
        else if (k == j) current_prob *= jet_prob_jets(jets_direct_gen().at(k).pt());
        else current_prob *= 1.0-jet_prob_jets(jets_direct_gen().at(k).pt());
      }
      prob += current_prob;
    }
  }
  return prob;
}

float nbtags_jets_reco2(){
  int temp = 0;
  for (size_t i = 0; i < btags_direct_gen().size(); i++){
    if (gen_btag_is_matched_direct().at(i)) temp++;
  }
  return temp;
}

float prob_3_jets(){
  if (jets_direct_gen().size() < 3) return 0;
  float prob = 0;
  for (size_t i = 0; i < jets_direct_gen().size()-2; i++){
    for (size_t j = i+1; j < jets_direct_gen().size()-1; j++){
      for (size_t m = j+1; m < jets_direct_gen().size(); m++){
        float current_prob = 1.0;
        for (size_t k = 0; k < jets_direct_gen().size(); k++){
          if (k == i) current_prob *= jet_prob_jets(jets_direct_gen().at(k).pt());
          else if (k == j) current_prob *= jet_prob_jets(jets_direct_gen().at(k).pt());
          else if (k == m) current_prob *= jet_prob_jets(jets_direct_gen().at(k).pt());
          else current_prob *= 1.0-jet_prob_jets(jets_direct_gen().at(k).pt());
        }
        prob += current_prob;
      }//m block
    }//j block
  }//i block
  return prob;
}

float prob_geq4_jets(){
  return 1.0-prob_3_jets()-prob_2_jets()-prob_1_jet()-prob_0_jet();
}

float prob_2or3_jets(){
  return prob_3_jets()+prob_2_jets();
}

float prob_one_btag(){
  float nbtag_prob = 0;
  if (nbtags_jets_gen() == 0) return 0;
  if (nbtags_jets_gen() >= 1){
    for (int i = 0; i < nbtags_jets_gen(); i++){
      float blah = 1.0;
      blah *= btag_prob_jets(btags_direct_gen().at(i).pt());
      for (int j = 0; j < nbtags_jets_gen(); j++){
        if (i != j) blah *= 1.0 - btag_prob_jets(btags_direct_gen().at(j).pt());
      }//loop over j
      nbtag_prob += blah;
    }//loop over i
  }//if nbtags >= 1
  return nbtag_prob;
}

float id_iso_prob(){
  float prob = 1.0;
  if (abs(id1_gen()) == 11) prob *= electron_prob(pt1_gen()); 
  if (abs(id1_gen()) == 13) prob *= muon_prob(pt1_gen()); 
  if (abs(id2_gen()) == 11) prob *= electron_prob(pt2_gen()); 
  if (abs(id2_gen()) == 13) prob *= muon_prob(pt2_gen()); 
  return prob;
}

float prob_zero_btag(){
  float prob = 1.0;
  for (int i = 0; i < nbtags_jets_gen(); i++){
    prob *= 1.0 - btag_prob_jets(btags_direct_gen().at(i).pt()); 
  }
  return prob;
}

void closure(const char* name, float gluino_mass = -1, float lsp_mass = -1, bool verbose = false){
  TFile *file = new TFile(Form("%s", name));
  TTree *tree = (TTree*)file->Get("tree");

  samesign.Init(tree);

  //Scores
  float exptl_overall[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
  float theor_overall[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  float exptl_id_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
  float theor_id_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  float exptl_met_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
  float theor_met_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  float exptl_ht_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
  float theor_ht_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  float exptl_njets_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
  float theor_njets_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  float exptl_nbtags_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
  float theor_nbtags_factor[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  //loop over all events
  for (int iEvent = 0; iEvent < tree->GetEntries(); iEvent++){
    samesign.GetEntry(iEvent);

    //Pre-cuts: we never want anything without 2 gen dileptons (implemented at baby level), which are SS, or 2 gen jets, or too few pT:
    if (id1_gen()*id2_gen() < 0) continue;
    if (jets_direct_gen().size() < 2) continue; //if less than 2 gen jets, then must be less than 2 reco jets also
    if (pt1_gen() < 20) continue;
    if (pt2_gen() < 20) continue;

    //Also bounce anything that doesn't have the required susy parms
    if (gluino_mass != -1 && sparm_value().at(0) != gluino_mass) continue;
    if (lsp_mass != -1 && sparm_value().at(1) != lsp_mass) continue;

    //***********************************OVERALL********************************************
    //Unfactorized exclusive yields
      //Prediction
    theor_overall[0] += met_prob_50(met_gen())*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(prob_2or3_jets()+prob_geq4_jets())*scale_factor(gen);
    theor_overall[1] += prob_zero_btag()*met_prob_50(met_gen())*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*(1.0-met_prob_120(met_gen()))*prob_2or3_jets()*scale_factor(gen);
    theor_overall[2] += prob_zero_btag()*met_prob_50(met_gen())*id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*(1.0-met_prob_120(met_gen()))*prob_2or3_jets()*scale_factor(gen);
    theor_overall[3] += prob_zero_btag()*met_prob_50(met_gen())*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*(1.0-met_prob_120(met_gen()))*prob_geq4_jets()*scale_factor(gen);
    theor_overall[4] += prob_zero_btag()*met_prob_50(met_gen())*id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*(1.0-met_prob_120(met_gen()))*prob_geq4_jets()*scale_factor(gen);
    theor_overall[5] += prob_zero_btag()*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*met_prob_120(met_gen())*prob_2or3_jets()*scale_factor(gen);
    theor_overall[6] += prob_zero_btag()*id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*met_prob_120(met_gen())*prob_2or3_jets()*scale_factor(gen);
    theor_overall[7] += prob_zero_btag()*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*prob_geq4_jets()*met_prob_120(met_gen())*scale_factor(gen);
    theor_overall[8] += prob_zero_btag()*id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*met_prob_120(met_gen())*prob_geq4_jets()*scale_factor(gen);
    
    theor_overall[11] += met_prob_50(met_gen())*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*(1.0-met_prob_120(met_gen()))*prob_one_btag()*prob_2or3_jets()*scale_factor(gen);
    theor_overall[12] += met_prob_50(met_gen())*id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*(1.0-met_prob_120(met_gen()))*prob_one_btag()*prob_2or3_jets()*scale_factor(gen);
    theor_overall[13] += met_prob_50(met_gen())*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*(1.0-met_prob_120(met_gen()))*prob_one_btag()*prob_geq4_jets()*scale_factor(gen);
    theor_overall[14] += met_prob_50(met_gen())*id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*(1.0-met_prob_120(met_gen()))*prob_one_btag()*prob_geq4_jets()*scale_factor(gen);
    theor_overall[15] += ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*met_prob_120(met_gen())*prob_one_btag()*prob_2or3_jets()*scale_factor(gen);
    theor_overall[16] += id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*met_prob_120(met_gen())*prob_one_btag()*prob_2or3_jets()*scale_factor(gen);
    theor_overall[17] += ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*met_prob_120(met_gen())*prob_one_btag()*prob_geq4_jets()*scale_factor(gen);
    theor_overall[18] += id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*met_prob_120(met_gen())*prob_one_btag()*prob_geq4_jets()*scale_factor(gen);

    theor_overall[21] += met_prob_50(met_gen())*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*(1.0-met_prob_120(met_gen()))*(1.0-prob_one_btag()-prob_zero_btag())*prob_2or3_jets()*scale_factor(gen);
    theor_overall[22] += met_prob_50(met_gen())*id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*(1.0-met_prob_120(met_gen()))*(1.0-prob_one_btag()-prob_zero_btag())*prob_2or3_jets()*scale_factor(gen);
    theor_overall[23] += met_prob_50(met_gen())*ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*(1.0-met_prob_120(met_gen()))*(1.0-prob_one_btag()-prob_zero_btag())*prob_geq4_jets()*scale_factor(gen);
    theor_overall[24] += met_prob_50(met_gen())*id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*(1.0-met_prob_120(met_gen()))*(1.0-prob_one_btag()-prob_zero_btag())*prob_geq4_jets()*scale_factor(gen);
    theor_overall[25] += ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*met_prob_120(met_gen())*(1.0-prob_one_btag()-prob_zero_btag())*prob_2or3_jets()*scale_factor(gen);
    theor_overall[26] += id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*met_prob_120(met_gen())*(1.0-prob_one_btag()-prob_zero_btag())*prob_2or3_jets()*scale_factor(gen);
    theor_overall[27] += ht_prob_jets_200(ht_jets_gen())*id_iso_prob()*(1.0-ht_prob_jets_400(ht_jets_gen()))*met_prob_120(met_gen())*(1.0-prob_one_btag()-prob_zero_btag())*prob_geq4_jets()*scale_factor(gen);
    theor_overall[28] += id_iso_prob()*ht_prob_jets_400(ht_jets_gen())*met_prob_120(met_gen())*(1.0-prob_one_btag()-prob_zero_btag())*prob_geq4_jets()*scale_factor(gen);

      //Observed
    if (njets_reco_direct() >= 2) exptl_overall[0] += (ht_direct_reco() > 200  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() ? scale_factor(reco) : 0);
    if (njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[1] += (nbtags_jets_reco2() == 0 && ht_direct_reco() > 200  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 && met_reco() < 120 ? scale_factor(reco) : 0);
    if (njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[2] += (nbtags_jets_reco2() == 0 && ht_direct_reco() > 400  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && met_reco() < 120 ? scale_factor(reco) : 0);
    if (njets_reco_direct() >= 4) exptl_overall[3] += (nbtags_jets_reco2() == 0 && ht_direct_reco() > 200  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 && met_reco() < 120 ? scale_factor(reco) : 0);
    if (njets_reco_direct() >= 4) exptl_overall[4] += (nbtags_jets_reco2() == 0 && ht_direct_reco() > 400  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && met_reco() < 120 ? scale_factor(reco) : 0);
    if (njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[5] += (nbtags_jets_reco2() == 0 && ht_direct_reco() > 200  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400  ? scale_factor(reco) : 0);
    if (njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[6] += (nbtags_jets_reco2() == 0 && ht_direct_reco() > 400  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() ? scale_factor(reco) : 0);
    if (njets_reco_direct() >= 4) exptl_overall[7] += (nbtags_jets_reco2() == 0 && ht_direct_reco() > 200  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 ? scale_factor(reco) : 0);
    if (njets_reco_direct() >= 4) exptl_overall[8] += (nbtags_jets_reco2() == 0 && ht_direct_reco() > 400  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2()  ? scale_factor(reco) : 0);

    if (nbtags_jets_reco2() == 1 && njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[11] += (ht_direct_reco() > 200  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 && met_reco() < 120 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() == 1 && njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[12] += (ht_direct_reco() > 400  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && met_reco() < 120 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() == 1 && njets_reco_direct() >= 4) exptl_overall[13] += (ht_direct_reco() > 200  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 && met_reco() < 120 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() == 1 && njets_reco_direct() >= 4) exptl_overall[14] += (ht_direct_reco() > 400  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && met_reco() < 120 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() == 1 && njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[15] += (ht_direct_reco() > 200  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400  ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() == 1 && njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[16] += (ht_direct_reco() > 400  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() == 1 && njets_reco_direct() >= 4) exptl_overall[17] += (ht_direct_reco() > 200  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() == 1 && njets_reco_direct() >= 4) exptl_overall[18] += (ht_direct_reco() > 400  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2()  ? scale_factor(reco) : 0);

    if (nbtags_jets_reco2() >= 2 && njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[21] += (ht_direct_reco() > 200  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 && met_reco() < 120 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() >= 2 && njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[22] += (ht_direct_reco() > 400  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && met_reco() < 120 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() >= 2 && njets_reco_direct() >= 4) exptl_overall[23] += (ht_direct_reco() > 200  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 && met_reco() < 120 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() >= 2 && njets_reco_direct() >= 4) exptl_overall[24] += (ht_direct_reco() > 400  && met_reco() > 50 && isNumeratorLepton_1() && isNumeratorLepton_2() && met_reco() < 120 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() >= 2 && njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[25] += (ht_direct_reco() > 200  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400  ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() >= 2 && njets_reco_direct() >= 2 && njets_reco_direct() < 4) exptl_overall[26] += (ht_direct_reco() > 400  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() >= 2 && njets_reco_direct() >= 4) exptl_overall[27] += (ht_direct_reco() > 200  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2() && ht_direct_reco() < 400 ? scale_factor(reco) : 0);
    if (nbtags_jets_reco2() >= 2 && njets_reco_direct() >= 4) exptl_overall[28] += (ht_direct_reco() > 400  && met_reco() > 120 && isNumeratorLepton_1() && isNumeratorLepton_2()  ? scale_factor(reco) : 0);

    if (verbose){
      //ID factorization
        //Predicted
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[1] += id_iso_prob()*scale_factor(gen); 
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_id_factor[2] += id_iso_prob()*scale_factor(gen); 
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[3] += id_iso_prob()*scale_factor(gen); 
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_id_factor[4] += id_iso_prob()*scale_factor(gen); 
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[5] += id_iso_prob()*scale_factor(gen); 
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_id_factor[6] += id_iso_prob()*scale_factor(gen); 
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[7] += id_iso_prob()*scale_factor(gen); 
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_id_factor[8] += id_iso_prob()*scale_factor(gen); 

      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[11] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_id_factor[12] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[13] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_id_factor[14] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[15] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_id_factor[16] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[17] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_id_factor[18] += id_iso_prob()*scale_factor(gen); 

      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[21] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_id_factor[22] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[23] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_id_factor[24] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[25] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_id_factor[26] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_id_factor[27] += id_iso_prob()*scale_factor(gen); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_id_factor[28] += id_iso_prob()*scale_factor(gen); 

        //Observed
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[1] += scale_factor(reco); 
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[2] += scale_factor(reco); 
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[3] += scale_factor(reco); 
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[4] += scale_factor(reco); 
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[5] += scale_factor(reco); 
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[6] += scale_factor(reco); 
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[7] += scale_factor(reco); 
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[8] += scale_factor(reco); 

      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[11] += scale_factor(reco); 
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[12] += scale_factor(reco); 
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[13] += scale_factor(reco); 
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[14] += scale_factor(reco); 
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[15] += scale_factor(reco); 
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[16] += scale_factor(reco); 
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[17] += scale_factor(reco); 
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[18] += scale_factor(reco); 

      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[21] += scale_factor(reco); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[22] += scale_factor(reco); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[23] += scale_factor(reco); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[24] += scale_factor(reco); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[25] += scale_factor(reco); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[26] += scale_factor(reco); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[27] += scale_factor(reco); 
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && isNumeratorLepton_1() && isNumeratorLepton_2()) exptl_id_factor[28] += scale_factor(reco); 

      //MET Factorization
        //Predicted
      if (jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[1] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_met_factor[2] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[3] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_met_factor[4] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[5] += met_prob_120(met_gen()); 
      if (jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_met_factor[6] += met_prob_120(met_gen()); 
      if (jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[7] += met_prob_120(met_gen()); 
      if (jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_met_factor[8] += met_prob_120(met_gen()); 

      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[11] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_met_factor[12] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[13] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_met_factor[14] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[15] += met_prob_120(met_gen()); 
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_met_factor[16] += met_prob_120(met_gen()); 
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[17] += met_prob_120(met_gen()); 
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_met_factor[18] += met_prob_120(met_gen()); 

      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[21] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_met_factor[22] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[23] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_met_factor[24] += met_prob_50(met_gen())*(1.0-met_prob_120(met_gen())); 
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[25] += met_prob_120(met_gen()); 
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_met_factor[26] += met_prob_120(met_gen()); 
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_met_factor[27] += met_prob_120(met_gen()); 
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_met_factor[28] += met_prob_120(met_gen()); 

        //Observed
      if (jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[1]++;
      if (jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[2]++;
      if (jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[3]++;
      if (jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[4]++;
      if (jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 120) exptl_met_factor[5]++;
      if (jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && met_reco() > 120) exptl_met_factor[6]++;
      if (jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 120) exptl_met_factor[7]++;
      if (jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && met_reco() > 120) exptl_met_factor[8]++;

      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[11]++;
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[12]++;
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[13]++;
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[14]++;
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 120) exptl_met_factor[15]++;
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && met_reco() > 120) exptl_met_factor[16]++;
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 120) exptl_met_factor[17]++;
      if (nbtags_jets_gen() == 1 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && met_reco() > 120) exptl_met_factor[18]++;

      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[21]++;
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[22]++;
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[23]++;
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && met_reco() > 50 && met_reco() < 120) exptl_met_factor[24]++;
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 120) exptl_met_factor[25]++;
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && met_reco() > 120) exptl_met_factor[26]++;
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_reco() > 120) exptl_met_factor[27]++;
      if (nbtags_jets_gen() >= 2 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && met_reco() > 120) exptl_met_factor[28]++;

      //nJets Factorization
        //Predicted
      if (ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[1] += prob_2or3_jets(); 
      if (ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[2] += prob_2or3_jets(); 
      if (ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[3] += prob_geq4_jets(); 
      if (ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[4] += prob_geq4_jets(); 
      if (ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) theor_njets_factor[5] += prob_2or3_jets(); 
      if (ht_jets_gen() > 400 && met_gen() > 120) theor_njets_factor[6] += prob_2or3_jets(); 
      if (ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) theor_njets_factor[7] += prob_geq4_jets(); 
      if (ht_jets_gen() > 400 && met_gen() > 120) theor_njets_factor[8] += prob_geq4_jets(); 

      if (nbtags_jets_gen() == 1 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[11] += prob_2or3_jets(); 
      if (nbtags_jets_gen() == 1 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[12] += prob_2or3_jets(); 
      if (nbtags_jets_gen() == 1 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[13] += prob_geq4_jets(); 
      if (nbtags_jets_gen() == 1 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[14] += prob_geq4_jets(); 
      if (nbtags_jets_gen() == 1 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) theor_njets_factor[15] += prob_2or3_jets(); 
      if (nbtags_jets_gen() == 1 && ht_jets_gen() > 400 && met_gen() > 120) theor_njets_factor[16] += prob_2or3_jets(); 
      if (nbtags_jets_gen() == 1 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) theor_njets_factor[17] += prob_geq4_jets(); 
      if (nbtags_jets_gen() == 1 && ht_jets_gen() > 400 && met_gen() > 120) theor_njets_factor[18] += prob_geq4_jets(); 

      if (nbtags_jets_gen() >= 2 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[21] += prob_2or3_jets(); 
      if (nbtags_jets_gen() >= 2 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[22] += prob_2or3_jets(); 
      if (nbtags_jets_gen() >= 2 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[23] += prob_geq4_jets(); 
      if (nbtags_jets_gen() >= 2 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) theor_njets_factor[24] += prob_geq4_jets(); 
      if (nbtags_jets_gen() >= 2 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) theor_njets_factor[25] += prob_2or3_jets(); 
      if (nbtags_jets_gen() >= 2 && ht_jets_gen() > 400 && met_gen() > 120) theor_njets_factor[26] += prob_2or3_jets(); 
      if (nbtags_jets_gen() >= 2 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) theor_njets_factor[27] += prob_geq4_jets(); 
      if (nbtags_jets_gen() >= 2 && ht_jets_gen() > 400 && met_gen() > 120) theor_njets_factor[28] += prob_geq4_jets(); 

        //Observed
      if (njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[1]++;
      if (njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[2]++;
      if (njets_reco_direct() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[3]++;
      if (njets_reco_direct() >= 4 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[4]++;
      if (njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) exptl_njets_factor[5]++;
      if (njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 400 && met_gen() > 120) exptl_njets_factor[6]++;
      if (njets_reco_direct() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) exptl_njets_factor[7]++;
      if (njets_reco_direct() >= 4 && ht_jets_gen() > 400 && met_gen() > 120) exptl_njets_factor[8]++;

      if (nbtags_jets_gen() == 1 && njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[11]++;
      if (nbtags_jets_gen() == 1 && njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[12]++;
      if (nbtags_jets_gen() == 1 && njets_reco_direct() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[13]++;
      if (nbtags_jets_gen() == 1 && njets_reco_direct() >= 4 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[14]++;
      if (nbtags_jets_gen() == 1 && njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) exptl_njets_factor[15]++;
      if (nbtags_jets_gen() == 1 && njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 400 && met_gen() > 120) exptl_njets_factor[16]++;
      if (nbtags_jets_gen() == 1 && njets_reco_direct() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) exptl_njets_factor[17]++;
      if (nbtags_jets_gen() == 1 && njets_reco_direct() >= 4 && ht_jets_gen() > 400 && met_gen() > 120) exptl_njets_factor[18]++;

      if (nbtags_jets_gen() >= 2 && njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[21]++;
      if (nbtags_jets_gen() >= 2 && njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[22]++;
      if (nbtags_jets_gen() >= 2 && njets_reco_direct() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[23]++;
      if (nbtags_jets_gen() >= 2 && njets_reco_direct() >= 4 && ht_jets_gen() > 400 && met_gen() > 50 && met_gen() < 120) exptl_njets_factor[24]++;
      if (nbtags_jets_gen() >= 2 && njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) exptl_njets_factor[25]++;
      if (nbtags_jets_gen() >= 2 && njets_reco_direct() >= 2 && njets_reco_direct() < 4 && ht_jets_gen() > 400 && met_gen() > 120) exptl_njets_factor[26]++;
      if (nbtags_jets_gen() >= 2 && njets_reco_direct() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && met_gen() > 120) exptl_njets_factor[27]++;
      if (nbtags_jets_gen() >= 2 && njets_reco_direct() >= 4 && ht_jets_gen() > 400 && met_gen() > 120) exptl_njets_factor[28]++;

      //HT Factorization
        //Predicted
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4) theor_ht_factor[1] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4) theor_ht_factor[2] += ht_prob_jets_400(ht_jets_gen());
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4) theor_ht_factor[3] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4) theor_ht_factor[4] += ht_prob_jets_400(ht_jets_gen());
      if (met_gen() > 120 && jets_direct_gen().size() < 4) theor_ht_factor[5] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (met_gen() > 120 && jets_direct_gen().size() < 4) theor_ht_factor[6] += ht_prob_jets_400(ht_jets_gen());
      if (met_gen() > 120 && jets_direct_gen().size() >= 4) theor_ht_factor[7] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (met_gen() > 120 && jets_direct_gen().size() >= 4) theor_ht_factor[8] += ht_prob_jets_400(ht_jets_gen());

      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4) theor_ht_factor[11] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4) theor_ht_factor[12] += ht_prob_jets_400(ht_jets_gen());
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4) theor_ht_factor[13] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4) theor_ht_factor[14] += ht_prob_jets_400(ht_jets_gen());
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() < 4) theor_ht_factor[15] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() < 4) theor_ht_factor[16] += ht_prob_jets_400(ht_jets_gen());
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() >= 4) theor_ht_factor[17] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() >= 4) theor_ht_factor[18] += ht_prob_jets_400(ht_jets_gen());

      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4) theor_ht_factor[21] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4) theor_ht_factor[22] += ht_prob_jets_400(ht_jets_gen());
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4) theor_ht_factor[23] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4) theor_ht_factor[24] += ht_prob_jets_400(ht_jets_gen());
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() < 4) theor_ht_factor[25] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() < 4) theor_ht_factor[26] += ht_prob_jets_400(ht_jets_gen());
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() >= 4) theor_ht_factor[27] += ht_prob_jets_200(ht_jets_gen())*(1.0-ht_prob_jets_400(ht_jets_gen()));
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() >= 4) theor_ht_factor[28] += ht_prob_jets_400(ht_jets_gen());

        //Observed
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[1]++;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) exptl_ht_factor[2]++;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[3]++;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) exptl_ht_factor[4]++;
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[5]++;
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) exptl_ht_factor[6]++;
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[7]++;
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) exptl_ht_factor[8]++;

      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[11]++;
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) exptl_ht_factor[12]++;
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[13]++;
      if (nbtags_jets_gen() == 1 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) exptl_ht_factor[14]++;
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[15]++;
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) exptl_ht_factor[16]++;
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[17]++;
      if (nbtags_jets_gen() == 1 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) exptl_ht_factor[18]++;

      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[21]++;
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) exptl_ht_factor[22]++;
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[23]++;
      if (nbtags_jets_gen() >= 2 && met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) exptl_ht_factor[24]++;
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[25]++;
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) exptl_ht_factor[26]++;
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) exptl_ht_factor[27]++;
      if (nbtags_jets_gen() >= 2 && met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) exptl_ht_factor[28]++;

      //nBtags Factorization
        //Predicted
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_nbtags_factor[11] += prob_one_btag();
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_nbtags_factor[12] += prob_one_btag();
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_nbtags_factor[13] += prob_one_btag();
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_nbtags_factor[14] += prob_one_btag();
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_nbtags_factor[15] += prob_one_btag();
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_nbtags_factor[16] += prob_one_btag();
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_nbtags_factor[17] += prob_one_btag();
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_nbtags_factor[18] += prob_one_btag();

      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_nbtags_factor[21] += 1.0-prob_one_btag()-prob_zero_btag();
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_nbtags_factor[22] += 1.0-prob_one_btag()-prob_zero_btag();
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_nbtags_factor[23] += 1.0-prob_one_btag()-prob_zero_btag();
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_nbtags_factor[24] += 1.0-prob_one_btag()-prob_zero_btag();
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_nbtags_factor[25] += 1.0-prob_one_btag()-prob_zero_btag();
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400) theor_nbtags_factor[26] += 1.0-prob_one_btag()-prob_zero_btag();
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400) theor_nbtags_factor[27] += 1.0-prob_one_btag()-prob_zero_btag();
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400) theor_nbtags_factor[28] += 1.0-prob_one_btag()-prob_zero_btag();

        //Observed
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && nbtags_jets_reco2() == 1) exptl_nbtags_factor[11] += 1;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && nbtags_jets_reco2() == 1) exptl_nbtags_factor[12] += 1;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && nbtags_jets_reco2() == 1) exptl_nbtags_factor[13] += 1;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && nbtags_jets_reco2() == 1) exptl_nbtags_factor[14] += 1;
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && nbtags_jets_reco2() == 1) exptl_nbtags_factor[15] += 1;
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && nbtags_jets_reco2() == 1) exptl_nbtags_factor[16] += 1;
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && nbtags_jets_reco2() == 1) exptl_nbtags_factor[17] += 1;
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && nbtags_jets_reco2() == 1) exptl_nbtags_factor[18] += 1;

      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && nbtags_jets_reco2() >= 2) exptl_nbtags_factor[21] += 1;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && nbtags_jets_reco2() >= 2) exptl_nbtags_factor[22] += 1;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && nbtags_jets_reco2() >= 2) exptl_nbtags_factor[23] += 1;
      if (met_gen() > 50 && met_gen() < 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && nbtags_jets_reco2() >= 2) exptl_nbtags_factor[24] += 1;
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && nbtags_jets_reco2() >= 2) exptl_nbtags_factor[25] += 1;
      if (met_gen() > 120 && jets_direct_gen().size() < 4 && ht_jets_gen() > 400 && nbtags_jets_reco2() >= 2) exptl_nbtags_factor[26] += 1;
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 200 && ht_jets_gen() < 400 && nbtags_jets_reco2() >= 2) exptl_nbtags_factor[27] += 1;
      if (met_gen() > 120 && jets_direct_gen().size() >= 4 && ht_jets_gen() > 400 && nbtags_jets_reco2() >= 2) exptl_nbtags_factor[28] += 1;
    }//verbose loop
  }//events loop

  cout << "Total closure: " << theor_overall[0] << " " << exptl_overall[0] << " = " << float(theor_overall[0])/float(exptl_overall[0]) << " pm " << sqrt( pow((float(sqrt(theor_overall[0]))/float(theor_overall[0])), 2) + pow((float(sqrt(exptl_overall[0]))/float(exptl_overall[0])), 2))    << endl;

  //Print out SR yields
  cout << "SR      Result    #predicted     #observed" << endl; 
  for (int i = 1; i < 9; i++){
    cout << "SR " << i << ": " << theor_overall[i]/exptl_overall[i] << " pm " << sqrt( pow((float(sqrt(theor_overall[i]))/float(theor_overall[i])), 2) + pow((float(sqrt(exptl_overall[i]))/float(exptl_overall[i])), 2))  << "         "  << endl;
    if (verbose) cout << "  ID/iso: " << theor_id_factor[i]/exptl_id_factor[i] << endl;
    if (verbose) cout << "  MET: " << theor_met_factor[i]/exptl_met_factor[i] << endl;
    if (verbose) cout << "  ht: " << theor_ht_factor[i]/exptl_ht_factor[i] << endl;
    if (verbose) cout << "  njets: " << theor_njets_factor[i]/exptl_njets_factor[i] << endl;
    if (verbose) cout << "  product of these: " << (theor_id_factor[i]/exptl_id_factor[i])*(theor_met_factor[i]/exptl_met_factor[i])*(theor_ht_factor[i]/exptl_ht_factor[i])*(theor_njets_factor[i]/exptl_njets_factor[i]) << endl;
  }
  for (int i = 11; i <29; i++){
    if (i == 19 || i == 20) continue;
    cout << "SR " << i << ": " << theor_overall[i]/exptl_overall[i] << " pm " << sqrt( pow((float(sqrt(theor_overall[i]))/float(theor_overall[i])), 2) + pow((float(sqrt(exptl_overall[i]))/float(exptl_overall[i])), 2))  << "        " << endl;
    if (verbose) cout << "  ID/iso: " << theor_id_factor[i]/exptl_id_factor[i] << endl;
    if (verbose) cout << "  MET: " << theor_met_factor[i]/exptl_met_factor[i] << endl;
    if (verbose) cout << "  ht: " << theor_ht_factor[i]/exptl_ht_factor[i] << endl;
    if (verbose) cout << "  njets: " << theor_njets_factor[i]/exptl_njets_factor[i] << endl;
    if (verbose) cout << "  nbtags: " << theor_nbtags_factor[i]/exptl_nbtags_factor[i] << endl;
    if (verbose) cout << "  product of these: " << (theor_id_factor[i]/exptl_id_factor[i])*(theor_met_factor[i]/exptl_met_factor[i])*(theor_ht_factor[i]/exptl_ht_factor[i])*(theor_njets_factor[i]/exptl_njets_factor[i])*(theor_nbtags_factor[i]/exptl_nbtags_factor[i]) << endl;
  }
}
