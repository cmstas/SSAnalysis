// Usage:
// > root -b doAll.C

// C++
#include <iostream>
#include <iomanip>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "Math/VectorUtil.h"
#include <unistd.h> //isatty

// lepfilter
#include "LeptonTree.cc"

#include "../../CORE/Tools/JetCorrector.cc"
#include "../../commonUtils.h"
#include "vtx_reweight.h"

using namespace std;
using namespace lepton_tree;

#ifdef __MAKECINT__
#pragma link C++ class ROOT::Math::PxPyPzE4D<float>+;
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >+;
#endif

//-------------------------------------------------------------------------------------------------
//
//
// Top level functions
//
//
//-------------------------------------------------------------------------------------------------
void beforeLoop();
void prepData();


//-------------------------------------------------------------------------------------------------
//
//
// Input variables (Saved as global variables)
//
//
//-------------------------------------------------------------------------------------------------
TChain* chain = 0;
TString outfile = "";
TString option = "";
bool fast = true;
int nEvents = -1;
void setInputArgumentVariables(
    TChain* chain,
    TString outfile,
    TString option="",
    bool fast=true,
    int nEvents=-1);


//-------------------------------------------------------------------------------------------------
//
//
// output histograms
//
//
//-------------------------------------------------------------------------------------------------

// The function that books the output histograms
void setOutputHistograms();

// Example Histograms
TDirectory *rootdir = 0;

// List of output histograms
TH2D *Nt_histo_e             = 0;
TH2D *Nl_histo_e             = 0;
TH2D *Nt_histo_mu            = 0;
TH2D *Nl_histo_mu            = 0;
TH2D *Nl_cone_histo_e        = 0;
TH2D *Nl_cone_histo_mu       = 0;
TH2D *Nt_jet_histo_e         = 0;
TH2D *Nt_jet_histo_mu        = 0;
TH2D *Nl_jet_histo_e         = 0;
TH2D *Nl_jet_histo_mu        = 0;
TH2D *Nt_jet_highpt_histo_e  = 0;
TH2D *Nt_jet_highpt_histo_mu = 0;
TH2D *Nl_jet_highpt_histo_e  = 0;
TH2D *Nl_jet_highpt_histo_mu = 0;
TH2D *Nt_jet_lowpt_histo_e   = 0;
TH2D *Nt_jet_lowpt_histo_mu  = 0;
TH2D *Nl_jet_lowpt_histo_e   = 0;
TH2D *Nl_jet_lowpt_histo_mu  = 0;

TH2D *pTrelvsIso_histo_el    = 0;
TH2D *pTrelvsIso_histo_mu    = 0;

TH1F *NBs_BR_histo_e         = 0;
TH1F *NBs_BR_histo_mu        = 0;
TH1F *NnotBs_BR_histo_e      = 0;
TH1F *NnotBs_BR_histo_mu     = 0;
TH1D *pTrel_histo_el         = 0;
TH1D *pTrel_histo_mu         = 0;
TH1F *histo_ht               = 0;
TH1F *histo_met              = 0;
TH1F *histo_met_all          = 0;
TH1F *histo_met_all_el       = 0;
TH1F *histo_met_all_mu       = 0;
TH1F *histo_met_lm           = 0;
TH1F *histo_met_lm_el        = 0;
TH1F *histo_met_lm_mu        = 0;
TH1F *histo_met_cr           = 0;
TH1F *histo_met_cr_el        = 0;
TH1F *histo_met_cr_mu        = 0;
TH1F *histo_mt               = 0;
TH1F *histo_mt_all           = 0;
TH1F *histo_mt_all_el        = 0;
TH1F *histo_mt_all_mu        = 0;
TH1F *histo_mt_lm            = 0;
TH1F *histo_mt_lm_el         = 0;
TH1F *histo_mt_lm_mu         = 0;
TH1F *histo_mt_cr            = 0;
TH1F *histo_mt_cr_el         = 0;
TH1F *histo_mt_cr_mu         = 0;
TH1F *histo_pt_mu            = 0;
TH1F *histo_pt_mu8           = 0;
TH1F *histo_pt_mu17          = 0;
TH1F *histo_pt_mu24          = 0;
TH1F *histo_pt_mu34          = 0;
TH1F *histo_pt_el            = 0;
TH1F *histo_pt_el8           = 0;
TH1F *histo_pt_el12          = 0;
TH1F *histo_pt_el17          = 0;
TH1F *histo_pt_el24          = 0;
TH1F *histo_pt_el34          = 0;
TH1F *njets40_histo          = 0;
TH1D *Nt_nvtx_histo_e        = 0;
TH1D *Nt_nvtx_histo_mu       = 0;
TH1D *Nl_cone_nvtx_histo_e   = 0;
TH1D *Nl_cone_nvtx_histo_mu  = 0;


//-------------------------------------------------------------------------------------------------
//
//
// Utilities
//
//
//-------------------------------------------------------------------------------------------------

float computePtRel(LorentzVector lepp4, LorentzVector jetp4, bool subtractLep=true);
float getPt(float pt, bool extrPtRel=false);
float getEta(float eta, float HT, bool extrPtRel=false);
double calculateMt(const LorentzVector p4, double met, double met_phi);
LorentzVector getJetP4(int i);
vector<LorentzVector> getJetP4s();
LorentzVector getCloseJetP4();
void correctMCTriggerEfficiency();

//-------------------------------------------------------------------------------------------------
//
//
// Variables/Functions related to configuration
//
//
//-------------------------------------------------------------------------------------------------

// The function that sets all the global configuration variables
void setGlobalConfig();

// Global configuration variables
bool  useRelIso          = false;
bool  useLooseEMVA       = false;
bool  usePtRatioCor      = false;
bool  doBonly            = false;
bool  doConly            = false;
bool  doLightonly        = false;
bool  useIsoTrigs        = false;
float anyPt              = false;
bool  doJEC              = false;
bool  debug              = false;
bool  applyDataVtxWeight = false;
int   nptbins            = 5;
int   netabins           = 3;
float ptbins[6]          = {10., 15., 25., 35., 50., 70.};
float etabins_mu[4]      = { 0., 1.2, 2.1  , 2.4};
float etabins_el[4]      = { 0., 0.8, 1.479, 2.5};

//-------------------------------------------------------------------------------------------------
//
//
// Prescale configurations
//
//
//-------------------------------------------------------------------------------------------------

// N.B.: At some point in the past, the miniAOD had the prescale variables filled in incorrectly.

// The function to set the prescale factors
void setTriggerPrescaleFactors();

// The global L1 prescale factors.
// L1 prescale "values" that comes out from miniAOD is unreliable at some point in time.
// So use getSF.C to recompute and fix by hand.
float e8i  = -999;
float e17i = -999;
float e8   = -999;
float e17  = -999;
float m8i  = -999;
float m17i = -999;
float m8   = -999;
float m17  = -999;

// So the prescale values for some triggers are recomputed using getSF.C
int prescale = -1; // This variable holds the recalculated prescale.
// The following function sets the prescale valuesj
void computePrescaleAndReWeightByPUwgt();


//-------------------------------------------------------------------------------------------------
//
//
// HLT efficiency tool
// Imported from "../../commonUtils.h"
//
//
//-------------------------------------------------------------------------------------------------

HLTEfficiency HLTEff("../../hlt/HLT_Efficiencies_7p65fb_2016.root");

//-------------------------------------------------------------------------------------------------
//
//
// JEC correctors
//
//
//-------------------------------------------------------------------------------------------------

// The function that sets the JEC corrector initialization
// The import of the tool is here #include "../../CORE/Tools/JetCorrector.cc"
void createJetCorrectors();

// JEC files -- 25 ns MC
std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1;
std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1L2L3;
// JEC files -- 25 ns DATA
std::vector<std::string> jetcorr_filenames_25ns_DATA_pfL1;
std::vector<std::string> jetcorr_filenames_25ns_DATA_pfL1L2L3;
// Jet Corrector instances for MC and DATA
FactorizedJetCorrector *jet_corrector_25ns_MC_pfL1;
FactorizedJetCorrector *jet_corrector_25ns_MC_pfL1L2L3;
FactorizedJetCorrector *jet_corrector_25ns_DATA_pfL1;
FactorizedJetCorrector *jet_corrector_25ns_DATA_pfL1L2L3;

// Place holders for the JetCorrector.
// Depending on which root files are being run over, they will be set accordingly.
FactorizedJetCorrector *jet_corrector_pfL1 = 0;
FactorizedJetCorrector *jet_corrector_pfL1MC = 0;
FactorizedJetCorrector *jet_corrector_pfL1L2L3 = 0;
std::vector <string> jetcorr_filenames_pfL1L2L3;

// The function that sets the place holder pointers based on the file name
void setJetCorrectorByFileName();

//-------------------------------------------------------------------------------------------------
//
//
// Some counter variables to get an "overview" value of the result
//
//
//-------------------------------------------------------------------------------------------------

// These variables are what you'd get if you didn't binned in pt or eta
// Just the total count.
// So after it's all done running you get a printout statement at the very end.

// e determination
float Nt           = 0.;  //# of tight leptons
float Nl           = 0.;  //# of loose leptons
float e            = 0.;   //rate              = Nt/(Nl)
float Nt_e         = 0.;
float Nl_e         = 0.;
float e_e          = 0.;
float Nt_mu        = 0.;
float Nl_mu        = 0.;
float e_mu         = 0.;
//----------------
float Bs_e         = 0.;
float notBs_e      = 0.;
float Bs_mu        = 0.;
float notBs_mu     = 0.;

//-------------------------------------------------------------------------------------------------
//
//
// Loop related variables
//
//
//-------------------------------------------------------------------------------------------------

// The function that sets the loop conditions (which file, how many events, etc.)
void setLoopCondition();

unsigned int nEventsTotal = 0;
unsigned int nEventsChain = 0;
TObjArray *listOfFiles = 0;
TFile *currentFile = 0;

// The function will access the lepton TTree from file and hook it up with lepton tree class
void setLeptonTree();
TFile *file = 0;
TTree *tree = 0;

// The function to check whether all events are processed
bool isAllEventsProcessed();

// Load event
void loadNextEvent(unsigned int eventnumber);

//-------------------------------------------------------------------------------------------------
//
//
// Boolean variables to indicate what kind of lepton baby we're looping over
//
//
//-------------------------------------------------------------------------------------------------

// Depending on which sample we're looping over we may want to do differen things.
// The function that parses various "boolean" flags based on the file name.

void setSampleInfoByFileName();

bool isSyncFile         = false;
bool isDataFromFileName = false;
bool isDoubleMuon       = false;
bool isQCD              = false;
bool isQCDMu            = false;
bool isQCDEl            = false;
bool isTTbar            = false;
bool doTrig             = false;
bool isEWK              = false;
bool isData             = false;
bool noMCMatch          = false;

void setMCMatchBoolVariable();

//-------------------------------------------------------------------------------------------------
//
//
// Print out statements
//
//
//-------------------------------------------------------------------------------------------------

void printWhichFileBeingLoopedOver();
void printWhichJECBeingUsed();
void printEventInfo();
void printCurrentLeptonInfo();
void printIthJetInfo(int ijet);
void printVariousVariables();

//-------------------------------------------------------------------------------------------------
//
//
// Some exit conditions
//
//
//-------------------------------------------------------------------------------------------------

// There are some "continue" statements if some criteria are ment.
bool isTtbarAndMotherIDPositive();
bool isEventWithDifferentFlavorLeptonInSameFlavorDileptonTrigger();
bool isBadEvents();
bool isBadWeightEvents();
bool isBadMCEvents();
bool isBadDataEvents();

//-------------------------------------------------------------------------------------------------
//
//
// Event Info variables
//
//
//-------------------------------------------------------------------------------------------------

// The function sets the lumi, puw, weight
void setEventInfoVariable();

float luminosity = 0;
float puw = 0;
float weight = 0;



//-------------------------------------------------------------------------------------------------
//
//
// Processing events
//
//
//-------------------------------------------------------------------------------------------------

/// important variables for the fake rate analysis
/// Jet releated variables
float HT            = 0.;
int   njets40       = 0;
int   nbtags        = 0;
bool  pass_jetptcut = false;
/// Lepton isolation variables
float ptrel      = 0;
float closejetpt = 0;
float relIso     = 0;
/// Tight ID = ID,
/// Fake-able object ID = FO
/// non-isolated Tight ID = ID,
/// non-isolated Fake-able object ID = FO
bool passId = false;
bool passFO =  false;
bool passId_noiso = false;
bool passFO_noiso = false;
/// MET and MT related variables
float evt_met    = -999;
float evt_metPhi = -999;
float evt_mt     = -999;


bool passIsolatedFOcut();
bool passJetPtCutOrHTCut();
bool passPtEtaSIPNFOLep();
bool passTriggerSelection();
bool passMETandMTCut();
bool passLooseObjectID();

void computeJetRelatedVariables(float&, int&, int&, bool&);
void computeLeptonIDFlags(bool&, bool&, bool&, bool&);
void computeLeptonIsolationVariables(float&, float&, float&);
void computeConePtCorrVariable(float&);
void computeMETandMT(float&, float&, float&);

void fillPassTightIDLeptonMTandMETplots();
void fillPassFOLeptonPtPlots();

void recomputePassFOByCorrectingPtRatio(bool&);
void recomputeLeptonIDFlagsForRelIso(bool&, bool&, float&);
