{

  gROOT->ProcessLine(".L /home/users/cgeorge/software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L SL.cc+");
  gROOT->ProcessLine(".L triggerStudy.C+");
  gROOT->ProcessLine("triggerStudy()");

}
