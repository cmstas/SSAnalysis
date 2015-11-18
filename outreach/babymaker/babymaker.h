#ifndef SSOUTREACH_H
#define SSOUTREACH_H

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

vector<LorentzVector> getGenJets(float pt_cut = 40, float eta_cut = 2.4, float muon_pt_cut = 10, float elec_pt_cut = 15, float deltaR = 0.1);
std::pair<GenParticleStruct, GenParticleStruct> getGenHyp(float min_pt_elec, float min_pt_muon);
vector <pair<GenParticleStruct, GenParticleStruct> > makeGenHyps();
hyp_type_t getHypType(int id1, int id2);

#endif
