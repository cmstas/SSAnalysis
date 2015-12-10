// -*- C++ -*-
#ifndef OUT_H
#define OUT_H
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

// Generated with the command
// makeCMS3ClassFiles("", "t", "OUT", "out", "outreach")

using namespace std; 
class OUT {
private: 
protected: 
	unsigned int index;
	vector<int> *genLep_id_;
	TBranch *genLep_id_branch;
	bool genLep_id_isLoaded;
	vector<int> *genLep_idx_;
	TBranch *genLep_idx_branch;
	bool genLep_idx_isLoaded;
	float	htGen_;
	TBranch *htGen_branch;
	bool htGen_isLoaded;
	float	ht_;
	TBranch *ht_branch;
	bool ht_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *genJets_;
	TBranch *genJets_branch;
	bool genJets_isLoaded;
	vector<bool> *genJets_isb_;
	TBranch *genJets_isb_branch;
	bool genJets_isb_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *recoJets_;
	TBranch *recoJets_branch;
	bool recoJets_isLoaded;
	int	nGenJets_;
	TBranch *nGenJets_branch;
	bool nGenJets_isLoaded;
	float	genmet_;
	TBranch *genmet_branch;
	bool genmet_isLoaded;
	float	met_;
	TBranch *met_branch;
	bool met_isLoaded;
	vector<bool> *lep_passID_;
	TBranch *lep_passID_branch;
	bool lep_passID_isLoaded;
	vector<bool> *lep_passID_loose_;
	TBranch *lep_passID_loose_branch;
	bool lep_passID_loose_isLoaded;
	vector<int> *id_reco_;
	TBranch *id_reco_branch;
	bool id_reco_isLoaded;
	vector<int> *idx_reco_;
	TBranch *idx_reco_branch;
	bool idx_reco_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *genLep_p4_;
	TBranch *genLep_p4_branch;
	bool genLep_p4_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *recoLep_p4_;
	TBranch *recoLep_p4_branch;
	bool recoLep_p4_isLoaded;
	vector<int> *genJets_matched_;
	TBranch *genJets_matched_branch;
	bool genJets_matched_isLoaded;
	vector<int> *genJets_matched_tob_;
	TBranch *genJets_matched_tob_branch;
	bool genJets_matched_tob_isLoaded;
	vector<int> *genJets_matchedID_;
	TBranch *genJets_matchedID_branch;
	bool genJets_matchedID_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *pfjets_p4_;
	TBranch *pfjets_p4_branch;
	bool pfjets_p4_isLoaded;
	vector<bool> *pfjets_isClean_;
	TBranch *pfjets_isClean_branch;
	bool pfjets_isClean_isLoaded;
	vector<bool> *pfjets_isb_;
	TBranch *pfjets_isb_branch;
	bool pfjets_isb_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *recoMuons_;
	TBranch *recoMuons_branch;
	bool recoMuons_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *recoElectrons_;
	TBranch *recoElectrons_branch;
	bool recoElectrons_isLoaded;
	vector<int> *recoMuonsMatch_;
	TBranch *recoMuonsMatch_branch;
	bool recoMuonsMatch_isLoaded;
	vector<bool> *recoMuonsID_;
	TBranch *recoMuonsID_branch;
	bool recoMuonsID_isLoaded;
	vector<bool> *recoMuonsID_loose_;
	TBranch *recoMuonsID_loose_branch;
	bool recoMuonsID_loose_isLoaded;
	vector<int> *recoElectronsMatch_;
	TBranch *recoElectronsMatch_branch;
	bool recoElectronsMatch_isLoaded;
	vector<bool> *recoElectronsID_;
	TBranch *recoElectronsID_branch;
	bool recoElectronsID_isLoaded;
	vector<bool> *recoElectronsID_loose_;
	TBranch *recoElectronsID_loose_branch;
	bool recoElectronsID_loose_isLoaded;
	vector<int> *pfjets_match_;
	TBranch *pfjets_match_branch;
	bool pfjets_match_isLoaded;
	vector<bool> *pfjets_matchb_;
	TBranch *pfjets_matchb_branch;
	bool pfjets_matchb_isLoaded;
	vector<bool> *pfjets_ID_;
	TBranch *pfjets_ID_branch;
	bool pfjets_ID_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *genLep1_p4_;
	TBranch *genLep1_p4_branch;
	bool genLep1_p4_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *genLep2_p4_;
	TBranch *genLep2_p4_branch;
	bool genLep2_p4_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *recoLep1_p4_;
	TBranch *recoLep1_p4_branch;
	bool recoLep1_p4_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *recoLep2_p4_;
	TBranch *recoLep2_p4_branch;
	bool recoLep2_p4_isLoaded;
	int	genLep1_id_;
	TBranch *genLep1_id_branch;
	bool genLep1_id_isLoaded;
	int	genLep2_id_;
	TBranch *genLep2_id_branch;
	bool genLep2_id_isLoaded;
	bool	lep1_passID_;
	TBranch *lep1_passID_branch;
	bool lep1_passID_isLoaded;
	bool	lep2_passID_;
	TBranch *lep2_passID_branch;
	bool lep2_passID_isLoaded;
	int	id1_reco_;
	TBranch *id1_reco_branch;
	bool id1_reco_isLoaded;
	int	id2_reco_;
	TBranch *id2_reco_branch;
	bool id2_reco_isLoaded;
	int	idx1_reco_;
	TBranch *idx1_reco_branch;
	bool idx1_reco_isLoaded;
	int	idx2_reco_;
	TBranch *idx2_reco_branch;
	bool idx2_reco_isLoaded;
public: 
void Init(TTree *tree);
void GetEntry(unsigned int idx); 
void LoadAllBranches(); 
	const vector<int> &genLep_id();
	const vector<int> &genLep_idx();
	const float &htGen();
	const float &ht();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genJets();
	const vector<bool> &genJets_isb();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoJets();
	const int &nGenJets();
	const float &genmet();
	const float &met();
	const vector<bool> &lep_passID();
	const vector<bool> &lep_passID_loose();
	const vector<int> &id_reco();
	const vector<int> &idx_reco();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genLep_p4();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoLep_p4();
	const vector<int> &genJets_matched();
	const vector<int> &genJets_matched_tob();
	const vector<int> &genJets_matchedID();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &pfjets_p4();
	const vector<bool> &pfjets_isClean();
	const vector<bool> &pfjets_isb();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoMuons();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoElectrons();
	const vector<int> &recoMuonsMatch();
	const vector<bool> &recoMuonsID();
	const vector<bool> &recoMuonsID_loose();
	const vector<int> &recoElectronsMatch();
	const vector<bool> &recoElectronsID();
	const vector<bool> &recoElectronsID_loose();
	const vector<int> &pfjets_match();
	const vector<bool> &pfjets_matchb();
	const vector<bool> &pfjets_ID();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep1_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep2_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &recoLep1_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &recoLep2_p4();
	const int &genLep1_id();
	const int &genLep2_id();
	const bool &	lep1_passID();
	const bool &	lep2_passID();
	const int &id1_reco();
	const int &id2_reco();
	const int &idx1_reco();
	const int &idx2_reco();

  static void progress( int nEventsTotal, int nEventsChain );
};

#ifndef __CINT__
extern OUT outreach;
#endif

namespace out {
	const vector<int> &genLep_id();
	const vector<int> &genLep_idx();
	const float &htGen();
	const float &ht();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genJets();
	const vector<bool> &genJets_isb();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoJets();
	const int &nGenJets();
	const float &genmet();
	const float &met();
	const vector<bool> &lep_passID();
	const vector<bool> &lep_passID_loose();
	const vector<int> &id_reco();
	const vector<int> &idx_reco();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genLep_p4();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoLep_p4();
	const vector<int> &genJets_matched();
	const vector<int> &genJets_matched_tob();
	const vector<int> &genJets_matchedID();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &pfjets_p4();
	const vector<bool> &pfjets_isClean();
	const vector<bool> &pfjets_isb();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoMuons();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoElectrons();
	const vector<int> &recoMuonsMatch();
	const vector<bool> &recoMuonsID();
	const vector<bool> &recoMuonsID_loose();
	const vector<int> &recoElectronsMatch();
	const vector<bool> &recoElectronsID();
	const vector<bool> &recoElectronsID_loose();
	const vector<int> &pfjets_match();
	const vector<bool> &pfjets_matchb();
	const vector<bool> &pfjets_ID();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep1_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep2_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &recoLep1_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &recoLep2_p4();
	const int &genLep1_id();
	const int &genLep2_id();
	const bool &lep1_passID();
	const bool &lep2_passID();
	const int &id1_reco();
	const int &id2_reco();
	const int &idx1_reco();
	const int &idx2_reco();
}
#endif
