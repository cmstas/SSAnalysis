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
	int	njets_;
	TBranch *njets_branch;
	bool njets_isLoaded;
	int	best_hyp_;
	TBranch *best_hyp_branch;
	bool best_hyp_isLoaded;
	int	hyp_class_;
	TBranch *hyp_class_branch;
	bool hyp_class_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep1_p4_;
	TBranch *lep1_p4_branch;
	bool lep1_p4_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep2_p4_;
	TBranch *lep2_p4_branch;
	bool lep2_p4_isLoaded;
	float	ht_;
	TBranch *ht_branch;
	bool ht_isLoaded;
	int	lep1_motherID_;
	TBranch *lep1_motherID_branch;
	bool lep1_motherID_isLoaded;
	int	lep2_motherID_;
	TBranch *lep2_motherID_branch;
	bool lep2_motherID_isLoaded;
	int	lep1_mc_id_;
	TBranch *lep1_mc_id_branch;
	bool lep1_mc_id_isLoaded;
	int	lep2_mc_id_;
	TBranch *lep2_mc_id_branch;
	bool lep2_mc_id_isLoaded;
	int	lep1_id_;
	TBranch *lep1_id_branch;
	bool lep1_id_isLoaded;
	int	lep2_id_;
	TBranch *lep2_id_branch;
	bool lep2_id_isLoaded;
	int	lep1_idx_;
	TBranch *lep1_idx_branch;
	bool lep1_idx_isLoaded;
	int	lep2_idx_;
	TBranch *lep2_idx_branch;
	bool lep2_idx_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_;
	TBranch *jets_branch;
	bool jets_isLoaded;
	vector<float> *btags_disc_;
	TBranch *btags_disc_branch;
	bool btags_disc_isLoaded;
	vector<float> *jets_disc_;
	TBranch *jets_disc_branch;
	bool jets_disc_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *btags_;
	TBranch *btags_branch;
	bool btags_isLoaded;
	int	nbtags_;
	TBranch *nbtags_branch;
	bool nbtags_isLoaded;
	float	sf_dilepTrig_hpt_;
	TBranch *sf_dilepTrig_hpt_branch;
	bool sf_dilepTrig_hpt_isLoaded;
	float	sf_dilepTrig_lpt_;
	TBranch *sf_dilepTrig_lpt_branch;
	bool sf_dilepTrig_lpt_isLoaded;
	float	sf_dilepTrig_vlpt_;
	TBranch *sf_dilepTrig_vlpt_branch;
	bool sf_dilepTrig_vlpt_isLoaded;
	int	hyp_type_;
	TBranch *hyp_type_branch;
	bool hyp_type_isLoaded;
	float	sf_dilep_eff_;
	TBranch *sf_dilep_eff_branch;
	bool sf_dilep_eff_isLoaded;
	float	mt_;
	TBranch *mt_branch;
	bool mt_isLoaded;
	float	mt_l2_;
	TBranch *mt_l2_branch;
	bool mt_l2_isLoaded;
	float	mt2_;
	TBranch *mt2_branch;
	bool mt2_isLoaded;
	float	mGluino_;
	TBranch *mGluino_branch;
	bool mGluino_isLoaded;
	float	mLSP_;
	TBranch *mLSP_branch;
	bool mLSP_isLoaded;
	float	mSbottom_;
	TBranch *mSbottom_branch;
	bool mSbottom_isLoaded;
	float	mChargino_;
	TBranch *mChargino_branch;
	bool mChargino_isLoaded;
	int	lep1_id_gen_;
	TBranch *lep1_id_gen_branch;
	bool lep1_id_gen_isLoaded;
	int	lep2_id_gen_;
	TBranch *lep2_id_gen_branch;
	bool lep2_id_gen_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep1_p4_gen_;
	TBranch *lep1_p4_gen_branch;
	bool lep1_p4_gen_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep2_p4_gen_;
	TBranch *lep2_p4_gen_branch;
	bool lep2_p4_gen_isLoaded;
	int	lep3_id_;
	TBranch *lep3_id_branch;
	bool lep3_id_isLoaded;
	int	lep3_idx_;
	TBranch *lep3_idx_branch;
	bool lep3_idx_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep3_p4_;
	TBranch *lep3_p4_branch;
	bool lep3_p4_isLoaded;
	int	lep3_quality_;
	TBranch *lep3_quality_branch;
	bool lep3_quality_isLoaded;
	float	lep1_iso_;
	TBranch *lep1_iso_branch;
	bool lep1_iso_isLoaded;
	float	lep2_iso_;
	TBranch *lep2_iso_branch;
	bool lep2_iso_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *dilep_p4_;
	TBranch *dilep_p4_branch;
	bool dilep_p4_isLoaded;
	TString *sample_;
	TBranch *sample_branch;
	bool sample_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *genps_p4_;
	TBranch *genps_p4_branch;
	bool genps_p4_isLoaded;
	vector<int> *genps_id_;
	TBranch *genps_id_branch;
	bool genps_id_isLoaded;
	vector<int> *genps_id_mother_;
	TBranch *genps_id_mother_branch;
	bool genps_id_mother_isLoaded;
	vector<int> *genps_status_;
	TBranch *genps_status_branch;
	bool genps_status_isLoaded;
	vector<int> *genps_id_grandma_;
	TBranch *genps_id_grandma_branch;
	bool genps_id_grandma_isLoaded;
	bool	lep1_passes_id_;
	TBranch *lep1_passes_id_branch;
	bool lep1_passes_id_isLoaded;
	bool	lep2_passes_id_;
	TBranch *lep2_passes_id_branch;
	bool lep2_passes_id_isLoaded;
	float	lep1_dxyPV_;
	TBranch *lep1_dxyPV_branch;
	bool lep1_dxyPV_isLoaded;
	float	lep2_dxyPV_;
	TBranch *lep2_dxyPV_branch;
	bool lep2_dxyPV_isLoaded;
	float	lep1_dZ_;
	TBranch *lep1_dZ_branch;
	bool lep1_dZ_isLoaded;
	float	lep2_dZ_;
	TBranch *lep2_dZ_branch;
	bool lep2_dZ_isLoaded;
	float	lep1_d0_err_;
	TBranch *lep1_d0_err_branch;
	bool lep1_d0_err_isLoaded;
	float	lep2_d0_err_;
	TBranch *lep2_d0_err_branch;
	bool lep2_d0_err_isLoaded;
	float	lep1_ip3d_;
	TBranch *lep1_ip3d_branch;
	bool lep1_ip3d_isLoaded;
	float	lep2_ip3d_;
	TBranch *lep2_ip3d_branch;
	bool lep2_ip3d_isLoaded;
	float	lep1_ip3d_err_;
	TBranch *lep1_ip3d_err_branch;
	bool lep1_ip3d_err_isLoaded;
	float	lep2_ip3d_err_;
	TBranch *lep2_ip3d_err_branch;
	bool lep2_ip3d_err_isLoaded;
	int	nGoodElectrons7_;
	TBranch *nGoodElectrons7_branch;
	bool nGoodElectrons7_isLoaded;
	int	nGoodElectrons10_;
	TBranch *nGoodElectrons10_branch;
	bool nGoodElectrons10_isLoaded;
	int	nGoodElectrons25_;
	TBranch *nGoodElectrons25_branch;
	bool nGoodElectrons25_isLoaded;
	int	nGoodMuons5_;
	TBranch *nGoodMuons5_branch;
	bool nGoodMuons5_isLoaded;
	int	nGoodMuons10_;
	TBranch *nGoodMuons10_branch;
	bool nGoodMuons10_isLoaded;
	int	nGoodMuons25_;
	TBranch *nGoodMuons25_branch;
	bool nGoodMuons25_isLoaded;
	TString *filename_;
	TBranch *filename_branch;
	bool filename_isLoaded;
	float	lep1_ptrel_v1_;
	TBranch *lep1_ptrel_v1_branch;
	bool lep1_ptrel_v1_isLoaded;
	float	lep2_ptrel_v1_;
	TBranch *lep2_ptrel_v1_branch;
	bool lep2_ptrel_v1_isLoaded;
public: 
void Init(TTree *tree) {
	lep1_p4_branch = 0;
	if (tree->GetBranch("lep1_p4") != 0) {
		lep1_p4_branch = tree->GetBranch("lep1_p4");
		if (lep1_p4_branch) {lep1_p4_branch->SetAddress(&lep1_p4_);}
	}
	lep2_p4_branch = 0;
	if (tree->GetBranch("lep2_p4") != 0) {
		lep2_p4_branch = tree->GetBranch("lep2_p4");
		if (lep2_p4_branch) {lep2_p4_branch->SetAddress(&lep2_p4_);}
	}
	jets_branch = 0;
	if (tree->GetBranch("jets") != 0) {
		jets_branch = tree->GetBranch("jets");
		if (jets_branch) {jets_branch->SetAddress(&jets_);}
	}
	btags_branch = 0;
	if (tree->GetBranch("btags") != 0) {
		btags_branch = tree->GetBranch("btags");
		if (btags_branch) {btags_branch->SetAddress(&btags_);}
	}
	lep1_p4_gen_branch = 0;
	if (tree->GetBranch("lep1_p4_gen") != 0) {
		lep1_p4_gen_branch = tree->GetBranch("lep1_p4_gen");
		if (lep1_p4_gen_branch) {lep1_p4_gen_branch->SetAddress(&lep1_p4_gen_);}
	}
	lep2_p4_gen_branch = 0;
	if (tree->GetBranch("lep2_p4_gen") != 0) {
		lep2_p4_gen_branch = tree->GetBranch("lep2_p4_gen");
		if (lep2_p4_gen_branch) {lep2_p4_gen_branch->SetAddress(&lep2_p4_gen_);}
	}
	lep3_p4_branch = 0;
	if (tree->GetBranch("lep3_p4") != 0) {
		lep3_p4_branch = tree->GetBranch("lep3_p4");
		if (lep3_p4_branch) {lep3_p4_branch->SetAddress(&lep3_p4_);}
	}
	dilep_p4_branch = 0;
	if (tree->GetBranch("dilep_p4") != 0) {
		dilep_p4_branch = tree->GetBranch("dilep_p4");
		if (dilep_p4_branch) {dilep_p4_branch->SetAddress(&dilep_p4_);}
	}
	genps_p4_branch = 0;
	if (tree->GetBranch("genps_p4") != 0) {
		genps_p4_branch = tree->GetBranch("genps_p4");
		if (genps_p4_branch) {genps_p4_branch->SetAddress(&genps_p4_);}
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
	best_hyp_branch = 0;
	if (tree->GetBranch("best_hyp") != 0) {
		best_hyp_branch = tree->GetBranch("best_hyp");
		if (best_hyp_branch) {best_hyp_branch->SetAddress(&best_hyp_);}
	}
	hyp_class_branch = 0;
	if (tree->GetBranch("hyp_class") != 0) {
		hyp_class_branch = tree->GetBranch("hyp_class");
		if (hyp_class_branch) {hyp_class_branch->SetAddress(&hyp_class_);}
	}
	ht_branch = 0;
	if (tree->GetBranch("ht") != 0) {
		ht_branch = tree->GetBranch("ht");
		if (ht_branch) {ht_branch->SetAddress(&ht_);}
	}
	lep1_motherID_branch = 0;
	if (tree->GetBranch("lep1_motherID") != 0) {
		lep1_motherID_branch = tree->GetBranch("lep1_motherID");
		if (lep1_motherID_branch) {lep1_motherID_branch->SetAddress(&lep1_motherID_);}
	}
	lep2_motherID_branch = 0;
	if (tree->GetBranch("lep2_motherID") != 0) {
		lep2_motherID_branch = tree->GetBranch("lep2_motherID");
		if (lep2_motherID_branch) {lep2_motherID_branch->SetAddress(&lep2_motherID_);}
	}
	lep1_mc_id_branch = 0;
	if (tree->GetBranch("lep1_mc_id") != 0) {
		lep1_mc_id_branch = tree->GetBranch("lep1_mc_id");
		if (lep1_mc_id_branch) {lep1_mc_id_branch->SetAddress(&lep1_mc_id_);}
	}
	lep2_mc_id_branch = 0;
	if (tree->GetBranch("lep2_mc_id") != 0) {
		lep2_mc_id_branch = tree->GetBranch("lep2_mc_id");
		if (lep2_mc_id_branch) {lep2_mc_id_branch->SetAddress(&lep2_mc_id_);}
	}
	lep1_id_branch = 0;
	if (tree->GetBranch("lep1_id") != 0) {
		lep1_id_branch = tree->GetBranch("lep1_id");
		if (lep1_id_branch) {lep1_id_branch->SetAddress(&lep1_id_);}
	}
	lep2_id_branch = 0;
	if (tree->GetBranch("lep2_id") != 0) {
		lep2_id_branch = tree->GetBranch("lep2_id");
		if (lep2_id_branch) {lep2_id_branch->SetAddress(&lep2_id_);}
	}
	lep1_idx_branch = 0;
	if (tree->GetBranch("lep1_idx") != 0) {
		lep1_idx_branch = tree->GetBranch("lep1_idx");
		if (lep1_idx_branch) {lep1_idx_branch->SetAddress(&lep1_idx_);}
	}
	lep2_idx_branch = 0;
	if (tree->GetBranch("lep2_idx") != 0) {
		lep2_idx_branch = tree->GetBranch("lep2_idx");
		if (lep2_idx_branch) {lep2_idx_branch->SetAddress(&lep2_idx_);}
	}
	btags_disc_branch = 0;
	if (tree->GetBranch("btags_disc") != 0) {
		btags_disc_branch = tree->GetBranch("btags_disc");
		if (btags_disc_branch) {btags_disc_branch->SetAddress(&btags_disc_);}
	}
	jets_disc_branch = 0;
	if (tree->GetBranch("jets_disc") != 0) {
		jets_disc_branch = tree->GetBranch("jets_disc");
		if (jets_disc_branch) {jets_disc_branch->SetAddress(&jets_disc_);}
	}
	nbtags_branch = 0;
	if (tree->GetBranch("nbtags") != 0) {
		nbtags_branch = tree->GetBranch("nbtags");
		if (nbtags_branch) {nbtags_branch->SetAddress(&nbtags_);}
	}
	sf_dilepTrig_hpt_branch = 0;
	if (tree->GetBranch("sf_dilepTrig_hpt") != 0) {
		sf_dilepTrig_hpt_branch = tree->GetBranch("sf_dilepTrig_hpt");
		if (sf_dilepTrig_hpt_branch) {sf_dilepTrig_hpt_branch->SetAddress(&sf_dilepTrig_hpt_);}
	}
	sf_dilepTrig_lpt_branch = 0;
	if (tree->GetBranch("sf_dilepTrig_lpt") != 0) {
		sf_dilepTrig_lpt_branch = tree->GetBranch("sf_dilepTrig_lpt");
		if (sf_dilepTrig_lpt_branch) {sf_dilepTrig_lpt_branch->SetAddress(&sf_dilepTrig_lpt_);}
	}
	sf_dilepTrig_vlpt_branch = 0;
	if (tree->GetBranch("sf_dilepTrig_vlpt") != 0) {
		sf_dilepTrig_vlpt_branch = tree->GetBranch("sf_dilepTrig_vlpt");
		if (sf_dilepTrig_vlpt_branch) {sf_dilepTrig_vlpt_branch->SetAddress(&sf_dilepTrig_vlpt_);}
	}
	hyp_type_branch = 0;
	if (tree->GetBranch("hyp_type") != 0) {
		hyp_type_branch = tree->GetBranch("hyp_type");
		if (hyp_type_branch) {hyp_type_branch->SetAddress(&hyp_type_);}
	}
	sf_dilep_eff_branch = 0;
	if (tree->GetBranch("sf_dilep_eff") != 0) {
		sf_dilep_eff_branch = tree->GetBranch("sf_dilep_eff");
		if (sf_dilep_eff_branch) {sf_dilep_eff_branch->SetAddress(&sf_dilep_eff_);}
	}
	mt_branch = 0;
	if (tree->GetBranch("mt") != 0) {
		mt_branch = tree->GetBranch("mt");
		if (mt_branch) {mt_branch->SetAddress(&mt_);}
	}
	mt_l2_branch = 0;
	if (tree->GetBranch("mt_l2") != 0) {
		mt_l2_branch = tree->GetBranch("mt_l2");
		if (mt_l2_branch) {mt_l2_branch->SetAddress(&mt_l2_);}
	}
	mt2_branch = 0;
	if (tree->GetBranch("mt2") != 0) {
		mt2_branch = tree->GetBranch("mt2");
		if (mt2_branch) {mt2_branch->SetAddress(&mt2_);}
	}
	mGluino_branch = 0;
	if (tree->GetBranch("mGluino") != 0) {
		mGluino_branch = tree->GetBranch("mGluino");
		if (mGluino_branch) {mGluino_branch->SetAddress(&mGluino_);}
	}
	mLSP_branch = 0;
	if (tree->GetBranch("mLSP") != 0) {
		mLSP_branch = tree->GetBranch("mLSP");
		if (mLSP_branch) {mLSP_branch->SetAddress(&mLSP_);}
	}
	mSbottom_branch = 0;
	if (tree->GetBranch("mSbottom") != 0) {
		mSbottom_branch = tree->GetBranch("mSbottom");
		if (mSbottom_branch) {mSbottom_branch->SetAddress(&mSbottom_);}
	}
	mChargino_branch = 0;
	if (tree->GetBranch("mChargino") != 0) {
		mChargino_branch = tree->GetBranch("mChargino");
		if (mChargino_branch) {mChargino_branch->SetAddress(&mChargino_);}
	}
	lep1_id_gen_branch = 0;
	if (tree->GetBranch("lep1_id_gen") != 0) {
		lep1_id_gen_branch = tree->GetBranch("lep1_id_gen");
		if (lep1_id_gen_branch) {lep1_id_gen_branch->SetAddress(&lep1_id_gen_);}
	}
	lep2_id_gen_branch = 0;
	if (tree->GetBranch("lep2_id_gen") != 0) {
		lep2_id_gen_branch = tree->GetBranch("lep2_id_gen");
		if (lep2_id_gen_branch) {lep2_id_gen_branch->SetAddress(&lep2_id_gen_);}
	}
	lep3_id_branch = 0;
	if (tree->GetBranch("lep3_id") != 0) {
		lep3_id_branch = tree->GetBranch("lep3_id");
		if (lep3_id_branch) {lep3_id_branch->SetAddress(&lep3_id_);}
	}
	lep3_idx_branch = 0;
	if (tree->GetBranch("lep3_idx") != 0) {
		lep3_idx_branch = tree->GetBranch("lep3_idx");
		if (lep3_idx_branch) {lep3_idx_branch->SetAddress(&lep3_idx_);}
	}
	lep3_quality_branch = 0;
	if (tree->GetBranch("lep3_quality") != 0) {
		lep3_quality_branch = tree->GetBranch("lep3_quality");
		if (lep3_quality_branch) {lep3_quality_branch->SetAddress(&lep3_quality_);}
	}
	lep1_iso_branch = 0;
	if (tree->GetBranch("lep1_iso") != 0) {
		lep1_iso_branch = tree->GetBranch("lep1_iso");
		if (lep1_iso_branch) {lep1_iso_branch->SetAddress(&lep1_iso_);}
	}
	lep2_iso_branch = 0;
	if (tree->GetBranch("lep2_iso") != 0) {
		lep2_iso_branch = tree->GetBranch("lep2_iso");
		if (lep2_iso_branch) {lep2_iso_branch->SetAddress(&lep2_iso_);}
	}
	sample_branch = 0;
	if (tree->GetBranch("sample") != 0) {
		sample_branch = tree->GetBranch("sample");
		if (sample_branch) {sample_branch->SetAddress(&sample_);}
	}
	genps_id_branch = 0;
	if (tree->GetBranch("genps_id") != 0) {
		genps_id_branch = tree->GetBranch("genps_id");
		if (genps_id_branch) {genps_id_branch->SetAddress(&genps_id_);}
	}
	genps_id_mother_branch = 0;
	if (tree->GetBranch("genps_id_mother") != 0) {
		genps_id_mother_branch = tree->GetBranch("genps_id_mother");
		if (genps_id_mother_branch) {genps_id_mother_branch->SetAddress(&genps_id_mother_);}
	}
	genps_status_branch = 0;
	if (tree->GetBranch("genps_status") != 0) {
		genps_status_branch = tree->GetBranch("genps_status");
		if (genps_status_branch) {genps_status_branch->SetAddress(&genps_status_);}
	}
	genps_id_grandma_branch = 0;
	if (tree->GetBranch("genps_id_grandma") != 0) {
		genps_id_grandma_branch = tree->GetBranch("genps_id_grandma");
		if (genps_id_grandma_branch) {genps_id_grandma_branch->SetAddress(&genps_id_grandma_);}
	}
	lep1_passes_id_branch = 0;
	if (tree->GetBranch("lep1_passes_id") != 0) {
		lep1_passes_id_branch = tree->GetBranch("lep1_passes_id");
		if (lep1_passes_id_branch) {lep1_passes_id_branch->SetAddress(&lep1_passes_id_);}
	}
	lep2_passes_id_branch = 0;
	if (tree->GetBranch("lep2_passes_id") != 0) {
		lep2_passes_id_branch = tree->GetBranch("lep2_passes_id");
		if (lep2_passes_id_branch) {lep2_passes_id_branch->SetAddress(&lep2_passes_id_);}
	}
	lep1_dxyPV_branch = 0;
	if (tree->GetBranch("lep1_dxyPV") != 0) {
		lep1_dxyPV_branch = tree->GetBranch("lep1_dxyPV");
		if (lep1_dxyPV_branch) {lep1_dxyPV_branch->SetAddress(&lep1_dxyPV_);}
	}
	lep2_dxyPV_branch = 0;
	if (tree->GetBranch("lep2_dxyPV") != 0) {
		lep2_dxyPV_branch = tree->GetBranch("lep2_dxyPV");
		if (lep2_dxyPV_branch) {lep2_dxyPV_branch->SetAddress(&lep2_dxyPV_);}
	}
	lep1_dZ_branch = 0;
	if (tree->GetBranch("lep1_dZ") != 0) {
		lep1_dZ_branch = tree->GetBranch("lep1_dZ");
		if (lep1_dZ_branch) {lep1_dZ_branch->SetAddress(&lep1_dZ_);}
	}
	lep2_dZ_branch = 0;
	if (tree->GetBranch("lep2_dZ") != 0) {
		lep2_dZ_branch = tree->GetBranch("lep2_dZ");
		if (lep2_dZ_branch) {lep2_dZ_branch->SetAddress(&lep2_dZ_);}
	}
	lep1_d0_err_branch = 0;
	if (tree->GetBranch("lep1_d0_err") != 0) {
		lep1_d0_err_branch = tree->GetBranch("lep1_d0_err");
		if (lep1_d0_err_branch) {lep1_d0_err_branch->SetAddress(&lep1_d0_err_);}
	}
	lep2_d0_err_branch = 0;
	if (tree->GetBranch("lep2_d0_err") != 0) {
		lep2_d0_err_branch = tree->GetBranch("lep2_d0_err");
		if (lep2_d0_err_branch) {lep2_d0_err_branch->SetAddress(&lep2_d0_err_);}
	}
	lep1_ip3d_branch = 0;
	if (tree->GetBranch("lep1_ip3d") != 0) {
		lep1_ip3d_branch = tree->GetBranch("lep1_ip3d");
		if (lep1_ip3d_branch) {lep1_ip3d_branch->SetAddress(&lep1_ip3d_);}
	}
	lep2_ip3d_branch = 0;
	if (tree->GetBranch("lep2_ip3d") != 0) {
		lep2_ip3d_branch = tree->GetBranch("lep2_ip3d");
		if (lep2_ip3d_branch) {lep2_ip3d_branch->SetAddress(&lep2_ip3d_);}
	}
	lep1_ip3d_err_branch = 0;
	if (tree->GetBranch("lep1_ip3d_err") != 0) {
		lep1_ip3d_err_branch = tree->GetBranch("lep1_ip3d_err");
		if (lep1_ip3d_err_branch) {lep1_ip3d_err_branch->SetAddress(&lep1_ip3d_err_);}
	}
	lep2_ip3d_err_branch = 0;
	if (tree->GetBranch("lep2_ip3d_err") != 0) {
		lep2_ip3d_err_branch = tree->GetBranch("lep2_ip3d_err");
		if (lep2_ip3d_err_branch) {lep2_ip3d_err_branch->SetAddress(&lep2_ip3d_err_);}
	}
	nGoodElectrons7_branch = 0;
	if (tree->GetBranch("nGoodElectrons7") != 0) {
		nGoodElectrons7_branch = tree->GetBranch("nGoodElectrons7");
		if (nGoodElectrons7_branch) {nGoodElectrons7_branch->SetAddress(&nGoodElectrons7_);}
	}
	nGoodElectrons10_branch = 0;
	if (tree->GetBranch("nGoodElectrons10") != 0) {
		nGoodElectrons10_branch = tree->GetBranch("nGoodElectrons10");
		if (nGoodElectrons10_branch) {nGoodElectrons10_branch->SetAddress(&nGoodElectrons10_);}
	}
	nGoodElectrons25_branch = 0;
	if (tree->GetBranch("nGoodElectrons25") != 0) {
		nGoodElectrons25_branch = tree->GetBranch("nGoodElectrons25");
		if (nGoodElectrons25_branch) {nGoodElectrons25_branch->SetAddress(&nGoodElectrons25_);}
	}
	nGoodMuons5_branch = 0;
	if (tree->GetBranch("nGoodMuons5") != 0) {
		nGoodMuons5_branch = tree->GetBranch("nGoodMuons5");
		if (nGoodMuons5_branch) {nGoodMuons5_branch->SetAddress(&nGoodMuons5_);}
	}
	nGoodMuons10_branch = 0;
	if (tree->GetBranch("nGoodMuons10") != 0) {
		nGoodMuons10_branch = tree->GetBranch("nGoodMuons10");
		if (nGoodMuons10_branch) {nGoodMuons10_branch->SetAddress(&nGoodMuons10_);}
	}
	nGoodMuons25_branch = 0;
	if (tree->GetBranch("nGoodMuons25") != 0) {
		nGoodMuons25_branch = tree->GetBranch("nGoodMuons25");
		if (nGoodMuons25_branch) {nGoodMuons25_branch->SetAddress(&nGoodMuons25_);}
	}
	filename_branch = 0;
	if (tree->GetBranch("filename") != 0) {
		filename_branch = tree->GetBranch("filename");
		if (filename_branch) {filename_branch->SetAddress(&filename_);}
	}
	lep1_ptrel_v1_branch = 0;
	if (tree->GetBranch("lep1_ptrel_v1") != 0) {
		lep1_ptrel_v1_branch = tree->GetBranch("lep1_ptrel_v1");
		if (lep1_ptrel_v1_branch) {lep1_ptrel_v1_branch->SetAddress(&lep1_ptrel_v1_);}
	}
	lep2_ptrel_v1_branch = 0;
	if (tree->GetBranch("lep2_ptrel_v1") != 0) {
		lep2_ptrel_v1_branch = tree->GetBranch("lep2_ptrel_v1");
		if (lep2_ptrel_v1_branch) {lep2_ptrel_v1_branch->SetAddress(&lep2_ptrel_v1_);}
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
		best_hyp_isLoaded = false;
		hyp_class_isLoaded = false;
		lep1_p4_isLoaded = false;
		lep2_p4_isLoaded = false;
		ht_isLoaded = false;
		lep1_motherID_isLoaded = false;
		lep2_motherID_isLoaded = false;
		lep1_mc_id_isLoaded = false;
		lep2_mc_id_isLoaded = false;
		lep1_id_isLoaded = false;
		lep2_id_isLoaded = false;
		lep1_idx_isLoaded = false;
		lep2_idx_isLoaded = false;
		jets_isLoaded = false;
		btags_disc_isLoaded = false;
		jets_disc_isLoaded = false;
		btags_isLoaded = false;
		nbtags_isLoaded = false;
		sf_dilepTrig_hpt_isLoaded = false;
		sf_dilepTrig_lpt_isLoaded = false;
		sf_dilepTrig_vlpt_isLoaded = false;
		hyp_type_isLoaded = false;
		sf_dilep_eff_isLoaded = false;
		mt_isLoaded = false;
		mt_l2_isLoaded = false;
		mt2_isLoaded = false;
		mGluino_isLoaded = false;
		mLSP_isLoaded = false;
		mSbottom_isLoaded = false;
		mChargino_isLoaded = false;
		lep1_id_gen_isLoaded = false;
		lep2_id_gen_isLoaded = false;
		lep1_p4_gen_isLoaded = false;
		lep2_p4_gen_isLoaded = false;
		lep3_id_isLoaded = false;
		lep3_idx_isLoaded = false;
		lep3_p4_isLoaded = false;
		lep3_quality_isLoaded = false;
		lep1_iso_isLoaded = false;
		lep2_iso_isLoaded = false;
		dilep_p4_isLoaded = false;
		sample_isLoaded = false;
		genps_p4_isLoaded = false;
		genps_id_isLoaded = false;
		genps_id_mother_isLoaded = false;
		genps_status_isLoaded = false;
		genps_id_grandma_isLoaded = false;
		lep1_passes_id_isLoaded = false;
		lep2_passes_id_isLoaded = false;
		lep1_dxyPV_isLoaded = false;
		lep2_dxyPV_isLoaded = false;
		lep1_dZ_isLoaded = false;
		lep2_dZ_isLoaded = false;
		lep1_d0_err_isLoaded = false;
		lep2_d0_err_isLoaded = false;
		lep1_ip3d_isLoaded = false;
		lep2_ip3d_isLoaded = false;
		lep1_ip3d_err_isLoaded = false;
		lep2_ip3d_err_isLoaded = false;
		nGoodElectrons7_isLoaded = false;
		nGoodElectrons10_isLoaded = false;
		nGoodElectrons25_isLoaded = false;
		nGoodMuons5_isLoaded = false;
		nGoodMuons10_isLoaded = false;
		nGoodMuons25_isLoaded = false;
		filename_isLoaded = false;
		lep1_ptrel_v1_isLoaded = false;
		lep2_ptrel_v1_isLoaded = false;
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
	if (best_hyp_branch != 0) best_hyp();
	if (hyp_class_branch != 0) hyp_class();
	if (lep1_p4_branch != 0) lep1_p4();
	if (lep2_p4_branch != 0) lep2_p4();
	if (ht_branch != 0) ht();
	if (lep1_motherID_branch != 0) lep1_motherID();
	if (lep2_motherID_branch != 0) lep2_motherID();
	if (lep1_mc_id_branch != 0) lep1_mc_id();
	if (lep2_mc_id_branch != 0) lep2_mc_id();
	if (lep1_id_branch != 0) lep1_id();
	if (lep2_id_branch != 0) lep2_id();
	if (lep1_idx_branch != 0) lep1_idx();
	if (lep2_idx_branch != 0) lep2_idx();
	if (jets_branch != 0) jets();
	if (btags_disc_branch != 0) btags_disc();
	if (jets_disc_branch != 0) jets_disc();
	if (btags_branch != 0) btags();
	if (nbtags_branch != 0) nbtags();
	if (sf_dilepTrig_hpt_branch != 0) sf_dilepTrig_hpt();
	if (sf_dilepTrig_lpt_branch != 0) sf_dilepTrig_lpt();
	if (sf_dilepTrig_vlpt_branch != 0) sf_dilepTrig_vlpt();
	if (hyp_type_branch != 0) hyp_type();
	if (sf_dilep_eff_branch != 0) sf_dilep_eff();
	if (mt_branch != 0) mt();
	if (mt_l2_branch != 0) mt_l2();
	if (mt2_branch != 0) mt2();
	if (mGluino_branch != 0) mGluino();
	if (mLSP_branch != 0) mLSP();
	if (mSbottom_branch != 0) mSbottom();
	if (mChargino_branch != 0) mChargino();
	if (lep1_id_gen_branch != 0) lep1_id_gen();
	if (lep2_id_gen_branch != 0) lep2_id_gen();
	if (lep1_p4_gen_branch != 0) lep1_p4_gen();
	if (lep2_p4_gen_branch != 0) lep2_p4_gen();
	if (lep3_id_branch != 0) lep3_id();
	if (lep3_idx_branch != 0) lep3_idx();
	if (lep3_p4_branch != 0) lep3_p4();
	if (lep3_quality_branch != 0) lep3_quality();
	if (lep1_iso_branch != 0) lep1_iso();
	if (lep2_iso_branch != 0) lep2_iso();
	if (dilep_p4_branch != 0) dilep_p4();
	if (sample_branch != 0) sample();
	if (genps_p4_branch != 0) genps_p4();
	if (genps_id_branch != 0) genps_id();
	if (genps_id_mother_branch != 0) genps_id_mother();
	if (genps_status_branch != 0) genps_status();
	if (genps_id_grandma_branch != 0) genps_id_grandma();
	if (lep1_passes_id_branch != 0) lep1_passes_id();
	if (lep2_passes_id_branch != 0) lep2_passes_id();
	if (lep1_dxyPV_branch != 0) lep1_dxyPV();
	if (lep2_dxyPV_branch != 0) lep2_dxyPV();
	if (lep1_dZ_branch != 0) lep1_dZ();
	if (lep2_dZ_branch != 0) lep2_dZ();
	if (lep1_d0_err_branch != 0) lep1_d0_err();
	if (lep2_d0_err_branch != 0) lep2_d0_err();
	if (lep1_ip3d_branch != 0) lep1_ip3d();
	if (lep2_ip3d_branch != 0) lep2_ip3d();
	if (lep1_ip3d_err_branch != 0) lep1_ip3d_err();
	if (lep2_ip3d_err_branch != 0) lep2_ip3d_err();
	if (nGoodElectrons7_branch != 0) nGoodElectrons7();
	if (nGoodElectrons10_branch != 0) nGoodElectrons10();
	if (nGoodElectrons25_branch != 0) nGoodElectrons25();
	if (nGoodMuons5_branch != 0) nGoodMuons5();
	if (nGoodMuons10_branch != 0) nGoodMuons10();
	if (nGoodMuons25_branch != 0) nGoodMuons25();
	if (filename_branch != 0) filename();
	if (lep1_ptrel_v1_branch != 0) lep1_ptrel_v1();
	if (lep2_ptrel_v1_branch != 0) lep2_ptrel_v1();
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
	int &njets()
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
	int &best_hyp()
	{
		if (not best_hyp_isLoaded) {
			if (best_hyp_branch != 0) {
				best_hyp_branch->GetEntry(index);
			} else { 
				printf("branch best_hyp_branch does not exist!\n");
				exit(1);
			}
			best_hyp_isLoaded = true;
		}
		return best_hyp_;
	}
	int &hyp_class()
	{
		if (not hyp_class_isLoaded) {
			if (hyp_class_branch != 0) {
				hyp_class_branch->GetEntry(index);
			} else { 
				printf("branch hyp_class_branch does not exist!\n");
				exit(1);
			}
			hyp_class_isLoaded = true;
		}
		return hyp_class_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4()
	{
		if (not lep1_p4_isLoaded) {
			if (lep1_p4_branch != 0) {
				lep1_p4_branch->GetEntry(index);
			} else { 
				printf("branch lep1_p4_branch does not exist!\n");
				exit(1);
			}
			lep1_p4_isLoaded = true;
		}
		return *lep1_p4_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4()
	{
		if (not lep2_p4_isLoaded) {
			if (lep2_p4_branch != 0) {
				lep2_p4_branch->GetEntry(index);
			} else { 
				printf("branch lep2_p4_branch does not exist!\n");
				exit(1);
			}
			lep2_p4_isLoaded = true;
		}
		return *lep2_p4_;
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
	int &lep1_motherID()
	{
		if (not lep1_motherID_isLoaded) {
			if (lep1_motherID_branch != 0) {
				lep1_motherID_branch->GetEntry(index);
			} else { 
				printf("branch lep1_motherID_branch does not exist!\n");
				exit(1);
			}
			lep1_motherID_isLoaded = true;
		}
		return lep1_motherID_;
	}
	int &lep2_motherID()
	{
		if (not lep2_motherID_isLoaded) {
			if (lep2_motherID_branch != 0) {
				lep2_motherID_branch->GetEntry(index);
			} else { 
				printf("branch lep2_motherID_branch does not exist!\n");
				exit(1);
			}
			lep2_motherID_isLoaded = true;
		}
		return lep2_motherID_;
	}
	int &lep1_mc_id()
	{
		if (not lep1_mc_id_isLoaded) {
			if (lep1_mc_id_branch != 0) {
				lep1_mc_id_branch->GetEntry(index);
			} else { 
				printf("branch lep1_mc_id_branch does not exist!\n");
				exit(1);
			}
			lep1_mc_id_isLoaded = true;
		}
		return lep1_mc_id_;
	}
	int &lep2_mc_id()
	{
		if (not lep2_mc_id_isLoaded) {
			if (lep2_mc_id_branch != 0) {
				lep2_mc_id_branch->GetEntry(index);
			} else { 
				printf("branch lep2_mc_id_branch does not exist!\n");
				exit(1);
			}
			lep2_mc_id_isLoaded = true;
		}
		return lep2_mc_id_;
	}
	int &lep1_id()
	{
		if (not lep1_id_isLoaded) {
			if (lep1_id_branch != 0) {
				lep1_id_branch->GetEntry(index);
			} else { 
				printf("branch lep1_id_branch does not exist!\n");
				exit(1);
			}
			lep1_id_isLoaded = true;
		}
		return lep1_id_;
	}
	int &lep2_id()
	{
		if (not lep2_id_isLoaded) {
			if (lep2_id_branch != 0) {
				lep2_id_branch->GetEntry(index);
			} else { 
				printf("branch lep2_id_branch does not exist!\n");
				exit(1);
			}
			lep2_id_isLoaded = true;
		}
		return lep2_id_;
	}
	int &lep1_idx()
	{
		if (not lep1_idx_isLoaded) {
			if (lep1_idx_branch != 0) {
				lep1_idx_branch->GetEntry(index);
			} else { 
				printf("branch lep1_idx_branch does not exist!\n");
				exit(1);
			}
			lep1_idx_isLoaded = true;
		}
		return lep1_idx_;
	}
	int &lep2_idx()
	{
		if (not lep2_idx_isLoaded) {
			if (lep2_idx_branch != 0) {
				lep2_idx_branch->GetEntry(index);
			} else { 
				printf("branch lep2_idx_branch does not exist!\n");
				exit(1);
			}
			lep2_idx_isLoaded = true;
		}
		return lep2_idx_;
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
	vector<float> &btags_disc()
	{
		if (not btags_disc_isLoaded) {
			if (btags_disc_branch != 0) {
				btags_disc_branch->GetEntry(index);
			} else { 
				printf("branch btags_disc_branch does not exist!\n");
				exit(1);
			}
			btags_disc_isLoaded = true;
		}
		return *btags_disc_;
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
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags()
	{
		if (not btags_isLoaded) {
			if (btags_branch != 0) {
				btags_branch->GetEntry(index);
			} else { 
				printf("branch btags_branch does not exist!\n");
				exit(1);
			}
			btags_isLoaded = true;
		}
		return *btags_;
	}
	int &nbtags()
	{
		if (not nbtags_isLoaded) {
			if (nbtags_branch != 0) {
				nbtags_branch->GetEntry(index);
			} else { 
				printf("branch nbtags_branch does not exist!\n");
				exit(1);
			}
			nbtags_isLoaded = true;
		}
		return nbtags_;
	}
	float &sf_dilepTrig_hpt()
	{
		if (not sf_dilepTrig_hpt_isLoaded) {
			if (sf_dilepTrig_hpt_branch != 0) {
				sf_dilepTrig_hpt_branch->GetEntry(index);
			} else { 
				printf("branch sf_dilepTrig_hpt_branch does not exist!\n");
				exit(1);
			}
			sf_dilepTrig_hpt_isLoaded = true;
		}
		return sf_dilepTrig_hpt_;
	}
	float &sf_dilepTrig_lpt()
	{
		if (not sf_dilepTrig_lpt_isLoaded) {
			if (sf_dilepTrig_lpt_branch != 0) {
				sf_dilepTrig_lpt_branch->GetEntry(index);
			} else { 
				printf("branch sf_dilepTrig_lpt_branch does not exist!\n");
				exit(1);
			}
			sf_dilepTrig_lpt_isLoaded = true;
		}
		return sf_dilepTrig_lpt_;
	}
	float &sf_dilepTrig_vlpt()
	{
		if (not sf_dilepTrig_vlpt_isLoaded) {
			if (sf_dilepTrig_vlpt_branch != 0) {
				sf_dilepTrig_vlpt_branch->GetEntry(index);
			} else { 
				printf("branch sf_dilepTrig_vlpt_branch does not exist!\n");
				exit(1);
			}
			sf_dilepTrig_vlpt_isLoaded = true;
		}
		return sf_dilepTrig_vlpt_;
	}
	int &hyp_type()
	{
		if (not hyp_type_isLoaded) {
			if (hyp_type_branch != 0) {
				hyp_type_branch->GetEntry(index);
			} else { 
				printf("branch hyp_type_branch does not exist!\n");
				exit(1);
			}
			hyp_type_isLoaded = true;
		}
		return hyp_type_;
	}
	float &sf_dilep_eff()
	{
		if (not sf_dilep_eff_isLoaded) {
			if (sf_dilep_eff_branch != 0) {
				sf_dilep_eff_branch->GetEntry(index);
			} else { 
				printf("branch sf_dilep_eff_branch does not exist!\n");
				exit(1);
			}
			sf_dilep_eff_isLoaded = true;
		}
		return sf_dilep_eff_;
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
	float &mt_l2()
	{
		if (not mt_l2_isLoaded) {
			if (mt_l2_branch != 0) {
				mt_l2_branch->GetEntry(index);
			} else { 
				printf("branch mt_l2_branch does not exist!\n");
				exit(1);
			}
			mt_l2_isLoaded = true;
		}
		return mt_l2_;
	}
	float &mt2()
	{
		if (not mt2_isLoaded) {
			if (mt2_branch != 0) {
				mt2_branch->GetEntry(index);
			} else { 
				printf("branch mt2_branch does not exist!\n");
				exit(1);
			}
			mt2_isLoaded = true;
		}
		return mt2_;
	}
	float &mGluino()
	{
		if (not mGluino_isLoaded) {
			if (mGluino_branch != 0) {
				mGluino_branch->GetEntry(index);
			} else { 
				printf("branch mGluino_branch does not exist!\n");
				exit(1);
			}
			mGluino_isLoaded = true;
		}
		return mGluino_;
	}
	float &mLSP()
	{
		if (not mLSP_isLoaded) {
			if (mLSP_branch != 0) {
				mLSP_branch->GetEntry(index);
			} else { 
				printf("branch mLSP_branch does not exist!\n");
				exit(1);
			}
			mLSP_isLoaded = true;
		}
		return mLSP_;
	}
	float &mSbottom()
	{
		if (not mSbottom_isLoaded) {
			if (mSbottom_branch != 0) {
				mSbottom_branch->GetEntry(index);
			} else { 
				printf("branch mSbottom_branch does not exist!\n");
				exit(1);
			}
			mSbottom_isLoaded = true;
		}
		return mSbottom_;
	}
	float &mChargino()
	{
		if (not mChargino_isLoaded) {
			if (mChargino_branch != 0) {
				mChargino_branch->GetEntry(index);
			} else { 
				printf("branch mChargino_branch does not exist!\n");
				exit(1);
			}
			mChargino_isLoaded = true;
		}
		return mChargino_;
	}
	int &lep1_id_gen()
	{
		if (not lep1_id_gen_isLoaded) {
			if (lep1_id_gen_branch != 0) {
				lep1_id_gen_branch->GetEntry(index);
			} else { 
				printf("branch lep1_id_gen_branch does not exist!\n");
				exit(1);
			}
			lep1_id_gen_isLoaded = true;
		}
		return lep1_id_gen_;
	}
	int &lep2_id_gen()
	{
		if (not lep2_id_gen_isLoaded) {
			if (lep2_id_gen_branch != 0) {
				lep2_id_gen_branch->GetEntry(index);
			} else { 
				printf("branch lep2_id_gen_branch does not exist!\n");
				exit(1);
			}
			lep2_id_gen_isLoaded = true;
		}
		return lep2_id_gen_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4_gen()
	{
		if (not lep1_p4_gen_isLoaded) {
			if (lep1_p4_gen_branch != 0) {
				lep1_p4_gen_branch->GetEntry(index);
			} else { 
				printf("branch lep1_p4_gen_branch does not exist!\n");
				exit(1);
			}
			lep1_p4_gen_isLoaded = true;
		}
		return *lep1_p4_gen_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4_gen()
	{
		if (not lep2_p4_gen_isLoaded) {
			if (lep2_p4_gen_branch != 0) {
				lep2_p4_gen_branch->GetEntry(index);
			} else { 
				printf("branch lep2_p4_gen_branch does not exist!\n");
				exit(1);
			}
			lep2_p4_gen_isLoaded = true;
		}
		return *lep2_p4_gen_;
	}
	int &lep3_id()
	{
		if (not lep3_id_isLoaded) {
			if (lep3_id_branch != 0) {
				lep3_id_branch->GetEntry(index);
			} else { 
				printf("branch lep3_id_branch does not exist!\n");
				exit(1);
			}
			lep3_id_isLoaded = true;
		}
		return lep3_id_;
	}
	int &lep3_idx()
	{
		if (not lep3_idx_isLoaded) {
			if (lep3_idx_branch != 0) {
				lep3_idx_branch->GetEntry(index);
			} else { 
				printf("branch lep3_idx_branch does not exist!\n");
				exit(1);
			}
			lep3_idx_isLoaded = true;
		}
		return lep3_idx_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep3_p4()
	{
		if (not lep3_p4_isLoaded) {
			if (lep3_p4_branch != 0) {
				lep3_p4_branch->GetEntry(index);
			} else { 
				printf("branch lep3_p4_branch does not exist!\n");
				exit(1);
			}
			lep3_p4_isLoaded = true;
		}
		return *lep3_p4_;
	}
	int &lep3_quality()
	{
		if (not lep3_quality_isLoaded) {
			if (lep3_quality_branch != 0) {
				lep3_quality_branch->GetEntry(index);
			} else { 
				printf("branch lep3_quality_branch does not exist!\n");
				exit(1);
			}
			lep3_quality_isLoaded = true;
		}
		return lep3_quality_;
	}
	float &lep1_iso()
	{
		if (not lep1_iso_isLoaded) {
			if (lep1_iso_branch != 0) {
				lep1_iso_branch->GetEntry(index);
			} else { 
				printf("branch lep1_iso_branch does not exist!\n");
				exit(1);
			}
			lep1_iso_isLoaded = true;
		}
		return lep1_iso_;
	}
	float &lep2_iso()
	{
		if (not lep2_iso_isLoaded) {
			if (lep2_iso_branch != 0) {
				lep2_iso_branch->GetEntry(index);
			} else { 
				printf("branch lep2_iso_branch does not exist!\n");
				exit(1);
			}
			lep2_iso_isLoaded = true;
		}
		return lep2_iso_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4()
	{
		if (not dilep_p4_isLoaded) {
			if (dilep_p4_branch != 0) {
				dilep_p4_branch->GetEntry(index);
			} else { 
				printf("branch dilep_p4_branch does not exist!\n");
				exit(1);
			}
			dilep_p4_isLoaded = true;
		}
		return *dilep_p4_;
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
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps_p4()
	{
		if (not genps_p4_isLoaded) {
			if (genps_p4_branch != 0) {
				genps_p4_branch->GetEntry(index);
			} else { 
				printf("branch genps_p4_branch does not exist!\n");
				exit(1);
			}
			genps_p4_isLoaded = true;
		}
		return *genps_p4_;
	}
	vector<int> &genps_id()
	{
		if (not genps_id_isLoaded) {
			if (genps_id_branch != 0) {
				genps_id_branch->GetEntry(index);
			} else { 
				printf("branch genps_id_branch does not exist!\n");
				exit(1);
			}
			genps_id_isLoaded = true;
		}
		return *genps_id_;
	}
	vector<int> &genps_id_mother()
	{
		if (not genps_id_mother_isLoaded) {
			if (genps_id_mother_branch != 0) {
				genps_id_mother_branch->GetEntry(index);
			} else { 
				printf("branch genps_id_mother_branch does not exist!\n");
				exit(1);
			}
			genps_id_mother_isLoaded = true;
		}
		return *genps_id_mother_;
	}
	vector<int> &genps_status()
	{
		if (not genps_status_isLoaded) {
			if (genps_status_branch != 0) {
				genps_status_branch->GetEntry(index);
			} else { 
				printf("branch genps_status_branch does not exist!\n");
				exit(1);
			}
			genps_status_isLoaded = true;
		}
		return *genps_status_;
	}
	vector<int> &genps_id_grandma()
	{
		if (not genps_id_grandma_isLoaded) {
			if (genps_id_grandma_branch != 0) {
				genps_id_grandma_branch->GetEntry(index);
			} else { 
				printf("branch genps_id_grandma_branch does not exist!\n");
				exit(1);
			}
			genps_id_grandma_isLoaded = true;
		}
		return *genps_id_grandma_;
	}
	bool &	lep1_passes_id()
	{
		if (not lep1_passes_id_isLoaded) {
			if (lep1_passes_id_branch != 0) {
				lep1_passes_id_branch->GetEntry(index);
			} else { 
				printf("branch lep1_passes_id_branch does not exist!\n");
				exit(1);
			}
			lep1_passes_id_isLoaded = true;
		}
		return lep1_passes_id_;
	}
	bool &	lep2_passes_id()
	{
		if (not lep2_passes_id_isLoaded) {
			if (lep2_passes_id_branch != 0) {
				lep2_passes_id_branch->GetEntry(index);
			} else { 
				printf("branch lep2_passes_id_branch does not exist!\n");
				exit(1);
			}
			lep2_passes_id_isLoaded = true;
		}
		return lep2_passes_id_;
	}
	float &lep1_dxyPV()
	{
		if (not lep1_dxyPV_isLoaded) {
			if (lep1_dxyPV_branch != 0) {
				lep1_dxyPV_branch->GetEntry(index);
			} else { 
				printf("branch lep1_dxyPV_branch does not exist!\n");
				exit(1);
			}
			lep1_dxyPV_isLoaded = true;
		}
		return lep1_dxyPV_;
	}
	float &lep2_dxyPV()
	{
		if (not lep2_dxyPV_isLoaded) {
			if (lep2_dxyPV_branch != 0) {
				lep2_dxyPV_branch->GetEntry(index);
			} else { 
				printf("branch lep2_dxyPV_branch does not exist!\n");
				exit(1);
			}
			lep2_dxyPV_isLoaded = true;
		}
		return lep2_dxyPV_;
	}
	float &lep1_dZ()
	{
		if (not lep1_dZ_isLoaded) {
			if (lep1_dZ_branch != 0) {
				lep1_dZ_branch->GetEntry(index);
			} else { 
				printf("branch lep1_dZ_branch does not exist!\n");
				exit(1);
			}
			lep1_dZ_isLoaded = true;
		}
		return lep1_dZ_;
	}
	float &lep2_dZ()
	{
		if (not lep2_dZ_isLoaded) {
			if (lep2_dZ_branch != 0) {
				lep2_dZ_branch->GetEntry(index);
			} else { 
				printf("branch lep2_dZ_branch does not exist!\n");
				exit(1);
			}
			lep2_dZ_isLoaded = true;
		}
		return lep2_dZ_;
	}
	float &lep1_d0_err()
	{
		if (not lep1_d0_err_isLoaded) {
			if (lep1_d0_err_branch != 0) {
				lep1_d0_err_branch->GetEntry(index);
			} else { 
				printf("branch lep1_d0_err_branch does not exist!\n");
				exit(1);
			}
			lep1_d0_err_isLoaded = true;
		}
		return lep1_d0_err_;
	}
	float &lep2_d0_err()
	{
		if (not lep2_d0_err_isLoaded) {
			if (lep2_d0_err_branch != 0) {
				lep2_d0_err_branch->GetEntry(index);
			} else { 
				printf("branch lep2_d0_err_branch does not exist!\n");
				exit(1);
			}
			lep2_d0_err_isLoaded = true;
		}
		return lep2_d0_err_;
	}
	float &lep1_ip3d()
	{
		if (not lep1_ip3d_isLoaded) {
			if (lep1_ip3d_branch != 0) {
				lep1_ip3d_branch->GetEntry(index);
			} else { 
				printf("branch lep1_ip3d_branch does not exist!\n");
				exit(1);
			}
			lep1_ip3d_isLoaded = true;
		}
		return lep1_ip3d_;
	}
	float &lep2_ip3d()
	{
		if (not lep2_ip3d_isLoaded) {
			if (lep2_ip3d_branch != 0) {
				lep2_ip3d_branch->GetEntry(index);
			} else { 
				printf("branch lep2_ip3d_branch does not exist!\n");
				exit(1);
			}
			lep2_ip3d_isLoaded = true;
		}
		return lep2_ip3d_;
	}
	float &lep1_ip3d_err()
	{
		if (not lep1_ip3d_err_isLoaded) {
			if (lep1_ip3d_err_branch != 0) {
				lep1_ip3d_err_branch->GetEntry(index);
			} else { 
				printf("branch lep1_ip3d_err_branch does not exist!\n");
				exit(1);
			}
			lep1_ip3d_err_isLoaded = true;
		}
		return lep1_ip3d_err_;
	}
	float &lep2_ip3d_err()
	{
		if (not lep2_ip3d_err_isLoaded) {
			if (lep2_ip3d_err_branch != 0) {
				lep2_ip3d_err_branch->GetEntry(index);
			} else { 
				printf("branch lep2_ip3d_err_branch does not exist!\n");
				exit(1);
			}
			lep2_ip3d_err_isLoaded = true;
		}
		return lep2_ip3d_err_;
	}
	int &nGoodElectrons7()
	{
		if (not nGoodElectrons7_isLoaded) {
			if (nGoodElectrons7_branch != 0) {
				nGoodElectrons7_branch->GetEntry(index);
			} else { 
				printf("branch nGoodElectrons7_branch does not exist!\n");
				exit(1);
			}
			nGoodElectrons7_isLoaded = true;
		}
		return nGoodElectrons7_;
	}
	int &nGoodElectrons10()
	{
		if (not nGoodElectrons10_isLoaded) {
			if (nGoodElectrons10_branch != 0) {
				nGoodElectrons10_branch->GetEntry(index);
			} else { 
				printf("branch nGoodElectrons10_branch does not exist!\n");
				exit(1);
			}
			nGoodElectrons10_isLoaded = true;
		}
		return nGoodElectrons10_;
	}
	int &nGoodElectrons25()
	{
		if (not nGoodElectrons25_isLoaded) {
			if (nGoodElectrons25_branch != 0) {
				nGoodElectrons25_branch->GetEntry(index);
			} else { 
				printf("branch nGoodElectrons25_branch does not exist!\n");
				exit(1);
			}
			nGoodElectrons25_isLoaded = true;
		}
		return nGoodElectrons25_;
	}
	int &nGoodMuons5()
	{
		if (not nGoodMuons5_isLoaded) {
			if (nGoodMuons5_branch != 0) {
				nGoodMuons5_branch->GetEntry(index);
			} else { 
				printf("branch nGoodMuons5_branch does not exist!\n");
				exit(1);
			}
			nGoodMuons5_isLoaded = true;
		}
		return nGoodMuons5_;
	}
	int &nGoodMuons10()
	{
		if (not nGoodMuons10_isLoaded) {
			if (nGoodMuons10_branch != 0) {
				nGoodMuons10_branch->GetEntry(index);
			} else { 
				printf("branch nGoodMuons10_branch does not exist!\n");
				exit(1);
			}
			nGoodMuons10_isLoaded = true;
		}
		return nGoodMuons10_;
	}
	int &nGoodMuons25()
	{
		if (not nGoodMuons25_isLoaded) {
			if (nGoodMuons25_branch != 0) {
				nGoodMuons25_branch->GetEntry(index);
			} else { 
				printf("branch nGoodMuons25_branch does not exist!\n");
				exit(1);
			}
			nGoodMuons25_isLoaded = true;
		}
		return nGoodMuons25_;
	}
	TString &filename()
	{
		if (not filename_isLoaded) {
			if (filename_branch != 0) {
				filename_branch->GetEntry(index);
			} else { 
				printf("branch filename_branch does not exist!\n");
				exit(1);
			}
			filename_isLoaded = true;
		}
		return *filename_;
	}
	float &lep1_ptrel_v1()
	{
		if (not lep1_ptrel_v1_isLoaded) {
			if (lep1_ptrel_v1_branch != 0) {
				lep1_ptrel_v1_branch->GetEntry(index);
			} else { 
				printf("branch lep1_ptrel_v1_branch does not exist!\n");
				exit(1);
			}
			lep1_ptrel_v1_isLoaded = true;
		}
		return lep1_ptrel_v1_;
	}
	float &lep2_ptrel_v1()
	{
		if (not lep2_ptrel_v1_isLoaded) {
			if (lep2_ptrel_v1_branch != 0) {
				lep2_ptrel_v1_branch->GetEntry(index);
			} else { 
				printf("branch lep2_ptrel_v1_branch does not exist!\n");
				exit(1);
			}
			lep2_ptrel_v1_isLoaded = true;
		}
		return lep2_ptrel_v1_;
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
	int &njets();
	int &best_hyp();
	int &hyp_class();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4();
	float &ht();
	int &lep1_motherID();
	int &lep2_motherID();
	int &lep1_mc_id();
	int &lep2_mc_id();
	int &lep1_id();
	int &lep2_id();
	int &lep1_idx();
	int &lep2_idx();
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets();
	vector<float> &btags_disc();
	vector<float> &jets_disc();
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags();
	int &nbtags();
	float &sf_dilepTrig_hpt();
	float &sf_dilepTrig_lpt();
	float &sf_dilepTrig_vlpt();
	int &hyp_type();
	float &sf_dilep_eff();
	float &mt();
	float &mt_l2();
	float &mt2();
	float &mGluino();
	float &mLSP();
	float &mSbottom();
	float &mChargino();
	int &lep1_id_gen();
	int &lep2_id_gen();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4_gen();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4_gen();
	int &lep3_id();
	int &lep3_idx();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep3_p4();
	int &lep3_quality();
	float &lep1_iso();
	float &lep2_iso();
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4();
	TString &sample();
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps_p4();
	vector<int> &genps_id();
	vector<int> &genps_id_mother();
	vector<int> &genps_status();
	vector<int> &genps_id_grandma();
	bool &lep1_passes_id();
	bool &lep2_passes_id();
	float &lep1_dxyPV();
	float &lep2_dxyPV();
	float &lep1_dZ();
	float &lep2_dZ();
	float &lep1_d0_err();
	float &lep2_d0_err();
	float &lep1_ip3d();
	float &lep2_ip3d();
	float &lep1_ip3d_err();
	float &lep2_ip3d_err();
	int &nGoodElectrons7();
	int &nGoodElectrons10();
	int &nGoodElectrons25();
	int &nGoodMuons5();
	int &nGoodMuons10();
	int &nGoodMuons25();
	TString &filename();
	float &lep1_ptrel_v1();
	float &lep2_ptrel_v1();
}
#endif
