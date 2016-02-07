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
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std; 
class SS {
private: 
protected: 
	unsigned int index;
	float	met_gen_;
	TBranch *met_gen_branch;
	bool met_gen_isLoaded;
	float	ht_gen_;
	TBranch *ht_gen_branch;
	bool ht_gen_isLoaded;
	float	ht_jets_gen_;
	TBranch *ht_jets_gen_branch;
	bool ht_jets_gen_isLoaded;
	int	njets_gen_;
	TBranch *njets_gen_branch;
	bool njets_gen_isLoaded;
	int	njets_jets_gen_;
	TBranch *njets_jets_gen_branch;
	bool njets_jets_gen_isLoaded;
	int	nbtags_gen_;
	TBranch *nbtags_gen_branch;
	bool nbtags_gen_isLoaded;
	int	nbtags_jets_gen_;
	TBranch *nbtags_jets_gen_branch;
	bool nbtags_jets_gen_isLoaded;
	float	pt1_gen_;
	TBranch *pt1_gen_branch;
	bool pt1_gen_isLoaded;
	float	pt2_gen_;
	TBranch *pt2_gen_branch;
	bool pt2_gen_isLoaded;
	float	eta1_gen_;
	TBranch *eta1_gen_branch;
	bool eta1_gen_isLoaded;
	float	eta2_gen_;
	TBranch *eta2_gen_branch;
	bool eta2_gen_isLoaded;
	int	id1_gen_;
	TBranch *id1_gen_branch;
	bool id1_gen_isLoaded;
	int	id2_gen_;
	TBranch *id2_gen_branch;
	bool id2_gen_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_gen_;
	TBranch *jets_gen_branch;
	bool jets_gen_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_direct_gen_;
	TBranch *jets_direct_gen_branch;
	bool jets_direct_gen_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *btags_gen_;
	TBranch *btags_gen_branch;
	bool btags_gen_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *btags_direct_gen_;
	TBranch *btags_direct_gen_branch;
	bool btags_direct_gen_isLoaded;
	bool	choseRightHyp_;
	TBranch *choseRightHyp_branch;
	bool choseRightHyp_isLoaded;
	int	id1_reco_;
	TBranch *id1_reco_branch;
	bool id1_reco_isLoaded;
	int	id2_reco_;
	TBranch *id2_reco_branch;
	bool id2_reco_isLoaded;
	float	pt1_reco_;
	TBranch *pt1_reco_branch;
	bool pt1_reco_isLoaded;
	float	pt2_reco_;
	TBranch *pt2_reco_branch;
	bool pt2_reco_isLoaded;
	float	eta1_reco_;
	TBranch *eta1_reco_branch;
	bool eta1_reco_isLoaded;
	float	eta2_reco_;
	TBranch *eta2_reco_branch;
	bool eta2_reco_isLoaded;
	float	met_reco_;
	TBranch *met_reco_branch;
	bool met_reco_isLoaded;
	float	ht_reco_;
	TBranch *ht_reco_branch;
	bool ht_reco_isLoaded;
	float	ht_direct_reco_;
	TBranch *ht_direct_reco_branch;
	bool ht_direct_reco_isLoaded;
	bool	isGoodLepton_1_;
	TBranch *isGoodLepton_1_branch;
	bool isGoodLepton_1_isLoaded;
	bool	isIsolatedLepton_1_;
	TBranch *isIsolatedLepton_1_branch;
	bool isIsolatedLepton_1_isLoaded;
	bool	isNumeratorLepton_1_;
	TBranch *isNumeratorLepton_1_branch;
	bool isNumeratorLepton_1_isLoaded;
	bool	isGoodLepton_2_;
	TBranch *isGoodLepton_2_branch;
	bool isGoodLepton_2_isLoaded;
	bool	isIsolatedLepton_2_;
	TBranch *isIsolatedLepton_2_branch;
	bool isIsolatedLepton_2_isLoaded;
	bool	isNumeratorLepton_2_;
	TBranch *isNumeratorLepton_2_branch;
	bool isNumeratorLepton_2_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_reco_;
	TBranch *jets_reco_branch;
	bool jets_reco_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_reco_direct_;
	TBranch *jets_reco_direct_branch;
	bool jets_reco_direct_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_reco_cms2_;
	TBranch *jets_reco_cms2_branch;
	bool jets_reco_cms2_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *btags_reco_;
	TBranch *btags_reco_branch;
	bool btags_reco_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *btags_jets_reco_;
	TBranch *btags_jets_reco_branch;
	bool btags_jets_reco_isLoaded;
	int	njets_reco_;
	TBranch *njets_reco_branch;
	bool njets_reco_isLoaded;
	int	njets_reco_direct_;
	TBranch *njets_reco_direct_branch;
	bool njets_reco_direct_isLoaded;
	int	njets_reco_cms2_;
	TBranch *njets_reco_cms2_branch;
	bool njets_reco_cms2_isLoaded;
	int	nbtags_reco_;
	TBranch *nbtags_reco_branch;
	bool nbtags_reco_isLoaded;
	int	nbtags_jets_reco_;
	TBranch *nbtags_jets_reco_branch;
	bool nbtags_jets_reco_isLoaded;
	vector<bool> *gen_btag_is_matched_;
	TBranch *gen_btag_is_matched_branch;
	bool gen_btag_is_matched_isLoaded;
	vector<bool> *gen_btag_is_matched_direct_;
	TBranch *gen_btag_is_matched_direct_branch;
	bool gen_btag_is_matched_direct_isLoaded;
	vector<bool> *jet_is_matched_;
	TBranch *jet_is_matched_branch;
	bool jet_is_matched_isLoaded;
	vector<bool> *jet_is_matched_direct_;
	TBranch *jet_is_matched_direct_branch;
	bool jet_is_matched_direct_isLoaded;
	vector<float> *sparm_value_;
	TBranch *sparm_value_branch;
	bool sparm_value_isLoaded;
	vector<TString> *sparm_name_;
	TBranch *sparm_name_branch;
	bool sparm_name_isLoaded;
public: 
void Init(TTree *tree) {
	jets_gen_branch = 0;
	if (tree->GetBranch("jets_gen") != 0) {
		jets_gen_branch = tree->GetBranch("jets_gen");
		if (jets_gen_branch) {jets_gen_branch->SetAddress(&jets_gen_);}
	}
	jets_direct_gen_branch = 0;
	if (tree->GetBranch("jets_direct_gen") != 0) {
		jets_direct_gen_branch = tree->GetBranch("jets_direct_gen");
		if (jets_direct_gen_branch) {jets_direct_gen_branch->SetAddress(&jets_direct_gen_);}
	}
	btags_gen_branch = 0;
	if (tree->GetBranch("btags_gen") != 0) {
		btags_gen_branch = tree->GetBranch("btags_gen");
		if (btags_gen_branch) {btags_gen_branch->SetAddress(&btags_gen_);}
	}
	btags_direct_gen_branch = 0;
	if (tree->GetBranch("btags_direct_gen") != 0) {
		btags_direct_gen_branch = tree->GetBranch("btags_direct_gen");
		if (btags_direct_gen_branch) {btags_direct_gen_branch->SetAddress(&btags_direct_gen_);}
	}
	jets_reco_branch = 0;
	if (tree->GetBranch("jets_reco") != 0) {
		jets_reco_branch = tree->GetBranch("jets_reco");
		if (jets_reco_branch) {jets_reco_branch->SetAddress(&jets_reco_);}
	}
	jets_reco_direct_branch = 0;
	if (tree->GetBranch("jets_reco_direct") != 0) {
		jets_reco_direct_branch = tree->GetBranch("jets_reco_direct");
		if (jets_reco_direct_branch) {jets_reco_direct_branch->SetAddress(&jets_reco_direct_);}
	}
	jets_reco_cms2_branch = 0;
	if (tree->GetBranch("jets_reco_cms2") != 0) {
		jets_reco_cms2_branch = tree->GetBranch("jets_reco_cms2");
		if (jets_reco_cms2_branch) {jets_reco_cms2_branch->SetAddress(&jets_reco_cms2_);}
	}
	btags_reco_branch = 0;
	if (tree->GetBranch("btags_reco") != 0) {
		btags_reco_branch = tree->GetBranch("btags_reco");
		if (btags_reco_branch) {btags_reco_branch->SetAddress(&btags_reco_);}
	}
	btags_jets_reco_branch = 0;
	if (tree->GetBranch("btags_jets_reco") != 0) {
		btags_jets_reco_branch = tree->GetBranch("btags_jets_reco");
		if (btags_jets_reco_branch) {btags_jets_reco_branch->SetAddress(&btags_jets_reco_);}
	}
  tree->SetMakeClass(1);
	met_gen_branch = 0;
	if (tree->GetBranch("met_gen") != 0) {
		met_gen_branch = tree->GetBranch("met_gen");
		if (met_gen_branch) {met_gen_branch->SetAddress(&met_gen_);}
	}
	ht_gen_branch = 0;
	if (tree->GetBranch("ht_gen") != 0) {
		ht_gen_branch = tree->GetBranch("ht_gen");
		if (ht_gen_branch) {ht_gen_branch->SetAddress(&ht_gen_);}
	}
	ht_jets_gen_branch = 0;
	if (tree->GetBranch("ht_jets_gen") != 0) {
		ht_jets_gen_branch = tree->GetBranch("ht_jets_gen");
		if (ht_jets_gen_branch) {ht_jets_gen_branch->SetAddress(&ht_jets_gen_);}
	}
	njets_gen_branch = 0;
	if (tree->GetBranch("njets_gen") != 0) {
		njets_gen_branch = tree->GetBranch("njets_gen");
		if (njets_gen_branch) {njets_gen_branch->SetAddress(&njets_gen_);}
	}
	njets_jets_gen_branch = 0;
	if (tree->GetBranch("njets_jets_gen") != 0) {
		njets_jets_gen_branch = tree->GetBranch("njets_jets_gen");
		if (njets_jets_gen_branch) {njets_jets_gen_branch->SetAddress(&njets_jets_gen_);}
	}
	nbtags_gen_branch = 0;
	if (tree->GetBranch("nbtags_gen") != 0) {
		nbtags_gen_branch = tree->GetBranch("nbtags_gen");
		if (nbtags_gen_branch) {nbtags_gen_branch->SetAddress(&nbtags_gen_);}
	}
	nbtags_jets_gen_branch = 0;
	if (tree->GetBranch("nbtags_jets_gen") != 0) {
		nbtags_jets_gen_branch = tree->GetBranch("nbtags_jets_gen");
		if (nbtags_jets_gen_branch) {nbtags_jets_gen_branch->SetAddress(&nbtags_jets_gen_);}
	}
	pt1_gen_branch = 0;
	if (tree->GetBranch("pt1_gen") != 0) {
		pt1_gen_branch = tree->GetBranch("pt1_gen");
		if (pt1_gen_branch) {pt1_gen_branch->SetAddress(&pt1_gen_);}
	}
	pt2_gen_branch = 0;
	if (tree->GetBranch("pt2_gen") != 0) {
		pt2_gen_branch = tree->GetBranch("pt2_gen");
		if (pt2_gen_branch) {pt2_gen_branch->SetAddress(&pt2_gen_);}
	}
	eta1_gen_branch = 0;
	if (tree->GetBranch("eta1_gen") != 0) {
		eta1_gen_branch = tree->GetBranch("eta1_gen");
		if (eta1_gen_branch) {eta1_gen_branch->SetAddress(&eta1_gen_);}
	}
	eta2_gen_branch = 0;
	if (tree->GetBranch("eta2_gen") != 0) {
		eta2_gen_branch = tree->GetBranch("eta2_gen");
		if (eta2_gen_branch) {eta2_gen_branch->SetAddress(&eta2_gen_);}
	}
	id1_gen_branch = 0;
	if (tree->GetBranch("id1_gen") != 0) {
		id1_gen_branch = tree->GetBranch("id1_gen");
		if (id1_gen_branch) {id1_gen_branch->SetAddress(&id1_gen_);}
	}
	id2_gen_branch = 0;
	if (tree->GetBranch("id2_gen") != 0) {
		id2_gen_branch = tree->GetBranch("id2_gen");
		if (id2_gen_branch) {id2_gen_branch->SetAddress(&id2_gen_);}
	}
	choseRightHyp_branch = 0;
	if (tree->GetBranch("choseRightHyp") != 0) {
		choseRightHyp_branch = tree->GetBranch("choseRightHyp");
		if (choseRightHyp_branch) {choseRightHyp_branch->SetAddress(&choseRightHyp_);}
	}
	id1_reco_branch = 0;
	if (tree->GetBranch("id1_reco") != 0) {
		id1_reco_branch = tree->GetBranch("id1_reco");
		if (id1_reco_branch) {id1_reco_branch->SetAddress(&id1_reco_);}
	}
	id2_reco_branch = 0;
	if (tree->GetBranch("id2_reco") != 0) {
		id2_reco_branch = tree->GetBranch("id2_reco");
		if (id2_reco_branch) {id2_reco_branch->SetAddress(&id2_reco_);}
	}
	pt1_reco_branch = 0;
	if (tree->GetBranch("pt1_reco") != 0) {
		pt1_reco_branch = tree->GetBranch("pt1_reco");
		if (pt1_reco_branch) {pt1_reco_branch->SetAddress(&pt1_reco_);}
	}
	pt2_reco_branch = 0;
	if (tree->GetBranch("pt2_reco") != 0) {
		pt2_reco_branch = tree->GetBranch("pt2_reco");
		if (pt2_reco_branch) {pt2_reco_branch->SetAddress(&pt2_reco_);}
	}
	eta1_reco_branch = 0;
	if (tree->GetBranch("eta1_reco") != 0) {
		eta1_reco_branch = tree->GetBranch("eta1_reco");
		if (eta1_reco_branch) {eta1_reco_branch->SetAddress(&eta1_reco_);}
	}
	eta2_reco_branch = 0;
	if (tree->GetBranch("eta2_reco") != 0) {
		eta2_reco_branch = tree->GetBranch("eta2_reco");
		if (eta2_reco_branch) {eta2_reco_branch->SetAddress(&eta2_reco_);}
	}
	met_reco_branch = 0;
	if (tree->GetBranch("met_reco") != 0) {
		met_reco_branch = tree->GetBranch("met_reco");
		if (met_reco_branch) {met_reco_branch->SetAddress(&met_reco_);}
	}
	ht_reco_branch = 0;
	if (tree->GetBranch("ht_reco") != 0) {
		ht_reco_branch = tree->GetBranch("ht_reco");
		if (ht_reco_branch) {ht_reco_branch->SetAddress(&ht_reco_);}
	}
	ht_direct_reco_branch = 0;
	if (tree->GetBranch("ht_direct_reco") != 0) {
		ht_direct_reco_branch = tree->GetBranch("ht_direct_reco");
		if (ht_direct_reco_branch) {ht_direct_reco_branch->SetAddress(&ht_direct_reco_);}
	}
	isGoodLepton_1_branch = 0;
	if (tree->GetBranch("isGoodLepton_1") != 0) {
		isGoodLepton_1_branch = tree->GetBranch("isGoodLepton_1");
		if (isGoodLepton_1_branch) {isGoodLepton_1_branch->SetAddress(&isGoodLepton_1_);}
	}
	isIsolatedLepton_1_branch = 0;
	if (tree->GetBranch("isIsolatedLepton_1") != 0) {
		isIsolatedLepton_1_branch = tree->GetBranch("isIsolatedLepton_1");
		if (isIsolatedLepton_1_branch) {isIsolatedLepton_1_branch->SetAddress(&isIsolatedLepton_1_);}
	}
	isNumeratorLepton_1_branch = 0;
	if (tree->GetBranch("isNumeratorLepton_1") != 0) {
		isNumeratorLepton_1_branch = tree->GetBranch("isNumeratorLepton_1");
		if (isNumeratorLepton_1_branch) {isNumeratorLepton_1_branch->SetAddress(&isNumeratorLepton_1_);}
	}
	isGoodLepton_2_branch = 0;
	if (tree->GetBranch("isGoodLepton_2") != 0) {
		isGoodLepton_2_branch = tree->GetBranch("isGoodLepton_2");
		if (isGoodLepton_2_branch) {isGoodLepton_2_branch->SetAddress(&isGoodLepton_2_);}
	}
	isIsolatedLepton_2_branch = 0;
	if (tree->GetBranch("isIsolatedLepton_2") != 0) {
		isIsolatedLepton_2_branch = tree->GetBranch("isIsolatedLepton_2");
		if (isIsolatedLepton_2_branch) {isIsolatedLepton_2_branch->SetAddress(&isIsolatedLepton_2_);}
	}
	isNumeratorLepton_2_branch = 0;
	if (tree->GetBranch("isNumeratorLepton_2") != 0) {
		isNumeratorLepton_2_branch = tree->GetBranch("isNumeratorLepton_2");
		if (isNumeratorLepton_2_branch) {isNumeratorLepton_2_branch->SetAddress(&isNumeratorLepton_2_);}
	}
	njets_reco_branch = 0;
	if (tree->GetBranch("njets_reco") != 0) {
		njets_reco_branch = tree->GetBranch("njets_reco");
		if (njets_reco_branch) {njets_reco_branch->SetAddress(&njets_reco_);}
	}
	njets_reco_direct_branch = 0;
	if (tree->GetBranch("njets_reco_direct") != 0) {
		njets_reco_direct_branch = tree->GetBranch("njets_reco_direct");
		if (njets_reco_direct_branch) {njets_reco_direct_branch->SetAddress(&njets_reco_direct_);}
	}
	njets_reco_cms2_branch = 0;
	if (tree->GetBranch("njets_reco_cms2") != 0) {
		njets_reco_cms2_branch = tree->GetBranch("njets_reco_cms2");
		if (njets_reco_cms2_branch) {njets_reco_cms2_branch->SetAddress(&njets_reco_cms2_);}
	}
	nbtags_reco_branch = 0;
	if (tree->GetBranch("nbtags_reco") != 0) {
		nbtags_reco_branch = tree->GetBranch("nbtags_reco");
		if (nbtags_reco_branch) {nbtags_reco_branch->SetAddress(&nbtags_reco_);}
	}
	nbtags_jets_reco_branch = 0;
	if (tree->GetBranch("nbtags_jets_reco") != 0) {
		nbtags_jets_reco_branch = tree->GetBranch("nbtags_jets_reco");
		if (nbtags_jets_reco_branch) {nbtags_jets_reco_branch->SetAddress(&nbtags_jets_reco_);}
	}
	gen_btag_is_matched_branch = 0;
	if (tree->GetBranch("gen_btag_is_matched") != 0) {
		gen_btag_is_matched_branch = tree->GetBranch("gen_btag_is_matched");
		if (gen_btag_is_matched_branch) {gen_btag_is_matched_branch->SetAddress(&gen_btag_is_matched_);}
	}
	gen_btag_is_matched_direct_branch = 0;
	if (tree->GetBranch("gen_btag_is_matched_direct") != 0) {
		gen_btag_is_matched_direct_branch = tree->GetBranch("gen_btag_is_matched_direct");
		if (gen_btag_is_matched_direct_branch) {gen_btag_is_matched_direct_branch->SetAddress(&gen_btag_is_matched_direct_);}
	}
	jet_is_matched_branch = 0;
	if (tree->GetBranch("jet_is_matched") != 0) {
		jet_is_matched_branch = tree->GetBranch("jet_is_matched");
		if (jet_is_matched_branch) {jet_is_matched_branch->SetAddress(&jet_is_matched_);}
	}
	jet_is_matched_direct_branch = 0;
	if (tree->GetBranch("jet_is_matched_direct") != 0) {
		jet_is_matched_direct_branch = tree->GetBranch("jet_is_matched_direct");
		if (jet_is_matched_direct_branch) {jet_is_matched_direct_branch->SetAddress(&jet_is_matched_direct_);}
	}
	sparm_value_branch = 0;
	if (tree->GetBranch("sparm_value") != 0) {
		sparm_value_branch = tree->GetBranch("sparm_value");
		if (sparm_value_branch) {sparm_value_branch->SetAddress(&sparm_value_);}
	}
	sparm_name_branch = 0;
	if (tree->GetBranch("sparm_name") != 0) {
		sparm_name_branch = tree->GetBranch("sparm_name");
		if (sparm_name_branch) {sparm_name_branch->SetAddress(&sparm_name_);}
	}
  tree->SetMakeClass(0);
}
void GetEntry(unsigned int idx) 
	// this only marks branches as not loaded, saving a lot of time
	{
		index = idx;
		met_gen_isLoaded = false;
		ht_gen_isLoaded = false;
		ht_jets_gen_isLoaded = false;
		njets_gen_isLoaded = false;
		njets_jets_gen_isLoaded = false;
		nbtags_gen_isLoaded = false;
		nbtags_jets_gen_isLoaded = false;
		pt1_gen_isLoaded = false;
		pt2_gen_isLoaded = false;
		eta1_gen_isLoaded = false;
		eta2_gen_isLoaded = false;
		id1_gen_isLoaded = false;
		id2_gen_isLoaded = false;
		jets_gen_isLoaded = false;
		jets_direct_gen_isLoaded = false;
		btags_gen_isLoaded = false;
		btags_direct_gen_isLoaded = false;
		choseRightHyp_isLoaded = false;
		id1_reco_isLoaded = false;
		id2_reco_isLoaded = false;
		pt1_reco_isLoaded = false;
		pt2_reco_isLoaded = false;
		eta1_reco_isLoaded = false;
		eta2_reco_isLoaded = false;
		met_reco_isLoaded = false;
		ht_reco_isLoaded = false;
		ht_direct_reco_isLoaded = false;
		isGoodLepton_1_isLoaded = false;
		isIsolatedLepton_1_isLoaded = false;
		isNumeratorLepton_1_isLoaded = false;
		isGoodLepton_2_isLoaded = false;
		isIsolatedLepton_2_isLoaded = false;
		isNumeratorLepton_2_isLoaded = false;
		jets_reco_isLoaded = false;
		jets_reco_direct_isLoaded = false;
		jets_reco_cms2_isLoaded = false;
		btags_reco_isLoaded = false;
		btags_jets_reco_isLoaded = false;
		njets_reco_isLoaded = false;
		njets_reco_direct_isLoaded = false;
		njets_reco_cms2_isLoaded = false;
		nbtags_reco_isLoaded = false;
		nbtags_jets_reco_isLoaded = false;
		gen_btag_is_matched_isLoaded = false;
		gen_btag_is_matched_direct_isLoaded = false;
		jet_is_matched_isLoaded = false;
		jet_is_matched_direct_isLoaded = false;
		sparm_value_isLoaded = false;
		sparm_name_isLoaded = false;
	}

void LoadAllBranches() 
	// load all branches
{
	if (met_gen_branch != 0) met_gen();
	if (ht_gen_branch != 0) ht_gen();
	if (ht_jets_gen_branch != 0) ht_jets_gen();
	if (njets_gen_branch != 0) njets_gen();
	if (njets_jets_gen_branch != 0) njets_jets_gen();
	if (nbtags_gen_branch != 0) nbtags_gen();
	if (nbtags_jets_gen_branch != 0) nbtags_jets_gen();
	if (pt1_gen_branch != 0) pt1_gen();
	if (pt2_gen_branch != 0) pt2_gen();
	if (eta1_gen_branch != 0) eta1_gen();
	if (eta2_gen_branch != 0) eta2_gen();
	if (id1_gen_branch != 0) id1_gen();
	if (id2_gen_branch != 0) id2_gen();
	if (jets_gen_branch != 0) jets_gen();
	if (jets_direct_gen_branch != 0) jets_direct_gen();
	if (btags_gen_branch != 0) btags_gen();
	if (btags_direct_gen_branch != 0) btags_direct_gen();
	if (choseRightHyp_branch != 0) choseRightHyp();
	if (id1_reco_branch != 0) id1_reco();
	if (id2_reco_branch != 0) id2_reco();
	if (pt1_reco_branch != 0) pt1_reco();
	if (pt2_reco_branch != 0) pt2_reco();
	if (eta1_reco_branch != 0) eta1_reco();
	if (eta2_reco_branch != 0) eta2_reco();
	if (met_reco_branch != 0) met_reco();
	if (ht_reco_branch != 0) ht_reco();
	if (ht_direct_reco_branch != 0) ht_direct_reco();
	if (isGoodLepton_1_branch != 0) isGoodLepton_1();
	if (isIsolatedLepton_1_branch != 0) isIsolatedLepton_1();
	if (isNumeratorLepton_1_branch != 0) isNumeratorLepton_1();
	if (isGoodLepton_2_branch != 0) isGoodLepton_2();
	if (isIsolatedLepton_2_branch != 0) isIsolatedLepton_2();
	if (isNumeratorLepton_2_branch != 0) isNumeratorLepton_2();
	if (jets_reco_branch != 0) jets_reco();
	if (jets_reco_direct_branch != 0) jets_reco_direct();
	if (jets_reco_cms2_branch != 0) jets_reco_cms2();
	if (btags_reco_branch != 0) btags_reco();
	if (btags_jets_reco_branch != 0) btags_jets_reco();
	if (njets_reco_branch != 0) njets_reco();
	if (njets_reco_direct_branch != 0) njets_reco_direct();
	if (njets_reco_cms2_branch != 0) njets_reco_cms2();
	if (nbtags_reco_branch != 0) nbtags_reco();
	if (nbtags_jets_reco_branch != 0) nbtags_jets_reco();
	if (gen_btag_is_matched_branch != 0) gen_btag_is_matched();
	if (gen_btag_is_matched_direct_branch != 0) gen_btag_is_matched_direct();
	if (jet_is_matched_branch != 0) jet_is_matched();
	if (jet_is_matched_direct_branch != 0) jet_is_matched_direct();
	if (sparm_value_branch != 0) sparm_value();
	if (sparm_name_branch != 0) sparm_name();
}

	float &met_gen()
	{
		if (not met_gen_isLoaded) {
			if (met_gen_branch != 0) {
				met_gen_branch->GetEntry(index);
			} else { 
				printf("branch met_gen_branch does not exist!\n");
				exit(1);
			}
			met_gen_isLoaded = true;
		}
		return met_gen_;
	}
	float &ht_gen()
	{
		if (not ht_gen_isLoaded) {
			if (ht_gen_branch != 0) {
				ht_gen_branch->GetEntry(index);
			} else { 
				printf("branch ht_gen_branch does not exist!\n");
				exit(1);
			}
			ht_gen_isLoaded = true;
		}
		return ht_gen_;
	}
	float &ht_jets_gen()
	{
		if (not ht_jets_gen_isLoaded) {
			if (ht_jets_gen_branch != 0) {
				ht_jets_gen_branch->GetEntry(index);
			} else { 
				printf("branch ht_jets_gen_branch does not exist!\n");
				exit(1);
			}
			ht_jets_gen_isLoaded = true;
		}
		return ht_jets_gen_;
	}
	int &njets_gen()
	{
		if (not njets_gen_isLoaded) {
			if (njets_gen_branch != 0) {
				njets_gen_branch->GetEntry(index);
			} else { 
				printf("branch njets_gen_branch does not exist!\n");
				exit(1);
			}
			njets_gen_isLoaded = true;
		}
		return njets_gen_;
	}
	int &njets_jets_gen()
	{
		if (not njets_jets_gen_isLoaded) {
			if (njets_jets_gen_branch != 0) {
				njets_jets_gen_branch->GetEntry(index);
			} else { 
				printf("branch njets_jets_gen_branch does not exist!\n");
				exit(1);
			}
			njets_jets_gen_isLoaded = true;
		}
		return njets_jets_gen_;
	}
	int &nbtags_gen()
	{
		if (not nbtags_gen_isLoaded) {
			if (nbtags_gen_branch != 0) {
				nbtags_gen_branch->GetEntry(index);
			} else { 
				printf("branch nbtags_gen_branch does not exist!\n");
				exit(1);
			}
			nbtags_gen_isLoaded = true;
		}
		return nbtags_gen_;
	}
	int &nbtags_jets_gen()
	{
		if (not nbtags_jets_gen_isLoaded) {
			if (nbtags_jets_gen_branch != 0) {
				nbtags_jets_gen_branch->GetEntry(index);
			} else { 
				printf("branch nbtags_jets_gen_branch does not exist!\n");
				exit(1);
			}
			nbtags_jets_gen_isLoaded = true;
		}
		return nbtags_jets_gen_;
	}
	float &pt1_gen()
	{
		if (not pt1_gen_isLoaded) {
			if (pt1_gen_branch != 0) {
				pt1_gen_branch->GetEntry(index);
			} else { 
				printf("branch pt1_gen_branch does not exist!\n");
				exit(1);
			}
			pt1_gen_isLoaded = true;
		}
		return pt1_gen_;
	}
	float &pt2_gen()
	{
		if (not pt2_gen_isLoaded) {
			if (pt2_gen_branch != 0) {
				pt2_gen_branch->GetEntry(index);
			} else { 
				printf("branch pt2_gen_branch does not exist!\n");
				exit(1);
			}
			pt2_gen_isLoaded = true;
		}
		return pt2_gen_;
	}
	float &eta1_gen()
	{
		if (not eta1_gen_isLoaded) {
			if (eta1_gen_branch != 0) {
				eta1_gen_branch->GetEntry(index);
			} else { 
				printf("branch eta1_gen_branch does not exist!\n");
				exit(1);
			}
			eta1_gen_isLoaded = true;
		}
		return eta1_gen_;
	}
	float &eta2_gen()
	{
		if (not eta2_gen_isLoaded) {
			if (eta2_gen_branch != 0) {
				eta2_gen_branch->GetEntry(index);
			} else { 
				printf("branch eta2_gen_branch does not exist!\n");
				exit(1);
			}
			eta2_gen_isLoaded = true;
		}
		return eta2_gen_;
	}
	int &id1_gen()
	{
		if (not id1_gen_isLoaded) {
			if (id1_gen_branch != 0) {
				id1_gen_branch->GetEntry(index);
			} else { 
				printf("branch id1_gen_branch does not exist!\n");
				exit(1);
			}
			id1_gen_isLoaded = true;
		}
		return id1_gen_;
	}
	int &id2_gen()
	{
		if (not id2_gen_isLoaded) {
			if (id2_gen_branch != 0) {
				id2_gen_branch->GetEntry(index);
			} else { 
				printf("branch id2_gen_branch does not exist!\n");
				exit(1);
			}
			id2_gen_isLoaded = true;
		}
		return id2_gen_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_gen()
	{
		if (not jets_gen_isLoaded) {
			if (jets_gen_branch != 0) {
				jets_gen_branch->GetEntry(index);
			} else { 
				printf("branch jets_gen_branch does not exist!\n");
				exit(1);
			}
			jets_gen_isLoaded = true;
		}
		return *jets_gen_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_direct_gen()
	{
		if (not jets_direct_gen_isLoaded) {
			if (jets_direct_gen_branch != 0) {
				jets_direct_gen_branch->GetEntry(index);
			} else { 
				printf("branch jets_direct_gen_branch does not exist!\n");
				exit(1);
			}
			jets_direct_gen_isLoaded = true;
		}
		return *jets_direct_gen_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_gen()
	{
		if (not btags_gen_isLoaded) {
			if (btags_gen_branch != 0) {
				btags_gen_branch->GetEntry(index);
			} else { 
				printf("branch btags_gen_branch does not exist!\n");
				exit(1);
			}
			btags_gen_isLoaded = true;
		}
		return *btags_gen_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_direct_gen()
	{
		if (not btags_direct_gen_isLoaded) {
			if (btags_direct_gen_branch != 0) {
				btags_direct_gen_branch->GetEntry(index);
			} else { 
				printf("branch btags_direct_gen_branch does not exist!\n");
				exit(1);
			}
			btags_direct_gen_isLoaded = true;
		}
		return *btags_direct_gen_;
	}
	bool &	choseRightHyp()
	{
		if (not choseRightHyp_isLoaded) {
			if (choseRightHyp_branch != 0) {
				choseRightHyp_branch->GetEntry(index);
			} else { 
				printf("branch choseRightHyp_branch does not exist!\n");
				exit(1);
			}
			choseRightHyp_isLoaded = true;
		}
		return choseRightHyp_;
	}
	int &id1_reco()
	{
		if (not id1_reco_isLoaded) {
			if (id1_reco_branch != 0) {
				id1_reco_branch->GetEntry(index);
			} else { 
				printf("branch id1_reco_branch does not exist!\n");
				exit(1);
			}
			id1_reco_isLoaded = true;
		}
		return id1_reco_;
	}
	int &id2_reco()
	{
		if (not id2_reco_isLoaded) {
			if (id2_reco_branch != 0) {
				id2_reco_branch->GetEntry(index);
			} else { 
				printf("branch id2_reco_branch does not exist!\n");
				exit(1);
			}
			id2_reco_isLoaded = true;
		}
		return id2_reco_;
	}
	float &pt1_reco()
	{
		if (not pt1_reco_isLoaded) {
			if (pt1_reco_branch != 0) {
				pt1_reco_branch->GetEntry(index);
			} else { 
				printf("branch pt1_reco_branch does not exist!\n");
				exit(1);
			}
			pt1_reco_isLoaded = true;
		}
		return pt1_reco_;
	}
	float &pt2_reco()
	{
		if (not pt2_reco_isLoaded) {
			if (pt2_reco_branch != 0) {
				pt2_reco_branch->GetEntry(index);
			} else { 
				printf("branch pt2_reco_branch does not exist!\n");
				exit(1);
			}
			pt2_reco_isLoaded = true;
		}
		return pt2_reco_;
	}
	float &eta1_reco()
	{
		if (not eta1_reco_isLoaded) {
			if (eta1_reco_branch != 0) {
				eta1_reco_branch->GetEntry(index);
			} else { 
				printf("branch eta1_reco_branch does not exist!\n");
				exit(1);
			}
			eta1_reco_isLoaded = true;
		}
		return eta1_reco_;
	}
	float &eta2_reco()
	{
		if (not eta2_reco_isLoaded) {
			if (eta2_reco_branch != 0) {
				eta2_reco_branch->GetEntry(index);
			} else { 
				printf("branch eta2_reco_branch does not exist!\n");
				exit(1);
			}
			eta2_reco_isLoaded = true;
		}
		return eta2_reco_;
	}
	float &met_reco()
	{
		if (not met_reco_isLoaded) {
			if (met_reco_branch != 0) {
				met_reco_branch->GetEntry(index);
			} else { 
				printf("branch met_reco_branch does not exist!\n");
				exit(1);
			}
			met_reco_isLoaded = true;
		}
		return met_reco_;
	}
	float &ht_reco()
	{
		if (not ht_reco_isLoaded) {
			if (ht_reco_branch != 0) {
				ht_reco_branch->GetEntry(index);
			} else { 
				printf("branch ht_reco_branch does not exist!\n");
				exit(1);
			}
			ht_reco_isLoaded = true;
		}
		return ht_reco_;
	}
	float &ht_direct_reco()
	{
		if (not ht_direct_reco_isLoaded) {
			if (ht_direct_reco_branch != 0) {
				ht_direct_reco_branch->GetEntry(index);
			} else { 
				printf("branch ht_direct_reco_branch does not exist!\n");
				exit(1);
			}
			ht_direct_reco_isLoaded = true;
		}
		return ht_direct_reco_;
	}
	bool &	isGoodLepton_1()
	{
		if (not isGoodLepton_1_isLoaded) {
			if (isGoodLepton_1_branch != 0) {
				isGoodLepton_1_branch->GetEntry(index);
			} else { 
				printf("branch isGoodLepton_1_branch does not exist!\n");
				exit(1);
			}
			isGoodLepton_1_isLoaded = true;
		}
		return isGoodLepton_1_;
	}
	bool &	isIsolatedLepton_1()
	{
		if (not isIsolatedLepton_1_isLoaded) {
			if (isIsolatedLepton_1_branch != 0) {
				isIsolatedLepton_1_branch->GetEntry(index);
			} else { 
				printf("branch isIsolatedLepton_1_branch does not exist!\n");
				exit(1);
			}
			isIsolatedLepton_1_isLoaded = true;
		}
		return isIsolatedLepton_1_;
	}
	bool &	isNumeratorLepton_1()
	{
		if (not isNumeratorLepton_1_isLoaded) {
			if (isNumeratorLepton_1_branch != 0) {
				isNumeratorLepton_1_branch->GetEntry(index);
			} else { 
				printf("branch isNumeratorLepton_1_branch does not exist!\n");
				exit(1);
			}
			isNumeratorLepton_1_isLoaded = true;
		}
		return isNumeratorLepton_1_;
	}
	bool &	isGoodLepton_2()
	{
		if (not isGoodLepton_2_isLoaded) {
			if (isGoodLepton_2_branch != 0) {
				isGoodLepton_2_branch->GetEntry(index);
			} else { 
				printf("branch isGoodLepton_2_branch does not exist!\n");
				exit(1);
			}
			isGoodLepton_2_isLoaded = true;
		}
		return isGoodLepton_2_;
	}
	bool &	isIsolatedLepton_2()
	{
		if (not isIsolatedLepton_2_isLoaded) {
			if (isIsolatedLepton_2_branch != 0) {
				isIsolatedLepton_2_branch->GetEntry(index);
			} else { 
				printf("branch isIsolatedLepton_2_branch does not exist!\n");
				exit(1);
			}
			isIsolatedLepton_2_isLoaded = true;
		}
		return isIsolatedLepton_2_;
	}
	bool &	isNumeratorLepton_2()
	{
		if (not isNumeratorLepton_2_isLoaded) {
			if (isNumeratorLepton_2_branch != 0) {
				isNumeratorLepton_2_branch->GetEntry(index);
			} else { 
				printf("branch isNumeratorLepton_2_branch does not exist!\n");
				exit(1);
			}
			isNumeratorLepton_2_isLoaded = true;
		}
		return isNumeratorLepton_2_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco()
	{
		if (not jets_reco_isLoaded) {
			if (jets_reco_branch != 0) {
				jets_reco_branch->GetEntry(index);
			} else { 
				printf("branch jets_reco_branch does not exist!\n");
				exit(1);
			}
			jets_reco_isLoaded = true;
		}
		return *jets_reco_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco_direct()
	{
		if (not jets_reco_direct_isLoaded) {
			if (jets_reco_direct_branch != 0) {
				jets_reco_direct_branch->GetEntry(index);
			} else { 
				printf("branch jets_reco_direct_branch does not exist!\n");
				exit(1);
			}
			jets_reco_direct_isLoaded = true;
		}
		return *jets_reco_direct_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco_cms2()
	{
		if (not jets_reco_cms2_isLoaded) {
			if (jets_reco_cms2_branch != 0) {
				jets_reco_cms2_branch->GetEntry(index);
			} else { 
				printf("branch jets_reco_cms2_branch does not exist!\n");
				exit(1);
			}
			jets_reco_cms2_isLoaded = true;
		}
		return *jets_reco_cms2_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_reco()
	{
		if (not btags_reco_isLoaded) {
			if (btags_reco_branch != 0) {
				btags_reco_branch->GetEntry(index);
			} else { 
				printf("branch btags_reco_branch does not exist!\n");
				exit(1);
			}
			btags_reco_isLoaded = true;
		}
		return *btags_reco_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_jets_reco()
	{
		if (not btags_jets_reco_isLoaded) {
			if (btags_jets_reco_branch != 0) {
				btags_jets_reco_branch->GetEntry(index);
			} else { 
				printf("branch btags_jets_reco_branch does not exist!\n");
				exit(1);
			}
			btags_jets_reco_isLoaded = true;
		}
		return *btags_jets_reco_;
	}
	int &njets_reco()
	{
		if (not njets_reco_isLoaded) {
			if (njets_reco_branch != 0) {
				njets_reco_branch->GetEntry(index);
			} else { 
				printf("branch njets_reco_branch does not exist!\n");
				exit(1);
			}
			njets_reco_isLoaded = true;
		}
		return njets_reco_;
	}
	int &njets_reco_direct()
	{
		if (not njets_reco_direct_isLoaded) {
			if (njets_reco_direct_branch != 0) {
				njets_reco_direct_branch->GetEntry(index);
			} else { 
				printf("branch njets_reco_direct_branch does not exist!\n");
				exit(1);
			}
			njets_reco_direct_isLoaded = true;
		}
		return njets_reco_direct_;
	}
	int &njets_reco_cms2()
	{
		if (not njets_reco_cms2_isLoaded) {
			if (njets_reco_cms2_branch != 0) {
				njets_reco_cms2_branch->GetEntry(index);
			} else { 
				printf("branch njets_reco_cms2_branch does not exist!\n");
				exit(1);
			}
			njets_reco_cms2_isLoaded = true;
		}
		return njets_reco_cms2_;
	}
	int &nbtags_reco()
	{
		if (not nbtags_reco_isLoaded) {
			if (nbtags_reco_branch != 0) {
				nbtags_reco_branch->GetEntry(index);
			} else { 
				printf("branch nbtags_reco_branch does not exist!\n");
				exit(1);
			}
			nbtags_reco_isLoaded = true;
		}
		return nbtags_reco_;
	}
	int &nbtags_jets_reco()
	{
		if (not nbtags_jets_reco_isLoaded) {
			if (nbtags_jets_reco_branch != 0) {
				nbtags_jets_reco_branch->GetEntry(index);
			} else { 
				printf("branch nbtags_jets_reco_branch does not exist!\n");
				exit(1);
			}
			nbtags_jets_reco_isLoaded = true;
		}
		return nbtags_jets_reco_;
	}
	const vector<bool> &gen_btag_is_matched()
	{
		if (not gen_btag_is_matched_isLoaded) {
			if (gen_btag_is_matched_branch != 0) {
				gen_btag_is_matched_branch->GetEntry(index);
			} else { 
				printf("branch gen_btag_is_matched_branch does not exist!\n");
				exit(1);
			}
			gen_btag_is_matched_isLoaded = true;
		}
		return *gen_btag_is_matched_;
	}
	const vector<bool> &gen_btag_is_matched_direct()
	{
		if (not gen_btag_is_matched_direct_isLoaded) {
			if (gen_btag_is_matched_direct_branch != 0) {
				gen_btag_is_matched_direct_branch->GetEntry(index);
			} else { 
				printf("branch gen_btag_is_matched_direct_branch does not exist!\n");
				exit(1);
			}
			gen_btag_is_matched_direct_isLoaded = true;
		}
		return *gen_btag_is_matched_direct_;
	}
	const vector<bool> &jet_is_matched()
	{
		if (not jet_is_matched_isLoaded) {
			if (jet_is_matched_branch != 0) {
				jet_is_matched_branch->GetEntry(index);
			} else { 
				printf("branch jet_is_matched_branch does not exist!\n");
				exit(1);
			}
			jet_is_matched_isLoaded = true;
		}
		return *jet_is_matched_;
	}
	const vector<bool> &jet_is_matched_direct()
	{
		if (not jet_is_matched_direct_isLoaded) {
			if (jet_is_matched_direct_branch != 0) {
				jet_is_matched_direct_branch->GetEntry(index);
			} else { 
				printf("branch jet_is_matched_direct_branch does not exist!\n");
				exit(1);
			}
			jet_is_matched_direct_isLoaded = true;
		}
		return *jet_is_matched_direct_;
	}
	const vector<float> &sparm_value()
	{
		if (not sparm_value_isLoaded) {
			if (sparm_value_branch != 0) {
				sparm_value_branch->GetEntry(index);
			} else { 
				printf("branch sparm_value_branch does not exist!\n");
				exit(1);
			}
			sparm_value_isLoaded = true;
		}
		return *sparm_value_;
	}
	const vector<TString> &sparm_name()
	{
		if (not sparm_name_isLoaded) {
			if (sparm_name_branch != 0) {
				sparm_name_branch->GetEntry(index);
			} else { 
				printf("branch sparm_name_branch does not exist!\n");
				exit(1);
			}
			sparm_name_isLoaded = true;
		}
		return *sparm_name_;
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
extern SS samesign;
#endif

namespace ss {
	const float &met_gen();
	const float &ht_gen();
	const float &ht_jets_gen();
	const int &njets_gen();
	const int &njets_jets_gen();
	const int &nbtags_gen();
	const int &nbtags_jets_gen();
	const float &pt1_gen();
	const float &pt2_gen();
	const float &eta1_gen();
	const float &eta2_gen();
	const int &id1_gen();
	const int &id2_gen();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_gen();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_direct_gen();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_gen();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_direct_gen();
	const bool &choseRightHyp();
	const int &id1_reco();
	const int &id2_reco();
	const float &pt1_reco();
	const float &pt2_reco();
	const float &eta1_reco();
	const float &eta2_reco();
	const float &met_reco();
	const float &ht_reco();
	const float &ht_direct_reco();
	const bool &isGoodLepton_1();
	const bool &isIsolatedLepton_1();
	const bool &isNumeratorLepton_1();
	const bool &isGoodLepton_2();
	const bool &isIsolatedLepton_2();
	const bool &isNumeratorLepton_2();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco_direct();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_reco_cms2();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_reco();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &btags_jets_reco();
	const int &njets_reco();
	const int &njets_reco_direct();
	const int &njets_reco_cms2();
	const int &nbtags_reco();
	const int &nbtags_jets_reco();
	const vector<bool> &gen_btag_is_matched();
	const vector<bool> &gen_btag_is_matched_direct();
	const vector<bool> &jet_is_matched();
	const vector<bool> &jet_is_matched_direct();
	const vector<float> &sparm_value();
	const vector<TString> &sparm_name();
}
#endif
