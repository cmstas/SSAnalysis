#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TF1.h"
#include "Math/VectorUtil.h"
#include <vector>

#include "include/CMS2.h"
#include "include/selections.h"
#include "include/structAG.h"
//#include "MT2AG.h"
#include "looper.h" 
#include "include/fromCore.h"
#include "include/structAG.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
typedef vector<pair<const LorentzVector *, double> > jets_with_corr_t;

using namespace std;

//Switches
char* path = "../fake_rate_output/V00-00-03";
bool verbose = 0;
unsigned int evt_cut = 74994186;

//Classes
class babyMaker {

  public:
    void MakeBabyNtuple(const char* output_name);
    void InitBabyNtuple();
    void InitMuonBranches(); //init. Muon variables only.
    void InitElectronBranches(); //init. Electron variables only
    int looper(TChain* chain, char* output_name, int nEvents = -1, string signal_in = "");

  protected:
    TFile* BabyFile;
    TTree* BabyTree;

  private:

    //for tree
    float met;
    float metPhi;
    int event;
    int lumi;
    int run;
    bool filt_csc;
    bool filt_hbhe;
    bool filt_hcallaser;
    bool filt_ecaltp;
    bool filt_trkfail;
    bool filt_eebadsc;
    bool is_real_data;
    float scale1fb;     
    float xsec;         
    float kfactor;      
    float gen_met;      
    float gen_met_phi;  
    float njets;  
    float ht;  
    vector <LorentzVector> jets;
    vector <float> jets_disc; 
    TString sample;
    int nFOs;
  //-------------------//
  //------MINE---------//
      //---both--//
  LorentzVector p4;
  LorentzVector mc_p4;
  LorentzVector mc_motherp4;
  int id; 
  int idx;
  float d0; //dxy?
  float dZ;
  float d0_err;
  int motherID;
  int mc_id;
  float iso; //RelIso03 (EA?)
  bool passes_id;
  bool FO;
  float ip3d;
  float ip3derr;
  int type;
  float mt;
      //---els---//
  float el_sigmaIEtaIEta_full5x5;
  float el_etaSC;
  float el_dEtaIn;
  float el_dPhiIn;
  float el_hOverE;
  float el_ecalEnergy;
  float el_eOverPIn;
  bool el_conv_vtx_flag;
  int el_exp_innerlayers;
  int el_charge;
  int el_sccharge;
  int el_ckf_charge;
  int el_trk_charge;
  bool el_threeChargeAgree;
      //---mus---//
  int mu_pid_PFMuon;
  float mu_gfit_chi2;
  int mu_gfit_validSTAHits;
  int mu_numberOfMatchedStations;
  int mu_validPixelHits;
  int mu_nlayers;
  //---------------------
    
};

//Main functions
void babyMaker::MakeBabyNtuple(const char* output_name){

  //Create Baby
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  rootdir->cd();
  BabyFile = new TFile(Form("%s/%s", path, output_name), "RECREATE");
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
  BabyTree->Branch("ht", &ht);
  BabyTree->Branch("jets", &jets);
  BabyTree->Branch("jets_disc", &jets_disc);
  BabyTree->Branch("sample", &sample);
  BabyTree->Branch("nFOs", &nFOs);
  //--------------------MINE----------------------------
         //---both--//
  BabyTree->Branch("p4", &p4);
  BabyTree->Branch("mc_p4", &mc_p4);
  BabyTree->Branch("mc_motherp4", &mc_motherp4);
  BabyTree->Branch("id", &id);
  BabyTree->Branch("idx", &idx);
  BabyTree->Branch("d0", &d0);
  BabyTree->Branch("dZ", &dZ);
  BabyTree->Branch("d0_err", &d0_err);
  BabyTree->Branch("motherID", &motherID);
  BabyTree->Branch("mc_id", &mc_id);
  BabyTree->Branch("iso", &iso);
  BabyTree->Branch("passes_id", &passes_id);
  BabyTree->Branch("FO", &FO);
  BabyTree->Branch("ip3d", &ip3d);
  BabyTree->Branch("ip3derr", &ip3derr);
  BabyTree->Branch("type", &type);
  BabyTree->Branch("mt", &mt);
          //---els---//
  BabyTree->Branch("el_sigmaIEtaIEta_full5x5", &el_sigmaIEtaIEta_full5x5);
  BabyTree->Branch("el_etaSC", &el_etaSC);
  BabyTree->Branch("el_dEtaIn", &el_dEtaIn);
  BabyTree->Branch("el_dPhiIn", &el_dPhiIn);
  BabyTree->Branch("el_hOverE", &el_hOverE);
  BabyTree->Branch("el_ecalEnergy", &el_ecalEnergy);
  BabyTree->Branch("el_eOverPIn", &el_eOverPIn);
  BabyTree->Branch("el_conv_vtx_flag", &el_conv_vtx_flag);
  BabyTree->Branch("el_exp_innerlayers", &el_exp_innerlayers);
  BabyTree->Branch("el_charge", &el_charge);
  BabyTree->Branch("el_sccharge", &el_sccharge);
  BabyTree->Branch("el_ckf_charge", &el_ckf_charge);
  BabyTree->Branch("el_threeChargeAgree", &el_threeChargeAgree);
         //---mus---//
  BabyTree->Branch("mu_pid_PFMuon", &mu_pid_PFMuon);
  BabyTree->Branch("mu_gfit_chi2", &mu_gfit_chi2);
  BabyTree->Branch("mu_gfit_validSTAHits", &mu_gfit_validSTAHits);
  BabyTree->Branch("mu_numberOfMatchedStations", &mu_numberOfMatchedStations);
  BabyTree->Branch("mu_validPixelHits", &mu_validPixelHits);
  BabyTree->Branch("mu_nlayers", &mu_nlayers);
  //----------------------------------------------------
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
    ht = -1;
    jets.clear();
	jets_disc.clear();
    sample = "";
    nFOs = -1;
	//--------MINE------------
         //---both--//
	p4 = LorentzVector(0,0,0,0); 
	mc_p4 = LorentzVector(0,0,0,0);
	mc_motherp4 = LorentzVector(0,0,0,0);
	id = -1; 
	idx = -1;
	d0 = -1;
	dZ = -1;
	d0_err = -1;
	motherID = -1;
    mc_id = -1;
	iso = -1;
	passes_id = 0;
	FO = 0;
	ip3d = -1;
	ip3derr = -1;
	type = -1;
    mt = -1;
         //---els---//
	el_sigmaIEtaIEta_full5x5 = -1;//below
	el_etaSC = -1;
	el_dEtaIn = -1;
	el_dPhiIn = -1;
	el_hOverE = -1;
	el_ecalEnergy = -1;
	el_eOverPIn = -1;
	el_conv_vtx_flag = 0;
	el_exp_innerlayers = -1;
	el_charge = -1;
	el_sccharge = -1;
	el_ckf_charge = -1;
	el_trk_charge = -1;
	el_threeChargeAgree = 0;
         //---mus---//
	mu_pid_PFMuon = -1;
	mu_gfit_chi2 = -1;
	mu_gfit_validSTAHits = -1;
	mu_numberOfMatchedStations = -1;
	mu_validPixelHits = -1;
	mu_nlayers = -1;
	//------------------------
} 

void babyMaker::InitMuonBranches(){
  //---both---//
    p4 = LorentzVector(0,0,0,0); 
	mc_p4 = LorentzVector(0,0,0,0);
	mc_motherp4 = LorentzVector(0,0,0,0);
	id = -1; 
    idx = -1;
	d0 = -1;
    dZ = -1;
	d0_err = -1;
	motherID = -1;
	mc_id = -1;
	iso = -1;
	passes_id = 0;
	FO = 0;
	type = -1;
	ip3d = -1;
	ip3derr = -1;
    mt = -1;
	//---mus---//
	mu_pid_PFMuon = -1;
	mu_gfit_chi2 = -1;
	mu_gfit_validSTAHits = -1;
	mu_numberOfMatchedStations = -1;
	mu_validPixelHits = -1;
	mu_nlayers = -1;
	//------------------------
}

void babyMaker::InitElectronBranches(){
  //---both--//
    p4 = LorentzVector(0,0,0,0);
	mc_p4 = LorentzVector(0,0,0,0);
	mc_motherp4 = LorentzVector(0,0,0,0);
	id = -1; 
	idx = -1;
	d0 = -1;
	dZ = -1;
	d0_err = -1;
	motherID = -1;
    mc_id = -1;
	iso = -1;
	passes_id = 0;
	FO = 0;
	ip3d = -1;
	ip3derr = -1;
	type = -1;
    mt = -1;
	//---els---//
	el_sigmaIEtaIEta_full5x5 = -1;
	el_etaSC = -1;
	el_dEtaIn = -1;
	el_dPhiIn = -1;
	el_hOverE = -1;
	el_ecalEnergy = -1;
	el_eOverPIn = -1;
	el_conv_vtx_flag = 0;
	el_exp_innerlayers = -1;
	el_charge = -1;
	el_sccharge = -1;
	el_ckf_charge = -1;
	el_trk_charge = -1;
	el_threeChargeAgree = 0;
}

template <typename T> int sgn(T val){
    return (T(0) < val) - (val < T(0));
}

double calculateMt(const LorentzVector p4, double met, double met_phi){  //<--MT, MET, MET_PHI ARE ALL FLOATS!!!
  float phi1 = p4.Phi();
  float phi2 = met_phi;
  float Et1  = p4.Et();
  float Et2  = met;
  return sqrt(2*Et1*Et2*(1.0 - cos(phi1-phi2)));
}

int lepMotherID(Lep lep){
  if (tas::evt_isRealData()) return 1;
  else if (isFromZ(lep) || isFromW(lep)){
    if (sgn(lep.pdgId()) == sgn(lep.mc_id())) return 1;
    else return 2;
  }
  else if (isFromB(lep)) return -1;
  else if (isFromC(lep)) return -2;
  return 0;
}

float leptonD0(const int id, const int idx){
    if (abs(id)==13) return tas::mus_dxyPV().at(idx); 
    else if (abs(id)==11) return tas::els_dxyPV().at(idx);   
    return -999999.0;
}

float leptonD0err(const int id, const int idx){
    if (abs(id)==13) return tas::mus_d0Err().at(idx); 
    else if (abs(id)==11) return tas::els_d0Err().at(idx);   
    return -999999.0;
}

float leptonDZ(const int id, const int idx){
    if (abs(id) == 11) return tas::els_dzPV().at(idx);
    if (abs(id) == 13) return tas::mus_dzPV().at(idx);
    return -999999.0;
}

//Main function
int babyMaker::looper(TChain* chain, char* output_name, int nEvents, string signal_in){

  //Print warning!
  cout << "Careful!! Path is " << path << endl;

 //Make Baby Ntuple  
  MakeBabyNtuple( Form("%s.root", output_name) );
  
  //Initialize Baby Ntuple
  InitBabyNtuple();

  //Set up loop over chain
  unsigned int nEventsDone = 0;
  unsigned int nEventsToDo = chain->GetEntries();
  if( nEvents >= 0 ) nEventsToDo = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
 
  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) { 

    // Get File Content
    if(nEventsDone >= nEventsToDo) continue;
    TFile *file = new TFile( currentFile->GetTitle() );
    TTree *tree = (TTree*)file->Get("Events");
    cms2.Init(tree);
  
    // Loop over Events in current file
    unsigned int nEventsTree = tree->GetEntriesFast();
    for(unsigned int evt = 0; evt < nEventsTree; evt++){
	  
	  //cout<<"\nEvent loop again"<<endl;	  

      // Get Event Content
      if(nEventsDone >= nEventsToDo) continue;   
      cms2.GetEntry(evt);
      nEventsDone++;

      //Initialize variables
      InitBabyNtuple();
     
      //Local variables
      bool isData = tas::evt_isRealData();
        
      // Progress
      CMS2::progress(nEventsDone, nEventsToDo);

      //Debug mode
      if (verbose && tas::evt_event() != evt_cut) continue;
      if (verbose) cout << "file name is " << file->GetName() << endl;

      // //Preliminary stuff
	  if (tas::mus_dxyPV().size() != tas::mus_dzPV().size()) continue;  //idk what this is

      //Fill Easy Variables
      met = cms2.evt_pfmet();
      metPhi = cms2.evt_pfmetPhi();
      event = tas::evt_event();
      lumi = tas::evt_lumiBlock();
      run = tas::evt_run();
      is_real_data = tas::evt_isRealData();
      xsec = tas::evt_xsec_incl();
      kfactor = tas::evt_kfactor();
      gen_met = tas::gen_met();
      gen_met_phi = tas::gen_metPhi();
      sample = Form("%s", file->GetName());

      //Fill data vs. mc variables
      filt_csc = is_real_data ? tas::evt_cscTightHaloId() : 1;
      filt_hbhe = is_real_data ? (tas::evt_hbheFilter() && tas::hcalnoise_isolatedNoiseSumE() < 50.0 && tas::hcalnoise_isolatedNoiseSumEt() < 25.0 && tas::hcalnoise_numIsolatedNoiseChannels() < 10) : 1;
      filt_hcallaser = is_real_data ? tas::filt_hcalLaser() : 1;
      filt_ecaltp = is_real_data ? tas::filt_ecalTP() : 1;
      filt_trkfail = is_real_data ? tas::filt_trackingFailure() : 1;
      filt_eebadsc = is_real_data ? tas::filt_eeBadSc() : 1;
      scale1fb = is_real_data ? 1 : tas::evt_scale1fb();
	  
	  //-------------------------------------------------------------------------------------------------------------
	  //Determine and save jet variables
      ht = 0;
      for (unsigned int i = 0; i < tas::pfjets_p4().size(); i++)
	  	{
	  	  LorentzVector jet = tas::pfjets_p4().at(i);

	  	  //Kinematic jet cuts
	  	  if (jet.pt() < 25) continue;   //<------WHAT CUTS DO WE WANT???
	  	  //if (fabs(jet.eta()) > 2.4) continue; //want jets of all eta's for fake rate

	  	  //Verbose
	  	  if (verbose) cout << "Possible jet with pT: " << jet.pt() << endl;
      
	  	  //Require loose jet ID
	  	  if (!isLoosePFJet(i)) continue;
      
	  	  //Jet cleaning -- electrons
	  	  bool jetIsLep = false;
	  	  for (unsigned int eidx = 0; eidx < tas::els_p4().size(); eidx++){
	  		LorentzVector electron = tas::els_p4().at(eidx);
	  		if (electron.pt() < 7) continue;
	  		if (!isGoodVetoElectron(eidx)) continue;
	  		if (ROOT::Math::VectorUtil::DeltaR(jet, electron) > 0.4) continue;
	  		jetIsLep = true;
	  	  }
	  	  if (jetIsLep == true) continue;

	  	  //Jet cleaning -- muons
	  	  for (unsigned int muidx = 0; muidx < tas::mus_p4().size(); muidx++){
	  		LorentzVector muon = tas::mus_p4().at(muidx);
	  		if (muon.pt() < 5) continue;
	  		if (!isGoodVetoMuon(muidx)) continue;
	  		if (ROOT::Math::VectorUtil::DeltaR(jet, muon) > 0.4) continue;
	  		jetIsLep = true;
	  	  }
	  	  if (jetIsLep == true) continue;

	  	  //Save jets that make it this far
	  	  jets.push_back(jet);
	  	  ht += jet.pt();
		  float disc = tas::pfjets_combinedInclusiveSecondaryVertexV2BJetTag().at(i);  //BRANCH DOESNT EXIST in old samples.
		  jets_disc.push_back(disc);
	
		}
      njets = jets.size();

      //Verbose for jets
      if (verbose){
        cout << "njets: " << njets << endl;
        for (unsigned int i = 0; i < jets.size(); i++) cout << i << " " << jets[i].pt() << " " << jets[i].eta() << endl;
      } 
	  //-----------------------------------------------------------------------------------------------------------------------

	  int count = 0;

	  for(int j = 0; j < tas::mus_p4().size(); j++)
	  	{
	  	  if(isFakableMuon(j))
	  		{count++;}
	  	}
	  for(int j = 0; j < tas::els_p4().size(); j++)
	  	{
	  	  if(isFakableElectron(j))
	  		{count++;}
	  	}
	  nFOs = count;

	  //Muon Loop
	  //cout<<"\nBegin Muon looping"<<endl;	  
	  for(unsigned int i=0; i<tas::mus_p4().size(); i++)  //What RECO and GEN variables are needed?
		{	
		  if (!isGoodVetoMuon(i)) continue;  //"fix me" in selection.cc

		  p4 = tas::mus_p4().at(i); 
		  mc_p4 = tas::mus_mc_p4().at(i);
		  id = -13.0*tas::mus_charge().at(i);
		  idx = i;  
		  mc_id = tas::mus_mc_id().at(i);
		  d0 = leptonD0(id, i);
		  d0_err = leptonD0err(id, i);
		  dZ = leptonDZ(id, i);
		  iso = muRelIso03(i);
		  mu_pid_PFMuon = tas::mus_pid_PFMuon().at(i);
		  mu_gfit_chi2 = tas::mus_gfit_chi2().at(i);
		  mu_gfit_validSTAHits = tas::mus_gfit_validSTAHits().at(i);
		  mu_numberOfMatchedStations = tas::mus_numberOfMatchedStations().at(i);
		  mu_validPixelHits = tas::mus_validPixelHits().at(i);
		  mu_nlayers = tas::mus_nlayers().at(i);
		  ip3d = tas::mus_ip3d().at(i);
		  ip3derr = tas::mus_ip3derr().at(i);
		  type = tas::mus_type().at(i);
		  mt = calculateMt(p4, met, metPhi); 

		  passes_id = isGoodMuon(i);   //"fix me" in selection.cc.
		  FO = isFakableMuon(i);    //"fix me" in selection.cc

		  Lep mu_temp = Lep(id, i);
		  motherID = lepMotherID(mu_temp);
		  mc_motherp4 = tas::mus_mc_motherp4().at(i);
		  
		  BabyTree->Fill(); 
		  //InitMuonBranches(); //can put this here to be extra safe between muon interations
		                      //or can put outside loop to protect 1st el interation

		  // cout<<"\nEnd Muon loop"<<endl;
		} //close muon loop
	
	  //AFTER LAST MUON INTERATION, MUON VARIABLES STILL SET (NOT GARBAGE VALUES).
	  //SO THEY WILL BE FILLED WITH THE EACH ELECTRON FILL.  NEED TO STOP THIS.
	  //LEPTON VARIABLES IN COMMON SHOULD BE OVERWRITTEN.
	  InitMuonBranches();
	  
      //Electron Loop
	  //cout<<"\nBegin electron looping"<<endl;	  
	  for(unsigned int i=0; i<tas::els_p4().size(); i++)
	  	{
		  
		  if (!isGoodVetoElectron(i)) continue; //"fix me" in selection.cc. Use at()?

		  p4 = tas::els_p4().at(i);    
		  mc_p4 = tas::els_mc_p4().at(i);  
		  id = -11.0*tas::els_charge().at(i); 
		  idx = i;
		  mc_id = tas::els_mc_id().at(i);  
		  d0 = leptonD0(id, i);
		  d0_err = leptonD0err(id, i);
		  dZ = leptonDZ(id, i);
		  iso = eleRelIso03(i);
		  el_sigmaIEtaIEta_full5x5 = tas::els_sigmaIEtaIEta_full5x5().at(i);//new below
		  el_etaSC = tas::els_etaSC().at(i);
		  el_dEtaIn = tas::els_dEtaIn().at(i);
		  el_dPhiIn = tas::els_dPhiIn().at(i);
		  el_hOverE = tas::els_hOverE().at(i);
		  ip3d = tas::els_ip3d().at(i);
		  ip3derr = tas::els_ip3derr().at(i);
		  el_ecalEnergy = tas::els_ecalEnergy().at(i);
		  el_eOverPIn = tas::els_eOverPIn().at(i);
		  el_conv_vtx_flag = tas::els_conv_vtx_flag().at(i);
		  el_exp_innerlayers = tas::els_exp_innerlayers().at(i);
		  el_charge = tas::els_charge().at(i);    //check on this
		  el_sccharge = tas::els_sccharge().at(i);    //check on this
		  el_ckf_charge = tas::els_ckf_charge().at(i);    //check on this
		  el_trk_charge = tas::els_trk_charge().at(i);
		  el_threeChargeAgree = threeChargeAgree(i);
		  type = tas::els_type().at(i);
		  mt = calculateMt(p4, met, metPhi); 

		  passes_id = isGoodElectron(i);  //"fix me" in selection.cc
		  FO = isFakableElectron(i);  //"fix me" in selection.cc
 
		  Lep el_temp = Lep(id, i);
		  motherID = lepMotherID(el_temp);
		  mc_motherp4 = tas::els_mc_motherp4().at(i); 

		  BabyTree->Fill(); 
		  //InitElectronBranches();  //can put this here to be extra safe between el interation
	  
	  	  //cout<<"\nEnd electron loop"<<endl;
	  	} //close electron loop
	  
	  
    }//close event loop
    
    file->Close();
    delete file;
	
	cout<<"\nFile done"<<endl;
  }//close file loop
  
  cout<<"\nWriting file"<<endl;

  BabyFile->cd();
  BabyTree->Write();
  BabyFile->Close();


  return 0;  

}
