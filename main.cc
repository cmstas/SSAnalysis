#include "TChain.h"
#include "looper.h"

int main() {

  looper *l = new looper();

  bool useSkim = true;

  bool skimAll = false;
  bool runAll  = true;
  bool runLepEff = false;
  bool runSync = false;
  bool runBaby = false;

  //looper::ScanChain( TChain* chain, TString prefix, TString postfix, bool isData, TString whatTest, int nEvents)

  if (runSync) { 
    TChain *chain_synctest = new TChain("Events");
    //chain_synctest->Add("./TTJets_skimSS/merged_ntuple_*.root");
    //chain_synctest->Add("/hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-04/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/merged/merged_ntuple_*.root");
    chain_synctest->Add("./phys14_sync_CMS3.root");
    std::vector<int> eventsToDebug;
    //eventsToDebug.push_back(4878);
    l->ScanChain(chain_synctest,"synctest","",0,"SyncTest",-1,eventsToDebug);
    return 0;
  }

  // TChain *chain_TTZJets = new TChain("Events");
  // chain_TTZJets->Add("./TTZJets_skimSS/merged_ntuple_*.root");
  // l->ScanChain(chain_TTZJets,"TTZJets_test","",0,"",-1);

  TChain *chain_T1ttttG1200 = new TChain("Events");
  TChain *chain_T1ttttG1500 = new TChain("Events");
  TChain *chain_T5Full1200 = new TChain("Events");
  TChain *chain_T5Full1500 = new TChain("Events");
  TChain *chain_TTJets = new TChain("Events");
  TChain *chain_TTWJets = new TChain("Events");
  TChain *chain_TTZJets = new TChain("Events");
  TChain *chain_WHZH = new TChain("Events");
  TChain *chain_WW = new TChain("Events");
  TChain *chain_WZJets = new TChain("Events");
  TChain *chain_DY = new TChain("Events");

  if (useSkim) {
    TString dir = "PHYS14";
    chain_TTJets->Add(dir+"/TTJets_skimSS/merged_ntuple_*.root");
    chain_TTWJets->Add(dir+"/TTWJets_skimSS/merged_ntuple_*.root");
    chain_TTZJets->Add(dir+"/TTZJets_skimSS/merged_ntuple_*.root");
    chain_WW->Add(dir+"/WW_skimSS/merged_ntuple_*.root");
    chain_WZJets->Add(dir+"/WZJets_skimSS/merged_ntuple_*.root");
    chain_WHZH->Add(dir+"/WHZH_skimSS/merged_ntuple_*.root");
    chain_T1ttttG1200->Add(dir+"/T1ttttG1200_skimSS/merged_ntuple_*.root");
    chain_T1ttttG1500->Add(dir+"/T1ttttG1500_skimSS/merged_ntuple_*.root");
    chain_T5Full1200->Add(dir+"/T5Full1200_skimSS/merged_ntuple_*.root");
    chain_T5Full1500->Add(dir+"/T5Full1500_skimSS/merged_ntuple_*.root");
    chain_DY->Add(dir+"/DYJetsLL/merged_ntuple_1.root");//fixme
  } else {
    TString dir = "/hadoop/cms/store/group/snt/phys14/";
    TString tag = "V07-02-05";
    TString tag2 = "V07-02-03";
    chain_T1ttttG1200->Add(dir+"SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_T1ttttG1500->Add(dir+"SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    //chain_T5Full1200->Add( dir+"T5Full_T5Full-1200-1000-800-Decay-MGMMatch50/"+tag+"/merged_ntuple_*.root");
    //chain_T5Full1500->Add( dir+"T5Full_T5Full-1500-800-100-Decay-MGMMatch50/"+tag+"/merged_ntuple_*.root");
    chain_TTJets->Add(     dir+"TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_TTWJets->Add(    dir+"TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_TTZJets->Add(    dir+"TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    //chain_WHZH->Add(       dir+"WH_ZH_HToWW_M-125_13TeV_pythia6/"+tag+"/merged_ntuple_*.root");
    //chain_WW->Add(         dir+"WWTo2L2Nu_CT10_13TeV-powheg-pythia8-tauola/"+tag+"/merged_ntuple_*.root");
    chain_WZJets->Add(     dir+"WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_DY->Add(         dir+"DYJetsToLL_M-50_13TeV-madgraph-pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");//fixme
  }

  if (skimAll) {
    l->ScanChain(chain_T1ttttG1200,"T1ttttG1200","",0,"SSskim",-1);
    l->ScanChain(chain_T1ttttG1500,"T1ttttG1500","",0,"SSskim",-1);
    //l->ScanChain(chain_T5Full1200,"T5Full1200","",0,"SSskim",-1);
    //l->ScanChain(chain_T5Full1500,"T5Full1500","",0,"SSskim",-1);
    l->ScanChain(chain_TTJets,"TTJets","",0,"SSskim",-1);
    l->ScanChain(chain_TTWJets,"TTWJets","",0,"SSskim",-1);
    l->ScanChain(chain_TTZJets,"TTZJets","",0,"SSskim",-1);
    //l->ScanChain(chain_WHZH,"WHZH","",0,"SSskim",-1);
    //l->ScanChain(chain_WW,"WW","",0,"SSskim",-1);
    l->ScanChain(chain_WZJets,"WZJets","",0,"SSskim",-1);
  }

  //l->ScanChain(chain_T1ttttG1500,"T1ttttG1500","",0,"",-1);
  //l->ScanChain(chain_TTJets,"ttbar","",0,"",-1);
  if (runAll) {
    l->ScanChain(chain_TTJets,"ttbar","",0,"",-1);
    l->ScanChain(chain_TTWJets,"TTWJets","",0,"",-1);
    l->ScanChain(chain_TTZJets,"TTZJets","",0,"",-1);
    //l->ScanChain(chain_WHZH,"WHZH","",0,"",-1);
    //l->ScanChain(chain_WW,"WW","",0,"",-1);
    l->ScanChain(chain_WZJets,"WZJets","",0,"",-1);
    l->ScanChain(chain_T1ttttG1200,"T1ttttG1200","",0,"",-1);
    l->ScanChain(chain_T1ttttG1500,"T1ttttG1500","",0,"",-1);
    //l->ScanChain(chain_T5Full1200,"T5Full1200","",0,"",-1);
    //l->ScanChain(chain_T5Full1500,"T5Full1500","",0,"",-1);
  }
  
  if (runLepEff) {
    l->ScanChain(chain_DY,"dy","effic",0,"DYtest",-1);
    l->ScanChain(chain_T1ttttG1200,"T1ttttG1200","effic",0,"DYtest",-1);
    l->ScanChain(chain_T1ttttG1500,"T1ttttG1500","effic",0,"DYtest",-1);
    //l->ScanChain(chain_T5Full1200,"T5Full1200","effic",0,"DYtest",-1);
    //l->ScanChain(chain_T5Full1500,"T5Full1500","effic",0,"DYtest",-1);
    l->ScanChain(chain_TTWJets,"TTWJets","effic",0,"DYtest",-1);
  }

  if (runBaby) {
    l->ScanChain(chain_TTJets,"ttbar","baby",0,"MakeBaby",-1);
    l->ScanChain(chain_TTWJets,"ttw","baby",0,"MakeBaby",-1);
    l->ScanChain(chain_TTZJets,"ttz","baby",0,"MakeBaby",-1);
    l->ScanChain(chain_WZJets,"wz","baby",0,"MakeBaby",-1);
    l->ScanChain(chain_T1ttttG1200,"t1tttt_1200_800","baby",0,"MakeBaby",-1);
    l->ScanChain(chain_T1ttttG1500,"t1tttt_1500_100","baby",0,"MakeBaby",-1);
  }

}
