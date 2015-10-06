#include "SS.h"
SSAG samesign;

void SSAG::Init(TTree *tree) {
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
	jets_corr_branch = 0;
	if (tree->GetBranch("jets_corr") != 0) {
		jets_corr_branch = tree->GetBranch("jets_corr");
		if (jets_corr_branch) {jets_corr_branch->SetAddress(&jets_corr_);}
	}
	btags_branch = 0;
	if (tree->GetBranch("btags") != 0) {
		btags_branch = tree->GetBranch("btags");
		if (btags_branch) {btags_branch->SetAddress(&btags_);}
	}
	btags_corr_branch = 0;
	if (tree->GetBranch("btags_corr") != 0) {
		btags_corr_branch = tree->GetBranch("btags_corr");
		if (btags_corr_branch) {btags_corr_branch->SetAddress(&btags_corr_);}
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
	jet_close_lep1_branch = 0;
	if (tree->GetBranch("jet_close_lep1") != 0) {
		jet_close_lep1_branch = tree->GetBranch("jet_close_lep1");
		if (jet_close_lep1_branch) {jet_close_lep1_branch->SetAddress(&jet_close_lep1_);}
	}
	jet_close_lep2_branch = 0;
	if (tree->GetBranch("jet_close_lep2") != 0) {
		jet_close_lep2_branch = tree->GetBranch("jet_close_lep2");
		if (jet_close_lep2_branch) {jet_close_lep2_branch->SetAddress(&jet_close_lep2_);}
	}
	mostJets_branch = 0;
	if (tree->GetBranch("mostJets") != 0) {
		mostJets_branch = tree->GetBranch("mostJets");
		if (mostJets_branch) {mostJets_branch->SetAddress(&mostJets_);}
	}
	lep1_closeJet_branch = 0;
	if (tree->GetBranch("lep1_closeJet") != 0) {
		lep1_closeJet_branch = tree->GetBranch("lep1_closeJet");
		if (lep1_closeJet_branch) {lep1_closeJet_branch->SetAddress(&lep1_closeJet_);}
	}
	lep2_closeJet_branch = 0;
	if (tree->GetBranch("lep2_closeJet") != 0) {
		lep2_closeJet_branch = tree->GetBranch("lep2_closeJet");
		if (lep2_closeJet_branch) {lep2_closeJet_branch->SetAddress(&lep2_closeJet_);}
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
	corrMET_branch = 0;
	if (tree->GetBranch("corrMET") != 0) {
		corrMET_branch = tree->GetBranch("corrMET");
		if (corrMET_branch) {corrMET_branch->SetAddress(&corrMET_);}
	}
	corrMETphi_branch = 0;
	if (tree->GetBranch("corrMETphi") != 0) {
		corrMETphi_branch = tree->GetBranch("corrMETphi");
		if (corrMETphi_branch) {corrMETphi_branch->SetAddress(&corrMETphi_);}
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
	genweights_branch = 0;
	if (tree->GetBranch("genweights") != 0) {
		genweights_branch = tree->GetBranch("genweights");
		if (genweights_branch) {genweights_branch->SetAddress(&genweights_);}
	}
	genweightsID_branch = 0;
	if (tree->GetBranch("genweightsID") != 0) {
		genweightsID_branch = tree->GetBranch("genweightsID");
		if (genweightsID_branch) {genweightsID_branch->SetAddress(&genweightsID_);}
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
	njets_corr_branch = 0;
	if (tree->GetBranch("njets_corr") != 0) {
		njets_corr_branch = tree->GetBranch("njets_corr");
		if (njets_corr_branch) {njets_corr_branch->SetAddress(&njets_corr_);}
	}
	hyp_class_branch = 0;
	if (tree->GetBranch("hyp_class") != 0) {
		hyp_class_branch = tree->GetBranch("hyp_class");
		if (hyp_class_branch) {hyp_class_branch->SetAddress(&hyp_class_);}
	}
	ht_corr_branch = 0;
	if (tree->GetBranch("ht_corr") != 0) {
		ht_corr_branch = tree->GetBranch("ht_corr");
		if (ht_corr_branch) {ht_corr_branch->SetAddress(&ht_corr_);}
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
	lep1_coneCorrPt_branch = 0;
	if (tree->GetBranch("lep1_coneCorrPt") != 0) {
		lep1_coneCorrPt_branch = tree->GetBranch("lep1_coneCorrPt");
		if (lep1_coneCorrPt_branch) {lep1_coneCorrPt_branch->SetAddress(&lep1_coneCorrPt_);}
	}
	lep2_coneCorrPt_branch = 0;
	if (tree->GetBranch("lep2_coneCorrPt") != 0) {
		lep2_coneCorrPt_branch = tree->GetBranch("lep2_coneCorrPt");
		if (lep2_coneCorrPt_branch) {lep2_coneCorrPt_branch->SetAddress(&lep2_coneCorrPt_);}
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
	jets_JEC_branch = 0;
	if (tree->GetBranch("jets_JEC") != 0) {
		jets_JEC_branch = tree->GetBranch("jets_JEC");
		if (jets_JEC_branch) {jets_JEC_branch->SetAddress(&jets_JEC_);}
	}
	btags_JEC_branch = 0;
	if (tree->GetBranch("btags_JEC") != 0) {
		btags_JEC_branch = tree->GetBranch("btags_JEC");
		if (btags_JEC_branch) {btags_JEC_branch->SetAddress(&btags_JEC_);}
	}
	jets_undoJEC_branch = 0;
	if (tree->GetBranch("jets_undoJEC") != 0) {
		jets_undoJEC_branch = tree->GetBranch("jets_undoJEC");
		if (jets_undoJEC_branch) {jets_undoJEC_branch->SetAddress(&jets_undoJEC_);}
	}
	btags_undoJEC_branch = 0;
	if (tree->GetBranch("btags_undoJEC") != 0) {
		btags_undoJEC_branch = tree->GetBranch("btags_undoJEC");
		if (btags_undoJEC_branch) {btags_undoJEC_branch->SetAddress(&btags_undoJEC_);}
	}
	btags_corr_disc_branch = 0;
	if (tree->GetBranch("btags_corr_disc") != 0) {
		btags_corr_disc_branch = tree->GetBranch("btags_corr_disc");
		if (btags_corr_disc_branch) {btags_corr_disc_branch->SetAddress(&btags_corr_disc_);}
	}
	jets_corr_disc_branch = 0;
	if (tree->GetBranch("jets_corr_disc") != 0) {
		jets_corr_disc_branch = tree->GetBranch("jets_corr_disc");
		if (jets_corr_disc_branch) {jets_corr_disc_branch->SetAddress(&jets_corr_disc_);}
	}
	jets_corr_JEC_branch = 0;
	if (tree->GetBranch("jets_corr_JEC") != 0) {
		jets_corr_JEC_branch = tree->GetBranch("jets_corr_JEC");
		if (jets_corr_JEC_branch) {jets_corr_JEC_branch->SetAddress(&jets_corr_JEC_);}
	}
	btags_corr_JEC_branch = 0;
	if (tree->GetBranch("btags_corr_JEC") != 0) {
		btags_corr_JEC_branch = tree->GetBranch("btags_corr_JEC");
		if (btags_corr_JEC_branch) {btags_corr_JEC_branch->SetAddress(&btags_corr_JEC_);}
	}
	jets_corr_undoJEC_branch = 0;
	if (tree->GetBranch("jets_corr_undoJEC") != 0) {
		jets_corr_undoJEC_branch = tree->GetBranch("jets_corr_undoJEC");
		if (jets_corr_undoJEC_branch) {jets_corr_undoJEC_branch->SetAddress(&jets_corr_undoJEC_);}
	}
	btags_corr_undoJEC_branch = 0;
	if (tree->GetBranch("btags_corr_undoJEC") != 0) {
		btags_corr_undoJEC_branch = tree->GetBranch("btags_corr_undoJEC");
		if (btags_corr_undoJEC_branch) {btags_corr_undoJEC_branch->SetAddress(&btags_corr_undoJEC_);}
	}
	nbtags_branch = 0;
	if (tree->GetBranch("nbtags") != 0) {
		nbtags_branch = tree->GetBranch("nbtags");
		if (nbtags_branch) {nbtags_branch->SetAddress(&nbtags_);}
	}
	nbtags_corr_branch = 0;
	if (tree->GetBranch("nbtags_corr") != 0) {
		nbtags_corr_branch = tree->GetBranch("nbtags_corr");
		if (nbtags_corr_branch) {nbtags_corr_branch->SetAddress(&nbtags_corr_);}
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
	mtmin_branch = 0;
	if (tree->GetBranch("mtmin") != 0) {
		mtmin_branch = tree->GetBranch("mtmin");
		if (mtmin_branch) {mtmin_branch->SetAddress(&mtmin_);}
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
	genps_idx_mother_branch = 0;
	if (tree->GetBranch("genps_idx_mother") != 0) {
		genps_idx_mother_branch = tree->GetBranch("genps_idx_mother");
		if (genps_idx_mother_branch) {genps_idx_mother_branch->SetAddress(&genps_idx_mother_);}
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
	lep3_passes_id_branch = 0;
	if (tree->GetBranch("lep3_passes_id") != 0) {
		lep3_passes_id_branch = tree->GetBranch("lep3_passes_id");
		if (lep3_passes_id_branch) {lep3_passes_id_branch->SetAddress(&lep3_passes_id_);}
	}
	lep3_tight_branch = 0;
	if (tree->GetBranch("lep3_tight") != 0) {
		lep3_tight_branch = tree->GetBranch("lep3_tight");
		if (lep3_tight_branch) {lep3_tight_branch->SetAddress(&lep3_tight_);}
	}
	lep3_veto_branch = 0;
	if (tree->GetBranch("lep3_veto") != 0) {
		lep3_veto_branch = tree->GetBranch("lep3_veto");
		if (lep3_veto_branch) {lep3_veto_branch->SetAddress(&lep3_veto_);}
	}
	lep3_fo_branch = 0;
	if (tree->GetBranch("lep3_fo") != 0) {
		lep3_fo_branch = tree->GetBranch("lep3_fo");
		if (lep3_fo_branch) {lep3_fo_branch->SetAddress(&lep3_fo_);}
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
	lep1_MVA_branch = 0;
	if (tree->GetBranch("lep1_MVA") != 0) {
		lep1_MVA_branch = tree->GetBranch("lep1_MVA");
		if (lep1_MVA_branch) {lep1_MVA_branch->SetAddress(&lep1_MVA_);}
	}
	lep2_MVA_branch = 0;
	if (tree->GetBranch("lep2_MVA") != 0) {
		lep2_MVA_branch = tree->GetBranch("lep2_MVA");
		if (lep2_MVA_branch) {lep2_MVA_branch->SetAddress(&lep2_MVA_);}
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
	nVetoElectrons7_branch = 0;
	if (tree->GetBranch("nVetoElectrons7") != 0) {
		nVetoElectrons7_branch = tree->GetBranch("nVetoElectrons7");
		if (nVetoElectrons7_branch) {nVetoElectrons7_branch->SetAddress(&nVetoElectrons7_);}
	}
	nVetoElectrons10_branch = 0;
	if (tree->GetBranch("nVetoElectrons10") != 0) {
		nVetoElectrons10_branch = tree->GetBranch("nVetoElectrons10");
		if (nVetoElectrons10_branch) {nVetoElectrons10_branch->SetAddress(&nVetoElectrons10_);}
	}
	nVetoElectrons25_branch = 0;
	if (tree->GetBranch("nVetoElectrons25") != 0) {
		nVetoElectrons25_branch = tree->GetBranch("nVetoElectrons25");
		if (nVetoElectrons25_branch) {nVetoElectrons25_branch->SetAddress(&nVetoElectrons25_);}
	}
	nVetoMuons5_branch = 0;
	if (tree->GetBranch("nVetoMuons5") != 0) {
		nVetoMuons5_branch = tree->GetBranch("nVetoMuons5");
		if (nVetoMuons5_branch) {nVetoMuons5_branch->SetAddress(&nVetoMuons5_);}
	}
	nVetoMuons10_branch = 0;
	if (tree->GetBranch("nVetoMuons10") != 0) {
		nVetoMuons10_branch = tree->GetBranch("nVetoMuons10");
		if (nVetoMuons10_branch) {nVetoMuons10_branch->SetAddress(&nVetoMuons10_);}
	}
	nVetoMuons25_branch = 0;
	if (tree->GetBranch("nVetoMuons25") != 0) {
		nVetoMuons25_branch = tree->GetBranch("nVetoMuons25");
		if (nVetoMuons25_branch) {nVetoMuons25_branch->SetAddress(&nVetoMuons25_);}
	}
	filename_branch = 0;
	if (tree->GetBranch("filename") != 0) {
		filename_branch = tree->GetBranch("filename");
		if (filename_branch) {filename_branch->SetAddress(&filename_);}
	}
	lep1_ptrel_v0_branch = 0;
	if (tree->GetBranch("lep1_ptrel_v0") != 0) {
		lep1_ptrel_v0_branch = tree->GetBranch("lep1_ptrel_v0");
		if (lep1_ptrel_v0_branch) {lep1_ptrel_v0_branch->SetAddress(&lep1_ptrel_v0_);}
	}
	lep1_ptrel_v1_branch = 0;
	if (tree->GetBranch("lep1_ptrel_v1") != 0) {
		lep1_ptrel_v1_branch = tree->GetBranch("lep1_ptrel_v1");
		if (lep1_ptrel_v1_branch) {lep1_ptrel_v1_branch->SetAddress(&lep1_ptrel_v1_);}
	}
	lep2_ptrel_v0_branch = 0;
	if (tree->GetBranch("lep2_ptrel_v0") != 0) {
		lep2_ptrel_v0_branch = tree->GetBranch("lep2_ptrel_v0");
		if (lep2_ptrel_v0_branch) {lep2_ptrel_v0_branch->SetAddress(&lep2_ptrel_v0_);}
	}
	lep2_ptrel_v1_branch = 0;
	if (tree->GetBranch("lep2_ptrel_v1") != 0) {
		lep2_ptrel_v1_branch = tree->GetBranch("lep2_ptrel_v1");
		if (lep2_ptrel_v1_branch) {lep2_ptrel_v1_branch->SetAddress(&lep2_ptrel_v1_);}
	}
	lep1_miniIso_branch = 0;
	if (tree->GetBranch("lep1_miniIso") != 0) {
		lep1_miniIso_branch = tree->GetBranch("lep1_miniIso");
		if (lep1_miniIso_branch) {lep1_miniIso_branch->SetAddress(&lep1_miniIso_);}
	}
	lep2_miniIso_branch = 0;
	if (tree->GetBranch("lep2_miniIso") != 0) {
		lep2_miniIso_branch = tree->GetBranch("lep2_miniIso");
		if (lep2_miniIso_branch) {lep2_miniIso_branch->SetAddress(&lep2_miniIso_);}
	}
	eleID_kfhits_branch = 0;
	if (tree->GetBranch("eleID_kfhits") != 0) {
		eleID_kfhits_branch = tree->GetBranch("eleID_kfhits");
		if (eleID_kfhits_branch) {eleID_kfhits_branch->SetAddress(&eleID_kfhits_);}
	}
	eleID_oldsigmaietaieta_branch = 0;
	if (tree->GetBranch("eleID_oldsigmaietaieta") != 0) {
		eleID_oldsigmaietaieta_branch = tree->GetBranch("eleID_oldsigmaietaieta");
		if (eleID_oldsigmaietaieta_branch) {eleID_oldsigmaietaieta_branch->SetAddress(&eleID_oldsigmaietaieta_);}
	}
	eleID_oldsigmaiphiiphi_branch = 0;
	if (tree->GetBranch("eleID_oldsigmaiphiiphi") != 0) {
		eleID_oldsigmaiphiiphi_branch = tree->GetBranch("eleID_oldsigmaiphiiphi");
		if (eleID_oldsigmaiphiiphi_branch) {eleID_oldsigmaiphiiphi_branch->SetAddress(&eleID_oldsigmaiphiiphi_);}
	}
	eleID_oldcircularity_branch = 0;
	if (tree->GetBranch("eleID_oldcircularity") != 0) {
		eleID_oldcircularity_branch = tree->GetBranch("eleID_oldcircularity");
		if (eleID_oldcircularity_branch) {eleID_oldcircularity_branch->SetAddress(&eleID_oldcircularity_);}
	}
	eleID_oldr9_branch = 0;
	if (tree->GetBranch("eleID_oldr9") != 0) {
		eleID_oldr9_branch = tree->GetBranch("eleID_oldr9");
		if (eleID_oldr9_branch) {eleID_oldr9_branch->SetAddress(&eleID_oldr9_);}
	}
	eleID_scletawidth_branch = 0;
	if (tree->GetBranch("eleID_scletawidth") != 0) {
		eleID_scletawidth_branch = tree->GetBranch("eleID_scletawidth");
		if (eleID_scletawidth_branch) {eleID_scletawidth_branch->SetAddress(&eleID_scletawidth_);}
	}
	eleID_sclphiwidth_branch = 0;
	if (tree->GetBranch("eleID_sclphiwidth") != 0) {
		eleID_sclphiwidth_branch = tree->GetBranch("eleID_sclphiwidth");
		if (eleID_sclphiwidth_branch) {eleID_sclphiwidth_branch->SetAddress(&eleID_sclphiwidth_);}
	}
	eleID_he_branch = 0;
	if (tree->GetBranch("eleID_he") != 0) {
		eleID_he_branch = tree->GetBranch("eleID_he");
		if (eleID_he_branch) {eleID_he_branch->SetAddress(&eleID_he_);}
	}
	eleID_psEoverEraw_branch = 0;
	if (tree->GetBranch("eleID_psEoverEraw") != 0) {
		eleID_psEoverEraw_branch = tree->GetBranch("eleID_psEoverEraw");
		if (eleID_psEoverEraw_branch) {eleID_psEoverEraw_branch->SetAddress(&eleID_psEoverEraw_);}
	}
	eleID_kfchi2_branch = 0;
	if (tree->GetBranch("eleID_kfchi2") != 0) {
		eleID_kfchi2_branch = tree->GetBranch("eleID_kfchi2");
		if (eleID_kfchi2_branch) {eleID_kfchi2_branch->SetAddress(&eleID_kfchi2_);}
	}
	eleID_chi2_hits_branch = 0;
	if (tree->GetBranch("eleID_chi2_hits") != 0) {
		eleID_chi2_hits_branch = tree->GetBranch("eleID_chi2_hits");
		if (eleID_chi2_hits_branch) {eleID_chi2_hits_branch->SetAddress(&eleID_chi2_hits_);}
	}
	eleID_fbrem_branch = 0;
	if (tree->GetBranch("eleID_fbrem") != 0) {
		eleID_fbrem_branch = tree->GetBranch("eleID_fbrem");
		if (eleID_fbrem_branch) {eleID_fbrem_branch->SetAddress(&eleID_fbrem_);}
	}
	eleID_ep_branch = 0;
	if (tree->GetBranch("eleID_ep") != 0) {
		eleID_ep_branch = tree->GetBranch("eleID_ep");
		if (eleID_ep_branch) {eleID_ep_branch->SetAddress(&eleID_ep_);}
	}
	eleID_eelepout_branch = 0;
	if (tree->GetBranch("eleID_eelepout") != 0) {
		eleID_eelepout_branch = tree->GetBranch("eleID_eelepout");
		if (eleID_eelepout_branch) {eleID_eelepout_branch->SetAddress(&eleID_eelepout_);}
	}
	eleID_IoEmIop_branch = 0;
	if (tree->GetBranch("eleID_IoEmIop") != 0) {
		eleID_IoEmIop_branch = tree->GetBranch("eleID_IoEmIop");
		if (eleID_IoEmIop_branch) {eleID_IoEmIop_branch->SetAddress(&eleID_IoEmIop_);}
	}
	eleID_deltaetain_branch = 0;
	if (tree->GetBranch("eleID_deltaetain") != 0) {
		eleID_deltaetain_branch = tree->GetBranch("eleID_deltaetain");
		if (eleID_deltaetain_branch) {eleID_deltaetain_branch->SetAddress(&eleID_deltaetain_);}
	}
	eleID_deltaphiin_branch = 0;
	if (tree->GetBranch("eleID_deltaphiin") != 0) {
		eleID_deltaphiin_branch = tree->GetBranch("eleID_deltaphiin");
		if (eleID_deltaphiin_branch) {eleID_deltaphiin_branch->SetAddress(&eleID_deltaphiin_);}
	}
	eleID_deltaetaseed_branch = 0;
	if (tree->GetBranch("eleID_deltaetaseed") != 0) {
		eleID_deltaetaseed_branch = tree->GetBranch("eleID_deltaetaseed");
		if (eleID_deltaetaseed_branch) {eleID_deltaetaseed_branch->SetAddress(&eleID_deltaetaseed_);}
	}
	eleID_pT_branch = 0;
	if (tree->GetBranch("eleID_pT") != 0) {
		eleID_pT_branch = tree->GetBranch("eleID_pT");
		if (eleID_pT_branch) {eleID_pT_branch->SetAddress(&eleID_pT_);}
	}
	eleID_isbarrel_branch = 0;
	if (tree->GetBranch("eleID_isbarrel") != 0) {
		eleID_isbarrel_branch = tree->GetBranch("eleID_isbarrel");
		if (eleID_isbarrel_branch) {eleID_isbarrel_branch->SetAddress(&eleID_isbarrel_);}
	}
	eleID_isendcap_branch = 0;
	if (tree->GetBranch("eleID_isendcap") != 0) {
		eleID_isendcap_branch = tree->GetBranch("eleID_isendcap");
		if (eleID_isendcap_branch) {eleID_isendcap_branch->SetAddress(&eleID_isendcap_);}
	}
	eleID_scl_eta_branch = 0;
	if (tree->GetBranch("eleID_scl_eta") != 0) {
		eleID_scl_eta_branch = tree->GetBranch("eleID_scl_eta");
		if (eleID_scl_eta_branch) {eleID_scl_eta_branch->SetAddress(&eleID_scl_eta_);}
	}
	muID_dzPV_branch = 0;
	if (tree->GetBranch("muID_dzPV") != 0) {
		muID_dzPV_branch = tree->GetBranch("muID_dzPV");
		if (muID_dzPV_branch) {muID_dzPV_branch->SetAddress(&muID_dzPV_);}
	}
	muID_ptSig_branch = 0;
	if (tree->GetBranch("muID_ptSig") != 0) {
		muID_ptSig_branch = tree->GetBranch("muID_ptSig");
		if (muID_ptSig_branch) {muID_ptSig_branch->SetAddress(&muID_ptSig_);}
	}
	muID_ip3dSig_branch = 0;
	if (tree->GetBranch("muID_ip3dSig") != 0) {
		muID_ip3dSig_branch = tree->GetBranch("muID_ip3dSig");
		if (muID_ip3dSig_branch) {muID_ip3dSig_branch->SetAddress(&muID_ip3dSig_);}
	}
	muID_medMuonPOG_branch = 0;
	if (tree->GetBranch("muID_medMuonPOG") != 0) {
		muID_medMuonPOG_branch = tree->GetBranch("muID_medMuonPOG");
		if (muID_medMuonPOG_branch) {muID_medMuonPOG_branch->SetAddress(&muID_medMuonPOG_);}
	}
	muID_pt_branch = 0;
	if (tree->GetBranch("muID_pt") != 0) {
		muID_pt_branch = tree->GetBranch("muID_pt");
		if (muID_pt_branch) {muID_pt_branch->SetAddress(&muID_pt_);}
	}
	muID_eta_branch = 0;
	if (tree->GetBranch("muID_eta") != 0) {
		muID_eta_branch = tree->GetBranch("muID_eta");
		if (muID_eta_branch) {muID_eta_branch->SetAddress(&muID_eta_);}
	}
	trueNumInt_branch = 0;
	if (tree->GetBranch("trueNumInt") != 0) {
		trueNumInt_branch = tree->GetBranch("trueNumInt");
		if (trueNumInt_branch) {trueNumInt_branch->SetAddress(&trueNumInt_);}
	}
	nPUvertices_branch = 0;
	if (tree->GetBranch("nPUvertices") != 0) {
		nPUvertices_branch = tree->GetBranch("nPUvertices");
		if (nPUvertices_branch) {nPUvertices_branch->SetAddress(&nPUvertices_);}
	}
	nGoodVertices_branch = 0;
	if (tree->GetBranch("nGoodVertices") != 0) {
		nGoodVertices_branch = tree->GetBranch("nGoodVertices");
		if (nGoodVertices_branch) {nGoodVertices_branch->SetAddress(&nGoodVertices_);}
	}
	lep1_trigMatch_noIsoReq_branch = 0;
	if (tree->GetBranch("lep1_trigMatch_noIsoReq") != 0) {
		lep1_trigMatch_noIsoReq_branch = tree->GetBranch("lep1_trigMatch_noIsoReq");
		if (lep1_trigMatch_noIsoReq_branch) {lep1_trigMatch_noIsoReq_branch->SetAddress(&lep1_trigMatch_noIsoReq_);}
	}
	lep1_trigMatch_isoReq_branch = 0;
	if (tree->GetBranch("lep1_trigMatch_isoReq") != 0) {
		lep1_trigMatch_isoReq_branch = tree->GetBranch("lep1_trigMatch_isoReq");
		if (lep1_trigMatch_isoReq_branch) {lep1_trigMatch_isoReq_branch->SetAddress(&lep1_trigMatch_isoReq_);}
	}
	lep2_trigMatch_noIsoReq_branch = 0;
	if (tree->GetBranch("lep2_trigMatch_noIsoReq") != 0) {
		lep2_trigMatch_noIsoReq_branch = tree->GetBranch("lep2_trigMatch_noIsoReq");
		if (lep2_trigMatch_noIsoReq_branch) {lep2_trigMatch_noIsoReq_branch->SetAddress(&lep2_trigMatch_noIsoReq_);}
	}
	lep2_trigMatch_isoReq_branch = 0;
	if (tree->GetBranch("lep2_trigMatch_isoReq") != 0) {
		lep2_trigMatch_isoReq_branch = tree->GetBranch("lep2_trigMatch_isoReq");
		if (lep2_trigMatch_isoReq_branch) {lep2_trigMatch_isoReq_branch->SetAddress(&lep2_trigMatch_isoReq_);}
	}
	met3p0_branch = 0;
	if (tree->GetBranch("met3p0") != 0) {
		met3p0_branch = tree->GetBranch("met3p0");
		if (met3p0_branch) {met3p0_branch->SetAddress(&met3p0_);}
	}
	jet_corr_pt_branch = 0;
	if (tree->GetBranch("jet_corr_pt") != 0) {
		jet_corr_pt_branch = tree->GetBranch("jet_corr_pt");
		if (jet_corr_pt_branch) {jet_corr_pt_branch->SetAddress(&jet_corr_pt_);}
	}
	metphi3p0_branch = 0;
	if (tree->GetBranch("metphi3p0") != 0) {
		metphi3p0_branch = tree->GetBranch("metphi3p0");
		if (metphi3p0_branch) {metphi3p0_branch->SetAddress(&metphi3p0_);}
	}
	passes_met_filters_branch = 0;
	if (tree->GetBranch("passes_met_filters") != 0) {
		passes_met_filters_branch = tree->GetBranch("passes_met_filters");
		if (passes_met_filters_branch) {passes_met_filters_branch->SetAddress(&passes_met_filters_);}
	}
	madeExtraZ_branch = 0;
	if (tree->GetBranch("madeExtraZ") != 0) {
		madeExtraZ_branch = tree->GetBranch("madeExtraZ");
		if (madeExtraZ_branch) {madeExtraZ_branch->SetAddress(&madeExtraZ_);}
	}
	madeExtraG_branch = 0;
	if (tree->GetBranch("madeExtraG") != 0) {
		madeExtraG_branch = tree->GetBranch("madeExtraG");
		if (madeExtraG_branch) {madeExtraG_branch->SetAddress(&madeExtraG_);}
	}
	lep1_isGoodLeg_branch = 0;
	if (tree->GetBranch("lep1_isGoodLeg") != 0) {
		lep1_isGoodLeg_branch = tree->GetBranch("lep1_isGoodLeg");
		if (lep1_isGoodLeg_branch) {lep1_isGoodLeg_branch->SetAddress(&lep1_isGoodLeg_);}
	}
	lep2_isGoodLeg_branch = 0;
	if (tree->GetBranch("lep2_isGoodLeg") != 0) {
		lep2_isGoodLeg_branch = tree->GetBranch("lep2_isGoodLeg");
		if (lep2_isGoodLeg_branch) {lep2_isGoodLeg_branch->SetAddress(&lep2_isGoodLeg_);}
	}
	lep1_isFakeLeg_branch = 0;
	if (tree->GetBranch("lep1_isFakeLeg") != 0) {
		lep1_isFakeLeg_branch = tree->GetBranch("lep1_isFakeLeg");
		if (lep1_isFakeLeg_branch) {lep1_isFakeLeg_branch->SetAddress(&lep1_isFakeLeg_);}
	}
	lep2_isFakeLeg_branch = 0;
	if (tree->GetBranch("lep2_isFakeLeg") != 0) {
		lep2_isFakeLeg_branch = tree->GetBranch("lep2_isFakeLeg");
		if (lep2_isFakeLeg_branch) {lep2_isFakeLeg_branch->SetAddress(&lep2_isFakeLeg_);}
	}
	lep1_multiIso_branch = 0;
	if (tree->GetBranch("lep1_multiIso") != 0) {
		lep1_multiIso_branch = tree->GetBranch("lep1_multiIso");
		if (lep1_multiIso_branch) {lep1_multiIso_branch->SetAddress(&lep1_multiIso_);}
	}
	lep2_multiIso_branch = 0;
	if (tree->GetBranch("lep2_multiIso") != 0) {
		lep2_multiIso_branch = tree->GetBranch("lep2_multiIso");
		if (lep2_multiIso_branch) {lep2_multiIso_branch->SetAddress(&lep2_multiIso_);}
	}
	lep1_sip_branch = 0;
	if (tree->GetBranch("lep1_sip") != 0) {
		lep1_sip_branch = tree->GetBranch("lep1_sip");
		if (lep1_sip_branch) {lep1_sip_branch->SetAddress(&lep1_sip_);}
	}
	lep2_sip_branch = 0;
	if (tree->GetBranch("lep2_sip") != 0) {
		lep2_sip_branch = tree->GetBranch("lep2_sip");
		if (lep2_sip_branch) {lep2_sip_branch->SetAddress(&lep2_sip_);}
	}
	passed_id_inSituFR_lep1_branch = 0;
	if (tree->GetBranch("passed_id_inSituFR_lep1") != 0) {
		passed_id_inSituFR_lep1_branch = tree->GetBranch("passed_id_inSituFR_lep1");
		if (passed_id_inSituFR_lep1_branch) {passed_id_inSituFR_lep1_branch->SetAddress(&passed_id_inSituFR_lep1_);}
	}
	passed_id_inSituFR_lep2_branch = 0;
	if (tree->GetBranch("passed_id_inSituFR_lep2") != 0) {
		passed_id_inSituFR_lep2_branch = tree->GetBranch("passed_id_inSituFR_lep2");
		if (passed_id_inSituFR_lep2_branch) {passed_id_inSituFR_lep2_branch->SetAddress(&passed_id_inSituFR_lep2_);}
	}
	fired_trigger_branch = 0;
	if (tree->GetBranch("fired_trigger") != 0) {
		fired_trigger_branch = tree->GetBranch("fired_trigger");
		if (fired_trigger_branch) {fired_trigger_branch->SetAddress(&fired_trigger_);}
	}
	triggers_branch = 0;
	if (tree->GetBranch("triggers") != 0) {
		triggers_branch = tree->GetBranch("triggers");
		if (triggers_branch) {triggers_branch->SetAddress(&triggers_);}
	}
  tree->SetMakeClass(0);
}
void SSAG::GetEntry(unsigned int idx) 
	// this only marks branches as not loaded, saving a lot of time
	{
		index = idx;
		met_isLoaded = false;
		metPhi_isLoaded = false;
		corrMET_isLoaded = false;
		corrMETphi_isLoaded = false;
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
		genweights_isLoaded = false;
		genweightsID_isLoaded = false;
		gen_met_phi_isLoaded = false;
		njets_isLoaded = false;
		njets_corr_isLoaded = false;
		hyp_class_isLoaded = false;
		lep1_p4_isLoaded = false;
		lep2_p4_isLoaded = false;
		ht_corr_isLoaded = false;
		ht_isLoaded = false;
		lep1_motherID_isLoaded = false;
		lep2_motherID_isLoaded = false;
		lep1_mc_id_isLoaded = false;
		lep2_mc_id_isLoaded = false;
		lep1_id_isLoaded = false;
		lep2_id_isLoaded = false;
		lep1_coneCorrPt_isLoaded = false;
		lep2_coneCorrPt_isLoaded = false;
		lep1_idx_isLoaded = false;
		lep2_idx_isLoaded = false;
		jets_isLoaded = false;
		jets_corr_isLoaded = false;
		btags_disc_isLoaded = false;
		jets_disc_isLoaded = false;
		jets_JEC_isLoaded = false;
		btags_JEC_isLoaded = false;
		jets_undoJEC_isLoaded = false;
		btags_undoJEC_isLoaded = false;
		btags_corr_disc_isLoaded = false;
		jets_corr_disc_isLoaded = false;
		jets_corr_JEC_isLoaded = false;
		btags_corr_JEC_isLoaded = false;
		jets_corr_undoJEC_isLoaded = false;
		btags_corr_undoJEC_isLoaded = false;
		btags_isLoaded = false;
		nbtags_isLoaded = false;
		btags_corr_isLoaded = false;
		nbtags_corr_isLoaded = false;
		sf_dilepTrig_hpt_isLoaded = false;
		sf_dilepTrig_lpt_isLoaded = false;
		sf_dilepTrig_vlpt_isLoaded = false;
		hyp_type_isLoaded = false;
		sf_dilep_eff_isLoaded = false;
		mt_isLoaded = false;
		mt_l2_isLoaded = false;
		mt2_isLoaded = false;
		mtmin_isLoaded = false;
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
		genps_p4_isLoaded = false;
		genps_id_isLoaded = false;
		genps_id_mother_isLoaded = false;
		genps_idx_mother_isLoaded = false;
		genps_status_isLoaded = false;
		genps_id_grandma_isLoaded = false;
		lep1_passes_id_isLoaded = false;
		lep2_passes_id_isLoaded = false;
		lep3_passes_id_isLoaded = false;
		lep3_tight_isLoaded = false;
		lep3_veto_isLoaded = false;
		lep3_fo_isLoaded = false;
		lep1_dxyPV_isLoaded = false;
		lep2_dxyPV_isLoaded = false;
		lep1_dZ_isLoaded = false;
		lep2_dZ_isLoaded = false;
		lep1_d0_err_isLoaded = false;
		lep2_d0_err_isLoaded = false;
		lep1_ip3d_isLoaded = false;
		lep2_ip3d_isLoaded = false;
		lep1_MVA_isLoaded = false;
		lep2_MVA_isLoaded = false;
		lep1_ip3d_err_isLoaded = false;
		lep2_ip3d_err_isLoaded = false;
		nVetoElectrons7_isLoaded = false;
		nVetoElectrons10_isLoaded = false;
		nVetoElectrons25_isLoaded = false;
		nVetoMuons5_isLoaded = false;
		nVetoMuons10_isLoaded = false;
		nVetoMuons25_isLoaded = false;
		filename_isLoaded = false;
		lep1_ptrel_v0_isLoaded = false;
		lep1_ptrel_v1_isLoaded = false;
		lep2_ptrel_v0_isLoaded = false;
		lep2_ptrel_v1_isLoaded = false;
		lep1_miniIso_isLoaded = false;
		lep2_miniIso_isLoaded = false;
		jet_close_lep1_isLoaded = false;
		jet_close_lep2_isLoaded = false;
		eleID_kfhits_isLoaded = false;
		eleID_oldsigmaietaieta_isLoaded = false;
		eleID_oldsigmaiphiiphi_isLoaded = false;
		eleID_oldcircularity_isLoaded = false;
		eleID_oldr9_isLoaded = false;
		eleID_scletawidth_isLoaded = false;
		eleID_sclphiwidth_isLoaded = false;
		eleID_he_isLoaded = false;
		eleID_psEoverEraw_isLoaded = false;
		eleID_kfchi2_isLoaded = false;
		eleID_chi2_hits_isLoaded = false;
		eleID_fbrem_isLoaded = false;
		eleID_ep_isLoaded = false;
		eleID_eelepout_isLoaded = false;
		eleID_IoEmIop_isLoaded = false;
		eleID_deltaetain_isLoaded = false;
		eleID_deltaphiin_isLoaded = false;
		eleID_deltaetaseed_isLoaded = false;
		eleID_pT_isLoaded = false;
		eleID_isbarrel_isLoaded = false;
		eleID_isendcap_isLoaded = false;
		eleID_scl_eta_isLoaded = false;
		muID_dzPV_isLoaded = false;
		muID_ptSig_isLoaded = false;
		muID_ip3dSig_isLoaded = false;
		muID_medMuonPOG_isLoaded = false;
		muID_pt_isLoaded = false;
		muID_eta_isLoaded = false;
		trueNumInt_isLoaded = false;
		nPUvertices_isLoaded = false;
		nGoodVertices_isLoaded = false;
		lep1_trigMatch_noIsoReq_isLoaded = false;
		lep1_trigMatch_isoReq_isLoaded = false;
		lep2_trigMatch_noIsoReq_isLoaded = false;
		lep2_trigMatch_isoReq_isLoaded = false;
		met3p0_isLoaded = false;
		jet_corr_pt_isLoaded = false;
		metphi3p0_isLoaded = false;
		passes_met_filters_isLoaded = false;
		mostJets_isLoaded = false;
		madeExtraZ_isLoaded = false;
		madeExtraG_isLoaded = false;
		lep1_isGoodLeg_isLoaded = false;
		lep2_isGoodLeg_isLoaded = false;
		lep1_isFakeLeg_isLoaded = false;
		lep2_isFakeLeg_isLoaded = false;
		lep1_multiIso_isLoaded = false;
		lep2_multiIso_isLoaded = false;
		lep1_sip_isLoaded = false;
		lep2_sip_isLoaded = false;
		lep1_closeJet_isLoaded = false;
		lep2_closeJet_isLoaded = false;
		passed_id_inSituFR_lep1_isLoaded = false;
		passed_id_inSituFR_lep2_isLoaded = false;
		fired_trigger_isLoaded = false;
		triggers_isLoaded = false;
	}

void SSAG::LoadAllBranches() 
	// load all branches
{
	if (met_branch != 0) met();
	if (metPhi_branch != 0) metPhi();
	if (corrMET_branch != 0) corrMET();
	if (corrMETphi_branch != 0) corrMETphi();
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
	if (genweights_branch != 0) genweights();
	if (genweightsID_branch != 0) genweightsID();
	if (gen_met_phi_branch != 0) gen_met_phi();
	if (njets_branch != 0) njets();
	if (njets_corr_branch != 0) njets_corr();
	if (hyp_class_branch != 0) hyp_class();
	if (lep1_p4_branch != 0) lep1_p4();
	if (lep2_p4_branch != 0) lep2_p4();
	if (ht_corr_branch != 0) ht_corr();
	if (ht_branch != 0) ht();
	if (lep1_motherID_branch != 0) lep1_motherID();
	if (lep2_motherID_branch != 0) lep2_motherID();
	if (lep1_mc_id_branch != 0) lep1_mc_id();
	if (lep2_mc_id_branch != 0) lep2_mc_id();
	if (lep1_id_branch != 0) lep1_id();
	if (lep2_id_branch != 0) lep2_id();
	if (lep1_coneCorrPt_branch != 0) lep1_coneCorrPt();
	if (lep2_coneCorrPt_branch != 0) lep2_coneCorrPt();
	if (lep1_idx_branch != 0) lep1_idx();
	if (lep2_idx_branch != 0) lep2_idx();
	if (jets_branch != 0) jets();
	if (jets_corr_branch != 0) jets_corr();
	if (btags_disc_branch != 0) btags_disc();
	if (jets_disc_branch != 0) jets_disc();
	if (jets_JEC_branch != 0) jets_JEC();
	if (btags_JEC_branch != 0) btags_JEC();
	if (jets_undoJEC_branch != 0) jets_undoJEC();
	if (btags_undoJEC_branch != 0) btags_undoJEC();
	if (btags_corr_disc_branch != 0) btags_corr_disc();
	if (jets_corr_disc_branch != 0) jets_corr_disc();
	if (jets_corr_JEC_branch != 0) jets_corr_JEC();
	if (btags_corr_JEC_branch != 0) btags_corr_JEC();
	if (jets_corr_undoJEC_branch != 0) jets_corr_undoJEC();
	if (btags_corr_undoJEC_branch != 0) btags_corr_undoJEC();
	if (btags_branch != 0) btags();
	if (nbtags_branch != 0) nbtags();
	if (btags_corr_branch != 0) btags_corr();
	if (nbtags_corr_branch != 0) nbtags_corr();
	if (sf_dilepTrig_hpt_branch != 0) sf_dilepTrig_hpt();
	if (sf_dilepTrig_lpt_branch != 0) sf_dilepTrig_lpt();
	if (sf_dilepTrig_vlpt_branch != 0) sf_dilepTrig_vlpt();
	if (hyp_type_branch != 0) hyp_type();
	if (sf_dilep_eff_branch != 0) sf_dilep_eff();
	if (mt_branch != 0) mt();
	if (mt_l2_branch != 0) mt_l2();
	if (mt2_branch != 0) mt2();
	if (mtmin_branch != 0) mtmin();
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
	if (genps_p4_branch != 0) genps_p4();
	if (genps_id_branch != 0) genps_id();
	if (genps_id_mother_branch != 0) genps_id_mother();
	if (genps_idx_mother_branch != 0) genps_idx_mother();
	if (genps_status_branch != 0) genps_status();
	if (genps_id_grandma_branch != 0) genps_id_grandma();
	if (lep1_passes_id_branch != 0) lep1_passes_id();
	if (lep2_passes_id_branch != 0) lep2_passes_id();
	if (lep3_passes_id_branch != 0) lep3_passes_id();
	if (lep3_tight_branch != 0) lep3_tight();
	if (lep3_veto_branch != 0) lep3_veto();
	if (lep3_fo_branch != 0) lep3_fo();
	if (lep1_dxyPV_branch != 0) lep1_dxyPV();
	if (lep2_dxyPV_branch != 0) lep2_dxyPV();
	if (lep1_dZ_branch != 0) lep1_dZ();
	if (lep2_dZ_branch != 0) lep2_dZ();
	if (lep1_d0_err_branch != 0) lep1_d0_err();
	if (lep2_d0_err_branch != 0) lep2_d0_err();
	if (lep1_ip3d_branch != 0) lep1_ip3d();
	if (lep2_ip3d_branch != 0) lep2_ip3d();
	if (lep1_MVA_branch != 0) lep1_MVA();
	if (lep2_MVA_branch != 0) lep2_MVA();
	if (lep1_ip3d_err_branch != 0) lep1_ip3d_err();
	if (lep2_ip3d_err_branch != 0) lep2_ip3d_err();
	if (nVetoElectrons7_branch != 0) nVetoElectrons7();
	if (nVetoElectrons10_branch != 0) nVetoElectrons10();
	if (nVetoElectrons25_branch != 0) nVetoElectrons25();
	if (nVetoMuons5_branch != 0) nVetoMuons5();
	if (nVetoMuons10_branch != 0) nVetoMuons10();
	if (nVetoMuons25_branch != 0) nVetoMuons25();
	if (filename_branch != 0) filename();
	if (lep1_ptrel_v0_branch != 0) lep1_ptrel_v0();
	if (lep1_ptrel_v1_branch != 0) lep1_ptrel_v1();
	if (lep2_ptrel_v0_branch != 0) lep2_ptrel_v0();
	if (lep2_ptrel_v1_branch != 0) lep2_ptrel_v1();
	if (lep1_miniIso_branch != 0) lep1_miniIso();
	if (lep2_miniIso_branch != 0) lep2_miniIso();
	if (jet_close_lep1_branch != 0) jet_close_lep1();
	if (jet_close_lep2_branch != 0) jet_close_lep2();
	if (eleID_kfhits_branch != 0) eleID_kfhits();
	if (eleID_oldsigmaietaieta_branch != 0) eleID_oldsigmaietaieta();
	if (eleID_oldsigmaiphiiphi_branch != 0) eleID_oldsigmaiphiiphi();
	if (eleID_oldcircularity_branch != 0) eleID_oldcircularity();
	if (eleID_oldr9_branch != 0) eleID_oldr9();
	if (eleID_scletawidth_branch != 0) eleID_scletawidth();
	if (eleID_sclphiwidth_branch != 0) eleID_sclphiwidth();
	if (eleID_he_branch != 0) eleID_he();
	if (eleID_psEoverEraw_branch != 0) eleID_psEoverEraw();
	if (eleID_kfchi2_branch != 0) eleID_kfchi2();
	if (eleID_chi2_hits_branch != 0) eleID_chi2_hits();
	if (eleID_fbrem_branch != 0) eleID_fbrem();
	if (eleID_ep_branch != 0) eleID_ep();
	if (eleID_eelepout_branch != 0) eleID_eelepout();
	if (eleID_IoEmIop_branch != 0) eleID_IoEmIop();
	if (eleID_deltaetain_branch != 0) eleID_deltaetain();
	if (eleID_deltaphiin_branch != 0) eleID_deltaphiin();
	if (eleID_deltaetaseed_branch != 0) eleID_deltaetaseed();
	if (eleID_pT_branch != 0) eleID_pT();
	if (eleID_isbarrel_branch != 0) eleID_isbarrel();
	if (eleID_isendcap_branch != 0) eleID_isendcap();
	if (eleID_scl_eta_branch != 0) eleID_scl_eta();
	if (muID_dzPV_branch != 0) muID_dzPV();
	if (muID_ptSig_branch != 0) muID_ptSig();
	if (muID_ip3dSig_branch != 0) muID_ip3dSig();
	if (muID_medMuonPOG_branch != 0) muID_medMuonPOG();
	if (muID_pt_branch != 0) muID_pt();
	if (muID_eta_branch != 0) muID_eta();
	if (trueNumInt_branch != 0) trueNumInt();
	if (nPUvertices_branch != 0) nPUvertices();
	if (nGoodVertices_branch != 0) nGoodVertices();
	if (lep1_trigMatch_noIsoReq_branch != 0) lep1_trigMatch_noIsoReq();
	if (lep1_trigMatch_isoReq_branch != 0) lep1_trigMatch_isoReq();
	if (lep2_trigMatch_noIsoReq_branch != 0) lep2_trigMatch_noIsoReq();
	if (lep2_trigMatch_isoReq_branch != 0) lep2_trigMatch_isoReq();
	if (met3p0_branch != 0) met3p0();
	if (jet_corr_pt_branch != 0) jet_corr_pt();
	if (metphi3p0_branch != 0) metphi3p0();
	if (passes_met_filters_branch != 0) passes_met_filters();
	if (mostJets_branch != 0) mostJets();
	if (madeExtraZ_branch != 0) madeExtraZ();
	if (madeExtraG_branch != 0) madeExtraG();
	if (lep1_isGoodLeg_branch != 0) lep1_isGoodLeg();
	if (lep2_isGoodLeg_branch != 0) lep2_isGoodLeg();
	if (lep1_isFakeLeg_branch != 0) lep1_isFakeLeg();
	if (lep2_isFakeLeg_branch != 0) lep2_isFakeLeg();
	if (lep1_multiIso_branch != 0) lep1_multiIso();
	if (lep2_multiIso_branch != 0) lep2_multiIso();
	if (lep1_sip_branch != 0) lep1_sip();
	if (lep2_sip_branch != 0) lep2_sip();
	if (lep1_closeJet_branch != 0) lep1_closeJet();
	if (lep2_closeJet_branch != 0) lep2_closeJet();
	if (passed_id_inSituFR_lep1_branch != 0) passed_id_inSituFR_lep1();
	if (passed_id_inSituFR_lep2_branch != 0) passed_id_inSituFR_lep2();
	if (fired_trigger_branch != 0) fired_trigger();
	if (triggers_branch != 0) triggers();
}

	const float &SSAG::met()
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
	const float &SSAG::metPhi()
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
	const float &SSAG::corrMET()
	{
		if (not corrMET_isLoaded) {
			if (corrMET_branch != 0) {
				corrMET_branch->GetEntry(index);
			} else { 
				printf("branch corrMET_branch does not exist!\n");
				exit(1);
			}
			corrMET_isLoaded = true;
		}
		return corrMET_;
	}
	const float &SSAG::corrMETphi()
	{
		if (not corrMETphi_isLoaded) {
			if (corrMETphi_branch != 0) {
				corrMETphi_branch->GetEntry(index);
			} else { 
				printf("branch corrMETphi_branch does not exist!\n");
				exit(1);
			}
			corrMETphi_isLoaded = true;
		}
		return corrMETphi_;
	}
	const int &SSAG::event()
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
	const int &SSAG::lumi()
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
	const int &SSAG::run()
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
	const bool &	SSAG::filt_csc()
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
	const bool &	SSAG::filt_hbhe()
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
	const bool &	SSAG::filt_hcallaser()
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
	const bool &	SSAG::filt_ecaltp()
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
	const bool &	SSAG::filt_trkfail()
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
	const bool &	SSAG::filt_eebadsc()
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
	const bool &	SSAG::is_real_data()
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
	const float &SSAG::scale1fb()
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
	const float &SSAG::xsec()
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
	const float &SSAG::kfactor()
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
	const float &SSAG::gen_met()
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
	const vector<float> &SSAG::genweights()
	{
		if (not genweights_isLoaded) {
			if (genweights_branch != 0) {
				genweights_branch->GetEntry(index);
			} else { 
				printf("branch genweights_branch does not exist!\n");
				exit(1);
			}
			genweights_isLoaded = true;
		}
		return *genweights_;
	}
	const vector<string> &SSAG::genweightsID()
	{
		if (not genweightsID_isLoaded) {
			if (genweightsID_branch != 0) {
				genweightsID_branch->GetEntry(index);
			} else { 
				printf("branch genweightsID_branch does not exist!\n");
				exit(1);
			}
			genweightsID_isLoaded = true;
		}
		return *genweightsID_;
	}
	const float &SSAG::gen_met_phi()
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
	const int &SSAG::njets()
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
	const int &SSAG::njets_corr()
	{
		if (not njets_corr_isLoaded) {
			if (njets_corr_branch != 0) {
				njets_corr_branch->GetEntry(index);
			} else { 
				printf("branch njets_corr_branch does not exist!\n");
				exit(1);
			}
			njets_corr_isLoaded = true;
		}
		return njets_corr_;
	}
	const int &SSAG::hyp_class()
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
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::lep1_p4()
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
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::lep2_p4()
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
	const float &SSAG::ht_corr()
	{
		if (not ht_corr_isLoaded) {
			if (ht_corr_branch != 0) {
				ht_corr_branch->GetEntry(index);
			} else { 
				printf("branch ht_corr_branch does not exist!\n");
				exit(1);
			}
			ht_corr_isLoaded = true;
		}
		return ht_corr_;
	}
	const float &SSAG::ht()
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
	const int &SSAG::lep1_motherID()
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
	const int &SSAG::lep2_motherID()
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
	const int &SSAG::lep1_mc_id()
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
	const int &SSAG::lep2_mc_id()
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
	const int &SSAG::lep1_id()
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
	const int &SSAG::lep2_id()
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
	const float &SSAG::lep1_coneCorrPt()
	{
		if (not lep1_coneCorrPt_isLoaded) {
			if (lep1_coneCorrPt_branch != 0) {
				lep1_coneCorrPt_branch->GetEntry(index);
			} else { 
				printf("branch lep1_coneCorrPt_branch does not exist!\n");
				exit(1);
			}
			lep1_coneCorrPt_isLoaded = true;
		}
		return lep1_coneCorrPt_;
	}
	const float &SSAG::lep2_coneCorrPt()
	{
		if (not lep2_coneCorrPt_isLoaded) {
			if (lep2_coneCorrPt_branch != 0) {
				lep2_coneCorrPt_branch->GetEntry(index);
			} else { 
				printf("branch lep2_coneCorrPt_branch does not exist!\n");
				exit(1);
			}
			lep2_coneCorrPt_isLoaded = true;
		}
		return lep2_coneCorrPt_;
	}
	const int &SSAG::lep1_idx()
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
	const int &SSAG::lep2_idx()
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
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &SSAG::jets()
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
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &SSAG::jets_corr()
	{
		if (not jets_corr_isLoaded) {
			if (jets_corr_branch != 0) {
				jets_corr_branch->GetEntry(index);
			} else { 
				printf("branch jets_corr_branch does not exist!\n");
				exit(1);
			}
			jets_corr_isLoaded = true;
		}
		return *jets_corr_;
	}
	const vector<float> &SSAG::btags_disc()
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
	const vector<float> &SSAG::jets_disc()
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
	const vector<float> &SSAG::jets_JEC()
	{
		if (not jets_JEC_isLoaded) {
			if (jets_JEC_branch != 0) {
				jets_JEC_branch->GetEntry(index);
			} else { 
				printf("branch jets_JEC_branch does not exist!\n");
				exit(1);
			}
			jets_JEC_isLoaded = true;
		}
		return *jets_JEC_;
	}
	const vector<float> &SSAG::btags_JEC()
	{
		if (not btags_JEC_isLoaded) {
			if (btags_JEC_branch != 0) {
				btags_JEC_branch->GetEntry(index);
			} else { 
				printf("branch btags_JEC_branch does not exist!\n");
				exit(1);
			}
			btags_JEC_isLoaded = true;
		}
		return *btags_JEC_;
	}
	const vector<float> &SSAG::jets_undoJEC()
	{
		if (not jets_undoJEC_isLoaded) {
			if (jets_undoJEC_branch != 0) {
				jets_undoJEC_branch->GetEntry(index);
			} else { 
				printf("branch jets_undoJEC_branch does not exist!\n");
				exit(1);
			}
			jets_undoJEC_isLoaded = true;
		}
		return *jets_undoJEC_;
	}
	const vector<float> &SSAG::btags_undoJEC()
	{
		if (not btags_undoJEC_isLoaded) {
			if (btags_undoJEC_branch != 0) {
				btags_undoJEC_branch->GetEntry(index);
			} else { 
				printf("branch btags_undoJEC_branch does not exist!\n");
				exit(1);
			}
			btags_undoJEC_isLoaded = true;
		}
		return *btags_undoJEC_;
	}
	const vector<float> &SSAG::btags_corr_disc()
	{
		if (not btags_corr_disc_isLoaded) {
			if (btags_corr_disc_branch != 0) {
				btags_corr_disc_branch->GetEntry(index);
			} else { 
				printf("branch btags_corr_disc_branch does not exist!\n");
				exit(1);
			}
			btags_corr_disc_isLoaded = true;
		}
		return *btags_corr_disc_;
	}
	const vector<float> &SSAG::jets_corr_disc()
	{
		if (not jets_corr_disc_isLoaded) {
			if (jets_corr_disc_branch != 0) {
				jets_corr_disc_branch->GetEntry(index);
			} else { 
				printf("branch jets_corr_disc_branch does not exist!\n");
				exit(1);
			}
			jets_corr_disc_isLoaded = true;
		}
		return *jets_corr_disc_;
	}
	const vector<float> &SSAG::jets_corr_JEC()
	{
		if (not jets_corr_JEC_isLoaded) {
			if (jets_corr_JEC_branch != 0) {
				jets_corr_JEC_branch->GetEntry(index);
			} else { 
				printf("branch jets_corr_JEC_branch does not exist!\n");
				exit(1);
			}
			jets_corr_JEC_isLoaded = true;
		}
		return *jets_corr_JEC_;
	}
	const vector<float> &SSAG::btags_corr_JEC()
	{
		if (not btags_corr_JEC_isLoaded) {
			if (btags_corr_JEC_branch != 0) {
				btags_corr_JEC_branch->GetEntry(index);
			} else { 
				printf("branch btags_corr_JEC_branch does not exist!\n");
				exit(1);
			}
			btags_corr_JEC_isLoaded = true;
		}
		return *btags_corr_JEC_;
	}
	const vector<float> &SSAG::jets_corr_undoJEC()
	{
		if (not jets_corr_undoJEC_isLoaded) {
			if (jets_corr_undoJEC_branch != 0) {
				jets_corr_undoJEC_branch->GetEntry(index);
			} else { 
				printf("branch jets_corr_undoJEC_branch does not exist!\n");
				exit(1);
			}
			jets_corr_undoJEC_isLoaded = true;
		}
		return *jets_corr_undoJEC_;
	}
	const vector<float> &SSAG::btags_corr_undoJEC()
	{
		if (not btags_corr_undoJEC_isLoaded) {
			if (btags_corr_undoJEC_branch != 0) {
				btags_corr_undoJEC_branch->GetEntry(index);
			} else { 
				printf("branch btags_corr_undoJEC_branch does not exist!\n");
				exit(1);
			}
			btags_corr_undoJEC_isLoaded = true;
		}
		return *btags_corr_undoJEC_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &SSAG::btags()
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
	const int &SSAG::nbtags()
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
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &SSAG::btags_corr()
	{
		if (not btags_corr_isLoaded) {
			if (btags_corr_branch != 0) {
				btags_corr_branch->GetEntry(index);
			} else { 
				printf("branch btags_corr_branch does not exist!\n");
				exit(1);
			}
			btags_corr_isLoaded = true;
		}
		return *btags_corr_;
	}
	const int &SSAG::nbtags_corr()
	{
		if (not nbtags_corr_isLoaded) {
			if (nbtags_corr_branch != 0) {
				nbtags_corr_branch->GetEntry(index);
			} else { 
				printf("branch nbtags_corr_branch does not exist!\n");
				exit(1);
			}
			nbtags_corr_isLoaded = true;
		}
		return nbtags_corr_;
	}
	const float &SSAG::sf_dilepTrig_hpt()
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
	const float &SSAG::sf_dilepTrig_lpt()
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
	const float &SSAG::sf_dilepTrig_vlpt()
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
	const int &SSAG::hyp_type()
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
	const float &SSAG::sf_dilep_eff()
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
	const float &SSAG::mt()
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
	const float &SSAG::mt_l2()
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
	const float &SSAG::mt2()
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
	const float &SSAG::mtmin()
	{
		if (not mtmin_isLoaded) {
			if (mtmin_branch != 0) {
				mtmin_branch->GetEntry(index);
			} else { 
				printf("branch mtmin_branch does not exist!\n");
				exit(1);
			}
			mtmin_isLoaded = true;
		}
		return mtmin_;
	}
	const float &SSAG::mGluino()
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
	const float &SSAG::mLSP()
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
	const float &SSAG::mSbottom()
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
	const float &SSAG::mChargino()
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
	const int &SSAG::lep1_id_gen()
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
	const int &SSAG::lep2_id_gen()
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
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::lep1_p4_gen()
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
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::lep2_p4_gen()
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
	const int &SSAG::lep3_id()
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
	const int &SSAG::lep3_idx()
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
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::lep3_p4()
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
	const int &SSAG::lep3_quality()
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
	const float &SSAG::lep1_iso()
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
	const float &SSAG::lep2_iso()
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
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::dilep_p4()
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
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &SSAG::genps_p4()
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
	const vector<int> &SSAG::genps_id()
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
	const vector<int> &SSAG::genps_id_mother()
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
	const vector<int> &SSAG::genps_idx_mother()
	{
		if (not genps_idx_mother_isLoaded) {
			if (genps_idx_mother_branch != 0) {
				genps_idx_mother_branch->GetEntry(index);
			} else { 
				printf("branch genps_idx_mother_branch does not exist!\n");
				exit(1);
			}
			genps_idx_mother_isLoaded = true;
		}
		return *genps_idx_mother_;
	}
	const vector<int> &SSAG::genps_status()
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
	const vector<int> &SSAG::genps_id_grandma()
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
	const bool &	SSAG::lep1_passes_id()
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
	const bool &	SSAG::lep2_passes_id()
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
	const bool &	SSAG::lep3_passes_id()
	{
		if (not lep3_passes_id_isLoaded) {
			if (lep3_passes_id_branch != 0) {
				lep3_passes_id_branch->GetEntry(index);
			} else { 
				printf("branch lep3_passes_id_branch does not exist!\n");
				exit(1);
			}
			lep3_passes_id_isLoaded = true;
		}
		return lep3_passes_id_;
	}
	const bool &	SSAG::lep3_tight()
	{
		if (not lep3_tight_isLoaded) {
			if (lep3_tight_branch != 0) {
				lep3_tight_branch->GetEntry(index);
			} else { 
				printf("branch lep3_tight_branch does not exist!\n");
				exit(1);
			}
			lep3_tight_isLoaded = true;
		}
		return lep3_tight_;
	}
	const bool &	SSAG::lep3_veto()
	{
		if (not lep3_veto_isLoaded) {
			if (lep3_veto_branch != 0) {
				lep3_veto_branch->GetEntry(index);
			} else { 
				printf("branch lep3_veto_branch does not exist!\n");
				exit(1);
			}
			lep3_veto_isLoaded = true;
		}
		return lep3_veto_;
	}
	const bool &	SSAG::lep3_fo()
	{
		if (not lep3_fo_isLoaded) {
			if (lep3_fo_branch != 0) {
				lep3_fo_branch->GetEntry(index);
			} else { 
				printf("branch lep3_fo_branch does not exist!\n");
				exit(1);
			}
			lep3_fo_isLoaded = true;
		}
		return lep3_fo_;
	}
	const float &SSAG::lep1_dxyPV()
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
	const float &SSAG::lep2_dxyPV()
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
	const float &SSAG::lep1_dZ()
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
	const float &SSAG::lep2_dZ()
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
	const float &SSAG::lep1_d0_err()
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
	const float &SSAG::lep2_d0_err()
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
	const float &SSAG::lep1_ip3d()
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
	const float &SSAG::lep2_ip3d()
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
	const float &SSAG::lep1_MVA()
	{
		if (not lep1_MVA_isLoaded) {
			if (lep1_MVA_branch != 0) {
				lep1_MVA_branch->GetEntry(index);
			} else { 
				printf("branch lep1_MVA_branch does not exist!\n");
				exit(1);
			}
			lep1_MVA_isLoaded = true;
		}
		return lep1_MVA_;
	}
	const float &SSAG::lep2_MVA()
	{
		if (not lep2_MVA_isLoaded) {
			if (lep2_MVA_branch != 0) {
				lep2_MVA_branch->GetEntry(index);
			} else { 
				printf("branch lep2_MVA_branch does not exist!\n");
				exit(1);
			}
			lep2_MVA_isLoaded = true;
		}
		return lep2_MVA_;
	}
	const float &SSAG::lep1_ip3d_err()
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
	const float &SSAG::lep2_ip3d_err()
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
	const int &SSAG::nVetoElectrons7()
	{
		if (not nVetoElectrons7_isLoaded) {
			if (nVetoElectrons7_branch != 0) {
				nVetoElectrons7_branch->GetEntry(index);
			} else { 
				printf("branch nVetoElectrons7_branch does not exist!\n");
				exit(1);
			}
			nVetoElectrons7_isLoaded = true;
		}
		return nVetoElectrons7_;
	}
	const int &SSAG::nVetoElectrons10()
	{
		if (not nVetoElectrons10_isLoaded) {
			if (nVetoElectrons10_branch != 0) {
				nVetoElectrons10_branch->GetEntry(index);
			} else { 
				printf("branch nVetoElectrons10_branch does not exist!\n");
				exit(1);
			}
			nVetoElectrons10_isLoaded = true;
		}
		return nVetoElectrons10_;
	}
	const int &SSAG::nVetoElectrons25()
	{
		if (not nVetoElectrons25_isLoaded) {
			if (nVetoElectrons25_branch != 0) {
				nVetoElectrons25_branch->GetEntry(index);
			} else { 
				printf("branch nVetoElectrons25_branch does not exist!\n");
				exit(1);
			}
			nVetoElectrons25_isLoaded = true;
		}
		return nVetoElectrons25_;
	}
	const int &SSAG::nVetoMuons5()
	{
		if (not nVetoMuons5_isLoaded) {
			if (nVetoMuons5_branch != 0) {
				nVetoMuons5_branch->GetEntry(index);
			} else { 
				printf("branch nVetoMuons5_branch does not exist!\n");
				exit(1);
			}
			nVetoMuons5_isLoaded = true;
		}
		return nVetoMuons5_;
	}
	const int &SSAG::nVetoMuons10()
	{
		if (not nVetoMuons10_isLoaded) {
			if (nVetoMuons10_branch != 0) {
				nVetoMuons10_branch->GetEntry(index);
			} else { 
				printf("branch nVetoMuons10_branch does not exist!\n");
				exit(1);
			}
			nVetoMuons10_isLoaded = true;
		}
		return nVetoMuons10_;
	}
	const int &SSAG::nVetoMuons25()
	{
		if (not nVetoMuons25_isLoaded) {
			if (nVetoMuons25_branch != 0) {
				nVetoMuons25_branch->GetEntry(index);
			} else { 
				printf("branch nVetoMuons25_branch does not exist!\n");
				exit(1);
			}
			nVetoMuons25_isLoaded = true;
		}
		return nVetoMuons25_;
	}
	const TString &SSAG::filename()
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
	const float &SSAG::lep1_ptrel_v0()
	{
		if (not lep1_ptrel_v0_isLoaded) {
			if (lep1_ptrel_v0_branch != 0) {
				lep1_ptrel_v0_branch->GetEntry(index);
			} else { 
				printf("branch lep1_ptrel_v0_branch does not exist!\n");
				exit(1);
			}
			lep1_ptrel_v0_isLoaded = true;
		}
		return lep1_ptrel_v0_;
	}
	const float &SSAG::lep1_ptrel_v1()
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
	const float &SSAG::lep2_ptrel_v0()
	{
		if (not lep2_ptrel_v0_isLoaded) {
			if (lep2_ptrel_v0_branch != 0) {
				lep2_ptrel_v0_branch->GetEntry(index);
			} else { 
				printf("branch lep2_ptrel_v0_branch does not exist!\n");
				exit(1);
			}
			lep2_ptrel_v0_isLoaded = true;
		}
		return lep2_ptrel_v0_;
	}
	const float &SSAG::lep2_ptrel_v1()
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
	const float &SSAG::lep1_miniIso()
	{
		if (not lep1_miniIso_isLoaded) {
			if (lep1_miniIso_branch != 0) {
				lep1_miniIso_branch->GetEntry(index);
			} else { 
				printf("branch lep1_miniIso_branch does not exist!\n");
				exit(1);
			}
			lep1_miniIso_isLoaded = true;
		}
		return lep1_miniIso_;
	}
	const float &SSAG::lep2_miniIso()
	{
		if (not lep2_miniIso_isLoaded) {
			if (lep2_miniIso_branch != 0) {
				lep2_miniIso_branch->GetEntry(index);
			} else { 
				printf("branch lep2_miniIso_branch does not exist!\n");
				exit(1);
			}
			lep2_miniIso_isLoaded = true;
		}
		return lep2_miniIso_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::jet_close_lep1()
	{
		if (not jet_close_lep1_isLoaded) {
			if (jet_close_lep1_branch != 0) {
				jet_close_lep1_branch->GetEntry(index);
			} else { 
				printf("branch jet_close_lep1_branch does not exist!\n");
				exit(1);
			}
			jet_close_lep1_isLoaded = true;
		}
		return *jet_close_lep1_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::jet_close_lep2()
	{
		if (not jet_close_lep2_isLoaded) {
			if (jet_close_lep2_branch != 0) {
				jet_close_lep2_branch->GetEntry(index);
			} else { 
				printf("branch jet_close_lep2_branch does not exist!\n");
				exit(1);
			}
			jet_close_lep2_isLoaded = true;
		}
		return *jet_close_lep2_;
	}
	const vector<int> &SSAG::eleID_kfhits()
	{
		if (not eleID_kfhits_isLoaded) {
			if (eleID_kfhits_branch != 0) {
				eleID_kfhits_branch->GetEntry(index);
			} else { 
				printf("branch eleID_kfhits_branch does not exist!\n");
				exit(1);
			}
			eleID_kfhits_isLoaded = true;
		}
		return *eleID_kfhits_;
	}
	const vector<float> &SSAG::eleID_oldsigmaietaieta()
	{
		if (not eleID_oldsigmaietaieta_isLoaded) {
			if (eleID_oldsigmaietaieta_branch != 0) {
				eleID_oldsigmaietaieta_branch->GetEntry(index);
			} else { 
				printf("branch eleID_oldsigmaietaieta_branch does not exist!\n");
				exit(1);
			}
			eleID_oldsigmaietaieta_isLoaded = true;
		}
		return *eleID_oldsigmaietaieta_;
	}
	const vector<float> &SSAG::eleID_oldsigmaiphiiphi()
	{
		if (not eleID_oldsigmaiphiiphi_isLoaded) {
			if (eleID_oldsigmaiphiiphi_branch != 0) {
				eleID_oldsigmaiphiiphi_branch->GetEntry(index);
			} else { 
				printf("branch eleID_oldsigmaiphiiphi_branch does not exist!\n");
				exit(1);
			}
			eleID_oldsigmaiphiiphi_isLoaded = true;
		}
		return *eleID_oldsigmaiphiiphi_;
	}
	const vector<float> &SSAG::eleID_oldcircularity()
	{
		if (not eleID_oldcircularity_isLoaded) {
			if (eleID_oldcircularity_branch != 0) {
				eleID_oldcircularity_branch->GetEntry(index);
			} else { 
				printf("branch eleID_oldcircularity_branch does not exist!\n");
				exit(1);
			}
			eleID_oldcircularity_isLoaded = true;
		}
		return *eleID_oldcircularity_;
	}
	const vector<float> &SSAG::eleID_oldr9()
	{
		if (not eleID_oldr9_isLoaded) {
			if (eleID_oldr9_branch != 0) {
				eleID_oldr9_branch->GetEntry(index);
			} else { 
				printf("branch eleID_oldr9_branch does not exist!\n");
				exit(1);
			}
			eleID_oldr9_isLoaded = true;
		}
		return *eleID_oldr9_;
	}
	const vector<float> &SSAG::eleID_scletawidth()
	{
		if (not eleID_scletawidth_isLoaded) {
			if (eleID_scletawidth_branch != 0) {
				eleID_scletawidth_branch->GetEntry(index);
			} else { 
				printf("branch eleID_scletawidth_branch does not exist!\n");
				exit(1);
			}
			eleID_scletawidth_isLoaded = true;
		}
		return *eleID_scletawidth_;
	}
	const vector<float> &SSAG::eleID_sclphiwidth()
	{
		if (not eleID_sclphiwidth_isLoaded) {
			if (eleID_sclphiwidth_branch != 0) {
				eleID_sclphiwidth_branch->GetEntry(index);
			} else { 
				printf("branch eleID_sclphiwidth_branch does not exist!\n");
				exit(1);
			}
			eleID_sclphiwidth_isLoaded = true;
		}
		return *eleID_sclphiwidth_;
	}
	const vector<float> &SSAG::eleID_he()
	{
		if (not eleID_he_isLoaded) {
			if (eleID_he_branch != 0) {
				eleID_he_branch->GetEntry(index);
			} else { 
				printf("branch eleID_he_branch does not exist!\n");
				exit(1);
			}
			eleID_he_isLoaded = true;
		}
		return *eleID_he_;
	}
	const vector<float> &SSAG::eleID_psEoverEraw()
	{
		if (not eleID_psEoverEraw_isLoaded) {
			if (eleID_psEoverEraw_branch != 0) {
				eleID_psEoverEraw_branch->GetEntry(index);
			} else { 
				printf("branch eleID_psEoverEraw_branch does not exist!\n");
				exit(1);
			}
			eleID_psEoverEraw_isLoaded = true;
		}
		return *eleID_psEoverEraw_;
	}
	const vector<float> &SSAG::eleID_kfchi2()
	{
		if (not eleID_kfchi2_isLoaded) {
			if (eleID_kfchi2_branch != 0) {
				eleID_kfchi2_branch->GetEntry(index);
			} else { 
				printf("branch eleID_kfchi2_branch does not exist!\n");
				exit(1);
			}
			eleID_kfchi2_isLoaded = true;
		}
		return *eleID_kfchi2_;
	}
	const vector<float> &SSAG::eleID_chi2_hits()
	{
		if (not eleID_chi2_hits_isLoaded) {
			if (eleID_chi2_hits_branch != 0) {
				eleID_chi2_hits_branch->GetEntry(index);
			} else { 
				printf("branch eleID_chi2_hits_branch does not exist!\n");
				exit(1);
			}
			eleID_chi2_hits_isLoaded = true;
		}
		return *eleID_chi2_hits_;
	}
	const vector<float> &SSAG::eleID_fbrem()
	{
		if (not eleID_fbrem_isLoaded) {
			if (eleID_fbrem_branch != 0) {
				eleID_fbrem_branch->GetEntry(index);
			} else { 
				printf("branch eleID_fbrem_branch does not exist!\n");
				exit(1);
			}
			eleID_fbrem_isLoaded = true;
		}
		return *eleID_fbrem_;
	}
	const vector<float> &SSAG::eleID_ep()
	{
		if (not eleID_ep_isLoaded) {
			if (eleID_ep_branch != 0) {
				eleID_ep_branch->GetEntry(index);
			} else { 
				printf("branch eleID_ep_branch does not exist!\n");
				exit(1);
			}
			eleID_ep_isLoaded = true;
		}
		return *eleID_ep_;
	}
	const vector<float> &SSAG::eleID_eelepout()
	{
		if (not eleID_eelepout_isLoaded) {
			if (eleID_eelepout_branch != 0) {
				eleID_eelepout_branch->GetEntry(index);
			} else { 
				printf("branch eleID_eelepout_branch does not exist!\n");
				exit(1);
			}
			eleID_eelepout_isLoaded = true;
		}
		return *eleID_eelepout_;
	}
	const vector<float> &SSAG::eleID_IoEmIop()
	{
		if (not eleID_IoEmIop_isLoaded) {
			if (eleID_IoEmIop_branch != 0) {
				eleID_IoEmIop_branch->GetEntry(index);
			} else { 
				printf("branch eleID_IoEmIop_branch does not exist!\n");
				exit(1);
			}
			eleID_IoEmIop_isLoaded = true;
		}
		return *eleID_IoEmIop_;
	}
	const vector<float> &SSAG::eleID_deltaetain()
	{
		if (not eleID_deltaetain_isLoaded) {
			if (eleID_deltaetain_branch != 0) {
				eleID_deltaetain_branch->GetEntry(index);
			} else { 
				printf("branch eleID_deltaetain_branch does not exist!\n");
				exit(1);
			}
			eleID_deltaetain_isLoaded = true;
		}
		return *eleID_deltaetain_;
	}
	const vector<float> &SSAG::eleID_deltaphiin()
	{
		if (not eleID_deltaphiin_isLoaded) {
			if (eleID_deltaphiin_branch != 0) {
				eleID_deltaphiin_branch->GetEntry(index);
			} else { 
				printf("branch eleID_deltaphiin_branch does not exist!\n");
				exit(1);
			}
			eleID_deltaphiin_isLoaded = true;
		}
		return *eleID_deltaphiin_;
	}
	const vector<float> &SSAG::eleID_deltaetaseed()
	{
		if (not eleID_deltaetaseed_isLoaded) {
			if (eleID_deltaetaseed_branch != 0) {
				eleID_deltaetaseed_branch->GetEntry(index);
			} else { 
				printf("branch eleID_deltaetaseed_branch does not exist!\n");
				exit(1);
			}
			eleID_deltaetaseed_isLoaded = true;
		}
		return *eleID_deltaetaseed_;
	}
	const vector<float> &SSAG::eleID_pT()
	{
		if (not eleID_pT_isLoaded) {
			if (eleID_pT_branch != 0) {
				eleID_pT_branch->GetEntry(index);
			} else { 
				printf("branch eleID_pT_branch does not exist!\n");
				exit(1);
			}
			eleID_pT_isLoaded = true;
		}
		return *eleID_pT_;
	}
	const vector<bool> &SSAG::eleID_isbarrel()
	{
		if (not eleID_isbarrel_isLoaded) {
			if (eleID_isbarrel_branch != 0) {
				eleID_isbarrel_branch->GetEntry(index);
			} else { 
				printf("branch eleID_isbarrel_branch does not exist!\n");
				exit(1);
			}
			eleID_isbarrel_isLoaded = true;
		}
		return *eleID_isbarrel_;
	}
	const vector<bool> &SSAG::eleID_isendcap()
	{
		if (not eleID_isendcap_isLoaded) {
			if (eleID_isendcap_branch != 0) {
				eleID_isendcap_branch->GetEntry(index);
			} else { 
				printf("branch eleID_isendcap_branch does not exist!\n");
				exit(1);
			}
			eleID_isendcap_isLoaded = true;
		}
		return *eleID_isendcap_;
	}
	const vector<float> &SSAG::eleID_scl_eta()
	{
		if (not eleID_scl_eta_isLoaded) {
			if (eleID_scl_eta_branch != 0) {
				eleID_scl_eta_branch->GetEntry(index);
			} else { 
				printf("branch eleID_scl_eta_branch does not exist!\n");
				exit(1);
			}
			eleID_scl_eta_isLoaded = true;
		}
		return *eleID_scl_eta_;
	}
	const vector<float> &SSAG::muID_dzPV()
	{
		if (not muID_dzPV_isLoaded) {
			if (muID_dzPV_branch != 0) {
				muID_dzPV_branch->GetEntry(index);
			} else { 
				printf("branch muID_dzPV_branch does not exist!\n");
				exit(1);
			}
			muID_dzPV_isLoaded = true;
		}
		return *muID_dzPV_;
	}
	const vector<float> &SSAG::muID_ptSig()
	{
		if (not muID_ptSig_isLoaded) {
			if (muID_ptSig_branch != 0) {
				muID_ptSig_branch->GetEntry(index);
			} else { 
				printf("branch muID_ptSig_branch does not exist!\n");
				exit(1);
			}
			muID_ptSig_isLoaded = true;
		}
		return *muID_ptSig_;
	}
	const vector<float> &SSAG::muID_ip3dSig()
	{
		if (not muID_ip3dSig_isLoaded) {
			if (muID_ip3dSig_branch != 0) {
				muID_ip3dSig_branch->GetEntry(index);
			} else { 
				printf("branch muID_ip3dSig_branch does not exist!\n");
				exit(1);
			}
			muID_ip3dSig_isLoaded = true;
		}
		return *muID_ip3dSig_;
	}
	const vector<float> &SSAG::muID_medMuonPOG()
	{
		if (not muID_medMuonPOG_isLoaded) {
			if (muID_medMuonPOG_branch != 0) {
				muID_medMuonPOG_branch->GetEntry(index);
			} else { 
				printf("branch muID_medMuonPOG_branch does not exist!\n");
				exit(1);
			}
			muID_medMuonPOG_isLoaded = true;
		}
		return *muID_medMuonPOG_;
	}
	const vector<float> &SSAG::muID_pt()
	{
		if (not muID_pt_isLoaded) {
			if (muID_pt_branch != 0) {
				muID_pt_branch->GetEntry(index);
			} else { 
				printf("branch muID_pt_branch does not exist!\n");
				exit(1);
			}
			muID_pt_isLoaded = true;
		}
		return *muID_pt_;
	}
	const vector<float> &SSAG::muID_eta()
	{
		if (not muID_eta_isLoaded) {
			if (muID_eta_branch != 0) {
				muID_eta_branch->GetEntry(index);
			} else { 
				printf("branch muID_eta_branch does not exist!\n");
				exit(1);
			}
			muID_eta_isLoaded = true;
		}
		return *muID_eta_;
	}
	const vector<float> &SSAG::trueNumInt()
	{
		if (not trueNumInt_isLoaded) {
			if (trueNumInt_branch != 0) {
				trueNumInt_branch->GetEntry(index);
			} else { 
				printf("branch trueNumInt_branch does not exist!\n");
				exit(1);
			}
			trueNumInt_isLoaded = true;
		}
		return *trueNumInt_;
	}
	const vector<int> &SSAG::nPUvertices()
	{
		if (not nPUvertices_isLoaded) {
			if (nPUvertices_branch != 0) {
				nPUvertices_branch->GetEntry(index);
			} else { 
				printf("branch nPUvertices_branch does not exist!\n");
				exit(1);
			}
			nPUvertices_isLoaded = true;
		}
		return *nPUvertices_;
	}
	const int &SSAG::nGoodVertices()
	{
		if (not nGoodVertices_isLoaded) {
			if (nGoodVertices_branch != 0) {
				nGoodVertices_branch->GetEntry(index);
			} else { 
				printf("branch nGoodVertices_branch does not exist!\n");
				exit(1);
			}
			nGoodVertices_isLoaded = true;
		}
		return nGoodVertices_;
	}
	const bool &	SSAG::lep1_trigMatch_noIsoReq()
	{
		if (not lep1_trigMatch_noIsoReq_isLoaded) {
			if (lep1_trigMatch_noIsoReq_branch != 0) {
				lep1_trigMatch_noIsoReq_branch->GetEntry(index);
			} else { 
				printf("branch lep1_trigMatch_noIsoReq_branch does not exist!\n");
				exit(1);
			}
			lep1_trigMatch_noIsoReq_isLoaded = true;
		}
		return lep1_trigMatch_noIsoReq_;
	}
	const bool &	SSAG::lep1_trigMatch_isoReq()
	{
		if (not lep1_trigMatch_isoReq_isLoaded) {
			if (lep1_trigMatch_isoReq_branch != 0) {
				lep1_trigMatch_isoReq_branch->GetEntry(index);
			} else { 
				printf("branch lep1_trigMatch_isoReq_branch does not exist!\n");
				exit(1);
			}
			lep1_trigMatch_isoReq_isLoaded = true;
		}
		return lep1_trigMatch_isoReq_;
	}
	const bool &	SSAG::lep2_trigMatch_noIsoReq()
	{
		if (not lep2_trigMatch_noIsoReq_isLoaded) {
			if (lep2_trigMatch_noIsoReq_branch != 0) {
				lep2_trigMatch_noIsoReq_branch->GetEntry(index);
			} else { 
				printf("branch lep2_trigMatch_noIsoReq_branch does not exist!\n");
				exit(1);
			}
			lep2_trigMatch_noIsoReq_isLoaded = true;
		}
		return lep2_trigMatch_noIsoReq_;
	}
	const bool &	SSAG::lep2_trigMatch_isoReq()
	{
		if (not lep2_trigMatch_isoReq_isLoaded) {
			if (lep2_trigMatch_isoReq_branch != 0) {
				lep2_trigMatch_isoReq_branch->GetEntry(index);
			} else { 
				printf("branch lep2_trigMatch_isoReq_branch does not exist!\n");
				exit(1);
			}
			lep2_trigMatch_isoReq_isLoaded = true;
		}
		return lep2_trigMatch_isoReq_;
	}
	const float &SSAG::met3p0()
	{
		if (not met3p0_isLoaded) {
			if (met3p0_branch != 0) {
				met3p0_branch->GetEntry(index);
			} else { 
				printf("branch met3p0_branch does not exist!\n");
				exit(1);
			}
			met3p0_isLoaded = true;
		}
		return met3p0_;
	}
	const vector<float> &SSAG::jet_corr_pt()
	{
		if (not jet_corr_pt_isLoaded) {
			if (jet_corr_pt_branch != 0) {
				jet_corr_pt_branch->GetEntry(index);
			} else { 
				printf("branch jet_corr_pt_branch does not exist!\n");
				exit(1);
			}
			jet_corr_pt_isLoaded = true;
		}
		return *jet_corr_pt_;
	}
	const float &SSAG::metphi3p0()
	{
		if (not metphi3p0_isLoaded) {
			if (metphi3p0_branch != 0) {
				metphi3p0_branch->GetEntry(index);
			} else { 
				printf("branch metphi3p0_branch does not exist!\n");
				exit(1);
			}
			metphi3p0_isLoaded = true;
		}
		return metphi3p0_;
	}
	const bool &	SSAG::passes_met_filters()
	{
		if (not passes_met_filters_isLoaded) {
			if (passes_met_filters_branch != 0) {
				passes_met_filters_branch->GetEntry(index);
			} else { 
				printf("branch passes_met_filters_branch does not exist!\n");
				exit(1);
			}
			passes_met_filters_isLoaded = true;
		}
		return passes_met_filters_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &SSAG::mostJets()
	{
		if (not mostJets_isLoaded) {
			if (mostJets_branch != 0) {
				mostJets_branch->GetEntry(index);
			} else { 
				printf("branch mostJets_branch does not exist!\n");
				exit(1);
			}
			mostJets_isLoaded = true;
		}
		return *mostJets_;
	}
	const bool &	SSAG::madeExtraZ()
	{
		if (not madeExtraZ_isLoaded) {
			if (madeExtraZ_branch != 0) {
				madeExtraZ_branch->GetEntry(index);
			} else { 
				printf("branch madeExtraZ_branch does not exist!\n");
				exit(1);
			}
			madeExtraZ_isLoaded = true;
		}
		return madeExtraZ_;
	}
	const bool &	SSAG::madeExtraG()
	{
		if (not madeExtraG_isLoaded) {
			if (madeExtraG_branch != 0) {
				madeExtraG_branch->GetEntry(index);
			} else { 
				printf("branch madeExtraG_branch does not exist!\n");
				exit(1);
			}
			madeExtraG_isLoaded = true;
		}
		return madeExtraG_;
	}
	const bool &	SSAG::lep1_isGoodLeg()
	{
		if (not lep1_isGoodLeg_isLoaded) {
			if (lep1_isGoodLeg_branch != 0) {
				lep1_isGoodLeg_branch->GetEntry(index);
			} else { 
				printf("branch lep1_isGoodLeg_branch does not exist!\n");
				exit(1);
			}
			lep1_isGoodLeg_isLoaded = true;
		}
		return lep1_isGoodLeg_;
	}
	const bool &	SSAG::lep2_isGoodLeg()
	{
		if (not lep2_isGoodLeg_isLoaded) {
			if (lep2_isGoodLeg_branch != 0) {
				lep2_isGoodLeg_branch->GetEntry(index);
			} else { 
				printf("branch lep2_isGoodLeg_branch does not exist!\n");
				exit(1);
			}
			lep2_isGoodLeg_isLoaded = true;
		}
		return lep2_isGoodLeg_;
	}
	const bool &	SSAG::lep1_isFakeLeg()
	{
		if (not lep1_isFakeLeg_isLoaded) {
			if (lep1_isFakeLeg_branch != 0) {
				lep1_isFakeLeg_branch->GetEntry(index);
			} else { 
				printf("branch lep1_isFakeLeg_branch does not exist!\n");
				exit(1);
			}
			lep1_isFakeLeg_isLoaded = true;
		}
		return lep1_isFakeLeg_;
	}
	const bool &	SSAG::lep2_isFakeLeg()
	{
		if (not lep2_isFakeLeg_isLoaded) {
			if (lep2_isFakeLeg_branch != 0) {
				lep2_isFakeLeg_branch->GetEntry(index);
			} else { 
				printf("branch lep2_isFakeLeg_branch does not exist!\n");
				exit(1);
			}
			lep2_isFakeLeg_isLoaded = true;
		}
		return lep2_isFakeLeg_;
	}
	const bool &	SSAG::lep1_multiIso()
	{
		if (not lep1_multiIso_isLoaded) {
			if (lep1_multiIso_branch != 0) {
				lep1_multiIso_branch->GetEntry(index);
			} else { 
				printf("branch lep1_multiIso_branch does not exist!\n");
				exit(1);
			}
			lep1_multiIso_isLoaded = true;
		}
		return lep1_multiIso_;
	}
	const bool &	SSAG::lep2_multiIso()
	{
		if (not lep2_multiIso_isLoaded) {
			if (lep2_multiIso_branch != 0) {
				lep2_multiIso_branch->GetEntry(index);
			} else { 
				printf("branch lep2_multiIso_branch does not exist!\n");
				exit(1);
			}
			lep2_multiIso_isLoaded = true;
		}
		return lep2_multiIso_;
	}
	const float &SSAG::lep1_sip()
	{
		if (not lep1_sip_isLoaded) {
			if (lep1_sip_branch != 0) {
				lep1_sip_branch->GetEntry(index);
			} else { 
				printf("branch lep1_sip_branch does not exist!\n");
				exit(1);
			}
			lep1_sip_isLoaded = true;
		}
		return lep1_sip_;
	}
	const float &SSAG::lep2_sip()
	{
		if (not lep2_sip_isLoaded) {
			if (lep2_sip_branch != 0) {
				lep2_sip_branch->GetEntry(index);
			} else { 
				printf("branch lep2_sip_branch does not exist!\n");
				exit(1);
			}
			lep2_sip_isLoaded = true;
		}
		return lep2_sip_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::lep1_closeJet()
	{
		if (not lep1_closeJet_isLoaded) {
			if (lep1_closeJet_branch != 0) {
				lep1_closeJet_branch->GetEntry(index);
			} else { 
				printf("branch lep1_closeJet_branch does not exist!\n");
				exit(1);
			}
			lep1_closeJet_isLoaded = true;
		}
		return *lep1_closeJet_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &SSAG::lep2_closeJet()
	{
		if (not lep2_closeJet_isLoaded) {
			if (lep2_closeJet_branch != 0) {
				lep2_closeJet_branch->GetEntry(index);
			} else { 
				printf("branch lep2_closeJet_branch does not exist!\n");
				exit(1);
			}
			lep2_closeJet_isLoaded = true;
		}
		return *lep2_closeJet_;
	}
	const bool &	SSAG::passed_id_inSituFR_lep1()
	{
		if (not passed_id_inSituFR_lep1_isLoaded) {
			if (passed_id_inSituFR_lep1_branch != 0) {
				passed_id_inSituFR_lep1_branch->GetEntry(index);
			} else { 
				printf("branch passed_id_inSituFR_lep1_branch does not exist!\n");
				exit(1);
			}
			passed_id_inSituFR_lep1_isLoaded = true;
		}
		return passed_id_inSituFR_lep1_;
	}
	const bool &	SSAG::passed_id_inSituFR_lep2()
	{
		if (not passed_id_inSituFR_lep2_isLoaded) {
			if (passed_id_inSituFR_lep2_branch != 0) {
				passed_id_inSituFR_lep2_branch->GetEntry(index);
			} else { 
				printf("branch passed_id_inSituFR_lep2_branch does not exist!\n");
				exit(1);
			}
			passed_id_inSituFR_lep2_isLoaded = true;
		}
		return passed_id_inSituFR_lep2_;
	}
	const bool &	SSAG::fired_trigger()
	{
		if (not fired_trigger_isLoaded) {
			if (fired_trigger_branch != 0) {
				fired_trigger_branch->GetEntry(index);
			} else { 
				printf("branch fired_trigger_branch does not exist!\n");
				exit(1);
			}
			fired_trigger_isLoaded = true;
		}
		return fired_trigger_;
	}
	const unsigned int &SSAG::triggers()
	{
		if (not triggers_isLoaded) {
			if (triggers_branch != 0) {
				triggers_branch->GetEntry(index);
			} else { 
				printf("branch triggers_branch does not exist!\n");
				exit(1);
			}
			triggers_isLoaded = true;
		}
		return triggers_;
	}

  void SSAG::progress( int nEventsTotal, int nEventsChain ){
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
  
namespace ss {
	const float &met() { return samesign.met(); }
	const float &metPhi() { return samesign.metPhi(); }
	const float &corrMET() { return samesign.corrMET(); }
	const float &corrMETphi() { return samesign.corrMETphi(); }
	const int &event() { return samesign.event(); }
	const int &lumi() { return samesign.lumi(); }
	const int &run() { return samesign.run(); }
	const bool &filt_csc() { return samesign.filt_csc(); }
	const bool &filt_hbhe() { return samesign.filt_hbhe(); }
	const bool &filt_hcallaser() { return samesign.filt_hcallaser(); }
	const bool &filt_ecaltp() { return samesign.filt_ecaltp(); }
	const bool &filt_trkfail() { return samesign.filt_trkfail(); }
	const bool &filt_eebadsc() { return samesign.filt_eebadsc(); }
	const bool &is_real_data() { return samesign.is_real_data(); }
	const float &scale1fb() { return samesign.scale1fb(); }
	const float &xsec() { return samesign.xsec(); }
	const float &kfactor() { return samesign.kfactor(); }
	const float &gen_met() { return samesign.gen_met(); }
	const vector<float> &genweights() { return samesign.genweights(); }
	const vector<string> &genweightsID() { return samesign.genweightsID(); }
	const float &gen_met_phi() { return samesign.gen_met_phi(); }
	const int &njets() { return samesign.njets(); }
	const int &njets_corr() { return samesign.njets_corr(); }
	const int &hyp_class() { return samesign.hyp_class(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4() { return samesign.lep1_p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4() { return samesign.lep2_p4(); }
	const float &ht_corr() { return samesign.ht_corr(); }
	const float &ht() { return samesign.ht(); }
	const int &lep1_motherID() { return samesign.lep1_motherID(); }
	const int &lep2_motherID() { return samesign.lep2_motherID(); }
	const int &lep1_mc_id() { return samesign.lep1_mc_id(); }
	const int &lep2_mc_id() { return samesign.lep2_mc_id(); }
	const int &lep1_id() { return samesign.lep1_id(); }
	const int &lep2_id() { return samesign.lep2_id(); }
	const float &lep1_coneCorrPt() { return samesign.lep1_coneCorrPt(); }
	const float &lep2_coneCorrPt() { return samesign.lep2_coneCorrPt(); }
	const int &lep1_idx() { return samesign.lep1_idx(); }
	const int &lep2_idx() { return samesign.lep2_idx(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets() { return samesign.jets(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_corr() { return samesign.jets_corr(); }
	const vector<float> &btags_disc() { return samesign.btags_disc(); }
	const vector<float> &jets_disc() { return samesign.jets_disc(); }
	const vector<float> &jets_JEC() { return samesign.jets_JEC(); }
	const vector<float> &btags_JEC() { return samesign.btags_JEC(); }
	const vector<float> &jets_undoJEC() { return samesign.jets_undoJEC(); }
	const vector<float> &btags_undoJEC() { return samesign.btags_undoJEC(); }
	const vector<float> &btags_corr_disc() { return samesign.btags_corr_disc(); }
	const vector<float> &jets_corr_disc() { return samesign.jets_corr_disc(); }
	const vector<float> &jets_corr_JEC() { return samesign.jets_corr_JEC(); }
	const vector<float> &btags_corr_JEC() { return samesign.btags_corr_JEC(); }
	const vector<float> &jets_corr_undoJEC() { return samesign.jets_corr_undoJEC(); }
	const vector<float> &btags_corr_undoJEC() { return samesign.btags_corr_undoJEC(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags() { return samesign.btags(); }
	const int &nbtags() { return samesign.nbtags(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_corr() { return samesign.btags_corr(); }
	const int &nbtags_corr() { return samesign.nbtags_corr(); }
	const float &sf_dilepTrig_hpt() { return samesign.sf_dilepTrig_hpt(); }
	const float &sf_dilepTrig_lpt() { return samesign.sf_dilepTrig_lpt(); }
	const float &sf_dilepTrig_vlpt() { return samesign.sf_dilepTrig_vlpt(); }
	const int &hyp_type() { return samesign.hyp_type(); }
	const float &sf_dilep_eff() { return samesign.sf_dilep_eff(); }
	const float &mt() { return samesign.mt(); }
	const float &mt_l2() { return samesign.mt_l2(); }
	const float &mt2() { return samesign.mt2(); }
	const float &mtmin() { return samesign.mtmin(); }
	const float &mGluino() { return samesign.mGluino(); }
	const float &mLSP() { return samesign.mLSP(); }
	const float &mSbottom() { return samesign.mSbottom(); }
	const float &mChargino() { return samesign.mChargino(); }
	const int &lep1_id_gen() { return samesign.lep1_id_gen(); }
	const int &lep2_id_gen() { return samesign.lep2_id_gen(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4_gen() { return samesign.lep1_p4_gen(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4_gen() { return samesign.lep2_p4_gen(); }
	const int &lep3_id() { return samesign.lep3_id(); }
	const int &lep3_idx() { return samesign.lep3_idx(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep3_p4() { return samesign.lep3_p4(); }
	const int &lep3_quality() { return samesign.lep3_quality(); }
	const float &lep1_iso() { return samesign.lep1_iso(); }
	const float &lep2_iso() { return samesign.lep2_iso(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4() { return samesign.dilep_p4(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps_p4() { return samesign.genps_p4(); }
	const vector<int> &genps_id() { return samesign.genps_id(); }
	const vector<int> &genps_id_mother() { return samesign.genps_id_mother(); }
	const vector<int> &genps_idx_mother() { return samesign.genps_idx_mother(); }
	const vector<int> &genps_status() { return samesign.genps_status(); }
	const vector<int> &genps_id_grandma() { return samesign.genps_id_grandma(); }
	const bool &lep1_passes_id() { return samesign.lep1_passes_id(); }
	const bool &lep2_passes_id() { return samesign.lep2_passes_id(); }
	const bool &lep3_passes_id() { return samesign.lep3_passes_id(); }
	const bool &lep3_tight() { return samesign.lep3_tight(); }
	const bool &lep3_veto() { return samesign.lep3_veto(); }
	const bool &lep3_fo() { return samesign.lep3_fo(); }
	const float &lep1_dxyPV() { return samesign.lep1_dxyPV(); }
	const float &lep2_dxyPV() { return samesign.lep2_dxyPV(); }
	const float &lep1_dZ() { return samesign.lep1_dZ(); }
	const float &lep2_dZ() { return samesign.lep2_dZ(); }
	const float &lep1_d0_err() { return samesign.lep1_d0_err(); }
	const float &lep2_d0_err() { return samesign.lep2_d0_err(); }
	const float &lep1_ip3d() { return samesign.lep1_ip3d(); }
	const float &lep2_ip3d() { return samesign.lep2_ip3d(); }
	const float &lep1_MVA() { return samesign.lep1_MVA(); }
	const float &lep2_MVA() { return samesign.lep2_MVA(); }
	const float &lep1_ip3d_err() { return samesign.lep1_ip3d_err(); }
	const float &lep2_ip3d_err() { return samesign.lep2_ip3d_err(); }
	const int &nVetoElectrons7() { return samesign.nVetoElectrons7(); }
	const int &nVetoElectrons10() { return samesign.nVetoElectrons10(); }
	const int &nVetoElectrons25() { return samesign.nVetoElectrons25(); }
	const int &nVetoMuons5() { return samesign.nVetoMuons5(); }
	const int &nVetoMuons10() { return samesign.nVetoMuons10(); }
	const int &nVetoMuons25() { return samesign.nVetoMuons25(); }
	const TString &filename() { return samesign.filename(); }
	const float &lep1_ptrel_v0() { return samesign.lep1_ptrel_v0(); }
	const float &lep1_ptrel_v1() { return samesign.lep1_ptrel_v1(); }
	const float &lep2_ptrel_v0() { return samesign.lep2_ptrel_v0(); }
	const float &lep2_ptrel_v1() { return samesign.lep2_ptrel_v1(); }
	const float &lep1_miniIso() { return samesign.lep1_miniIso(); }
	const float &lep2_miniIso() { return samesign.lep2_miniIso(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &jet_close_lep1() { return samesign.jet_close_lep1(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &jet_close_lep2() { return samesign.jet_close_lep2(); }
	const vector<int> &eleID_kfhits() { return samesign.eleID_kfhits(); }
	const vector<float> &eleID_oldsigmaietaieta() { return samesign.eleID_oldsigmaietaieta(); }
	const vector<float> &eleID_oldsigmaiphiiphi() { return samesign.eleID_oldsigmaiphiiphi(); }
	const vector<float> &eleID_oldcircularity() { return samesign.eleID_oldcircularity(); }
	const vector<float> &eleID_oldr9() { return samesign.eleID_oldr9(); }
	const vector<float> &eleID_scletawidth() { return samesign.eleID_scletawidth(); }
	const vector<float> &eleID_sclphiwidth() { return samesign.eleID_sclphiwidth(); }
	const vector<float> &eleID_he() { return samesign.eleID_he(); }
	const vector<float> &eleID_psEoverEraw() { return samesign.eleID_psEoverEraw(); }
	const vector<float> &eleID_kfchi2() { return samesign.eleID_kfchi2(); }
	const vector<float> &eleID_chi2_hits() { return samesign.eleID_chi2_hits(); }
	const vector<float> &eleID_fbrem() { return samesign.eleID_fbrem(); }
	const vector<float> &eleID_ep() { return samesign.eleID_ep(); }
	const vector<float> &eleID_eelepout() { return samesign.eleID_eelepout(); }
	const vector<float> &eleID_IoEmIop() { return samesign.eleID_IoEmIop(); }
	const vector<float> &eleID_deltaetain() { return samesign.eleID_deltaetain(); }
	const vector<float> &eleID_deltaphiin() { return samesign.eleID_deltaphiin(); }
	const vector<float> &eleID_deltaetaseed() { return samesign.eleID_deltaetaseed(); }
	const vector<float> &eleID_pT() { return samesign.eleID_pT(); }
	const vector<bool> &eleID_isbarrel() { return samesign.eleID_isbarrel(); }
	const vector<bool> &eleID_isendcap() { return samesign.eleID_isendcap(); }
	const vector<float> &eleID_scl_eta() { return samesign.eleID_scl_eta(); }
	const vector<float> &muID_dzPV() { return samesign.muID_dzPV(); }
	const vector<float> &muID_ptSig() { return samesign.muID_ptSig(); }
	const vector<float> &muID_ip3dSig() { return samesign.muID_ip3dSig(); }
	const vector<float> &muID_medMuonPOG() { return samesign.muID_medMuonPOG(); }
	const vector<float> &muID_pt() { return samesign.muID_pt(); }
	const vector<float> &muID_eta() { return samesign.muID_eta(); }
	const vector<float> &trueNumInt() { return samesign.trueNumInt(); }
	const vector<int> &nPUvertices() { return samesign.nPUvertices(); }
	const int &nGoodVertices() { return samesign.nGoodVertices(); }
	const bool &lep1_trigMatch_noIsoReq() { return samesign.lep1_trigMatch_noIsoReq(); }
	const bool &lep1_trigMatch_isoReq() { return samesign.lep1_trigMatch_isoReq(); }
	const bool &lep2_trigMatch_noIsoReq() { return samesign.lep2_trigMatch_noIsoReq(); }
	const bool &lep2_trigMatch_isoReq() { return samesign.lep2_trigMatch_isoReq(); }
	const float &met3p0() { return samesign.met3p0(); }
	const vector<float> &jet_corr_pt() { return samesign.jet_corr_pt(); }
	const float &metphi3p0() { return samesign.metphi3p0(); }
	const bool &passes_met_filters() { return samesign.passes_met_filters(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &mostJets() { return samesign.mostJets(); }
	const bool &madeExtraZ() { return samesign.madeExtraZ(); }
	const bool &madeExtraG() { return samesign.madeExtraG(); }
	const bool &lep1_isGoodLeg() { return samesign.lep1_isGoodLeg(); }
	const bool &lep2_isGoodLeg() { return samesign.lep2_isGoodLeg(); }
	const bool &lep1_isFakeLeg() { return samesign.lep1_isFakeLeg(); }
	const bool &lep2_isFakeLeg() { return samesign.lep2_isFakeLeg(); }
	const bool &lep1_multiIso() { return samesign.lep1_multiIso(); }
	const bool &lep2_multiIso() { return samesign.lep2_multiIso(); }
	const float &lep1_sip() { return samesign.lep1_sip(); }
	const float &lep2_sip() { return samesign.lep2_sip(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_closeJet() { return samesign.lep1_closeJet(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_closeJet() { return samesign.lep2_closeJet(); }
	const bool &passed_id_inSituFR_lep1() { return samesign.passed_id_inSituFR_lep1(); }
	const bool &passed_id_inSituFR_lep2() { return samesign.passed_id_inSituFR_lep2(); }
	const bool &fired_trigger() { return samesign.fired_trigger(); }
	const unsigned int &triggers() { return samesign.triggers(); }
}
