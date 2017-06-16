#include "ScanChain.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Few things to note
///  + Global variables
///  + Pass by reference
///
///
///
///
///
////////////////////////////////////////////////////////////////////////////////////////////////////

int ScanChain(
    TChain* chain_,
    TString outfile_,
    TString option_="",
    bool fast_=true,
    int nEvents_=-1)
{

  // Save the input arguments into a global variable
  setInputArgumentVariables(chain_, outfile_, option_, fast_, nEvents_);

  // Actions to take before the loop starts (setting up global tools, booking output histograms etc.)
  beforeLoop();

  // File Loop
  TIter fileIter(listOfFiles);
  while ( (currentFile = (TFile*)fileIter.Next()) )
  {

    // Things related to opening TTree, hooking up with LeptonBaby class.
    // Now that we have a file open, we can setup some preliminary tool
    // settings that are different depending on which sample it is running on.
    // e.g. set up JetCorrector (either for MC or DATA)
    // I call this "prep" stage
    prepData();

    // Loop over Events in current file   //ACTUALLY A LEPTON "EVENT" LOOP
    unsigned int nEventsTree = tree->GetEntriesFast();
//	    for( unsigned int event = 0; event < nEventsTree; ++event)
    for (unsigned int event = 0; event < 1000; ++event)
    {

      // If all events processed just exit
      if (isAllEventsProcessed()) continue; //------------------------------------------------------> possible exit of the loop

      // Load the event into lepton tree
      loadNextEvent(event);

      // There are some criteria that makes an MC event bad
      if (isBadEvents()) continue; //---------------------------------------------------------------> possible exit of the loop

      // ==========================================================================================
      //
      //
      //
      // <3 of the code related to actual analysis.
      //
      //
      //
      // ==========================================================================================


      /// ~-~-~-~-~-~-~-~
      ///  Jet variables
      /// ~-~-~-~-~-~-~-~

      /// The following step computes the four variables:
      /// 1. HT of the event.
      /// 2. Njets with pT > 40.
      /// 3. N btags with pT > 40.
      /// 4. boolean flag for passing a jet pt cut or not
      HT            = 0.;
      njets40       = 0;
      nbtags        = 0;
      pass_jetptcut = false;
      computeJetRelatedVariables(/*outputs*/HT, njets40, nbtags, pass_jetptcut);

      /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~
      ///  Lepton isolation variable
      /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~

      /// The following step computes the lepton isolation variables
      ptrel      = 0;
      closejetpt = 0;
      relIso     = 0;
      computeLeptonIsolationVariables(/*outputs*/ptrel, closejetpt, relIso);

      // need to pass either jet pt cut > 40 or HT > 40.
      if (!passJetPtCutOrHTCut()) continue; //------------------------------------------------------> possible exit of the loop

      /// ~-~-~-~-~-~-~-~-~-~
      ///  Trigger selection
      /// ~-~-~-~-~-~-~-~-~-~

      // Trigger selection
      if (!passTriggerSelection()) continue; //-----------------------------------------------------> possible exit of the loop

      /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-
      ///  Lepton kinematic selection
      /// ~-~-~-~-~-~-~-~-~-~-~-~-~-~-

      // Apply some selections
      // Pt, Eta, sIP3D, and number of fake-able object.
      if (!passPtEtaSIPNFOLep()) continue; //-------------------------------------------------------> possible exit of the loop

      /// ~-~-~-~-~-~-~-~-~-~-~
      ///  Lepton ID selection
      /// ~-~-~-~-~-~-~-~-~-~-~

      /// Compute whether the lepton passes:
      /// Tight ID = ID,
      /// Fake-able object ID = FO
      /// non-isolated Tight ID = ID,
      /// non-isolated Fake-able object ID = FO
      passId = false;
      passFO =  false;
      passId_noiso = false;
      passFO_noiso = false;
      computeLeptonIDFlags(/*outputs*/passId, passFO, passId_noiso, passFO_noiso);

      if (!passIsolatedFOcut()) continue; //--------------------------------------------------------> possible exit of the loop

      /// ~-~-~-~-~-~-~-~-~-
      ///  MET/MT variables
      /// ~-~-~-~-~-~-~-~-~-

      evt_met    = -999;
      evt_metPhi = -999;
      evt_mt     = -999;
      computeMETandMT(evt_met, evt_metPhi, evt_mt);

      /// ~-~-~-~-~-~-~-~-~-~-
      ///  Filling Histograms
      /// ~-~-~-~-~-~-~-~-~-~-

      /// Fill MT and/or MET plot before the MET and/or MT cut is applied.
      /// For example, MT control region plot will be plotted in the following.
      fillPassTightIDLeptonMTandMETplots();

      /// This function must come after the fillPassTightIDLeptonMTandMETplots()
      /// Otherwise the histogram in that function will be messed up!
      if (!passMETandMTCut()) continue; //----------------------------------------------------------> possible exit of the loop

      /// Fill the denominator object pt plots
      fillPassFOLeptonPtPlots();

      /// Recompute the passFO if PtRatioCorrection is to be used
      if (usePtRatioCor) recomputePassFOByCorrectingPtRatio(/*output*/passFO);

      /// Compute the cone pt corr variable.
      /// This variable has less dependencies to the pt of the parent jet parton that fakes the lepton.
      coneptcorr = 0.;
      computeConePtCorrVariable(/*output*/coneptcorr);

      if (useRelIso) recomputeLeptonIDFlagsForRelIso(/*outputs*/ passId, passFO, coneptcorr);

      if (!passLooseObjectID()) continue; //--------------------------------------------------------> possible exit of the loop

      /// Compute a flat fake rate overall all pt/eta range.
      countVariablesForFlatFakeRate();

      /// Compute a pt/eta binned fake rate histogram
      fillFakeRateHistogram();

    } //end event loop
  }

  // Sanity check whether we looped over every events
  if (nEventsChain != nEventsTotal)
  {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }

  /// Compute the fake rate
  computeFlatFakeRate();

  /// Compute the full fake rate histogram
  computeFakeRateHistogram();

//	  std::cout << "jetcorr_filenames_25ns_MC_pfL1       "  << &jetcorr_filenames_25ns_MC_pfL1 << std::endl;
//	  std::cout << "jetcorr_filenames_25ns_MC_pfL1L2L3   "  << &jetcorr_filenames_25ns_MC_pfL1L2L3 << std::endl;
//	  std::cout << "jetcorr_filenames_25ns_DATA_pfL1     "  << &jetcorr_filenames_25ns_DATA_pfL1 << std::endl;
//	  std::cout << "jetcorr_filenames_25ns_DATA_pfL1L2L3 "  << &jetcorr_filenames_25ns_DATA_pfL1L2L3 << std::endl;
//	  std::cout << "jetcorr_filenames_pfL1L2L3           "  << &jetcorr_filenames_pfL1L2L3 << std::endl;

  return 0;
}

//_________________________________________________________________________________________________
void beforeLoop()
{

  // Set option.
  setGlobalConfig();

  // Set prescale factors to fix the issue where L1 prescales are unreliable.
  setTriggerPrescaleFactors();

  // Create Jet Correctors
  createJetCorrectors();

  // book all the output histograms here
  setOutputHistograms();

  // Set loop related variables (nEvents, which file, etc.)
  setLoopCondition();
}

//_________________________________________________________________________________________________
void prepData()
{

  // Load TFile -> TTree -> Initialize Lepton Tree class with the TTree
  setLeptonTree();

  // set various boolen flags that will be used to figure out which sample we're running over.
  // e.g. "isDataFromFileName", "isTtbar", etc.
  setSampleInfoByFileName();

  // Set the JEC pointer based on the input file name
  setJetCorrectorByFileName();

  // Print which file we're working on
  printWhichFileBeingLoopedOver();

  // Print which JEC we're using
  printWhichJECBeingUsed();

}



//_________________________________________________________________________________________________
void setOutputHistograms()
{

  // Example Histograms
  rootdir = gDirectory->GetDirectory("Rint:");

  // List of output histograms
  Nt_histo_e             = new TH2D("Nt_histo_e"             , "Nt vs Pt, Eta (electrons)"          , nptbins , ptbins , netabins , etabins_el); Nt_histo_e->SetDirectory(rootdir); Nt_histo_e->Sumw2();
  Nl_histo_e             = new TH2D("Nl_histo_e"             , "Nl vs Pt, Eta (electrons)"          , nptbins , ptbins , netabins , etabins_el); Nl_histo_e->SetDirectory(rootdir); Nl_histo_e->Sumw2();
  Nt_histo_mu            = new TH2D("Nt_histo_mu"            , "Nt vs Pt, Eta (muons)"              , nptbins , ptbins , netabins , etabins_mu); Nt_histo_mu->SetDirectory(rootdir); Nt_histo_mu->Sumw2();
  Nl_histo_mu            = new TH2D("Nl_histo_mu"            , "Nl vs Pt, Eta (muons)"              , nptbins , ptbins , netabins , etabins_mu); Nl_histo_mu->SetDirectory(rootdir); Nl_histo_mu->Sumw2();
  Nl_cone_histo_e        = new TH2D("Nl_cone_histo_e"        , "Nl vs Cone Energy, Eta (electrons)" , nptbins , ptbins , netabins , etabins_el); Nl_cone_histo_e->SetDirectory(rootdir); Nl_cone_histo_e->Sumw2();
  Nl_cone_histo_mu       = new TH2D("Nl_cone_histo_mu"       , "Nl vs Cone Energy, Eta (muons)"     , nptbins , ptbins , netabins , etabins_mu); Nl_cone_histo_mu->SetDirectory(rootdir); Nl_cone_histo_mu->Sumw2();
  Nt_jet_histo_e         = new TH2D("Nt_jet_histo_e"         , "Nt vs Jet Energy, Eta (electrons)"  , nptbins , ptbins , netabins , etabins_el); Nt_jet_histo_e->SetDirectory(rootdir); Nt_jet_histo_e->Sumw2();
  Nt_jet_histo_mu        = new TH2D("Nt_jet_histo_mu"        , "Nt vs Jet Energy, Eta (muons)"      , nptbins , ptbins , netabins , etabins_mu); Nt_jet_histo_mu->SetDirectory(rootdir); Nt_jet_histo_mu->Sumw2();
  Nl_jet_histo_e         = new TH2D("Nl_jet_histo_e"         , "Nl vs Jet Energy, Eta (electrons)"  , nptbins , ptbins , netabins , etabins_el); Nl_jet_histo_e->SetDirectory(rootdir); Nl_jet_histo_e->Sumw2();
  Nl_jet_histo_mu        = new TH2D("Nl_jet_histo_mu"        , "Nl vs Jet Energy, Eta (muons)"      , nptbins , ptbins , netabins , etabins_mu); Nl_jet_histo_mu->SetDirectory(rootdir); Nl_jet_histo_mu->Sumw2();
  Nt_jet_highpt_histo_e  = new TH2D("Nt_jet_highpt_histo_e"  , "Nt vs Jet Energy, Eta (electrons)"  , nptbins , ptbins , netabins , etabins_el); Nt_jet_highpt_histo_e->SetDirectory(rootdir); Nt_jet_highpt_histo_e->Sumw2();
  Nt_jet_highpt_histo_mu = new TH2D("Nt_jet_highpt_histo_mu" , "Nt vs Jet Energy, Eta (muons)"      , nptbins , ptbins , netabins , etabins_mu); Nt_jet_highpt_histo_mu->SetDirectory(rootdir); Nt_jet_highpt_histo_mu->Sumw2();
  Nl_jet_highpt_histo_e  = new TH2D("Nl_jet_highpt_histo_e"  , "Nl vs Jet Energy, Eta (electrons)"  , nptbins , ptbins , netabins , etabins_el); Nl_jet_highpt_histo_e->SetDirectory(rootdir); Nl_jet_highpt_histo_e->Sumw2();
  Nl_jet_highpt_histo_mu = new TH2D("Nl_jet_highpt_histo_mu" , "Nl vs Jet Energy, Eta (muons)"      , nptbins , ptbins , netabins , etabins_mu); Nl_jet_highpt_histo_mu->SetDirectory(rootdir); Nl_jet_highpt_histo_mu->Sumw2();
  Nt_jet_lowpt_histo_e   = new TH2D("Nt_jet_lowpt_histo_e"   , "Nt vs Jet Energy, Eta (electrons)"  , nptbins , ptbins , netabins , etabins_el); Nt_jet_lowpt_histo_e->SetDirectory(rootdir); Nt_jet_lowpt_histo_e->Sumw2();
  Nt_jet_lowpt_histo_mu  = new TH2D("Nt_jet_lowpt_histo_mu"  , "Nt vs Jet Energy, Eta (muons)"      , nptbins , ptbins , netabins , etabins_mu); Nt_jet_lowpt_histo_mu->SetDirectory(rootdir); Nt_jet_lowpt_histo_mu->Sumw2();
  Nl_jet_lowpt_histo_e   = new TH2D("Nl_jet_lowpt_histo_e"   , "Nl vs Jet Energy, Eta (electrons)"  , nptbins , ptbins , netabins , etabins_el); Nl_jet_lowpt_histo_e->SetDirectory(rootdir); Nl_jet_lowpt_histo_e->Sumw2();
  Nl_jet_lowpt_histo_mu  = new TH2D("Nl_jet_lowpt_histo_mu"  , "Nl vs Jet Energy, Eta (muons)"      , nptbins , ptbins , netabins , etabins_mu); Nl_jet_lowpt_histo_mu->SetDirectory(rootdir); Nl_jet_lowpt_histo_mu->Sumw2();

  pTrelvsIso_histo_el    = new TH2D("pTrelvsIso_histo_el"    , "pTrel vs Iso (Electrons)" , 10 , 0. , 1. , 15 , 0. , 30.); pTrelvsIso_histo_el->SetDirectory(rootdir); pTrelvsIso_histo_el->Sumw2();
  pTrelvsIso_histo_mu    = new TH2D("pTrelvsIso_histo_mu"    , "pTrel vs Iso (Muons)"     , 10 , 0. , 1. , 15 , 0. , 30.); pTrelvsIso_histo_mu->SetDirectory(rootdir); pTrelvsIso_histo_mu->Sumw2();

  NBs_BR_histo_e         = new TH1F("NBs_BR_histo_e"        , "Number of FO's from B's vs Nbtags (els)"       , 5   , 0  , 5); NBs_BR_histo_e->SetDirectory(rootdir); NBs_BR_histo_e->Sumw2();
  NBs_BR_histo_mu        = new TH1F("NBs_BR_histo_mu"       , "Number of FO's from B's vs Nbtags (muons)"     , 5   , 0  , 5); NBs_BR_histo_mu->SetDirectory(rootdir); NBs_BR_histo_mu->Sumw2();
  NnotBs_BR_histo_e      = new TH1F("NnotBs_BR_histo_e"     , "Number of FO's NOT from B's vs Nbtags (els)"   , 5   , 0  , 5); NnotBs_BR_histo_e->SetDirectory(rootdir); NnotBs_BR_histo_e->Sumw2();
  NnotBs_BR_histo_mu     = new TH1F("NnotBs_BR_histo_mu"    , "Number of FO's NOT from B's vs Nbtags (muons)" , 5   , 0  , 5); NnotBs_BR_histo_mu->SetDirectory(rootdir); NnotBs_BR_histo_mu->Sumw2();
  pTrel_histo_el         = new TH1D("pTrel_histo_el"        , "pTrel (Electrons)"                             , 15  , 0. , 30.); pTrel_histo_el->SetDirectory(rootdir); pTrel_histo_el->Sumw2();
  pTrel_histo_mu         = new TH1D("pTrel_histo_mu"        , "pTrel (Muons)"                                 , 15  , 0. , 30.); pTrel_histo_mu->SetDirectory(rootdir); pTrel_histo_mu->Sumw2();
  histo_ht               = new TH1F("histo_ht"              , "HT"                                            , 20  , 0  , 1000); histo_ht->SetDirectory(rootdir); histo_ht->Sumw2();
  histo_met              = new TH1F("histo_met"             , "MET"                                           , 20  , 0  , 1000); histo_met->SetDirectory(rootdir); histo_met->Sumw2();
  histo_met_all          = new TH1F("histo_met_all"         , "MET"                                           , 20  , 0  , 200); histo_met_all->SetDirectory(rootdir); histo_met_all->Sumw2();
  histo_met_all_el       = new TH1F("histo_met_all_el"      , "MET"                                           , 20  , 0  , 200); histo_met_all_el->SetDirectory(rootdir); histo_met_all_el->Sumw2();
  histo_met_all_mu       = new TH1F("histo_met_all_mu"      , "MET"                                           , 20  , 0  , 200); histo_met_all_mu->SetDirectory(rootdir); histo_met_all_mu->Sumw2();
  histo_met_lm           = new TH1F("histo_met_lm"          , "MET"                                           , 20  , 0  , 200); histo_met_lm->SetDirectory(rootdir); histo_met_lm->Sumw2();
  histo_met_lm_el        = new TH1F("histo_met_lm_el"       , "MET"                                           , 20  , 0  , 200); histo_met_lm_el->SetDirectory(rootdir); histo_met_lm_el->Sumw2();
  histo_met_lm_mu        = new TH1F("histo_met_lm_mu"       , "MET"                                           , 20  , 0  , 200); histo_met_lm_mu->SetDirectory(rootdir); histo_met_lm_mu->Sumw2();
  histo_met_cr           = new TH1F("histo_met_cr"          , "MET"                                           , 20  , 0  , 200); histo_met_cr->SetDirectory(rootdir); histo_met_cr->Sumw2();
  histo_met_cr_el        = new TH1F("histo_met_cr_el"       , "MET"                                           , 20  , 0  , 200); histo_met_cr_el->SetDirectory(rootdir); histo_met_cr_el->Sumw2();
  histo_met_cr_mu        = new TH1F("histo_met_cr_mu"       , "MET"                                           , 20  , 0  , 200); histo_met_cr_mu->SetDirectory(rootdir); histo_met_cr_mu->Sumw2();
  histo_mt               = new TH1F("histo_mt"              , "MT"                                            , 20  , 0  , 1000); histo_mt->SetDirectory(rootdir); histo_mt->Sumw2();
  histo_mt_all           = new TH1F("histo_mt_all"          , "MT"                                            , 20  , 0  , 200); histo_mt_all->SetDirectory(rootdir); histo_mt_all->Sumw2();
  histo_mt_all_el        = new TH1F("histo_mt_all_el"       , "MT"                                            , 20  , 0  , 200); histo_mt_all_el->SetDirectory(rootdir); histo_mt_all_el->Sumw2();
  histo_mt_all_mu        = new TH1F("histo_mt_all_mu"       , "MT"                                            , 20  , 0  , 200); histo_mt_all_mu->SetDirectory(rootdir); histo_mt_all_mu->Sumw2();
  histo_mt_lm            = new TH1F("histo_mt_lm"           , "MT"                                            , 20  , 0  , 200); histo_mt_lm->SetDirectory(rootdir); histo_mt_lm->Sumw2();
  histo_mt_lm_el         = new TH1F("histo_mt_lm_el"        , "MT"                                            , 20  , 0  , 200); histo_mt_lm_el->SetDirectory(rootdir); histo_mt_lm_el->Sumw2();
  histo_mt_lm_mu         = new TH1F("histo_mt_lm_mu"        , "MT"                                            , 20  , 0  , 200); histo_mt_lm_mu->SetDirectory(rootdir); histo_mt_lm_mu->Sumw2();
  histo_mt_cr            = new TH1F("histo_mt_cr"           , "MT"                                            , 20  , 0  , 200); histo_mt_cr->SetDirectory(rootdir); histo_mt_cr->Sumw2();
  histo_mt_cr_el         = new TH1F("histo_mt_cr_el"        , "MT"                                            , 20  , 0  , 200); histo_mt_cr_el->SetDirectory(rootdir); histo_mt_cr_el->Sumw2();
  histo_mt_cr_mu         = new TH1F("histo_mt_cr_mu"        , "MT"                                            , 20  , 0  , 200); histo_mt_cr_mu->SetDirectory(rootdir); histo_mt_cr_mu->Sumw2();
  histo_pt_mu            = new TH1F("histo_pt_mu"           , "pt mu"                                         , 100 , 0  , 200); histo_pt_mu->SetDirectory(rootdir); histo_pt_mu->Sumw2();
  histo_pt_mu8           = new TH1F("histo_pt_mu8"          , "pt mu8"                                        , 100 , 0  , 200); histo_pt_mu8->SetDirectory(rootdir); histo_pt_mu8->Sumw2();
  histo_pt_mu17          = new TH1F("histo_pt_mu17"         , "pt mu17"                                       , 100 , 0  , 200); histo_pt_mu17->SetDirectory(rootdir); histo_pt_mu17->Sumw2();
  histo_pt_mu24          = new TH1F("histo_pt_mu24"         , "pt mu24"                                       , 100 , 0  , 200); histo_pt_mu24->SetDirectory(rootdir); histo_pt_mu24->Sumw2();
  histo_pt_mu34          = new TH1F("histo_pt_mu34"         , "pt mu34"                                       , 100 , 0  , 200); histo_pt_mu34->SetDirectory(rootdir); histo_pt_mu34->Sumw2();
  histo_pt_el            = new TH1F("histo_pt_el"           , "pt el"                                         , 100 , 0  , 200); histo_pt_el->SetDirectory(rootdir); histo_pt_el->Sumw2();
  histo_pt_el8           = new TH1F("histo_pt_el8"          , "pt el8"                                        , 100 , 0  , 200); histo_pt_el8->SetDirectory(rootdir); histo_pt_el8->Sumw2();
  histo_pt_el12          = new TH1F("histo_pt_el12"         , "pt el12"                                       , 100 , 0  , 200); histo_pt_el12->SetDirectory(rootdir); histo_pt_el12->Sumw2();
  histo_pt_el17          = new TH1F("histo_pt_el17"         , "pt el17"                                       , 100 , 0  , 200); histo_pt_el17->SetDirectory(rootdir); histo_pt_el17->Sumw2();
  histo_pt_el24          = new TH1F("histo_pt_el24"         , "pt el24"                                       , 100 , 0  , 200); histo_pt_el24->SetDirectory(rootdir); histo_pt_el24->Sumw2();
  histo_pt_el34          = new TH1F("histo_pt_el34"         , "pt el34"                                       , 100 , 0  , 200); histo_pt_el34->SetDirectory(rootdir); histo_pt_el34->Sumw2();
  njets40_histo          = new TH1F("njets40_histo"         , "Njets with pT > 40 GeV"                        , 5   , 0  , 5); njets40_histo->SetDirectory(rootdir); njets40_histo->Sumw2();
  Nt_nvtx_histo_e        = new TH1D("Nt_nvtx_histo_e"       , ""                                              , 20  , 0  , 40); Nt_nvtx_histo_e->SetDirectory(rootdir); Nt_nvtx_histo_e->Sumw2();
  Nt_nvtx_histo_mu       = new TH1D("Nt_nvtx_histo_mu"      , ""                                              , 20  , 0  , 40); Nt_nvtx_histo_mu->SetDirectory(rootdir); Nt_nvtx_histo_mu->Sumw2();
  Nl_cone_nvtx_histo_e   = new TH1D("Nl_cone_nvtx_histo_e"  , ""                                              , 20  , 0  , 40); Nl_cone_nvtx_histo_e->SetDirectory(rootdir); Nl_cone_nvtx_histo_e->Sumw2();
  Nl_cone_nvtx_histo_mu  = new TH1D("Nl_cone_nvtx_histo_mu" , ""                                              , 20  , 0  , 40); Nl_cone_nvtx_histo_mu->SetDirectory(rootdir); Nl_cone_nvtx_histo_mu->Sumw2();
}

//_________________________________________________________________________________________________
void setInputArgumentVariables(
    TChain* chain_,
    TString outfile_,
    TString option_,
    bool fast_,
    int nEvents_)
{
  chain   = chain_;
  outfile = outfile_;
  option  = option_;
  fast    = fast_;
  nEvents = nEvents_;
}

//_________________________________________________________________________________________________
float computePtRel(LorentzVector lepp4, LorentzVector jetp4, bool subtractLep)
{
  /// Pt rel calculation.
  /// Pt rel is defined in SS analysis AN note SUS-16-034.
  /// The lepp4 is lepton's p4.
  /// The jetp4 is the closest jet's p4.
  /// The subtracLep is to subtract the lepton component from the jet or not.
  /// (Almost always we do subtract them.)
  if (jetp4.pt() == 0) return 0.;
  if (subtractLep) jetp4 -= lepp4;
  float dot = lepp4.Vect().Dot(jetp4.Vect());
  float ptrel = lepp4.P2() - dot * dot / jetp4.P2();
  ptrel = ptrel > 0 ? sqrt(ptrel) : 0.0;
  return ptrel;
}

//_________________________________________________________________________________________________
float getPt(float pt, bool extrPtRel)
{
  /// Converting Pt values to fit the histogramming binning.
  /// The second option extrPtRel is almost never used.
  if (!extrPtRel && pt >= 70.)
    return 69.;
  if (extrPtRel && pt >= 150.)
    return 149.;
  /// Use this if lower FR histo bound is 10.
  if (pt < 10.)  return 11.;
  return pt;
}

//_________________________________________________________________________________________________
float getEta(float eta, float HT, bool extrPtRel)
{
  /// Converting Eta values to fit the histogramming binning.
  /// The second option extrPtRel is almost never used.
  if (extrPtRel)
  {
    if (HT >= 800) return 799;
    return HT;
  }
  if (fabs(eta) >= 2.4)
    return 2.3;
  return fabs(eta);
}

//_________________________________________________________________________________________________
double calculateMt(const LorentzVector p4, double met, double met_phi)
{
  /// Computing MT variable given a four vector + met and metphi value.
  /// Pretty standard stuff. (https://en.wikipedia.org/wiki/Transverse_mass)
  float phi1 = p4.Phi();
  float phi2 = met_phi;
  float Et1  = p4.Et();
  float Et2  = met;
  return sqrt(2*Et1*Et2*(1.0 - cos(phi1-phi2)));
}

//_________________________________________________________________________________________________
void setGlobalConfig()
{
  /// The "option" gets fed from the ScanChain main function call.
  /// It parses the string and sets the configuration boolean variables.

  //default is MultiIso no SIP (Idk. what this means.)
  if (option.Contains("useRelIso"     )) useRelIso = true;
  if (option.Contains("useLooseEMVA"  )) useLooseEMVA = true;
  if (option.Contains("usePtRatioCor" )) usePtRatioCor = true;
  if (option.Contains("doBonly"       )) doBonly = true;
  if (option.Contains("doConly"       )) doConly = true;
  if (option.Contains("doLightonly"   )) doLightonly = true;
  if (option.Contains("IsoTrigs"      )) useIsoTrigs = true;
  anyPt              = false;
  doJEC              = false;
  debug              = false;
  applyDataVtxWeight = false;
  nptbins            = 5;
  netabins           = 3;
}

//_________________________________________________________________________________________________
void setTriggerPrescaleFactors()
{
  /// [6/14/17, 4:35:31 PM] Nick Amin: these are obtained with the getSF macro
  /// [6/14/17, 4:35:58 PM] Nick Amin: for the 2016 data, the L1 prescales were broken in miniaod
  /// [6/14/17, 4:36:14 PM] Nick Amin: so it was hard to guarantee the correct prescale for a trigger
  /// [6/14/17, 4:36:24 PM] Nick Amin: thus, we derived these "effective" factors and used them instead
  /// For 36.8/fb json with reRECO
  e8i = 4208.14;
  e17i = 617.166;
  e8 = 4209.02;
  e17 = 568.98;
  m8i = 3716.07;
  m17i = 181.349;
  m8 = 7361.42;
  m17 = 139.899;
}

//_________________________________________________________________________________________________
void createJetCorrectors()
{

  if (doJEC)
  {

    //-------- JEC application
    //JEC files -- 25 ns MC
    jetcorr_filenames_25ns_MC_pfL1.push_back      ("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L3Absolute_AK4PFchs.txt");

    //JEC files -- 25 ns DATA
    jetcorr_filenames_25ns_DATA_pfL1.push_back    ("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV5_DATA_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV5_DATA_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV5_DATA_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV5_DATA_L3Absolute_AK4PFchs.txt");
    jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("../../CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV5_DATA_L2L3Residual_AK4PFchs.txt");

    //Fill the JEC
    jet_corrector_25ns_MC_pfL1 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1);
    jet_corrector_25ns_MC_pfL1L2L3 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1L2L3);
    jet_corrector_25ns_DATA_pfL1 = makeJetCorrector(jetcorr_filenames_25ns_DATA_pfL1);
    jet_corrector_25ns_DATA_pfL1L2L3 = makeJetCorrector(jetcorr_filenames_25ns_DATA_pfL1L2L3);

  }

}

//_________________________________________________________________________________________________
void setLoopCondition()
{
  // Loop over events to Analyze
  nEventsChain = chain->GetEntries();

  // Override the total number of events to run over.
  if (nEvents >= 0)
    nEventsChain = nEvents;

  // Get list of files to run over
  listOfFiles = chain->GetListOfFiles();

}

//_________________________________________________________________________________________________
void setSampleInfoByFileName()
{
  /// Parse the file name and set various boolean flags to indicate which sample we're running over
  isSyncFile = TString(currentFile->GetTitle()).Contains("Sync");

  /// Apply JEC
  isDataFromFileName =
    TString(currentFile->GetTitle()).Contains("2015C") ||
    TString(currentFile->GetTitle()).Contains("2015D") ||
    TString(currentFile->GetTitle()).Contains("DoubleMu") ||
    TString(currentFile->GetTitle()).Contains("DoubleEG");
  isDoubleMuon = TString(currentFile->GetTitle()).Contains("DoubleMu");
  isQCD = TString(currentFile->GetTitle()).Contains("QCD");
  isQCDMu = TString(currentFile->GetTitle()).Contains("QCD_Mu");
  isQCDEl = TString(currentFile->GetTitle()).Contains("QCD_El");
  isTTbar = TString(currentFile->GetTitle()).Contains("TTbar");
  doTrig = isQCD;
  isEWK =
    TString(currentFile->GetTitle()).Contains("WJets") ||
    TString(currentFile->GetTitle()).Contains("DY");
}

//_________________________________________________________________________________________________
void setLeptonTree()
{
  file = new TFile(currentFile->GetTitle());
  tree = (TTree*)file->Get("t");
  if (fast) TTreeCache::SetLearnEntries(10);
  if (fast) tree->SetCacheSize(128*1024*1024);
  lepton_tree_obj.Init(tree);
}

//_________________________________________________________________________________________________
void setJetCorrectorByFileName()
{
  if (isDataFromFileName){
    jet_corrector_pfL1 = jet_corrector_25ns_DATA_pfL1;
    jet_corrector_pfL1MC = jet_corrector_25ns_MC_pfL1;
    jet_corrector_pfL1L2L3 = jet_corrector_25ns_DATA_pfL1L2L3;
    jetcorr_filenames_pfL1L2L3 = jetcorr_filenames_25ns_DATA_pfL1L2L3;
  } else {
    jet_corrector_pfL1 = jet_corrector_25ns_MC_pfL1;
    jet_corrector_pfL1MC = jet_corrector_25ns_MC_pfL1;
    jet_corrector_pfL1L2L3 = jet_corrector_25ns_MC_pfL1L2L3;
    jetcorr_filenames_pfL1L2L3 = jetcorr_filenames_25ns_MC_pfL1L2L3;
  }
}

//_________________________________________________________________________________________________
void printWhichFileBeingLoopedOver()
{
  // Print which file we're working on
  cout << " => File: " << TString(currentFile->GetTitle()) << endl;
}

//_________________________________________________________________________________________________
void printWhichJECBeingUsed()
{
  if (doJEC)
  {
    cout << "applying JEC from the following files:" << endl;
    for (unsigned int ifile = 0; ifile < jetcorr_filenames_pfL1L2L3.size(); ++ifile)
    {
      cout << (isDataFromFileName ? "DATA: " : "MC: ") << jetcorr_filenames_pfL1L2L3.at(ifile) << endl;
    }
  }
}

//_________________________________________________________________________________________________
bool isAllEventsProcessed()
{
  return (nEventsTotal >= nEventsChain);
}

//_________________________________________________________________________________________________
void loadNextEvent(unsigned int event)
{
  // Load the event from ttree
  if (fast) tree->LoadTree(event);

  // Load the variables to lepton tree
  lepton_tree_obj.GetEntry(event);

  // Count up the event counter
  ++nEventsTotal;

  // Print progress
  LeptonTree::progress(nEventsTotal, nEventsChain);

  // Print current event number and run number
  printEventInfo();

  // Print current lepton entry information
  printCurrentLeptonInfo();

  // Set whether to perform mcMatch or not
  setMCMatchBoolVariable();

  // Set event info variable (lumi, puw, weight, etc.)
  setEventInfoVariable();

  // Compute prescale and set weight variables
  computePrescaleAndReWeightByPUwgt();

}

//_________________________________________________________________________________________________
void printEventInfo()
{
  if (debug) cout << "event=" << evt_event() << " run=" << evt_run() << endl;
}

//_________________________________________________________________________________________________
void printCurrentLeptonInfo()
{
  if (debug) cout << "lepp4=" << p4() << " pt=" << p4().pt() << " eta=" << p4().eta() << " phi=" << p4().phi() << " jetp4=" << jet_close_lep() << endl;
}

//_________________________________________________________________________________________________
void printIthJetInfo(int i)
{
  if (debug) cout << "event has jet with pt,eta,phi: " << jets()[i].pt() << "," << jets()[i].eta() << "," << jets()[i].phi() << endl;
}

//_________________________________________________________________________________________________
bool isTtbarAndMotherIDPositive()
{
  if (isTTbar)
    if (motherID() > 0)
      return true;
  return false;
}

//_________________________________________________________________________________________________
bool isEventWithDifferentFlavorLeptonInSameFlavorDileptonTrigger()
{
  // Reject electrons from DoubleMu and muons from DoubleEG
  if (debug) cout << "check dataset" << endl;
  if (isData) {
    if ( isDoubleMuon && !isMuon()    ) return true;
    if (!isDoubleMuon && !isElectron()) return true;
  }
  return false;
}

//_________________________________________________________________________________________________
LorentzVector getJetP4(int i)
{
  /// Return a jet corrected jet (if doJEC is true) or just return the jet from the jet collection
  float jetcorrection = 1.0;
  float jetarea = jets_area().at(i);
  float undoJEC = jets_undoJEC().at(i);
  printIthJetInfo(i);

  if (doJEC)
  {
    if (jetcorr_filenames_pfL1L2L3.size() > 0)
    {
      jet_corrector_pfL1->setJetEta(jets()[i].eta());
      jet_corrector_pfL1->setJetPt(jets()[i].pt());
      jet_corrector_pfL1->setJetA(jetarea);
      jet_corrector_pfL1->setRho(rho());
      jetcorrection = jet_corrector_pfL1->getCorrection();
    }
    if (undoJEC > 0)
      jetcorrection /= undoJEC; // if we have undoJEC from miniAOD->CMS3, divide it out
  }
  return jets()[i]*jetcorrection;
}

//_________________________________________________________________________________________________
vector<LorentzVector> getJetP4s()
{
  /// Return a jet corrected jets (if doJEC is true) or just return the jet from the jet collection
  vector<LorentzVector> jets_recorr;
  for (unsigned int i = 0; i < jets().size(); i++)
    jets_recorr.push_back(getJetP4(i)); // add our own JEC
  return jets_recorr;
}

//_________________________________________________________________________________________________
LorentzVector getCloseJetP4()
{
  LorentzVector jet_close_lep_p4 = jet_close_lep();
  if (doJEC && jet_close_lep_idx() >= 0 && jetcorr_filenames_pfL1L2L3.size()>0) {
    jet_corrector_pfL1->setJetEta(jet_close_lep().eta());
    jet_corrector_pfL1->setJetPt(jet_close_lep().pt());
    jet_corrector_pfL1->setJetA(jet_close_lep_area());
    jet_corrector_pfL1->setRho(rho());

    jet_close_lep_p4 /= jet_close_lep_undoJEC(); // undoJEC from miniAOD->CMS3
    jet_close_lep_p4 *= jet_corrector_pfL1->getCorrection();
  }
  return jet_close_lep_p4;
}

//_________________________________________________________________________________________________
void setMCMatchBoolVariable()
{
  /// Set whether to perform mcMatch or not
  isData = evt_isRealData();
  noMCMatch = false;
  if (isData || isEWK) noMCMatch = true;
}

//_________________________________________________________________________________________________
void setEventInfoVariable()
{
  /// Set event wide info variable
  luminosity = getLumi();//in /fb
  puw = getTruePUw_Moriond(nvtx());
  weight = scale1fb()*luminosity*puw;
  if (isData) weight = 1.;
  // Correct HLT trigger efficiency for MC
//	  if (!isData) correctMCTriggerEfficiency();
}

//_________________________________________________________________________________________________
void correctMCTriggerEfficiency()
{
//	  weight *= HLTEff.getEfficiency(ss::lep1_p4().pt(),ss::lep1_p4().eta(), ss::lep1_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::lep2_id(), ss::HT(), 0);
}

//_________________________________________________________________________________________________
bool isBadWeightEvents()
{
  if (isData==0 && isEWK==0) {

    /* 80X Moriond maps
       QCD_Pt-20to30_EMEnriched         881.181274
       QCD_Pt-30to50_EMEnriched         1618.659058
       QCD_Pt-50to80_EMEnriched         150.182861
       QCD_Pt-80to120_EMEnriched        9.359426
       QCD_Pt-120to170_EMEnriched       1.836049
       QCD_Pt-170to300_EMEnriched       1.653828
       QCD_Pt-300toInf_EMEnriched       0.198102

       QCD_Pt_15to20_bcToE              98.568138
       QCD_Pt_20to30_bcToE              35.548122
       QCD_Pt_30to80_bcToE              30.127947
       QCD_Pt_80to170_bcToE             2.624143
       QCD_Pt_170to250_bcToE            0.306375
       QCD_Pt_250toInf_bcToE            0.084854

       QCD_Pt-20toInf_MuEnrichedPt15    14.833723

       QCD_Pt-15to20_MuEnrichedPt5      978.755066
       QCD_Pt-20to30_MuEnrichedPt5      106.777184
       QCD_Pt-30to50_MuEnrichedPt5      56.263439
       QCD_Pt-50to80_MuEnrichedPt5      23.256641
       QCD_Pt-80to120_MuEnrichedPt5     8.022378
       QCD_Pt-120to170_MuEnrichedPt5    3.168120
       QCD_Pt-170to300_MuEnrichedPt5    1.029381
       QCD_Pt-300to470_MuEnrichedPt5    0.036246
       QCD_Pt-470to600_MuEnrichedPt5    0.015916
       QCD_Pt-600to800_MuEnrichedPt5    0.007053
       QCD_Pt-800to1000_MuEnrichedPt5   0.001322
       QCD_Pt-1000toInf_MuEnrichedPt5   0.000198
       */

    if (isMuon()) {

      if (debug) cout << "check qcd" << endl;

      bool nonEnriched = false;

      if (nonEnriched) {
        // throw away el pt_uncorr<15 for nonenriched FIXME
        if (p4().pt()<15) return true;
      } else {
        // 76X
        // if (p4().pt()<15. &&  scale1fb() > 14.2 && scale1fb() < 14.3 ) return true;  //take only Mu15 above pT=15
        // if (p4().pt()>15. && (scale1fb() < 14.2 || scale1fb() > 14.3)) return true;  //take only Mu5 below pT=15
        // if (scale1fb() < 1.2 || scale1fb() > 851.) return true; //avoid extreme ranges and weights

        if (isQCDMu) {
          // Moriond 80X
          if (p4().pt()<15. &&  scale1fb() > 14.8 && scale1fb() < 14.9 ) return true;  //take only Mu15 above pT=15
          if (p4().pt()>15. && (scale1fb() < 14.8 || scale1fb() > 14.9)) return true;  //take only Mu5 below pT=15
          if (scale1fb() < 1.03 || scale1fb() > 600.) return true; //avoid extreme ranges and weights
        }
      }

      // // 74X
      // if (p4().pt()<15. &&  scale1fb() > 22.9 && scale1fb() < 23.0 ) return true;  //take only Mu15 above pT=15
      // if (p4().pt()>15. && (scale1fb() < 22.9 || scale1fb() > 23.0)) return true;  //take only Mu5 below pT=15
      // if (scale1fb() < 5.0 || scale1fb() > 600.) return true; //avoid extreme ranges and weights

    }

    if (isElectron()) {
      /*
      //Map of samples and correspongding scale1fb
      /QCD_Pt_15to20_bcToE   280.49151
      /QCD_Pt_20to30_bcToE   101.22869
      /QCD_Pt_30to80_bcToE   79.645675
      /QCD_Pt_80to170_bcToE  7.4107956
      /QCD_Pt_170to250_bcToE 0.9143287
      /QCD_Pt_250toInf_bcToE 0.2412396
      /QCD_Pt-15to20_EMEnriched   1085.0196
      /QCD_Pt-20to30_EMEnriched   583.37152
      /QCD_Pt-30to50_EMEnriched   2114.1918
      /QCD_Pt-50to80_EMEnriched   553.06378
      /QCD_Pt-80to120_EMEnriched  43.048187
      /QCD_Pt-120to170_EMEnriched 7.4082317
      /QCD_Pt-170to300_EMEnriched 3.2795264
      /QCD_Pt-300toInf_EMEnriched 0.3651889
      */


      // // FIXME
      // //if(isData==0 && scale1fb() > 100000.) return true;  //excludes 5to10 and 10to20 EM Enriched, 15to30 non-Enriched
      // if (debug) cout << "check qcd" << endl;
      // if (scale1fb() < 5.0) return true; //avoid extreme ranges and weights
      // if (scale1fb() > 280 && scale1fb() < 281) return true;
      // if (scale1fb() > 1085 && scale1fb() < 1086) return true;

    }

    //fixme
    //make sure we use mu from MuEnrich and el from EG+BCtoE
    // if (isData==0 && abs(id())==13 && fabs(scale1fb()-20.94)>0.1 && fabs(scale1fb()-79.81)>0.1 && fabs(scale1fb()-85.19)>0.1 && fabs(scale1fb()-357.93)>0.1) return true;
    // if (isData==0 && abs(id())==11 && !(fabs(scale1fb()-20.94)>0.1 && fabs(scale1fb()-79.81)>0.1 && fabs(scale1fb()-85.19)>0.1 && fabs(scale1fb()-357.93)>0.1)) return true;

  }
  return false;
}

//_________________________________________________________________________________________________
bool isBadEvents()
{
  if (isBadMCEvents()) return true;
  if (isBadDataEvents()) return true;
  return false;
}

//_________________________________________________________________________________________________
bool isBadMCEvents()
{

  // Some QCD events have large weights and (some bad pt range?). Toss them.
  if (isBadWeightEvents()) return true;

  // ?? Why??
  if (isTtbarAndMotherIDPositive()) return true;

  return false;
}

//_________________________________________________________________________________________________
bool isBadDataEvents()
{
  // Reject electrons from DoubleMu and muons from DoubleEG
  if (isEventWithDifferentFlavorLeptonInSameFlavorDileptonTrigger()) return true;
  return false;
}

//_________________________________________________________________________________________________
void computeJetRelatedVariables(float& HT, int& njets40, int& nbtags, bool& pass_jetptcut)
{
  /// This function wants to compute 4 things.
  /// 1. HT of the event.
  /// 2. Njets with pT > 40.
  /// 3. N btags with pT > 40.
  /// 4. boolean flag for passing a jet pt cut or not

  /// First get list of jets (jet corrected, if doJEC is true.)
  vector<LorentzVector> jets_recorr = getJetP4s();

  /// Then, loop over the jets
  for (unsigned int i = 0; i < jets_recorr.size(); i++)
  {
    /// Require that the jets are dR > 1 away from the lepton
    if (ROOT::Math::VectorUtil::DeltaR(jets_recorr[i], p4()) < 1.)
      continue; //0.4 in babymaker

    /// Jet's CSV discriminator of value above 0.8 are considered btagged
    if (jets_disc()[i] > 0.800)
      nbtags++;

    /// Compute HT if the pt of the jets are above 40 and eta < 2.4
    /// Also count njets pt > 40.
    if (jets_recorr[i].pt() > 40. && fabs(jets_recorr[i].eta()) < 2.4)
    {
      HT += jets_recorr[i].pt();
      njets40++;
      if (debug)
      {
        cout << "-->recoiljet:" << jets_recorr[i].pt() << "," << jets_recorr[i].eta() << "," << jets_recorr[i].phi() << endl;
        cout << "-->deltaR " << ROOT::Math::VectorUtil::DeltaR(jets_recorr[i], p4()) << endl;
      }
    }
  }

  /// If there are at least jet with pT > 40 then it passes the jet pt cut.
  /// (i.e. jet pt > 40 GeV cut.)
  if (njets40 > 0)
    pass_jetptcut = true;

  if (debug) cout << "check jet: njets40=" << njets40 << " ht_SS=" << ht_SS() << endl;

}

//_________________________________________________________________________________________________
void computeLeptonIsolationVariables(float& ptrel, float& closejetpt, float& relIso)
{
  // Get the p4 of the closest jet to the lepton
  LorentzVector jet_close_lep_p4 = getCloseJetP4();

  LorentzVector closejet = (jet_close_lep_p4*jet_close_lep_undoJEC()*jet_close_L1() - p4())*jet_close_L2L3() + p4(); // V5
  ptrel      = computePtRel(p4(), closejet, true);
  closejetpt = closejet.pt();
  relIso     = RelIso03EA();

  if (debug)
    cout << "close jet raw p4=" << jet_close_lep_p4*jet_close_lep_undoJEC()
         << " pt="              << (jet_close_lep_p4*jet_close_lep_undoJEC()).pt()
         << " corrected p4="    << closejet
         << " pt="              << closejet.pt()
         << " L1="              << jet_close_L1()
         << " L2L3="            << jet_close_L2L3()
         << " rho="             << rho()
         << " ptrel="           << ptrel
         << " ptratio="         << p4().pt()/closejetpt
         << endl;

  if (debug)
  {
    cout << "lepton pt,eta,phi: " << p4().pt() << "," << p4().eta() << "," << p4().phi() << endl;
    cout << "closejet pt,eta,phi: " << closejet.pt() << "," << closejet.eta() << "," << closejet.phi() << endl;
    cout << "deltaR " << ROOT::Math::VectorUtil::DeltaR(closejet,p4()) << endl;
  }

}

//_________________________________________________________________________________________________
bool passJetPtCutOrHTCut()
{
  /// pass either at least one jet with pt > 40 or HT > 40.
  if( !pass_jetptcut || ht_SS()<40 )
    return false;
  else
    return true;
}

//_________________________________________________________________________________________________
bool passTriggerSelection()
{
  /// This function checks whether the event passes some triggre requirements.
  if (debug) cout << "check hlt HLT_Mu8=" << HLT_Mu8() << " HLT_Mu17=" << HLT_Mu17() << " HLT_Ele12_CaloIdM_TrackIdM_PFJet30=" << HLT_Ele12_CaloIdM_TrackIdM_PFJet30() << endl;
  if (isElectron() && (isData || doTrig))
  {
    // ele12 for 2015D
    if (useIsoTrigs)
    {
      if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0 && HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0) return false;
    }
    else
    {
      if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30()<=0 && HLT_Ele17_CaloIdM_TrackIdM_PFJet30()<=0) return false;
    }
  }
  if (isMuon() && (isData || doTrig)) {
    // mu8 or mu17 for 2015D
    if (useIsoTrigs) {
      if (HLT_Mu8_TrkIsoVVL()<=0 && HLT_Mu17_TrkIsoVVL()<=0) return false;
    }
    else
    {
      if (HLT_Mu8()<=0 && HLT_Mu17()<=0) return false;
    }
  }
  // From the function "computePrescaleAndReWeightByPUwgt()" the prescale is computed.
  // If the prescale is less than or equal to zero, skip the event.
  if (prescale <= 0) return false;
  return true;
}

//_________________________________________________________________________________________________
void computePrescaleAndReWeightByPUwgt()
{
  /// This function fixes the L1 prescale info being screwed up in the miniAOD.
  /// Using a separate macro called getSF.C the prescale values are computed.
  /// variables like e8i, contains the prescales, (look for it in the header file.)
  /// While at it, it also sets a boolean flag whether it passes the pt cut for the trigger or not.
  /// Eventually outputs, prescale, and passed_ptcut_for_trigger.

  float vtxWeight = 0.;
  // Check prescales, apply cuts on the pT range depending on the trigger
  if (!isData) prescale = 1; // triggers messed up in 80X MC
  if (isElectron() && (isData || doTrig))
  {
    if (useIsoTrigs)
    {
      if (p4().pt() >= 10 && p4().pt() < 25 && HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30()>0)
      {
        prescale = HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30();
        if (!doTrig) prescale = e8i; // FIXME
        vtxWeight = getPUw_iso_8_el(nvtx());
      }
      if ((anyPt || p4().pt() >= 25) && HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30()>0)
      {
        prescale = HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30();
        if (!doTrig) prescale = e17i; // FIXME
        vtxWeight = getPUw_iso_17_el(nvtx());
      }
      if (prescale>0) weight *= prescale;
    }
    else
    {
      if (p4().pt() >= 10 && p4().pt() < 25 && HLT_Ele8_CaloIdM_TrackIdM_PFJet30()>0)
      {
        prescale = HLT_Ele8_CaloIdM_TrackIdM_PFJet30();
        if (!doTrig) prescale = e8; // FIXME
        vtxWeight = getPUw_8_el(nvtx());
      }
      if ((anyPt || p4().pt() >= 25) && HLT_Ele17_CaloIdM_TrackIdM_PFJet30()>0)
      {
        prescale = HLT_Ele17_CaloIdM_TrackIdM_PFJet30();
        if (!doTrig) prescale = e17; // FIXME
        vtxWeight = getPUw_17_el(nvtx());
      }
      if (prescale>0) weight *= prescale;
    }
  }
  if (isMuon() && (isData || doTrig))
  {
    // use mu8+mu17
    if (useIsoTrigs)
    {
      if (p4().pt()>=10 && p4().pt()<25 && HLT_Mu8_TrkIsoVVL()>0)
      {
        prescale = HLT_Mu8_TrkIsoVVL();
        if (!doTrig) prescale = m8i; // FIXME
        vtxWeight = getPUw_iso_8_mu(nvtx());
      }

      if ((anyPt || p4().pt()>=25) && HLT_Mu17_TrkIsoVVL()>0)
      {
        prescale = HLT_Mu17_TrkIsoVVL();
        if (!doTrig) prescale = m17i; // FIXME
        vtxWeight = getPUw_iso_17_mu(nvtx());
      }

      if (prescale>0) weight *= prescale;
    }
    else
    {
      if (p4().pt()>=10 && p4().pt()<25 && HLT_Mu8()>0)
      {
        prescale = HLT_Mu8();
        if (!doTrig) prescale = m8; // FIXME
        vtxWeight = getPUw_8_mu(nvtx());
      }

      if ((anyPt || p4().pt() >= 25) && HLT_Mu17()>0)
      {
        prescale = HLT_Mu17();
        if (!doTrig) prescale = m17; // FIXME
        vtxWeight = getPUw_17_mu(nvtx());
      }

      if (prescale>0) weight *= prescale;
    }
  }

  if (applyDataVtxWeight)
    weight *= vtxWeight;
}

//_________________________________________________________________________________________________
bool passPtEtaSIPNFOLep()
{
  if (debug) cout << "check nFO_SS" << endl;
  /// Require one lepton event
  if (nFOs_SS() > 1) //if more than 1 FO in event
    return false;

  // Ditch bounds here and just enforce correct reading of histo in getFakeRate() in app_region/ScanChain.C???
  // If we dont want leptons w/ |eta|>2.4 in ttbar application, filling rate histos with leptons w/
  //  |eta|>2.4 will mess up the rate in the highest eta bins (2<|eta|<3)
  // Don't think eta cut is anywhere else
  if (debug) cout << "check pt/eta" << endl;
  if (p4().pt() < 10.) return false;
  if (isElectron() && fabs(p4().eta()) > 2.5) return false;
  if (isMuon() && fabs(p4().eta()) > 2.4) return false;
  if (doLightonly && isElectron() && p4().pt() < 20.) return false; //because EMEnriched does not go below 20 GeV
  if (debug) cout << "check sip " << fabs(ip3d()/ip3derr()) << endl;
  if (fabs(ip3d()/ip3derr())>4. ) return false;
  return true;
}

//_________________________________________________________________________________________________
void computeLeptonIDFlags(bool& passId, bool& passFO, bool& passId_noiso, bool& passFO_noiso)
{

  /// This function may be the most important part of the code.
  /// This function sets the lepton ID flags being used to measure the fake rate.
  /// This function ultimately computes the four flags for the given lepton that we're looping over.
  /// pass_Id       : Tight ID = ID,
  /// pass_FO       : Fake-able object ID = FO
  /// pass_Id_noiso : non-isolated Tight ID = ID,
  /// pass_Id_noiso : non-isolated Fake-able object ID = FO

  /// Currently set to the same-sign analysis lepton IDs. (SUS-16-034)
  passId = passes_SS_tight_v5();
  passFO = passes_SS_fo_v5();
  passId_noiso = passes_SS_tight_noiso_v5();
  passFO_noiso = passes_SS_fo_noiso_v5();

  /// For electrons, there are some "trigger safe cuts" that we also apply on top of our ID.
  /// https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#HLT_safe_selection_for_2016_data
  if (useLooseEMVA && isElectron())
  {
    bool isEB = true;
    if (fabs(etaSC())>1.479) isEB = false;
    float sIeIe = sigmaIEtaIEta_full5x5();
    float hoe = hOverE();
    float deta = fabs(dEtaIn());
    float dphi = fabs(dPhiIn());
    float invep = fabs(1./ecalEnergy() - 1./p4().P());
    float cut_sIeIe = isEB ? 0.011 : 0.031;
    float cut_hoe   = 0.08;
    float cut_deta  = 0.01;
    float cut_dphi  = isEB ? 0.04 : 0.08;
    float cut_invep = 0.01;
    bool passHltCuts = ( sIeIe<cut_sIeIe && hoe<cut_hoe && deta<cut_deta && dphi<cut_dphi && invep<cut_invep );
    if (useIsoTrigs) {
      float ePFIso = ecalPFClusterIso()/p4().pt();
      float hPFIso = hcalPFClusterIso()/p4().pt();
      float trkIso = tkIso()/p4().pt();
      float cut_ePFIso = 0.45;
      float cut_hPFIso = 0.25;
      float cut_trkIso  = 0.2;
      passHltCuts = passHltCuts && ePFIso<cut_ePFIso && hPFIso<cut_hPFIso && trkIso<cut_trkIso;
    }
    passFO = passHltCuts && passes_SS_fo_looseMVA_v5();
    passFO_noiso = passHltCuts && passes_SS_fo_looseMVA_noiso_v5();
  }
}

//_________________________________________________________________________________________________
bool passIsolatedFOcut()
{
  /// Check whether the event is an isolated FO.
  /// The function definition is in the commonUtils.h
  if (useLooseEMVA && isElectron())
  {
    if (useIsoTrigs) {
      if (debug) cout << "check iso FO" << endl;
      if (!passIsolatedFO(id(),etaSC(),mva_25ns(),p4().pt())) return false;
    }
  }
  return true;
}

//_________________________________________________________________________________________________
void fillPassTightIDLeptonMTandMETplots()
{
  /// This function fills the histograms for MT control region, (I think?)
  /// This function must be called before exiting the main loop with MET/MT cut.
  if (passId) {
    //mt control region
    if (evt_met > 30. && p4().pt()>30) {
      if (true        ) histo_mt_all   ->Fill( std::min(evt_mt,float(200.)), weight );
      if (isElectron()) histo_mt_all_el->Fill( std::min(evt_mt,float(200.)), weight );
      if (isMuon()    ) histo_mt_all_mu->Fill( std::min(evt_mt,float(200.)), weight );
    }
    if (evt_met < 20.) {
      if (true        ) histo_mt_lm   ->Fill( std::min(evt_mt,float(200.)), weight );
      if (isElectron()) histo_mt_lm_el->Fill( std::min(evt_mt,float(200.)), weight );
      if (isMuon()    ) histo_mt_lm_mu->Fill( std::min(evt_mt,float(200.)), weight );
    }
    if (evt_met > 30.) {
      if (true        ) histo_mt_cr   ->Fill( std::min(evt_mt,float(200.)), weight );
      if (isElectron()) histo_mt_cr_el->Fill( std::min(evt_mt,float(200.)), weight );
      if (isMuon()    ) histo_mt_cr_mu->Fill( std::min(evt_mt,float(200.)), weight );
    }
    //test if bad data/MC ratio in mt control region is due to met
    if (p4().pt()>30) {
      if (true        ) histo_met_all   ->Fill( std::min(evt_met,float(200.)), weight );
      if (isElectron()) histo_met_all_el->Fill( std::min(evt_met,float(200.)), weight );
      if (isMuon()    ) histo_met_all_mu->Fill( std::min(evt_met,float(200.)), weight );
    }
    if (evt_mt < 20.) {
      if (true        ) histo_met_lm   ->Fill( std::min(evt_met,float(200.)), weight );
      if (isElectron()) histo_met_lm_el->Fill( std::min(evt_met,float(200.)), weight );
      if (isMuon()    ) histo_met_lm_mu->Fill( std::min(evt_met,float(200.)), weight );
    }
    if (evt_mt > 30.) {
      if (true        ) histo_met_cr   ->Fill( std::min(evt_met,float(200.)), weight );
      if (isElectron()) histo_met_cr_el->Fill( std::min(evt_met,float(200.)), weight );
      if (isMuon()    ) histo_met_cr_mu->Fill( std::min(evt_met,float(200.)), weight );
    }
  }
}

//_________________________________________________________________________________________________
bool passMETandMTCut()
{
  /// This function checks whether it passes the MET and MT cut
  if (debug) cout << "check met/mt " << evt_met << " / " << evt_mt << " metPhi=" << evt_metPhi << endl;

  if(!( evt_met < 20. && evt_mt < 20 )) // I want my leptons to be either low MET or low MT
    return false;

  return true;
}

//_________________________________________________________________________________________________
void fillPassFOLeptonPtPlots()
{
  if (isData && passFO)
  {
    if (isElectron())
    {
      if (true                                    ) histo_pt_el  -> Fill(p4().pt(), prescale                            );
      if (HLT_Ele33_CaloIdM_TrackIdM_PFJet30() > 0) histo_pt_el34-> Fill(p4().pt(), HLT_Ele33_CaloIdM_TrackIdM_PFJet30());
      if (HLT_Ele23_CaloIdM_TrackIdM_PFJet30() > 0) histo_pt_el24-> Fill(p4().pt(), HLT_Ele23_CaloIdM_TrackIdM_PFJet30());
      if (HLT_Ele17_CaloIdM_TrackIdM_PFJet30() > 0) histo_pt_el17-> Fill(p4().pt(), HLT_Ele17_CaloIdM_TrackIdM_PFJet30());
      if (HLT_Ele12_CaloIdM_TrackIdM_PFJet30() > 0) histo_pt_el12-> Fill(p4().pt(), HLT_Ele12_CaloIdM_TrackIdM_PFJet30());
      if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30()  > 0) histo_pt_el8 -> Fill(p4().pt(), HLT_Ele8_CaloIdM_TrackIdM_PFJet30 ());
    }
    if (isMuon())
    {
      if (true          ) histo_pt_mu  -> Fill(p4().pt(), prescale  );
      if (HLT_Mu34() > 0) histo_pt_mu34-> Fill(p4().pt(), HLT_Mu34());
      if (HLT_Mu24() > 0) histo_pt_mu24-> Fill(p4().pt(), HLT_Mu24());
      if (HLT_Mu17() > 0) histo_pt_mu17-> Fill(p4().pt(), HLT_Mu17());
      if (HLT_Mu8()  > 0) histo_pt_mu8 -> Fill(p4().pt(), HLT_Mu8 ());
    }
  }
}

//_________________________________________________________________________________________________
void recomputePassFOByCorrectingPtRatio(bool& passFO)
{
  /// This function recomputes the passFO flag via using the corrected pt ratio variable.
  /// The details of the definition and the "Why" may be found in AN's of the SS analysis.
  if (isElectron())
  {
    float ptratiocor = closejetpt > 0. ? p4().pt() * (1 + std::max(0., miniiso()-0.12)) / closejetpt : 1.;
    passFO = passes_SS_fo_v5() && (ptratiocor > 0.76 || ptrel > 7.2);
  }
  else
  {
    float ptratiocor = closejetpt > 0. ? p4().pt() * (1 + std::max(0., miniiso()-0.16)) / closejetpt : 1.;
    passFO = passes_SS_fo_v5() && (ptratiocor > 0.80 || ptrel > 7.2);
  }
}

//_________________________________________________________________________________________________
void computeConePtCorrVariable(float& coneptcorr)
{
  /// This function computes the conePtCorr variable.
  /// The details of the definition and the "Why" may be found in AN's of the SS analysis.
  /// The gist of the variable is that this is a variable that has less
  /// dependencies to the pt of the parent jet parton that fakes a lepton.
  if (isElectron())
  {
    if (ptrel > 7.2)
    {
      coneptcorr = std::max(0., miniiso()-0.12);
    }
    else
    {
      coneptcorr = max(double(0.), (closejetpt * 0.80 / p4().pt() - 1.));
    }
  }
  else
  {
    if (ptrel > 7.2)
    {
      coneptcorr = std::max(0., miniiso() - 0.16);
    }
    else
    {
      coneptcorr = max(double(0.), (closejetpt * 0.76 / p4().pt() - 1.));
    }
  }
}

//_________________________________________________________________________________________________
void recomputeLeptonIDFlagsForRelIso(
    bool& passId,
    bool& passFO,
    float& coneptcorr)
{
  /// This function recomputes the passId, and passFO flag with some differen reliso variable.
  passId = passId_noiso && relIso<0.1;
  passFO = passFO_noiso && relIso<0.5;
  coneptcorr = std::max(0.,relIso-0.1);
}

//_________________________________________________________________________________________________
bool passLooseObjectID()
{
  if (debug) cout << Form("check FO miniiso=%.2f ptratio=%.2f ptrel=%5.2f mva=%5.2f",miniiso(),p4().pt()/closejetpt,ptrel,mva_25ns()) << endl;
  if (!passFO) return false;
  if (debug) cout << "passed FO" << endl;
  return true;
}

//_________________________________________________________________________________________________
void computeMETandMT(float& evt_met, float& evt_metPhi, float& evt_mt)
{
  evt_met = evt_corrMET();
  evt_metPhi = evt_corrMETPhi();
  evt_mt = calculateMt(p4(),evt_met,evt_metPhi);
}

//_________________________________________________________________________________________________
bool isNonPromptLepton()
{
  return (motherID() <= 0 &&
      (doBonly==0     || motherID() == -1) &&
      (doConly==0     || motherID() == -2) &&
      (doLightonly==0 || motherID() ==  0));
}

//_________________________________________________________________________________________________
void countVariablesForFlatFakeRate()
{
  /// Find ratio of nonprompt leps passing tight to nonprompt leps passing at least loose.  This is the fake rate
  /// Use lep_passes_id to see if num.  Use FO to see if Fakable Object (denom)
  /// Calculate e=Nt/(Nl) where l->loose  (as opposed to loose-not-tight).

  /// Using gen level info to see if prompt -> no prompt contamination in measurement region
  /// everything else is RECO (p4, id, passes_id, FO, etc.)

  if (noMCMatch || isNonPromptLepton() )  //if lep is nonprompt
  {

    if (isElectron()) //it's an el
    {
      if (passId)  //if el is tight
      {
        Nt = Nt + weight;
        Nt_e = Nt_e + weight;
      }
      if (passFO)
      {
        Nl = Nl + weight;     //l now means loose, as opposed to loose-not-tight
        Nl_e = Nl_e + weight;
      }
    }

    if (isMuon()) //it's a mu
    {
      if (passId)  //if mu is tight
      {
        Nt = Nt + weight;
        Nt_mu = Nt_mu + weight;
      }
      if (passFO)
      {
        Nl = Nl + weight;     //l now means loose, as opposed to loose-not-tight
        Nl_mu = Nl_mu + weight;
      }
    }
  }
}

//_________________________________________________________________________________________________
bool isElectron()
{
  return (abs(id()) == 11);
}

//_________________________________________________________________________________________________
bool isMuon()
{
  return (abs(id()) == 13);
}

//_________________________________________________________________________________________________
void fillFakeRateHistogram()
{

  //Find ratio of nonprompt leps passing tight to nonprompt leps passing at least loose.  This is the fake rate
  // Use lep_passes_id to see if num.  Use FO to see if Fakable Object (denom)
  //Calculate e=Nt/(Nl) where l->loose  (as opposed to loose-not-tight).

  //Using gen level info to see if prompt -> no prompt contamination in measurement region
  //everything else is RECO (p4, id, passes_id, FO, etc.)

  if (noMCMatch || isNonPromptLepton())  //if el is nonprompt (GEN info)
  {

    if (passFO)
    {
      histo_ht ->Fill( std::min(HT     , float(1000.)) );
      histo_met->Fill( std::min(evt_met, float(1000.)) );
      histo_mt ->Fill( std::min(evt_mt , float(1000.)) );

      if (isElectron()) pTrelvsIso_histo_el->Fill( std::min(RelIso03EA(), float(0.99)), std::min(ptrel,float(29.9)) );
      if (isMuon()    ) pTrelvsIso_histo_mu->Fill( std::min(RelIso03EA(), float(0.99)), std::min(ptrel,float(29.9)) );
      if (isElectron()) pTrel_histo_el     ->Fill( std::min(ptrel,        float(29.9)) );
      if (isMuon()    ) pTrel_histo_mu     ->Fill( std::min(ptrel,        float(29.9)) );
    }

    if (isElectron()) // it's an el
    {
      if (passId)  //if el is tight
      {
        //uncorrected and cone corrected FR
        Nt_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),HT,false), weight);   //

        // NJA
        Nt_nvtx_histo_e->Fill(nvtx(), weight);   //

        //jet corrected FR
        Nt_jet_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
        if (p4().pt()>25.) Nt_jet_highpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
        else Nt_jet_lowpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
      }

      if (passFO)  //if el is FO
      {
        //not corrected FR
        Nl_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),HT,false), weight);   //  <-- loose (as opposed to l!t)
        //cone corrected FR
        if( passId ) Nl_cone_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),HT,false), weight);   //  <-- loose (as opposed to l!t)
        else Nl_cone_histo_e->Fill(getPt(p4().pt()*(1+coneptcorr),false), getEta(fabs(p4().eta()),HT,false), weight);

        // NJA
        Nl_cone_nvtx_histo_e->Fill(nvtx(), weight);

        //jet corrected FR
        Nl_jet_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
        if (p4().pt()>25.) Nl_jet_highpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
        else Nl_jet_lowpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);

        if (isSyncFile) {
          cout << Form("Electron FO raw pt=%6.2f corr pt=%6.2f eta=%5.2f miniiso=%.2f ptratio=%.2f ptrel=%5.2f mva=%5.2f isNum=%1i met=%5.2f mt=%5.2f event %i",
              p4().pt(),p4().pt()*(1+coneptcorr),p4().eta(),miniiso(),p4().pt()/closejetpt,ptrel,mva_25ns(),passId,evt_met,evt_mt,(int)evt_event()) << endl;
        }

        njets40_histo->Fill(njets40, weight);

        if (noMCMatch==0 && doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
        {
          if(motherID()==-1){
            NBs_BR_histo_e ->Fill(nbtags, weight);
            Bs_e = Bs_e + weight;
          }
          else if(motherID()==-2 || motherID()==0){
            NnotBs_BR_histo_e ->Fill(nbtags, weight);
            notBs_e = notBs_e + weight;
          }
        }
      }
    }
    if (isMuon()) // it's a mu
    {
      if (passId)  //if mu is tight
      {
        //uncorrected and cone corrected FR
        Nt_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),HT,false), weight);   //

        // NJA
        Nt_nvtx_histo_mu->Fill(nvtx(), weight);   //

        //jet corrected FR
        Nt_jet_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
        if (p4().pt()>25.) Nt_jet_highpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
        else Nt_jet_lowpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
      }

      if (passFO)  //if mu is FO
      {
        //not corrected FR
        Nl_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),HT,false), weight);   //  <-- loose (as opposed to l!t)
        //cone corrected FR
        if( passId ) Nl_cone_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),HT,false), weight);   //  <-- loose (as opposed to l!t)
        else Nl_cone_histo_mu->Fill(getPt(p4().pt()*(1+coneptcorr),false), getEta(fabs(p4().eta()),HT,false), weight);

        // NJA
        Nl_cone_nvtx_histo_mu->Fill(nvtx(), weight);

        //jet corrected FR
        Nl_jet_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
        if (p4().pt()>25.) Nl_jet_highpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);
        else Nl_jet_lowpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),HT,false), weight);

        if (isSyncFile) {

          cout << Form("%1llu %7.3f %7.3f %6.3f %6.3f %6.3f %6.3f %1i %6.3f %6.3f %6.0f",(unsigned long long)evt_event() , p4().pt(),p4().pt()*(1+coneptcorr),p4().eta(),miniiso(),p4().pt()/closejetpt,ptrel,passId,evt_met,evt_mt,weight) << endl;
        }

        njets40_histo->Fill(njets40, weight);

        if (noMCMatch==0 && doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
        {
          if(motherID()==-1){
            NBs_BR_histo_mu ->Fill(nbtags, weight);
            Bs_mu = Bs_mu + weight;
          }
          else if(motherID()==-2 || motherID()==0){
            NnotBs_BR_histo_mu ->Fill(nbtags, weight);
            notBs_mu = notBs_mu + weight;
          }
        }
      }
    }
  }
}

//_________________________________________________________________________________________________
void computeFlatFakeRate()
{
  /// Outside of the loop, now compute the flat fake rate
  e = Nt/(Nl);
  e_e = Nt_e/(Nl_e);
  e_mu = Nt_mu/(Nl_mu);

  cout<<"\nReco: "<<"Nt = "<<Nt<<", Nl = "<<Nl<<", e ="<<e<<endl;
  cout<<"\nReco (el): "<<"Nt = "<<Nt_e<<", Nl = "<<Nl_e<<", e ="<<e_e<<endl;
  cout<<"\nReco (mu): "<<"Nt = "<<Nt_mu<<", Nl = "<<Nl_mu<<", e ="<<e_mu<<endl<<endl;
  cout<<"\nAve B abundance (els)= "<<Bs_e/(Bs_e + notBs_e)<<endl;
  cout<<"Ave B abundance (mus)= "<<Bs_mu/(Bs_mu + notBs_mu)<<endl;

}

//_________________________________________________________________________________________________
void computeFakeRateHistogram()
{
  //Histograms
  // TH2D *rate_histo = (TH2D*) Nt_histo->Clone("rate_histo");
  TH2D *rate_histo_e = (TH2D*) Nt_histo_e->Clone("rate_histo_e");
  TH2D *rate_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_histo_mu");
  TH2D *rate_cone_histo_e = (TH2D*) Nt_histo_e->Clone("rate_cone_histo_e");
  TH2D *rate_cone_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_cone_histo_mu");
  TH2D *rate_jet_histo_e = (TH2D*) Nt_jet_histo_e->Clone("rate_jet_histo_e");
  TH2D *rate_jet_histo_mu = (TH2D*) Nt_jet_histo_mu->Clone("rate_jet_histo_mu");
  TH2D *rate_jet_highpt_histo_e = (TH2D*) Nt_jet_highpt_histo_e->Clone("rate_jet_highpt_histo_e");
  TH2D *rate_jet_highpt_histo_mu = (TH2D*) Nt_jet_highpt_histo_mu->Clone("rate_jet_highpt_histo_mu");
  TH2D *rate_jet_lowpt_histo_e = (TH2D*) Nt_jet_lowpt_histo_e->Clone("rate_jet_lowpt_histo_e");
  TH2D *rate_jet_lowpt_histo_mu = (TH2D*) Nt_jet_lowpt_histo_mu->Clone("rate_jet_lowpt_histo_mu");
  TH1F *total_BR_histo_e = (TH1F*) NBs_BR_histo_e->Clone("total_BR_histo_e");
  TH1F *total_BR_histo_mu = (TH1F*) NBs_BR_histo_mu->Clone("total_BR_histo_mu");

  // NJA
  TH1D *rate_cone_nvtx_histo_e = (TH1D*) Nt_nvtx_histo_e->Clone("rate_cone_nvtx_histo_e");
  TH1D *rate_cone_nvtx_histo_mu = (TH1D*) Nt_nvtx_histo_mu->Clone("rate_cone_nvtx_histo_mu");
  rate_cone_nvtx_histo_e->Divide(rate_cone_nvtx_histo_e,Nl_cone_nvtx_histo_e);
  rate_cone_nvtx_histo_mu->Divide(rate_cone_nvtx_histo_mu,Nl_cone_nvtx_histo_mu);

  rate_histo_e->Divide(rate_histo_e,Nl_histo_e,1,1,"B");
  rate_histo_mu->Divide(rate_histo_mu,Nl_histo_mu,1,1,"B");
  rate_cone_histo_e->Divide(rate_cone_histo_e,Nl_cone_histo_e,1,1,"B");
  rate_cone_histo_mu->Divide(rate_cone_histo_mu,Nl_cone_histo_mu,1,1,"B");
  rate_jet_histo_e->Divide(rate_jet_histo_e,Nl_jet_histo_e,1,1,"B");
  rate_jet_histo_mu->Divide(rate_jet_histo_mu,Nl_jet_histo_mu,1,1,"B");
  rate_jet_highpt_histo_e->Divide(rate_jet_highpt_histo_e,Nl_jet_highpt_histo_e,1,1,"B");
  rate_jet_highpt_histo_mu->Divide(rate_jet_highpt_histo_mu,Nl_jet_highpt_histo_mu,1,1,"B");
  rate_jet_lowpt_histo_e->Divide(rate_jet_lowpt_histo_e,Nl_jet_lowpt_histo_e,1,1,"B");
  rate_jet_lowpt_histo_mu->Divide(rate_jet_lowpt_histo_mu,Nl_jet_lowpt_histo_mu,1,1,"B");
  total_BR_histo_e->Add(NnotBs_BR_histo_e);
  total_BR_histo_mu->Add(NnotBs_BR_histo_mu);
  NBs_BR_histo_e->Divide(NBs_BR_histo_e, total_BR_histo_e,1,1,"B");
  NBs_BR_histo_mu->Divide(NBs_BR_histo_mu, total_BR_histo_mu,1,1,"B");

  rate_histo_e->GetXaxis()->SetTitle("pT (GeV)");
  rate_histo_e->GetYaxis()->SetTitle("eta");
  rate_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_histo_e->SetTitle("Fake Rate vs Pt, Eta (electrons)");
  rate_histo_mu->GetXaxis()->SetTitle("pT (GeV)");
  rate_histo_mu->GetYaxis()->SetTitle("eta");
  rate_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_histo_mu->SetTitle("Fake Rate vs Pt, Eta (muons)");
  rate_cone_histo_e->GetXaxis()->SetTitle("corrected pT (GeV)");
  rate_cone_histo_e->GetYaxis()->SetTitle("eta");
  rate_cone_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_cone_histo_e->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (electrons)");
  rate_cone_histo_mu->GetXaxis()->SetTitle("corrected pT (GeV)");
  rate_cone_histo_mu->GetYaxis()->SetTitle("eta");
  rate_cone_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_cone_histo_mu->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (muons)");
  rate_jet_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)");
  rate_jet_histo_e->GetYaxis()->SetTitle("eta");
  rate_jet_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
  rate_jet_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)");
  rate_jet_histo_mu->GetYaxis()->SetTitle("eta");
  rate_jet_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
  rate_jet_highpt_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)");
  rate_jet_highpt_histo_e->GetYaxis()->SetTitle("eta");
  rate_jet_highpt_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_highpt_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
  rate_jet_highpt_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)");
  rate_jet_highpt_histo_mu->GetYaxis()->SetTitle("eta");
  rate_jet_highpt_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_highpt_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
  rate_jet_lowpt_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)");
  rate_jet_lowpt_histo_e->GetYaxis()->SetTitle("eta");
  rate_jet_lowpt_histo_e->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_lowpt_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
  rate_jet_lowpt_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)");
  rate_jet_lowpt_histo_mu->GetYaxis()->SetTitle("eta");
  rate_jet_lowpt_histo_mu->GetZaxis()->SetRangeUser(0,.5);
  rate_jet_lowpt_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
  NBs_BR_histo_e->GetXaxis()->SetTitle("Nbjets");
  NBs_BR_histo_e->GetYaxis()->SetTitle("Abundance");
  NBs_BR_histo_e->GetYaxis()->SetRangeUser(0., 1.);
  NBs_BR_histo_e->SetTitle("B Abundance vs Nbtags (electrons)");
  NBs_BR_histo_mu->GetXaxis()->SetTitle("Nbjets");
  NBs_BR_histo_mu->GetYaxis()->SetTitle("Abundance");
  NBs_BR_histo_mu->GetYaxis()->SetRangeUser(0., 1.);
  NBs_BR_histo_mu->SetTitle("B Abundance vs Nbtags (muons)");
  pTrelvsIso_histo_el->GetXaxis()->SetTitle("Iso");
  pTrelvsIso_histo_el->GetYaxis()->SetTitle("pTrel");
  pTrelvsIso_histo_mu->GetXaxis()->SetTitle("Iso");
  pTrelvsIso_histo_mu->GetYaxis()->SetTitle("pTrel");
  njets40_histo->GetXaxis()->SetTitle("Njets");
  njets40_histo->GetYaxis()->SetTitle("Events");
  njets40_histo->SetTitle("Njets with pT > 40 GeV");
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("1.3f");

  TCanvas *c11=new TCanvas("c11","B Abundance vs Nbjets (electrons)",800,800);
  c11->cd();
  NBs_BR_histo_e->Draw("histE");
  TCanvas *c12=new TCanvas("c12","B Abundance vs Nbjets (muons)",800,800);
  c12->cd();
  NBs_BR_histo_mu->Draw("histE");
  TCanvas *c13=new TCanvas("c13","Njets with pT > 40 GeV",800,800);
  c13->cd();
  njets40_histo->Draw("histE");

  //---save histos-------//
  TFile *OutputFile = new TFile(outfile,"recreate");
  OutputFile->cd();
  Nl_histo_e->Write();
  Nl_histo_mu->Write();
  Nt_histo_e->Write();
  Nt_histo_mu->Write();
  rate_histo_e->Write();
  rate_histo_mu->Write();
  Nl_cone_histo_e->Write();
  Nl_cone_histo_mu->Write();
  rate_cone_histo_e->Write();
  rate_cone_histo_mu->Write();

  // NJA
  rate_cone_nvtx_histo_e->Write();
  rate_cone_nvtx_histo_mu->Write();

  Nl_jet_histo_e->Write();
  Nl_jet_histo_mu->Write();
  Nt_jet_histo_e->Write();
  Nt_jet_histo_mu->Write();
  rate_jet_histo_e->Write();
  rate_jet_histo_mu->Write();
  Nl_jet_highpt_histo_e->Write();
  Nl_jet_highpt_histo_mu->Write();
  Nt_jet_highpt_histo_e->Write();
  Nt_jet_highpt_histo_mu->Write();
  rate_jet_highpt_histo_e->Write();
  rate_jet_highpt_histo_mu->Write();
  Nl_jet_lowpt_histo_e->Write();
  Nl_jet_lowpt_histo_mu->Write();
  Nt_jet_lowpt_histo_e->Write();
  Nt_jet_lowpt_histo_mu->Write();
  rate_jet_lowpt_histo_e->Write();
  rate_jet_lowpt_histo_mu->Write();
  NBs_BR_histo_e->Write();
  NBs_BR_histo_mu->Write();
  pTrelvsIso_histo_el->Write();
  pTrelvsIso_histo_mu->Write();
  pTrel_histo_el->Write();
  pTrel_histo_mu->Write();
  histo_ht->Write();
  histo_met->Write();
  histo_met_all->Write();
  histo_met_all_el->Write();
  histo_met_all_mu->Write();
  histo_met_lm->Write();
  histo_met_lm_el->Write();
  histo_met_lm_mu->Write();
  histo_met_cr->Write();
  histo_met_cr_el->Write();
  histo_met_cr_mu->Write();
  histo_mt->Write();
  histo_mt_all->Write();
  histo_mt_all_el->Write();
  histo_mt_all_mu->Write();
  histo_mt_lm->Write();
  histo_mt_lm_el->Write();
  histo_mt_lm_mu->Write();
  histo_mt_cr->Write();
  histo_mt_cr_el->Write();
  histo_mt_cr_mu->Write();
  histo_pt_mu34->Write();
  histo_pt_mu24->Write();
  histo_pt_mu17->Write();
  histo_pt_mu8->Write();
  histo_pt_mu->Write();
  histo_pt_el34->Write();
  histo_pt_el24->Write();
  histo_pt_el17->Write();
  histo_pt_el12->Write();
  histo_pt_el8->Write();
  histo_pt_el->Write();
  njets40_histo->Write();

}
