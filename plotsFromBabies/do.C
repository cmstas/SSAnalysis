{

  gROOT->ProcessLine(".L /home/users/cgeorge/software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L contour.C+");
  gROOT->ProcessLine(".L plots.C+");
  gROOT->ProcessLine("plots()");

}
