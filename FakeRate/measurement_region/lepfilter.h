// -*- C++ -*-
#ifndef lepfilter_H
#define lepfilter_H
#include "Math/LorentzVector.h"
#include "Math/Point3D.h"
#include "TMath.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBits.h"
#include <vector> 
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std; 
class lepfilter {
private: 
protected: 
	unsigned int index;
	float	met_;
	TBranch *met_branch;
	bool met_isLoaded;
	float	metPhi_;
	TBranch *metPhi_branch;
	bool metPhi_isLoaded;
	int	event_;
	TBranch *event_branch;
	bool event_isLoaded;
	int	lumi_;
	TBranch *lumi_branch;
	bool lumi_isLoaded;
	int	run_;
	TBranch *run_branch;
	bool run_isLoaded;
	bool	filt_csc_;
	TBranch *filt_csc_branch;
	bool filt_csc_isLoaded;
	bool	filt_hbhe_;
	TBranch *filt_hbhe_branch;
	bool filt_hbhe_isLoaded;
	bool	filt_hcallaser_;
	TBranch *filt_hcallaser_branch;
	bool filt_hcallaser_isLoaded;
	bool	filt_ecaltp_;
	TBranch *filt_ecaltp_branch;
	bool filt_ecaltp_isLoaded;
	bool	filt_trkfail_;
	TBranch *filt_trkfail_branch;
	bool filt_trkfail_isLoaded;
	bool	filt_eebadsc_;
	TBranch *filt_eebadsc_branch;
	bool filt_eebadsc_isLoaded;
	bool	is_real_data_;
	TBranch *is_real_data_branch;
	bool is_real_data_isLoaded;
	float	scale1fb_;
	TBranch *scale1fb_branch;
	bool scale1fb_isLoaded;
	float	xsec_;
	TBranch *xsec_branch;
	bool xsec_isLoaded;
	float	kfactor_;
	TBranch *kfactor_branch;
	bool kfactor_isLoaded;
	float	gen_met_;
	TBranch *gen_met_branch;
	bool gen_met_isLoaded;
	float	gen_met_phi_;
	TBranch *gen_met_phi_branch;
	bool gen_met_phi_isLoaded;
	float	njets_;
	TBranch *njets_branch;
	bool njets_isLoaded;
	float	ht_;
	TBranch *ht_branch;
	bool ht_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_;
	TBranch *jets_branch;
	bool jets_isLoaded;
	vector<float> *jets_disc_;
	TBranch *jets_disc_branch;
	bool jets_disc_isLoaded;
	TString *sample_;
	TBranch *sample_branch;
	bool sample_isLoaded;
	int	nFOs_;
	TBranch *nFOs_branch;
	bool nFOs_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *p4_;
	TBranch *p4_branch;
	bool p4_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *mc_p4_;
	TBranch *mc_p4_branch;
	bool mc_p4_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *mc_motherp4_;
	TBranch *mc_motherp4_branch;
	bool mc_motherp4_isLoaded;
	int	id_;
	TBranch *id_branch;
	bool id_isLoaded;
	int	idx_;
	TBranch *idx_branch;
	bool idx_isLoaded;
	float	d0_;
	TBranch *d0_branch;
	bool d0_isLoaded;
	float	dZ_;
	TBranch *dZ_branch;
	bool dZ_isLoaded;
	float	d0_err_;
	TBranch *d0_err_branch;
	bool d0_err_isLoaded;
	int	motherID_;
	TBranch *motherID_branch;
	bool motherID_isLoaded;
	int	mc_id_;
	TBranch *mc_id_branch;
	bool mc_id_isLoaded;
	float	iso_;
	TBranch *iso_branch;
	bool iso_isLoaded;
	bool	passes_id_;
	TBranch *passes_id_branch;
	bool passes_id_isLoaded;
	bool	passes_id_ptrel_;
	TBranch *passes_id_ptrel_branch;
	bool passes_id_ptrel_isLoaded;
	bool	FO_;
	TBranch *FO_branch;
	bool FO_isLoaded;
	bool	FO_ptrel_;
	TBranch *FO_ptrel_branch;
	bool FO_ptrel_isLoaded;
	bool	FO_NoIso_;
	TBranch *FO_NoIso_branch;
	bool FO_NoIso_isLoaded;
	float	ip3d_;
	TBranch *ip3d_branch;
	bool ip3d_isLoaded;
	float	ip3derr_;
	TBranch *ip3derr_branch;
	bool ip3derr_isLoaded;
	int	type_;
	TBranch *type_branch;
	bool type_isLoaded;
	float	mt_;
	TBranch *mt_branch;
	bool mt_isLoaded;
	float	ptrelv0_;
	TBranch *ptrelv0_branch;
	bool ptrelv0_isLoaded;
	float	ptrelv1_;
	TBranch *ptrelv1_branch;
	bool ptrelv1_isLoaded;
	float	el_sigmaIEtaIEta_full5x5_;
	TBranch *el_sigmaIEtaIEta_full5x5_branch;
	bool el_sigmaIEtaIEta_full5x5_isLoaded;
	float	el_etaSC_;
	TBranch *el_etaSC_branch;
	bool el_etaSC_isLoaded;
	float	el_dEtaIn_;
	TBranch *el_dEtaIn_branch;
	bool el_dEtaIn_isLoaded;
	float	el_dPhiIn_;
	TBranch *el_dPhiIn_branch;
	bool el_dPhiIn_isLoaded;
	float	el_hOverE_;
	TBranch *el_hOverE_branch;
	bool el_hOverE_isLoaded;
	float	el_ecalEnergy_;
	TBranch *el_ecalEnergy_branch;
	bool el_ecalEnergy_isLoaded;
	float	el_eOverPIn_;
	TBranch *el_eOverPIn_branch;
	bool el_eOverPIn_isLoaded;
	bool	el_conv_vtx_flag_;
	TBranch *el_conv_vtx_flag_branch;
	bool el_conv_vtx_flag_isLoaded;
	int	el_exp_innerlayers_;
	TBranch *el_exp_innerlayers_branch;
	bool el_exp_innerlayers_isLoaded;
	int	el_charge_;
	TBranch *el_charge_branch;
	bool el_charge_isLoaded;
	int	el_sccharge_;
	TBranch *el_sccharge_branch;
	bool el_sccharge_isLoaded;
	int	el_ckf_charge_;
	TBranch *el_ckf_charge_branch;
	bool el_ckf_charge_isLoaded;
	bool	el_threeChargeAgree_;
	TBranch *el_threeChargeAgree_branch;
	bool el_threeChargeAgree_isLoaded;
	int	mu_pid_PFMuon_;
	TBranch *mu_pid_PFMuon_branch;
	bool mu_pid_PFMuon_isLoaded;
	float	mu_gfit_chi2_;
	TBranch *mu_gfit_chi2_branch;
	bool mu_gfit_chi2_isLoaded;
	float	mu_gfit_ndof_;
	TBranch *mu_gfit_ndof_branch;
	bool mu_gfit_ndof_isLoaded;
	int	mu_gfit_validSTAHits_;
	TBranch *mu_gfit_validSTAHits_branch;
	bool mu_gfit_validSTAHits_isLoaded;
	int	mu_numberOfMatchedStations_;
	TBranch *mu_numberOfMatchedStations_branch;
	bool mu_numberOfMatchedStations_isLoaded;
	int	mu_validPixelHits_;
	TBranch *mu_validPixelHits_branch;
	bool mu_validPixelHits_isLoaded;
	int	mu_nlayers_;
	TBranch *mu_nlayers_branch;
	bool mu_nlayers_isLoaded;
public: 
void Init(TTree *tree) {
	jets_branch = 0;
	if (tree->GetBranch("jets") != 0) {
		jets_branch = tree->GetBranch("jets");
		if (jets_branch) {jets_branch->SetAddress(&jets_);}
	}
	p4_branch = 0;
	if (tree->GetBranch("p4") != 0) {
		p4_branch = tree->GetBranch("p4");
		if (p4_branch) {p4_branch->SetAddress(&p4_);}
	}
	mc_p4_branch = 0;
	if (tree->GetBranch("mc_p4") != 0) {
		mc_p4_branch = tree->GetBranch("mc_p4");
		if (mc_p4_branch) {mc_p4_branch->SetAddress(&mc_p4_);}
	}
	mc_motherp4_branch = 0;
	if (tree->GetBranch("mc_motherp4") != 0) {
		mc_motherp4_branch = tree->GetBranch("mc_motherp4");
		if (mc_motherp4_branch) {mc_motherp4_branch->SetAddress(&mc_motherp4_);}
	}
  tree->SetMakeClass(1);
	met_branch = 0;
	if (tree->GetBranch("met") != 0) {
		met_branch = tree->GetBranch("met");
		if (met_branch) {met_branch->SetAddress(&met_);}
	}
	metPhi_branch = 0;
	if (tree->GetBranch("metPhi") != 0) {
		metPhi_branch = tree->GetBranch("metPhi");
		if (metPhi_branch) {metPhi_branch->SetAddress(&metPhi_);}
	}
	event_branch = 0;
	if (tree->GetBranch("event") != 0) {
		event_branch = tree->GetBranch("event");
		if (event_branch) {event_branch->SetAddress(&event_);}
	}
	lumi_branch = 0;
	if (tree->GetBranch("lumi") != 0) {
		lumi_branch = tree->GetBranch("lumi");
		if (lumi_branch) {lumi_branch->SetAddress(&lumi_);}
	}
	run_branch = 0;
	if (tree->GetBranch("run") != 0) {
		run_branch = tree->GetBranch("run");
		if (run_branch) {run_branch->SetAddress(&run_);}
	}
	filt_csc_branch = 0;
	if (tree->GetBranch("filt_csc") != 0) {
		filt_csc_branch = tree->GetBranch("filt_csc");
		if (filt_csc_branch) {filt_csc_branch->SetAddress(&filt_csc_);}
	}
	filt_hbhe_branch = 0;
	if (tree->GetBranch("filt_hbhe") != 0) {
		filt_hbhe_branch = tree->GetBranch("filt_hbhe");
		if (filt_hbhe_branch) {filt_hbhe_branch->SetAddress(&filt_hbhe_);}
	}
	filt_hcallaser_branch = 0;
	if (tree->GetBranch("filt_hcallaser") != 0) {
		filt_hcallaser_branch = tree->GetBranch("filt_hcallaser");
		if (filt_hcallaser_branch) {filt_hcallaser_branch->SetAddress(&filt_hcallaser_);}
	}
	filt_ecaltp_branch = 0;
	if (tree->GetBranch("filt_ecaltp") != 0) {
		filt_ecaltp_branch = tree->GetBranch("filt_ecaltp");
		if (filt_ecaltp_branch) {filt_ecaltp_branch->SetAddress(&filt_ecaltp_);}
	}
	filt_trkfail_branch = 0;
	if (tree->GetBranch("filt_trkfail") != 0) {
		filt_trkfail_branch = tree->GetBranch("filt_trkfail");
		if (filt_trkfail_branch) {filt_trkfail_branch->SetAddress(&filt_trkfail_);}
	}
	filt_eebadsc_branch = 0;
	if (tree->GetBranch("filt_eebadsc") != 0) {
		filt_eebadsc_branch = tree->GetBranch("filt_eebadsc");
		if (filt_eebadsc_branch) {filt_eebadsc_branch->SetAddress(&filt_eebadsc_);}
	}
	is_real_data_branch = 0;
	if (tree->GetBranch("is_real_data") != 0) {
		is_real_data_branch = tree->GetBranch("is_real_data");
		if (is_real_data_branch) {is_real_data_branch->SetAddress(&is_real_data_);}
	}
	scale1fb_branch = 0;
	if (tree->GetBranch("scale1fb") != 0) {
		scale1fb_branch = tree->GetBranch("scale1fb");
		if (scale1fb_branch) {scale1fb_branch->SetAddress(&scale1fb_);}
	}
	xsec_branch = 0;
	if (tree->GetBranch("xsec") != 0) {
		xsec_branch = tree->GetBranch("xsec");
		if (xsec_branch) {xsec_branch->SetAddress(&xsec_);}
	}
	kfactor_branch = 0;
	if (tree->GetBranch("kfactor") != 0) {
		kfactor_branch = tree->GetBranch("kfactor");
		if (kfactor_branch) {kfactor_branch->SetAddress(&kfactor_);}
	}
	gen_met_branch = 0;
	if (tree->GetBranch("gen_met") != 0) {
		gen_met_branch = tree->GetBranch("gen_met");
		if (gen_met_branch) {gen_met_branch->SetAddress(&gen_met_);}
	}
	gen_met_phi_branch = 0;
	if (tree->GetBranch("gen_met_phi") != 0) {
		gen_met_phi_branch = tree->GetBranch("gen_met_phi");
		if (gen_met_phi_branch) {gen_met_phi_branch->SetAddress(&gen_met_phi_);}
	}
	njets_branch = 0;
	if (tree->GetBranch("njets") != 0) {
		njets_branch = tree->GetBranch("njets");
		if (njets_branch) {njets_branch->SetAddress(&njets_);}
	}
	ht_branch = 0;
	if (tree->GetBranch("ht") != 0) {
		ht_branch = tree->GetBranch("ht");
		if (ht_branch) {ht_branch->SetAddress(&ht_);}
	}
	jets_disc_branch = 0;
	if (tree->GetBranch("jets_disc") != 0) {
		jets_disc_branch = tree->GetBranch("jets_disc");
		if (jets_disc_branch) {jets_disc_branch->SetAddress(&jets_disc_);}
	}
	sample_branch = 0;
	if (tree->GetBranch("sample") != 0) {
		sample_branch = tree->GetBranch("sample");
		if (sample_branch) {sample_branch->SetAddress(&sample_);}
	}
	nFOs_branch = 0;
	if (tree->GetBranch("nFOs") != 0) {
		nFOs_branch = tree->GetBranch("nFOs");
		if (nFOs_branch) {nFOs_branch->SetAddress(&nFOs_);}
	}
	id_branch = 0;
	if (tree->GetBranch("id") != 0) {
		id_branch = tree->GetBranch("id");
		if (id_branch) {id_branch->SetAddress(&id_);}
	}
	idx_branch = 0;
	if (tree->GetBranch("idx") != 0) {
		idx_branch = tree->GetBranch("idx");
		if (idx_branch) {idx_branch->SetAddress(&idx_);}
	}
	d0_branch = 0;
	if (tree->GetBranch("d0") != 0) {
		d0_branch = tree->GetBranch("d0");
		if (d0_branch) {d0_branch->SetAddress(&d0_);}
	}
	dZ_branch = 0;
	if (tree->GetBranch("dZ") != 0) {
		dZ_branch = tree->GetBranch("dZ");
		if (dZ_branch) {dZ_branch->SetAddress(&dZ_);}
	}
	d0_err_branch = 0;
	if (tree->GetBranch("d0_err") != 0) {
		d0_err_branch = tree->GetBranch("d0_err");
		if (d0_err_branch) {d0_err_branch->SetAddress(&d0_err_);}
	}
	motherID_branch = 0;
	if (tree->GetBranch("motherID") != 0) {
		motherID_branch = tree->GetBranch("motherID");
		if (motherID_branch) {motherID_branch->SetAddress(&motherID_);}
	}
	mc_id_branch = 0;
	if (tree->GetBranch("mc_id") != 0) {
		mc_id_branch = tree->GetBranch("mc_id");
		if (mc_id_branch) {mc_id_branch->SetAddress(&mc_id_);}
	}
	iso_branch = 0;
	if (tree->GetBranch("iso") != 0) {
		iso_branch = tree->GetBranch("iso");
		if (iso_branch) {iso_branch->SetAddress(&iso_);}
	}
	passes_id_branch = 0;
	if (tree->GetBranch("passes_id") != 0) {
		passes_id_branch = tree->GetBranch("passes_id");
		if (passes_id_branch) {passes_id_branch->SetAddress(&passes_id_);}
	}
	passes_id_ptrel_branch = 0;
	if (tree->GetBranch("passes_id_ptrel") != 0) {
		passes_id_ptrel_branch = tree->GetBranch("passes_id_ptrel");
		if (passes_id_ptrel_branch) {passes_id_ptrel_branch->SetAddress(&passes_id_ptrel_);}
	}
	FO_branch = 0;
	if (tree->GetBranch("FO") != 0) {
		FO_branch = tree->GetBranch("FO");
		if (FO_branch) {FO_branch->SetAddress(&FO_);}
	}
	FO_ptrel_branch = 0;
	if (tree->GetBranch("FO_ptrel") != 0) {
		FO_ptrel_branch = tree->GetBranch("FO_ptrel");
		if (FO_ptrel_branch) {FO_ptrel_branch->SetAddress(&FO_ptrel_);}
	}
	FO_NoIso_branch = 0;
	if (tree->GetBranch("FO_NoIso") != 0) {
		FO_NoIso_branch = tree->GetBranch("FO_NoIso");
		if (FO_NoIso_branch) {FO_NoIso_branch->SetAddress(&FO_NoIso_);}
	}
	ip3d_branch = 0;
	if (tree->GetBranch("ip3d") != 0) {
		ip3d_branch = tree->GetBranch("ip3d");
		if (ip3d_branch) {ip3d_branch->SetAddress(&ip3d_);}
	}
	ip3derr_branch = 0;
	if (tree->GetBranch("ip3derr") != 0) {
		ip3derr_branch = tree->GetBranch("ip3derr");
		if (ip3derr_branch) {ip3derr_branch->SetAddress(&ip3derr_);}
	}
	type_branch = 0;
	if (tree->GetBranch("type") != 0) {
		type_branch = tree->GetBranch("type");
		if (type_branch) {type_branch->SetAddress(&type_);}
	}
	mt_branch = 0;
	if (tree->GetBranch("mt") != 0) {
		mt_branch = tree->GetBranch("mt");
		if (mt_branch) {mt_branch->SetAddress(&mt_);}
	}
	ptrelv0_branch = 0;
	if (tree->GetBranch("ptrelv0") != 0) {
		ptrelv0_branch = tree->GetBranch("ptrelv0");
		if (ptrelv0_branch) {ptrelv0_branch->SetAddress(&ptrelv0_);}
	}
	ptrelv1_branch = 0;
	if (tree->GetBranch("ptrelv1") != 0) {
		ptrelv1_branch = tree->GetBranch("ptrelv1");
		if (ptrelv1_branch) {ptrelv1_branch->SetAddress(&ptrelv1_);}
	}
	el_sigmaIEtaIEta_full5x5_branch = 0;
	if (tree->GetBranch("el_sigmaIEtaIEta_full5x5") != 0) {
		el_sigmaIEtaIEta_full5x5_branch = tree->GetBranch("el_sigmaIEtaIEta_full5x5");
		if (el_sigmaIEtaIEta_full5x5_branch) {el_sigmaIEtaIEta_full5x5_branch->SetAddress(&el_sigmaIEtaIEta_full5x5_);}
	}
	el_etaSC_branch = 0;
	if (tree->GetBranch("el_etaSC") != 0) {
		el_etaSC_branch = tree->GetBranch("el_etaSC");
		if (el_etaSC_branch) {el_etaSC_branch->SetAddress(&el_etaSC_);}
	}
	el_dEtaIn_branch = 0;
	if (tree->GetBranch("el_dEtaIn") != 0) {
		el_dEtaIn_branch = tree->GetBranch("el_dEtaIn");
		if (el_dEtaIn_branch) {el_dEtaIn_branch->SetAddress(&el_dEtaIn_);}
	}
	el_dPhiIn_branch = 0;
	if (tree->GetBranch("el_dPhiIn") != 0) {
		el_dPhiIn_branch = tree->GetBranch("el_dPhiIn");
		if (el_dPhiIn_branch) {el_dPhiIn_branch->SetAddress(&el_dPhiIn_);}
	}
	el_hOverE_branch = 0;
	if (tree->GetBranch("el_hOverE") != 0) {
		el_hOverE_branch = tree->GetBranch("el_hOverE");
		if (el_hOverE_branch) {el_hOverE_branch->SetAddress(&el_hOverE_);}
	}
	el_ecalEnergy_branch = 0;
	if (tree->GetBranch("el_ecalEnergy") != 0) {
		el_ecalEnergy_branch = tree->GetBranch("el_ecalEnergy");
		if (el_ecalEnergy_branch) {el_ecalEnergy_branch->SetAddress(&el_ecalEnergy_);}
	}
	el_eOverPIn_branch = 0;
	if (tree->GetBranch("el_eOverPIn") != 0) {
		el_eOverPIn_branch = tree->GetBranch("el_eOverPIn");
		if (el_eOverPIn_branch) {el_eOverPIn_branch->SetAddress(&el_eOverPIn_);}
	}
	el_conv_vtx_flag_branch = 0;
	if (tree->GetBranch("el_conv_vtx_flag") != 0) {
		el_conv_vtx_flag_branch = tree->GetBranch("el_conv_vtx_flag");
		if (el_conv_vtx_flag_branch) {el_conv_vtx_flag_branch->SetAddress(&el_conv_vtx_flag_);}
	}
	el_exp_innerlayers_branch = 0;
	if (tree->GetBranch("el_exp_innerlayers") != 0) {
		el_exp_innerlayers_branch = tree->GetBranch("el_exp_innerlayers");
		if (el_exp_innerlayers_branch) {el_exp_innerlayers_branch->SetAddress(&el_exp_innerlayers_);}
	}
	el_charge_branch = 0;
	if (tree->GetBranch("el_charge") != 0) {
		el_charge_branch = tree->GetBranch("el_charge");
		if (el_charge_branch) {el_charge_branch->SetAddress(&el_charge_);}
	}
	el_sccharge_branch = 0;
	if (tree->GetBranch("el_sccharge") != 0) {
		el_sccharge_branch = tree->GetBranch("el_sccharge");
		if (el_sccharge_branch) {el_sccharge_branch->SetAddress(&el_sccharge_);}
	}
	el_ckf_charge_branch = 0;
	if (tree->GetBranch("el_ckf_charge") != 0) {
		el_ckf_charge_branch = tree->GetBranch("el_ckf_charge");
		if (el_ckf_charge_branch) {el_ckf_charge_branch->SetAddress(&el_ckf_charge_);}
	}
	el_threeChargeAgree_branch = 0;
	if (tree->GetBranch("el_threeChargeAgree") != 0) {
		el_threeChargeAgree_branch = tree->GetBranch("el_threeChargeAgree");
		if (el_threeChargeAgree_branch) {el_threeChargeAgree_branch->SetAddress(&el_threeChargeAgree_);}
	}
	mu_pid_PFMuon_branch = 0;
	if (tree->GetBranch("mu_pid_PFMuon") != 0) {
		mu_pid_PFMuon_branch = tree->GetBranch("mu_pid_PFMuon");
		if (mu_pid_PFMuon_branch) {mu_pid_PFMuon_branch->SetAddress(&mu_pid_PFMuon_);}
	}
	mu_gfit_chi2_branch = 0;
	if (tree->GetBranch("mu_gfit_chi2") != 0) {
		mu_gfit_chi2_branch = tree->GetBranch("mu_gfit_chi2");
		if (mu_gfit_chi2_branch) {mu_gfit_chi2_branch->SetAddress(&mu_gfit_chi2_);}
	}
	mu_gfit_ndof_branch = 0;
	if (tree->GetBranch("mu_gfit_ndof") != 0) {
		mu_gfit_ndof_branch = tree->GetBranch("mu_gfit_ndof");
		if (mu_gfit_ndof_branch) {mu_gfit_ndof_branch->SetAddress(&mu_gfit_ndof_);}
	}
	mu_gfit_validSTAHits_branch = 0;
	if (tree->GetBranch("mu_gfit_validSTAHits") != 0) {
		mu_gfit_validSTAHits_branch = tree->GetBranch("mu_gfit_validSTAHits");
		if (mu_gfit_validSTAHits_branch) {mu_gfit_validSTAHits_branch->SetAddress(&mu_gfit_validSTAHits_);}
	}
	mu_numberOfMatchedStations_branch = 0;
	if (tree->GetBranch("mu_numberOfMatchedStations") != 0) {
		mu_numberOfMatchedStations_branch = tree->GetBranch("mu_numberOfMatchedStations");
		if (mu_numberOfMatchedStations_branch) {mu_numberOfMatchedStations_branch->SetAddress(&mu_numberOfMatchedStations_);}
	}
	mu_validPixelHits_branch = 0;
	if (tree->GetBranch("mu_validPixelHits") != 0) {
		mu_validPixelHits_branch = tree->GetBranch("mu_validPixelHits");
		if (mu_validPixelHits_branch) {mu_validPixelHits_branch->SetAddress(&mu_validPixelHits_);}
	}
	mu_nlayers_branch = 0;
	if (tree->GetBranch("mu_nlayers") != 0) {
		mu_nlayers_branch = tree->GetBranch("mu_nlayers");
		if (mu_nlayers_branch) {mu_nlayers_branch->SetAddress(&mu_nlayers_);}
	}
  tree->SetMakeClass(0);
}
void GetEntry(unsigned int idx) 
	// this only marks branches as not loaded, saving a lot of time
	{
		index = idx;
		met_isLoaded = false;
		metPhi_isLoaded = false;
		event_isLoaded = false;
		lumi_isLoaded = false;
		run_isLoaded = false;
		filt_csc_isLoaded = false;
		filt_hbhe_isLoaded = false;
		filt_hcallaser_isLoaded = false;
		filt_ecaltp_isLoaded = false;
		filt_trkfail_isLoaded = false;
		filt_eebadsc_isLoaded = false;
		is_real_data_isLoaded = false;
		scale1fb_isLoaded = false;
		xsec_isLoaded = false;
		kfactor_isLoaded = false;
		gen_met_isLoaded = false;
		gen_met_phi_isLoaded = false;
		njets_isLoaded = false;
		ht_isLoaded = false;
		jets_isLoaded = false;
		jets_disc_isLoaded = false;
		sample_isLoaded = false;
		nFOs_isLoaded = false;
		p4_isLoaded = false;
		mc_p4_isLoaded = false;
		mc_motherp4_isLoaded = false;
		id_isLoaded = false;
		idx_isLoaded = false;
		d0_isLoaded = false;
		dZ_isLoaded = false;
		d0_err_isLoaded = false;
		motherID_isLoaded = false;
		mc_id_isLoaded = false;
		iso_isLoaded = false;
		passes_id_isLoaded = false;
		passes_id_ptrel_isLoaded = false;
		FO_isLoaded = false;
		FO_ptrel_isLoaded = false;
		FO_NoIso_isLoaded = false;
		ip3d_isLoaded = false;
		ip3derr_isLoaded = false;
		type_isLoaded = false;
		mt_isLoaded = false;
		ptrelv0_isLoaded = false;
		ptrelv1_isLoaded = false;
		el_sigmaIEtaIEta_full5x5_isLoaded = false;
		el_etaSC_isLoaded = false;
		el_dEtaIn_isLoaded = false;
		el_dPhiIn_isLoaded = false;
		el_hOverE_isLoaded = false;
		el_ecalEnergy_isLoaded = false;
		el_eOverPIn_isLoaded = false;
		el_conv_vtx_flag_isLoaded = false;
		el_exp_innerlayers_isLoaded = false;
		el_charge_isLoaded = false;
		el_sccharge_isLoaded = false;
		el_ckf_charge_isLoaded = false;
		el_threeChargeAgree_isLoaded = false;
		mu_pid_PFMuon_isLoaded = false;
		mu_gfit_chi2_isLoaded = false;
		mu_gfit_ndof_isLoaded = false;
		mu_gfit_validSTAHits_isLoaded = false;
		mu_numberOfMatchedStations_isLoaded = false;
		mu_validPixelHits_isLoaded = false;
		mu_nlayers_isLoaded = false;
	}

void LoadAllBranches() 
	// load all branches
{
	if (met_branch != 0) met();
	if (metPhi_branch != 0) metPhi();
	if (event_branch != 0) event();
	if (lumi_branch != 0) lumi();
	if (run_branch != 0) run();
	if (filt_csc_branch != 0) filt_csc();
	if (filt_hbhe_branch != 0) filt_hbhe();
	if (filt_hcallaser_branch != 0) filt_hcallaser();
	if (filt_ecaltp_branch != 0) filt_ecaltp();
	if (filt_trkfail_branch != 0) filt_trkfail();
	if (filt_eebadsc_branch != 0) filt_eebadsc();
	if (is_real_data_branch != 0) is_real_data();
	if (scale1fb_branch != 0) scale1fb();
	if (xsec_branch != 0) xsec();
	if (kfactor_branch != 0) kfactor();
	if (gen_met_branch != 0) gen_met();
	if (gen_met_phi_branch != 0) gen_met_phi();
	if (njets_branch != 0) njets();
	if (ht_branch != 0) ht();
	if (jets_branch != 0) jets();
	if (jets_disc_branch != 0) jets_disc();
	if (sample_branch != 0) sample();
	if (nFOs_branch != 0) nFOs();
	if (p4_branch != 0) p4();
	if (mc_p4_branch != 0) mc_p4();
	if (mc_motherp4_branch != 0) mc_motherp4();
	if (id_branch != 0) id();
	if (idx_branch != 0) idx();
	if (d0_branch != 0) d0();
	if (dZ_branch != 0) dZ();
	if (d0_err_branch != 0) d0_err();
	if (motherID_branch != 0) motherID();
	if (mc_id_branch != 0) mc_id();
	if (iso_branch != 0) iso();
	if (passes_id_branch != 0) passes_id();
	if (passes_id_ptrel_branch != 0) passes_id_ptrel();
	if (FO_branch != 0) FO();
	if (FO_ptrel_branch != 0) FO_ptrel();
	if (FO_NoIso_branch != 0) FO_NoIso();
	if (ip3d_branch != 0) ip3d();
	if (ip3derr_branch != 0) ip3derr();
	if (type_branch != 0) type();
	if (mt_branch != 0) mt();
	if (ptrelv0_branch != 0) ptrelv0();
	if (ptrelv1_branch != 0) ptrelv1();
	if (el_sigmaIEtaIEta_full5x5_branch != 0) el_sigmaIEtaIEta_full5x5();
	if (el_etaSC_branch != 0) el_etaSC();
	if (el_dEtaIn_branch != 0) el_dEtaIn();
	if (el_dPhiIn_branch != 0) el_dPhiIn();
	if (el_hOverE_branch != 0) el_hOverE();
	if (el_ecalEnergy_branch != 0) el_ecalEnergy();
	if (el_eOverPIn_branch != 0) el_eOverPIn();
	if (el_conv_vtx_flag_branch != 0) el_conv_vtx_flag();
	if (el_exp_innerlayers_branch != 0) el_exp_innerlayers();
	if (el_charge_branch != 0) el_charge();
	if (el_sccharge_branch != 0) el_sccharge();
	if (el_ckf_charge_branch != 0) el_ckf_charge();
	if (el_threeChargeAgree_branch != 0) el_threeChargeAgree();
	if (mu_pid_PFMuon_branch != 0) mu_pid_PFMuon();
	if (mu_gfit_chi2_branch != 0) mu_gfit_chi2();
	if (mu_gfit_ndof_branch != 0) mu_gfit_ndof();
	if (mu_gfit_validSTAHits_branch != 0) mu_gfit_validSTAHits();
	if (mu_numberOfMatchedStations_branch != 0) mu_numberOfMatchedStations();
	if (mu_validPixelHits_branch != 0) mu_validPixelHits();
	if (mu_nlayers_branch != 0) mu_nlayers();
}

	float &met()
	{
		if (not met_isLoaded) {
			if (met_branch != 0) {
				met_branch->GetEntry(index);
			} else { 
				printf("branch met_branch does not exist!\n");
				exit(1);
			}
			met_isLoaded = true;
		}
		return met_;
	}
	float &metPhi()
	{
		if (not metPhi_isLoaded) {
			if (metPhi_branch != 0) {
				metPhi_branch->GetEntry(index);
			} else { 
				printf("branch metPhi_branch does not exist!\n");
				exit(1);
			}
			metPhi_isLoaded = true;
		}
		return metPhi_;
	}
	int &event()
	{
		if (not event_isLoaded) {
			if (event_branch != 0) {
				event_branch->GetEntry(index);
			} else { 
				printf("branch event_branch does not exist!\n");
				exit(1);
			}
			event_isLoaded = true;
		}
		return event_;
	}
	int &lumi()
	{
		if (not lumi_isLoaded) {
			if (lumi_branch != 0) {
				lumi_branch->GetEntry(index);
			} else { 
				printf("branch lumi_branch does not exist!\n");
				exit(1);
			}
			lumi_isLoaded = true;
		}
		return lumi_;
	}
	int &run()
	{
		if (not run_isLoaded) {
			if (run_branch != 0) {
				run_branch->GetEntry(index);
			} else { 
				printf("branch run_branch does not exist!\n");
				exit(1);
			}
			run_isLoaded = true;
		}
		return run_;
	}
	bool &	filt_csc()
	{
		if (not filt_csc_isLoaded) {
			if (filt_csc_branch != 0) {
				filt_csc_branch->GetEntry(index);
			} else { 
				printf("branch filt_csc_branch does not exist!\n");
				exit(1);
			}
			filt_csc_isLoaded = true;
		}
		return filt_csc_;
	}
	bool &	filt_hbhe()
	{
		if (not filt_hbhe_isLoaded) {
			if (filt_hbhe_branch != 0) {
				filt_hbhe_branch->GetEntry(index);
			} else { 
				printf("branch filt_hbhe_branch does not exist!\n");
				exit(1);
			}
			filt_hbhe_isLoaded = true;
		}
		return filt_hbhe_;
	}
	bool &	filt_hcallaser()
	{
		if (not filt_hcallaser_isLoaded) {
			if (filt_hcallaser_branch != 0) {
				filt_hcallaser_branch->GetEntry(index);
			} else { 
				printf("branch filt_hcallaser_branch does not exist!\n");
				exit(1);
			}
			filt_hcallaser_isLoaded = true;
		}
		return filt_hcallaser_;
	}
	bool &	filt_ecaltp()
	{
		if (not filt_ecaltp_isLoaded) {
			if (filt_ecaltp_branch != 0) {
				filt_ecaltp_branch->GetEntry(index);
			} else { 
				printf("branch filt_ecaltp_branch does not exist!\n");
				exit(1);
			}
			filt_ecaltp_isLoaded = true;
		}
		return filt_ecaltp_;
	}
	bool &	filt_trkfail()
	{
		if (not filt_trkfail_isLoaded) {
			if (filt_trkfail_branch != 0) {
				filt_trkfail_branch->GetEntry(index);
			} else { 
				printf("branch filt_trkfail_branch does not exist!\n");
				exit(1);
			}
			filt_trkfail_isLoaded = true;
		}
		return filt_trkfail_;
	}
	bool &	filt_eebadsc()
	{
		if (not filt_eebadsc_isLoaded) {
			if (filt_eebadsc_branch != 0) {
				filt_eebadsc_branch->GetEntry(index);
			} else { 
				printf("branch filt_eebadsc_branch does not exist!\n");
				exit(1);
			}
			filt_eebadsc_isLoaded = true;
		}
		return filt_eebadsc_;
	}
	bool &	is_real_data()
	{
		if (not is_real_data_isLoaded) {
			if (is_real_data_branch != 0) {
				is_real_data_branch->GetEntry(index);
			} else { 
				printf("branch is_real_data_branch does not exist!\n");
				exit(1);
			}
			is_real_data_isLoaded = true;
		}
		return is_real_data_;
	}
	float &scale1fb()
	{
		if (not scale1fb_isLoaded) {
			if (scale1fb_branch != 0) {
				scale1fb_branch->GetEntry(index);
			} else { 
				printf("branch scale1fb_branch does not exist!\n");
				exit(1);
			}
			scale1fb_isLoaded = true;
		}
		return scale1fb_;
	}
	float &xsec()
	{
		if (not xsec_isLoaded) {
			if (xsec_branch != 0) {
				xsec_branch->GetEntry(index);
			} else { 
				printf("branch xsec_branch does not exist!\n");
				exit(1);
			}
			xsec_isLoaded = true;
		}
		return xsec_;
	}
	float &kfactor()
	{
		if (not kfactor_isLoaded) {
			if (kfactor_branch != 0) {
				kfactor_branch->GetEntry(index);
			} else { 
				printf("branch kfactor_branch does not exist!\n");
				exit(1);
			}
			kfactor_isLoaded = true;
		}
		return kfactor_;
	}
	float &gen_met()
	{
		if (not gen_met_isLoaded) {
			if (gen_met_branch != 0) {
				gen_met_branch->GetEntry(index);
			} else { 
				printf("branch gen_met_branch does not exist!\n");
				exit(1);
			}
			gen_met_isLoaded = true;
		}
		return gen_met_;
	}
	float &gen_met_phi()
	{
		if (not gen_met_phi_isLoaded) {
			if (gen_met_phi_branch != 0) {
				gen_met_phi_branch->GetEntry(index);
			} else { 
				printf("branch gen_met_phi_branch does not exist!\n");
				exit(1);
			}
			gen_met_phi_isLoaded = true;
		}
		return gen_met_phi_;
	}
	float &njets()
	{
		if (not njets_isLoaded) {
			if (njets_branch != 0) {
				njets_branch->GetEntry(index);
			} else { 
				printf("branch njets_branch does not exist!\n");
				exit(1);
			}
			njets_isLoaded = true;
		}
		return njets_;
	}
	float &ht()
	{
		if (not ht_isLoaded) {
			if (ht_branch != 0) {
				ht_branch->GetEntry(index);
			} else { 
				printf("branch ht_branch does not exist!\n");
				exit(1);
			}
			ht_isLoaded = true;
		}
		return ht_;
	}
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets()
	{
		if (not jets_isLoaded) {
			if (jets_branch != 0) {
				jets_branch->GetEntry(index);
			} else { 
				printf("branch jets_branch does not exist!\n");
				exit(1);
			}
			jets_isLoaded = true;
		}
		return *jets_;
	}
	vector<float> &jets_disc()
	{
		if (not jets_disc_isLoaded) {
			if (jets_disc_branch != 0) {
				jets_disc_branch->GetEntry(index);
			} else { 
				printf("branch jets_disc_branch does not exist!\n");
				exit(1);
			}
			jets_disc_isLoaded = true;
		}
		return *jets_disc_;
	}
	TString &sample()
	{
		if (not sample_isLoaded) {
			if (sample_branch != 0) {
				sample_branch->GetEntry(index);
			} else { 
				printf("branch sample_branch does not exist!\n");
				exit(1);
			}
			sample_isLoaded = true;
		}
		return *sample_;
	}
	int &nFOs()
	{
		if (not nFOs_isLoaded) {
			if (nFOs_branch != 0) {
				nFOs_branch->GetEntry(index);
			} else { 
				printf("branch nFOs_branch does not exist!\n");
				exit(1);
			}
			nFOs_isLoaded = true;
		}
		return nFOs_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &p4()
	{
		if (not p4_isLoaded) {
			if (p4_branch != 0) {
				p4_branch->GetEntry(index);
			} else { 
				printf("branch p4_branch does not exist!\n");
				exit(1);
			}
			p4_isLoaded = true;
		}
		return *p4_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &mc_p4()
	{
		if (not mc_p4_isLoaded) {
			if (mc_p4_branch != 0) {
				mc_p4_branch->GetEntry(index);
			} else { 
				printf("branch mc_p4_branch does not exist!\n");
				exit(1);
			}
			mc_p4_isLoaded = true;
		}
		return *mc_p4_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &mc_motherp4()
	{
		if (not mc_motherp4_isLoaded) {
			if (mc_motherp4_branch != 0) {
				mc_motherp4_branch->GetEntry(index);
			} else { 
				printf("branch mc_motherp4_branch does not exist!\n");
				exit(1);
			}
			mc_motherp4_isLoaded = true;
		}
		return *mc_motherp4_;
	}
	int &id()
	{
		if (not id_isLoaded) {
			if (id_branch != 0) {
				id_branch->GetEntry(index);
			} else { 
				printf("branch id_branch does not exist!\n");
				exit(1);
			}
			id_isLoaded = true;
		}
		return id_;
	}
	int &idx()
	{
		if (not idx_isLoaded) {
			if (idx_branch != 0) {
				idx_branch->GetEntry(index);
			} else { 
				printf("branch idx_branch does not exist!\n");
				exit(1);
			}
			idx_isLoaded = true;
		}
		return idx_;
	}
	float &d0()
	{
		if (not d0_isLoaded) {
			if (d0_branch != 0) {
				d0_branch->GetEntry(index);
			} else { 
				printf("branch d0_branch does not exist!\n");
				exit(1);
			}
			d0_isLoaded = true;
		}
		return d0_;
	}
	float &dZ()
	{
		if (not dZ_isLoaded) {
			if (dZ_branch != 0) {
				dZ_branch->GetEntry(index);
			} else { 
				printf("branch dZ_branch does not exist!\n");
				exit(1);
			}
			dZ_isLoaded = true;
		}
		return dZ_;
	}
	float &d0_err()
	{
		if (not d0_err_isLoaded) {
			if (d0_err_branch != 0) {
				d0_err_branch->GetEntry(index);
			} else { 
				printf("branch d0_err_branch does not exist!\n");
				exit(1);
			}
			d0_err_isLoaded = true;
		}
		return d0_err_;
	}
	int &motherID()
	{
		if (not motherID_isLoaded) {
			if (motherID_branch != 0) {
				motherID_branch->GetEntry(index);
			} else { 
				printf("branch motherID_branch does not exist!\n");
				exit(1);
			}
			motherID_isLoaded = true;
		}
		return motherID_;
	}
	int &mc_id()
	{
		if (not mc_id_isLoaded) {
			if (mc_id_branch != 0) {
				mc_id_branch->GetEntry(index);
			} else { 
				printf("branch mc_id_branch does not exist!\n");
				exit(1);
			}
			mc_id_isLoaded = true;
		}
		return mc_id_;
	}
	float &iso()
	{
		if (not iso_isLoaded) {
			if (iso_branch != 0) {
				iso_branch->GetEntry(index);
			} else { 
				printf("branch iso_branch does not exist!\n");
				exit(1);
			}
			iso_isLoaded = true;
		}
		return iso_;
	}
	bool &	passes_id()
	{
		if (not passes_id_isLoaded) {
			if (passes_id_branch != 0) {
				passes_id_branch->GetEntry(index);
			} else { 
				printf("branch passes_id_branch does not exist!\n");
				exit(1);
			}
			passes_id_isLoaded = true;
		}
		return passes_id_;
	}
	bool &	passes_id_ptrel()
	{
		if (not passes_id_ptrel_isLoaded) {
			if (passes_id_ptrel_branch != 0) {
				passes_id_ptrel_branch->GetEntry(index);
			} else { 
				printf("branch passes_id_ptrel_branch does not exist!\n");
				exit(1);
			}
			passes_id_ptrel_isLoaded = true;
		}
		return passes_id_ptrel_;
	}
	bool &	FO()
	{
		if (not FO_isLoaded) {
			if (FO_branch != 0) {
				FO_branch->GetEntry(index);
			} else { 
				printf("branch FO_branch does not exist!\n");
				exit(1);
			}
			FO_isLoaded = true;
		}
		return FO_;
	}
	bool &	FO_ptrel()
	{
		if (not FO_ptrel_isLoaded) {
			if (FO_ptrel_branch != 0) {
				FO_ptrel_branch->GetEntry(index);
			} else { 
				printf("branch FO_ptrel_branch does not exist!\n");
				exit(1);
			}
			FO_ptrel_isLoaded = true;
		}
		return FO_ptrel_;
	}
	bool &	FO_NoIso()
	{
		if (not FO_NoIso_isLoaded) {
			if (FO_NoIso_branch != 0) {
				FO_NoIso_branch->GetEntry(index);
			} else { 
				printf("branch FO_NoIso_branch does not exist!\n");
				exit(1);
			}
			FO_NoIso_isLoaded = true;
		}
		return FO_NoIso_;
	}
	float &ip3d()
	{
		if (not ip3d_isLoaded) {
			if (ip3d_branch != 0) {
				ip3d_branch->GetEntry(index);
			} else { 
				printf("branch ip3d_branch does not exist!\n");
				exit(1);
			}
			ip3d_isLoaded = true;
		}
		return ip3d_;
	}
	float &ip3derr()
	{
		if (not ip3derr_isLoaded) {
			if (ip3derr_branch != 0) {
				ip3derr_branch->GetEntry(index);
			} else { 
				printf("branch ip3derr_branch does not exist!\n");
				exit(1);
			}
			ip3derr_isLoaded = true;
		}
		return ip3derr_;
	}
	int &type()
	{
		if (not type_isLoaded) {
			if (type_branch != 0) {
				type_branch->GetEntry(index);
			} else { 
				printf("branch type_branch does not exist!\n");
				exit(1);
			}
			type_isLoaded = true;
		}
		return type_;
	}
	float &mt()
	{
		if (not mt_isLoaded) {
			if (mt_branch != 0) {
				mt_branch->GetEntry(index);
			} else { 
				printf("branch mt_branch does not exist!\n");
				exit(1);
			}
			mt_isLoaded = true;
		}
		return mt_;
	}
	float &ptrelv0()
	{
		if (not ptrelv0_isLoaded) {
			if (ptrelv0_branch != 0) {
				ptrelv0_branch->GetEntry(index);
			} else { 
				printf("branch ptrelv0_branch does not exist!\n");
				exit(1);
			}
			ptrelv0_isLoaded = true;
		}
		return ptrelv0_;
	}
	float &ptrelv1()
	{
		if (not ptrelv1_isLoaded) {
			if (ptrelv1_branch != 0) {
				ptrelv1_branch->GetEntry(index);
			} else { 
				printf("branch ptrelv1_branch does not exist!\n");
				exit(1);
			}
			ptrelv1_isLoaded = true;
		}
		return ptrelv1_;
	}
	float &el_sigmaIEtaIEta_full5x5()
	{
		if (not el_sigmaIEtaIEta_full5x5_isLoaded) {
			if (el_sigmaIEtaIEta_full5x5_branch != 0) {
				el_sigmaIEtaIEta_full5x5_branch->GetEntry(index);
			} else { 
				printf("branch el_sigmaIEtaIEta_full5x5_branch does not exist!\n");
				exit(1);
			}
			el_sigmaIEtaIEta_full5x5_isLoaded = true;
		}
		return el_sigmaIEtaIEta_full5x5_;
	}
	float &el_etaSC()
	{
		if (not el_etaSC_isLoaded) {
			if (el_etaSC_branch != 0) {
				el_etaSC_branch->GetEntry(index);
			} else { 
				printf("branch el_etaSC_branch does not exist!\n");
				exit(1);
			}
			el_etaSC_isLoaded = true;
		}
		return el_etaSC_;
	}
	float &el_dEtaIn()
	{
		if (not el_dEtaIn_isLoaded) {
			if (el_dEtaIn_branch != 0) {
				el_dEtaIn_branch->GetEntry(index);
			} else { 
				printf("branch el_dEtaIn_branch does not exist!\n");
				exit(1);
			}
			el_dEtaIn_isLoaded = true;
		}
		return el_dEtaIn_;
	}
	float &el_dPhiIn()
	{
		if (not el_dPhiIn_isLoaded) {
			if (el_dPhiIn_branch != 0) {
				el_dPhiIn_branch->GetEntry(index);
			} else { 
				printf("branch el_dPhiIn_branch does not exist!\n");
				exit(1);
			}
			el_dPhiIn_isLoaded = true;
		}
		return el_dPhiIn_;
	}
	float &el_hOverE()
	{
		if (not el_hOverE_isLoaded) {
			if (el_hOverE_branch != 0) {
				el_hOverE_branch->GetEntry(index);
			} else { 
				printf("branch el_hOverE_branch does not exist!\n");
				exit(1);
			}
			el_hOverE_isLoaded = true;
		}
		return el_hOverE_;
	}
	float &el_ecalEnergy()
	{
		if (not el_ecalEnergy_isLoaded) {
			if (el_ecalEnergy_branch != 0) {
				el_ecalEnergy_branch->GetEntry(index);
			} else { 
				printf("branch el_ecalEnergy_branch does not exist!\n");
				exit(1);
			}
			el_ecalEnergy_isLoaded = true;
		}
		return el_ecalEnergy_;
	}
	float &el_eOverPIn()
	{
		if (not el_eOverPIn_isLoaded) {
			if (el_eOverPIn_branch != 0) {
				el_eOverPIn_branch->GetEntry(index);
			} else { 
				printf("branch el_eOverPIn_branch does not exist!\n");
				exit(1);
			}
			el_eOverPIn_isLoaded = true;
		}
		return el_eOverPIn_;
	}
	bool &	el_conv_vtx_flag()
	{
		if (not el_conv_vtx_flag_isLoaded) {
			if (el_conv_vtx_flag_branch != 0) {
				el_conv_vtx_flag_branch->GetEntry(index);
			} else { 
				printf("branch el_conv_vtx_flag_branch does not exist!\n");
				exit(1);
			}
			el_conv_vtx_flag_isLoaded = true;
		}
		return el_conv_vtx_flag_;
	}
	int &el_exp_innerlayers()
	{
		if (not el_exp_innerlayers_isLoaded) {
			if (el_exp_innerlayers_branch != 0) {
				el_exp_innerlayers_branch->GetEntry(index);
			} else { 
				printf("branch el_exp_innerlayers_branch does not exist!\n");
				exit(1);
			}
			el_exp_innerlayers_isLoaded = true;
		}
		return el_exp_innerlayers_;
	}
	int &el_charge()
	{
		if (not el_charge_isLoaded) {
			if (el_charge_branch != 0) {
				el_charge_branch->GetEntry(index);
			} else { 
				printf("branch el_charge_branch does not exist!\n");
				exit(1);
			}
			el_charge_isLoaded = true;
		}
		return el_charge_;
	}
	int &el_sccharge()
	{
		if (not el_sccharge_isLoaded) {
			if (el_sccharge_branch != 0) {
				el_sccharge_branch->GetEntry(index);
			} else { 
				printf("branch el_sccharge_branch does not exist!\n");
				exit(1);
			}
			el_sccharge_isLoaded = true;
		}
		return el_sccharge_;
	}
	int &el_ckf_charge()
	{
		if (not el_ckf_charge_isLoaded) {
			if (el_ckf_charge_branch != 0) {
				el_ckf_charge_branch->GetEntry(index);
			} else { 
				printf("branch el_ckf_charge_branch does not exist!\n");
				exit(1);
			}
			el_ckf_charge_isLoaded = true;
		}
		return el_ckf_charge_;
	}
	bool &	el_threeChargeAgree()
	{
		if (not el_threeChargeAgree_isLoaded) {
			if (el_threeChargeAgree_branch != 0) {
				el_threeChargeAgree_branch->GetEntry(index);
			} else { 
				printf("branch el_threeChargeAgree_branch does not exist!\n");
				exit(1);
			}
			el_threeChargeAgree_isLoaded = true;
		}
		return el_threeChargeAgree_;
	}
	int &mu_pid_PFMuon()
	{
		if (not mu_pid_PFMuon_isLoaded) {
			if (mu_pid_PFMuon_branch != 0) {
				mu_pid_PFMuon_branch->GetEntry(index);
			} else { 
				printf("branch mu_pid_PFMuon_branch does not exist!\n");
				exit(1);
			}
			mu_pid_PFMuon_isLoaded = true;
		}
		return mu_pid_PFMuon_;
	}
	float &mu_gfit_chi2()
	{
		if (not mu_gfit_chi2_isLoaded) {
			if (mu_gfit_chi2_branch != 0) {
				mu_gfit_chi2_branch->GetEntry(index);
			} else { 
				printf("branch mu_gfit_chi2_branch does not exist!\n");
				exit(1);
			}
			mu_gfit_chi2_isLoaded = true;
		}
		return mu_gfit_chi2_;
	}
	float &mu_gfit_ndof()
	{
		if (not mu_gfit_ndof_isLoaded) {
			if (mu_gfit_ndof_branch != 0) {
				mu_gfit_ndof_branch->GetEntry(index);
			} else { 
				printf("branch mu_gfit_ndof_branch does not exist!\n");
				exit(1);
			}
			mu_gfit_ndof_isLoaded = true;
		}
		return mu_gfit_ndof_;
	}
	int &mu_gfit_validSTAHits()
	{
		if (not mu_gfit_validSTAHits_isLoaded) {
			if (mu_gfit_validSTAHits_branch != 0) {
				mu_gfit_validSTAHits_branch->GetEntry(index);
			} else { 
				printf("branch mu_gfit_validSTAHits_branch does not exist!\n");
				exit(1);
			}
			mu_gfit_validSTAHits_isLoaded = true;
		}
		return mu_gfit_validSTAHits_;
	}
	int &mu_numberOfMatchedStations()
	{
		if (not mu_numberOfMatchedStations_isLoaded) {
			if (mu_numberOfMatchedStations_branch != 0) {
				mu_numberOfMatchedStations_branch->GetEntry(index);
			} else { 
				printf("branch mu_numberOfMatchedStations_branch does not exist!\n");
				exit(1);
			}
			mu_numberOfMatchedStations_isLoaded = true;
		}
		return mu_numberOfMatchedStations_;
	}
	int &mu_validPixelHits()
	{
		if (not mu_validPixelHits_isLoaded) {
			if (mu_validPixelHits_branch != 0) {
				mu_validPixelHits_branch->GetEntry(index);
			} else { 
				printf("branch mu_validPixelHits_branch does not exist!\n");
				exit(1);
			}
			mu_validPixelHits_isLoaded = true;
		}
		return mu_validPixelHits_;
	}
	int &mu_nlayers()
	{
		if (not mu_nlayers_isLoaded) {
			if (mu_nlayers_branch != 0) {
				mu_nlayers_branch->GetEntry(index);
			} else { 
				printf("branch mu_nlayers_branch does not exist!\n");
				exit(1);
			}
			mu_nlayers_isLoaded = true;
		}
		return mu_nlayers_;
	}

  static void progress( int nEventsTotal, int nEventsChain ){
    int period = 1000;
    if(nEventsTotal%1000 == 0) {
      // xterm magic from L. Vacavant and A. Cerri
      if (isatty(1)) {
        if( ( nEventsChain - nEventsTotal ) > period ){
          float frac = (float)nEventsTotal/(nEventsChain*0.01);
          printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", frac);
          fflush(stdout);
        }
        else {
          printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
                 "\033[0m\033[32m <---\033[0m\015", 100.);
          cout << endl;
        }
      }
    }
  }
  
};

#ifndef __CINT__
extern lepfilter ss;
#endif

namespace samesign {
	float &met();
	float &metPhi();
	int &event();
	int &lumi();
	int &run();
	bool &filt_csc();
	bool &filt_hbhe();
	bool &filt_hcallaser();
	bool &filt_ecaltp();
	bool &filt_trkfail();
	bool &filt_eebadsc();
	bool &is_real_data();
	float &scale1fb();
	float &xsec();
	float &kfactor();
	float &gen_met();
	float &gen_met_phi();
	float &njets();
	float &ht();
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets();
	vector<float> &jets_disc();
	TString &sample();
	int &nFOs();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &p4();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &mc_p4();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &mc_motherp4();
	int &id();
	int &idx();
	float &d0();
	float &dZ();
	float &d0_err();
	int &motherID();
	int &mc_id();
	float &iso();
	bool &passes_id();
	bool &passes_id_ptrel();
	bool &FO();
	bool &FO_ptrel();
	bool &FO_NoIso();
	float &ip3d();
	float &ip3derr();
	int &type();
	float &mt();
	float &ptrelv0();
	float &ptrelv1();
	float &el_sigmaIEtaIEta_full5x5();
	float &el_etaSC();
	float &el_dEtaIn();
	float &el_dPhiIn();
	float &el_hOverE();
	float &el_ecalEnergy();
	float &el_eOverPIn();
	bool &el_conv_vtx_flag();
	int &el_exp_innerlayers();
	int &el_charge();
	int &el_sccharge();
	int &el_ckf_charge();
	bool &el_threeChargeAgree();
	int &mu_pid_PFMuon();
	float &mu_gfit_chi2();
	float &mu_gfit_ndof();
	int &mu_gfit_validSTAHits();
	int &mu_numberOfMatchedStations();
	int &mu_validPixelHits();
	int &mu_nlayers();
}
#endif
