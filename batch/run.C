#include "CORE/CMS3.h"
#include "CORE/SSSelections.h"
#include "TTree.h"
#include "TFile.h"
#include "helper_babymaker.h"
#include "base.h"
#include "CORE/Tools/goodrun.h"

void run(int which_in, int file, int ptrel_in, bool expt){

  sample_t which = static_cast<sample_t>(which_in);
  IsolationMethods ptrel = static_cast<IsolationMethods>(ptrel_in); 

  babyMaker *mylooper = new babyMaker();

  //Path, filename, suffix
  string path = "";
  string name = ""; 
  string shortname = ""; 
  const char* filename = Form("merged_ntuple_%i.root", file);
  const char* suffix = file == 0 ? "" : Form("_%i", file);

  //Info for each sample
  switch (which){
    case TTBAR:
      name = "TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
      shortname = "ttbar";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case TTW:
      cout << "TTW NOT SUPPORTED!!" << endl;
      name = "";
      shortname = "ttw";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case TTZ:
      cout << "TTZ NOT SUPPORTED!!" << endl;
      name = "TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "ttz";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case WZ:
      name = "WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2";
      shortname = "wz";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case T1TTTT_1500:
      cout << "T1TTTT NOT SUPPORTED!!" << endl;
      name = "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
      shortname = "t1tttt_1500";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case T1TTTT_1200:
      cout << "T1TTTT NOT SUPPORTED!!" << endl;
      name = "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
      shortname = "t1tttt_1200";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case DY_high:
      name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2";
      shortname = "dy";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case DY_low:
      name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
      shortname = "dy";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case WJets:
      name="WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
      shortname = "wjets";
      path = "/hadoop/cms/store/group/snt/run2_50ns/"; 
      break;
    case DataDoubleMuon:
      name="Run2015B_DoubleMuon_MINIAOD_PromptReco-v1/merged/";
      shortname = "DataDoubleMuon";
      path = "/hadoop/cms/store/group/snt/run2_data/"; 
      break;
    case DataDoubleEG:
      name="Run2015B_DoubleEG_MINIAOD_PromptReco-v1/merged/";
      shortname = "DataDoubleEG";
      path = "/hadoop/cms/store/group/snt/run2_data/"; 
      break;
  }
  
  //Name of tag to be used
  string tag = "V07-04-04";
  
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
  set_goodrun_file("goodRunList/json_DCSONLY_Run2015B_snt_071315.txt");

  //Init MVA
  createAndInitMVA("./CORE");

  //Event Loop
  for(unsigned int event = 0; event < nEvents; event++){

    //Get Event Content
    cms3.GetEntry(event);
    nEventsTotal++;

    //If data, check good run list
    if (tas::evt_isRealData() && !goodrun(tas::evt_run(), tas::evt_lumiBlock())) continue; 

    //Progress bar
    CMS3::progress(nEventsTotal, nEvents);

    mylooper->ProcessBaby(ptrel, f->GetTitle(), expt); 

  }//event loop 

  //Delete Chain
  mylooper->CloseBabyNtuple();
  delete f; 

}
