// -*- C++ -*-
#ifndef SYST_H
#define SYST_H
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
class SYST {
private: 
protected: 
	unsigned int index;
	vector<string> weightsID_;
	TBranch *weightsID_branch;
	bool weightsID_isLoaded;
	bool keep_;
	TBranch *keep_branch;
	bool keep_isLoaded;
	double qScale_;
	TBranch *qScale_branch;
	bool qScale_isLoaded;
	float ht_;
	TBranch *ht_branch;
	bool ht_isLoaded;
	float met_;
	TBranch *met_branch;
	bool met_isLoaded;
	float metphi_;
	TBranch *metphi_branch;
	bool metphi_isLoaded;
	float iso1_;
	TBranch *iso1_branch;
	bool iso1_isLoaded;
	float iso2_;
	TBranch *iso2_branch;
	bool iso2_isLoaded;
	float weight_;
	TBranch *weight_branch;
	bool weight_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >  lep1_;
	TBranch *lep1_branch;
	bool lep1_isLoaded;
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >  lep2_;
	TBranch *lep2_branch;
	bool lep2_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > genBs_;
	TBranch *genBs_branch;
	bool genBs_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > genjet_;
	TBranch *genjet_branch;
	bool genjet_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > genps_;
	TBranch *genps_branch;
	bool genps_isLoaded;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > gen_mother_p4_;
	TBranch *gen_mother_p4_branch;
	bool gen_mother_p4_isLoaded;
	vector<float> jet_eta_;
	TBranch *jet_eta_branch;
	bool jet_eta_isLoaded;
	vector<float> jet_phi_;
	TBranch *jet_phi_branch;
	bool jet_phi_isLoaded;
	vector<float> jet_pt_;
	TBranch *jet_pt_branch;
	bool jet_pt_isLoaded;
	vector<float> gen_eta_;
	TBranch *gen_eta_branch;
	bool gen_eta_isLoaded;
	vector<float> gen_phi_;
	TBranch *gen_phi_branch;
	bool gen_phi_isLoaded;
	vector<float> gen_pt_;
	TBranch *gen_pt_branch;
	bool gen_pt_isLoaded;
	vector<float> weights_;
	TBranch *weights_branch;
	bool weights_isLoaded;
	int gen_gmother_id_;
	TBranch *gen_gmother_id_branch;
	bool gen_gmother_id_isLoaded;
	int gramid1_;
	TBranch *gramid1_branch;
	bool gramid1_isLoaded;
	int id1_;
	TBranch *id1_branch;
	bool id1_isLoaded;
	int gramid2_;
	TBranch *gramid2_branch;
	bool gramid2_isLoaded;
	int id2_;
	TBranch *id2_branch;
	bool id2_isLoaded;
	int nBtags_;
	TBranch *nBtags_branch;
	bool nBtags_isLoaded;
	int nJets40_;
	TBranch *nJets40_branch;
	bool nJets40_isLoaded;
	int nLep_;
	TBranch *nLep_branch;
	bool nLep_isLoaded;
	vector<int> genBIDs_;
	TBranch *genBIDs_branch;
	bool genBIDs_isLoaded;
	vector<int> gen_id_;
	TBranch *gen_id_branch;
	bool gen_id_isLoaded;
	vector<int> gen_matchtojet_;
	TBranch *gen_matchtojet_branch;
	bool gen_matchtojet_isLoaded;
	vector<int> gen_mother_id_;
	TBranch *gen_mother_id_branch;
	bool gen_mother_id_isLoaded;
	vector<int> gen_status_;
	TBranch *gen_status_branch;
	bool gen_status_isLoaded;
public: 
void Init(TTree *tree) {
	lep1_branch = 0;
	if (tree->GetAlias("lep1") != 0) {
		lep1_branch = tree->GetBranch(tree->GetAlias("lep1"));
		if (lep1_branch) {lep1_branch->SetAddress(&lep1_);}
	}
	lep2_branch = 0;
	if (tree->GetAlias("lep2") != 0) {
		lep2_branch = tree->GetBranch(tree->GetAlias("lep2"));
		if (lep2_branch) {lep2_branch->SetAddress(&lep2_);}
	}
	genBs_branch = 0;
	if (tree->GetAlias("genBs") != 0) {
		genBs_branch = tree->GetBranch(tree->GetAlias("genBs"));
		if (genBs_branch) {genBs_branch->SetAddress(&genBs_);}
	}
	genjet_branch = 0;
	if (tree->GetAlias("genjet") != 0) {
		genjet_branch = tree->GetBranch(tree->GetAlias("genjet"));
		if (genjet_branch) {genjet_branch->SetAddress(&genjet_);}
	}
	genps_branch = 0;
	if (tree->GetAlias("genps") != 0) {
		genps_branch = tree->GetBranch(tree->GetAlias("genps"));
		if (genps_branch) {genps_branch->SetAddress(&genps_);}
	}
	gen_mother_p4_branch = 0;
	if (tree->GetAlias("gen_mother_p4") != 0) {
		gen_mother_p4_branch = tree->GetBranch(tree->GetAlias("gen_mother_p4"));
		if (gen_mother_p4_branch) {gen_mother_p4_branch->SetAddress(&gen_mother_p4_);}
	}
  tree->SetMakeClass(1);
	weightsID_branch = 0;
	if (tree->GetAlias("weightsID") != 0) {
		weightsID_branch = tree->GetBranch(tree->GetAlias("weightsID"));
		if (weightsID_branch) {weightsID_branch->SetAddress(&weightsID_);}
	}
	keep_branch = 0;
	if (tree->GetAlias("keep") != 0) {
		keep_branch = tree->GetBranch(tree->GetAlias("keep"));
		if (keep_branch) {keep_branch->SetAddress(&keep_);}
	}
	qScale_branch = 0;
	if (tree->GetAlias("qScale") != 0) {
		qScale_branch = tree->GetBranch(tree->GetAlias("qScale"));
		if (qScale_branch) {qScale_branch->SetAddress(&qScale_);}
	}
	ht_branch = 0;
	if (tree->GetAlias("ht") != 0) {
		ht_branch = tree->GetBranch(tree->GetAlias("ht"));
		if (ht_branch) {ht_branch->SetAddress(&ht_);}
	}
	met_branch = 0;
	if (tree->GetAlias("met") != 0) {
		met_branch = tree->GetBranch(tree->GetAlias("met"));
		if (met_branch) {met_branch->SetAddress(&met_);}
	}
	metphi_branch = 0;
	if (tree->GetAlias("metphi") != 0) {
		metphi_branch = tree->GetBranch(tree->GetAlias("metphi"));
		if (metphi_branch) {metphi_branch->SetAddress(&metphi_);}
	}
	iso1_branch = 0;
	if (tree->GetAlias("iso1") != 0) {
		iso1_branch = tree->GetBranch(tree->GetAlias("iso1"));
		if (iso1_branch) {iso1_branch->SetAddress(&iso1_);}
	}
	iso2_branch = 0;
	if (tree->GetAlias("iso2") != 0) {
		iso2_branch = tree->GetBranch(tree->GetAlias("iso2"));
		if (iso2_branch) {iso2_branch->SetAddress(&iso2_);}
	}
	weight_branch = 0;
	if (tree->GetAlias("weight") != 0) {
		weight_branch = tree->GetBranch(tree->GetAlias("weight"));
		if (weight_branch) {weight_branch->SetAddress(&weight_);}
	}
	jet_eta_branch = 0;
	if (tree->GetAlias("jet_eta") != 0) {
		jet_eta_branch = tree->GetBranch(tree->GetAlias("jet_eta"));
		if (jet_eta_branch) {jet_eta_branch->SetAddress(&jet_eta_);}
	}
	jet_phi_branch = 0;
	if (tree->GetAlias("jet_phi") != 0) {
		jet_phi_branch = tree->GetBranch(tree->GetAlias("jet_phi"));
		if (jet_phi_branch) {jet_phi_branch->SetAddress(&jet_phi_);}
	}
	jet_pt_branch = 0;
	if (tree->GetAlias("jet_pt") != 0) {
		jet_pt_branch = tree->GetBranch(tree->GetAlias("jet_pt"));
		if (jet_pt_branch) {jet_pt_branch->SetAddress(&jet_pt_);}
	}
	gen_eta_branch = 0;
	if (tree->GetAlias("gen_eta") != 0) {
		gen_eta_branch = tree->GetBranch(tree->GetAlias("gen_eta"));
		if (gen_eta_branch) {gen_eta_branch->SetAddress(&gen_eta_);}
	}
	gen_phi_branch = 0;
	if (tree->GetAlias("gen_phi") != 0) {
		gen_phi_branch = tree->GetBranch(tree->GetAlias("gen_phi"));
		if (gen_phi_branch) {gen_phi_branch->SetAddress(&gen_phi_);}
	}
	gen_pt_branch = 0;
	if (tree->GetAlias("gen_pt") != 0) {
		gen_pt_branch = tree->GetBranch(tree->GetAlias("gen_pt"));
		if (gen_pt_branch) {gen_pt_branch->SetAddress(&gen_pt_);}
	}
	weights_branch = 0;
	if (tree->GetAlias("weights") != 0) {
		weights_branch = tree->GetBranch(tree->GetAlias("weights"));
		if (weights_branch) {weights_branch->SetAddress(&weights_);}
	}
	gen_gmother_id_branch = 0;
	if (tree->GetAlias("gen_gmother_id") != 0) {
		gen_gmother_id_branch = tree->GetBranch(tree->GetAlias("gen_gmother_id"));
		if (gen_gmother_id_branch) {gen_gmother_id_branch->SetAddress(&gen_gmother_id_);}
	}
	gramid1_branch = 0;
	if (tree->GetAlias("gramid1") != 0) {
		gramid1_branch = tree->GetBranch(tree->GetAlias("gramid1"));
		if (gramid1_branch) {gramid1_branch->SetAddress(&gramid1_);}
	}
	id1_branch = 0;
	if (tree->GetAlias("id1") != 0) {
		id1_branch = tree->GetBranch(tree->GetAlias("id1"));
		if (id1_branch) {id1_branch->SetAddress(&id1_);}
	}
	gramid2_branch = 0;
	if (tree->GetAlias("gramid2") != 0) {
		gramid2_branch = tree->GetBranch(tree->GetAlias("gramid2"));
		if (gramid2_branch) {gramid2_branch->SetAddress(&gramid2_);}
	}
	id2_branch = 0;
	if (tree->GetAlias("id2") != 0) {
		id2_branch = tree->GetBranch(tree->GetAlias("id2"));
		if (id2_branch) {id2_branch->SetAddress(&id2_);}
	}
	nBtags_branch = 0;
	if (tree->GetAlias("nBtags") != 0) {
		nBtags_branch = tree->GetBranch(tree->GetAlias("nBtags"));
		if (nBtags_branch) {nBtags_branch->SetAddress(&nBtags_);}
	}
	nJets40_branch = 0;
	if (tree->GetAlias("nJets40") != 0) {
		nJets40_branch = tree->GetBranch(tree->GetAlias("nJets40"));
		if (nJets40_branch) {nJets40_branch->SetAddress(&nJets40_);}
	}
	nLep_branch = 0;
	if (tree->GetAlias("nLep") != 0) {
		nLep_branch = tree->GetBranch(tree->GetAlias("nLep"));
		if (nLep_branch) {nLep_branch->SetAddress(&nLep_);}
	}
	genBIDs_branch = 0;
	if (tree->GetAlias("genBIDs") != 0) {
		genBIDs_branch = tree->GetBranch(tree->GetAlias("genBIDs"));
		if (genBIDs_branch) {genBIDs_branch->SetAddress(&genBIDs_);}
	}
	gen_id_branch = 0;
	if (tree->GetAlias("gen_id") != 0) {
		gen_id_branch = tree->GetBranch(tree->GetAlias("gen_id"));
		if (gen_id_branch) {gen_id_branch->SetAddress(&gen_id_);}
	}
	gen_matchtojet_branch = 0;
	if (tree->GetAlias("gen_matchtojet") != 0) {
		gen_matchtojet_branch = tree->GetBranch(tree->GetAlias("gen_matchtojet"));
		if (gen_matchtojet_branch) {gen_matchtojet_branch->SetAddress(&gen_matchtojet_);}
	}
	gen_mother_id_branch = 0;
	if (tree->GetAlias("gen_mother_id") != 0) {
		gen_mother_id_branch = tree->GetBranch(tree->GetAlias("gen_mother_id"));
		if (gen_mother_id_branch) {gen_mother_id_branch->SetAddress(&gen_mother_id_);}
	}
	gen_status_branch = 0;
	if (tree->GetAlias("gen_status") != 0) {
		gen_status_branch = tree->GetBranch(tree->GetAlias("gen_status"));
		if (gen_status_branch) {gen_status_branch->SetAddress(&gen_status_);}
	}
  tree->SetMakeClass(0);
}
void GetEntry(unsigned int idx) 
	// this only marks branches as not loaded, saving a lot of time
	{
		index = idx;
		weightsID_isLoaded = false;
		keep_isLoaded = false;
		qScale_isLoaded = false;
		ht_isLoaded = false;
		met_isLoaded = false;
		metphi_isLoaded = false;
		iso1_isLoaded = false;
		iso2_isLoaded = false;
		weight_isLoaded = false;
		lep1_isLoaded = false;
		lep2_isLoaded = false;
		genBs_isLoaded = false;
		genjet_isLoaded = false;
		genps_isLoaded = false;
		gen_mother_p4_isLoaded = false;
		jet_eta_isLoaded = false;
		jet_phi_isLoaded = false;
		jet_pt_isLoaded = false;
		gen_eta_isLoaded = false;
		gen_phi_isLoaded = false;
		gen_pt_isLoaded = false;
		weights_isLoaded = false;
		gen_gmother_id_isLoaded = false;
		gramid1_isLoaded = false;
		id1_isLoaded = false;
		gramid2_isLoaded = false;
		id2_isLoaded = false;
		nBtags_isLoaded = false;
		nJets40_isLoaded = false;
		nLep_isLoaded = false;
		genBIDs_isLoaded = false;
		gen_id_isLoaded = false;
		gen_matchtojet_isLoaded = false;
		gen_mother_id_isLoaded = false;
		gen_status_isLoaded = false;
	}

void LoadAllBranches() 
	// load all branches
{
	if (weightsID_branch != 0) weightsID();
	if (keep_branch != 0) keep();
	if (qScale_branch != 0) qScale();
	if (ht_branch != 0) ht();
	if (met_branch != 0) met();
	if (metphi_branch != 0) metphi();
	if (iso1_branch != 0) iso1();
	if (iso2_branch != 0) iso2();
	if (weight_branch != 0) weight();
	if (lep1_branch != 0) lep1();
	if (lep2_branch != 0) lep2();
	if (genBs_branch != 0) genBs();
	if (genjet_branch != 0) genjet();
	if (genps_branch != 0) genps();
	if (gen_mother_p4_branch != 0) gen_mother_p4();
	if (jet_eta_branch != 0) jet_eta();
	if (jet_phi_branch != 0) jet_phi();
	if (jet_pt_branch != 0) jet_pt();
	if (gen_eta_branch != 0) gen_eta();
	if (gen_phi_branch != 0) gen_phi();
	if (gen_pt_branch != 0) gen_pt();
	if (weights_branch != 0) weights();
	if (gen_gmother_id_branch != 0) gen_gmother_id();
	if (gramid1_branch != 0) gramid1();
	if (id1_branch != 0) id1();
	if (gramid2_branch != 0) gramid2();
	if (id2_branch != 0) id2();
	if (nBtags_branch != 0) nBtags();
	if (nJets40_branch != 0) nJets40();
	if (nLep_branch != 0) nLep();
	if (genBIDs_branch != 0) genBIDs();
	if (gen_id_branch != 0) gen_id();
	if (gen_matchtojet_branch != 0) gen_matchtojet();
	if (gen_mother_id_branch != 0) gen_mother_id();
	if (gen_status_branch != 0) gen_status();
}

	const vector<string> &weightsID()
	{
		if (not weightsID_isLoaded) {
			if (weightsID_branch != 0) {
				weightsID_branch->GetEntry(index);
			} else { 
				printf("branch weightsID_branch does not exist!\n");
				exit(1);
			}
			weightsID_isLoaded = true;
		}
		return weightsID_;
	}
	bool &keep()
	{
		if (not keep_isLoaded) {
			if (keep_branch != 0) {
				keep_branch->GetEntry(index);
			} else { 
				printf("branch keep_branch does not exist!\n");
				exit(1);
			}
			keep_isLoaded = true;
		}
		return keep_;
	}
	double &qScale()
	{
		if (not qScale_isLoaded) {
			if (qScale_branch != 0) {
				qScale_branch->GetEntry(index);
			} else { 
				printf("branch qScale_branch does not exist!\n");
				exit(1);
			}
			qScale_isLoaded = true;
		}
		return qScale_;
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
	float &metphi()
	{
		if (not metphi_isLoaded) {
			if (metphi_branch != 0) {
				metphi_branch->GetEntry(index);
			} else { 
				printf("branch metphi_branch does not exist!\n");
				exit(1);
			}
			metphi_isLoaded = true;
		}
		return metphi_;
	}
	float &iso1()
	{
		if (not iso1_isLoaded) {
			if (iso1_branch != 0) {
				iso1_branch->GetEntry(index);
			} else { 
				printf("branch iso1_branch does not exist!\n");
				exit(1);
			}
			iso1_isLoaded = true;
		}
		return iso1_;
	}
	float &iso2()
	{
		if (not iso2_isLoaded) {
			if (iso2_branch != 0) {
				iso2_branch->GetEntry(index);
			} else { 
				printf("branch iso2_branch does not exist!\n");
				exit(1);
			}
			iso2_isLoaded = true;
		}
		return iso2_;
	}
	float &weight()
	{
		if (not weight_isLoaded) {
			if (weight_branch != 0) {
				weight_branch->GetEntry(index);
			} else { 
				printf("branch weight_branch does not exist!\n");
				exit(1);
			}
			weight_isLoaded = true;
		}
		return weight_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >  &lep1()
	{
		if (not lep1_isLoaded) {
			if (lep1_branch != 0) {
				lep1_branch->GetEntry(index);
			} else { 
				printf("branch lep1_branch does not exist!\n");
				exit(1);
			}
			lep1_isLoaded = true;
		}
		return lep1_;
	}
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >  &lep2()
	{
		if (not lep2_isLoaded) {
			if (lep2_branch != 0) {
				lep2_branch->GetEntry(index);
			} else { 
				printf("branch lep2_branch does not exist!\n");
				exit(1);
			}
			lep2_isLoaded = true;
		}
		return lep2_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genBs()
	{
		if (not genBs_isLoaded) {
			if (genBs_branch != 0) {
				genBs_branch->GetEntry(index);
			} else { 
				printf("branch genBs_branch does not exist!\n");
				exit(1);
			}
			genBs_isLoaded = true;
		}
		return genBs_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genjet()
	{
		if (not genjet_isLoaded) {
			if (genjet_branch != 0) {
				genjet_branch->GetEntry(index);
			} else { 
				printf("branch genjet_branch does not exist!\n");
				exit(1);
			}
			genjet_isLoaded = true;
		}
		return genjet_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps()
	{
		if (not genps_isLoaded) {
			if (genps_branch != 0) {
				genps_branch->GetEntry(index);
			} else { 
				printf("branch genps_branch does not exist!\n");
				exit(1);
			}
			genps_isLoaded = true;
		}
		return genps_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_mother_p4()
	{
		if (not gen_mother_p4_isLoaded) {
			if (gen_mother_p4_branch != 0) {
				gen_mother_p4_branch->GetEntry(index);
			} else { 
				printf("branch gen_mother_p4_branch does not exist!\n");
				exit(1);
			}
			gen_mother_p4_isLoaded = true;
		}
		return gen_mother_p4_;
	}
	const vector<float> &jet_eta()
	{
		if (not jet_eta_isLoaded) {
			if (jet_eta_branch != 0) {
				jet_eta_branch->GetEntry(index);
			} else { 
				printf("branch jet_eta_branch does not exist!\n");
				exit(1);
			}
			jet_eta_isLoaded = true;
		}
		return jet_eta_;
	}
	const vector<float> &jet_phi()
	{
		if (not jet_phi_isLoaded) {
			if (jet_phi_branch != 0) {
				jet_phi_branch->GetEntry(index);
			} else { 
				printf("branch jet_phi_branch does not exist!\n");
				exit(1);
			}
			jet_phi_isLoaded = true;
		}
		return jet_phi_;
	}
	const vector<float> &jet_pt()
	{
		if (not jet_pt_isLoaded) {
			if (jet_pt_branch != 0) {
				jet_pt_branch->GetEntry(index);
			} else { 
				printf("branch jet_pt_branch does not exist!\n");
				exit(1);
			}
			jet_pt_isLoaded = true;
		}
		return jet_pt_;
	}
	const vector<float> &gen_eta()
	{
		if (not gen_eta_isLoaded) {
			if (gen_eta_branch != 0) {
				gen_eta_branch->GetEntry(index);
			} else { 
				printf("branch gen_eta_branch does not exist!\n");
				exit(1);
			}
			gen_eta_isLoaded = true;
		}
		return gen_eta_;
	}
	const vector<float> &gen_phi()
	{
		if (not gen_phi_isLoaded) {
			if (gen_phi_branch != 0) {
				gen_phi_branch->GetEntry(index);
			} else { 
				printf("branch gen_phi_branch does not exist!\n");
				exit(1);
			}
			gen_phi_isLoaded = true;
		}
		return gen_phi_;
	}
	const vector<float> &gen_pt()
	{
		if (not gen_pt_isLoaded) {
			if (gen_pt_branch != 0) {
				gen_pt_branch->GetEntry(index);
			} else { 
				printf("branch gen_pt_branch does not exist!\n");
				exit(1);
			}
			gen_pt_isLoaded = true;
		}
		return gen_pt_;
	}
	const vector<float> &weights()
	{
		if (not weights_isLoaded) {
			if (weights_branch != 0) {
				weights_branch->GetEntry(index);
			} else { 
				printf("branch weights_branch does not exist!\n");
				exit(1);
			}
			weights_isLoaded = true;
		}
		return weights_;
	}
	int &gen_gmother_id()
	{
		if (not gen_gmother_id_isLoaded) {
			if (gen_gmother_id_branch != 0) {
				gen_gmother_id_branch->GetEntry(index);
			} else { 
				printf("branch gen_gmother_id_branch does not exist!\n");
				exit(1);
			}
			gen_gmother_id_isLoaded = true;
		}
		return gen_gmother_id_;
	}
	int &gramid1()
	{
		if (not gramid1_isLoaded) {
			if (gramid1_branch != 0) {
				gramid1_branch->GetEntry(index);
			} else { 
				printf("branch gramid1_branch does not exist!\n");
				exit(1);
			}
			gramid1_isLoaded = true;
		}
		return gramid1_;
	}
	int &id1()
	{
		if (not id1_isLoaded) {
			if (id1_branch != 0) {
				id1_branch->GetEntry(index);
			} else { 
				printf("branch id1_branch does not exist!\n");
				exit(1);
			}
			id1_isLoaded = true;
		}
		return id1_;
	}
	int &gramid2()
	{
		if (not gramid2_isLoaded) {
			if (gramid2_branch != 0) {
				gramid2_branch->GetEntry(index);
			} else { 
				printf("branch gramid2_branch does not exist!\n");
				exit(1);
			}
			gramid2_isLoaded = true;
		}
		return gramid2_;
	}
	int &id2()
	{
		if (not id2_isLoaded) {
			if (id2_branch != 0) {
				id2_branch->GetEntry(index);
			} else { 
				printf("branch id2_branch does not exist!\n");
				exit(1);
			}
			id2_isLoaded = true;
		}
		return id2_;
	}
	int &nBtags()
	{
		if (not nBtags_isLoaded) {
			if (nBtags_branch != 0) {
				nBtags_branch->GetEntry(index);
			} else { 
				printf("branch nBtags_branch does not exist!\n");
				exit(1);
			}
			nBtags_isLoaded = true;
		}
		return nBtags_;
	}
	int &nJets40()
	{
		if (not nJets40_isLoaded) {
			if (nJets40_branch != 0) {
				nJets40_branch->GetEntry(index);
			} else { 
				printf("branch nJets40_branch does not exist!\n");
				exit(1);
			}
			nJets40_isLoaded = true;
		}
		return nJets40_;
	}
	int &nLep()
	{
		if (not nLep_isLoaded) {
			if (nLep_branch != 0) {
				nLep_branch->GetEntry(index);
			} else { 
				printf("branch nLep_branch does not exist!\n");
				exit(1);
			}
			nLep_isLoaded = true;
		}
		return nLep_;
	}
	const vector<int> &genBIDs()
	{
		if (not genBIDs_isLoaded) {
			if (genBIDs_branch != 0) {
				genBIDs_branch->GetEntry(index);
			} else { 
				printf("branch genBIDs_branch does not exist!\n");
				exit(1);
			}
			genBIDs_isLoaded = true;
		}
		return genBIDs_;
	}
	const vector<int> &gen_id()
	{
		if (not gen_id_isLoaded) {
			if (gen_id_branch != 0) {
				gen_id_branch->GetEntry(index);
			} else { 
				printf("branch gen_id_branch does not exist!\n");
				exit(1);
			}
			gen_id_isLoaded = true;
		}
		return gen_id_;
	}
	const vector<int> &gen_matchtojet()
	{
		if (not gen_matchtojet_isLoaded) {
			if (gen_matchtojet_branch != 0) {
				gen_matchtojet_branch->GetEntry(index);
			} else { 
				printf("branch gen_matchtojet_branch does not exist!\n");
				exit(1);
			}
			gen_matchtojet_isLoaded = true;
		}
		return gen_matchtojet_;
	}
	const vector<int> &gen_mother_id()
	{
		if (not gen_mother_id_isLoaded) {
			if (gen_mother_id_branch != 0) {
				gen_mother_id_branch->GetEntry(index);
			} else { 
				printf("branch gen_mother_id_branch does not exist!\n");
				exit(1);
			}
			gen_mother_id_isLoaded = true;
		}
		return gen_mother_id_;
	}
	const vector<int> &gen_status()
	{
		if (not gen_status_isLoaded) {
			if (gen_status_branch != 0) {
				gen_status_branch->GetEntry(index);
			} else { 
				printf("branch gen_status_branch does not exist!\n");
				exit(1);
			}
			gen_status_isLoaded = true;
		}
		return gen_status_;
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
extern SYST systematics;
#endif

namespace syst {
	const vector<string> &weightsID();
	const bool &keep();
	const double &qScale();
	const float &ht();
	const float &met();
	const float &metphi();
	const float &iso1();
	const float &iso2();
	const float &weight();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >  &lep1();
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >  &lep2();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genBs();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genjet();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genps();
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_mother_p4();
	const vector<float> &jet_eta();
	const vector<float> &jet_phi();
	const vector<float> &jet_pt();
	const vector<float> &gen_eta();
	const vector<float> &gen_phi();
	const vector<float> &gen_pt();
	const vector<float> &weights();
	const int &gen_gmother_id();
	const int &gramid1();
	const int &id1();
	const int &gramid2();
	const int &id2();
	const int &nBtags();
	const int &nJets40();
	const int &nLep();
	const vector<int> &genBIDs();
	const vector<int> &gen_id();
	const vector<int> &gen_matchtojet();
	const vector<int> &gen_mother_id();
	const vector<int> &gen_status();
}
#endif
