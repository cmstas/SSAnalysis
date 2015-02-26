#include "lepfilter.h"
lepfilter ss;
namespace samesign {
	float &met() { return ss.met(); }
	float &metPhi() { return ss.metPhi(); }
	int &event() { return ss.event(); }
	int &lumi() { return ss.lumi(); }
	int &run() { return ss.run(); }
	bool &filt_csc() { return ss.filt_csc(); }
	bool &filt_hbhe() { return ss.filt_hbhe(); }
	bool &filt_hcallaser() { return ss.filt_hcallaser(); }
	bool &filt_ecaltp() { return ss.filt_ecaltp(); }
	bool &filt_trkfail() { return ss.filt_trkfail(); }
	bool &filt_eebadsc() { return ss.filt_eebadsc(); }
	bool &is_real_data() { return ss.is_real_data(); }
	float &scale1fb() { return ss.scale1fb(); }
	float &xsec() { return ss.xsec(); }
	float &kfactor() { return ss.kfactor(); }
	float &gen_met() { return ss.gen_met(); }
	float &gen_met_phi() { return ss.gen_met_phi(); }
	float &njets() { return ss.njets(); }
	float &ht() { return ss.ht(); }
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets() { return ss.jets(); }
	vector<float> &jets_disc() { return ss.jets_disc(); }
	TString &sample() { return ss.sample(); }
	int &nFOs() { return ss.nFOs(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &p4() { return ss.p4(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &mc_p4() { return ss.mc_p4(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &mc_motherp4() { return ss.mc_motherp4(); }
	int &id() { return ss.id(); }
	int &idx() { return ss.idx(); }
	float &d0() { return ss.d0(); }
	float &dZ() { return ss.dZ(); }
	float &d0_err() { return ss.d0_err(); }
	int &motherID() { return ss.motherID(); }
	int &mc_id() { return ss.mc_id(); }
	float &iso() { return ss.iso(); }
	bool &passes_id() { return ss.passes_id(); }
	bool &passes_id_ptrel() { return ss.passes_id_ptrel(); }
	bool &FO() { return ss.FO(); }
	bool &FO_ptrel() { return ss.FO_ptrel(); }
	bool &FO_NoIso() { return ss.FO_NoIso(); }
	float &ip3d() { return ss.ip3d(); }
	float &ip3derr() { return ss.ip3derr(); }
	int &type() { return ss.type(); }
	float &mt() { return ss.mt(); }
	float &ptrelv0() { return ss.ptrelv0(); }
	float &ptrelv1() { return ss.ptrelv1(); }
	float &el_sigmaIEtaIEta_full5x5() { return ss.el_sigmaIEtaIEta_full5x5(); }
	float &el_etaSC() { return ss.el_etaSC(); }
	float &el_dEtaIn() { return ss.el_dEtaIn(); }
	float &el_dPhiIn() { return ss.el_dPhiIn(); }
	float &el_hOverE() { return ss.el_hOverE(); }
	float &el_ecalEnergy() { return ss.el_ecalEnergy(); }
	float &el_eOverPIn() { return ss.el_eOverPIn(); }
	bool &el_conv_vtx_flag() { return ss.el_conv_vtx_flag(); }
	int &el_exp_innerlayers() { return ss.el_exp_innerlayers(); }
	int &el_charge() { return ss.el_charge(); }
	int &el_sccharge() { return ss.el_sccharge(); }
	int &el_ckf_charge() { return ss.el_ckf_charge(); }
	bool &el_threeChargeAgree() { return ss.el_threeChargeAgree(); }
	int &mu_pid_PFMuon() { return ss.mu_pid_PFMuon(); }
	float &mu_gfit_chi2() { return ss.mu_gfit_chi2(); }
	float &mu_gfit_ndof() { return ss.mu_gfit_ndof(); }
	int &mu_gfit_validSTAHits() { return ss.mu_gfit_validSTAHits(); }
	int &mu_numberOfMatchedStations() { return ss.mu_numberOfMatchedStations(); }
	int &mu_validPixelHits() { return ss.mu_validPixelHits(); }
	int &mu_nlayers() { return ss.mu_nlayers(); }
}
