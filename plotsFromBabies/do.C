{
  gROOT->ProcessLine(".L /home/users/cgeorge/software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L plots.C+");
  gROOT->ProcessLine("plots()");
}
