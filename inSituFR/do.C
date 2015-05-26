{
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc++");
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/PlotMaker2D.cc++");
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L SS.cc++");
  gROOT->ProcessLine(".L FR.C++");
  gROOT->ProcessLine("FR()");
}
