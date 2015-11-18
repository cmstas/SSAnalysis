#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TColor.h"
#include "TH1F.h"
#include "/home/users/cgeorge/software/makeCMS3ClassFiles/SS.h"
#include "../CORE/Tools/utils.h"
#include "../CORE/SSSelections.h"
#include "../CORE/MCSelections.h"
#include "../CORE/CMS3.h"
#include "babymaker.h"

hyp_type_t getHypType(int id1, int id2){
        
    if (abs(id1) != 11 && abs(id1) != 13) return UNASSIGNED;
    if (abs(id2) != 11 && abs(id2) != 13) return UNASSIGNED;

    if (abs(id1) == 11 && abs(id2) == 11) return EE; 
    else if (abs(id1) == 13 && abs(id2) == 13) return MM; 
    else return EM; 
}

vector <pair<GenParticleStruct, GenParticleStruct> > makeGenHyps(){

  //List Gen Particles
  vector <GenParticleStruct> gleps;
  for (int iGen = 0; (unsigned)iGen < tas::genps_p4().size(); iGen++) {

    //Variables
    int   id  = tas::genps_id().at(iGen);
    float pt  = tas::genps_p4().at(iGen).pt();
    float eta = tas::genps_p4().at(iGen).eta();

    //Require that it is from a W
    if (abs(tas::genps_id_mother().at(iGen)) != 24) continue;

    //Eta cut
    if (fabs(eta) > 2.4) continue;

    //Electrons and Muons
    if (abs(id) == 11 || abs(id) == 13) {
      GenParticleStruct temp = {id, iGen, pt, eta, -999, 999, -999., 999.};
      gleps.push_back(temp);
    }

    //Taus
    if (abs(id) == 15){
      GenParticleStruct tmp_gp = {0,0,0,0,0,0,0,0};
      for (int didx = 0; (unsigned)didx < tas::genps_lepdaughter_id().at(iGen).size(); didx++){
        int did = tas::genps_lepdaughter_id().at(iGen).at(didx);
        if (abs(did) != 11 && abs(did) != 13) continue;
        if (fabs(tas::genps_lepdaughter_p4().at(iGen).at(didx).eta()) > 2.4) continue;
        float dpt  = tas::genps_lepdaughter_p4().at(iGen).at(didx).pt();
        float deta = tas::genps_lepdaughter_p4().at(iGen).at(didx).eta();
        GenParticleStruct temp2 = {id, iGen, pt, eta, did, didx, dpt, deta};
        if (dpt > tmp_gp.dpt) tmp_gp = temp2;
      }
      if (tmp_gp.dpt > 0.1) gleps.push_back(tmp_gp);
    }

  }//gen loop

  //Now make gen hyps
  vector <pair <GenParticleStruct, GenParticleStruct> > glepPairs;
  for (unsigned int idx1 = 0; idx1 < gleps.size(); idx1++){
    for (unsigned int idx2 = idx1 + 1; idx2 < gleps.size(); idx2++){
      glepPairs.push_back(make_pair(gleps.at(idx1), gleps.at(idx2)));
    }
  }

  return glepPairs;
}

std::pair<GenParticleStruct, GenParticleStruct> getGenHyp(float min_pt_elec, float min_pt_muon){
                
    vector <pair<GenParticleStruct, GenParticleStruct> > glepPairs = makeGenHyps();
    unsigned int npairs = glepPairs.size();

    GenParticleStruct gp = {0, 999999, 0., 0., 0, 999999, 0., 0.};
    pair<GenParticleStruct, GenParticleStruct> good_gen_hyp = make_pair(gp, gp);
    hyp_type_t good_hyp_type = UNASSIGNED;
                
    for (unsigned int idx = 0; idx < npairs; idx++){
            
        GenParticleStruct gp1 = glepPairs.at(idx).first;
        GenParticleStruct gp2 = glepPairs.at(idx).second;

        float gpt1  = (abs(gp1.id) == 15) ? gp1.dpt : gp1.pt;
        int gid1    = (abs(gp1.id) == 15) ? gp1.did : gp1.id;
        float gpt2  = (abs(gp2.id) == 15) ? gp2.dpt : gp2.pt;
        int gid2    = (abs(gp2.id) == 15) ? gp2.did : gp2.id;
    
        if (abs(gp1.id) == 11 && gp1.pt < min_pt_elec) continue;     
        if (abs(gp2.id) == 11 && gp2.pt < min_pt_elec) continue;     
        if (abs(gp1.id) == 13 && gp1.pt < min_pt_muon) continue;     
        if (abs(gp2.id) == 13 && gp2.pt < min_pt_muon) continue;     
        if (abs(gp1.id) == 15 && abs(gp1.did) == 11 && gp1.pt < min_pt_elec) continue;
        if (abs(gp2.id) == 15 && abs(gp2.did) == 11 && gp2.pt < min_pt_elec) continue;
        if (abs(gp1.id) == 15 && abs(gp1.did) == 13 && gp1.pt < min_pt_muon) continue;
        if (abs(gp2.id) == 15 && abs(gp2.did) == 13 && gp2.pt < min_pt_muon) continue;
    
        hyp_type_t tmp_type = getHypType(gid1, gid2);
        if (tmp_type == UNASSIGNED) continue;
        if (good_hyp_type == UNASSIGNED){
            good_hyp_type = tmp_type;
            good_gen_hyp = glepPairs.at(idx);
        }
        else if (tmp_type < good_hyp_type) {
            good_hyp_type = tmp_type;
            good_gen_hyp = glepPairs.at(idx);
        }
        else if (tmp_type == good_hyp_type) {

            float ggpt1 = (abs(good_gen_hyp.first.id) == 15) ? good_gen_hyp.first.dpt : good_gen_hyp.first.pt;
            float ggpt2 = (abs(good_gen_hyp.second.id) == 15) ? good_gen_hyp.second.dpt : good_gen_hyp.second.pt;

            if ( (gpt1+gpt2) > (ggpt1+ggpt2) ) {
                good_hyp_type = tmp_type;
                good_gen_hyp = glepPairs.at(idx);
            }
        }
    }// end loop over gen hyps
    return good_gen_hyp;
}

vector<LorentzVector> getGenJets(float pt_cut, float eta_cut, float muon_pt_cut, float elec_pt_cut, float deltaR){

  //Return LorentzVectors of jets
  vector<LorentzVector> tmp_vec;

  //Loop over jets
  for (unsigned int jidx = 0; jidx < tas::genjets_p4NoMuNoNu().size(); jidx++){
  
    //Jet
    LorentzVector vjet = tas::genjets_p4NoMuNoNu().at(jidx);

    //Kinematic variables
    if (vjet.pt() < pt_cut) continue;
    if (fabs(vjet.eta()) > eta_cut) continue;
    
    //Remove jets that overlap with leptons
    bool jetislep = false;
    for (size_t iGen = 0; iGen < tas::genps_p4().size(); iGen++){
      if (abs(tas::genps_id_mother().at(iGen)) != 24) continue;
      if (abs(tas::genps_status().at(iGen)) != 1) continue;
      if (abs(tas::genps_id().at(iGen)) != 11 && abs(tas::genps_id().at(iGen)) != 13) continue;
      if (fabs(tas::genps_p4().at(iGen).eta()) < 2.4) continue;
      if (abs(tas::genps_id().at(iGen)) == 11 && tas::genps_p4().at(iGen).pt() < elec_pt_cut) continue;
      if (abs(tas::genps_id().at(iGen)) == 13 && tas::genps_p4().at(iGen).pt() < muon_pt_cut) continue;
      if (ROOT::Math::VectorUtil::DeltaR(vjet, tas::genps_p4().at(iGen)) < deltaR){ jetislep = true; break; }
    }   
    if (jetislep == true) continue;

    //If we get this far, good gen jet
    tmp_vec.push_back(vjet);

  }//for-loop

  return tmp_vec;
}

void babymaker(){

  TChain *chain = new TChain("Events"); 
  chain->Add("/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2/SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/V07-04-11/merged_ntuple_*.root"); 

  //Baby tree
  TFile* BabyFile = new TFile("outreachbaby.root", "RECREATE");
  BabyFile->cd();
  TTree* BabyTree = new TTree("t", "SS2015 Outreach Ntuple");

  //Variables
  float htGen = 0;
  int nGenJets = 0;
  vector <LorentzVector> genJets;
  int genLep1_id = -9999;
  int genLep2_id = -9999;
  int genLep1_idx = -9999;
  int genLep2_idx = -9999;
  bool lep1_passID = 0;
  bool lep2_passID = 0;
  LorentzVector pt1_reco;
  LorentzVector pt2_reco;
  int id1_reco    = -1;
  int id2_reco    = -1;     
  int idx1_reco   = -1;
  int idx2_reco   = -1;     
  float genmet    = -1;
  LorentzVector genLep1_p4;
  LorentzVector genLep2_p4;

  //Baby branches
  BabyTree->Branch("genLep1_id" , &genLep1_id );
  BabyTree->Branch("genLep2_id" , &genLep2_id );
  BabyTree->Branch("genLep1_idx", &genLep1_idx);
  BabyTree->Branch("genLep2_idx", &genLep2_idx);
  BabyTree->Branch("htGen"      , &htGen      );
  BabyTree->Branch("genJets"    , &genJets    );
  BabyTree->Branch("nGenJets"   , &nGenJets   );
  BabyTree->Branch("genmet"     , &genmet     );
  BabyTree->Branch("lep1_passID", &lep1_passID);
  BabyTree->Branch("lep2_passID", &lep2_passID);
  BabyTree->Branch("pt1_reco"   , &pt1_reco   );
  BabyTree->Branch("pt2_reco"   , &pt2_reco   );
  BabyTree->Branch("id1_reco"   , &pt1_reco   );
  BabyTree->Branch("id2_reco"   , &pt2_reco   );
  BabyTree->Branch("idx1_reco"  , &idx1_reco  );
  BabyTree->Branch("idx2_reco"  , &idx2_reco  );
  BabyTree->Branch("genLep1_p4" , &genLep1_p4 ); 
  BabyTree->Branch("genLep2_p4" , &genLep2_p4 ); 

  //MVA function
  createAndInitMVA("../CORE", true);

  //nEvents in chain
  unsigned int nEventsTotal = 0; 
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0; 

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("Events");
    cms3.Init(tree);

    //Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntries(); event++){

      //Get Event Content
      cms3.GetEntry(event);
      nEventsTotal++;

      //Progress
      CMS3::progress(nEventsTotal, nEventsChain);

      //Find gen particles, jets
      std::pair<GenParticleStruct, GenParticleStruct> genHyp = getGenHyp(15, 10);
      if (genHyp.first.id == 0) continue;
      GenParticleStruct GenParticle1 = genHyp.first;
      GenParticleStruct GenParticle2 = genHyp.second;
      genLep1_id = (abs(GenParticle1.id) == 15) ? GenParticle1.did : GenParticle1.id;
      genLep2_id = (abs(GenParticle2.id) == 15) ? GenParticle2.did : GenParticle2.id;
      genLep1_idx = (abs(GenParticle1.id) == 15) ? GenParticle1.didx : GenParticle1.idx;
      genLep2_idx = (abs(GenParticle2.id) == 15) ? GenParticle2.didx : GenParticle2.idx;
      genJets = getGenJets(); 
      nGenJets = genJets.size(); 
      htGen = 0;
      for (int i = 0; i < nGenJets; i++) htGen += genJets.at(i).pt(); 
      genmet = tas::gen_met(); 
      genLep1_p4 = tas::genps_p4().at(genLep1_idx); 
      genLep2_p4 = tas::genps_p4().at(genLep2_idx); 

      //Determine whether leptons are reconstructed.  Loop over all reco particles to see if any of them correspond to the gen leptons.
      id1_reco    = -1;
      id2_reco    = -1;     
      idx1_reco   = -1;
      idx2_reco   = -1;     
      float dR_best_1 = 0.1; 
      float dR_best_2 = 0.1; 
      for (size_t ilep = 0; ilep < tas::els_p4().size(); ilep++){
        float dR_1 = DeltaR(tas::els_p4().at(ilep), tas::genps_p4().at(genLep1_idx)); 
        float dR_2 = DeltaR(tas::els_p4().at(ilep), tas::genps_p4().at(genLep2_idx)); 
        if (dR_1 < dR_best_1){ dR_best_1 = dR_1; idx1_reco = ilep; id1_reco = -11*tas::els_charge().at(ilep); } 
        if (dR_2 < dR_best_2){ dR_best_2 = dR_2; idx2_reco = ilep; id2_reco = -11*tas::els_charge().at(ilep); } 
      }
      for (size_t ilep = 0; ilep < tas::mus_p4().size(); ilep++){
        float dR_1 = DeltaR(tas::mus_p4().at(ilep), tas::genps_p4().at(genLep1_idx)); 
        float dR_2 = DeltaR(tas::mus_p4().at(ilep), tas::genps_p4().at(genLep2_idx)); 
        if (dR_1 < dR_best_1){ dR_best_1 = dR_1; idx1_reco = ilep; id1_reco = -13*tas::mus_charge().at(ilep); } ;
        if (dR_2 < dR_best_2){ dR_best_2 = dR_2; idx2_reco = ilep; id2_reco = -13*tas::mus_charge().at(ilep); } ;
      }

      //Record reco pTs
      if (idx1_reco >= 0) pt1_reco = (abs(id1_reco) == 11) ? tas::els_p4().at(idx1_reco) : tas::mus_p4().at(idx1_reco);
      if (idx2_reco >= 0) pt2_reco = (abs(id2_reco) == 11) ? tas::els_p4().at(idx2_reco) : tas::mus_p4().at(idx2_reco);
 
      //See if reco lepton passes ID
      lep1_passID = (idx1_reco >= 0 && isGoodLepton(id1_reco, idx1_reco));
      lep2_passID = (idx2_reco >= 0 && isGoodLepton(id2_reco, idx2_reco));
    
      BabyTree->Fill();

    }//event loop

  }//file loop

  BabyFile->cd(); 
  BabyTree->Write(); 
  BabyFile->Close(); 
  
}
