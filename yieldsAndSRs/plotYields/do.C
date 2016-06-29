{

  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+");
  //gSystem->Load("../software/tableMaker/libSimpleTable.so");
  gROOT->ProcessLine(".L ../../CORE/Tools/dorky/dorky.cc++");
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../classFiles/v6.02/SS.cc+");

  gROOT->ProcessLine(".L ratios.C++");
  // gROOT->ProcessLine("ratios(1)");
  gROOT->ProcessLine("ratios(0)");

  // gROOT->ProcessLine(".L yields.C++");
  // gROOT->ProcessLine("yields()");
  
}
