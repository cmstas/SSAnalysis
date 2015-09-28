// -*- C++ -*-
#ifndef SS_H
#define SS_H
#include "Math/LorentzVector.h"
#include "Math/Point3D.h"
#include "TMath.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBits.h"
#include <vector> 
#include <unistd.h> 
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std; 
class SS {
private: 
protected: 
	unsigned int index;
	float	met_;
	TBranch *met_branch;
	bool met_isLoaded;
	float	metPhi_;
	TBranch *metPhi_branch;
	bool metPhi_isLoaded;
	float	corrMET_;
	TBranch *corrMET_branch;
	bool corrMET_isLoaded;
	float	corrMETphi_;
	TBranch *corrMETphi_branch;
	bool corrMETphi_isLoaded;
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
	int	njets_corr_;
	TBranch *njets_corr_branch;
	bool njets_corr_isLoaded;
	int	hyp_class_;
	TBranch *hyp_class_branch;
	bool hyp_class_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep1_p4_;
	TBranch *lep1_p4_branch;
	bool lep1_p4_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep2_p4_;
	TBranch *lep2_p4_branch;
	bool lep2_p4_isLoaded;
	float	ht_corr_;
	TBranch *ht_corr_branch;
	bool ht_corr_isLoaded;
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
	float	lep1_coneCorrPt_;
	TBranch *lep1_coneCorrPt_branch;
	bool lep1_coneCorrPt_isLoaded;
	float	lep2_coneCorrPt_;
	TBranch *lep2_coneCorrPt_branch;
	bool lep2_coneCorrPt_isLoaded;
	int	lep1_idx_;
	TBranch *lep1_idx_branch;
	bool lep1_idx_isLoaded;
	int	lep2_idx_;
	TBranch *lep2_idx_branch;
	bool lep2_idx_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_;
	TBranch *jets_branch;
	bool jets_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_corr_;
	TBranch *jets_corr_branch;
	bool jets_corr_isLoaded;
	vector<float> *btags_disc_;
	TBranch *btags_disc_branch;
	bool btags_disc_isLoaded;
	vector<float> *jets_disc_;
	TBranch *jets_disc_branch;
	bool jets_disc_isLoaded;
	vector<float> *jets_JEC_;
	TBranch *jets_JEC_branch;
	bool jets_JEC_isLoaded;
	vector<float> *btags_JEC_;
	TBranch *btags_JEC_branch;
	bool btags_JEC_isLoaded;
	vector<float> *jets_undoJEC_;
	TBranch *jets_undoJEC_branch;
	bool jets_undoJEC_isLoaded;
	vector<float> *btags_undoJEC_;
	TBranch *btags_undoJEC_branch;
	bool btags_undoJEC_isLoaded;
	vector<float> *btags_corr_disc_;
	TBranch *btags_corr_disc_branch;
	bool btags_corr_disc_isLoaded;
	vector<float> *jets_corr_disc_;
	TBranch *jets_corr_disc_branch;
	bool jets_corr_disc_isLoaded;
	vector<float> *jets_corr_JEC_;
	TBranch *jets_corr_JEC_branch;
	bool jets_corr_JEC_isLoaded;
	vector<float> *btags_corr_JEC_;
	TBranch *btags_corr_JEC_branch;
	bool btags_corr_JEC_isLoaded;
	vector<float> *jets_corr_undoJEC_;
	TBranch *jets_corr_undoJEC_branch;
	bool jets_corr_undoJEC_isLoaded;
	vector<float> *btags_corr_undoJEC_;
	TBranch *btags_corr_undoJEC_branch;
	bool btags_corr_undoJEC_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *btags_;
	TBranch *btags_branch;
	bool btags_isLoaded;
	int	nbtags_;
	TBranch *nbtags_branch;
	bool nbtags_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *btags_corr_;
	TBranch *btags_corr_branch;
	bool btags_corr_isLoaded;
	int	nbtags_corr_;
	TBranch *nbtags_corr_branch;
	bool nbtags_corr_isLoaded;
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
	float	mtmin_;
	TBranch *mtmin_branch;
	bool mtmin_isLoaded;
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
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *genps_p4_;
	TBranch *genps_p4_branch;
	bool genps_p4_isLoaded;
	vector<int> *genps_id_;
	TBranch *genps_id_branch;
	bool genps_id_isLoaded;
	vector<int> *genps_id_mother_;
	TBranch *genps_id_mother_branch;
	bool genps_id_mother_isLoaded;
	vector<int> *genps_idx_mother_;
	TBranch *genps_idx_mother_branch;
	bool genps_idx_mother_isLoaded;
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
	bool	lep3_passes_id_;
	TBranch *lep3_passes_id_branch;
	bool lep3_passes_id_isLoaded;
	bool	lep3_tight_;
	TBranch *lep3_tight_branch;
	bool lep3_tight_isLoaded;
	bool	lep3_veto_;
	TBranch *lep3_veto_branch;
	bool lep3_veto_isLoaded;
	bool	lep3_fo_;
	TBranch *lep3_fo_branch;
	bool lep3_fo_isLoaded;
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
	float	lep1_MVA_;
	TBranch *lep1_MVA_branch;
	bool lep1_MVA_isLoaded;
	float	lep2_MVA_;
	TBranch *lep2_MVA_branch;
	bool lep2_MVA_isLoaded;
	float	lep1_ip3d_err_;
	TBranch *lep1_ip3d_err_branch;
	bool lep1_ip3d_err_isLoaded;
	float	lep2_ip3d_err_;
	TBranch *lep2_ip3d_err_branch;
	bool lep2_ip3d_err_isLoaded;
	int	nVetoElectrons7_;
	TBranch *nVetoElectrons7_branch;
	bool nVetoElectrons7_isLoaded;
	int	nVetoElectrons10_;
	TBranch *nVetoElectrons10_branch;
	bool nVetoElectrons10_isLoaded;
	int	nVetoElectrons25_;
	TBranch *nVetoElectrons25_branch;
	bool nVetoElectrons25_isLoaded;
	int	nVetoMuons5_;
	TBranch *nVetoMuons5_branch;
	bool nVetoMuons5_isLoaded;
	int	nVetoMuons10_;
	TBranch *nVetoMuons10_branch;
	bool nVetoMuons10_isLoaded;
	int	nVetoMuons25_;
	TBranch *nVetoMuons25_branch;
	bool nVetoMuons25_isLoaded;
	TString *filename_;
	TBranch *filename_branch;
	bool filename_isLoaded;
	float	lep1_ptrel_v0_;
	TBranch *lep1_ptrel_v0_branch;
	bool lep1_ptrel_v0_isLoaded;
	float	lep1_ptrel_v1_;
	TBranch *lep1_ptrel_v1_branch;
	bool lep1_ptrel_v1_isLoaded;
	float	lep2_ptrel_v0_;
	TBranch *lep2_ptrel_v0_branch;
	bool lep2_ptrel_v0_isLoaded;
	float	lep2_ptrel_v1_;
	TBranch *lep2_ptrel_v1_branch;
	bool lep2_ptrel_v1_isLoaded;
	float	lep1_miniIso_;
	TBranch *lep1_miniIso_branch;
	bool lep1_miniIso_isLoaded;
	float	lep2_miniIso_;
	TBranch *lep2_miniIso_branch;
	bool lep2_miniIso_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *jet_close_lep1_;
	TBranch *jet_close_lep1_branch;
	bool jet_close_lep1_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *jet_close_lep2_;
	TBranch *jet_close_lep2_branch;
	bool jet_close_lep2_isLoaded;
	vector<int> *eleID_kfhits_;
	TBranch *eleID_kfhits_branch;
	bool eleID_kfhits_isLoaded;
	vector<float> *eleID_oldsigmaietaieta_;
	TBranch *eleID_oldsigmaietaieta_branch;
	bool eleID_oldsigmaietaieta_isLoaded;
	vector<float> *eleID_oldsigmaiphiiphi_;
	TBranch *eleID_oldsigmaiphiiphi_branch;
	bool eleID_oldsigmaiphiiphi_isLoaded;
	vector<float> *eleID_oldcircularity_;
	TBranch *eleID_oldcircularity_branch;
	bool eleID_oldcircularity_isLoaded;
	vector<float> *eleID_oldr9_;
	TBranch *eleID_oldr9_branch;
	bool eleID_oldr9_isLoaded;
	vector<float> *eleID_scletawidth_;
	TBranch *eleID_scletawidth_branch;
	bool eleID_scletawidth_isLoaded;
	vector<float> *eleID_sclphiwidth_;
	TBranch *eleID_sclphiwidth_branch;
	bool eleID_sclphiwidth_isLoaded;
	vector<float> *eleID_he_;
	TBranch *eleID_he_branch;
	bool eleID_he_isLoaded;
	vector<float> *eleID_psEoverEraw_;
	TBranch *eleID_psEoverEraw_branch;
	bool eleID_psEoverEraw_isLoaded;
	vector<float> *eleID_kfchi2_;
	TBranch *eleID_kfchi2_branch;
	bool eleID_kfchi2_isLoaded;
	vector<float> *eleID_chi2_hits_;
	TBranch *eleID_chi2_hits_branch;
	bool eleID_chi2_hits_isLoaded;
	vector<float> *eleID_fbrem_;
	TBranch *eleID_fbrem_branch;
	bool eleID_fbrem_isLoaded;
	vector<float> *eleID_ep_;
	TBranch *eleID_ep_branch;
	bool eleID_ep_isLoaded;
	vector<float> *eleID_eelepout_;
	TBranch *eleID_eelepout_branch;
	bool eleID_eelepout_isLoaded;
	vector<float> *eleID_IoEmIop_;
	TBranch *eleID_IoEmIop_branch;
	bool eleID_IoEmIop_isLoaded;
	vector<float> *eleID_deltaetain_;
	TBranch *eleID_deltaetain_branch;
	bool eleID_deltaetain_isLoaded;
	vector<float> *eleID_deltaphiin_;
	TBranch *eleID_deltaphiin_branch;
	bool eleID_deltaphiin_isLoaded;
	vector<float> *eleID_deltaetaseed_;
	TBranch *eleID_deltaetaseed_branch;
	bool eleID_deltaetaseed_isLoaded;
	vector<float> *eleID_pT_;
	TBranch *eleID_pT_branch;
	bool eleID_pT_isLoaded;
	vector<bool> *eleID_isbarrel_;
	TBranch *eleID_isbarrel_branch;
	bool eleID_isbarrel_isLoaded;
	vector<bool> *eleID_isendcap_;
	TBranch *eleID_isendcap_branch;
	bool eleID_isendcap_isLoaded;
	vector<float> *eleID_scl_eta_;
	TBranch *eleID_scl_eta_branch;
	bool eleID_scl_eta_isLoaded;
	vector<float> *muID_dzPV_;
	TBranch *muID_dzPV_branch;
	bool muID_dzPV_isLoaded;
	vector<float> *muID_ptSig_;
	TBranch *muID_ptSig_branch;
	bool muID_ptSig_isLoaded;
	vector<float> *muID_ip3dSig_;
	TBranch *muID_ip3dSig_branch;
	bool muID_ip3dSig_isLoaded;
	vector<float> *muID_medMuonPOG_;
	TBranch *muID_medMuonPOG_branch;
	bool muID_medMuonPOG_isLoaded;
	vector<float> *muID_pt_;
	TBranch *muID_pt_branch;
	bool muID_pt_isLoaded;
	vector<float> *muID_eta_;
	TBranch *muID_eta_branch;
	bool muID_eta_isLoaded;
	vector<float> *trueNumInt_;
	TBranch *trueNumInt_branch;
	bool trueNumInt_isLoaded;
	vector<int> *nPUvertices_;
	TBranch *nPUvertices_branch;
	bool nPUvertices_isLoaded;
	int	nGoodVertices_;
	TBranch *nGoodVertices_branch;
	bool nGoodVertices_isLoaded;
	bool	lep1_trigMatch_noIsoReq_;
	TBranch *lep1_trigMatch_noIsoReq_branch;
	bool lep1_trigMatch_noIsoReq_isLoaded;
	bool	lep1_trigMatch_isoReq_;
	TBranch *lep1_trigMatch_isoReq_branch;
	bool lep1_trigMatch_isoReq_isLoaded;
	bool	lep2_trigMatch_noIsoReq_;
	TBranch *lep2_trigMatch_noIsoReq_branch;
	bool lep2_trigMatch_noIsoReq_isLoaded;
	bool	lep2_trigMatch_isoReq_;
	TBranch *lep2_trigMatch_isoReq_branch;
	bool lep2_trigMatch_isoReq_isLoaded;
	float	met3p0_;
	TBranch *met3p0_branch;
	bool met3p0_isLoaded;
	vector<float> *jet_corr_pt_;
	TBranch *jet_corr_pt_branch;
	bool jet_corr_pt_isLoaded;
	float	metphi3p0_;
	TBranch *metphi3p0_branch;
	bool metphi3p0_isLoaded;
	bool	passes_met_filters_;
	TBranch *passes_met_filters_branch;
	bool passes_met_filters_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *mostJets_;
	TBranch *mostJets_branch;
	bool mostJets_isLoaded;
	bool	madeExtraZ_;
	TBranch *madeExtraZ_branch;
	bool madeExtraZ_isLoaded;
	bool	madeExtraG_;
	TBranch *madeExtraG_branch;
	bool madeExtraG_isLoaded;
	bool	lep1_isGoodLeg_;
	TBranch *lep1_isGoodLeg_branch;
	bool lep1_isGoodLeg_isLoaded;
	bool	lep2_isGoodLeg_;
	TBranch *lep2_isGoodLeg_branch;
	bool lep2_isGoodLeg_isLoaded;
	bool	lep1_isFakeLeg_;
	TBranch *lep1_isFakeLeg_branch;
	bool lep1_isFakeLeg_isLoaded;
	bool	lep2_isFakeLeg_;
	TBranch *lep2_isFakeLeg_branch;
	bool lep2_isFakeLeg_isLoaded;
	bool	lep1_multiIso_;
	TBranch *lep1_multiIso_branch;
	bool lep1_multiIso_isLoaded;
	bool	lep2_multiIso_;
	TBranch *lep2_multiIso_branch;
	bool lep2_multiIso_isLoaded;
	float	lep1_sip_;
	TBranch *lep1_sip_branch;
	bool lep1_sip_isLoaded;
	float	lep2_sip_;
	TBranch *lep2_sip_branch;
	bool lep2_sip_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep1_closeJet_;
	TBranch *lep1_closeJet_branch;
	bool lep1_closeJet_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *lep2_closeJet_;
	TBranch *lep2_closeJet_branch;
	bool lep2_closeJet_isLoaded;
	bool	passed_id_inSituFR_lep1_;
	TBranch *passed_id_inSituFR_lep1_branch;
	bool passed_id_inSituFR_lep1_isLoaded;
	bool	passed_id_inSituFR_lep2_;
	TBranch *passed_id_inSituFR_lep2_branch;
	bool passed_id_inSituFR_lep2_isLoaded;
	bool	fired_trigger_;
	TBranch *fired_trigger_branch;
	bool fired_trigger_isLoaded;
	unsigned int	triggers_;
	TBranch *triggers_branch;
	bool triggers_isLoaded;
public: 
void Init(TTree *tree);
void GetEntry(unsigned int idx); 
void LoadAllBranches(); 
	const float &met();
	const float &metPhi();
	const float &corrMET();
	const float &corrMETphi();
	const int &event();
	const int &lumi();
	const int &run();
	const bool &	filt_csc();
	const bool &	filt_hbhe();
	const bool &	filt_hcallaser();
	const bool &	filt_ecaltp();
	const bool &	filt_trkfail();
	const bool &	filt_eebadsc();
	const bool &	is_real_data();
	const float &scale1fb();
	const float &xsec();
	const float &kfactor();
	const float &gen_met();
	const float &gen_met_phi();
	const int &njets();
	const int &njets_corr();
	const int &hyp_class();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4();
	const float &ht_corr();
	const float &ht();
	const int &lep1_motherID();
	const int &lep2_motherID();
	const int &lep1_mc_id();
	const int &lep2_mc_id();
	const int &lep1_id();
	const int &lep2_id();
	const float &lep1_coneCorrPt();
	const float &lep2_coneCorrPt();
	const int &lep1_idx();
	const int &lep2_idx();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_corr();
	const vector<float> &btags_disc();
	const vector<float> &jets_disc();
	const vector<float> &jets_JEC();
	const vector<float> &btags_JEC();
	const vector<float> &jets_undoJEC();
	const vector<float> &btags_undoJEC();
	const vector<float> &btags_corr_disc();
	const vector<float> &jets_corr_disc();
	const vector<float> &jets_corr_JEC();
	const vector<float> &btags_corr_JEC();
	const vector<float> &jets_corr_undoJEC();
	const vector<float> &btags_corr_undoJEC();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags();
	const int &nbtags();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_corr();
	const int &nbtags_corr();
	const float &sf_dilepTrig_hpt();
	const float &sf_dilepTrig_lpt();
	const float &sf_dilepTrig_vlpt();
	const int &hyp_type();
	const float &sf_dilep_eff();
	const float &mt();
	const float &mt_l2();
	const float &mt2();
	const float &mtmin();
	const float &mGluino();
	const float &mLSP();
	const float &mSbottom();
	const float &mChargino();
	const int &lep1_id_gen();
	const int &lep2_id_gen();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4_gen();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4_gen();
	const int &lep3_id();
	const int &lep3_idx();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep3_p4();
	const int &lep3_quality();
	const float &lep1_iso();
	const float &lep2_iso();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps_p4();
	const vector<int> &genps_id();
	const vector<int> &genps_id_mother();
	const vector<int> &genps_idx_mother();
	const vector<int> &genps_status();
	const vector<int> &genps_id_grandma();
	const bool &	lep1_passes_id();
	const bool &	lep2_passes_id();
	const bool &	lep3_passes_id();
	const bool &	lep3_tight();
	const bool &	lep3_veto();
	const bool &	lep3_fo();
	const float &lep1_dxyPV();
	const float &lep2_dxyPV();
	const float &lep1_dZ();
	const float &lep2_dZ();
	const float &lep1_d0_err();
	const float &lep2_d0_err();
	const float &lep1_ip3d();
	const float &lep2_ip3d();
	const float &lep1_MVA();
	const float &lep2_MVA();
	const float &lep1_ip3d_err();
	const float &lep2_ip3d_err();
	const int &nVetoElectrons7();
	const int &nVetoElectrons10();
	const int &nVetoElectrons25();
	const int &nVetoMuons5();
	const int &nVetoMuons10();
	const int &nVetoMuons25();
	const TString &filename();
	const float &lep1_ptrel_v0();
	const float &lep1_ptrel_v1();
	const float &lep2_ptrel_v0();
	const float &lep2_ptrel_v1();
	const float &lep1_miniIso();
	const float &lep2_miniIso();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &jet_close_lep1();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &jet_close_lep2();
	const vector<int> &eleID_kfhits();
	const vector<float> &eleID_oldsigmaietaieta();
	const vector<float> &eleID_oldsigmaiphiiphi();
	const vector<float> &eleID_oldcircularity();
	const vector<float> &eleID_oldr9();
	const vector<float> &eleID_scletawidth();
	const vector<float> &eleID_sclphiwidth();
	const vector<float> &eleID_he();
	const vector<float> &eleID_psEoverEraw();
	const vector<float> &eleID_kfchi2();
	const vector<float> &eleID_chi2_hits();
	const vector<float> &eleID_fbrem();
	const vector<float> &eleID_ep();
	const vector<float> &eleID_eelepout();
	const vector<float> &eleID_IoEmIop();
	const vector<float> &eleID_deltaetain();
	const vector<float> &eleID_deltaphiin();
	const vector<float> &eleID_deltaetaseed();
	const vector<float> &eleID_pT();
	const vector<bool> &eleID_isbarrel();
	const vector<bool> &eleID_isendcap();
	const vector<float> &eleID_scl_eta();
	const vector<float> &muID_dzPV();
	const vector<float> &muID_ptSig();
	const vector<float> &muID_ip3dSig();
	const vector<float> &muID_medMuonPOG();
	const vector<float> &muID_pt();
	const vector<float> &muID_eta();
	const vector<float> &trueNumInt();
	const vector<int> &nPUvertices();
	const int &nGoodVertices();
	const bool &	lep1_trigMatch_noIsoReq();
	const bool &	lep1_trigMatch_isoReq();
	const bool &	lep2_trigMatch_noIsoReq();
	const bool &	lep2_trigMatch_isoReq();
	const float &met3p0();
	const vector<float> &jet_corr_pt();
	const float &metphi3p0();
	const bool &	passes_met_filters();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &mostJets();
	const bool &	madeExtraZ();
	const bool &	madeExtraG();
	const bool &	lep1_isGoodLeg();
	const bool &	lep2_isGoodLeg();
	const bool &	lep1_isFakeLeg();
	const bool &	lep2_isFakeLeg();
	const bool &	lep1_multiIso();
	const bool &	lep2_multiIso();
	const float &lep1_sip();
	const float &lep2_sip();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_closeJet();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_closeJet();
	const bool &	passed_id_inSituFR_lep1();
	const bool &	passed_id_inSituFR_lep2();
	const bool &	fired_trigger();
	const unsigned int &triggers();

  static void progress( int nEventsTotal, int nEventsChain );
};

#ifndef __CINT__
extern SS samesign;
#endif

namespace ss {
	const float &met();
	const float &metPhi();
	const float &corrMET();
	const float &corrMETphi();
	const int &event();
	const int &lumi();
	const int &run();
	const bool &filt_csc();
	const bool &filt_hbhe();
	const bool &filt_hcallaser();
	const bool &filt_ecaltp();
	const bool &filt_trkfail();
	const bool &filt_eebadsc();
	const bool &is_real_data();
	const float &scale1fb();
	const float &xsec();
	const float &kfactor();
	const float &gen_met();
	const float &gen_met_phi();
	const int &njets();
	const int &njets_corr();
	const int &hyp_class();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4();
	const float &ht_corr();
	const float &ht();
	const int &lep1_motherID();
	const int &lep2_motherID();
	const int &lep1_mc_id();
	const int &lep2_mc_id();
	const int &lep1_id();
	const int &lep2_id();
	const float &lep1_coneCorrPt();
	const float &lep2_coneCorrPt();
	const int &lep1_idx();
	const int &lep2_idx();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_corr();
	const vector<float> &btags_disc();
	const vector<float> &jets_disc();
	const vector<float> &jets_JEC();
	const vector<float> &btags_JEC();
	const vector<float> &jets_undoJEC();
	const vector<float> &btags_undoJEC();
	const vector<float> &btags_corr_disc();
	const vector<float> &jets_corr_disc();
	const vector<float> &jets_corr_JEC();
	const vector<float> &btags_corr_JEC();
	const vector<float> &jets_corr_undoJEC();
	const vector<float> &btags_corr_undoJEC();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags();
	const int &nbtags();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_corr();
	const int &nbtags_corr();
	const float &sf_dilepTrig_hpt();
	const float &sf_dilepTrig_lpt();
	const float &sf_dilepTrig_vlpt();
	const int &hyp_type();
	const float &sf_dilep_eff();
	const float &mt();
	const float &mt_l2();
	const float &mt2();
	const float &mtmin();
	const float &mGluino();
	const float &mLSP();
	const float &mSbottom();
	const float &mChargino();
	const int &lep1_id_gen();
	const int &lep2_id_gen();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_p4_gen();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_p4_gen();
	const int &lep3_id();
	const int &lep3_idx();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep3_p4();
	const int &lep3_quality();
	const float &lep1_iso();
	const float &lep2_iso();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps_p4();
	const vector<int> &genps_id();
	const vector<int> &genps_id_mother();
	const vector<int> &genps_idx_mother();
	const vector<int> &genps_status();
	const vector<int> &genps_id_grandma();
	const bool &lep1_passes_id();
	const bool &lep2_passes_id();
	const bool &lep3_passes_id();
	const bool &lep3_tight();
	const bool &lep3_veto();
	const bool &lep3_fo();
	const float &lep1_dxyPV();
	const float &lep2_dxyPV();
	const float &lep1_dZ();
	const float &lep2_dZ();
	const float &lep1_d0_err();
	const float &lep2_d0_err();
	const float &lep1_ip3d();
	const float &lep2_ip3d();
	const float &lep1_MVA();
	const float &lep2_MVA();
	const float &lep1_ip3d_err();
	const float &lep2_ip3d_err();
	const int &nVetoElectrons7();
	const int &nVetoElectrons10();
	const int &nVetoElectrons25();
	const int &nVetoMuons5();
	const int &nVetoMuons10();
	const int &nVetoMuons25();
	const TString &filename();
	const float &lep1_ptrel_v0();
	const float &lep1_ptrel_v1();
	const float &lep2_ptrel_v0();
	const float &lep2_ptrel_v1();
	const float &lep1_miniIso();
	const float &lep2_miniIso();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &jet_close_lep1();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &jet_close_lep2();
	const vector<int> &eleID_kfhits();
	const vector<float> &eleID_oldsigmaietaieta();
	const vector<float> &eleID_oldsigmaiphiiphi();
	const vector<float> &eleID_oldcircularity();
	const vector<float> &eleID_oldr9();
	const vector<float> &eleID_scletawidth();
	const vector<float> &eleID_sclphiwidth();
	const vector<float> &eleID_he();
	const vector<float> &eleID_psEoverEraw();
	const vector<float> &eleID_kfchi2();
	const vector<float> &eleID_chi2_hits();
	const vector<float> &eleID_fbrem();
	const vector<float> &eleID_ep();
	const vector<float> &eleID_eelepout();
	const vector<float> &eleID_IoEmIop();
	const vector<float> &eleID_deltaetain();
	const vector<float> &eleID_deltaphiin();
	const vector<float> &eleID_deltaetaseed();
	const vector<float> &eleID_pT();
	const vector<bool> &eleID_isbarrel();
	const vector<bool> &eleID_isendcap();
	const vector<float> &eleID_scl_eta();
	const vector<float> &muID_dzPV();
	const vector<float> &muID_ptSig();
	const vector<float> &muID_ip3dSig();
	const vector<float> &muID_medMuonPOG();
	const vector<float> &muID_pt();
	const vector<float> &muID_eta();
	const vector<float> &trueNumInt();
	const vector<int> &nPUvertices();
	const int &nGoodVertices();
	const bool &lep1_trigMatch_noIsoReq();
	const bool &lep1_trigMatch_isoReq();
	const bool &lep2_trigMatch_noIsoReq();
	const bool &lep2_trigMatch_isoReq();
	const float &met3p0();
	const vector<float> &jet_corr_pt();
	const float &metphi3p0();
	const bool &passes_met_filters();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &mostJets();
	const bool &madeExtraZ();
	const bool &madeExtraG();
	const bool &lep1_isGoodLeg();
	const bool &lep2_isGoodLeg();
	const bool &lep1_isFakeLeg();
	const bool &lep2_isFakeLeg();
	const bool &lep1_multiIso();
	const bool &lep2_multiIso();
	const float &lep1_sip();
	const float &lep2_sip();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep1_closeJet();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &lep2_closeJet();
	const bool &passed_id_inSituFR_lep1();
	const bool &passed_id_inSituFR_lep2();
	const bool &fired_trigger();
	const unsigned int &triggers();
}
#endif
