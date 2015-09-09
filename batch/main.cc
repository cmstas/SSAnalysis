#include "./CORE/CMS3.h"
#include "./CORE/SSSelections.h"
#include "TTree.h"
#include "TFile.h"
#include "helper_babymaker.h"
#include "base.h"
#include "CORE/Tools/goodrun.h"
#include "CORE/Tools/JetCorrector.h"

int main(int argc, char *argv[]){

  int which_in = 0;
  int file = 2;
  int ptrel_in = 4;
  int expt = 0;

  if (argc > 3) {
     which_in = atoi(argv[1]);
     file     = atoi(argv[2]);
     ptrel_in = atoi(argv[3]);
     expt     = atoi(argv[4]);
  } 
  else { 
    cout << "not enough arguments!" << endl;  
    return 0;
  }


  sample_t which = static_cast<sample_t>(which_in);
  IsolationMethods ptrel = static_cast<IsolationMethods>(ptrel_in); 

  babyMaker *mylooper = new babyMaker();

  //Path, filename, suffix
  string path = "";
  string name = ""; 
  string shortname = ""; 
  const char* filename = Form("merged_ntuple_%i.root", file);
  const char* suffix = file < 0 ? "" : Form("_%i", file);
  string tag = "V07-04-08";
  bool is25 = true;
  bool isData = false;


  //Info for each sample
  switch (which){
    case TTBAR:
      name="TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2";
      shortname="ttbar";
      tag = "V07-04-08";
      path = "/store/group/snt/run2_25ns/";
      break;
    case WZ3LNu:
      name="WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      shortname="wz3lnu";
      tag="V07-04-08";
      path="/store/group/snt/run2_25ns/";
      break;
    case TTW:
      name="TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="ttw";
      path = "/store/group/snt/run2_25ns/";
      break;
    case TTZL:
      name="TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="ttzl";
      path = "/store/group/snt/run2_25ns/";
      break;
    case WZ:
      name="WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="wz";
      path = "/store/group/snt/run2_25ns/";
      break;
    case DY_low:
      name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="dy_low";
      path = "/store/group/snt/run2_25ns/";
      break;
    case DY_high:
      name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3";
      tag = "V07-04-08";
      shortname="dy_high";
      path = "/store/group/snt/run2_25ns/";
      break;
    case WJets:
      name="WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="wjets";
      path = "/store/group/snt/run2_25ns/";
      break;
    case DataDoubleMuon:
      name="Run2015B_DoubleMuon_MINIAOD_PromptReco-v1/merged";
      tag = "V07-04-08";
      shortname="datadoublemuon";
      path = "/store/group/snt/run2_data/"; 
      isData = true;
      is25 = false;
      break;
    case DataDoubleEG:
      name="Run2015B_DoubleEG_MINIAOD_PromptReco-v1/merged";
      tag = "V07-04-08";
      shortname="datadoubleeg";
      path = "/store/group/snt/run2_data/"; 
      isData = true;
      is25 = false;
      break;
    case DATAMUONEG:
      name="Run2015B_MuonEG_MINIAOD_PromptReco-v1/merged";
      tag = "V07-04-08";
      shortname="datamuoneg";
      path = "/store/group/snt/run2_data/"; 
      is25 = false;
      isData = true;
      break;
    case DataDoubleMuon2:
      name="Run2015B_DoubleMuon_MINIAOD_17Jul2015-v1/merged";
      tag = "V07-04-08";
      shortname="datadoublemuon2";
      path = "/store/group/snt/run2_data/"; 
      isData = true;
      is25 = false;
      break;
    case DataDoubleEG2:
      name="Run2015B_DoubleEG_MINIAOD_17Jul2015-v1/merged";
      tag = "V07-04-08";
      shortname="datadoubleeg2";
      path = "/store/group/snt/run2_data/"; 
      isData = true;
      is25 = false;
      break;
    case DATAMUONEG2:
      name="Run2015B_MuonEG_MINIAOD_17Jul2015-v1/merged";
      tag = "V07-04-08";
      shortname="datamuoneg2";
      path = "/store/group/snt/run2_data/"; 
      is25 = false;
      isData = true;
      break;
    case DataDoubleMuonC:
      name="Run2015C_DoubleMuon_MINIAOD_PromptReco-v1/merged";
      tag = "V07-04-08";
      shortname="datadoublemuonc";
      path = "/store/group/snt/run2_data/"; 
      isData = true;
      is25 = true;
      break;
    case DataDoubleEGC:
      name="Run2015C_DoubleEG_MINIAOD_PromptReco-v1/merged";
      tag = "V07-04-08";
      shortname="datadoubleegc";
      path = "/store/group/snt/run2_data/"; 
      isData = true;
      is25 = true;
      break;
    case DATAMUONEGC:
      name="Run2015C_MuonEG_MINIAOD_PromptReco-v1/merged";
      tag = "V07-04-08";
      shortname="datamuonegc";
      path = "/store/group/snt/run2_data/"; 
      is25 = true;
      isData = true;
      break;
    case TTZQ:
      name="TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="ttzq";
      path = "/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP1:
      name="ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      shortname="singletop1";
      tag = "V07-04-08";
      path = "/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP2:
      name="ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="singletop2";
      path = "/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP3:
      name="ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="singletop3";
      path = "/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP4:
      name="ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="singletop4";
      path = "/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP5:
      name="ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="singletop5";
      path = "/store/group/snt/run2_25ns/";
      break;
    case TTWQQ:
      name="TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-08";
      shortname="ttwqq";
      path = "/store/group/snt/run2_25ns/";
      break;
    default:
      name="dummy";
      path = "/store/group/snt/run2_25ns/";
      break;
  }
  
  //Set up file and tree
  cout << "Using xrootd " << endl;
  mylooper->MakeBabyNtuple(Form("%s%s", shortname.c_str(), suffix), expt);
  TFile *f = TFile::Open(Form("/hadoop/cms/%s/%s/%s/%s", path.c_str(), name.c_str(), tag.c_str(), filename)); 
  cout << Form("/hadoop/cms/%s/%s/%s/%s", path.c_str(), name.c_str(), tag.c_str(), filename) << endl;
  cout << "File opened...." << endl;
  TTree *tree = (TTree*)f->Get("Events");
  cms3.Init(tree);

  //Event Counting
  unsigned int nEvents = tree->GetEntries();
  unsigned int nEventsTotal = 0;
  cout << "nEvents: " << tree->GetEntries() << endl;

  //Add good run list
  if (!is25) set_goodrun_file("final_golden_50ns_40p03pb.txt");
  if ( is25) set_goodrun_file("golden_25ns_20pb.txt");

  //Init MVA
  createAndInitMVA("./CORE");

  //JEC files -- 50 ns MC
  std::vector<std::string> jetcorr_filenames_50ns_MC_pfL1L2L3;
  jetcorr_filenames_50ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_MC_L1FastJet_AK4PFchs.txt");
  jetcorr_filenames_50ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_MC_L2Relative_AK4PFchs.txt");
  jetcorr_filenames_50ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_MC_L3Absolute_AK4PFchs.txt");

  //JEC files -- 50 ns DATA
  std::vector<std::string> jetcorr_filenames_50ns_DATA_pfL1L2L3;
  jetcorr_filenames_50ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_L1FastJet_AK4PFchs.txt");
  jetcorr_filenames_50ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_L2Relative_AK4PFchs.txt");
  jetcorr_filenames_50ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_L3Absolute_AK4PFchs.txt");
  jetcorr_filenames_50ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_L2L3Residual_AK4PFchs.txt");

  //JEC files -- 25 ns MC
  std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1L2L3;
  jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L1FastJet_AK4PFchs.txt");
  jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L2Relative_AK4PFchs.txt");
  jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L3Absolute_AK4PFchs.txt");

  //JEC files -- 25 ns DATA
  jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_DATA_L1FastJet_AK4PFchs.txt");
  jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_DATA_L2L3Residual_AK4PFchs.txt");
  jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_DATA_L2Relative_AK4PFchs.txt");
  jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_DATA_L3Absolute_AK4PFchs.txt");

  //JECs
  std::vector<std::string> filenames;
  if ( is25 &&  isData) filenames = jetcorr_filenames_25ns_DATA_pfL1L2L3;
  if ( is25 && !isData) filenames = jetcorr_filenames_25ns_MC_pfL1L2L3;
  if (!is25 && !isData) filenames = jetcorr_filenames_50ns_MC_pfL1L2L3;
  if (!is25 &&  isData) filenames = jetcorr_filenames_50ns_DATA_pfL1L2L3;
  FactorizedJetCorrector *jetCorrAG;
  jetCorrAG = makeJetCorrector(filenames);

  //Event Loop
  for(unsigned int eventAG = 0; eventAG < nEvents; eventAG++){

    //Get Event Content
    cms3.GetEntry(eventAG);
    nEventsTotal++;

    //If data, check good run list
    if (tas::evt_isRealData() && !goodrun(tas::evt_run(), tas::evt_lumiBlock())) continue; 

    //Progress bar
    CMS3::progress(nEventsTotal, nEvents);

    mylooper->ProcessBaby(ptrel, name, jetCorrAG, expt); 

  }//event loop 

  //Delete Chain
  mylooper->CloseBabyNtuple();
  delete f; 

  return 0;

}
