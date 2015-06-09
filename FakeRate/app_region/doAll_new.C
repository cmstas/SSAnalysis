{
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gSystem->Load("../../Tools/libTools.so");

  gROOT->ProcessLine(".L lepfilter.cc+");
  gROOT->ProcessLine(".L ScanChain.C+");

  bool doConeCorr   = 1;
  bool doJetCorr    = 0;
  bool doLooseEMVA  = 0;
  bool doPtRatioCor = 0;
  bool doBonly      = 0;
  bool doConly      = 0;
  bool doLightonly  = 0;

  bool highhigh   = 1;
  bool highlow    = 0;
  bool lowlow     = 0;
  
  TString option = "";
  if (doLooseEMVA) option+="_LooseEMVA";
  if (doPtRatioCor) option+="_PtRatioCor";
  if (doBonly) option+="_doBonly";
  if (doConly) option+="_doConly";
  if (doLightonly) option+="_doLightonly";

  TString ptRegion = "";
  if (highhigh) ptRegion = "HH";
  else if (highlow)  ptRegion = "HL";
  else if (lowlow)   ptRegion = "LL";

  TString fakeratefile = "../measurement_region/rate_histos_qcd"+option+".root";

  if (doConeCorr) option+="_coneCorr";     //option only for ScanChain
  else if (doJetCorr) option+="_jetCorr";  //option only for ScanChain

  TChain *ch = new TChain("t"); 
  if (doLooseEMVA) ch->Add("../../babies/v1.16_looseEMVA/TTBAR.root");
  else ch->Add("../../babies/v1.21/TTBAR.root");
  ScanChain(ch, fakeratefile, option, ptRegion); 

  //TChain *ch_wjets = new TChain("t"); 
  //ch_wjets->Add("/nfs-7/userdata/ss2015/ssBabies/v1.04/Wjets_baby.root"); //this one!
}
