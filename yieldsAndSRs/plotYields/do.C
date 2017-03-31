{

  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+");
  //gSystem->Load("../software/tableMaker/libSimpleTable.so");
  gROOT->ProcessLine(".L ../../CORE/Tools/dorky/dorky.cc++");
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../classFiles/v6.02/SS.cc+");

  // gROOT->ProcessLine(".L ratios.C++");
  // ratios(0, "doTTZ,doCompareNLOwithLO");
  // ratios(0, "doCompareNLOwithLO");
  // ratios(0, "doTTZ");
  // ratios(0, "");

  // gROOT->ProcessLine(".L ratios_wz.C++");
  // ratios(0, "");
  gROOT->ProcessLine(".L ratios.C++");
  // ratios(0, "--doTTZ");
  // ratios(0, "--doTTW");
  ratios(0, "--doWZ");

  // gROOT->ProcessLine(".L yields.C++");
  // gROOT->ProcessLine("yields()");
  
}
