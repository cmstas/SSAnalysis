{
  //not sure which ones I need
  gROOT->ProcessLine(".L ../../CORE/CMS3.cc+");
  gROOT->ProcessLine(".L ../../CORE/Base.cc+");
  gROOT->ProcessLine(".L ../../CORE/ElectronSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/MuonSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/MCSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/JetSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/IsolationTools.cc+");
  gROOT->ProcessLine(".L ../../CORE/SSSelections.cc+");

  gROOT->ProcessLine(".L ScanChain.C+");
  bool doConeCorr = 0;
  bool doNoSIP    = 1;
  bool doBonly    = 0;
  bool doConly    = 0;
  bool doLightonly= 0;
  bool doPtRel    = 0;
  bool doLowPtRel = 0;
  bool doExtrPtRel= 0;

  bool highhigh   = 1;
  bool highlow    = 0;
  bool lowlow     = 0;
  
  TString option = "";
  if (doPtRel) option+="_ptRel";//option only for fakeratefile
  if (doNoSIP) option+="_noSIP";
  if (doBonly) option+="_doBonly";
  if (doConly) option+="_doConly";
  if (doLightonly) option+="_doLightonly";
  if (doLowPtRel) option+="_lowPtRel";
  if (doExtrPtRel) option+="_extrPtRel";

  TString ptRegion = "";
  if (highhigh) ptRegion = "HH";
  else if (highlow)  ptRegion = "HL";
  else if (lowlow)   ptRegion = "LL";

  TString fakeratefile = "../measurement_region/rate_histos_qcd"+option+".root";
  // TString fakeratefile = "../measurement_region/rate_histos_qcdnonEnriched"+option+".root";
  // TString fakeratefile = "../measurement_region/rate_histos_ttbar.root"; //from applying FR to ttbar FR baby

  if (doConeCorr) option+="_coneCorr";//option only for ScanChain
  if (doExtrPtRel) doPtRel=1;

  TChain *ch = new TChain("t"); 
  if (doExtrPtRel) {
    ch->Add("/home/users/cerati/SSAnalysis/SSAnalysis/babies/v1.0X-ptrelfo6/ttbar_baby_ptRel.root"); //or this one!
  } else {
    if (doPtRel) ch->Add("/home/users/cerati/SSAnalysis/SSAnalysis/babies/v1.04/ttbar_baby_ptRel.root"); //or this one!
    else ch->Add("/home/users/cerati/SSAnalysis/SSAnalysis/babies/v1.04/ttbar_baby.root"); //this one!
  }

  TChain *ch_wjets = new TChain("t"); 
  if (doPtRel) ch_wjets->Add("/nfs-7/userdata/ss2015/ssBabies/v1.04/Wjets_baby_ptRel.root"); //or this one!
  else {
	ch_wjets->Add("/nfs-7/userdata/ss2015/ssBabies/v1.04/Wjets_baby.root"); //this one!
	ch_wjets->Add("/nfs-7/userdata/iandyckes/fake_rate_output/V00-00-05/wjetsHT_SSbaby.root");
	//ch_wjets->Add("./wjetsHT_baby.root");
  }
  ScanChain(ch, fakeratefile, option, ptRegion); 
  //ScanChain(ch_wjets, fakeratefile, option, ptRegion); 
}
