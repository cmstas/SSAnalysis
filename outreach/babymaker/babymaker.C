#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TColor.h"
#include "TH1F.h"
#include "../CORE/Tools/utils.h"
#include "../CORE/SSSelections.h"
#include "../CORE/MCSelections.h"
#include "../CORE/CMS3.h"
#include "babymaker.h"

//bool tas::passHLTTrigger(TString trigName) { return cms3.tas::passHLTTrigger(trigName); }

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

    GenParticleStruct gp = {0, -999999, 0., 0., 0, -999999, 0., 0.};
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
  vector <bool> genJets_isb;
  vector <LorentzVector> recoJets;
  vector <int> genLep_id;
  vector <int> genLep_idx;
  vector <bool> lep_passID; 
  vector <bool> lep_passID_loose; 
  vector <LorentzVector> recoLep_p4;
  vector <int> id_reco;
  vector <int> idx_reco;
  float genmet = -1;
  float met = -1;
  vector <LorentzVector> genLep_p4;
  vector <int> genJets_matched; 
  vector <int> genJets_matched_tob; 
  vector <int> genJets_matchedID; 
  vector <LorentzVector> pfjets_p4; 
  vector <bool> pfjets_isClean; 
  vector <bool> pfjets_isb; 
  vector <LorentzVector> recoMuons;
  vector <LorentzVector> recoElectrons;
  vector <int> recoMuonsMatch;
  vector <bool> recoMuonsID;
  vector <bool> recoMuonsID_loose;
  vector <int> recoElectronsMatch;
  vector <bool> recoElectronsID;
  vector <bool> recoElectronsID_loose;
  vector <int> pfjets_match; 
  vector <bool> pfjets_matchb; 
  vector <bool> pfjets_ID; 
  LorentzVector genLep1_p4;
  LorentzVector genLep2_p4;
  LorentzVector recoLep1_p4;
  LorentzVector recoLep2_p4;
  int genLep1_id; 
  int genLep2_id; 
  bool lep1_passID; 
  bool lep2_passID; 
  int id1_reco;
  int id2_reco;
  int idx1_reco;
  int idx2_reco;
  bool fired_trigger_1; 
  bool fired_trigger_2; 

  //Baby branches
  BabyTree->Branch("genLep_id"             , &genLep_id             );
  BabyTree->Branch("genLep_idx"            , &genLep_idx            );
  BabyTree->Branch("htGen"                 , &htGen                 );
  BabyTree->Branch("ht"                    , &ht                    );
  BabyTree->Branch("genJets"               , &genJets               );
  BabyTree->Branch("genJets_isb"           , &genJets_isb           );
  BabyTree->Branch("recoJets"              , &recoJets              );
  BabyTree->Branch("nGenJets"              , &nGenJets              );
  BabyTree->Branch("genmet"                , &genmet                );
  BabyTree->Branch("met"                   , &met                   );
  BabyTree->Branch("lep_passID"            , &lep_passID            );
  BabyTree->Branch("lep_passID_loose"      , &lep_passID_loose      );
  BabyTree->Branch("id_reco"               , &id_reco               );
  BabyTree->Branch("idx_reco"              , &idx_reco              );
  BabyTree->Branch("genLep_p4"             , &genLep_p4             );
  BabyTree->Branch("recoLep_p4"            , &recoLep_p4            );
  BabyTree->Branch("genJets_matched"       , &genJets_matched       );
  BabyTree->Branch("genJets_matched_tob"   , &genJets_matched_tob   );
  BabyTree->Branch("genJets_matchedID"     , &genJets_matchedID     );
  BabyTree->Branch("pfjets_p4"             , &pfjets_p4             );
  BabyTree->Branch("pfjets_isClean"        , &pfjets_isClean        );
  BabyTree->Branch("pfjets_isb"            , &pfjets_isb            );
  BabyTree->Branch("recoMuons"             , &recoMuons             );
  BabyTree->Branch("recoElectrons"         , &recoElectrons         );
  BabyTree->Branch("recoMuonsMatch"        , &recoMuonsMatch        );
  BabyTree->Branch("recoMuonsID"           , &recoMuonsID           );
  BabyTree->Branch("recoMuonsID_loose"     , &recoMuonsID_loose     );
  BabyTree->Branch("recoElectronsMatch"    , &recoElectronsMatch    );
  BabyTree->Branch("recoElectronsID"       , &recoElectronsID       );
  BabyTree->Branch("recoElectronsID_loose" , &recoElectronsID_loose );
  BabyTree->Branch("pfjets_match"          , &pfjets_match          );
  BabyTree->Branch("pfjets_matchb"         , &pfjets_matchb         );
  BabyTree->Branch("pfjets_ID"             , &pfjets_ID             );
  BabyTree->Branch("genLep1_p4"            , &genLep1_p4            );
  BabyTree->Branch("genLep2_p4"            , &genLep2_p4            );
  BabyTree->Branch("recoLep1_p4"           , &recoLep1_p4           );
  BabyTree->Branch("recoLep2_p4"           , &recoLep2_p4           );
  BabyTree->Branch("genLep1_id"            , &genLep1_id            );
  BabyTree->Branch("genLep2_id"            , &genLep2_id            );
  BabyTree->Branch("lep1_passID"           , &lep1_passID           );
  BabyTree->Branch("lep2_passID"           , &lep2_passID           );
  BabyTree->Branch("id1_reco"              , &id1_reco              );
  BabyTree->Branch("id2_reco"              , &id2_reco              );
  BabyTree->Branch("idx1_reco"             , &idx1_reco             );
  BabyTree->Branch("idx2_reco"             , &idx2_reco             );
  BabyTree->Branch("fired_trigger_1"       , &fired_trigger_1       );
  BabyTree->Branch("fired_trigger_2"       , &fired_trigger_2       );

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
      genJets_matched_tob.clear(); 
      genJets_matchedID.clear(); 
      pfjets_p4.clear();
      pfjets_isClean.clear();
      pfjets_isb.clear();
      genLep_id.clear();
      genLep_p4.clear();
      genLep_idx.clear();
      id_reco.clear();
      idx_reco.clear();
      lep_passID.clear();
      lep_passID_loose.clear();
      recoLep_p4.clear();  
      recoElectronsID.clear();
      recoElectronsID_loose.clear();
      recoElectronsMatch.clear();
      recoMuonsID.clear();
      recoMuonsID_loose.clear();
      recoMuonsMatch.clear();
      pfjets_match.clear(); 
      pfjets_matchb.clear(); 
      pfjets_ID.clear();
      genJets_isb.clear(); 
      genLep1_p4 = {0,0,0,0};
      genLep2_p4 = {0,0,0,0};
      recoLep1_p4 = {0,0,0,0};
      recoLep2_p4 = {0,0,0,0};
      lep1_passID = 0; 
      lep2_passID = 0; 
      genLep1_id = 0;
      genLep2_id = 0;

      //Find Gen pair
      std::pair<GenParticleStruct, GenParticleStruct> genHyp = getGenHyp(15, 10);  
      if (genHyp.first.id != 0){
        //Particles
        GenParticleStruct GenParticle1 = genHyp.first;
        GenParticleStruct GenParticle2 = genHyp.second;
        genLep1_id  = (abs(GenParticle1.id) == 15) ? GenParticle1.did : GenParticle1.id;
        genLep2_id  = (abs(GenParticle2.id) == 15) ? GenParticle2.did : GenParticle2.id;
        int genLep1_idx = (abs(GenParticle1.id) == 15) ? GenParticle1.didx : GenParticle1.idx;
        int genLep2_idx = (abs(GenParticle2.id) == 15) ? GenParticle2.didx : GenParticle2.idx;
        genLep1_p4  = tas::genps_p4().at(genLep1_idx);
        genLep2_p4  = tas::genps_p4().at(genLep2_idx);
        genLep1_id  = genHyp.first.id; 
        genLep2_id  = genHyp.second.id; 
        id1_reco    = -1;
        id2_reco    = -1;     
        idx1_reco   = -1;
        idx2_reco   = -1;     

        //See if gen pair is reconstructed
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
        lep1_passID = (idx1_reco >= 0 && isGoodLepton(id1_reco, idx1_reco));
        lep2_passID = (idx2_reco >= 0 && isGoodLepton(id2_reco, idx2_reco));
        if (abs(id1_reco) == 11) recoLep1_p4 = tas::els_p4().at(idx1_reco); 
        if (abs(id2_reco) == 11) recoLep2_p4 = tas::els_p4().at(idx2_reco); 
        if (abs(id1_reco) == 13) recoLep1_p4 = tas::mus_p4().at(idx1_reco); 
        if (abs(id2_reco) == 13) recoLep2_p4 = tas::mus_p4().at(idx2_reco); 
      }

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
        pfjets_isb.push_back((tas::pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(i) > .89)); 
      }

      //Determine whether gen jet is a b or not
      for (unsigned int iJet = 0; iJet < genJets.size(); iJet++){
        bool isb = false;        
        for (unsigned int iGen = 0; iGen < tas::genps_p4().size(); iGen++){
          if (abs(tas::genps_id().at(iGen)) != 5) continue;
          float dR = DeltaR(genJets.at(iJet), tas::genps_p4().at(iGen)); 
          if (dR < 0.4){ isb = true; break; } 
        }
        genJets_isb.push_back(isb);  
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
        genJets_matched_tob.push_back(idx_matched < 0 ? 0 : tas::pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(idx_matched) > 0.89);
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
        int iReco = idx_reco.at(iGen);
        if (iReco < 0){ lep_passID.push_back(0); lep_passID_loose.push_back(0); continue; }
        lep_passID.push_back(isGoodLeptonNoIso(id_reco.at(iGen), iReco));
        lep_passID_loose.push_back((abs(id_reco.at(iGen)) == 11 ? isLooseElectronPOG(iReco) : isLooseMuonPOG(iReco)));
      }

      //Mistags: go through reco electrons and see which ones are mapped to gen
      recoElectrons = tas::els_p4(); 
      for (unsigned int iElec = 0; iElec < tas::els_p4().size(); iElec++){
        float dR_best = 0.1; 
        int match = -1;
        for (unsigned int iGen = 0; iGen < tas::genps_id().size(); iGen++){
          if (abs(tas::genps_id().at(iGen)) != 11) continue;
          float dR = DeltaR(tas::genps_p4().at(iGen), tas::els_p4().at(iElec));  
          if (dR < dR_best){ dR_best = dR; match = iGen; } 
        }
        recoElectronsMatch.push_back(match); 
        recoElectronsID.push_back(isGoodLeptonNoIso(11, iElec)); 
        recoElectronsID_loose.push_back(isLooseElectronPOG(iElec)); 
      }

      //Mistags: same thing for muons
      recoMuons = tas::mus_p4(); 
      for (unsigned int iMuon = 0; iMuon < tas::mus_p4().size(); iMuon++){
        float dR_best = 0.1; 
        int match = -1;
        for (unsigned int iGen = 0; iGen < tas::genps_id().size(); iGen++){
          if (abs(tas::genps_id().at(iGen)) != 13) continue;
          float dR = DeltaR(tas::genps_p4().at(iGen), tas::mus_p4().at(iMuon));  
          if (dR < dR_best){ dR_best = dR; match = iGen; } 
        }
        recoMuonsMatch.push_back(match); 
        recoMuonsID.push_back(isGoodLeptonNoIso(13, iMuon)); 
        recoMuonsID_loose.push_back(isLooseMuonPOG(iMuon)); 
      }

      //Mistags: same thing for jets
      for (unsigned int iJet = 0; iJet < tas::pfjets_p4().size(); iJet++){
        float dR_best = 0.1; 
        int match = -1;
        for (unsigned int iGen = 0; iGen < tas::genjets_p4NoMuNoNu().size(); iGen++){
          float dR = DeltaR(tas::genjets_p4NoMuNoNu().at(iGen), tas::pfjets_p4().at(iJet));  
          if (dR < dR_best){ dR_best = dR; match = iGen; } 
        }
        pfjets_match.push_back(match); 
        pfjets_ID.push_back(isLoosePFJet_50nsV1(iJet));
      }

      //Mistags: same thing for btags
      for (unsigned int iJet = 0; iJet < tas::pfjets_p4().size(); iJet++){
        bool isb = 0;
        for (unsigned int iGen = 0; iGen < tas::genps_id().size(); iGen++){
          if (abs(tas::genps_id().at(iGen)) != 5 && abs(tas::genps_id().at(iGen)) != 6) continue;
          float dR = DeltaR(tas::genps_p4().at(iGen), tas::pfjets_p4().at(iJet));  
          if (dR < 0.4) isb = 1; 
        }
        pfjets_matchb.push_back(isb); 
      }

      //Record which reco jets overlap with leptons
  
      for (unsigned int iJet = 0; iJet < tas::pfjets_p4().size(); iJet++){
        LorentzVector vjet = tas::pfjets_p4().at(iJet); 
        bool jetisClean = true;
        for (size_t iGen = 0; iGen < tas::genps_p4().size(); iGen++){
          if (abs(tas::genps_id_mother().at(iGen)) != 24) continue;
          if (abs(tas::genps_status().at(iGen)) != 1) continue;
          if (abs(tas::genps_id().at(iGen)) != 11 && abs(tas::genps_id().at(iGen)) != 13) continue;
          if (fabs(tas::genps_p4().at(iGen).eta()) < 2.4) continue;
          if (abs(tas::genps_id().at(iGen)) == 11 && tas::genps_p4().at(iGen).pt() < 10) continue;
          if (abs(tas::genps_id().at(iGen)) == 13 && tas::genps_p4().at(iGen).pt() < 10) continue;
          if (ROOT::Math::VectorUtil::DeltaR(vjet, tas::genps_p4().at(iGen)) < 0.1){ jetisClean = false; break; }
        }   
        pfjets_isClean.push_back(jetisClean); 
      }

      //Trigger stuff
      if (genHyp.first.id != 0){
        fired_trigger_1 = false;
        fired_trigger_2 = false;
        int lep1_id = id1_reco;
        int lep2_id = id2_reco;
        int lep1_idx = idx1_reco;
        int lep2_idx = idx2_reco;
        LorentzVector lep1_p4 = recoLep1_p4;
        LorentzVector lep2_p4 = recoLep2_p4;
        bool lep1_trigMatch_isoReq = 0;
        bool lep2_trigMatch_isoReq = 0;
        bool lep1_trigMatch_noIsoReq = 0;
        bool lep2_trigMatch_noIsoReq = 0;
  
        //Electron Trigger Stuff
        if (abs(lep1_id) == 11 && lep1_idx >= 0){
          //Double electron triggers
          if (tas::els_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_ElectronLeg().at(lep1_idx) > 0) lep1_trigMatch_noIsoReq = true;
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "hltEle17Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg1Filter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;
            if (matchToHLTFilter("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "hltEle17Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg2Filter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;
          }
          //Mu-El triggers
          if (tas::els_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_ElectronLeg().at(lep1_idx) > 0) lep1_trigMatch_noIsoReq = true;
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
          }
        }
        if (abs(lep2_id) == 11 && lep2_idx >= 0){
          //Double electron triggers
            if (tas::els_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_ElectronLeg().at(lep2_idx) > 0) lep2_trigMatch_noIsoReq = true;
          { 
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "hltEle17Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg1Filter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;
            if (matchToHLTFilter("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "hltEle17Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg2Filter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;
          }
          //Mu-El triggers
          if (tas::els_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_ElectronLeg().at(lep2_idx) > 0) lep2_trigMatch_noIsoReq = true;
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
          }      
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
          }      
        }

        //Muon trigger stuff
        if (abs(lep1_id) == 13 && lep1_idx >= 0){
          //Double muon triggers
          if (tas::mus_HLT_DoubleMu8_Mass8_PFHT300_MuonLeg().at(lep1_idx) > 0) lep1_trigMatch_noIsoReq = true;
          if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v"), lep1_p4) > 0)             lep1_trigMatch_isoReq = true;
          if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"), lep1_p4) > 0)           lep1_trigMatch_isoReq = true;
          //Mu-El triggers
          if (tas::mus_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_MuonLeg().at(lep1_idx) > 0) lep1_trigMatch_noIsoReq = true;
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
          }
        }
        if (abs(lep2_id) == 13 && lep2_idx >= 0){
          //Double muon triggers
          if (tas::mus_HLT_DoubleMu8_Mass8_PFHT300_MuonLeg().at(lep2_idx) > 0) lep2_trigMatch_noIsoReq = true;
          if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v"), lep2_p4) > 0)             lep2_trigMatch_isoReq = true;
          if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"), lep2_p4) > 0)           lep2_trigMatch_isoReq = true;
          //Mu-El triggers
          if (tas::mus_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_MuonLeg().at(lep2_idx) > 0) lep2_trigMatch_noIsoReq = true;
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
          }
          {
            float dummypt = 0.;
            if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
          }
        }
        if (ht < 300 && lep1_trigMatch_isoReq) fired_trigger_1 = true; 
        if (ht < 300 && lep2_trigMatch_isoReq) fired_trigger_2 = true; 
        if (ht > 300 && lep1_trigMatch_noIsoReq) fired_trigger_1 = true; 
        if (ht > 300 && lep2_trigMatch_noIsoReq) fired_trigger_2 = true; 
      }

      BabyTree->Fill();

    }//event loop

  }//file loop

  BabyFile->cd(); 
  BabyTree->Write(); 
  BabyFile->Close(); 
  
}
