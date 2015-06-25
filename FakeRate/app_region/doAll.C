{
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gSystem->Load("../../Tools/libTools.so");
  gSystem->Load("../../software/tableMaker/libSimpleTable.so");

  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L ScanChain.C++");

  bool doInSitu = 1;

  bool highhigh   = 1;
  bool highlow    = 0;
  bool lowlow     = 0;

  //These are only for not-in-situ
  bool doConeCorr   = 1;
  bool doJetCorr    = 0;
  bool doPtRatioCor = 0;
  bool doBonly      = 0;
  bool doConly      = 0;
  bool doLightonly  = 0;

  //For both inSitu and not-in-Situ
  bool doLooseEMVA  = 1;

  //These are only for inSitu (choose only one of these)
  bool soup         = 0;
  bool PC           = 0;   
  bool ssZ          = 0;
  bool PCssZ        = 0;
  bool notCC        = 0;

  TString fakeratefile = "";
  TString option = "";

  if (doLooseEMVA) option+="_LooseEMVA";

  if (!doInSitu){
    if (doPtRatioCor) option+="_PtRatioCor";
    if (doBonly) option+="_doBonly";
    if (doConly) option+="_doConly";
    if (doLightonly) option+="_doLightonly";
    fakeratefile = "../measurement_region/rate_histos_qcd"+option+".root";
  }
  else {
    if (soup) option += "_soup";
    if (PC) option += "_PC";
    if (ssZ) option += "_ssZ";
    if (PCssZ) option += "_PCssZ";
    if (notCC) option += "_notCC";
    if (soup) fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_soup.root";
    else if (PC) fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_PC.root";
    else if (ssZ) fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_ssZ.root";
    else if (PCssZ) fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_PCssZ.root";
    else if (notCC) fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_notCC.root";
    else if (doLooseEMVA) fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_FO2pFO4.root";
    else fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos.root";
    option += "_inSitu"; 
  }

  TString ptRegion = "";
  if (highhigh) ptRegion = "HH";
  else if (highlow)  ptRegion = "HL";
  else if (lowlow)   ptRegion = "LL";

  if (doConeCorr) option+="_coneCorr";     //option only for ScanChain
  else if (doJetCorr) option+="_jetCorr";  //option only for ScanChain

  TChain *ch = new TChain("t"); 
  if (doLooseEMVA) ch->Add("../../babies/v1.26/TTBAR_1.root");
  else ch->Add("../../babies/v1.26/TTBAR_0.root");
  ScanChain(ch, fakeratefile, option, ptRegion); 

  //TChain *ch_wjets = new TChain("t"); 
  //ch_wjets->Add("/nfs-7/userdata/ss2015/ssBabies/v1.04/Wjets_baby.root"); //this one!
}
