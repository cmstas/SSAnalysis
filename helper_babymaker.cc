#include "helper_babymaker.h"
#include "CORE/Tools/btagsf/BTagCalibrationStandalone.h"
#include "CORE/Tools/jetcorr/Utilities.icc"
#include "CORE/Tools/jetcorr/SimpleJetCorrectionUncertainty.icc"
#include "CORE/Tools/jetcorr/JetCorrectionUncertainty.icc"

using namespace tas;

const bool applyBtagSFs = true;

//Main functions
void babyMaker::MakeBabyNtuple(const char* output_name, bool expt){

  //Create Baby
  BabyFile = new TFile(Form("%s/%s_%i.root", path.Data(), output_name, expt ? 1 : 0), "RECREATE");
  BabyFile->cd();
  BabyTree = new TTree("t", "SS2015 Baby Ntuple");

  //Define Branches
  BabyTree->Branch("met"                     , &met                     );
  BabyTree->Branch("filenumber"              , &filenumber              );
  BabyTree->Branch("metPhi"                  , &metPhi                  );
  BabyTree->Branch("corrMET"                 , &corrMET                 );
  BabyTree->Branch("corrMETphi"              , &corrMETphi              );
  BabyTree->Branch("event"                   , &event                   );
  BabyTree->Branch("lumi"                    , &lumi                    );
  BabyTree->Branch("run"                     , &run                     );
  BabyTree->Branch("filt_csc"                , &filt_csc                );
  BabyTree->Branch("filt_hbhe"               , &filt_hbhe               );
  BabyTree->Branch("filt_hcallaser"          , &filt_hcallaser          );
  BabyTree->Branch("filt_ecaltp"             , &filt_ecaltp             );
  BabyTree->Branch("filt_trkfail"            , &filt_trkfail            );
  BabyTree->Branch("filt_eebadsc"            , &filt_eebadsc            );
  BabyTree->Branch("is_real_data"            , &is_real_data            );
  BabyTree->Branch("scale1fb"                , &scale1fb                );
  BabyTree->Branch("xsec"                    , &xsec                    );
  BabyTree->Branch("kfactor"                 , &kfactor                 );
  BabyTree->Branch("gen_met"                 , &gen_met                 );
  BabyTree->Branch("genweights"              , &genweights              );
  BabyTree->Branch("genweightsID"            , &genweightsID            );
  BabyTree->Branch("gen_met_phi"             , &gen_met_phi             );
  BabyTree->Branch("njets"                   , &njets                   );
  BabyTree->Branch("njets_corr"              , &njets_corr              );
  BabyTree->Branch("hyp_class"               , &hyp_class               );
  BabyTree->Branch("lep1_p4"                 , &lep1_p4                 );
  BabyTree->Branch("lep2_p4"                 , &lep2_p4                 );
  BabyTree->Branch("ht_corr"                 , &ht_corr                 );
  BabyTree->Branch("ht"                      , &ht                      );
  BabyTree->Branch("lep1_motherID"           , &lep1_motherID           );
  BabyTree->Branch("lep2_motherID"           , &lep2_motherID           );
  BabyTree->Branch("lep1_mc_id"              , &lep1_mc_id              );
  BabyTree->Branch("lep2_mc_id"              , &lep2_mc_id              );
  BabyTree->Branch("lep1_id"                 , &lep1_id                 );
  BabyTree->Branch("lep2_id"                 , &lep2_id                 );
  BabyTree->Branch("lep1_coneCorrPt"         , &lep1_coneCorrPt         );
  BabyTree->Branch("lep2_coneCorrPt"         , &lep2_coneCorrPt         );
  BabyTree->Branch("lep1_idx"                , &lep1_idx                );
  BabyTree->Branch("lep2_idx"                , &lep2_idx                );
  BabyTree->Branch("jets"                    , &jets                    );
  BabyTree->Branch("jets_corr"               , &jets_corr               );
  BabyTree->Branch("btags_disc"              , &btags_disc              );
  BabyTree->Branch("jets_disc"               , &jets_disc               );
  BabyTree->Branch("jets_JEC"                , &jets_JEC                );
  BabyTree->Branch("btags_JEC"               , &btags_JEC               );
  BabyTree->Branch("jets_undoJEC"            , &jets_undoJEC            );
  BabyTree->Branch("btags_undoJEC"           , &btags_undoJEC           );
  BabyTree->Branch("btags_corr_disc"         , &btags_corr_disc         );
  BabyTree->Branch("jets_corr_disc"          , &jets_corr_disc          );
  BabyTree->Branch("jets_corr_JEC"           , &jets_corr_JEC           );
  BabyTree->Branch("btags_corr_JEC"          , &btags_corr_JEC          );
  BabyTree->Branch("jets_corr_undoJEC"       , &jets_corr_undoJEC       );
  BabyTree->Branch("btags_corr_undoJEC"      , &btags_corr_undoJEC      );
  BabyTree->Branch("btags_corr_unc"          , &btags_corr_unc          );
  BabyTree->Branch("jets_corr_unc"           , &jets_corr_unc           );
  BabyTree->Branch("btags_unc"               , &btags_unc               );
  BabyTree->Branch("jets_unc"                , &jets_unc                );
  BabyTree->Branch("btags"                   , &btags                   );
  BabyTree->Branch("nbtags"                  , &nbtags                  );
  BabyTree->Branch("btags_corr"              , &btags_corr              );
  BabyTree->Branch("nbtags_corr"             , &nbtags_corr             );
  BabyTree->Branch("sf_dilepTrig_hpt"        , &sf_dilepTrig_hpt        );
  BabyTree->Branch("sf_dilepTrig_lpt"        , &sf_dilepTrig_lpt        );
  BabyTree->Branch("sf_dilepTrig_vlpt"       , &sf_dilepTrig_vlpt       );
  BabyTree->Branch("hyp_type"                , &hyp_type                );
  BabyTree->Branch("sf_dilep_eff"            , &sf_dilep_eff            );
  BabyTree->Branch("mt"                      , &mt                      );
  BabyTree->Branch("mt_l2"                   , &mt_l2                   );
  BabyTree->Branch("mt2"                     , &mt2                     );
  BabyTree->Branch("mtmin"                   , &mtmin                   );
  BabyTree->Branch("mGluino"                 , &mGluino                 );
  BabyTree->Branch("mLSP"                    , &mLSP                    );
  BabyTree->Branch("mSbottom"                , &mSbottom                );
  BabyTree->Branch("mChargino"               , &mChargino               );
  BabyTree->Branch("lep1_id_gen"             , &lep1_id_gen             );
  BabyTree->Branch("lep2_id_gen"             , &lep2_id_gen             );
  BabyTree->Branch("lep1_p4_gen"             , &lep1_p4_gen             );
  BabyTree->Branch("lep2_p4_gen"             , &lep2_p4_gen             );
  BabyTree->Branch("lep3_id"                 , &lep3_id                 );
  BabyTree->Branch("lep3_idx"                , &lep3_idx                );
  BabyTree->Branch("lep3_p4"                 , &lep3_p4                 );
  BabyTree->Branch("lep3_quality"            , &lep3_quality            );
  BabyTree->Branch("lep4_id"                 , &lep4_id                 );
  BabyTree->Branch("lep4_idx"                , &lep4_idx                );
  BabyTree->Branch("lep4_p4"                 , &lep4_p4                 );
  BabyTree->Branch("lep1_iso"                , &lep1_iso                );
  BabyTree->Branch("lep2_iso"                , &lep2_iso                );
  BabyTree->Branch("lep1_tkIso"              , &lep1_tkIso              );
  BabyTree->Branch("lep2_tkIso"              , &lep2_tkIso              );
  BabyTree->Branch("dilep_p4"                , &dilep_p4                );
  BabyTree->Branch("genps_p4"                , &genps_p4                );
  BabyTree->Branch("genps_id"                , &genps_id                );
  BabyTree->Branch("genps_id_mother"         , &genps_id_mother         );
  BabyTree->Branch("genps_idx_mother"        , &genps_idx_mother        );
  BabyTree->Branch("genps_status"            , &genps_status            );
  BabyTree->Branch("genps_id_grandma"        , &genps_id_grandma        );
  BabyTree->Branch("lep1_passes_id"          , &lep1_passes_id          );
  BabyTree->Branch("lep2_passes_id"          , &lep2_passes_id          );
  BabyTree->Branch("lep3_passes_id"          , &lep3_passes_id          );
  BabyTree->Branch("lep4_passes_id"          , &lep4_passes_id          );
  BabyTree->Branch("lep3_tight"              , &lep3_tight              );
  BabyTree->Branch("lep3_veto"               , &lep3_veto               );
  BabyTree->Branch("lep3_fo"                 , &lep3_fo                 );
  BabyTree->Branch("lep4_tight"              , &lep4_tight              );
  BabyTree->Branch("lep4_veto"               , &lep4_veto               );
  BabyTree->Branch("lep4_fo"                 , &lep4_fo                 );
  BabyTree->Branch("lep1_dxyPV"              , &lep1_dxyPV              );
  BabyTree->Branch("lep2_dxyPV"              , &lep2_dxyPV              );
  BabyTree->Branch("lep1_dZ"                 , &lep1_dZ                 );
  BabyTree->Branch("lep2_dZ"                 , &lep2_dZ                 );
  BabyTree->Branch("lep1_d0_err"             , &lep1_d0_err             );
  BabyTree->Branch("lep2_d0_err"             , &lep2_d0_err             );
  BabyTree->Branch("lep1_ip3d"               , &lep1_ip3d               );
  BabyTree->Branch("lep2_ip3d"               , &lep2_ip3d               );
  BabyTree->Branch("lep1_MVA"                , &lep1_MVA                );
  BabyTree->Branch("lep2_MVA"                , &lep2_MVA                );
  BabyTree->Branch("lep1_ip3d_err"           , &lep1_ip3d_err           );
  BabyTree->Branch("lep2_ip3d_err"           , &lep2_ip3d_err           );
  BabyTree->Branch("nVetoElectrons7"         , &nVetoElectrons7         );
  BabyTree->Branch("nVetoElectrons10"        , &nVetoElectrons10        );
  BabyTree->Branch("nVetoElectrons25"        , &nVetoElectrons25        );
  BabyTree->Branch("nVetoMuons5"             , &nVetoMuons5             );
  BabyTree->Branch("nVetoMuons10"            , &nVetoMuons10            );
  BabyTree->Branch("nVetoMuons25"            , &nVetoMuons25            );
  BabyTree->Branch("filename"                , &filename                );
  BabyTree->Branch("lep1_ptrel_v0"           , &lep1_ptrel_v0           );
  BabyTree->Branch("lep1_ptrel_v1"           , &lep1_ptrel_v1           );
  BabyTree->Branch("lep2_ptrel_v0"           , &lep2_ptrel_v0           );
  BabyTree->Branch("lep2_ptrel_v1"           , &lep2_ptrel_v1           );
  BabyTree->Branch("lep1_miniIso"            , &lep1_miniIso            );
  BabyTree->Branch("lep2_miniIso"            , &lep2_miniIso            );
  BabyTree->Branch("jet_close_lep1"          , &jet_close_lep1          );
  BabyTree->Branch("jet_close_lep2"          , &jet_close_lep2          );
  BabyTree->Branch("eleID_kfhits"            , &eleID_kfhits            );
  BabyTree->Branch("eleID_oldsigmaietaieta"  , &eleID_oldsigmaietaieta  );
  BabyTree->Branch("eleID_oldsigmaiphiiphi"  , &eleID_oldsigmaiphiiphi  );
  BabyTree->Branch("eleID_oldcircularity"    , &eleID_oldcircularity    );
  BabyTree->Branch("eleID_oldr9"             , &eleID_oldr9             );
  BabyTree->Branch("eleID_scletawidth"       , &eleID_scletawidth       );
  BabyTree->Branch("eleID_sclphiwidth"       , &eleID_sclphiwidth       );
  BabyTree->Branch("eleID_he"                , &eleID_he                );
  BabyTree->Branch("eleID_psEoverEraw"       , &eleID_psEoverEraw       );
  BabyTree->Branch("eleID_kfchi2"            , &eleID_kfchi2            );
  BabyTree->Branch("eleID_chi2_hits"         , &eleID_chi2_hits         );
  BabyTree->Branch("eleID_fbrem"             , &eleID_fbrem             );
  BabyTree->Branch("eleID_ep"                , &eleID_ep                );
  BabyTree->Branch("eleID_eelepout"          , &eleID_eelepout          );
  BabyTree->Branch("eleID_IoEmIop"           , &eleID_IoEmIop           );
  BabyTree->Branch("eleID_deltaetain"        , &eleID_deltaetain        );
  BabyTree->Branch("eleID_deltaphiin"        , &eleID_deltaphiin        );
  BabyTree->Branch("eleID_deltaetaseed"      , &eleID_deltaetaseed      );
  BabyTree->Branch("eleID_pT"                , &eleID_pT                );
  BabyTree->Branch("eleID_isbarrel"          , &eleID_isbarrel          );
  BabyTree->Branch("eleID_isendcap"          , &eleID_isendcap          );
  BabyTree->Branch("eleID_scl_eta"           , &eleID_scl_eta           );
  BabyTree->Branch("muID_dzPV"               , &muID_dzPV               );
  BabyTree->Branch("muID_ptSig"              , &muID_ptSig              );
  BabyTree->Branch("muID_ip3dSig"            , &muID_ip3dSig            );
  BabyTree->Branch("muID_medMuonPOG"         , &muID_medMuonPOG         );
  BabyTree->Branch("muID_pt"                 , &muID_pt                 );
  BabyTree->Branch("muID_eta"                , &muID_eta                );
  BabyTree->Branch("trueNumInt"              , &trueNumInt              );
  BabyTree->Branch("nPUvertices"             , &nPUvertices             ); 
  BabyTree->Branch("nGoodVertices"           , &nGoodVertices           ); 
  BabyTree->Branch("lep1_trigMatch_noIsoReq" , &lep1_trigMatch_noIsoReq );
  BabyTree->Branch("lep1_trigMatch_isoReq"   , &lep1_trigMatch_isoReq   );
  BabyTree->Branch("lep2_trigMatch_noIsoReq" , &lep2_trigMatch_noIsoReq );
  BabyTree->Branch("lep2_trigMatch_isoReq"   , &lep2_trigMatch_isoReq   );
  BabyTree->Branch("met3p0"                  , &met3p0                  );
  BabyTree->Branch("jet_corr_pt"             , &jet_corr_pt             );
  BabyTree->Branch("metphi3p0"               , &metphi3p0               );
  BabyTree->Branch("passes_met_filters"      , &passes_met_filters      );
  BabyTree->Branch("mostJets"                , &mostJets                );
  BabyTree->Branch("madeExtraZ"              , &madeExtraZ               );
  BabyTree->Branch("madeExtraG"              , &madeExtraG               );
  BabyTree->Branch("lep3_mcid"               , &lep3_mcid                );
  BabyTree->Branch("lep3_mcidx"              , &lep3_mcidx               );
  BabyTree->Branch("lep4_mcid"               , &lep4_mcid                );
  BabyTree->Branch("lep4_mcidx"              , &lep4_mcidx               );
  BabyTree->Branch("mostJets_rawp4"          , &mostJets_rawp4          );
  BabyTree->Branch("mostJets_disc"           , &mostJets_disc           );
  BabyTree->Branch("mostJets_unc"            , &mostJets_unc            );
  BabyTree->Branch("mostJets_JEC"            , &mostJets_JEC            );
  BabyTree->Branch("mostJets_undoJEC"        , &mostJets_undoJEC        );
  BabyTree->Branch("mostJets_passID"         , &mostJets_passID         );
  BabyTree->Branch("mostJets_passCleaning"   , &mostJets_passCleaning   );
  
  //InSituFR
  BabyTree->Branch("lep1_isGoodLeg"         , &lep1_isGoodLeg         );
  BabyTree->Branch("lep2_isGoodLeg"         , &lep2_isGoodLeg         );
  BabyTree->Branch("lep1_isFakeLeg"         , &lep1_isFakeLeg         );
  BabyTree->Branch("lep2_isFakeLeg"         , &lep2_isFakeLeg         );
  BabyTree->Branch("lep1_multiIso"          , &lep1_multiIso          );
  BabyTree->Branch("lep2_multiIso"          , &lep2_multiIso          );
  BabyTree->Branch("lep1_sip"               , &lep1_sip               );
  BabyTree->Branch("lep2_sip"               , &lep2_sip               );
  BabyTree->Branch("lep1_closeJet"          , &lep1_closeJet          );
  BabyTree->Branch("lep2_closeJet"          , &lep2_closeJet          );
  BabyTree->Branch("passed_id_inSituFR_lep1", &passed_id_inSituFR_lep1); 
  BabyTree->Branch("passed_id_inSituFR_lep2", &passed_id_inSituFR_lep2); 
  
  //Triggers
  BabyTree->Branch("fired_trigger"          , &fired_trigger          ); 
  BabyTree->Branch("triggers"               , &triggers               ); 

  BabyTree->Branch("weight_btagsf"                  , &weight_btagsf                  ); 
  BabyTree->Branch("weight_btagsf_UP"               , &weight_btagsf_UP               ); 
  BabyTree->Branch("weight_btagsf_DN"               , &weight_btagsf_DN               ); 

  if (applyBtagSFs) {
    // setup btag calibration readers
    calib = new BTagCalibration("csvv2", "btagsf/CSVv2.csv"); // 50ns version of SFs
    reader_heavy    = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", "central"); // central
    reader_heavy_UP = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", "up");      // sys up
    reader_heavy_DN = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", "down");    // sys down
    reader_light    = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "central");   // central
    reader_light_UP = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "up");        // sys up
    reader_light_DN = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "down");      // sys down

    // get btag efficiencies
    TFile* f_btag_eff          = new TFile("btagsf/btageff__ttbar_powheg_pythia8_25ns.root");
    TH2D* h_btag_eff_b_temp    = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_b");
    TH2D* h_btag_eff_c_temp    = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_c");
    TH2D* h_btag_eff_udsg_temp = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_udsg");
    BabyFile->cd();
    h_btag_eff_b               = (TH2D*) h_btag_eff_b_temp->Clone("h_btag_eff_b");
    h_btag_eff_c               = (TH2D*) h_btag_eff_c_temp->Clone("h_btag_eff_c");
    h_btag_eff_udsg            = (TH2D*) h_btag_eff_udsg_temp->Clone("h_btag_eff_udsg");
    f_btag_eff->Close();
  }

  //Print warning!
  cout << "Careful!! Path is " << path << endl;

}

void babyMaker::InitBabyNtuple(){

    met = -1;
    metPhi = -1;
    corrMET = -1;
    corrMETphi = -1;
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
    genweights.clear();
    genweightsID.clear();
    gen_met_phi = -1;
    njets = -1;
    njets_corr = -1;
    hyp_class = -1;
    ht = -1;
    ht_corr = -1;
    lep1_motherID = 0;
    lep2_motherID = 0;
    lep1_mc_id = -1;
    lep2_mc_id = -1; 
    lep1_id = -1;
    lep2_id = -1;
    lep1_coneCorrPt = -1;
    lep2_coneCorrPt = -1;
    lep1_idx = -1;
    lep2_idx = -1;
    jets.clear();
    btags_disc.clear();
    jet_corr_pt.clear(); 
    ht_corr = -1;
    jets_disc.clear();
    jets_JEC.clear();
    btags_JEC.clear();
    jets_undoJEC.clear();
    btags_undoJEC.clear();
    btags.clear();
    jets_corr.clear();
    btags_corr_disc.clear();
    jets_corr_disc.clear();
    jets_corr_JEC.clear();
    btags_corr_JEC.clear();
    jets_corr_undoJEC.clear();
    btags_corr_undoJEC.clear();
    jets_corr_unc.clear(); 
    btags_corr_unc.clear(); 
    jets_unc.clear(); 
    btags_unc.clear(); 
    btags.clear(); 
    btags_corr.clear();
    nbtags = -1;
    nbtags_corr = -1;
    sf_dilepTrig_hpt = -1;
    sf_dilepTrig_lpt = -1;
    sf_dilepTrig_vlpt = -1;
    hyp_type = -1;
    sf_dilep_eff = -1;
    mt = -1;
    mt_l2 = -1;
    mt2 = -1;
    mtmin = -1;
    mGluino = -1;
    mLSP = -1;
    mSbottom = -1;
    mChargino = -1;
    lep1_id_gen = -1;
    lep2_id_gen = -1;
    lep3_id = -1;
    lep3_idx = -1;
    lep4_id = -1;
    lep4_idx = -1;
    lep3_quality = -1;
    lep3_mcid = -1; 
    lep3_mcidx = -1;
    lep4_mcid = -1; 
    lep4_mcidx = -1;
    lep1_iso = -1;
    lep2_iso = -1;
    lep1_tkIso = -1;
    lep2_tkIso = -1;
    genps_p4.clear();
    genps_id.clear();
    genps_id_mother.clear();
    genps_idx_mother.clear();
    genps_status.clear();
    genps_id_grandma.clear();
    lep1_passes_id = false;
    lep2_passes_id = false;
    lep3_passes_id = false;
    lep4_passes_id = false;
    lep3_tight = false;
    lep3_veto = false;
    lep3_fo = false;
    lep4_tight = false;
    lep4_veto = false;
    lep4_fo = false;
    lep1_dxyPV = -999998;
    lep2_dxyPV = -999998;
    lep1_dZ = -999998;
    lep2_dZ = -999998;
    lep1_d0_err = -999998;
    lep2_d0_err = -999998;
    lep1_ip3d = -999998;
    lep2_ip3d = -999998;
    lep1_MVA = -999998;
    lep2_MVA = -999998;
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
    //lep4_p4 = LorentzVector(0,0,0,0);
    lep1_p4_gen = LorentzVector(0,0,0,0);
    lep2_p4_gen = LorentzVector(0,0,0,0);
    lep1_closeJet = LorentzVector(0,0,0,0);
    lep2_closeJet = LorentzVector(0,0,0,0);
    dilep_p4 = LorentzVector(0,0,0,0);
    eleID_kfhits.clear();
    eleID_oldsigmaietaieta.clear();
    eleID_oldsigmaiphiiphi.clear();
    eleID_oldcircularity.clear();
    eleID_oldr9.clear();
    eleID_scletawidth.clear();
    eleID_sclphiwidth.clear();
    eleID_he.clear();
    eleID_psEoverEraw.clear();
    eleID_kfchi2.clear();
    eleID_chi2_hits.clear();
    eleID_fbrem.clear();
    eleID_ep.clear();
    eleID_eelepout.clear();
    eleID_IoEmIop.clear();
    eleID_deltaetain.clear();
    eleID_deltaphiin.clear();
    eleID_deltaetaseed.clear();
    eleID_pT.clear();
    eleID_isbarrel.clear();
    eleID_isendcap.clear();
    eleID_scl_eta.clear();
    muID_dzPV.clear();      
    muID_ptSig.clear();     
    muID_ip3dSig.clear();
    muID_medMuonPOG.clear();
    mostJets.clear(); 
    muID_pt.clear();        
    muID_eta.clear();
    lep1_isGoodLeg = 0; 
    lep2_isGoodLeg = 0; 
    lep1_isFakeLeg = 0; 
    lep2_isFakeLeg = 0; 
    lep1_multiIso          = 0;
    lep2_multiIso          = 0;
    lep1_sip = -1;
    lep2_sip = -1;
    passed_id_inSituFR_lep1 = 0;
    passed_id_inSituFR_lep2 = 0;
    trueNumInt.clear();
    nPUvertices.clear(); 
    nGoodVertices = 0; 
    lep1_trigMatch_noIsoReq = 0;
    lep1_trigMatch_isoReq = 0;
    lep2_trigMatch_noIsoReq = 0;
    lep2_trigMatch_isoReq = 0; 
    fired_trigger = 0;
    triggers = 0;
    met3p0 = 0;
    metphi3p0 = 0;
    passes_met_filters = 0;
    madeExtraZ = 0;  
    madeExtraG = 0;  
    weight_btagsf  = -9999.;
    weight_btagsf_UP = -9999.;
    weight_btagsf_DN = -9999.;
    filenumber = -1;
    mostJets_rawp4.clear(); 
    mostJets_disc.clear(); 
    mostJets_unc.clear(); 
    mostJets_JEC.clear(); 
    mostJets_undoJEC.clear(); 
    mostJets_passID.clear(); 
    mostJets_passCleaning.clear(); 
} 

//Main function
int babyMaker::ProcessBaby(string filename_in, FactorizedJetCorrector* jetCorr, JetCorrectionUncertainty* jecUnc, bool expt, int file){

  //Figure out if this is a 25ns or 50ns sample
  bool is25 = true;
  if (filename_in.find("50ns") < filename_in.size()) is25 = false;
  if (filename_in.find("Run2015B") < filename_in.size()) is25 = false;

  //Initialize variables
  InitBabyNtuple();
  
  //Debug mode
  if (verbose && evt_cut>0 && tas::evt_event() != evt_cut) return -1;

  //Preliminary stuff
  if (tas::hyp_type().size() < 1) return -1;
  if (tas::mus_dxyPV().size() != tas::mus_dzPV().size()) return -1;

  //if (tas::evt_event() != 381733) return -1;

  //Fill Easy Variables
  filename = filename_in;
  filenumber = file; 
  met = evt_pfmet();
  metPhi = evt_pfmetPhi();
  event = tas::evt_event();
  lumi = tas::evt_lumiBlock();
  run = tas::evt_run();
  is_real_data = tas::evt_isRealData();

  if (!is_real_data){
    trueNumInt = tas::puInfo_trueNumInteractions();
    nPUvertices = puInfo_nPUvertices();
    xsec = tas::evt_xsec_incl();
    kfactor = tas::evt_kfactor();
    gen_met = tas::gen_met();
    gen_met_phi = tas::gen_metPhi();
    genweights = tas::genweights(); 
    genweightsID = tas::genweightsID(); 
  }

  //Fill data vs. mc variables
  filt_csc = is_real_data ? tas::evt_cscTightHaloId() : 1;
  filt_hbhe = is_real_data ? (tas::evt_hbheFilter() && tas::hcalnoise_isolatedNoiseSumE() < 50.0 && tas::hcalnoise_isolatedNoiseSumEt() < 25.0 && tas::hcalnoise_numIsolatedNoiseChannels() < 10) : 1;
  filt_hcallaser = is_real_data ? tas::filt_hcalLaser() : 1;
  filt_ecaltp = is_real_data ? tas::filt_ecalTP() : 1;
  filt_trkfail = is_real_data ? tas::filt_trackingFailure() : 1;
  filt_eebadsc = is_real_data ? tas::filt_eeBadSc() : 1;

  //Scale1fb
  scale1fb = is_real_data ? 1 : tas::evt_scale1fb();

  //Fill lepton variables
  hyp_result_t best_hyp_info = chooseBestHyp(expt, verbose);
  hyp_class = best_hyp_info.hyp_class;
  int best_hyp = best_hyp_info.best_hyp;
  if (hyp_class == 6){
    madeExtraZ = makesExtraZ(best_hyp).result;
    madeExtraG = makesExtraGammaStar(best_hyp);
  }
  if (verbose) cout << "chose hyp: " << best_hyp << " of class" << hyp_class << endl;
  if (hyp_class == 0 || hyp_class == -1) return -1;
  lep1_p4 = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_p4().at(best_hyp) : tas::hyp_lt_p4().at(best_hyp);
  lep2_p4 = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_p4().at(best_hyp) : tas::hyp_lt_p4().at(best_hyp);
  lep1_id = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
  lep2_id = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
  lep1_idx = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
  lep2_idx = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
  lep1_coneCorrPt = coneCorrPt(lep1_id, lep1_idx);
  lep2_coneCorrPt = coneCorrPt(lep2_id, lep2_idx);
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
  hyp_type = tas::hyp_type().at(best_hyp);
  pair <Lep, int> thirdLepton = getThirdLepton(best_hyp);
  lep3_id = thirdLepton.first.pdgId();
  lep3_idx = thirdLepton.first.idx();
  if (lep3_idx >= 0 && (abs(lep3_id) == 11 || abs(lep3_id) == 13)){
    lep3_p4 = thirdLepton.first.p4();
    if (!is_real_data){
      lep3_mcid = thirdLepton.first.mc_id();
      lep3_mcidx = thirdLepton.first.mcidx();
    }
  }
  lep3_quality = thirdLepton.second;
  Lep fourthLepton = getFourthLepton(best_hyp);
  lep4_id = fourthLepton.pdgId();
  lep4_idx = fourthLepton.idx();
  if (lep4_idx >= 0 && (abs(lep4_id) == 11 || abs(lep4_id) == 13)){
    lep4_p4 = fourthLepton.p4();
    if (!is_real_data){
      lep4_mcid = fourthLepton.mc_id();
      lep4_mcidx = fourthLepton.mcidx();
    }
  }
  lep1_iso = abs(lep1_id) == 11 ? eleRelIso03(lep1_idx, SS) :  muRelIso03(lep1_idx, SS);
  lep2_iso = abs(lep2_id) == 11 ? eleRelIso03(lep2_idx, SS) :  muRelIso03(lep2_idx, SS);
  lep1_tkIso = abs(lep1_id) == 11 ? els_tkIso().at(lep1_idx)/lep1_p4.pt() : mus_iso03_sumPt().at(lep1_idx)/lep1_p4.pt();
  lep2_tkIso = abs(lep2_id) == 11 ? els_tkIso().at(lep2_idx)/lep2_p4.pt() : mus_iso03_sumPt().at(lep2_idx)/lep2_p4.pt();
  lep1_multiIso = abs(lep1_id) == 11 ? passMultiIso(11, lep1_idx, 0.12, 0.80, 7.2, 1, 2) : passMultiIso(13, lep1_idx, 0.16, 0.76, 7.2, 1, 2);
  lep2_multiIso = abs(lep2_id) == 11 ? passMultiIso(11, lep2_idx, 0.12, 0.80, 7.2, 1, 2) : passMultiIso(13, lep2_idx, 0.16, 0.76, 7.2, 1, 2);
  lep1_sip = abs(lep1_id) == 11 ? fabs(els_ip3d().at(lep1_idx))/els_ip3derr().at(lep1_idx) : fabs(mus_ip3d().at(lep1_idx))/mus_ip3derr().at(lep1_idx); 
  lep2_sip = abs(lep2_id) == 11 ? fabs(els_ip3d().at(lep2_idx))/els_ip3derr().at(lep2_idx) : fabs(mus_ip3d().at(lep2_idx))/mus_ip3derr().at(lep2_idx); 
  dilep_p4 = lep1_p4 + lep2_p4; 
  lep1_passes_id = isGoodLepton(lep1_id, lep1_idx);
  lep2_passes_id = isGoodLepton(lep2_id, lep2_idx);
  lep1_MVA = abs(lep1_id) == 11 ? getMVAoutput(lep1_idx) : -9999; 
  lep2_MVA = abs(lep2_id) == 11 ? getMVAoutput(lep2_idx) : -9999; 

  //More Third lepton stuff
  if (abs(lep3_id) == 11 || abs(lep3_id) == 13){
    lep3_passes_id = isGoodLepton(lep3_id, lep3_idx);
    lep3_tight = abs(lep3_id) == 11 ? isGoodElectron(lep3_idx) : isGoodMuon(lep3_idx);
    lep3_veto = abs(lep3_id) == 11 ? isGoodVetoElectron(lep3_idx) : isGoodVetoMuon(lep3_idx);
    lep3_fo = abs(lep3_id) == 11 ? isFakableElectron(lep3_idx) : isFakableMuon(lep3_idx);
  }

  //More Fourth lepton stuff
  if (abs(lep4_id) == 11 || abs(lep4_id) == 13){
    lep4_passes_id = isGoodLepton(lep4_id, lep4_idx);
    lep4_tight = abs(lep4_id) == 11 ? isGoodElectron(lep4_idx) : isGoodMuon(lep4_idx);
    lep4_veto = abs(lep4_id) == 11 ? isGoodVetoElectron(lep4_idx) : isGoodVetoMuon(lep4_idx);
    lep4_fo = abs(lep4_id) == 11 ? isFakableElectron(lep4_idx) : isFakableMuon(lep4_idx);
  }

  //Lepton MC variables
  if (!is_real_data){
    lep1_mc_id = lep1.mc_id();
    lep2_mc_id = lep2.mc_id();
    lep1_motherID = lepMotherID(lep1);
    lep2_motherID = lepMotherID(lep2);
  }

  //Electron trigger stuff
  if (abs(lep1_id) == 11 && lep1_idx >= 0){
    //Double electron triggers
    if (passHLTTrigger(triggerName("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v"))) {
      if (tas::els_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_ElectronLeg().at(lep1_idx) > 0) lep1_trigMatch_noIsoReq = true;
    }
    if (passHLTTrigger(triggerName("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v")) || passHLTTrigger(triggerName("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"))) { 
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "hltEle17Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg1Filter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;
      if (matchToHLTFilter("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "hltEle17Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg2Filter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;
    }
    //Mu-El triggers
    if (passHLTTrigger(triggerName("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v"))) {
      if (tas::els_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_ElectronLeg().at(lep1_idx) > 0) lep1_trigMatch_noIsoReq = true;
    }
    if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"))) {
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
    }
    if (passHLTTrigger(triggerName("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v"))) {
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
    }
  }
  if (abs(lep2_id) == 11 && lep2_idx >= 0){
    //Double electron triggers
    if (passHLTTrigger(triggerName("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v"))) {
      if (tas::els_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_ElectronLeg().at(lep2_idx) > 0) lep2_trigMatch_noIsoReq = true;
    }
    if (passHLTTrigger(triggerName("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v")) || passHLTTrigger(triggerName("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"))) { 
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "hltEle17Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg1Filter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;
      if (matchToHLTFilter("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "hltEle17Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg2Filter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;
    }
    //Mu-El triggers
    if (passHLTTrigger(triggerName("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v"))) {
      if (tas::els_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_ElectronLeg().at(lep2_idx) > 0) lep2_trigMatch_noIsoReq = true;
    }
    if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"))) {
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
    }
    if (passHLTTrigger(triggerName("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v"))) {
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
    }      
  }

  //Muon trigger stuff
  if (abs(lep1_id) == 13 && lep1_idx >= 0){
    //Double muon triggers
    if (passHLTTrigger(triggerName("HLT_DoubleMu8_Mass8_PFHT300_v"))) {
      if (tas::mus_HLT_DoubleMu8_Mass8_PFHT300_MuonLeg().at(lep1_idx) > 0) lep1_trigMatch_noIsoReq = true;
    }
    if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v"), lep1_p4) > 0)             lep1_trigMatch_isoReq = true;
    if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"), lep1_p4) > 0)           lep1_trigMatch_isoReq = true;
    //Mu-El triggers
    if (passHLTTrigger(triggerName("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v"))) {
      if (tas::mus_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_MuonLeg().at(lep1_idx) > 0) lep1_trigMatch_noIsoReq = true;
    }
    if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"))) {
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
    }
    if (passHLTTrigger(triggerName("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v"))) {
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8", lep1_p4, 0.1, &dummypt)) lep1_trigMatch_isoReq = true;		
    }
  }
  if (abs(lep2_id) == 13 && lep2_idx >= 0){
    //Double muon triggers
    if (passHLTTrigger(triggerName("HLT_DoubleMu8_Mass8_PFHT300_v"))) {
      if (tas::mus_HLT_DoubleMu8_Mass8_PFHT300_MuonLeg().at(lep2_idx) > 0) lep2_trigMatch_noIsoReq = true;
    }
    if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v"), lep2_p4) > 0)             lep2_trigMatch_isoReq = true;
    if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"), lep2_p4) > 0)           lep2_trigMatch_isoReq = true;
    //Mu-El triggers
    if (passHLTTrigger(triggerName("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v"))) {
      if (tas::mus_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_MuonLeg().at(lep2_idx) > 0) lep2_trigMatch_noIsoReq = true;
    }
    if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"))) {
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
    }
    if (passHLTTrigger(triggerName("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v"))) {
      float dummypt = 0.;
      if (matchToHLTFilter("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v", "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8", lep2_p4, 0.1, &dummypt)) lep2_trigMatch_isoReq = true;		
    }
  }

  //PtRel for both leptons
  lep1_ptrel_v0 = getPtRel(lep1_id, lep1_idx, false, ssWhichCorr);
  lep2_ptrel_v0 = getPtRel(lep2_id, lep2_idx, false, ssWhichCorr);
  lep1_ptrel_v1 = getPtRel(lep1_id, lep1_idx, true, ssWhichCorr);
  lep2_ptrel_v1 = getPtRel(lep2_id, lep2_idx, true, ssWhichCorr);

  //MiniIso
  lep1_miniIso = abs(lep1_id)==11 ? elMiniRelIsoCMS3_EA(lep1_idx, ssEAversion) : muMiniRelIsoCMS3_EA(lep1_idx, ssEAversion);
  lep2_miniIso = abs(lep2_id)==11 ? elMiniRelIsoCMS3_EA(lep2_idx, ssEAversion) : muMiniRelIsoCMS3_EA(lep2_idx, ssEAversion);

  //For inSituFR, both must pass looser ID (easier than selection ID)
  passed_id_inSituFR_lep1 = isInSituFRLepton(lep1_id, lep1_idx, expt); 
  passed_id_inSituFR_lep2 = isInSituFRLepton(lep2_id, lep2_idx, expt); 

  //Closest jet for both leptons
  lep1_closeJet = closestJet(lep1_p4, 0.4, 3.0, ssWhichCorr);
  lep2_closeJet = closestJet(lep2_p4, 0.4, 3.0, ssWhichCorr);

  if (verbose){
    cout << "lep1 ratio: " << lep1_p4.pt()/lep1_closeJet.pt() << endl;
    cout << "lep2 ratio: " << lep2_p4.pt()/lep2_closeJet.pt() << endl;
  }
  
  //Fill all generated particles
  if (!is_real_data){
    genps_p4 = tas::genps_p4();
    genps_id = tas::genps_id();
    genps_id_mother = tas::genps_id_mother();
    genps_idx_mother = tas::genps_idx_mother();
    genps_status = tas::genps_status(); 
    genps_id_grandma = tas::genps_id_simplegrandma(); 
  }
  
  //Determine and save jet and b-tag variables, uncorrected
  std::pair <vector <Jet>, vector <Jet> > jet_results = SSJetsCalculator(jetCorr, 0, 1);
  for (unsigned int i = 0; i < jet_results.first.size(); i++)  jets.push_back(jet_results.first.at(i).p4());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags.push_back(jet_results.second.at(i).p4());
  for (unsigned int i = 0; i < jet_results.first.size(); i++)  jets_disc.push_back(jet_results.first.at(i).csv());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_disc.push_back(jet_results.second.at(i).csv());
  for (unsigned int i = 0; i < jet_results.first.size(); i++)  jets_JEC.push_back(jet_results.first.at(i).jec());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_JEC.push_back(jet_results.second.at(i).jec());
  for (unsigned int i = 0; i < jet_results.first.size(); i++)  jets_undoJEC.push_back(jet_results.first.at(i).undo_jec());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_undoJEC.push_back(jet_results.second.at(i).undo_jec());
  for (unsigned int i = 0; i < jet_results.first.size(); i++){
    jecUnc->setJetEta(jets[i].eta()); 
    jecUnc->setJetPt(jets[i].pt()*jets_undoJEC[i]*jets_JEC[i]); 
    jets_unc.push_back(jecUnc->getUncertainty(true)); 
  }
  for (unsigned int i = 0; i < jet_results.second.size(); i++){
    jecUnc->setJetEta(btags[i].eta()); 
    jecUnc->setJetPt(btags[i].pt()*btags_undoJEC[i]*btags_JEC[i]); 
    btags_unc.push_back(jecUnc->getUncertainty(true)); 
  }
  njets = jets.size();
  nbtags = btags.size();
  ht = 0;
  for (unsigned int i = 0; i < jets.size(); i++) ht += jets.at(i).pt(); 
  if (verbose) for (unsigned int i = 0; i < btags.size(); i++) cout << "btag: " << btags.at(i).pt() << endl;

  //Reject events that fail trigger matching
  if (ht < 300 && hyp_type != 0){
    if (abs(lep1_id) == 11 && !isTriggerSafe_v1(lep1_idx)) return -1;
    if (abs(lep2_id) == 11 && !isTriggerSafe_v1(lep2_idx)) return -1;
  }

  // for applying btagging SFs, using Method 1a from the twiki below:
  //   https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
  //   https://twiki.cern.ch/twiki/pub/CMS/BTagSFMethods/Method1aExampleCode_CSVM.cc.txt
  float btagprob_data = 1.;
  float btagprob_err_heavy_UP = 0.;
  float btagprob_err_heavy_DN = 0.;
  float btagprob_err_light_UP = 0.;
  float btagprob_err_light_DN = 0.;
  float btagprob_mc = 1.;  

  //Determine and save jet and b-tag variables, corrected
  jet_results = SSJetsCalculator(jetCorr, 1);
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_corr.push_back(jet_results.first.at(i).p4());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_corr.push_back(jet_results.second.at(i).p4());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_corr_disc.push_back(jet_results.first.at(i).csv());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_corr_disc.push_back(jet_results.second.at(i).csv());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_corr_JEC.push_back(jet_results.first.at(i).jec());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_corr_JEC.push_back(jet_results.second.at(i).jec());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_corr_undoJEC.push_back(jet_results.first.at(i).undo_jec());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_corr_undoJEC.push_back(jet_results.second.at(i).undo_jec());
  for (unsigned int i = 0; i < jet_results.first.size(); i++){
    jecUnc->setJetEta(jets_corr[i].eta()); 
    jecUnc->setJetPt(jets_corr[i].pt()*jets_corr_undoJEC[i]*jets_corr_JEC[i]); 
    jets_corr_unc.push_back(jecUnc->getUncertainty(true)); 
  }
  for (unsigned int i = 0; i < jet_results.second.size(); i++){
    jecUnc->setJetEta(btags_corr[i].eta()); 
    jecUnc->setJetPt(btags_corr[i].pt()*btags_corr_undoJEC[i]*btags_corr_JEC[i]); 
    btags_corr_unc.push_back(jecUnc->getUncertainty(true)); 
  }
  njets_corr = jets_corr.size();
  nbtags_corr = btags_corr.size();
  ht_corr = 0;
  for (unsigned int i = 0; i < jets_corr.size(); i++){
     jet_corr_pt.push_back(jets_corr.at(i).pt()*jets_corr_undoJEC.at(i)*jets_corr_JEC.at(i)); 
     ht_corr += jets_corr.at(i).pt()*jets_corr_undoJEC.at(i)*jets_corr_JEC.at(i); 
  }

  //now look at jets for get the btag scale factor (need to save down to 25 GeV corrected)
  jet_results = SSJetsCalculator(jetCorr, 1, 1);
  for (unsigned int i = 0; i < jet_results.first.size(); i++) {
     if (is_real_data) continue;
     //get btag eff weights
     float jet_pt = jet_results.first.at(i).p4().pt()*jet_results.first.at(i).undo_jec()*jet_results.first.at(i).jec();
     if (jet_pt<25.) continue;
     float jet_eta = jet_results.first.at(i).p4().eta();
     int jet_mcFlavour = jet_results.first.at(i).mcFlavor();
     float eff = getBtagEffFromFile(jet_pt, jet_eta, jet_mcFlavour);
     BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;
     if (abs(jet_mcFlavour) == 5) flavor = BTagEntry::FLAV_B;
     else if (abs(jet_mcFlavour) == 4) flavor = BTagEntry::FLAV_C;
     float pt_cutoff = std::max(30.,std::min(669.,double(jet_pt)));
     float weight_cent(1.), weight_UP(1.), weight_DN(1.);
     if (flavor == BTagEntry::FLAV_UDSG) {
       weight_cent = reader_light->eval(flavor, jet_eta, pt_cutoff);
       weight_UP = reader_light_UP->eval(flavor, jet_eta, pt_cutoff);
       weight_DN = reader_light_DN->eval(flavor, jet_eta, pt_cutoff);
     } else {
       weight_cent = reader_heavy->eval(flavor, jet_eta, pt_cutoff);
       weight_UP = reader_heavy_UP->eval(flavor, jet_eta, pt_cutoff);
       weight_DN = reader_heavy_DN->eval(flavor, jet_eta, pt_cutoff);
     }
     if (jet_results.first.at(i).isBtag()) {
       btagprob_data *= weight_cent * eff;
       btagprob_mc *= eff;
       float abserr_UP = weight_UP - weight_cent;
       float abserr_DN = weight_cent - weight_DN;
       if (flavor == BTagEntry::FLAV_UDSG) {
        btagprob_err_light_UP += abserr_UP/weight_cent;
        btagprob_err_light_DN += abserr_DN/weight_cent;
       } else {
        btagprob_err_heavy_UP += abserr_UP/weight_cent;
        btagprob_err_heavy_DN += abserr_DN/weight_cent;
       }
     } else {
       btagprob_data *= (1. - weight_cent * eff);
       btagprob_mc *= (1. - eff);
       float abserr_UP = weight_UP - weight_cent;
       float abserr_DN = weight_cent - weight_DN;
       if (flavor == BTagEntry::FLAV_UDSG) {
        btagprob_err_light_UP += (-eff * abserr_UP)/(1 - eff * weight_cent);
        btagprob_err_light_DN += (-eff * abserr_DN)/(1 - eff * weight_cent);
       } else {
        btagprob_err_heavy_UP += (-eff * abserr_UP)/(1 - eff * weight_cent);
        btagprob_err_heavy_DN += (-eff * abserr_DN)/(1 - eff * weight_cent);
       }
     }
  }
  weight_btagsf = btagprob_data / btagprob_mc;
  weight_btagsf_UP = weight_btagsf + (sqrt(pow(btagprob_err_heavy_UP,2) + pow(btagprob_err_light_UP,2)) * weight_btagsf);
  weight_btagsf_DN = weight_btagsf - (sqrt(pow(btagprob_err_heavy_DN,2) + pow(btagprob_err_light_DN,2)) * weight_btagsf);

  //Save Most jets 
  vector <Jet> mostJets_jet;
  for (unsigned int i = 0; i < tas::pfjets_p4().size(); i++){
    //Alias
    LorentzVector jet = tas::pfjets_p4().at(i);
  
    //Cuts
    if (jet.pt() < 5.) continue;
    if (fabs(jet.eta()) > 2.4) continue;
  
    //Calculate raw pt
    float rawpt = jet.pt()*tas::pfjets_undoJEC().at(i);
    
    //Calculate jet corr
    jetCorr->setJetEta(jet.eta()); 
    jetCorr->setJetPt(rawpt); 
    jetCorr->setJetA(tas::pfjets_area().at(i)); 
    jetCorr->setRho(tas::evt_fixgridfastjet_all_rho()); 
    float JEC = jetCorr->getCorrection(); 
  
    //Calculate jet unc
    jecUnc->setJetEta(pfjets_p4().at(i).eta()); 
    jecUnc->setJetPt(rawpt*JEC); 
    float jetUnc = jecUnc->getUncertainty(true);
  
    //Save results
    mostJets.push_back(jet);
    mostJets_jet.push_back(Jet(i, JEC)); 
    mostJets_rawp4.push_back(jet*tas::pfjets_undoJEC().at(i)); 
    mostJets_disc.push_back(tas::getbtagvalue("pfCombinedSecondaryVertexV2BJetTags",i)); 
    mostJets_unc.push_back(jetUnc); 
    mostJets_JEC.push_back(JEC); 
    mostJets_undoJEC.push_back(tas::pfjets_undoJEC().at(i)); 
    mostJets_passID.push_back(isLoosePFJet_50nsV1(i));
  }
  mostJets_passCleaning = cleanJets(mostJets_jet);
  
  //Verbose for jets
  if (verbose){
    cout << "njets: " << njets << endl;
    cout << "nbtags: " <<  nbtags << endl;
    cout << "Printing Jets: " << endl;
    for (unsigned int i = 0; i < jets.size(); i++) cout << i << " " << jets[i].pt() << " " << jets[i].eta() << endl;
  } 

  //Closest Jet
  jet_close_lep1 = closestJet(lep1_p4, 0.4, 3.0, ssWhichCorr);
  jet_close_lep2 = closestJet(lep2_p4, 0.4, 3.0, ssWhichCorr);

  //nVeto Leptons
  if (verbose) cout << " PRINTING RECO ELECTRONS" << endl;
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
  if (verbose) cout << " PRINTING RECO MUONS" << endl;
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
  mt    = MT(lep1_p4.pt(), lep1_p4.phi(), met, metPhi);
  mt_l2 = MT(lep2_p4.pt(), lep2_p4.phi(), met, metPhi);
  mt2   = MT2(met, metPhi, lep1_p4, lep2_p4);
  mtmin = mt > mt_l2 ? mt_l2 : mt; 
  
  //Ht and MET
  if (verbose) cout << "ht: " << ht << endl;
  if (verbose) cout << "met: " << met << endl;

  //Make sure one of our triggers fired
  if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v")))  (triggers |= 1<<1); 
  if (passHLTTrigger(triggerName("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v")))   (triggers |= 1<<2); 
  if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v")))              (triggers |= 1<<3); 
  if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v")))            (triggers |= 1<<4); 
  if (passHLTTrigger(triggerName("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v")) || 
      passHLTTrigger(triggerName("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v")))        (triggers |= 1<<6); 
  if (passHLTTrigger(triggerName("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v")))      (triggers |= 1<<0);
  if (passHLTTrigger(triggerName("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v")))    (triggers |= 1<<5); 
  if (passHLTTrigger(triggerName("HLT_DoubleMu8_Mass8_PFHT300_v")))                      (triggers |= 1<<7); 
  fired_trigger = false;
  if (triggers != 0) {
    if (ht_corr<300) {
      if ( hyp_type==0 && ((triggers & 1<<3)==(1<<3) || (triggers & 1<<4)==(1<<4)) ) fired_trigger = true;
      if ( hyp_type==3 && (triggers & 1<<6)==(1<<6) ) fired_trigger = true;
      if ( (hyp_type==1 || hyp_type==2) && ((triggers & 1<<1)==(1<<1) || (triggers & 1<<2)==(1<<2)) ) fired_trigger = true;
    } else {
      if ( hyp_type==0 && (triggers & 1<<7)==(1<<7) ) fired_trigger = true;
      if ( hyp_type==3 && (triggers & 1<<5)==(1<<5) ) fired_trigger = true;
      if ( (hyp_type==1 || hyp_type==2) && (triggers & 1<<0)==(1<<0) ) fired_trigger = true;
    }
  }

  //Electron ID variables
  for (unsigned int index = 0; index < els_fbrem().size(); index++){
    if (!isGoodVetoElectron(index)) continue;
    if (tas::els_p4().at(index).pt() < 7) continue;
    if ((tas::els_p4().at(index).eta()) > 2.5) continue;
    eleID_kfhits          .push_back(tas::els_ckf_laywithmeas().at(index));
    eleID_oldsigmaietaieta.push_back(tas::els_sigmaIEtaIEta_full5x5().at(index)); 
    eleID_oldsigmaiphiiphi.push_back(tas::els_sigmaIPhiIPhi_full5x5().at(index));
    eleID_oldcircularity  .push_back(1.0 - tas::els_e1x5_full5x5().at(index)/tas::els_e5x5_full5x5().at(index)); 
    eleID_oldr9           .push_back(tas::els_r9_full5x5().at(index)); 
    eleID_scletawidth     .push_back(tas::els_etaSCwidth().at(index));
    eleID_sclphiwidth     .push_back(tas::els_phiSCwidth().at(index));
    eleID_he              .push_back(tas::els_hOverE().at(index));
    eleID_psEoverEraw     .push_back(tas::els_eSCPresh().at(index)/tas::els_eSCRaw().at(index));
    eleID_kfchi2          .push_back(tas::els_ckf_chi2().at(index)/tas::els_ckf_ndof().at(index));
    eleID_chi2_hits       .push_back(tas::els_chi2().at(index)/tas::els_ndof().at(index));
    eleID_fbrem           .push_back(tas::els_fbrem().at(index));
    eleID_ep              .push_back(tas::els_eOverPIn().at(index));
    eleID_eelepout        .push_back(tas::els_eOverPOut().at(index));
    eleID_IoEmIop         .push_back(tas::els_ecalEnergy().at(index) != 0 ? 1.0/tas::els_ecalEnergy().at(index) - tas::els_eOverPIn().at(index)/tas::els_ecalEnergy().at(index) : 999999);
    eleID_deltaetain      .push_back(tas::els_dEtaIn().at(index));
    eleID_deltaphiin      .push_back(tas::els_dPhiIn().at(index));
    eleID_deltaetaseed    .push_back(tas::els_dEtaOut().at(index));
    eleID_pT              .push_back(tas::els_p4().at(index).pt()); 
    eleID_isbarrel        .push_back(fabs(tas::els_etaSC().at(index)) < 1.479 ? 1 : 0); 
    eleID_isendcap        .push_back(fabs(tas::els_etaSC().at(index)) > 1.479 ? 1 : 0); 
    eleID_scl_eta         .push_back(tas::els_etaSC().at(index)); 
  }

  //Muon ID variables
  for (unsigned int index = 0; index < mus_ip3d().size(); index++){
    if (!isGoodVetoMuon(index)) continue;
    if (tas::mus_p4().at(index).pt() < 5) continue;
    muID_dzPV      .push_back(fabs(mus_dzPV().at(index)));
    muID_ptSig     .push_back(mus_ptErr().at(index)/mus_trk_p4().at(index).pt());
    muID_ip3dSig   .push_back(fabs(mus_ip3d().at(index))/mus_ip3derr().at(index));
    muID_medMuonPOG.push_back(isMediumMuonPOG(index));
    muID_pt        .push_back(mus_p4().at(index).pt());
    muID_eta       .push_back(fabs(mus_p4().at(index).eta()));
  }

  //Number of good vertices
  for (unsigned int i = 0; i < tas::vtxs_ndof().size(); i++){
    if (!isGoodVertex(i)) continue;
    nGoodVertices++;
  }

  //Correct the met
  pair <float, float> T1CHSMET = getT1CHSMET_fromMINIAOD(jetCorr);
  corrMET = T1CHSMET.first;
  corrMETphi = T1CHSMET.second;

  //MET3p0 (aka FKW MET)
  pair<float,float> MET3p0_ = MET3p0();
  met3p0 = MET3p0_.first;
  metphi3p0 = MET3p0_.second;

  //MET filters
  passes_met_filters = passesMETfilterv2();

  //Fill Baby
  BabyTree->Fill();

  return 0;  

}

float babyMaker::getBtagEffFromFile(float pt, float eta, int mcFlavour){
  if(!h_btag_eff_b || !h_btag_eff_c || !h_btag_eff_udsg) {
    std::cout << "babyMaker::getBtagEffFromFile: ERROR: missing input hists" << std::endl;
    return 1.;
  }

  // only use pt bins up to 400 GeV for charm and udsg
  float pt_cutoff = std::max(20.,std::min(399.,double(pt)));
  TH2D* h(0);
  if (abs(mcFlavour) == 5) {
    h = h_btag_eff_b;
    // use pt bins up to 600 GeV for b
    pt_cutoff = std::max(20.,std::min(599.,double(pt)));
  }
  else if (abs(mcFlavour) == 4) {
    h = h_btag_eff_c;
  }
  else {
    h = h_btag_eff_udsg;
  }
    
  int binx = h->GetXaxis()->FindBin(pt_cutoff);
  int biny = h->GetYaxis()->FindBin(fabs(eta));
  return h->GetBinContent(binx,biny);
}
