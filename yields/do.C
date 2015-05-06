{

  gROOT->ProcessLine(".L /home/users/cgeorge/software/dataMCplotMaker/dataMCplotMaker.cc+");
  gSystem->Load("/home/users/cgeorge/CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L yields.C+");
  gROOT->ProcessLine("yields()");
}
