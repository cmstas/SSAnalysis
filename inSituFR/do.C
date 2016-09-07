{
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../CORE/Tools/dorky/dorky.cc+"); 
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/PlotMaker2D.cc+");
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L ../classFiles/v8.04/SS.cc+");
  gROOT->ProcessLine(".L FR.C+");

  // FR();

  TString opts = "";
  // opts = "doPromptsOnly";
  FR(0, opts); // do low HT
  FR(1, opts); // do high HT

  // prompt_corrected_FR(0); // do low HT
  // prompt_corrected_FR(1); // do high HT





  // // Recipe to calculate promptCorrected FR
  // for(int doHighHT = 0; doHighHT < 2; doHighHT++) {
  //     FR(doHighHT, "doPromptsOnly");
  //     FR(doHighHT, "doData");
  //     prompt_corrected_FR(doHighHT);
  // }



}
