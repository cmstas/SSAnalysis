{

  gROOT->ProcessLine(".L ../../../software/dataMCplotMaker/dataMCplotMaker.cc++");
  gROOT->ProcessLine(".L ../../../classFiles/v4.00/SS.cc+");
  gROOT->ProcessLine(".L Flip.C+");
  gROOT->ProcessLine("flip()"); 

}
