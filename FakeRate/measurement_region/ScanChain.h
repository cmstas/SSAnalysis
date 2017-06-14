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

// Utilities
float computePtRel(LorentzVector lepp4, LorentzVector jetp4, bool subtractLep=true);
float getPt(float pt, bool extrPtRel=false);
float getEta(float eta, float ht, bool extrPtRel=false);
double calculateMt(const LorentzVector p4, double met, double met_phi);

// Variables/Functions related to configuration
bool useRelIso = false;
bool useLooseEMVA = false;
bool usePtRatioCor = false;
bool doBonly = false;
bool doConly = false;
bool doLightonly = false;
bool useIsoTrigs = false;
float anyPt = false;
bool doJEC = false;
bool debug = false;
bool applyDataVtxWeight = false;
int nptbins = 5;
int netabins = 3;
float ptbins[6] = {10., 15., 25., 35., 50., 70.};
float etabins_mu[4] = {0., 1.2, 2.1, 2.4};
float etabins_el[4] = {0., 0.8, 1.479, 2.5};
HLTEfficiency HLTEff("../../hlt/HLT_Efficiencies_7p65fb_2016.root");
void setGlobalConfig(TString option);
