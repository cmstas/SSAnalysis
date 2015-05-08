#include "SYST.h"
SYST systematics;
namespace syst {
	const vector<string> &weightsID() { return systematics.weightsID(); }
	const bool &keep() { return systematics.keep(); }
	const double &qScale() { return systematics.qScale(); }
	const float &ht() { return systematics.ht(); }
	const float &met() { return systematics.met(); }
	const float &metphi() { return systematics.metphi(); }
	const float &iso1() { return systematics.iso1(); }
	const float &iso2() { return systematics.iso2(); }
	const float &weight() { return systematics.weight(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >  &lep1() { return systematics.lep1(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >  &lep2() { return systematics.lep2(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genBs() { return systematics.genBs(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genjet() { return systematics.genjet(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps() { return systematics.genps(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_mother_p4() { return systematics.gen_mother_p4(); }
	const vector<float> &jet_eta() { return systematics.jet_eta(); }
	const vector<float> &jet_phi() { return systematics.jet_phi(); }
	const vector<float> &jet_pt() { return systematics.jet_pt(); }
	const vector<float> &gen_eta() { return systematics.gen_eta(); }
	const vector<float> &gen_phi() { return systematics.gen_phi(); }
	const vector<float> &gen_pt() { return systematics.gen_pt(); }
	const vector<float> &weights() { return systematics.weights(); }
	const int &gen_gmother_id() { return systematics.gen_gmother_id(); }
	const int &gramid1() { return systematics.gramid1(); }
	const int &id1() { return systematics.id1(); }
	const int &gramid2() { return systematics.gramid2(); }
	const int &id2() { return systematics.id2(); }
	const int &nBtags() { return systematics.nBtags(); }
	const int &nJets40() { return systematics.nJets40(); }
	const int &nLep() { return systematics.nLep(); }
	const vector<int> &genBIDs() { return systematics.genBIDs(); }
	const vector<int> &gen_id() { return systematics.gen_id(); }
	const vector<int> &gen_matchtojet() { return systematics.gen_matchtojet(); }
	const vector<int> &gen_mother_id() { return systematics.gen_mother_id(); }
	const vector<int> &gen_status() { return systematics.gen_status(); }
}
