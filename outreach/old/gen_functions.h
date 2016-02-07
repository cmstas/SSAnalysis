#ifndef GENFUNCTIONS_H
#define GENFUNCTIONS_H

#include "Math/VectorUtil.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

struct GenParticleStruct {
    int id;
    int idx;
    float pt;
    float eta;
    int did;
    int didx;
    float dpt;
    float deta;
};

enum dileptonCharge {OS, SS, BOTH};
enum dileptonType {EE, EM, MM, ALL};

namespace gen_tools{
  bool leptonOverlapsWithParton(const LorentzVector& p4, float min_parton_pt = 20.0, float dr = 0.3);

  vector<LorentzVector> getGenJets (float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR = 0.4);
  
  vector<LorentzVector> getGenJetsDirectly (float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR = 0.4);

  float getGenHT(float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR = 0.4);

  float getGenHTdirectly(float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR = 0.4);
  
  vector <LorentzVector> getGenBjets(float pt_cut, float eta_cut, float muon_pt_cut, float electron_pt_cut, float deltaR = 0.4);

  vector <LorentzVector> getGenBjetsDirectly(float pt_cut, float eta_cut, float muon_pt_cut, float electron_pt_cut, float deltaR = 0.4);
  
  dileptonType getHypType(int id1, int id2);
  
  pair<GenParticleStruct, GenParticleStruct> getGenHyp(float min_pt_upper, float min_pt_lower, dileptonCharge hypType);
  
  vector <pair <GenParticleStruct, GenParticleStruct> > makeGenHyps(float eta_cut = 2.4, bool removeLeptonsOverlappingWithPartons = false, bool includeTaus = true);
}



#endif
