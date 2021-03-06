#include "TChain.h"
#include "looper.h"

int runMain(bool useSkim, bool skimAll, bool runAll, bool runLepEff, bool runSync, string runBaby_str, bool expt, std::vector<int> eventsToDebug){

  //Figure out which babies to make
  bool runBaby = false;
  bool babiesToMake[50] = {0};
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

  if (runSync) { 
    TChain *chain_synctest = new TChain("Events");
    chain_synctest->Add("./phys14_sync_CMS3.root");
    l->ScanChain(chain_synctest,"synctest","",0,"SyncTest",-1, eventsToDebug);
    return 0;
  }

  TChain *chain_T1ttttG1200                                                     = new TChain("Events");
  TChain *chain_T1ttttG1500                                                     = new TChain("Events");
  TChain *chain_T5qqqqWW1200                                                    = new TChain("Events");
  TChain *chain_T5qqqqWW1500                                                    = new TChain("Events");
  TChain *chain_T6ttWW600_150                                                   = new TChain("Events");
  TChain *chain_T6ttWW600_425                                                   = new TChain("Events");
  TChain *chain_TTJets                                                          = new TChain("Events");
  TChain *chain_TTWJets                                                         = new TChain("Events");
  TChain *chain_TTZJets                                                         = new TChain("Events");
  TChain *chain_WHZH                                                            = new TChain("Events");
  TChain *chain_WW                                                              = new TChain("Events");
  TChain *chain_WZJets                                                          = new TChain("Events");
  TChain *chain_DY                                                              = new TChain("Events");
  TChain *chain_TTbarH                                                          = new TChain("Events");
  TChain *chain_TW                                                              = new TChain("Events");
  TChain *chain_WJets                                                           = new TChain("Events");
  TChain *chain_Wjets_ht                                                        = new TChain("Events");
  TChain *chain_ZZTo4L                                                          = new TChain("Events");
  TChain *chain_Wjets                                                           = new TChain("Events");
  TChain *chain_sync                                                            = new TChain("Events");
  TChain *chain_T1ttbb_2J_mGo1500_mChi100_3bodydec_asymmDecOnly                 = new TChain("Events");
  TChain *chain_T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec_v2                  = new TChain("Events");
  TChain *chain_T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec_v2                  = new TChain("Events");
  TChain *chain_T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec_v2                  = new TChain("Events");
  TChain *chain_T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec_v2                  = new TChain("Events");
  TChain *chain_T5Full_Gl1500_Chi800_LSP100                                     = new TChain("Events");
  TChain *chain_T5qqqqWW_1200_1000_800                                          = new TChain("Events");
  TChain *chain_T5qqqqWW_deg_1000_315_300                                       = new TChain("Events");
  TChain *chain_T5qqqqWWDeg_mGo1000_mCh310_mChi300                              = new TChain("Events");
  TChain *chain_T5qqqqWWDeg_mGo1000_mCh315_mChi300                              = new TChain("Events");
  TChain *chain_T5qqqqWWDeg_mGo1000_mCh325_mChi300                              = new TChain("Events");
  TChain *chain_T5qqqqWWDeg_mGo800_mCh305_mChi300                               = new TChain("Events");
  TChain *chain_T5qqqqWW_mGo1000_mCh800_mChi700                                 = new TChain("Events");
  TChain *chain_T5qqqqWW_mGo1200_mCh1000_mChi800                                = new TChain("Events");
  TChain *chain_T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_23bodydec_v2          = new TChain("Events");
  TChain *chain_T5ttttDeg_mGo1000_mStop300_mChi280_4bodydec_v2                  = new TChain("Events");
  TChain *chain_T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_23bodydec_v2          = new TChain("Events");
  TChain *chain_T5ttttDeg_mGo1300_mStop300_mChi280_4bodydec_v2                  = new TChain("Events");
  TChain *chain_T6ttWW_600_425_50_v2                                            = new TChain("Events");
  TChain *chain_T6ttWW_650_150_50_v2                                            = new TChain("Events");
  TChain *chain_data_DoubleEG                                                   = new TChain("Events"); 
  TChain *chain_data_DoubleMuon                                                 = new TChain("Events"); 
  TChain *chain_data_MuonEG                                                     = new TChain("Events"); 
  TChain *chain_test                                                            = new TChain("Events");
  TChain *chain_05Aug                                                           = new TChain("Events");
  TChain *chain_sync_DoubleMuon                                                 = new TChain("Events");
  TChain *chain_sync_DoubleEG                                                   = new TChain("Events");
  TChain *chain_sync_MuonEG                                                     = new TChain("Events");
  TChain *chain_sync_TTW                                                        = new TChain("Events");
  TChain *chain_sync_TTG                                                        = new TChain("Events");

  TChain *chain_TTJets_50ns = new TChain("Events");
  chain_TTJets_50ns->Add("/hadoop/cms/store/group/snt/run2_50ns/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/V07-04-03/merged_ntuple_*.root");
  //l->ScanChain(chain_TTJets_50ns, "ttbar_50ns", "baby", 0, "MakeBaby", -1);
  //l->ScanChain(chain_TTJets,      "ttbar"       , "", 0, "", -1);

  TChain *chain_TTJets_25ns = new TChain("Events");
  //chain_TTJets_25ns->Add("/hadoop/cms/store/group/snt/run2_25ns/TT_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-03/merged_ntuple_*.root");
  chain_TTJets_25ns->Add("/hadoop/cms/store/group/snt/run2_25ns/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-03/merged_ntuple_*.root");
  // l->ScanChain(chain_TTJets_25ns, "ttbar_25ns", "baby", 0, "MakeBaby", -1);

  TChain *chain_WZJets_25ns = new TChain("Events");
  chain_WZJets_25ns->Add("/hadoop/cms/store/group/snt/run2_25ns/WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/V07-04-03/merged_ntuple_*.root"); 
  //l->ScanChain(chain_WZJets_25ns, "wz_25ns", "baby", 0, "MakeBaby", -1);
  //l->ScanChain(chain_WZJets,      "WZJets"      , "", 0, "", -1);

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
    TString dir = "/hadoop/cms/store/group/snt/run2_25ns/";
    TString dataDir = "/hadoop/cms/store/group/snt/run2_data/";
    TString tag = "V07-04-08/merged_ntuple_*.root";
    TString dataTag = "V07-04-08/merged_ntuple_*.root";

    //SMS
    chain_T1ttttG1200 ->Add(dir + "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/"    + tag);
    chain_T1ttttG1500 ->Add(dir + "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/"    + tag);

    //Main SM
    chain_TTJets  ->Add(dir + "TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/"                + tag);
    chain_TTWJets ->Add(dir + "TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/" + tag);
    chain_TTZJets ->Add(dir + "TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/"         + tag);
    chain_WZJets  ->Add(dir + "WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/"                                + tag); 
    chain_DY      ->Add(dir + "DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/"  + tag); 
    chain_DY      ->Add(dir + "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/"      + tag);
    chain_WHZH    ->Add(dir + "WH_ZH_HToWW_M-125_13TeV_pythia6/"                                                           + tag);
    chain_WW      ->Add(dir + "WWTo2L2Nu_CT10_13TeV-powheg-pythia8-tauola/"                                                + tag);
    chain_TTbarH  ->Add(dir + "TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/"          + tag);
    chain_ZZTo4L  ->Add(dir + "ZZTo4L_Tune4C_13TeV-powheg-pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"                      + tag);

    //W + Jets
    chain_Wjets   ->Add(dir + "WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/"           + tag);
    chain_Wjets_ht->Add(dir + "WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_Wjets_ht->Add(dir + "WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_Wjets_ht->Add(dir + "WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_Wjets_ht->Add(dir + "WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);

    //TW
    chain_TW ->Add(dir + "T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"           + tag);
    chain_TW ->Add(dir + "Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"        + tag);
    chain_TW ->Add(dir + "TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"    + tag);
    chain_TW ->Add(dir + "TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);
    chain_TW ->Add(dir + "TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/"    + tag);
    chain_TW ->Add(dir + "TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1/" + tag);

    //data
    //chain_data_DoubleEG   ->Add(dataDir + "Run2015C_DoubleEG_MINIAOD_PromptReco-v1/merged/V07-04-08/*.root"); 
    chain_data_DoubleEG   ->Add(dataDir + "Run2015D_DoubleEG_MINIAOD_PromptReco-v3/merged/V07-04-09/*.root"); 
    //chain_data_DoubleMuon ->Add(dataDir + "Run2015C_DoubleMuon_MINIAOD_PromptReco-v1/merged/V07-04-08/*.root"); 
    chain_data_DoubleMuon ->Add(dataDir + "Run2015D_DoubleMuon_MINIAOD_PromptReco-v3/merged/V07-04-09/*.root"); 
    //chain_data_MuonEG     ->Add(dataDir + "Run2015C_MuonEG_MINIAOD_PromptReco-v1/merged/V07-04-08/*.root"); 
    chain_data_MuonEG     ->Add(dataDir + "Run2015D_MuonEG_MINIAOD_PromptReco-v3/merged/V07-04-09/*.root"); 

    //test
    chain_test->Add(dir + "TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/V07-04-08/merged_ntuple_1.root"); 

    //Sync sample
    chain_sync->Add("/nfs-7/userdata/ss2015/sync_file/V07-02-08/sync_file.root");
    chain_sync_DoubleMuon->Add("/nfs-7/userdata/ss2015/sync_files_new/V07-04-08/DoubleMuon_MINIAOD_PromptReco-v3_000_257_531_00000_B26DC190-8166-E511-AE66-02163E0133A7.root");
    chain_sync_DoubleMuon->Add("/nfs-7/userdata/ss2015/sync_files_new/V07-04-08/DoubleMuon_MINIAOD_PromptReco-v3_000_257_599_00000_908F5705-8666-E511-8362-02163E01432A.root");
    chain_sync_DoubleMuon->Add("/nfs-7/userdata/ss2015/sync_files_new/V07-04-08/DoubleMuon_MINIAOD_PromptReco-v3_000_256_941_00000_10AA8DF0-0062-E511-B01C-02163E0146ED.root");
    //chain_sync_DoubleEG  ->Add("/nfs-7/userdata/ss2015/sync_files_new/V07-04-08/DoubleEG_MINIAOD_PromptReco-v3_000_257_531_00000_2CFA9A0A-0F66-E511-88FF-02163E013484.root");
    //chain_sync_DoubleEG  ->Add("/nfs-7/userdata/ss2015/sync_files_new/V07-04-08/DoubleEG_257599_4EF0C7A4-8E66-E511-AF95-02163E011C53.root");
    chain_sync_DoubleEG  ->Add("/hadoop/cms/store/group/snt/run2_data/Run2015D_DoubleEG_MINIAOD_PromptReco-v4/merged/V07-04-11/merged_ntuple_134.root");
    //chain_sync_MuonEG    ->Add("/nfs-7/userdata/ss2015/sync_files_new/V07-04-08/MuonEG_MINIAOD_PromptReco-v3_000_257_531_00000_D62608D5-FB65-E511-B031-02163E0145C1.root");
    chain_sync_MuonEG    ->Add("/hadoop/cms/store/group/snt/run2_data/Run2015D_MuonEG_MINIAOD_PromptReco-v4/merged/V07-04-11/merged_ntuple_24.root"); 
    chain_sync_TTW       ->Add("/nfs-7/userdata/ss2015/sync_files_new/V07-04-10/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_MINIAODSIM_74X_mcRun2_asymptotic_v2-v1_10000_3A2285C7-CA6D-E511-B68F-0CC47A13D09C.root");
    chain_sync_TTG       ->Add("/nfs-7/userdata/ss2015/sync_files_new/V07-04-11/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_MINIAODSIM_74X_mcRun2_asymptotic_v2-v1_60000_0235B538-676E-E511-8A03-001E67E6F855.root");

    //Private samples
    TString private_dir = "/nfs-7/userdata/ss2015/signalSamples/V07-02-08/";
    chain_T1ttbb_2J_mGo1500_mChi100_3bodydec_asymmDecOnly       ->Add(private_dir + "T1ttbb_2J_mGo1500_mChi100_3bodydec_asymmDecOnly/*.root"); 
    chain_T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec_v2        ->Add(private_dir + "T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec_v2/*.root");
    chain_T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec_v2        ->Add(private_dir + "T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec_v2/*.root");
    chain_T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec_v2        ->Add(private_dir + "T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec_v2/*.root");
    chain_T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec_v2        ->Add(private_dir + "T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec_v2/*.root");
    chain_T5Full_Gl1500_Chi800_LSP100                           ->Add(private_dir + "T5Full_Gl1500_Chi800_LSP100/*.root");
    chain_T5qqqqWW_1200_1000_800                                ->Add(private_dir + "T5qqqqWW_1200_1000_800/*.root");
    chain_T5qqqqWW_deg_1000_315_300                             ->Add(private_dir + "T5qqqqWW_deg_1000_315_300/*.root");
    chain_T5qqqqWWDeg_mGo1000_mCh310_mChi300                    ->Add(private_dir + "T5qqqqWWDeg_mGo1000_mCh310_mChi300/*.root");
    chain_T5qqqqWWDeg_mGo1000_mCh315_mChi300                    ->Add(private_dir + "T5qqqqWWDeg_mGo1000_mCh315_mChi300/*.root");
    chain_T5qqqqWWDeg_mGo1000_mCh325_mChi300                    ->Add(private_dir + "T5qqqqWWDeg_mGo1000_mCh325_mChi300/*.root");
    chain_T5qqqqWWDeg_mGo800_mCh305_mChi300                     ->Add(private_dir + "T5qqqqWWDeg_mGo800_mCh305_mChi300/*.root");
    chain_T5qqqqWW_mGo1000_mCh800_mChi700                       ->Add(private_dir + "T5qqqqWW_mGo1000_mCh800_mChi700/*.root");
    chain_T5qqqqWW_mGo1200_mCh1000_mChi800                      ->Add(private_dir + "T5qqqqWW_mGo1200_mCh1000_mChi800/*.root");
    chain_T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_23bodydec_v2->Add(private_dir + "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_23bodydec_v2/*.root");
    chain_T5ttttDeg_mGo1000_mStop300_mChi280_4bodydec_v2        ->Add(private_dir + "T5ttttDeg_mGo1000_mStop300_mChi280_4bodydec_v2/*.root");
    chain_T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_23bodydec_v2->Add(private_dir + "T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_23bodydec_v2/*.root");
    chain_T5ttttDeg_mGo1300_mStop300_mChi280_4bodydec_v2        ->Add(private_dir + "T5ttttDeg_mGo1300_mStop300_mChi280_4bodydec_v2/*.root");
    chain_T6ttWW_600_425_50_v2                                  ->Add(private_dir + "T6ttWW_600_425_50_v2/*.root");
    chain_T6ttWW_650_150_50_v2                                  ->Add(private_dir + "T6ttWW_650_150_50_v2/*.root");

    //05 Aug
    chain_05Aug->Add("/hadoop/cms/store/user/jgran/dataTuple/Run2015B_DoubleEG_MINIAOD_05Aug2015-v1/V07-05-00/*.root"); 
    chain_05Aug->Add("/hadoop/cms/store/user/jgran/dataTuple/Run2015B_DoubleMuon_MINIAOD_05Aug2015-v1/V07-05-00/*.root"); 

  }

  if (skimAll){

    //SMS
    l->ScanChain(chain_T1ttttG1200 , "T1ttttG1200",  "", 0, "SSskim",-1);
    l->ScanChain(chain_T1ttttG1500 , "T1ttttG1500",  "", 0, "SSskim",-1);
    l->ScanChain(chain_T5qqqqWW1200, "T5qqqqWW1200", "", 0, "SSskim",-1);
    l->ScanChain(chain_T5qqqqWW1500, "T5qqqqWW1500", "", 0, "SSskim",-1);

    //SM
    l->ScanChain(chain_TTWJets, "TTWJets", "", 0, "SSskim", -1);
    l->ScanChain(chain_TTZJets, "TTZJets", "", 0, "SSskim", -1);
    l->ScanChain(chain_WHZH   , "WHZH"   , "", 0, "SSskim", -1);
    l->ScanChain(chain_WW     , "WW"     , "", 0, "SSskim", -1);
    l->ScanChain(chain_WZJets , "WZJets" , "", 0, "SSskim", -1);
    l->ScanChain(chain_TTbarH , "TTbarH" , "", 0, "SSskim", -1);
    l->ScanChain(chain_WJets  , "WJets"  , "", 0, "SSskim", -1);
    l->ScanChain(chain_ZZTo4L , "ZZTo4L" , "", 0, "SSskim", -1);
    l->ScanChain(chain_TTJets , "TTJets" , "", 0, "SSskim", -1);
    l->ScanChain(chain_TW     , "TW"     , "", 0, "SSskim", -1);

  }

  if (runAll){
    l->ScanChain(chain_TTJets,      "ttbar"       , "", 0, "", -1);
    l->ScanChain(chain_TTWJets,     "TTWJets"     , "", 0, "", -1);
    l->ScanChain(chain_TTZJets,     "TTZJets"     , "", 0, "", -1);
    l->ScanChain(chain_WHZH,        "WHZH"        , "", 0, "", -1);
    l->ScanChain(chain_WW,          "WW"          , "", 0, "", -1);
    l->ScanChain(chain_WZJets,      "WZJets"      , "", 0, "", -1);
    l->ScanChain(chain_T1ttttG1200, "T1ttttG1200" , "", 0, "", -1);
    l->ScanChain(chain_T1ttttG1500, "T1ttttG1500" , "", 0, "", -1);
    l->ScanChain(chain_T5qqqqWW1200,"T5qqqqWW1200", "", 0, "", -1);
    l->ScanChain(chain_T5qqqqWW1500,"T5qqqqWW1500", "", 0, "", -1);
  }
  
  if (runLepEff){
    l->ScanChain(chain_DY          , "dy"           , "effic", 0, "DYtest", -1);
    l->ScanChain(chain_T1ttttG1200 , "T1ttttG1200"  , "effic", 0, "DYtest", -1);
    l->ScanChain(chain_T1ttttG1500 , "T1ttttG1500"  , "effic", 0, "DYtest", -1);
    l->ScanChain(chain_T5qqqqWW1200, "T5qqqqWW1200" , "effic", 0, "DYtest", -1);
    l->ScanChain(chain_T5qqqqWW1500, "T5qqqqWW1500" , "effic", 0, "DYtest", -1);
    l->ScanChain(chain_TTWJets     , "TTWJets"      , "effic", 0, "DYtest", -1);
  }

  if (runBaby){
    if (babiesToMake[0]  == 1) l->ScanChain(chain_TTZJets                  , "ttz"                      , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[1]  == 1) l->ScanChain(chain_WZJets                   , "wz"                       , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[2]  == 1) l->ScanChain(chain_T1ttttG1200              , "t1tttt_1200_800"          , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[3]  == 1) l->ScanChain(chain_T1ttttG1500              , "t1tttt_1500_100"          , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[4]  == 1) l->ScanChain(chain_TTJets                   , "ttbar"                    , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[5]  == 1) l->ScanChain(chain_TTWJets                  , "ttw"                      , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[6]  == 1) l->ScanChain(chain_T5qqqqWW1200             , "t5qqqqWW1200"             , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[7]  == 1) l->ScanChain(chain_T5qqqqWW1500             , "t5qqqqWW1500"             , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[8]  == 1) l->ScanChain(chain_T6ttWW600_150            , "t6ttWW600_150"            , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[9]  == 1) l->ScanChain(chain_T6ttWW600_425            , "t6ttWW600_425"            , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[10] == 1) l->ScanChain(chain_Wjets                    , "Wjets"                    , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[11] == 1) l->ScanChain(chain_Wjets_ht                 , "Wjets_ht"                 , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[12] == 1) l->ScanChain(chain_sync                     , "sync"                     , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[13] == 1) l->ScanChain(chain_T1ttbb_2J_mGo1500_mChi100_3bodydec_asymmDecOnly       , "T1ttbb_2J_mGo1500_mChi100_3bodydec_asymmDecOnly", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[14] == 1) l->ScanChain(chain_T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec_v2        , "T1ttbbWW_2J_mGo1000_mCh725_mChi715_3bodydec_v2" , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[15] == 1) l->ScanChain(chain_T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec_v2        , "T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec_v2" , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[16] == 1) l->ScanChain(chain_T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec_v2        , "T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec_v2" , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[17] == 1) l->ScanChain(chain_T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec_v2        , "T1ttbbWW_2J_mGo1300_mCh300_mChi295_3bodydec_v2" , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[18] == 1) l->ScanChain(chain_T5Full_Gl1500_Chi800_LSP100                           , "T5Full_Gl1500_Chi800_LSP100", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[19] == 1) l->ScanChain(chain_T5qqqqWW_1200_1000_800                                , "T5qqqqWW_1200_1000_800", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[20] == 1) l->ScanChain(chain_T5qqqqWW_deg_1000_315_300                             , "T5qqqqWW_deg_1000_315_300", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[21] == 1) l->ScanChain(chain_T5qqqqWWDeg_mGo1000_mCh310_mChi300                    , "T5qqqqWWDeg_mGo1000_mCh310_mChi300", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[22] == 1) l->ScanChain(chain_T5qqqqWWDeg_mGo1000_mCh315_mChi300                    , "T5qqqqWWDeg_mGo1000_mCh315_mChi300", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[23] == 1) l->ScanChain(chain_T5qqqqWWDeg_mGo1000_mCh325_mChi300                    , "T5qqqqWWDeg_mGo1000_mCh325_mChi300", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[24] == 1) l->ScanChain(chain_T5qqqqWWDeg_mGo800_mCh305_mChi300                     , "T5qqqqWWDeg_mGo800_mCh305_mChi300", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[25] == 1) l->ScanChain(chain_T5qqqqWW_mGo1000_mCh800_mChi700                       , "T5qqqqWW_mGo1000_mCh800_mChi700", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[26] == 1) l->ScanChain(chain_T5qqqqWW_mGo1200_mCh1000_mChi800                      , "T5qqqqWW_mGo1200_mCh1000_mChi800", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[27] == 1) l->ScanChain(chain_T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_23bodydec_v2, "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_23bodydec_v2", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[28] == 1) l->ScanChain(chain_T5ttttDeg_mGo1000_mStop300_mChi280_4bodydec_v2        , "T5ttttDeg_mGo1000_mStop300_mChi280_4bodydec_v2", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[29] == 1) l->ScanChain(chain_T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_23bodydec_v2, "T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_23bodydec_v2", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[30] == 1) l->ScanChain(chain_T5ttttDeg_mGo1300_mStop300_mChi280_4bodydec_v2        , "T5ttttDeg_mGo1300_mStop300_mChi280_4bodydec_v2", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[31] == 1) l->ScanChain(chain_T6ttWW_600_425_50_v2                                  , "T6ttWW_600_425_50_v2", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[32] == 1) l->ScanChain(chain_T6ttWW_650_150_50_v2                                  , "T6ttWW_650_150_50_v2", "baby", 0, "MakeBaby", -1);
    if (babiesToMake[33] == 1) l->ScanChain(chain_test                                                  , "test"                , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[34] == 1) l->ScanChain(chain_data_DoubleEG                                         , "data_DoubleEG"     , "baby", 1, "MakeBaby", -1); 
    if (babiesToMake[35] == 1) l->ScanChain(chain_TTJets_50ns                                           , "TTJets_50ns"       , "baby", 0, "MakeBaby", -1); 
    if (babiesToMake[36] == 1) l->ScanChain(chain_05Aug                                                 , "data05Aug"         , "baby", 0, "MakeBaby", -1); 
    if (babiesToMake[37] == 1) l->ScanChain(chain_data_DoubleMuon                                       , "data_DoubleMuon"   , "baby", 1, "MakeBaby", -1); 
    if (babiesToMake[38] == 1) l->ScanChain(chain_data_MuonEG                                           , "data_MuonEG"       , "baby", 1, "MakeBaby", -1); 
    if (babiesToMake[39] == 1) l->ScanChain(chain_DY                       , "dy"                       , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[40] == 1) l->ScanChain(chain_sync_DoubleMuon          , "sync_DoubleMuon"          , "baby", 1, "MakeBaby", -1);
    if (babiesToMake[41] == 1) l->ScanChain(chain_sync_DoubleEG            , "sync_DoubleEG"            , "baby", 1, "MakeBaby", -1);
    if (babiesToMake[42] == 1) l->ScanChain(chain_sync_MuonEG              , "sync_MuonEG"              , "baby", 1, "MakeBaby", -1);
    if (babiesToMake[43] == 1) l->ScanChain(chain_sync_TTW                 , "sync_TTW"                 , "baby", 0, "MakeBaby", -1);
    if (babiesToMake[44] == 1) l->ScanChain(chain_sync_TTG                 , "sync_TTG"                 , "baby", 0, "MakeBaby", -1);


  }
 
  return 0;

}
