{
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gSystem->Load("../../software/tableMaker/libSimpleTable.so");
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L ../FO4.cc+");
  gROOT->ProcessLine(".L ../PC.cc+");
  gROOT->ProcessLine(".L ../threesig_nocont.cc+");
  gROOT->ProcessLine(".L closure.C+");
  gROOT->ProcessLine("closure()");
}
