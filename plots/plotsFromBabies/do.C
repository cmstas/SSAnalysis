{

  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L ../../classFiles/v8.04/SS.cc+");
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../CORE/Tools/dorky/dorky.cc+"); 
  gSystem->Load("../../software/tableMaker/libSimpleTable.so"); 

  gROOT->ProcessLine(".L plots.C+");
  // gROOT->ProcessLine(".L test.C+");
  gROOT->ProcessLine("plots()");

  // gROOT->ProcessLine(".L sip3d_plots.C+");
  // gROOT->ProcessLine("sip3d_plots()");

}
