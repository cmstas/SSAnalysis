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
  string tag = "V07-04-03";

  //Info for each sample
  switch (which){
    case TTBAR:
      name="TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2";
      shortname="ttbar";
      tag = "V07-04-03";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case TTW:
      name="TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-07";
      shortname="ttw";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case TTZL:
      name="TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-07";
      shortname="ttzl";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case WZ:
      name="WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-03";
      shortname="wz";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case DY_low:
      name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-03";
      shortname="dy_low";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case DY_high:
      name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3";
      tag = "V07-04-06";
      shortname="dy_high";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case WJets:
      name="WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-03";
      shortname="wjets";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case DataDoubleMuon:
      name="Run2015B_DoubleMuon_MINIAOD_PromptReco-v1/merged";
      tag = "V07-04-07";
      shortname="datadoublemuon";
      path = "/hadoop/cms/store/group/snt/run2_data/"; 
      break;
    case DataDoubleEG:
      name="Run2015B_DoubleEG_MINIAOD_PromptReco-v1/merged";
      tag = "V07-04-07";
      shortname="datadoubleeg";
      path = "/hadoop/cms/store/group/snt/run2_data/"; 
      break;
    case TTZQ:
      name="TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/merged";
      tag = "V07-04-07";
      shortname="ttzq";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case DATAMUONEG:
      name="Run2015B_MuonEG_MINIAOD_PromptReco-v1";
      tag = "V07-04-07";
      shortname="datamuoneg";
      path = "/hadoop/cms/store/group/snt/run2_data/"; 
      break;
    case SINGLETOP1:
      name="ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      shortname="singletop1";
      tag = "V07-04-03";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP2:
      name="ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-03";
      shortname="singletop2";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP3:
      name="ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-03";
      shortname="singletop3";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP4:
      name="ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-03";
      shortname="singletop4";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    case SINGLETOP5:
      name="ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1";
      tag = "V07-04-03";
      shortname="singletop5";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
    default:
      name="dummy";
      path = "/hadoop/cms/store/group/snt/run2_25ns/";
      break;
  }
  
  //Set up file and tree
  cout << "Using xrootd " << endl;
  mylooper->MakeBabyNtuple(Form("%s%s", shortname.c_str(), suffix), expt);
  TFile *f = TFile::Open(Form("%s/%s/%s/%s", path.c_str(), name.c_str(), tag.c_str(), filename)); 
  cout << "File opened...." << endl;
  TTree *tree = (TTree*)f->Get("Events");
  cms3.Init(tree);

  //Event Counting
  unsigned int nEvents = tree->GetEntries();
  unsigned int nEventsTotal = 0;
  cout << "nEvents: " << tree->GetEntries() << endl;

  //Add good run list
  set_goodrun_file("final_golden_50ns_40p03pb.txt");

  //Init MVA
  createAndInitMVA("./CORE");

  //JECs
  std::vector<std::string> filenames;
  FactorizedJetCorrector *jetCorrAG;
  std::string blah1 = "CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV2_MC_L1FastJet_AK4PFchs.txt";
  std::string blah2 = "CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV2_MC_L2Relative_AK4PFchs.txt";
  std::string blah3 = "CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV2_MC_L3Absolute_AK4PFchs.txt";
  filenames.push_back(blah1);
  filenames.push_back(blah2);
  filenames.push_back(blah3);
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

    mylooper->ProcessBaby(ptrel, f->GetTitle(), jetCorrAG, expt); 

  }//event loop 

  //Delete Chain
  mylooper->CloseBabyNtuple();
  delete f; 

  return 0;

}
