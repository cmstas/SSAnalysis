{

  gSystem->Load("~/CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc++"); 
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L plots.C++");
  gROOT->ProcessLine("plots()"); 

}
