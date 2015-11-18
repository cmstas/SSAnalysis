#include "OUT.h"
OUT outreach;

void OUT::Init(TTree *tree) {
	genJets_branch = 0;
	if (tree->GetBranch("genJets") != 0) {
		genJets_branch = tree->GetBranch("genJets");
		if (genJets_branch) {genJets_branch->SetAddress(&genJets_);}
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
  tree->SetMakeClass(1);
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
	genLep1_idx_branch = 0;
	if (tree->GetBranch("genLep1_idx") != 0) {
		genLep1_idx_branch = tree->GetBranch("genLep1_idx");
		if (genLep1_idx_branch) {genLep1_idx_branch->SetAddress(&genLep1_idx_);}
	}
	genLep2_idx_branch = 0;
	if (tree->GetBranch("genLep2_idx") != 0) {
		genLep2_idx_branch = tree->GetBranch("genLep2_idx");
		if (genLep2_idx_branch) {genLep2_idx_branch->SetAddress(&genLep2_idx_);}
	}
	htGen_branch = 0;
	if (tree->GetBranch("htGen") != 0) {
		htGen_branch = tree->GetBranch("htGen");
		if (htGen_branch) {htGen_branch->SetAddress(&htGen_);}
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
  tree->SetMakeClass(0);
}
void OUT::GetEntry(unsigned int idx) 
	// this only marks branches as not loaded, saving a lot of time
	{
		index = idx;
		genLep1_id_isLoaded = false;
		genLep2_id_isLoaded = false;
		genLep1_idx_isLoaded = false;
		genLep2_idx_isLoaded = false;
		htGen_isLoaded = false;
		genJets_isLoaded = false;
		nGenJets_isLoaded = false;
		genmet_isLoaded = false;
		lep1_passID_isLoaded = false;
		lep2_passID_isLoaded = false;
		pt1_reco_isLoaded = false;
		pt2_reco_isLoaded = false;
		id1_reco_isLoaded = false;
		id2_reco_isLoaded = false;
		idx1_reco_isLoaded = false;
		idx2_reco_isLoaded = false;
		genLep1_p4_isLoaded = false;
		genLep2_p4_isLoaded = false;
	}

void OUT::LoadAllBranches() 
	// load all branches
{
	if (genLep1_id_branch != 0) genLep1_id();
	if (genLep2_id_branch != 0) genLep2_id();
	if (genLep1_idx_branch != 0) genLep1_idx();
	if (genLep2_idx_branch != 0) genLep2_idx();
	if (htGen_branch != 0) htGen();
	if (genJets_branch != 0) genJets();
	if (nGenJets_branch != 0) nGenJets();
	if (genmet_branch != 0) genmet();
	if (lep1_passID_branch != 0) lep1_passID();
	if (lep2_passID_branch != 0) lep2_passID();
	if (pt1_reco_branch != 0) pt1_reco();
	if (pt2_reco_branch != 0) pt2_reco();
	if (id1_reco_branch != 0) id1_reco();
	if (id2_reco_branch != 0) id2_reco();
	if (idx1_reco_branch != 0) idx1_reco();
	if (idx2_reco_branch != 0) idx2_reco();
	if (genLep1_p4_branch != 0) genLep1_p4();
	if (genLep2_p4_branch != 0) genLep2_p4();
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
	const int &OUT::genLep1_idx()
	{
		if (not genLep1_idx_isLoaded) {
			if (genLep1_idx_branch != 0) {
				genLep1_idx_branch->GetEntry(index);
			} else { 
				printf("branch genLep1_idx_branch does not exist!\n");
				exit(1);
			}
			genLep1_idx_isLoaded = true;
		}
		return genLep1_idx_;
	}
	const int &OUT::genLep2_idx()
	{
		if (not genLep2_idx_isLoaded) {
			if (genLep2_idx_branch != 0) {
				genLep2_idx_branch->GetEntry(index);
			} else { 
				printf("branch genLep2_idx_branch does not exist!\n");
				exit(1);
			}
			genLep2_idx_isLoaded = true;
		}
		return genLep2_idx_;
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
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &OUT::pt1_reco()
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
		return *pt1_reco_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &OUT::pt2_reco()
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
		return *pt2_reco_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &OUT::id1_reco()
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
		return *id1_reco_;
	}
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &OUT::id2_reco()
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
		return *id2_reco_;
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
	const int &genLep1_id() { return outreach.genLep1_id(); }
	const int &genLep2_id() { return outreach.genLep2_id(); }
	const int &genLep1_idx() { return outreach.genLep1_idx(); }
	const int &genLep2_idx() { return outreach.genLep2_idx(); }
	const float &htGen() { return outreach.htGen(); }
	const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &genJets() { return outreach.genJets(); }
	const int &nGenJets() { return outreach.nGenJets(); }
	const float &genmet() { return outreach.genmet(); }
	const bool &lep1_passID() { return outreach.lep1_passID(); }
	const bool &lep2_passID() { return outreach.lep2_passID(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &pt1_reco() { return outreach.pt1_reco(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &pt2_reco() { return outreach.pt2_reco(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &id1_reco() { return outreach.id1_reco(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &id2_reco() { return outreach.id2_reco(); }
	const int &idx1_reco() { return outreach.idx1_reco(); }
	const int &idx2_reco() { return outreach.idx2_reco(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep1_p4() { return outreach.genLep1_p4(); }
	const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &genLep2_p4() { return outreach.genLep2_p4(); }
}
