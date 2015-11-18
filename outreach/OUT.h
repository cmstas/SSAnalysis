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
	int	genLep1_id_;
	TBranch *genLep1_id_branch;
	bool genLep1_id_isLoaded;
	int	genLep2_id_;
	TBranch *genLep2_id_branch;
	bool genLep2_id_isLoaded;
	int	genLep1_idx_;
	TBranch *genLep1_idx_branch;
	bool genLep1_idx_isLoaded;
	int	genLep2_idx_;
	TBranch *genLep2_idx_branch;
	bool genLep2_idx_isLoaded;
	float	htGen_;
	TBranch *htGen_branch;
	bool htGen_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *genJets_;
	TBranch *genJets_branch;
	bool genJets_isLoaded;
	int	nGenJets_;
	TBranch *nGenJets_branch;
	bool nGenJets_isLoaded;
	float	genmet_;
	TBranch *genmet_branch;
	bool genmet_isLoaded;
	bool	lep1_passID_;
	TBranch *lep1_passID_branch;
	bool lep1_passID_isLoaded;
	bool	lep2_passID_;
	TBranch *lep2_passID_branch;
	bool lep2_passID_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *pt1_reco_;
	TBranch *pt1_reco_branch;
	bool pt1_reco_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *pt2_reco_;
	TBranch *pt2_reco_branch;
	bool pt2_reco_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *id1_reco_;
	TBranch *id1_reco_branch;
	bool id1_reco_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *id2_reco_;
	TBranch *id2_reco_branch;
	bool id2_reco_isLoaded;
	int	idx1_reco_;
	TBranch *idx1_reco_branch;
	bool idx1_reco_isLoaded;
	int	idx2_reco_;
	TBranch *idx2_reco_branch;
	bool idx2_reco_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *genLep1_p4_;
	TBranch *genLep1_p4_branch;
	bool genLep1_p4_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *genLep2_p4_;
	TBranch *genLep2_p4_branch;
	bool genLep2_p4_isLoaded;
public: 
void Init(TTree *tree);
void GetEntry(unsigned int idx); 
void LoadAllBranches(); 
	const int &genLep1_id();
	const int &genLep2_id();
	const int &genLep1_idx();
	const int &genLep2_idx();
	const float &htGen();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genJets();
	const int &nGenJets();
	const float &genmet();
	const bool &	lep1_passID();
	const bool &	lep2_passID();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &pt1_reco();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &pt2_reco();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &id1_reco();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &id2_reco();
	const int &idx1_reco();
	const int &idx2_reco();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep1_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep2_p4();

  static void progress( int nEventsTotal, int nEventsChain );
};

#ifndef __CINT__
extern OUT outreach;
#endif

namespace out {
	const int &genLep1_id();
	const int &genLep2_id();
	const int &genLep1_idx();
	const int &genLep2_idx();
	const float &htGen();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genJets();
	const int &nGenJets();
	const float &genmet();
	const bool &lep1_passID();
	const bool &lep2_passID();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &pt1_reco();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &pt2_reco();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &id1_reco();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &id2_reco();
	const int &idx1_reco();
	const int &idx2_reco();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep1_p4();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep2_p4();
}
#endif
