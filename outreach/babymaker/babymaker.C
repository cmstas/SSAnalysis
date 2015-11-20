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

vector <GenParticleStruct> makeGenParticles(){

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

  return gleps;

}

vector <pair<GenParticleStruct, GenParticleStruct> > makeGenHyps(){

  vector <GenParticleStruct> gleps = makeGenParticles(); 

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
  float ht = 0;
  int nGenJets = 0;
  vector <LorentzVector> genJets;
  vector <LorentzVector> recoJets;
  vector <int> genLep_id;
  vector <int> genLep_idx;
  vector <bool> lep_passID; 
  vector <LorentzVector> recoLep_p4;
  vector <int> id_reco;
  vector <int> idx_reco;
  float genmet = -1;
  float met = -1;
  vector <LorentzVector> genLep_p4;
  vector <int> genJets_matched; 
  vector <int> genJets_matchedID; 
  vector <LorentzVector> pfjets_p4; 

  //Baby branches
  BabyTree->Branch("genLep_id"         , &genLep_id        );
  BabyTree->Branch("genLep_idx"        , &genLep_idx       );
  BabyTree->Branch("htGen"             , &htGen            );
  BabyTree->Branch("ht"                , &ht               );
  BabyTree->Branch("genJets"           , &genJets          );
  BabyTree->Branch("recoJets"          , &recoJets         );
  BabyTree->Branch("nGenJets"          , &nGenJets         );
  BabyTree->Branch("genmet"            , &genmet           );
  BabyTree->Branch("met"               , &met              );
  BabyTree->Branch("lep_passID"        , &lep_passID       );
  BabyTree->Branch("id_reco"           , &id_reco          );
  BabyTree->Branch("idx_reco"          , &idx_reco         );
  BabyTree->Branch("genLep_p4"         , &genLep_p4        );
  BabyTree->Branch("recoLep_p4"        , &recoLep_p4       );
  BabyTree->Branch("genJets_matched"   , &genJets_matched  );
  BabyTree->Branch("genJets_matchedID" , &genJets_matchedID);
  BabyTree->Branch("pfjets_p4"         , &pfjets_p4        );

  //MVA function
  createAndInitMVA("../CORE", true);

  //Jet Corrections
  std::vector<std::string> filenames;
  filenames.push_back("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV6_MC_L1FastJet_AK4PFchs.txt");
  filenames.push_back("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV6_MC_L2Relative_AK4PFchs.txt");
  filenames.push_back("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV6_MC_L3Absolute_AK4PFchs.txt");
  FactorizedJetCorrector *jetCorr;
  jetCorr = makeJetCorrector(filenames);

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
   
      //Initialize variables
      recoJets.clear(); 
      htGen = 0;
      ht = 0;
      genJets_matched.clear(); 
      genJets_matchedID.clear(); 
      pfjets_p4.clear();
      genLep_id.clear();
      genLep_p4.clear();
      genLep_idx.clear();
      id_reco.clear();
      idx_reco.clear();
      lep_passID.clear();
      recoLep_p4.clear();  

      //Find gen particles, jets
      vector <GenParticleStruct> genParticles = makeGenParticles();
      if (genParticles.size() == 0) continue;
      for (unsigned int i = 0; i < genParticles.size(); i++){
        GenParticleStruct GenParticle = genParticles.at(i); 
        float genLep_id_ = (abs(GenParticle.id) == 15) ? GenParticle.did : GenParticle.id;
        float genLep_idx_ = (abs(GenParticle.id) == 15) ? GenParticle.didx : GenParticle.idx;
        LorentzVector genLep_p4_ = tas::genps_p4().at(genLep_idx_); 
        genLep_id.push_back(genLep_id_); 
        genLep_idx.push_back(genLep_idx_); 
        genLep_p4.push_back(genLep_p4_); 
      }
      genJets = getGenJets(); 
      nGenJets = genJets.size(); 
      for (int i = 0; i < nGenJets; i++) htGen += genJets.at(i).pt(); 
      genmet = tas::gen_met(); 
      pair <float, float> T1CHSMET = getT1CHSMET_fromMINIAOD(jetCorr);
      met = T1CHSMET.first;
      std::pair <vector <Jet>, vector <Jet> > jet_results = SSJetsCalculator(jetCorr, 1);
      for (unsigned int i = 0; i < jet_results.first.size(); i++){
        ht += jet_results.first.at(i).p4().pt()*jet_results.first.at(i).undo_jec()*jet_results.first.at(i).jec(); 
        recoJets.push_back(jet_results.first.at(i).p4()*jet_results.first.at(i).undo_jec()*jet_results.first.at(i).jec()); 
      }
      for (unsigned int i = 0; i < tas::pfjets_p4().size(); i++){
        LorentzVector jet = tas::pfjets_p4().at(i);
        jetCorr->setJetEta(jet.eta()); 
        jetCorr->setJetPt(jet.pt()*tas::pfjets_undoJEC().at(i)); 
        jetCorr->setJetA(tas::pfjets_area().at(i)); 
        jetCorr->setRho(tas::evt_fixgridfastjet_all_rho()); 
        float JEC = jetCorr->getCorrection(); 
        pfjets_p4.push_back(jet*JEC*tas::pfjets_undoJEC().at(i)); 
      }

      //Determine whether leptons are reconstructed.  Loop over all reco particles to see if any of them correspond to the gen leptons.
      for (unsigned int iGen = 0; iGen < genLep_id.size(); iGen++){
        int id_reco_    = -1;
        int idx_reco_   = -1;
        float dR_best = 0.1; 
        for (unsigned int ilep = 0; ilep < tas::els_p4().size(); ilep++){
          float dR = DeltaR(tas::els_p4().at(ilep), tas::genps_p4().at(genLep_idx.at(iGen))); 
          if (dR < dR_best){ dR_best = dR; idx_reco_ = ilep; id_reco_ = -11*tas::els_charge().at(ilep); } 
        }
        for (unsigned int ilep = 0; ilep < tas::mus_p4().size(); ilep++){
          float dR = DeltaR(tas::mus_p4().at(ilep), tas::genps_p4().at(genLep_idx.at(iGen))); 
          if (dR < dR_best){ dR_best = dR; idx_reco_ = ilep; id_reco_ = -13*tas::mus_charge().at(ilep); } 
        }
        id_reco.push_back(id_reco_); 
        idx_reco.push_back(idx_reco_); 
      }

      //For each gen jet, loop over all reco jets to see if any of them correspond to the gen jets (NO ID)
      for (unsigned int iJet = 0; iJet < genJets.size(); iJet++){
        int idx_matched = -1; 
        float dR_best = 0.1;
        for (unsigned int iReco = 0; iReco < tas::pfjets_p4().size(); iReco++){
          float dR = DeltaR(tas::pfjets_p4().at(iReco), genJets.at(iJet));
          if (dR < dR_best){ dR_best = dR; idx_matched = iReco; } 
        }
        genJets_matched.push_back(idx_matched); 
      }

      //And now the same thing (FULL ID)
      for (unsigned int iJet = 0; iJet < genJets.size(); iJet++){
        int idx_matched = -1; 
        float dR_best = 0.1;
        for (unsigned int iReco = 0; iReco < recoJets.size(); iReco++){
          float dR = DeltaR(recoJets.at(iReco), genJets.at(iJet));
          if (dR < dR_best){ dR_best = dR; idx_matched = iReco; } 
        }
        genJets_matchedID.push_back(idx_matched); 
      }

      //Record reco pTs
      LorentzVector null = {0,0,0,0}; 
      for (unsigned int iGen = 0; iGen < idx_reco.size(); iGen++){
        if (idx_reco.at(iGen) >= 0) recoLep_p4.push_back((abs(id_reco.at(iGen)) == 11) ? tas::els_p4().at(idx_reco.at(iGen)) : tas::mus_p4().at(idx_reco.at(iGen)));
        else recoLep_p4.push_back(null);
      }
 
      //See if reco lepton passes ID
      for (unsigned int iGen = 0; iGen < idx_reco.size(); iGen++){
        lep_passID.push_back((idx_reco.at(iGen) >= 0 && isGoodLepton(id_reco.at(iGen), idx_reco.at(iGen))));
      }

      BabyTree->Fill();

    }//event loop

  }//file loop

  BabyFile->cd(); 
  BabyTree->Write(); 
  BabyFile->Close(); 
  
}
