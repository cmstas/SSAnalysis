#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TF1.h"
#include "Math/VectorUtil.h" 
#include "CORE/CMS3.h"
#include "tools.h"
#include "Tools/MT2/MT2.h"
#include "CORE/SSSelections.h"
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
      evt_cut = 13398;
    }
    void MakeBabyNtuple(const char* output_name, bool usePtRel);
    void InitBabyNtuple();
    void CloseBabyNtuple () { BabyFile->cd();BabyTree->Write();BabyFile->Close();}
    int ProcessBaby(bool usePtRel);

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

    //nGoodElectrons passing certain pT cuts
    int nGoodElectrons7;
    int nGoodElectrons10;
    int nGoodElectrons25;
    int nGoodMuons5;
    int nGoodMuons10;
    int nGoodMuons25;

    //PtRel, v0 and v1 (v1 is lepton-subtracted)
    float lep1_ptrel_v0;
    float lep1_ptrel_v1;
    float lep2_ptrel_v0;
    float lep2_ptrel_v1;
};
