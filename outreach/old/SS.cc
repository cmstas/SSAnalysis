#include "SS.h"
SS samesign;
namespace ss {
	const float &met_gen() { return samesign.met_gen(); }
	const float &ht_gen() { return samesign.ht_gen(); }
	const float &ht_jets_gen() { return samesign.ht_jets_gen(); }
	const int &njets_gen() { return samesign.njets_gen(); }
	const int &njets_jets_gen() { return samesign.njets_jets_gen(); }
	const int &nbtags_gen() { return samesign.nbtags_gen(); }
	const int &nbtags_jets_gen() { return samesign.nbtags_jets_gen(); }
	const float &pt1_gen() { return samesign.pt1_gen(); }
	const float &pt2_gen() { return samesign.pt2_gen(); }
	const float &eta1_gen() { return samesign.eta1_gen(); }
	const float &eta2_gen() { return samesign.eta2_gen(); }
	const int &id1_gen() { return samesign.id1_gen(); }
	const int &id2_gen() { return samesign.id2_gen(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_gen() { return samesign.jets_gen(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_direct_gen() { return samesign.jets_direct_gen(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_gen() { return samesign.btags_gen(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_direct_gen() { return samesign.btags_direct_gen(); }
	const bool &choseRightHyp() { return samesign.choseRightHyp(); }
	const int &id1_reco() { return samesign.id1_reco(); }
	const int &id2_reco() { return samesign.id2_reco(); }
	const float &pt1_reco() { return samesign.pt1_reco(); }
	const float &pt2_reco() { return samesign.pt2_reco(); }
	const float &eta1_reco() { return samesign.eta1_reco(); }
	const float &eta2_reco() { return samesign.eta2_reco(); }
	const float &met_reco() { return samesign.met_reco(); }
	const float &ht_reco() { return samesign.ht_reco(); }
	const float &ht_direct_reco() { return samesign.ht_direct_reco(); }
	const bool &isGoodLepton_1() { return samesign.isGoodLepton_1(); }
	const bool &isIsolatedLepton_1() { return samesign.isIsolatedLepton_1(); }
	const bool &isNumeratorLepton_1() { return samesign.isNumeratorLepton_1(); }
	const bool &isGoodLepton_2() { return samesign.isGoodLepton_2(); }
	const bool &isIsolatedLepton_2() { return samesign.isIsolatedLepton_2(); }
	const bool &isNumeratorLepton_2() { return samesign.isNumeratorLepton_2(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco() { return samesign.jets_reco(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco_direct() { return samesign.jets_reco_direct(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco_cms2() { return samesign.jets_reco_cms2(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_reco() { return samesign.btags_reco(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_jets_reco() { return samesign.btags_jets_reco(); }
	const int &njets_reco() { return samesign.njets_reco(); }
	const int &njets_reco_direct() { return samesign.njets_reco_direct(); }
	const int &njets_reco_cms2() { return samesign.njets_reco_cms2(); }
	const int &nbtags_reco() { return samesign.nbtags_reco(); }
	const int &nbtags_jets_reco() { return samesign.nbtags_jets_reco(); }
	const vector<bool> &gen_btag_is_matched() { return samesign.gen_btag_is_matched(); }
	const vector<bool> &gen_btag_is_matched_direct() { return samesign.gen_btag_is_matched_direct(); }
	const vector<bool> &jet_is_matched() { return samesign.jet_is_matched(); }
	const vector<bool> &jet_is_matched_direct() { return samesign.jet_is_matched_direct(); }
	const vector<float> &sparm_value() { return samesign.sparm_value(); }
	const vector<TString> &sparm_name() { return samesign.sparm_name(); }
}
