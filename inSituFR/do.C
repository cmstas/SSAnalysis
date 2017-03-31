{
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../CORE/Tools/dorky/dorky.cc+"); 
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/PlotMaker2D.cc+");
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L ../classFiles/v8.04/SS.cc+");
  gROOT->ProcessLine(".L FR.C+");

  // FR();

  // TString opts = "";
  // // opts = "doSoup";
  // opts = "doData";
  // // opts = "doPromptsOnly";
  // FR(0, opts); // do low HT
  // FR(1, opts); // do high HT
  // prompt_corrected_FR(0); // do low HT
  // prompt_corrected_FR(1); // do high HT
  // FR(-1, "doSoup,test_MVA");
  // FR(0, "doSoup,test_MVA");
  // FR(1, "doSoup,test_MVA");
  // // FR(-1, "doData");
  // FR(0, "doData,doExtraElectronPtBin");
  // FR(1, "doData,doExtraElectronPtBin");

  // // FR(-1, "doSoup,test_MVA,doExtraElectronPtBin");
  // FR(-1, "doSoup,doExtraElectronPtBin");
  // FR(0, "doSoup,doExtraElectronPtBin");
  // FR(1, "doSoup,doExtraElectronPtBin");

  // // FR(-1, "doSoup,test_MVA,doExtraElectronPtBin");
  // FR(-1, "test_MVA,doExtraElectronPtBin");
  // // FR(-1, "test_MVA,doSoup,doExtraElectronPtBin");

  // // Recipe to get inclusive, and low/high ht fake rate maps from MC
  // for(int doHighHT = -1; doHighHT < 2; doHighHT++) {
  //     FR(doHighHT, "doSoup,test_MVA,doExtraElectronPtBin");
  // }

  // Recipe to calculate promptCorrected FR for data with extra electron PtBin
  for(int doHighHT = -1; doHighHT < 2; doHighHT++) {
      FR(doHighHT, "doPromptsOnly,test_MVA,doExtraElectronPtBin");
      FR(doHighHT, "doData,test_MVA,doExtraElectronPtBin");
      prompt_corrected_FR(doHighHT);
  }



}
