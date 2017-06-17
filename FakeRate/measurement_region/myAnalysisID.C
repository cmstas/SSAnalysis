//	#include "LeptonTree.cc"
#include "../../commonUtils.h"
//	using namespace lepton_tree;


//=====================================================================================================
// The following function defines the ID being used to measure the fake rate.
// The function returns true, if the given lepton entry in the LeptonTree is good.
// The function returns false, if the given lepton entry in the LeptonTree is to be skipped.
// This function will be passed on to the ScanChain.C, which is our LeptonTree looper, as an argment.
// The doAll.C is where the ScanChain.C is called for each TTree and this function is passed.
bool myAnalysisID(bool& passId, bool& passFO)
{
  bool useLooseEMVA = true;
  bool useIsoTrigs = true;
  bool debug = false;
  passId = passes_SS_tight_v5();
  passFO = passes_SS_fo_v5();
  if (useLooseEMVA && abs(id())==11) {
    bool isEB = true;
    if ( fabs(etaSC())>1.479 ) isEB = false;
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
      if (debug) cout << "check iso FO" << endl;
      if (!passIsolatedFO(id(),etaSC(),mva_25ns(),p4().pt())) return false;
      float ePFIso = ecalPFClusterIso()/p4().pt();
      float hPFIso = hcalPFClusterIso()/p4().pt();
      float trkIso = tkIso()/p4().pt();
      float cut_ePFIso = 0.45;
      float cut_hPFIso = 0.25;
      float cut_trkIso  = 0.2;
      passHltCuts = passHltCuts && ePFIso<cut_ePFIso && hPFIso<cut_hPFIso && trkIso<cut_trkIso;
    }
    passFO = passHltCuts && passes_SS_fo_looseMVA_v5();
  }
  return true;
}
