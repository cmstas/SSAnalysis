{
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gSystem->Load("../../software/tableMaker/libSimpleTable.so");
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L closure.C+");
  gROOT->ProcessLine("closure()");
}
