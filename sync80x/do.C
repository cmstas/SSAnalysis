{
  // gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  // gSystem->Load("../software/tableMaker/libSimpleTable.so");
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../classFiles/v8.05/SS.cc+");
  // gROOT->ProcessLine(".L ../CORE/Tools/dorky/dorky.cc+");
  gROOT->ProcessLine(".L yields.C+");
  gROOT->ProcessLine("yields()");

  // gROOT->ProcessLine(".L compare_74_76.C+");
  // gROOT->ProcessLine("compare()");
}
