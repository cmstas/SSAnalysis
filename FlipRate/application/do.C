{

  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+"); 
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L closure.C+");
  gROOT->ProcessLine("closure()"); 

}
