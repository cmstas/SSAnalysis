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
	int &njets() { return ss.njets(); }
	int &best_hyp() { return ss.best_hyp(); }
	int &hyp_class() { return ss.hyp_class(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4() { return ss.lep1_p4(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4() { return ss.lep2_p4(); }
	float &ht() { return ss.ht(); }
	int &lep1_motherID() { return ss.lep1_motherID(); }
	int &lep2_motherID() { return ss.lep2_motherID(); }
	int &lep1_mc_id() { return ss.lep1_mc_id(); }
	int &lep2_mc_id() { return ss.lep2_mc_id(); }
	int &lep1_id() { return ss.lep1_id(); }
	int &lep2_id() { return ss.lep2_id(); }
	int &lep1_idx() { return ss.lep1_idx(); }
	int &lep2_idx() { return ss.lep2_idx(); }
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets() { return ss.jets(); }
	vector<float> &btags_disc() { return ss.btags_disc(); }
	vector<float> &jets_disc() { return ss.jets_disc(); }
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags() { return ss.btags(); }
	int &nbtags() { return ss.nbtags(); }
	float &sf_dilepTrig_hpt() { return ss.sf_dilepTrig_hpt(); }
	float &sf_dilepTrig_lpt() { return ss.sf_dilepTrig_lpt(); }
	float &sf_dilepTrig_vlpt() { return ss.sf_dilepTrig_vlpt(); }
	int &hyp_type() { return ss.hyp_type(); }
	float &sf_dilep_eff() { return ss.sf_dilep_eff(); }
	float &mt() { return ss.mt(); }
	float &mt_l2() { return ss.mt_l2(); }
	float &mt2() { return ss.mt2(); }
	float &mGluino() { return ss.mGluino(); }
	float &mLSP() { return ss.mLSP(); }
	float &mSbottom() { return ss.mSbottom(); }
	float &mChargino() { return ss.mChargino(); }
	int &lep1_id_gen() { return ss.lep1_id_gen(); }
	int &lep2_id_gen() { return ss.lep2_id_gen(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4_gen() { return ss.lep1_p4_gen(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4_gen() { return ss.lep2_p4_gen(); }
	int &lep3_id() { return ss.lep3_id(); }
	int &lep3_idx() { return ss.lep3_idx(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep3_p4() { return ss.lep3_p4(); }
	int &lep3_quality() { return ss.lep3_quality(); }
	float &lep1_iso() { return ss.lep1_iso(); }
	float &lep2_iso() { return ss.lep2_iso(); }
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4() { return ss.dilep_p4(); }
	TString &sample() { return ss.sample(); }
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps_p4() { return ss.genps_p4(); }
	vector<int> &genps_id() { return ss.genps_id(); }
	vector<int> &genps_id_mother() { return ss.genps_id_mother(); }
	vector<int> &genps_status() { return ss.genps_status(); }
	vector<int> &genps_id_grandma() { return ss.genps_id_grandma(); }
	bool &lep1_passes_id() { return ss.lep1_passes_id(); }
	bool &lep2_passes_id() { return ss.lep2_passes_id(); }
	float &lep1_dxyPV() { return ss.lep1_dxyPV(); }
	float &lep2_dxyPV() { return ss.lep2_dxyPV(); }
	float &lep1_dZ() { return ss.lep1_dZ(); }
	float &lep2_dZ() { return ss.lep2_dZ(); }
	float &lep1_d0_err() { return ss.lep1_d0_err(); }
	float &lep2_d0_err() { return ss.lep2_d0_err(); }
	float &lep1_ip3d() { return ss.lep1_ip3d(); }
	float &lep2_ip3d() { return ss.lep2_ip3d(); }
	float &lep1_ip3d_err() { return ss.lep1_ip3d_err(); }
	float &lep2_ip3d_err() { return ss.lep2_ip3d_err(); }
	int &nGoodElectrons7() { return ss.nGoodElectrons7(); }
	int &nGoodElectrons10() { return ss.nGoodElectrons10(); }
	int &nGoodElectrons25() { return ss.nGoodElectrons25(); }
	int &nGoodMuons5() { return ss.nGoodMuons5(); }
	int &nGoodMuons10() { return ss.nGoodMuons10(); }
	int &nGoodMuons25() { return ss.nGoodMuons25(); }
	TString &filename() { return ss.filename(); }
	float &lep1_ptrel_v1() { return ss.lep1_ptrel_v1(); }
	float &lep2_ptrel_v1() { return ss.lep2_ptrel_v1(); }
}
