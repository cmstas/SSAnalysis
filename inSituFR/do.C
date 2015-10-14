{
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/PlotMaker2D.cc+");
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L FR.C+");
  gROOT->ProcessLine("FR()");
}
