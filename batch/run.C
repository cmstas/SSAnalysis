#include "CORE/CMS3.h"
#include "TTree.h"
#include "TFile.h"
#include "helper_babymaker.h"

enum sample_t { TTBAR, TTW, TTZ, WZ, T1TTTT_1500, T1TTTT_1200 };

void run(sample_t which, int file, bool ptrel){

  babyMaker *mylooper = new babyMaker();

  //Path, filename, suffix
  string path = "root://xrootd.t2.ucsd.edu///store/group/snt/phys14/";
  const char* filename = Form("merged_ntuple_%i.root", file);
  const char* ptrel_name = ptrel == 1 ? "_ptRel" : "";  
  const char* suffix = file == 0 ? "" : Form("_%i", file);

  //Name of each sample
  string name = ""; 
  if (which == TTBAR) name = "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  if (which == TTW) name = "TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  if (which == TTZ) name = "TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  if (which == WZ) name = "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  if (which == T1TTTT_1500) name = "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
  if (which == T1TTTT_1200) name = "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
  
  //Name of output babies
  string shortname = "";
  if (which == TTBAR) shortname = "ttbar";
  if (which == TTW) shortname = "ttw";
  if (which == TTZ) shortname = "ttz";
  if (which == WZ) shortname = "wz";
  if (which == T1TTTT_1500) shortname = "t1tttt_1500";
  if (which == T1TTTT_1200) shortname = "t1tttt_1200";
  
  //Name of tag to be used
  string tag = "V07-02-08";
  
  //Set up file and tree
  cout << "Using xrootd " << endl;
  mylooper->MakeBabyNtuple(Form("%s%s%s", shortname.c_str(), ptrel_name, suffix));
  TFile *f = TFile::Open(Form("%s/%s/%s/%s", path.c_str(), name.c_str(), tag.c_str(), filename)); 
  cout << "File opened...." << endl;
  TTree *tree = (TTree*)f->Get("Events");
  cms3.Init(tree);

  //Event Counting
  unsigned int nEvents = tree->GetEntries();
  unsigned int nEventsTotal = 0;
  cout << "nEvents: " << tree->GetEntries() << endl;

  //Event Loop
  for(unsigned int event = 0; event < nEvents; ++event) {
   
    //Get Event Content
    cms3.GetEntry(event);
    nEventsTotal++;

    //Progress bar
    CMS3::progress(nEventsTotal, nEvents);

    mylooper->ProcessBaby(ptrel); 

  }//event loop 

//Delete Chain
mylooper->CloseBabyNtuple();
delete f; 

}
