#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TF1.h"
#include "Math/VectorUtil.h" 
#include "CORE/CMS3.h"
#include "Tools/utils.h"
#include "Tools/MT2/MT2.h"
#include "CORE/SSSelections.h"
#include "CORE/ElectronSelections.h"
#include "CORE/IsolationTools.h"
#include "TROOT.h"
#include <vector>

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
typedef vector<pair<const LorentzVector *, double> > jets_with_corr_t;

using namespace std;

//Classes
class babyMaker {

  public:
    babyMaker(bool debug = 0) {
      path = ".";
      verbose = debug;
      evt_cut = 0;
    }
    void MakeBabyNtuple(const char* output_name);
    void InitBabyNtuple();
    void CloseBabyNtuple () { BabyFile->cd(); BabyTree->Write(); BabyFile->Close(); }
    int ProcessBaby(IsolationMethods isoCase, string filename_in);

  protected:
    TFile* BabyFile;
    TTree* BabyTree;

  private:

    //Switches
    TString path;
    bool verbose;
    unsigned int evt_cut;

    //MET
    float met;
    float metPhi;
 
    //Meta Variables
    int event;
    int lumi;
    int run;
    bool is_real_data;
    float scale1fb;     
    float xsec;         
    float kfactor;      
    TString filename;

    //Filters
    bool filt_csc;
    bool filt_hbhe;
    bool filt_hcallaser;
    bool filt_ecaltp;
    bool filt_trkfail;
    bool filt_eebadsc;

    //Gen MET 
    float gen_met;      
    float gen_met_phi;  

    //Jets
    int njets;
    float ht;
    vector <LorentzVector> jets;
    vector <float> jets_disc;

    //Hyp Class -- in this order
       //3 for num-num SS leptons
       //2 for num-den SS leptons
       //1 for den-den SS leptons
       //4 for num-num OS leptons
       //0 otherwise (not saved)
    int hyp_class;

    //Leptons
    LorentzVector lep1_p4;
    LorentzVector lep2_p4;
    LorentzVector dilep_p4;
    int lep1_id;
    int lep2_id;
    int lep1_idx;
    int lep2_idx;
    int hyp_type; 

    //Lepton Mother (old isFromW function)
    int lep1_motherID;
    int lep2_motherID;

    //Lepton Truth ID
      //From src/MatchUtilities/matchCandToGen function
      //delta-R matches to nearest particle other than neutrino or LSP
      //must be within 0.2
    int lep1_mc_id;
    int lep2_mc_id;

    //b-tags
    vector <LorentzVector> btags;
    vector <float> btags_disc;
    int nbtags;

    //Scale factors (from 8 TeV, outdated)
    float sf_dilepTrig_hpt; 
    float sf_dilepTrig_lpt; 
    float sf_dilepTrig_vlpt; 
    float sf_dilep_eff;

    //mT for both leptons, mt2
    float mt;
    float mt_l2;
    float mt2;
    float mtmin;

    //SUSY sparms
    float mLSP;
    float mGluino;
    float mSbottom;
    float mChargino;
 
    //Look at gen-level particles and choose favorite hypothesis
    //Limited usefulness as-is; probably not smart enough to reject non-prompt leptons
    int lep1_id_gen;
    int lep2_id_gen;
    LorentzVector lep1_p4_gen;
    LorentzVector lep2_p4_gen;

    //Third lepton -- quality is 2 for good, 1 for fakeable, 0 for veto
    int lep3_id;
    int lep3_idx;
    int lep3_quality;
    LorentzVector lep3_p4;

    //Isolation
    float lep1_iso;
    float lep2_iso;

    //Gen Leptons
    vector <LorentzVector> genps_p4;
    vector <int> genps_id;
    vector <int> genps_id_mother;
    vector <int> genps_status;
    vector <int> genps_id_grandma;

    //Leptons pass numerator ID
    bool lep1_passes_id;
    bool lep2_passes_id;

    //Imparct parameter
    float lep1_dxyPV;
    float lep2_dxyPV;
    float lep1_d0_err;
    float lep2_d0_err;
    float lep1_dZ;
    float lep2_dZ;
    float lep1_ip3d;
    float lep1_ip3d_err;
    float lep2_ip3d;
    float lep2_ip3d_err;

    //nVetoElectrons passing certain pT cuts
    int nVetoElectrons7;
    int nVetoElectrons10;
    int nVetoElectrons25;
    int nVetoMuons5;
    int nVetoMuons10;
    int nVetoMuons25;

    //PtRel, v0 and v1 (v1 is lepton-subtracted)
    float lep1_ptrel_v0;
    float lep1_ptrel_v1;
    float lep2_ptrel_v0;
    float lep2_ptrel_v1;

    //MiniIso
    float lep1_miniIso;
    float lep2_miniIso;

    //Closest Jet
    LorentzVector jet_close_lep1;
    LorentzVector jet_close_lep2;

    //Electron ID variables
    vector <int>   eleID_kfhits;
    vector <float> eleID_oldsigmaietaieta;
    vector <float> eleID_oldsigmaiphiiphi;
    vector <float> eleID_oldcircularity;
    vector <float> eleID_oldr9;
    vector <float> eleID_scletawidth;
    vector <float> eleID_sclphiwidth;
    vector <float> eleID_he;
    vector <float> eleID_psEoverEraw;
    vector <float> eleID_kfchi2;
    vector <float> eleID_chi2_hits;
    vector <float> eleID_fbrem;
    vector <float> eleID_ep;
    vector <float> eleID_eelepout;
    vector <float> eleID_IoEmIop;
    vector <float> eleID_deltaetain;
    vector <float> eleID_deltaphiin;
    vector <float> eleID_deltaetaseed;
    vector <float> eleID_pT;
    vector <bool>  eleID_isbarrel;
    vector <bool>  eleID_isendcap;
    vector <float> eleID_scl_eta;

    //Muon ID variables
    vector <float> muID_dzPV;      
    vector <float> muID_ptSig;     
    vector <float> muID_ip3dSig;   
    vector <float> muID_medMuonPOG;
    vector <float> muID_pt;        
    vector <float> muID_eta;       

    //InSituFR
    bool lep1_isGoodLeg;
    bool lep2_isGoodLeg;
    bool lep1_isFakeLeg;
    bool lep2_isFakeLeg;
    bool truth_inSituFR;
    bool lep1_multiIso;
    bool lep2_multiIso;
    float lep1_sip; 
    float lep2_sip; 
    LorentzVector lep1_closeJet;
    LorentzVector lep2_closeJet;
};
