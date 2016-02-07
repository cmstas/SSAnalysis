#include <vector>

#include "/home/users/cgeorge/CORE/CMS2.h"
#include "/home/users/cgeorge/CORE/ssSelections.h"
#include "gen_functions.h"

using namespace tas;

bool gen_tools::leptonOverlapsWithParton(const LorentzVector& p4, float min_parton_pt, float dr) {
    
    for (unsigned int idx = 0; idx < cms2.genps_p4().size(); idx++) {
    
        if (cms2.genps_status().at(idx) != 3)
            continue;
        if ((abs(cms2.genps_id().at(idx)) < 1 || abs(cms2.genps_id().at(idx)) > 5) && abs(cms2.genps_id().at(idx)) != 21) // require a quark or gluon
            continue;
        if (cms2.genps_p4().at(idx).pt() < min_parton_pt)
            continue;
        if (ROOT::Math::VectorUtil::DeltaR(cms2.genps_p4().at(idx), p4) < dr) 
            return true;
    }   

    return false;
}

dileptonType gen_tools::getHypType(int id1, int id2){
        
    if (abs(id1) != 11 && abs(id1) != 13) 
        return ALL;
    if (abs(id2) != 11 && abs(id2) != 13) 
        return ALL;

    if (abs(id1) == 11 && abs(id2) == 11) 
        return EE; 
    else if (abs(id1) == 13 && abs(id2) == 13) 
        return MM; 
    else
        return EM; 
}

vector<LorentzVector> gen_tools::getGenJetsDirectly (float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR){
  vector<LorentzVector> tmp_vec;
  tmp_vec.clear();
  for (unsigned int jidx = 0; jidx < genjets_p4().size(); jidx++){
    LorentzVector vjet = cms2.genjets_p4().at(jidx);
    bool jetislep = false;
    if (genjets_p4().at(jidx).pt() < pt_cut) continue;
    if (fabs(genjets_p4().at(jidx).eta()) > eta_cut) continue;
    for (size_t i = 0; i < genps_p4().size(); i++){
      if (genps_status().at(i) != 3) continue;
      if (abs(genps_id().at(i) != 11) && abs(genps_id().at(i)) != 13) continue;
      if (fabs(genps_p4().at(i).eta()) < 2.4) continue;
      if (abs(genps_id().at(i)) == 11 && genps_p4().at(i).pt() < elec_pt_cut) continue;
      if (abs(genps_id().at(i)) == 13 && genps_p4().at(i).pt() < muon_pt_cut) continue;
      if (ROOT::Math::VectorUtil::DeltaR(vjet, genps_p4().at(i)) < deltaR){ jetislep = true; break; }
    }
    if (jetislep == true) continue;
    tmp_vec.push_back(vjet);
  }//for-loop

  return tmp_vec;
}

vector<LorentzVector> gen_tools::getGenJets (float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR){
    vector<LorentzVector> tmp_vec;
    bool past_tbar = false;
    bool past_t = false; //Note: this step is necessary to avoid particles that do not become jets
    tmp_vec.clear();
    for (unsigned int gidx = 0; gidx < cms2.genps_status().size(); gidx++) {
        LorentzVector vjet = cms2.genps_p4().at(gidx);
        bool jetislep = false;
        if (cms2.genps_id().at(gidx) == 6) past_tbar = true;
        if (cms2.genps_id().at(gidx) == -6) past_t = true;
        if (past_t == 0 || past_tbar == 0) continue;
        if (cms2.genps_status().at(gidx) != 3) continue;
        if ((abs(cms2.genps_id().at(gidx)) < 1 || abs(cms2.genps_id().at(gidx)) > 5) && abs(cms2.genps_id().at(gidx)) != 21) continue;
        if (fabs(cms2.genps_p4().at(gidx).eta()) > eta_cut) continue;
        if (cms2.genps_p4().at(gidx).pt() < pt_cut) continue;
        for (size_t i = 0; i < genps_p4().size(); i++){
          if (genps_status().at(i) != 3) continue;
          if (abs(genps_id().at(i) != 11) && abs(genps_id().at(i)) != 13) continue;
          if (i == gidx) continue;
          if (fabs(genps_p4().at(i).eta()) < 2.4) continue;
          if (abs(genps_id().at(i)) == 11 && genps_p4().at(i).pt() < elec_pt_cut) continue;
          if (abs(genps_id().at(i)) == 13 && genps_p4().at(i).pt() < muon_pt_cut) continue;
          if (ROOT::Math::VectorUtil::DeltaR(vjet, genps_p4().at(i)) < deltaR){ jetislep = true; break; }
        }
        if (jetislep == true) continue;
        tmp_vec.push_back(vjet);
    }

    return tmp_vec;
}

float gen_tools::getGenHTdirectly(float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR){
    vector <LorentzVector> tmp_vec = getGenJetsDirectly(pt_cut, eta_cut, muon_pt_cut, elec_pt_cut, deltaR);
    float tmp_ht = 0.;
    for(unsigned int idx = 0; idx < tmp_vec.size(); idx++) {
        tmp_ht += tmp_vec.at(idx).pt();
    }

    return tmp_ht;
}

float gen_tools::getGenHT(float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR){
    vector <LorentzVector> tmp_vec = getGenJets(pt_cut, eta_cut, muon_pt_cut, elec_pt_cut, deltaR);
    float tmp_ht = 0.;
    for(unsigned int idx = 0; idx < tmp_vec.size(); idx++) {
        tmp_ht += tmp_vec.at(idx).pt();
    }

    return tmp_ht;
}

vector<LorentzVector> gen_tools::getGenBjetsDirectly(float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR){
  vector<LorentzVector> tmp_vec;
  tmp_vec.clear();
  int id = 100000;
  bool matched = false;
  float closest = 0.4;
  float tmp_matched_pt = 10000;
  for (unsigned int jidx = 0; jidx < genjets_p4().size(); jidx++){
    matched = false;
    closest = 0.4;
    tmp_matched_pt = 10000;
    LorentzVector vjet = genjets_p4().at(jidx);
    bool jetislep = false;
    if (genjets_p4().at(jidx).pt() < pt_cut) continue;
    if (fabs(genjets_p4().at(jidx).eta()) > eta_cut) continue;
    for (size_t i = 0; i < genps_p4().size(); i++){
      if (genps_status().at(i) != 3) continue;
      if (abs(genps_id().at(i) != 11) && abs(genps_id().at(i)) != 13) continue;
      if (fabs(genps_p4().at(i).eta()) < 2.4) continue;
      if (abs(genps_id().at(i)) == 11 && genps_p4().at(i).pt() < elec_pt_cut) continue;
      if (abs(genps_id().at(i)) == 13 && genps_p4().at(i).pt() < muon_pt_cut) continue;
      if (ROOT::Math::VectorUtil::DeltaR(vjet, genps_p4().at(i)) < deltaR){ jetislep = true; break; }
    }
    //Add b-tagging requirement
    for (size_t i = 0; i < genps_p4().size(); i++){
      if (genps_status().at(i) != 3) continue;
      if (fabs(genps_p4().at(i).eta()) > 2.4) continue;
      if ((abs(genps_id().at(i)) < -1 || abs(genps_id().at(i)) > 5) && abs(genps_id().at(i)) != 21) continue;
      if (ROOT::Math::VectorUtil::DeltaR(vjet, genps_p4().at(i)) < closest){ 
        matched = true; 
        closest = ROOT::Math::VectorUtil::DeltaR(vjet, genps_p4().at(i));
        id = genps_id().at(i);
        tmp_matched_pt = genps_p4().at(i).pt();
        //cout << "matched" << endl;
      } 
    }
    if (!matched) continue;
    if (abs(id) != 5) continue;
    if (jetislep == true) continue;
    tmp_vec.push_back(vjet);
    //cout << "Mass difference: " << tmp_matched_pt << " - " << vjet.pt() << " = " << fabs(tmp_matched_pt - vjet.pt()) << endl;
    //cout << "DeltaR: " << fabs(closest) << endl;
  }//for-loop
  return tmp_vec;
}

vector <LorentzVector> gen_tools::getGenBjets(float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR){
    bool past_tbar = false;
    bool past_t = false; //Note: this step is necessary to avoid particles that do not become jets
    vector<LorentzVector> tmp_vec;
    tmp_vec.clear();

    for (unsigned int gidx = 0; gidx < cms2.genps_status().size(); gidx++) {
        LorentzVector vjet = cms2.genps_p4().at(gidx);
        bool jetislep = false;
        if (cms2.genps_id().at(gidx) == 6) past_tbar = true;
        if (cms2.genps_id().at(gidx) == -6) past_t = true;
        if (past_t == 0 || past_tbar == 0)  continue;
        if (cms2.genps_status().at(gidx) != 3) continue;
        if (abs(cms2.genps_id().at(gidx)) != 5)  continue;
        if (fabs(cms2.genps_p4().at(gidx).eta()) > eta_cut) continue;
        if (cms2.genps_p4().at(gidx).pt() < pt_cut) continue;
        for (size_t i = 0; i < genps_p4().size(); i++){
          if (genps_status().at(i) != 3) continue;
          if (abs(genps_id().at(i) != 11) && abs(genps_id().at(i)) != 13) continue;
          if (i == gidx) continue;
          if (fabs(genps_p4().at(i).eta()) < 2.4) continue;
          if (abs(genps_id().at(i)) == 11 && genps_p4().at(i).pt() < elec_pt_cut) continue;
          if (abs(genps_id().at(i)) == 13 && genps_p4().at(i).pt() < muon_pt_cut) continue;
          if (ROOT::Math::VectorUtil::DeltaR(vjet, genps_p4().at(i)) < deltaR){ jetislep = true; break; }
        }
        if (jetislep == true) continue;


        tmp_vec.push_back(vjet);
    }

    return tmp_vec;
}

vector <pair <GenParticleStruct, GenParticleStruct> > gen_tools::makeGenHyps(float eta_cut, bool removeLeptonsOverlappingWithPartons, bool includeTaus) {

    // loop over gen particles
    vector <GenParticleStruct> gleps;
    gleps.clear();
    for (unsigned int gidx = 0; gidx < cms2.genps_p4().size(); gidx++) {

        if (cms2.genps_status().at(gidx) != 3) continue;

        int id    = cms2.genps_id().at(gidx);
        float pt  = cms2.genps_p4().at(gidx).pt();
        float eta = cms2.genps_p4().at(gidx).eta();

        if (fabs(eta) > eta_cut) continue;

        if (abs(id) == 11 || abs(id) == 13) {
            if (removeLeptonsOverlappingWithPartons && gen_tools::leptonOverlapsWithParton(cms2.genps_p4().at(gidx))) continue;

            GenParticleStruct temp = {id, gidx, pt, eta, -999, 999, -999., 999.};

            gleps.push_back(temp);
        }

        if (includeTaus && abs(id) == 15) {

            GenParticleStruct tmp_gp =  {0,0,0,0,0,0,0,0};
            for (unsigned int didx = 0; didx < cms2.genps_lepdaughter_id().at(gidx).size(); didx++) {

                int did = cms2.genps_lepdaughter_id().at(gidx).at(didx);
                if (abs(did) != 11 && abs(did) != 13) continue;
             if (fabs(cms2.genps_lepdaughter_p4().at(gidx).at(didx).eta()) > eta_cut) continue;

                if (removeLeptonsOverlappingWithPartons && gen_tools::leptonOverlapsWithParton(cms2.genps_lepdaughter_p4().at(gidx).at(didx))) continue;

                float dpt  = cms2.genps_lepdaughter_p4().at(gidx).at(didx).pt();
                float deta = cms2.genps_lepdaughter_p4().at(gidx).at(didx).eta();
                GenParticleStruct temp2 = {id, gidx, pt, eta, did, didx, dpt, deta};
                if (dpt > tmp_gp.dpt) tmp_gp = temp2;
            }

            if (tmp_gp.dpt > 0.1) gleps.push_back(tmp_gp);

        } // end tau block
    } // end loop over gen particles

    // now make gen hyps
    vector <pair <GenParticleStruct, GenParticleStruct> > glepPairs;
    for (unsigned int idx1 = 0; idx1 < gleps.size(); idx1++) {
        for (unsigned int idx2 = idx1 + 1; idx2 < gleps.size(); idx2++) {
            glepPairs.push_back(make_pair(gleps.at(idx1), gleps.at(idx2)));
        }
    }// loop to make gen pairs

    return glepPairs;
}

pair<GenParticleStruct, GenParticleStruct> gen_tools::getGenHyp(float min_pt_elec, float min_pt_muon, dileptonCharge hypType) {
                
    vector <pair<GenParticleStruct, GenParticleStruct> > glepPairs = makeGenHyps();
    unsigned int npairs = glepPairs.size();

    GenParticleStruct gp = {0, 999999, 0., 0., 0, 999999, 0., 0.};
    pair<GenParticleStruct, GenParticleStruct> good_gen_hyp = make_pair(gp, gp);
    dileptonType good_hyp_type = ALL;
                
    for (unsigned int idx = 0; idx < npairs; idx++){
            
        GenParticleStruct gp1 = glepPairs.at(idx).first;
        GenParticleStruct gp2 = glepPairs.at(idx).second;

        float gpt1  = (abs(gp1.id) == 15) ? gp1.dpt : gp1.pt;
        int gid1    = (abs(gp1.id) == 15) ? gp1.did : gp1.id;
        float gpt2  = (abs(gp2.id) == 15) ? gp2.dpt : gp2.pt;
        int gid2    = (abs(gp2.id) == 15) ? gp2.did : gp2.id;
    
        if (gid1 * gid2 < 0 && hypType == SS) continue;
        else if (gid1 * gid2 > 0 && hypType == OS) continue;
       
        if (abs(gp1.id) == 11 && gp1.pt < min_pt_elec) continue;     
        if (abs(gp2.id) == 11 && gp2.pt < min_pt_elec) continue;     
        if (abs(gp1.id) == 13 && gp1.pt < min_pt_muon) continue;     
        if (abs(gp2.id) == 13 && gp2.pt < min_pt_muon) continue;     
        if (abs(gp1.id) == 15 && abs(gp1.did) == 11 && gp1.pt < min_pt_elec) continue;
        if (abs(gp2.id) == 15 && abs(gp2.did) == 11 && gp2.pt < min_pt_elec) continue;
        if (abs(gp1.id) == 15 && abs(gp1.did) == 13 && gp1.pt < min_pt_muon) continue;
        if (abs(gp2.id) == 15 && abs(gp2.did) == 13 && gp2.pt < min_pt_muon) continue;
    
        dileptonType tmp_type = getHypType(gid1, gid2);
        if (tmp_type == ALL) continue;
        if (good_hyp_type == ALL){
            good_hyp_type = tmp_type;
            good_gen_hyp = glepPairs.at(idx);
        }
        else if (tmp_type < good_hyp_type) {
            good_hyp_type = tmp_type;
            good_gen_hyp = glepPairs.at(idx);
        }
        else if (tmp_type == good_hyp_type) {

            float ggpt1 = (abs(good_gen_hyp.first.id) == 15) ? good_gen_hyp.first.dpt : good_gen_hyp.first.pt;
            float ggpt2 = (abs(good_gen_hyp.second.id) == 15) ? good_gen_hyp.second.dpt : good_gen_hyp.second.pt;

            if ( (gpt1+gpt2) > (ggpt1+ggpt2) ) {
                good_hyp_type = tmp_type;
                good_gen_hyp = glepPairs.at(idx);
            }
        }
    }// end loop over gen hyps

    return good_gen_hyp;
}

