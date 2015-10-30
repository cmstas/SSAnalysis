{
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gSystem->Load("../software/tableMaker/libSimpleTable.so");
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../classFiles/v4.03/SS.cc+");
  gROOT->ProcessLine(".L ../CORE/Tools/dorky/dorky.cc+");
  gROOT->ProcessLine(".L yields.C+");
  gROOT->ProcessLine("yields()");
}
