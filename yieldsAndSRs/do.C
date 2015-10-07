{

  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  //gSystem->Load("../software/tableMaker/libSimpleTable.so");
  gROOT->ProcessLine(".L ../CORE/Tools/dorky/dorky.cc++");
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L yields.C++");
  gROOT->ProcessLine("yields()");
}
