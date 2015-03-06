#include "TChain.h"
#include "looper.h"

int main() {

  looper *l = new looper();

  bool useSkim   = false;
  bool skimAll   = false;
  bool runAll    = false;
  bool runLepEff = false;
  bool runSync   = false;
  bool runBaby   = true;
  bool usePtRel  = false;

  if (runSync) { 
    TChain *chain_synctest = new TChain("Events");
    //chain_synctest->Add("./TTJets_skimSS/merged_ntuple_*.root");
    //chain_synctest->Add("/hadoop/cms/store/group/snt/csa14/MC_CMS3_V07-00-04/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/merged/merged_ntuple_*.root");
    chain_synctest->Add("./phys14_sync_CMS3.root");
    std::vector<int> eventsToDebug;
    //eventsToDebug.push_back(4878);
    l->ScanChain(chain_synctest,"synctest","",0,"SyncTest",-1,usePtRel, eventsToDebug);
    return 0;
  }

  TChain *chain_T1ttttG1200   = new TChain("Events");
  TChain *chain_T1ttttG1500   = new TChain("Events");
  TChain *chain_T5qqqqWW1200  = new TChain("Events");
  TChain *chain_T5qqqqWW1500  = new TChain("Events");
  TChain *chain_T6ttWW600_150 = new TChain("Events");
  TChain *chain_T6ttWW600_425 = new TChain("Events");
  TChain *chain_TTJets        = new TChain("Events");
  TChain *chain_TTWJets       = new TChain("Events");
  TChain *chain_TTZJets       = new TChain("Events");
  TChain *chain_WHZH          = new TChain("Events");
  TChain *chain_WW            = new TChain("Events");
  TChain *chain_WZJets        = new TChain("Events");
  TChain *chain_DY            = new TChain("Events");
  TChain *chain_TTbarH        = new TChain("Events");
  TChain *chain_TW            = new TChain("Events");
  TChain *chain_WJets         = new TChain("Events");
  TChain *chain_Wjets_ht      = new TChain("Events");
  TChain *chain_ZZTo4L        = new TChain("Events");
  TChain *chain_Wjets         = new TChain("Events");

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
    chain_T5qqqqWW1200->Add(dir+"/T5qqqqWW1200_skimSS/merged_ntuple_*.root");
    chain_T5qqqqWW1500->Add(dir+"/T5qqqqWW1500_skimSS/merged_ntuple_*.root");
    chain_DY->Add(dir+"/DYJetsLL/merged_ntuple_1.root");//fixme
  } 
  else {
    TString dir  = "/hadoop/cms/store/group/snt/phys14/";
    TString dir2 = "/nfs-7/userdata/ss2015/ttbar_copy/";
    TString tag  = "V07-02-05";
    TString tag2 = "V07-02-03";
    TString tag3 = "V07-02-06";
    chain_Wjets->Add(dir+"WJetsToLNu_13TeV-madgraph-pythia8-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag3+"/merged_ntuple_*.root");
    chain_T1ttttG1200->Add(dir+"SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_T1ttttG1500->Add(dir+"SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_T5qqqqWW1200->Add("/hadoop/cms/store/user/cgeorge/privateSusySignalsSS/T5qqqqWW_mGo1200_mCh1000_mChi800/merged/merged_ntuple_*.root");
    chain_T5qqqqWW1500->Add("/hadoop/cms/store/user/cgeorge/privateSusySignalsSS/13TeV_T5qqqqWW_Gl1500_Chi800_LSP100/merged/merged_ntuple_*.root");
    chain_T6ttWW600_150->Add("/hadoop/cms/store/user/cgeorge/privateSusySignalsSS/T6ttWW_mSbottom600_mCh150_mChi50_v2/merged/merged_ntuple_*.root");
    chain_T6ttWW600_425->Add("/hadoop/cms/store/user/cgeorge/privateSusySignalsSS/T6ttWW_mSbottom600_mCh425_mChi50_v2/merged/merged_ntuple_*.root");
    chain_TTJets->Add(    dir2+"merged_ntuple_*.root");
    chain_TTWJets->Add(    dir+"TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_TTZJets->Add(    dir+"TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_WHZH->Add(       dir+"WH_ZH_HToWW_M-125_13TeV_pythia6/"+tag+"/merged_ntuple_*.root");
    chain_WW->Add(         dir+"WWTo2L2Nu_CT10_13TeV-powheg-pythia8-tauola/"+tag+"/merged_ntuple_*.root");
    chain_WZJets->Add(     dir+"WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag+"/merged_ntuple_*.root");
    chain_DY->Add(         dir+"DYJetsToLL_M-50_13TeV-madgraph-pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");//fixme

    chain_TTbarH->Add(dir+"TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/"+tag2+"/merged_ntuple_*.root");
    chain_WJets->Add(dir+"WJetsToLNu_13TeV-madgraph-pythia8-tauola/"+tag+"/merged_ntuple_*.root");
    chain_Wjets_ht->Add(dir+"WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag2 + "/merged_ntuple_*.root");
    chain_Wjets_ht->Add(dir+"WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag2 + "/merged_ntuple_*.root");
    chain_Wjets_ht->Add(dir+"WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag2 + "/merged_ntuple_*.root");
    chain_Wjets_ht->Add(dir+"WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag2 + "/merged_ntuple_*.root");
    chain_ZZTo4L->Add(dir+"ZZTo4L_Tune4C_13TeV-powheg-pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");
    chain_TW->Add(dir+"T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");
    chain_TW->Add(dir+"Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");
    chain_TW->Add(dir+"TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");
    chain_TW->Add(dir+"TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");
    chain_TW->Add(dir+"TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");
    chain_TW->Add(dir+"TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"+tag2+"/merged_ntuple_*.root");
  }

  if (skimAll) {

    l->ScanChain(chain_T1ttttG1200,"T1ttttG1200","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_T1ttttG1500,"T1ttttG1500","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_T5qqqqWW1200,"T5qqqqWW1200","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_T5qqqqWW1500,"T5qqqqWW1500","",0,"SSskim",-1,usePtRel);

    l->ScanChain(chain_TTWJets,"TTWJets","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_TTZJets,"TTZJets","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_WHZH,"WHZH","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_WW,"WW","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_WZJets,"WZJets","",0,"SSskim",-1,usePtRel);

    l->ScanChain(chain_TTbarH,"TTbarH","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_WJets,"WJets","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_ZZTo4L,"ZZTo4L","",0,"SSskim",-1,usePtRel);

    l->ScanChain(chain_TTJets,"TTJets","",0,"SSskim",-1,usePtRel);
    l->ScanChain(chain_TW,"TW","",0,"SSskim",-1,usePtRel);

  }

  //l->ScanChain(chain_T1ttttG1500,"T1ttttG1500","",0,"",-1,usePtRel);
  //l->ScanChain(chain_TTJets,"ttbar","",0,"",-1,usePtRel);
  if (runAll) {
    l->ScanChain(chain_TTJets,"ttbar","",0,"",-1,usePtRel);
    l->ScanChain(chain_TTWJets,"TTWJets","",0,"",-1,usePtRel);
    l->ScanChain(chain_TTZJets,"TTZJets","",0,"",-1,usePtRel);
    // l->ScanChain(chain_WHZH,"WHZH","",0,"",-1,usePtRel);
    // l->ScanChain(chain_WW,"WW","",0,"",-1,usePtRel);
    l->ScanChain(chain_WZJets,"WZJets","",0,"",-1,usePtRel);
    l->ScanChain(chain_T1ttttG1200,"T1ttttG1200","",0,"",-1,usePtRel);
    l->ScanChain(chain_T1ttttG1500,"T1ttttG1500","",0,"",-1,usePtRel);
    //l->ScanChain(chain_T5qqqqWW1200,"T5qqqqWW1200","",0,"",-1,usePtRel);
    //l->ScanChain(chain_T5qqqqWW1500,"T5qqqqWW1500","",0,"",-1,usePtRel);
  }
  
  if (runLepEff) {
    l->ScanChain(chain_DY,"dy","effic",0,"DYtest",-1,usePtRel);
    l->ScanChain(chain_T1ttttG1200,"T1ttttG1200","effic",0,"DYtest",-1,usePtRel);
    l->ScanChain(chain_T1ttttG1500,"T1ttttG1500","effic",0,"DYtest",-1,usePtRel);
    //l->ScanChain(chain_T5qqqqWW1200,"T5qqqqWW1200","effic",0,"DYtest",-1,usePtRel);
    //l->ScanChain(chain_T5qqqqWW1500,"T5qqqqWW1500","effic",0,"DYtest",-1,usePtRel);
    l->ScanChain(chain_TTWJets,"TTWJets","effic",0,"DYtest",-1,usePtRel);
  }

  if (runBaby){
    //l->ScanChain(chain_TTZJets,       "ttz"             , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_WZJets,        "wz"              , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_T1ttttG1200,   "t1tttt_1200_800" , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_T1ttttG1500,   "t1tttt_1500_100" , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_TTJets,        "ttbar"           , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_TTWJets,       "ttw"             , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_T5qqqqWW1200,  "t5qqqqWW1200"    , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_T5qqqqWW1500,  "t5qqqqWW1500"    , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_T6ttWW600_150, "t6ttWW600_150"   , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_T6ttWW600_425, "t6ttWW600_425"   , "baby", 0, "MakeBaby", -1, usePtRel);
    //l->ScanChain(chain_Wjets,         "Wjets"           , "baby", 0, "MakeBaby", -1, usePtRel);
      l->ScanChain(chain_Wjets_ht,      "Wjets_ht"        , "baby", 0, "MakeBaby", -1, usePtRel);

  }

}
