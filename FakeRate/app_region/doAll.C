{
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gSystem->Load("../../Tools/libTools.so");

  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L ScanChain.C++");

  bool doInSitu = 1;

  bool highhigh   = 1;
  bool highlow    = 0;
  bool lowlow     = 0;

  //These are only for not-in-situ
  bool doConeCorr   = 1;
  bool doJetCorr    = 0;
  bool doLooseEMVA  = 0;
  bool doPtRatioCor = 0;
  bool doInvPtRatio = 0;
  bool doBonly      = 0;
  bool doConly      = 0;
  bool doLightonly  = 0;

  TString fakeratefile = "";
  TString option = "";

  if (!doInSitu){
    if (doLooseEMVA) option+="_LooseEMVA";
    if (doPtRatioCor) option+="_PtRatioCor";
    if (doInvPtRatio) option+="_InvPtRatio";
    if (doBonly) option+="_doBonly";
    if (doConly) option+="_doConly";
    if (doLightonly) option+="_doLightonly";
    fakeratefile = "../measurement_region/rate_histos_qcd"+option+".root";
  }
  else {
    fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos.root";
    //fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_notCC.root";
    //fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_PC.root";
    //fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_soup.root";
    //fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_ssZ.root";
    //fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_PCssZ.root";
    //fakeratefile = "../../inSituFR/inSituFR_cone_FR_histos_FO2pFO4.root";
    option += "inSitu"; 
  }

  TString ptRegion = "";
  if (highhigh) ptRegion = "HH";
  else if (highlow)  ptRegion = "HL";
  else if (lowlow)   ptRegion = "LL";

  if (doConeCorr) option+="_coneCorr";     //option only for ScanChain
  else if (doJetCorr) option+="_jetCorr";  //option only for ScanChain

  TChain *ch = new TChain("t"); 
  if (doLooseEMVA) ch->Add("../../babies/v1.16_looseEMVA/TTBAR.root");
  else ch->Add("../../babies/v1.21/TTBAR.root");
  ScanChain(ch, fakeratefile, option, ptRegion); 

  //TChain *ch_wjets = new TChain("t"); 
  //ch_wjets->Add("/nfs-7/userdata/ss2015/ssBabies/v1.04/Wjets_baby.root"); //this one!
}
