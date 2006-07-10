#include "CORE/CMS3.h"
#include "CORE/SSSelections.h"
#include "TTree.h"
#include "TFile.h"
#include "helper_babymaker.h"

enum sample_t { TTBAR, TTW, TTZ, WZ, T1TTTT_1500, T1TTTT_1200, T5qqqqWW_1200_1000_800, T5qqqqWW_deg_1000_315_300, DY1, DY2, DY3, DY4, WJets1, WJets2, WJets3, WJets4, DY, WJets };

void run(sample_t which, int file, IsolationMethods ptrel, bool expt){

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
      name = "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "ttbar";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case TTW:
      name = "TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "ttw";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case TTZ:
      name = "TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "ttz";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case WZ:
      name = "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "wz";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case T1TTTT_1500:
      name = "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
      shortname = "t1tttt_1500";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case T1TTTT_1200:
      name = "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
      shortname = "t1tttt_1200";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case DY1:
      name = "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/";
      shortname = "dy1";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case DY2:
      name = "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/";
      shortname = "dy2";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case DY3:
      name = "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/";
      shortname = "dy3";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case DY4: 
      name = "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/";
      shortname = "dy4";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case WJets1:
      name = "WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "wjets1";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case WJets2:
      name = "WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "wjets2";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case WJets3:
      name = "WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "wjets3";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case WJets4:
      name = "WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "wjets4";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case DY:
      name="DYJetsToLL_M-50_13TeV-madgraph-pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "dy";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case WJets:
      name="WJetsToLNu_13TeV-madgraph-pythia8-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
      shortname = "wjets";
      path = "/hadoop/cms/store/group/snt/phys14/"; 
      break;
    case  T5qqqqWW_1200_1000_800:
      name = "T5qqqqWW_1200_1000_800";
      shortname = "t5qqqqww_1200_1000_800";
      path = "/hadoop/cms/store/user/cgeorge/privateSusySignalsSS/"; 
      break;
    case T5qqqqWW_deg_1000_315_300:
      name = "T5qqqqWW_deg_1000_315_300";
      shortname = "t5qqqqww_deg_1000_315_300";
      path = "/hadoop/cms/store/user/cgeorge/privateSusySignalsSS/"; 
      break;
  }
  
  //Name of tag to be used
  string tag = "V07-02-08";
  
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

  //Init MVA
  createAndInitMVA("./CORE");

  //Event Loop
  for(unsigned int event = 0; event < nEvents; event++){

    //Get Event Content
    cms3.GetEntry(event);
    nEventsTotal++;

    //Progress bar
    CMS3::progress(nEventsTotal, nEvents);

    mylooper->ProcessBaby(ptrel, f->GetTitle(), expt); 

  }//event loop 

//Delete Chain
mylooper->CloseBabyNtuple();
delete f; 

}
