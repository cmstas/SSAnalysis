#include "helper_babymaker.h"

using namespace tas;

//Main functions
void babyMaker::MakeBabyNtuple(const char* output_name, IsolationMethods isoCase){

  //Create Baby
  string suffix = "";
  if (isoCase == PtRel) suffix = "_ptRel";
  else if (isoCase == MiniIso) suffix = "_miniIso";
  else if (isoCase == NewMiniIso) suffix = "_newMiniIso";
  BabyFile = new TFile(Form("%s/%s%s.root", path.Data(), output_name, suffix.c_str()), "RECREATE");
  BabyFile->cd();
  BabyTree = new TTree("t", "SS2015 Baby Ntuple");

  //Define Branches
  BabyTree->Branch("met", &met);
  BabyTree->Branch("metPhi", &metPhi);
  BabyTree->Branch("event", &event);
  BabyTree->Branch("lumi", &lumi);
  BabyTree->Branch("run", &run);
  BabyTree->Branch("filt_csc", &filt_csc);
  BabyTree->Branch("filt_hbhe", &filt_hbhe);
  BabyTree->Branch("filt_hcallaser", &filt_hcallaser);
  BabyTree->Branch("filt_ecaltp", &filt_ecaltp);
  BabyTree->Branch("filt_trkfail", &filt_trkfail);
  BabyTree->Branch("filt_eebadsc", &filt_eebadsc);
  BabyTree->Branch("is_real_data", &is_real_data);
  BabyTree->Branch("scale1fb", &scale1fb);
  BabyTree->Branch("xsec", &xsec);
  BabyTree->Branch("kfactor", &kfactor);
  BabyTree->Branch("gen_met", &gen_met);
  BabyTree->Branch("gen_met_phi", &gen_met_phi);
  BabyTree->Branch("njets", &njets);
  BabyTree->Branch("hyp_class", &hyp_class);
  BabyTree->Branch("lep1_p4", &lep1_p4);
  BabyTree->Branch("lep2_p4", &lep2_p4);
  BabyTree->Branch("ht", &ht);
  BabyTree->Branch("lep1_motherID", &lep1_motherID);
  BabyTree->Branch("lep2_motherID", &lep2_motherID);
  BabyTree->Branch("lep1_mc_id", &lep1_mc_id);
  BabyTree->Branch("lep2_mc_id", &lep2_mc_id);
  BabyTree->Branch("lep1_id", &lep1_id);
  BabyTree->Branch("lep2_id", &lep2_id);
  BabyTree->Branch("lep1_idx", &lep1_idx);
  BabyTree->Branch("lep2_idx", &lep2_idx);
  BabyTree->Branch("jets", &jets);
  BabyTree->Branch("btags_disc", &btags_disc);
  BabyTree->Branch("jets_disc", &jets_disc);
  BabyTree->Branch("btags", &btags);
  BabyTree->Branch("nbtags", &nbtags);
  BabyTree->Branch("sf_dilepTrig_hpt", &sf_dilepTrig_hpt);
  BabyTree->Branch("sf_dilepTrig_lpt", &sf_dilepTrig_lpt);
  BabyTree->Branch("sf_dilepTrig_vlpt", &sf_dilepTrig_vlpt);
  BabyTree->Branch("hyp_type", &hyp_type);
  BabyTree->Branch("sf_dilep_eff", &sf_dilep_eff);
  BabyTree->Branch("mt", &mt);
  BabyTree->Branch("mt_l2", &mt_l2);
  BabyTree->Branch("mt2", &mt2);
  BabyTree->Branch("mGluino", &mGluino);
  BabyTree->Branch("mLSP", &mLSP);
  BabyTree->Branch("mSbottom", &mSbottom);
  BabyTree->Branch("mChargino", &mChargino);
  BabyTree->Branch("lep1_id_gen", &lep1_id_gen);
  BabyTree->Branch("lep2_id_gen", &lep2_id_gen);
  BabyTree->Branch("lep1_p4_gen", &lep1_p4_gen);
  BabyTree->Branch("lep2_p4_gen", &lep2_p4_gen);
  BabyTree->Branch("lep3_id", &lep3_id);
  BabyTree->Branch("lep3_idx", &lep3_idx);
  BabyTree->Branch("lep3_p4", &lep3_p4);
  BabyTree->Branch("lep3_quality", &lep3_quality);
  BabyTree->Branch("lep1_iso", &lep1_iso);
  BabyTree->Branch("lep2_iso", &lep2_iso);
  BabyTree->Branch("dilep_p4", &dilep_p4);
  BabyTree->Branch("genps_p4", &genps_p4);
  BabyTree->Branch("genps_id", &genps_id);
  BabyTree->Branch("genps_id_mother", &genps_id_mother);
  BabyTree->Branch("genps_status", &genps_status);
  BabyTree->Branch("genps_id_grandma", &genps_id_grandma);
  BabyTree->Branch("lep1_passes_id", &lep1_passes_id);
  BabyTree->Branch("lep2_passes_id", &lep2_passes_id);
  BabyTree->Branch("lep1_dxyPV", &lep1_dxyPV);
  BabyTree->Branch("lep2_dxyPV", &lep2_dxyPV);
  BabyTree->Branch("lep1_dZ", &lep1_dZ);
  BabyTree->Branch("lep2_dZ", &lep2_dZ);
  BabyTree->Branch("lep1_d0_err", &lep1_d0_err);
  BabyTree->Branch("lep2_d0_err", &lep2_d0_err);
  BabyTree->Branch("lep1_ip3d", &lep1_ip3d);
  BabyTree->Branch("lep2_ip3d", &lep2_ip3d);
  BabyTree->Branch("lep1_ip3d_err", &lep1_ip3d_err);
  BabyTree->Branch("lep2_ip3d_err", &lep2_ip3d_err);
  BabyTree->Branch("nVetoElectrons7", &nVetoElectrons7);
  BabyTree->Branch("nVetoElectrons10", &nVetoElectrons10);
  BabyTree->Branch("nVetoElectrons25", &nVetoElectrons25);
  BabyTree->Branch("nVetoMuons5", &nVetoMuons5);
  BabyTree->Branch("nVetoMuons10", &nVetoMuons10);
  BabyTree->Branch("nVetoMuons25", &nVetoMuons25);
  BabyTree->Branch("filename", &filename);
  BabyTree->Branch("lep1_ptrel_v0", &lep1_ptrel_v0);
  BabyTree->Branch("lep1_ptrel_v1", &lep1_ptrel_v1);
  BabyTree->Branch("lep2_ptrel_v0", &lep2_ptrel_v0);
  BabyTree->Branch("lep2_ptrel_v1", &lep2_ptrel_v1);
  BabyTree->Branch("lep1_miniIso", &lep1_miniIso);
  BabyTree->Branch("lep2_miniIso", &lep2_miniIso);
  BabyTree->Branch("jet_close_lep1", &jet_close_lep1);
  BabyTree->Branch("jet_close_lep2", &jet_close_lep2);

  //Print warning!
  cout << "Careful!! Path is " << path << endl;

}

void babyMaker::InitBabyNtuple(){

    //Gen variables
    met = -1;
    metPhi = -1;
    event = -1;
    lumi = -1;
    run = -1;
    filt_csc = 0;
    filt_hbhe = 0;
    filt_hcallaser = 0;
    filt_ecaltp = 0;
    filt_trkfail = 0;
    filt_eebadsc = 0;
    is_real_data = 0;
    scale1fb = -1;
    xsec = -1;
    kfactor = -1;
    gen_met = -1;
    gen_met_phi = -1;
    njets = -1;
    hyp_class = -1;
    ht = -1;
    lep1_motherID = 0;
    lep2_motherID = 0;
    lep1_mc_id = -1;
    lep2_mc_id = -1; 
    lep1_id = -1;
    lep2_id = -1;
    lep1_idx = -1;
    lep2_idx = -1;
    jets.clear();
    btags_disc.clear();
    jets_disc.clear();
    btags.clear();
    nbtags = -1;
    sf_dilepTrig_hpt = -1;
    sf_dilepTrig_lpt = -1;
    sf_dilepTrig_vlpt = -1;
    hyp_type = -1;
    sf_dilep_eff = -1;
    mt = -1;
    mt_l2 = -1;
    mt2 = -1;
    mGluino = -1;
    mLSP = -1;
    mSbottom = -1;
    mChargino = -1;
    lep1_id_gen = -1;
    lep2_id_gen = -1;
    lep3_id = -1;
    lep3_idx = -1;
    lep3_quality = -1;
    lep1_iso = -1;
    lep2_iso = -1;
    genps_p4.clear();
    genps_id.clear();
    genps_id_mother.clear();
    genps_status.clear();
    genps_id_grandma.clear();
    lep1_passes_id = false;
    lep2_passes_id = false;
    lep1_dxyPV = -999998;
    lep2_dxyPV = -999998;
    lep1_dZ = -999998;
    lep2_dZ = -999998;
    lep1_d0_err = -999998;
    lep2_d0_err = -999998;
    lep1_ip3d = -999998;
    lep2_ip3d = -999998;
    lep1_ip3d_err = -999998;
    lep2_ip3d_err = -999998;
    nVetoElectrons7 = 0;
    nVetoElectrons10 = 0;
    nVetoElectrons25 = 0;
    nVetoMuons5 = 0;
    nVetoMuons10 = 0;
    nVetoMuons25 = 0;
    filename = "";
    lep1_ptrel_v0 = -1;
    lep1_ptrel_v1 = -1;
    lep2_ptrel_v0 = -1;
    lep2_ptrel_v1 = -1;
    lep1_miniIso = -1;
    lep2_miniIso = -1;
    jet_close_lep1 = LorentzVector(0,0,0,0);
    jet_close_lep2 = LorentzVector(0,0,0,0);
    lep1_p4 = LorentzVector(0,0,0,0);
    lep2_p4 = LorentzVector(0,0,0,0);
    lep3_p4 = LorentzVector(0,0,0,0);
    lep1_p4_gen = LorentzVector(0,0,0,0);
    lep2_p4_gen = LorentzVector(0,0,0,0);
    dilep_p4 = LorentzVector(0,0,0,0);
} 

//Main function
int babyMaker::ProcessBaby(IsolationMethods isoCase){

  //Initialize variables
  InitBabyNtuple();
  
  //Local variables
  bool isData = tas::evt_isRealData();
  
  //Debug mode
  if (verbose && tas::evt_event() != evt_cut) return -1;

  //Preliminary stuff
  if (tas::hyp_type().size() < 1) return -1;
  if (tas::mus_dxyPV().size() != tas::mus_dzPV().size()) return -1;

  //Fill Easy Variables
  //filename = currentFile->GetTitle();//fixme
  met = evt_pfmet();
  metPhi = evt_pfmetPhi();
  event = tas::evt_event();
  lumi = tas::evt_lumiBlock();
  run = tas::evt_run();
  is_real_data = tas::evt_isRealData();
  xsec = tas::evt_xsec_incl();
  kfactor = tas::evt_kfactor();
  gen_met = tas::gen_met();
  gen_met_phi = tas::gen_metPhi();

  //Fill data vs. mc variables
  filt_csc = is_real_data ? tas::evt_cscTightHaloId() : 1;
  filt_hbhe = is_real_data ? (tas::evt_hbheFilter() && tas::hcalnoise_isolatedNoiseSumE() < 50.0 && tas::hcalnoise_isolatedNoiseSumEt() < 25.0 && tas::hcalnoise_numIsolatedNoiseChannels() < 10) : 1;
  filt_hcallaser = is_real_data ? tas::filt_hcalLaser() : 1;
  filt_ecaltp = is_real_data ? tas::filt_ecalTP() : 1;
  filt_trkfail = is_real_data ? tas::filt_trackingFailure() : 1;
  filt_eebadsc = is_real_data ? tas::filt_eeBadSc() : 1;
  scale1fb = is_real_data ? 1 : tas::evt_scale1fb();
  
  //Fill lepton variables
  hyp_result_t best_hyp_info = chooseBestHyp(isoCase);
  hyp_class = best_hyp_info.hyp_class;
  int best_hyp = best_hyp_info.best_hyp;
  if (verbose) cout << "chose hyp: " << best_hyp << " of class" << hyp_class << endl;
  if (hyp_class == 0 || hyp_class == -1) return -1;
  lep1_p4 = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_p4().at(best_hyp) : tas::hyp_lt_p4().at(best_hyp);
  lep2_p4 = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_p4().at(best_hyp) : tas::hyp_lt_p4().at(best_hyp);
  lep1_id = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
  lep2_id = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
  lep1_idx = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
  lep2_idx = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
  Lep lep1 = Lep(lep1_id, lep1_idx);
  Lep lep2 = Lep(lep2_id, lep2_idx);
  lep1_dxyPV = lep1.dxyPV();
  lep2_dxyPV = lep2.dxyPV();
  lep1_dZ = lep1.dzPV();
  lep2_dZ = lep2.dzPV();
  lep1_d0_err = lep1.d0Err();
  lep2_d0_err = lep2.d0Err();
  lep1_ip3d = lep1.ip3d();
  lep2_ip3d = lep2.ip3d();
  lep1_ip3d_err = lep1.ip3dErr();
  lep2_ip3d_err = lep2.ip3dErr();
  lep1_motherID = lepMotherID(lep1);
  lep2_motherID = lepMotherID(lep2);
  lep1_mc_id = lep1.mc_id();
  lep2_mc_id = lep2.mc_id();
  hyp_type = tas::hyp_type().at(best_hyp);
  pair <particle_t, int> thirdLepton = getThirdLepton(best_hyp);
  lep3_id = thirdLepton.first.id;
  lep3_idx = thirdLepton.first.idx;
  lep3_p4 = thirdLepton.first.p4;
  lep3_quality = thirdLepton.second;
  lep1_iso = abs(lep1_id) == 11 ? eleRelIso03(lep1_idx, SS) :  muRelIso03(lep1_idx, SS);
  lep2_iso = abs(lep2_id) == 11 ? eleRelIso03(lep2_idx, SS) :  muRelIso03(lep2_idx, SS);
  dilep_p4 = lep1_p4 + lep2_p4; 
  lep1_passes_id = isGoodLepton(lep1_id, lep1_idx, isoCase);
  lep2_passes_id = isGoodLepton(lep2_id, lep2_idx, isoCase);
  
  //Fill generated lepton variables, ignoring reco (matching to reco done above)
  vector <particle_t> genPair = getGenPair(verbose);
  if (genPair.size() == 2){
    lep1_id_gen = genPair.at(0).id;
    lep2_id_gen = genPair.at(1).id;
    lep1_p4_gen = genPair.at(0).p4;
    lep2_p4_gen = genPair.at(1).p4;
  }
  
  //Fill all generated particles
  if (!isData){
    genps_p4 = tas::genps_p4();
    genps_id = tas::genps_id();
    genps_id_mother = tas::genps_id_mother();
    genps_status = tas::genps_status(); 
    genps_id_grandma = tas::genps_id_simplegrandma(); 
  }
  
  //Determine and save jet and b-tag variables
  std::pair <vector <Jet>, vector <Jet> > jet_results = SSJetsCalculator();
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets.push_back(jet_results.first.at(i).p4());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags.push_back(jet_results.second.at(i).p4());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_disc.push_back(jet_results.first.at(i).csv());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_disc.push_back(jet_results.second.at(i).csv());
  njets = jets.size();
  nbtags = btags.size();
  ht = 0;
  for (unsigned int i = 0; i < jets.size(); i++) ht += jets.at(i).pt(); 
  
  //Verbose for jets
  if (verbose){
    cout << "njets: " << njets << endl;
    cout << "nbtags: " <<  nbtags << endl;
    for (unsigned int i = 0; i < jets.size(); i++) cout << i << " " << jets[i].pt() << " " << jets[i].eta() << endl;
  } 

  //PtRel for both leptons
  lep1_ptrel_v0 = getPtRel(lep1_id, lep1_idx, false);
  lep2_ptrel_v0 = getPtRel(lep2_id, lep2_idx, false);
  lep1_ptrel_v1 = getPtRel(lep1_id, lep1_idx, true);
  lep2_ptrel_v1 = getPtRel(lep2_id, lep2_idx, true);

  //MiniIso
  if (abs(lep1_id)==11) lep1_miniIso = elMiniRelIso(lep1_idx, 0.1, true);
  else                  lep1_miniIso = muMiniRelIso(lep1_idx, 0.1, true);
  if (abs(lep2_id)==11) lep2_miniIso = elMiniRelIso(lep2_idx, 0.1, true);
  else                  lep2_miniIso = muMiniRelIso(lep2_idx, 0.1, true);

  //Closest Jet
  jet_close_lep1 = closestJet(lep1_p4);
  jet_close_lep2 = closestJet(lep2_p4);

  //nVeto Leptons
  for (unsigned int eidx = 0; eidx < tas::els_p4().size(); eidx++){
    if (!isGoodVetoElectron(eidx)) continue;
    if (tas::els_p4().at(eidx).pt() < 7) continue;
    nVetoElectrons7++;
    if (verbose) cout << "good elec: " << tas::els_p4().at(eidx).pt() << endl;
    if (tas::els_p4().at(eidx).pt() < 10) continue;
    nVetoElectrons10++;
    if (tas::els_p4().at(eidx).pt() < 25) continue;
    nVetoElectrons25++;
  }
  for (unsigned int muidx = 0; muidx < tas::mus_p4().size(); muidx++){
    if (!isGoodVetoMuon(muidx)) continue;
    if (tas::mus_p4().at(muidx).pt() < 5) continue;
    nVetoMuons5++;
    if (verbose) cout << "good muon: " << tas::mus_p4().at(muidx).pt() << endl;
    if (tas::mus_p4().at(muidx).pt() < 10) continue;
    nVetoMuons10++;
    if (tas::mus_p4().at(muidx).pt() < 25) continue;
    nVetoMuons25++;
  }
  
  //MT variables
  mt = MT(lep1_p4.pt(),met,lep1_p4.phi(), metPhi);
  mt2 = MT2(met, metPhi, lep1_p4, lep2_p4);
  mt_l2 = MT(lep2_p4.pt(), met, lep2_p4.phi(), metPhi);
  
  //Ht and MET
  if (verbose) cout << "ht: " << ht << endl;
  if (verbose) cout << "met: " << met << endl;
  
  //Fill Baby
  BabyTree->Fill();

  return 0;  

}
