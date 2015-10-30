{

  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc++"); 
  gSystem->Load("../../CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L ../../classFiles/v4.03/SS.cc+");
  gROOT->ProcessLine(".L JEC_unc.C+");
  gROOT->ProcessLine("JEC_unc()"); 

}
