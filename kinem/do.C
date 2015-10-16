{

  gSystem->Load("../CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L ../CORE/Tools/dorky/dorky.cc+");
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+"); 
  gROOT->ProcessLine(".L ../classFiles/v4.00/SS.cc+");
  gROOT->ProcessLine(".L plots.C+");
  gROOT->ProcessLine("plots()"); 

}
