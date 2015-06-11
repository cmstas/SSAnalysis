#include "lepfilter.h"
lepfilter ss;
namespace samesign {
	const float &evt_pfmet() { return ss.evt_pfmet(); }
	const float &evt_pfmetPhi() { return ss.evt_pfmetPhi(); }
	const float &evt_trackmet() { return ss.evt_trackmet(); }
	const float &evt_trackmetPhi() { return ss.evt_trackmetPhi(); }
	const float &evt_pfsumet() { return ss.evt_pfsumet(); }
	const float &evt_pfmetSig() { return ss.evt_pfmetSig(); }
	const int &evt_event() { return ss.evt_event(); }
	const int &evt_lumiBlock() { return ss.evt_lumiBlock(); }
	const int &evt_run() { return ss.evt_run(); }
	const bool &filt_csc() { return ss.filt_csc(); }
	const bool &filt_hbhe() { return ss.filt_hbhe(); }
	const bool &filt_hcallaser() { return ss.filt_hcallaser(); }
	const bool &filt_ecaltp() { return ss.filt_ecaltp(); }
	const bool &filt_trkfail() { return ss.filt_trkfail(); }
	const bool &filt_eebadsc() { return ss.filt_eebadsc(); }
	const bool &evt_isRealData() { return ss.evt_isRealData(); }
	const float &scale1fb() { return ss.scale1fb(); }
	const float &evt_xsec_incl() { return ss.evt_xsec_incl(); }
	const float &evt_kfactor() { return ss.evt_kfactor(); }
	const float &gen_met() { return ss.gen_met(); }
	const float &gen_metPhi() { return ss.gen_metPhi(); }
	const float &njets() { return ss.njets(); }
	const float &ht() { return ss.ht(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets() { return ss.jets(); }
	const vector<float> &jets_disc() { return ss.jets_disc(); }
	const TString &sample() { return ss.sample(); }
	const int &nFOs_SS() { return ss.nFOs_SS(); }
	const int &nvtx() { return ss.nvtx(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &p4() { return ss.p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &tag_p4() { return ss.tag_p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4() { return ss.dilep_p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &mc_p4() { return ss.mc_p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &mc_motherp4() { return ss.mc_motherp4(); }
	const int &id() { return ss.id(); }
	const int &idx() { return ss.idx(); }
	const float &dxyPV() { return ss.dxyPV(); }
	const float &dZ() { return ss.dZ(); }
	const float &dxyPV_err() { return ss.dxyPV_err(); }
	const int &motherID() { return ss.motherID(); }
	const int &mc_id() { return ss.mc_id(); }
	const float &RelIso03EA() { return ss.RelIso03EA(); }
	const bool &passes_SS_tight_v3() { return ss.passes_SS_tight_v3(); }
	const bool &passes_SS_tight_noiso_v3() { return ss.passes_SS_tight_noiso_v3(); }
	const bool &passes_SS_fo_v3() { return ss.passes_SS_fo_v3(); }
	const bool &passes_SS_fo_noiso_v3() { return ss.passes_SS_fo_noiso_v3(); }
	const bool &passes_SS_fo_looseMVA_v3() { return ss.passes_SS_fo_looseMVA_v3(); }
	const bool &passes_SS_fo_looseMVA_noiso_v3() { return ss.passes_SS_fo_looseMVA_noiso_v3(); }
	const float &ip3d() { return ss.ip3d(); }
	const float &ip3derr() { return ss.ip3derr(); }
	const int &type() { return ss.type(); }
	const float &mt() { return ss.mt(); }
	const float &ptrelv0() { return ss.ptrelv0(); }
	const float &ptrelv1() { return ss.ptrelv1(); }
	const float &miniiso() { return ss.miniiso(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &jet_close_lep() { return ss.jet_close_lep(); }
	const float &ptratio() { return ss.ptratio(); }
	const int &tag_charge() { return ss.tag_charge(); }
	const float &dilep_mass() { return ss.dilep_mass(); }
	const float &sigmaIEtaIEta_full5x5() { return ss.sigmaIEtaIEta_full5x5(); }
	const float &etaSC() { return ss.etaSC(); }
	const float &dEtaIn() { return ss.dEtaIn(); }
	const float &dPhiIn() { return ss.dPhiIn(); }
	const float &hOverE() { return ss.hOverE(); }
	const float &mva() { return ss.mva(); }
	const float &ecalEnergy() { return ss.ecalEnergy(); }
	const float &eOverPIn() { return ss.eOverPIn(); }
	const bool &conv_vtx_flag() { return ss.conv_vtx_flag(); }
	const int &exp_innerlayers() { return ss.exp_innerlayers(); }
	const int &charge() { return ss.charge(); }
	const int &sccharge() { return ss.sccharge(); }
	const int &ckf_charge() { return ss.ckf_charge(); }
	const bool &threeChargeAgree() { return ss.threeChargeAgree(); }
	const int &pid_PFMuon() { return ss.pid_PFMuon(); }
	const float &gfit_chi2() { return ss.gfit_chi2(); }
	const float &gfit_ndof() { return ss.gfit_ndof(); }
	const int &gfit_validSTAHits() { return ss.gfit_validSTAHits(); }
	const int &numberOfMatchedStations() { return ss.numberOfMatchedStations(); }
	const int &validPixelHits() { return ss.validPixelHits(); }
	const int &nlayers() { return ss.nlayers(); }
}
