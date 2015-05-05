#include "TChain.h"
#include "looper.h"

int runMain(bool useSkim, bool skimAll, bool runAll, bool runLepEff, bool runSync, string runBaby_str, std::vector<int> eventsToDebug){

  //Figure out which babies to make
  bool runBaby = false;
  bool babiesToMake[20] = {0};
  if (runBaby_str != ""){
    runBaby = true;
    unsigned int i = 0;

    unsigned int maxRuns = std::count(runBaby_str.begin(), runBaby_str.end(), ',') + 1;

    if (maxRuns == 1) babiesToMake[std::stoi(runBaby_str.c_str())] = 1; 
    else { 
      while (i < maxRuns){
        int comma = runBaby_str.find(","); 
        int baby  = stoi(runBaby_str.substr(0, comma).c_str()); 
        runBaby_str = runBaby_str.substr(comma+1, string::npos); 
        babiesToMake[baby] = 1;
        i++;
      }
    }
  }

  looper *l = new looper();

  IsolationMethods isoCase = MultiIso;

  if (runSync) { 
    TChain *chain_synctest = new TChain("Events");
    chain_synctest->Add("./phys14_sync_CMS3.root");
    l->ScanChain(chain_synctest,"synctest","",0,"SyncTest",-1,isoCase, eventsToDebug);
    return 0;
  }

  TChain *chain_T1ttttG1200               = new TChain("Events");
  TChain *chain_T1ttttG1500               = new TChain("Events");
  TChain *chain_T5qqqqWW1200              = new TChain("Events");
  TChain *chain_T5qqqqWW1500              = new TChain("Events");
  TChain *chain_T6ttWW600_150             = new TChain("Events");
  TChain *chain_T6ttWW600_425             = new TChain("Events");
  TChain *chain_TTJets                    = new TChain("Events");
  TChain *chain_TTWJets                   = new TChain("Events");
  TChain *chain_TTZJets                   = new TChain("Events");
  TChain *chain_WHZH                      = new TChain("Events");
  TChain *chain_WW                        = new TChain("Events");
  TChain *chain_WZJets                    = new TChain("Events");
  TChain *chain_DY                        = new TChain("Events");
  TChain *chain_TTbarH                    = new TChain("Events");
  TChain *chain_TW                        = new TChain("Events");
  TChain *chain_WJets                     = new TChain("Events");
  TChain *chain_Wjets_ht                  = new TChain("Events");
  TChain *chain_ZZTo4L                    = new TChain("Events");
  TChain *chain_Wjets                     = new TChain("Events");
  TChain *chain_T5qqqqWW_deg_1000_315_300 = new TChain("Events");
  TChain *chain_T5qqqqWW_1200_1000_800    = new TChain("Events");
  TChain *chain_sync                      = new TChain("Events");

  if (useSkim) {
    TString dir  = "PHYS14_v2_new";
    TString dir2 = "/merged_ntuple_*.root";

    chain_TTJets       ->Add(dir + "/TTJets_skimSS"       + dir2);
    chain_TTWJets      ->Add(dir + "/TTWJets_skimSS"      + dir2);
    chain_TTZJets      ->Add(dir + "/TTZJets_skimSS"      + dir2);
    chain_WW           ->Add(dir + "/WW_skimSS"           + dir2);
    chain_WZJets       ->Add(dir + "/WZJets_skimSS"       + dir2);
    chain_WHZH         ->Add(dir + "/WHZH_skimSS"         + dir2);
    chain_T1ttttG1200  ->Add(dir + "/T1ttttG1200_skimSS"  + dir2);
    chain_T1ttttG1500  ->Add(dir + "/T1ttttG1500_skimSS"  + dir2);
    chain_T5qqqqWW1200 ->Add(dir + "/T5qqqqWW1200_skimSS" + dir2);
    chain_T5qqqqWW1500 ->Add(dir + "/T5qqqqWW1500_skimSS" + dir2);
    chain_DY           ->Add(dir + "/DYJetsLL"            + dir2); 
  } 
  else {
    TString dir = "/hadoop/cms/store/group/snt/phys14/";
    TString tag = "V07-02-08/merged_ntuple_*.root";

    //SMS
    chain_T1ttttG1200 ->Add(dir + "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/" + tag);
    chain_T1ttttG1500 ->Add(dir + "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/" + tag);

    //Main SM
    chain_TTJets  ->Add(dir + "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_TTWJets ->Add(dir + "TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"                    + tag);
    chain_TTZJets ->Add(dir + "TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"                    + tag);
    chain_WHZH    ->Add(dir + "WH_ZH_HToWW_M-125_13TeV_pythia6/"                                                           + tag);
    chain_WW      ->Add(dir + "WWTo2L2Nu_CT10_13TeV-powheg-pythia8-tauola/"                                                + tag);
    chain_WZJets  ->Add(dir + "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"               + tag); 
    chain_DY      ->Add(dir + "DYJetsToLL_M-50_13TeV-madgraph-pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"                  + tag); 
    chain_TTbarH  ->Add(dir + "TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/"          + tag);
    chain_ZZTo4L  ->Add(dir + "ZZTo4L_Tune4C_13TeV-powheg-pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"                      + tag);

    //W + Jets
    chain_WJets    ->Add(dir+"WJetsToLNu_13TeV-madgraph-pythia8-tauola/"                                              + tag);
    chain_Wjets    ->Add(dir+"WJetsToLNu_13TeV-madgraph-pythia8-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"            + tag);
    chain_Wjets_ht ->Add(dir+"WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_Wjets_ht ->Add(dir+"WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_Wjets_ht ->Add(dir+"WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_Wjets_ht ->Add(dir+"WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);

    //TW
    chain_TW ->Add(dir + "T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"           + tag);
    chain_TW ->Add(dir + "Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"        + tag);
    chain_TW ->Add(dir + "TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"    + tag);
    chain_TW ->Add(dir + "TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_TW ->Add(dir + "TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"    + tag);
    chain_TW ->Add(dir + "TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);

    //Private samples
    TString private_dir = "/nfs-7/userdata/ss2015/privateSamples/";
    chain_T5qqqqWW_deg_1000_315_300 ->Add(private_dir + "T5qqqqWW_deg_1000_315_300/" + tag);
    chain_T5qqqqWW_1200_1000_800    ->Add(private_dir + "T5qqqqWW_1200_1000_800/"    + tag);

    //Private (I don't trust these anymore)
    dir = "/hadoop/cms/store/user/cgeorge/privateSusySignalsSS/";
    TString dir2 = "/merged/merged_ntuple_*.root";
    chain_T5qqqqWW1200 ->Add(dir + "T5qqqqWW_mGo1200_mCh1000_mChi800"    + dir2);
    chain_T5qqqqWW1500 ->Add(dir + "13TeV_T5qqqqWW_Gl1500_Chi800_LSP100" + dir2);
    chain_T6ttWW600_150->Add(dir + "T6ttWW_mSbottom600_mCh150_mChi50_v2" + dir2);
    chain_T6ttWW600_425->Add(dir + "T6ttWW_mSbottom600_mCh425_mChi50_v2" + dir2);

    //Sync sample
    chain_sync->Add("/nfs-7/userdata/ss2015/sync_file/V07-02-08/sync_file.root");

  }

  if (skimAll){

    //SMS
    l->ScanChain(chain_T1ttttG1200 , "T1ttttG1200",  "", 0, "SSskim",-1,isoCase);
    l->ScanChain(chain_T1ttttG1500 , "T1ttttG1500",  "", 0, "SSskim",-1,isoCase);
    l->ScanChain(chain_T5qqqqWW1200, "T5qqqqWW1200", "", 0, "SSskim",-1,isoCase);
    l->ScanChain(chain_T5qqqqWW1500, "T5qqqqWW1500", "", 0, "SSskim",-1,isoCase);

    //SM
    l->ScanChain(chain_TTWJets, "TTWJets", "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_TTZJets, "TTZJets", "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_WHZH   , "WHZH"   , "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_WW     , "WW"     , "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_WZJets , "WZJets" , "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_TTbarH , "TTbarH" , "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_WJets  , "WJets"  , "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_ZZTo4L , "ZZTo4L" , "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_TTJets , "TTJets" , "", 0, "SSskim", -1, isoCase);
    l->ScanChain(chain_TW     , "TW"     , "", 0, "SSskim", -1, isoCase);

  }

  if (runAll){
    l->ScanChain(chain_TTJets,      "ttbar"       , "", 0, "", -1, isoCase);
    l->ScanChain(chain_TTWJets,     "TTWJets"     , "", 0, "", -1, isoCase);
    l->ScanChain(chain_TTZJets,     "TTZJets"     , "", 0, "", -1, isoCase);
    l->ScanChain(chain_WHZH,        "WHZH"        , "", 0, "", -1, isoCase);
    l->ScanChain(chain_WW,          "WW"          , "", 0, "", -1, isoCase);
    l->ScanChain(chain_WZJets,      "WZJets"      , "", 0, "", -1, isoCase);
    l->ScanChain(chain_T1ttttG1200, "T1ttttG1200" , "", 0, "", -1, isoCase);
    l->ScanChain(chain_T1ttttG1500, "T1ttttG1500" , "", 0, "", -1, isoCase);
    l->ScanChain(chain_T5qqqqWW1200,"T5qqqqWW1200", "", 0, "", -1, isoCase);
    l->ScanChain(chain_T5qqqqWW1500,"T5qqqqWW1500", "", 0, "", -1, isoCase);
  }
  
  if (runLepEff){
    l->ScanChain(chain_DY          , "dy"           , "effic", 0, "DYtest", -1, isoCase);
    l->ScanChain(chain_T1ttttG1200 , "T1ttttG1200"  , "effic", 0, "DYtest", -1, isoCase);
    l->ScanChain(chain_T1ttttG1500 , "T1ttttG1500"  , "effic", 0, "DYtest", -1, isoCase);
    l->ScanChain(chain_T5qqqqWW1200, "T5qqqqWW1200" , "effic", 0, "DYtest", -1, isoCase);
    l->ScanChain(chain_T5qqqqWW1500, "T5qqqqWW1500" , "effic", 0, "DYtest", -1, isoCase);
    l->ScanChain(chain_TTWJets     , "TTWJets"      , "effic", 0, "DYtest", -1, isoCase);
  }

  if (runBaby){
    if (babiesToMake[0]  == 1) l->ScanChain(chain_TTZJets                  , "ttz"                      , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[1]  == 1) l->ScanChain(chain_WZJets                   , "wz"                       , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[2]  == 1) l->ScanChain(chain_T1ttttG1200              , "t1tttt_1200_800"          , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[3]  == 1) l->ScanChain(chain_T1ttttG1500              , "t1tttt_1500_100"          , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[4]  == 1) l->ScanChain(chain_TTJets                   , "ttbar"                    , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[5]  == 1) l->ScanChain(chain_TTWJets                  , "ttw"                      , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[6]  == 1) l->ScanChain(chain_T5qqqqWW1200             , "t5qqqqWW1200"             , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[7]  == 1) l->ScanChain(chain_T5qqqqWW1500             , "t5qqqqWW1500"             , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[8]  == 1) l->ScanChain(chain_T6ttWW600_150            , "t6ttWW600_150"            , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[9]  == 1) l->ScanChain(chain_T6ttWW600_425            , "t6ttWW600_425"            , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[10] == 1) l->ScanChain(chain_Wjets                    , "Wjets"                    , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[11] == 1) l->ScanChain(chain_Wjets_ht                 , "Wjets_ht"                 , "baby", 0, "MakeBaby", -1, isoCase);
    if (babiesToMake[12] == 1) l->ScanChain(chain_T5qqqqWW_deg_1000_315_300, "t5qqqqWW_deg_1000_315_300", "baby", 0, "MakeBaby", -1, isoCase); 
    if (babiesToMake[13] == 1) l->ScanChain(chain_T5qqqqWW_1200_1000_800   , "t5qqqqWW_1200_1000_800"   , "baby", 0, "MakeBaby", -1, isoCase); 
    if (babiesToMake[14] == 1) l->ScanChain(chain_sync                     , "sync"                     , "baby", 0, "MakeBaby", -1, isoCase);
  }
 
  return 0;

}
