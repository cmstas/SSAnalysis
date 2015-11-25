{

  gSystem->Load("../CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L OUT.cc+");
  gROOT->ProcessLine(".L outreachPlots.C+");
  gROOT->ProcessLine("outreachPlots()"); 

}
