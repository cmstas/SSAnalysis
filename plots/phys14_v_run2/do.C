{

  gROOT->ProcessLine(".L ../../../software/dataMCplotMaker/dataMCplotMaker.cc++"); 
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L compare_phys14_run2.C++");
  gROOT->ProcessLine("compare_phys14_run2()"); 

}
