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

  bool doConeCorr = 1;
  bool doNoSIP    = 1;
  bool doBonly    = 0;
  bool doPtRel    = 0;

  TString option = "";
  if (doPtRel) option+="_ptRel";//option only for fakeratefile
  if (doNoSIP) option+="_noSIP";
  if (doBonly) option+="_doBonly";

  TString fakeratefile = "../measurement_region/rate_histos_qcd"+option+".root";
  // TString fakeratefile = "../measurement_region/rate_histos_ttbar.root";

  if (doConeCorr) option+="_coneCorr";//option only for ScanChain

  TChain *ch = new TChain("t"); 
  ch->Add("/home/users/cerati/SSAnalysis/SSAnalysis/babies/v1.04/ttbar_baby.root"); //this one!
  if (doPtRel) ch->Add("/home/users/cerati/SSAnalysis/SSAnalysis/babies/v1.04/ttbar_baby_ptRel.root"); //or this one!

  ScanChain(ch, fakeratefile, option); 
}
