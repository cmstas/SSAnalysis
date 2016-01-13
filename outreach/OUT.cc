#include "OUT.h"
OUT outreach;

void OUT::Init(TTree *tree) {
	genJets_branch = 0;
	if (tree->GetBranch("genJets") != 0) {
		genJets_branch = tree->GetBranch("genJets");
		if (genJets_branch) {genJets_branch->SetAddress(&genJets_);}
	}
	recoJets_branch = 0;
	if (tree->GetBranch("recoJets") != 0) {
		recoJets_branch = tree->GetBranch("recoJets");
		if (recoJets_branch) {recoJets_branch->SetAddress(&recoJets_);}
	}
	genLep_p4_branch = 0;
	if (tree->GetBranch("genLep_p4") != 0) {
		genLep_p4_branch = tree->GetBranch("genLep_p4");
		if (genLep_p4_branch) {genLep_p4_branch->SetAddress(&genLep_p4_);}
	}
	recoLep_p4_branch = 0;
	if (tree->GetBranch("recoLep_p4") != 0) {
		recoLep_p4_branch = tree->GetBranch("recoLep_p4");
		if (recoLep_p4_branch) {recoLep_p4_branch->SetAddress(&recoLep_p4_);}
	}
	pfjets_p4_branch = 0;
	if (tree->GetBranch("pfjets_p4") != 0) {
		pfjets_p4_branch = tree->GetBranch("pfjets_p4");
		if (pfjets_p4_branch) {pfjets_p4_branch->SetAddress(&pfjets_p4_);}
	}
	recoMuons_branch = 0;
	if (tree->GetBranch("recoMuons") != 0) {
		recoMuons_branch = tree->GetBranch("recoMuons");
		if (recoMuons_branch) {recoMuons_branch->SetAddress(&recoMuons_);}
	}
	recoElectrons_branch = 0;
	if (tree->GetBranch("recoElectrons") != 0) {
		recoElectrons_branch = tree->GetBranch("recoElectrons");
		if (recoElectrons_branch) {recoElectrons_branch->SetAddress(&recoElectrons_);}
	}
	genLep1_p4_branch = 0;
	if (tree->GetBranch("genLep1_p4") != 0) {
		genLep1_p4_branch = tree->GetBranch("genLep1_p4");
		if (genLep1_p4_branch) {genLep1_p4_branch->SetAddress(&genLep1_p4_);}
	}
	genLep2_p4_branch = 0;
	if (tree->GetBranch("genLep2_p4") != 0) {
		genLep2_p4_branch = tree->GetBranch("genLep2_p4");
		if (genLep2_p4_branch) {genLep2_p4_branch->SetAddress(&genLep2_p4_);}
	}
	recoLep1_p4_branch = 0;
	if (tree->GetBranch("recoLep1_p4") != 0) {
		recoLep1_p4_branch = tree->GetBranch("recoLep1_p4");
		if (recoLep1_p4_branch) {recoLep1_p4_branch->SetAddress(&recoLep1_p4_);}
	}
	recoLep2_p4_branch = 0;
	if (tree->GetBranch("recoLep2_p4") != 0) {
		recoLep2_p4_branch = tree->GetBranch("recoLep2_p4");
		if (recoLep2_p4_branch) {recoLep2_p4_branch->SetAddress(&recoLep2_p4_);}
	}
  tree->SetMakeClass(1);
	event_branch = 0;
	if (tree->GetBranch("event") != 0) {
		event_branch = tree->GetBranch("event");
		if (event_branch) {event_branch->SetAddress(&event_);}
	}
	genLep_id_branch = 0;
	if (tree->GetBranch("genLep_id") != 0) {
		genLep_id_branch = tree->GetBranch("genLep_id");
		if (genLep_id_branch) {genLep_id_branch->SetAddress(&genLep_id_);}
	}
	genLep_idx_branch = 0;
	if (tree->GetBranch("genLep_idx") != 0) {
		genLep_idx_branch = tree->GetBranch("genLep_idx");
		if (genLep_idx_branch) {genLep_idx_branch->SetAddress(&genLep_idx_);}
	}
	htGen_branch = 0;
	if (tree->GetBranch("htGen") != 0) {
		htGen_branch = tree->GetBranch("htGen");
		if (htGen_branch) {htGen_branch->SetAddress(&htGen_);}
	}
	ht_branch = 0;
	if (tree->GetBranch("ht") != 0) {
		ht_branch = tree->GetBranch("ht");
		if (ht_branch) {ht_branch->SetAddress(&ht_);}
	}
	genJets_isb_branch = 0;
	if (tree->GetBranch("genJets_isb") != 0) {
		genJets_isb_branch = tree->GetBranch("genJets_isb");
		if (genJets_isb_branch) {genJets_isb_branch->SetAddress(&genJets_isb_);}
	}
	nGenJets_branch = 0;
	if (tree->GetBranch("nGenJets") != 0) {
		nGenJets_branch = tree->GetBranch("nGenJets");
		if (nGenJets_branch) {nGenJets_branch->SetAddress(&nGenJets_);}
	}
	genmet_branch = 0;
	if (tree->GetBranch("genmet") != 0) {
		genmet_branch = tree->GetBranch("genmet");
		if (genmet_branch) {genmet_branch->SetAddress(&genmet_);}
	}
	met_branch = 0;
	if (tree->GetBranch("met") != 0) {
		met_branch = tree->GetBranch("met");
		if (met_branch) {met_branch->SetAddress(&met_);}
	}
	lep_passID_branch = 0;
	if (tree->GetBranch("lep_passID") != 0) {
		lep_passID_branch = tree->GetBranch("lep_passID");
		if (lep_passID_branch) {lep_passID_branch->SetAddress(&lep_passID_);}
	}
	lep_passID_loose_branch = 0;
	if (tree->GetBranch("lep_passID_loose") != 0) {
		lep_passID_loose_branch = tree->GetBranch("lep_passID_loose");
		if (lep_passID_loose_branch) {lep_passID_loose_branch->SetAddress(&lep_passID_loose_);}
	}
	id_reco_branch = 0;
	if (tree->GetBranch("id_reco") != 0) {
		id_reco_branch = tree->GetBranch("id_reco");
		if (id_reco_branch) {id_reco_branch->SetAddress(&id_reco_);}
	}
	idx_reco_branch = 0;
	if (tree->GetBranch("idx_reco") != 0) {
		idx_reco_branch = tree->GetBranch("idx_reco");
		if (idx_reco_branch) {idx_reco_branch->SetAddress(&idx_reco_);}
	}
	genJets_matched_branch = 0;
	if (tree->GetBranch("genJets_matched") != 0) {
		genJets_matched_branch = tree->GetBranch("genJets_matched");
		if (genJets_matched_branch) {genJets_matched_branch->SetAddress(&genJets_matched_);}
	}
	genJets_matched_tob_branch = 0;
	if (tree->GetBranch("genJets_matched_tob") != 0) {
		genJets_matched_tob_branch = tree->GetBranch("genJets_matched_tob");
		if (genJets_matched_tob_branch) {genJets_matched_tob_branch->SetAddress(&genJets_matched_tob_);}
	}
	genJets_matchedID_branch = 0;
	if (tree->GetBranch("genJets_matchedID") != 0) {
		genJets_matchedID_branch = tree->GetBranch("genJets_matchedID");
		if (genJets_matchedID_branch) {genJets_matchedID_branch->SetAddress(&genJets_matchedID_);}
	}
	pfjets_isClean_branch = 0;
	if (tree->GetBranch("pfjets_isClean") != 0) {
		pfjets_isClean_branch = tree->GetBranch("pfjets_isClean");
		if (pfjets_isClean_branch) {pfjets_isClean_branch->SetAddress(&pfjets_isClean_);}
	}
	pfjets_isb_branch = 0;
	if (tree->GetBranch("pfjets_isb") != 0) {
		pfjets_isb_branch = tree->GetBranch("pfjets_isb");
		if (pfjets_isb_branch) {pfjets_isb_branch->SetAddress(&pfjets_isb_);}
	}
	recoMuonsMatch_branch = 0;
	if (tree->GetBranch("recoMuonsMatch") != 0) {
		recoMuonsMatch_branch = tree->GetBranch("recoMuonsMatch");
		if (recoMuonsMatch_branch) {recoMuonsMatch_branch->SetAddress(&recoMuonsMatch_);}
	}
	recoMuonsID_branch = 0;
	if (tree->GetBranch("recoMuonsID") != 0) {
		recoMuonsID_branch = tree->GetBranch("recoMuonsID");
		if (recoMuonsID_branch) {recoMuonsID_branch->SetAddress(&recoMuonsID_);}
	}
	recoMuonsID_loose_branch = 0;
	if (tree->GetBranch("recoMuonsID_loose") != 0) {
		recoMuonsID_loose_branch = tree->GetBranch("recoMuonsID_loose");
		if (recoMuonsID_loose_branch) {recoMuonsID_loose_branch->SetAddress(&recoMuonsID_loose_);}
	}
	recoElectronsMatch_branch = 0;
	if (tree->GetBranch("recoElectronsMatch") != 0) {
		recoElectronsMatch_branch = tree->GetBranch("recoElectronsMatch");
		if (recoElectronsMatch_branch) {recoElectronsMatch_branch->SetAddress(&recoElectronsMatch_);}
	}
	recoElectronsID_branch = 0;
	if (tree->GetBranch("recoElectronsID") != 0) {
		recoElectronsID_branch = tree->GetBranch("recoElectronsID");
		if (recoElectronsID_branch) {recoElectronsID_branch->SetAddress(&recoElectronsID_);}
	}
	recoElectronsID_loose_branch = 0;
	if (tree->GetBranch("recoElectronsID_loose") != 0) {
		recoElectronsID_loose_branch = tree->GetBranch("recoElectronsID_loose");
		if (recoElectronsID_loose_branch) {recoElectronsID_loose_branch->SetAddress(&recoElectronsID_loose_);}
	}
	pfjets_match_branch = 0;
	if (tree->GetBranch("pfjets_match") != 0) {
		pfjets_match_branch = tree->GetBranch("pfjets_match");
		if (pfjets_match_branch) {pfjets_match_branch->SetAddress(&pfjets_match_);}
	}
	pfjets_matchb_branch = 0;
	if (tree->GetBranch("pfjets_matchb") != 0) {
		pfjets_matchb_branch = tree->GetBranch("pfjets_matchb");
		if (pfjets_matchb_branch) {pfjets_matchb_branch->SetAddress(&pfjets_matchb_);}
	}
	pfjets_ID_branch = 0;
	if (tree->GetBranch("pfjets_ID") != 0) {
		pfjets_ID_branch = tree->GetBranch("pfjets_ID");
		if (pfjets_ID_branch) {pfjets_ID_branch->SetAddress(&pfjets_ID_);}
	}
	genLep1_id_branch = 0;
	if (tree->GetBranch("genLep1_id") != 0) {
		genLep1_id_branch = tree->GetBranch("genLep1_id");
		if (genLep1_id_branch) {genLep1_id_branch->SetAddress(&genLep1_id_);}
	}
	genLep2_id_branch = 0;
	if (tree->GetBranch("genLep2_id") != 0) {
		genLep2_id_branch = tree->GetBranch("genLep2_id");
		if (genLep2_id_branch) {genLep2_id_branch->SetAddress(&genLep2_id_);}
	}
	lep1_passID_branch = 0;
	if (tree->GetBranch("lep1_passID") != 0) {
		lep1_passID_branch = tree->GetBranch("lep1_passID");
		if (lep1_passID_branch) {lep1_passID_branch->SetAddress(&lep1_passID_);}
	}
	lep2_passID_branch = 0;
	if (tree->GetBranch("lep2_passID") != 0) {
		lep2_passID_branch = tree->GetBranch("lep2_passID");
		if (lep2_passID_branch) {lep2_passID_branch->SetAddress(&lep2_passID_);}
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
	idx1_reco_branch = 0;
	if (tree->GetBranch("idx1_reco") != 0) {
		idx1_reco_branch = tree->GetBranch("idx1_reco");
		if (idx1_reco_branch) {idx1_reco_branch->SetAddress(&idx1_reco_);}
	}
	idx2_reco_branch = 0;
	if (tree->GetBranch("idx2_reco") != 0) {
		idx2_reco_branch = tree->GetBranch("idx2_reco");
		if (idx2_reco_branch) {idx2_reco_branch->SetAddress(&idx2_reco_);}
	}
	fired_trigger_1_branch = 0;
	if (tree->GetBranch("fired_trigger_1") != 0) {
		fired_trigger_1_branch = tree->GetBranch("fired_trigger_1");
		if (fired_trigger_1_branch) {fired_trigger_1_branch->SetAddress(&fired_trigger_1_);}
	}
	fired_trigger_2_branch = 0;
	if (tree->GetBranch("fired_trigger_2") != 0) {
		fired_trigger_2_branch = tree->GetBranch("fired_trigger_2");
		if (fired_trigger_2_branch) {fired_trigger_2_branch->SetAddress(&fired_trigger_2_);}
	}
  tree->SetMakeClass(0);
}
void OUT::GetEntry(unsigned int idx) 
	// this only marks branches as not loaded, saving a lot of time
	{
		index = idx;
		event_isLoaded = false;
		genLep_id_isLoaded = false;
		genLep_idx_isLoaded = false;
		htGen_isLoaded = false;
		ht_isLoaded = false;
		genJets_isLoaded = false;
		genJets_isb_isLoaded = false;
		recoJets_isLoaded = false;
		nGenJets_isLoaded = false;
		genmet_isLoaded = false;
		met_isLoaded = false;
		lep_passID_isLoaded = false;
		lep_passID_loose_isLoaded = false;
		id_reco_isLoaded = false;
		idx_reco_isLoaded = false;
		genLep_p4_isLoaded = false;
		recoLep_p4_isLoaded = false;
		genJets_matched_isLoaded = false;
		genJets_matched_tob_isLoaded = false;
		genJets_matchedID_isLoaded = false;
		pfjets_p4_isLoaded = false;
		pfjets_isClean_isLoaded = false;
		pfjets_isb_isLoaded = false;
		recoMuons_isLoaded = false;
		recoElectrons_isLoaded = false;
		recoMuonsMatch_isLoaded = false;
		recoMuonsID_isLoaded = false;
		recoMuonsID_loose_isLoaded = false;
		recoElectronsMatch_isLoaded = false;
		recoElectronsID_isLoaded = false;
		recoElectronsID_loose_isLoaded = false;
		pfjets_match_isLoaded = false;
		pfjets_matchb_isLoaded = false;
		pfjets_ID_isLoaded = false;
		genLep1_p4_isLoaded = false;
		genLep2_p4_isLoaded = false;
		recoLep1_p4_isLoaded = false;
		recoLep2_p4_isLoaded = false;
		genLep1_id_isLoaded = false;
		genLep2_id_isLoaded = false;
		lep1_passID_isLoaded = false;
		lep2_passID_isLoaded = false;
		id1_reco_isLoaded = false;
		id2_reco_isLoaded = false;
		idx1_reco_isLoaded = false;
		idx2_reco_isLoaded = false;
		fired_trigger_1_isLoaded = false;
		fired_trigger_2_isLoaded = false;
	}

void OUT::LoadAllBranches() 
	// load all branches
{
	if (event_branch != 0) event();
	if (genLep_id_branch != 0) genLep_id();
	if (genLep_idx_branch != 0) genLep_idx();
	if (htGen_branch != 0) htGen();
	if (ht_branch != 0) ht();
	if (genJets_branch != 0) genJets();
	if (genJets_isb_branch != 0) genJets_isb();
	if (recoJets_branch != 0) recoJets();
	if (nGenJets_branch != 0) nGenJets();
	if (genmet_branch != 0) genmet();
	if (met_branch != 0) met();
	if (lep_passID_branch != 0) lep_passID();
	if (lep_passID_loose_branch != 0) lep_passID_loose();
	if (id_reco_branch != 0) id_reco();
	if (idx_reco_branch != 0) idx_reco();
	if (genLep_p4_branch != 0) genLep_p4();
	if (recoLep_p4_branch != 0) recoLep_p4();
	if (genJets_matched_branch != 0) genJets_matched();
	if (genJets_matched_tob_branch != 0) genJets_matched_tob();
	if (genJets_matchedID_branch != 0) genJets_matchedID();
	if (pfjets_p4_branch != 0) pfjets_p4();
	if (pfjets_isClean_branch != 0) pfjets_isClean();
	if (pfjets_isb_branch != 0) pfjets_isb();
	if (recoMuons_branch != 0) recoMuons();
	if (recoElectrons_branch != 0) recoElectrons();
	if (recoMuonsMatch_branch != 0) recoMuonsMatch();
	if (recoMuonsID_branch != 0) recoMuonsID();
	if (recoMuonsID_loose_branch != 0) recoMuonsID_loose();
	if (recoElectronsMatch_branch != 0) recoElectronsMatch();
	if (recoElectronsID_branch != 0) recoElectronsID();
	if (recoElectronsID_loose_branch != 0) recoElectronsID_loose();
	if (pfjets_match_branch != 0) pfjets_match();
	if (pfjets_matchb_branch != 0) pfjets_matchb();
	if (pfjets_ID_branch != 0) pfjets_ID();
	if (genLep1_p4_branch != 0) genLep1_p4();
	if (genLep2_p4_branch != 0) genLep2_p4();
	if (recoLep1_p4_branch != 0) recoLep1_p4();
	if (recoLep2_p4_branch != 0) recoLep2_p4();
	if (genLep1_id_branch != 0) genLep1_id();
	if (genLep2_id_branch != 0) genLep2_id();
	if (lep1_passID_branch != 0) lep1_passID();
	if (lep2_passID_branch != 0) lep2_passID();
	if (id1_reco_branch != 0) id1_reco();
	if (id2_reco_branch != 0) id2_reco();
	if (idx1_reco_branch != 0) idx1_reco();
	if (idx2_reco_branch != 0) idx2_reco();
	if (fired_trigger_1_branch != 0) fired_trigger_1();
	if (fired_trigger_2_branch != 0) fired_trigger_2();
}

	const unsigned long long &OUT::event()
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
	const vector<int> &OUT::genLep_id()
	{
		if (not genLep_id_isLoaded) {
			if (genLep_id_branch != 0) {
				genLep_id_branch->GetEntry(index);
			} else { 
				printf("branch genLep_id_branch does not exist!\n");
				exit(1);
			}
			genLep_id_isLoaded = true;
		}
		return *genLep_id_;
	}
	const vector<int> &OUT::genLep_idx()
	{
		if (not genLep_idx_isLoaded) {
			if (genLep_idx_branch != 0) {
				genLep_idx_branch->GetEntry(index);
			} else { 
				printf("branch genLep_idx_branch does not exist!\n");
				exit(1);
			}
			genLep_idx_isLoaded = true;
		}
		return *genLep_idx_;
	}
	const float &OUT::htGen()
	{
		if (not htGen_isLoaded) {
			if (htGen_branch != 0) {
				htGen_branch->GetEntry(index);
			} else { 
				printf("branch htGen_branch does not exist!\n");
				exit(1);
			}
			htGen_isLoaded = true;
		}
		return htGen_;
	}
	const float &OUT::ht()
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
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &OUT::genJets()
	{
		if (not genJets_isLoaded) {
			if (genJets_branch != 0) {
				genJets_branch->GetEntry(index);
			} else { 
				printf("branch genJets_branch does not exist!\n");
				exit(1);
			}
			genJets_isLoaded = true;
		}
		return *genJets_;
	}
	const vector<bool> &OUT::genJets_isb()
	{
		if (not genJets_isb_isLoaded) {
			if (genJets_isb_branch != 0) {
				genJets_isb_branch->GetEntry(index);
			} else { 
				printf("branch genJets_isb_branch does not exist!\n");
				exit(1);
			}
			genJets_isb_isLoaded = true;
		}
		return *genJets_isb_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &OUT::recoJets()
	{
		if (not recoJets_isLoaded) {
			if (recoJets_branch != 0) {
				recoJets_branch->GetEntry(index);
			} else { 
				printf("branch recoJets_branch does not exist!\n");
				exit(1);
			}
			recoJets_isLoaded = true;
		}
		return *recoJets_;
	}
	const int &OUT::nGenJets()
	{
		if (not nGenJets_isLoaded) {
			if (nGenJets_branch != 0) {
				nGenJets_branch->GetEntry(index);
			} else { 
				printf("branch nGenJets_branch does not exist!\n");
				exit(1);
			}
			nGenJets_isLoaded = true;
		}
		return nGenJets_;
	}
	const float &OUT::genmet()
	{
		if (not genmet_isLoaded) {
			if (genmet_branch != 0) {
				genmet_branch->GetEntry(index);
			} else { 
				printf("branch genmet_branch does not exist!\n");
				exit(1);
			}
			genmet_isLoaded = true;
		}
		return genmet_;
	}
	const float &OUT::met()
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
	const vector<bool> &OUT::lep_passID()
	{
		if (not lep_passID_isLoaded) {
			if (lep_passID_branch != 0) {
				lep_passID_branch->GetEntry(index);
			} else { 
				printf("branch lep_passID_branch does not exist!\n");
				exit(1);
			}
			lep_passID_isLoaded = true;
		}
		return *lep_passID_;
	}
	const vector<bool> &OUT::lep_passID_loose()
	{
		if (not lep_passID_loose_isLoaded) {
			if (lep_passID_loose_branch != 0) {
				lep_passID_loose_branch->GetEntry(index);
			} else { 
				printf("branch lep_passID_loose_branch does not exist!\n");
				exit(1);
			}
			lep_passID_loose_isLoaded = true;
		}
		return *lep_passID_loose_;
	}
	const vector<int> &OUT::id_reco()
	{
		if (not id_reco_isLoaded) {
			if (id_reco_branch != 0) {
				id_reco_branch->GetEntry(index);
			} else { 
				printf("branch id_reco_branch does not exist!\n");
				exit(1);
			}
			id_reco_isLoaded = true;
		}
		return *id_reco_;
	}
	const vector<int> &OUT::idx_reco()
	{
		if (not idx_reco_isLoaded) {
			if (idx_reco_branch != 0) {
				idx_reco_branch->GetEntry(index);
			} else { 
				printf("branch idx_reco_branch does not exist!\n");
				exit(1);
			}
			idx_reco_isLoaded = true;
		}
		return *idx_reco_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &OUT::genLep_p4()
	{
		if (not genLep_p4_isLoaded) {
			if (genLep_p4_branch != 0) {
				genLep_p4_branch->GetEntry(index);
			} else { 
				printf("branch genLep_p4_branch does not exist!\n");
				exit(1);
			}
			genLep_p4_isLoaded = true;
		}
		return *genLep_p4_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &OUT::recoLep_p4()
	{
		if (not recoLep_p4_isLoaded) {
			if (recoLep_p4_branch != 0) {
				recoLep_p4_branch->GetEntry(index);
			} else { 
				printf("branch recoLep_p4_branch does not exist!\n");
				exit(1);
			}
			recoLep_p4_isLoaded = true;
		}
		return *recoLep_p4_;
	}
	const vector<int> &OUT::genJets_matched()
	{
		if (not genJets_matched_isLoaded) {
			if (genJets_matched_branch != 0) {
				genJets_matched_branch->GetEntry(index);
			} else { 
				printf("branch genJets_matched_branch does not exist!\n");
				exit(1);
			}
			genJets_matched_isLoaded = true;
		}
		return *genJets_matched_;
	}
	const vector<int> &OUT::genJets_matched_tob()
	{
		if (not genJets_matched_tob_isLoaded) {
			if (genJets_matched_tob_branch != 0) {
				genJets_matched_tob_branch->GetEntry(index);
			} else { 
				printf("branch genJets_matched_tob_branch does not exist!\n");
				exit(1);
			}
			genJets_matched_tob_isLoaded = true;
		}
		return *genJets_matched_tob_;
	}
	const vector<int> &OUT::genJets_matchedID()
	{
		if (not genJets_matchedID_isLoaded) {
			if (genJets_matchedID_branch != 0) {
				genJets_matchedID_branch->GetEntry(index);
			} else { 
				printf("branch genJets_matchedID_branch does not exist!\n");
				exit(1);
			}
			genJets_matchedID_isLoaded = true;
		}
		return *genJets_matchedID_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &OUT::pfjets_p4()
	{
		if (not pfjets_p4_isLoaded) {
			if (pfjets_p4_branch != 0) {
				pfjets_p4_branch->GetEntry(index);
			} else { 
				printf("branch pfjets_p4_branch does not exist!\n");
				exit(1);
			}
			pfjets_p4_isLoaded = true;
		}
		return *pfjets_p4_;
	}
	const vector<bool> &OUT::pfjets_isClean()
	{
		if (not pfjets_isClean_isLoaded) {
			if (pfjets_isClean_branch != 0) {
				pfjets_isClean_branch->GetEntry(index);
			} else { 
				printf("branch pfjets_isClean_branch does not exist!\n");
				exit(1);
			}
			pfjets_isClean_isLoaded = true;
		}
		return *pfjets_isClean_;
	}
	const vector<bool> &OUT::pfjets_isb()
	{
		if (not pfjets_isb_isLoaded) {
			if (pfjets_isb_branch != 0) {
				pfjets_isb_branch->GetEntry(index);
			} else { 
				printf("branch pfjets_isb_branch does not exist!\n");
				exit(1);
			}
			pfjets_isb_isLoaded = true;
		}
		return *pfjets_isb_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &OUT::recoMuons()
	{
		if (not recoMuons_isLoaded) {
			if (recoMuons_branch != 0) {
				recoMuons_branch->GetEntry(index);
			} else { 
				printf("branch recoMuons_branch does not exist!\n");
				exit(1);
			}
			recoMuons_isLoaded = true;
		}
		return *recoMuons_;
	}
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &OUT::recoElectrons()
	{
		if (not recoElectrons_isLoaded) {
			if (recoElectrons_branch != 0) {
				recoElectrons_branch->GetEntry(index);
			} else { 
				printf("branch recoElectrons_branch does not exist!\n");
				exit(1);
			}
			recoElectrons_isLoaded = true;
		}
		return *recoElectrons_;
	}
	const vector<int> &OUT::recoMuonsMatch()
	{
		if (not recoMuonsMatch_isLoaded) {
			if (recoMuonsMatch_branch != 0) {
				recoMuonsMatch_branch->GetEntry(index);
			} else { 
				printf("branch recoMuonsMatch_branch does not exist!\n");
				exit(1);
			}
			recoMuonsMatch_isLoaded = true;
		}
		return *recoMuonsMatch_;
	}
	const vector<bool> &OUT::recoMuonsID()
	{
		if (not recoMuonsID_isLoaded) {
			if (recoMuonsID_branch != 0) {
				recoMuonsID_branch->GetEntry(index);
			} else { 
				printf("branch recoMuonsID_branch does not exist!\n");
				exit(1);
			}
			recoMuonsID_isLoaded = true;
		}
		return *recoMuonsID_;
	}
	const vector<bool> &OUT::recoMuonsID_loose()
	{
		if (not recoMuonsID_loose_isLoaded) {
			if (recoMuonsID_loose_branch != 0) {
				recoMuonsID_loose_branch->GetEntry(index);
			} else { 
				printf("branch recoMuonsID_loose_branch does not exist!\n");
				exit(1);
			}
			recoMuonsID_loose_isLoaded = true;
		}
		return *recoMuonsID_loose_;
	}
	const vector<int> &OUT::recoElectronsMatch()
	{
		if (not recoElectronsMatch_isLoaded) {
			if (recoElectronsMatch_branch != 0) {
				recoElectronsMatch_branch->GetEntry(index);
			} else { 
				printf("branch recoElectronsMatch_branch does not exist!\n");
				exit(1);
			}
			recoElectronsMatch_isLoaded = true;
		}
		return *recoElectronsMatch_;
	}
	const vector<bool> &OUT::recoElectronsID()
	{
		if (not recoElectronsID_isLoaded) {
			if (recoElectronsID_branch != 0) {
				recoElectronsID_branch->GetEntry(index);
			} else { 
				printf("branch recoElectronsID_branch does not exist!\n");
				exit(1);
			}
			recoElectronsID_isLoaded = true;
		}
		return *recoElectronsID_;
	}
	const vector<bool> &OUT::recoElectronsID_loose()
	{
		if (not recoElectronsID_loose_isLoaded) {
			if (recoElectronsID_loose_branch != 0) {
				recoElectronsID_loose_branch->GetEntry(index);
			} else { 
				printf("branch recoElectronsID_loose_branch does not exist!\n");
				exit(1);
			}
			recoElectronsID_loose_isLoaded = true;
		}
		return *recoElectronsID_loose_;
	}
	const vector<int> &OUT::pfjets_match()
	{
		if (not pfjets_match_isLoaded) {
			if (pfjets_match_branch != 0) {
				pfjets_match_branch->GetEntry(index);
			} else { 
				printf("branch pfjets_match_branch does not exist!\n");
				exit(1);
			}
			pfjets_match_isLoaded = true;
		}
		return *pfjets_match_;
	}
	const vector<bool> &OUT::pfjets_matchb()
	{
		if (not pfjets_matchb_isLoaded) {
			if (pfjets_matchb_branch != 0) {
				pfjets_matchb_branch->GetEntry(index);
			} else { 
				printf("branch pfjets_matchb_branch does not exist!\n");
				exit(1);
			}
			pfjets_matchb_isLoaded = true;
		}
		return *pfjets_matchb_;
	}
	const vector<bool> &OUT::pfjets_ID()
	{
		if (not pfjets_ID_isLoaded) {
			if (pfjets_ID_branch != 0) {
				pfjets_ID_branch->GetEntry(index);
			} else { 
				printf("branch pfjets_ID_branch does not exist!\n");
				exit(1);
			}
			pfjets_ID_isLoaded = true;
		}
		return *pfjets_ID_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &OUT::genLep1_p4()
	{
		if (not genLep1_p4_isLoaded) {
			if (genLep1_p4_branch != 0) {
				genLep1_p4_branch->GetEntry(index);
			} else { 
				printf("branch genLep1_p4_branch does not exist!\n");
				exit(1);
			}
			genLep1_p4_isLoaded = true;
		}
		return *genLep1_p4_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &OUT::genLep2_p4()
	{
		if (not genLep2_p4_isLoaded) {
			if (genLep2_p4_branch != 0) {
				genLep2_p4_branch->GetEntry(index);
			} else { 
				printf("branch genLep2_p4_branch does not exist!\n");
				exit(1);
			}
			genLep2_p4_isLoaded = true;
		}
		return *genLep2_p4_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &OUT::recoLep1_p4()
	{
		if (not recoLep1_p4_isLoaded) {
			if (recoLep1_p4_branch != 0) {
				recoLep1_p4_branch->GetEntry(index);
			} else { 
				printf("branch recoLep1_p4_branch does not exist!\n");
				exit(1);
			}
			recoLep1_p4_isLoaded = true;
		}
		return *recoLep1_p4_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &OUT::recoLep2_p4()
	{
		if (not recoLep2_p4_isLoaded) {
			if (recoLep2_p4_branch != 0) {
				recoLep2_p4_branch->GetEntry(index);
			} else { 
				printf("branch recoLep2_p4_branch does not exist!\n");
				exit(1);
			}
			recoLep2_p4_isLoaded = true;
		}
		return *recoLep2_p4_;
	}
	const int &OUT::genLep1_id()
	{
		if (not genLep1_id_isLoaded) {
			if (genLep1_id_branch != 0) {
				genLep1_id_branch->GetEntry(index);
			} else { 
				printf("branch genLep1_id_branch does not exist!\n");
				exit(1);
			}
			genLep1_id_isLoaded = true;
		}
		return genLep1_id_;
	}
	const int &OUT::genLep2_id()
	{
		if (not genLep2_id_isLoaded) {
			if (genLep2_id_branch != 0) {
				genLep2_id_branch->GetEntry(index);
			} else { 
				printf("branch genLep2_id_branch does not exist!\n");
				exit(1);
			}
			genLep2_id_isLoaded = true;
		}
		return genLep2_id_;
	}
	const bool &	OUT::lep1_passID()
	{
		if (not lep1_passID_isLoaded) {
			if (lep1_passID_branch != 0) {
				lep1_passID_branch->GetEntry(index);
			} else { 
				printf("branch lep1_passID_branch does not exist!\n");
				exit(1);
			}
			lep1_passID_isLoaded = true;
		}
		return lep1_passID_;
	}
	const bool &	OUT::lep2_passID()
	{
		if (not lep2_passID_isLoaded) {
			if (lep2_passID_branch != 0) {
				lep2_passID_branch->GetEntry(index);
			} else { 
				printf("branch lep2_passID_branch does not exist!\n");
				exit(1);
			}
			lep2_passID_isLoaded = true;
		}
		return lep2_passID_;
	}
	const int &OUT::id1_reco()
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
	const int &OUT::id2_reco()
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
	const int &OUT::idx1_reco()
	{
		if (not idx1_reco_isLoaded) {
			if (idx1_reco_branch != 0) {
				idx1_reco_branch->GetEntry(index);
			} else { 
				printf("branch idx1_reco_branch does not exist!\n");
				exit(1);
			}
			idx1_reco_isLoaded = true;
		}
		return idx1_reco_;
	}
	const int &OUT::idx2_reco()
	{
		if (not idx2_reco_isLoaded) {
			if (idx2_reco_branch != 0) {
				idx2_reco_branch->GetEntry(index);
			} else { 
				printf("branch idx2_reco_branch does not exist!\n");
				exit(1);
			}
			idx2_reco_isLoaded = true;
		}
		return idx2_reco_;
	}
	const bool &	OUT::fired_trigger_1()
	{
		if (not fired_trigger_1_isLoaded) {
			if (fired_trigger_1_branch != 0) {
				fired_trigger_1_branch->GetEntry(index);
			} else { 
				printf("branch fired_trigger_1_branch does not exist!\n");
				exit(1);
			}
			fired_trigger_1_isLoaded = true;
		}
		return fired_trigger_1_;
	}
	const bool &	OUT::fired_trigger_2()
	{
		if (not fired_trigger_2_isLoaded) {
			if (fired_trigger_2_branch != 0) {
				fired_trigger_2_branch->GetEntry(index);
			} else { 
				printf("branch fired_trigger_2_branch does not exist!\n");
				exit(1);
			}
			fired_trigger_2_isLoaded = true;
		}
		return fired_trigger_2_;
	}

  void OUT::progress( int nEventsTotal, int nEventsChain ){
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
  
namespace out {
	const unsigned long long &event() { return outreach.event(); }
	const vector<int> &genLep_id() { return outreach.genLep_id(); }
	const vector<int> &genLep_idx() { return outreach.genLep_idx(); }
	const float &htGen() { return outreach.htGen(); }
	const float &ht() { return outreach.ht(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genJets() { return outreach.genJets(); }
	const vector<bool> &genJets_isb() { return outreach.genJets_isb(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoJets() { return outreach.recoJets(); }
	const int &nGenJets() { return outreach.nGenJets(); }
	const float &genmet() { return outreach.genmet(); }
	const float &met() { return outreach.met(); }
	const vector<bool> &lep_passID() { return outreach.lep_passID(); }
	const vector<bool> &lep_passID_loose() { return outreach.lep_passID_loose(); }
	const vector<int> &id_reco() { return outreach.id_reco(); }
	const vector<int> &idx_reco() { return outreach.idx_reco(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genLep_p4() { return outreach.genLep_p4(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoLep_p4() { return outreach.recoLep_p4(); }
	const vector<int> &genJets_matched() { return outreach.genJets_matched(); }
	const vector<int> &genJets_matched_tob() { return outreach.genJets_matched_tob(); }
	const vector<int> &genJets_matchedID() { return outreach.genJets_matchedID(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &pfjets_p4() { return outreach.pfjets_p4(); }
	const vector<bool> &pfjets_isClean() { return outreach.pfjets_isClean(); }
	const vector<bool> &pfjets_isb() { return outreach.pfjets_isb(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoMuons() { return outreach.recoMuons(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &recoElectrons() { return outreach.recoElectrons(); }
	const vector<int> &recoMuonsMatch() { return outreach.recoMuonsMatch(); }
	const vector<bool> &recoMuonsID() { return outreach.recoMuonsID(); }
	const vector<bool> &recoMuonsID_loose() { return outreach.recoMuonsID_loose(); }
	const vector<int> &recoElectronsMatch() { return outreach.recoElectronsMatch(); }
	const vector<bool> &recoElectronsID() { return outreach.recoElectronsID(); }
	const vector<bool> &recoElectronsID_loose() { return outreach.recoElectronsID_loose(); }
	const vector<int> &pfjets_match() { return outreach.pfjets_match(); }
	const vector<bool> &pfjets_matchb() { return outreach.pfjets_matchb(); }
	const vector<bool> &pfjets_ID() { return outreach.pfjets_ID(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep1_p4() { return outreach.genLep1_p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep2_p4() { return outreach.genLep2_p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &recoLep1_p4() { return outreach.recoLep1_p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &recoLep2_p4() { return outreach.recoLep2_p4(); }
	const int &genLep1_id() { return outreach.genLep1_id(); }
	const int &genLep2_id() { return outreach.genLep2_id(); }
	const bool &lep1_passID() { return outreach.lep1_passID(); }
	const bool &lep2_passID() { return outreach.lep2_passID(); }
	const int &id1_reco() { return outreach.id1_reco(); }
	const int &id2_reco() { return outreach.id2_reco(); }
	const int &idx1_reco() { return outreach.idx1_reco(); }
	const int &idx2_reco() { return outreach.idx2_reco(); }
	const bool &fired_trigger_1() { return outreach.fired_trigger_1(); }
	const bool &fired_trigger_2() { return outreach.fired_trigger_2(); }
}
