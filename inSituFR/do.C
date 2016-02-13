{
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../CORE/Tools/dorky/dorky.cc+"); 
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/PlotMaker2D.cc+");
  gROOT->ProcessLine(".L ../classFiles/v6.02/SS.cc+");
  gROOT->ProcessLine(".L FR.C+");
  gROOT->ProcessLine("FR()");
}
