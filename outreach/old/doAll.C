{
  gSystem->Load("~/CORE/libCMS2NtupleMacrosCORE.so");
  gROOT->ProcessLine(".L gen_functions.cc++");
  gROOT->ProcessLine(".L looper.C++");
  
  TChain *ch = new TChain("Events"); 
  babyMaker *looper = new babyMaker();

  //ch->Add("/hadoop/cms/store/group/snt/papers2012/Summer12_53X_MC/SMS-T1tttt_Mgluino-350to1200_mLSP-0to850_8TeV-Pythia6Z_StoreResults-PU_START52_V9_FastSim-v1/V05-03-23/merged_ntuple_1.root");
 for (int i = 60; i < 67; i++){
    //ch->Add(Form("/hadoop/cms/store/group/snt/papers2012/Summer12_53X_MC/SMS-T1tttt_Mgluino-350to1200_mLSP-0to850_8TeV-Pythia6Z_StoreResults-PU_START52_V9_FastSim-v1/V05-03-23/merged_ntuple_%i.root",i));
 }
    ch->Add("/hadoop/cms/store/group/snt/papers2012/Summer12_53X_MC/SMS-T1tttt_Mgluino-350to1200_mLSP-0to850_8TeV-Pythia6Z_StoreResults-PU_START52_V9_FastSim-v1/V05-03-23/merged_ntuple_*.root");
   // ch->Add("/hadoop/cms/store/group/snt/papers2012/Summer12_53X_MC/SMS-T1tttt_Mgluino-350to1200_mLSP-0to850_8TeV-Pythia6Z_StoreResults-PU_START52_V9_FastSim-v1/V05-03-23/merged_ntuple_1.root");
   // ch->Add("/hadoop/cms/store/group/snt/papers2012/Summer12_53X_MC/SMS-T4tW_Msbottom-325to700_mChargino-150to625_8TeV-Madgraph_Summer12-START52_V9_FSIM/V05-03-27/merged_ntuple_1*.root");
  looper->ScanChain(ch, "T1tttt_all", -1);
}
