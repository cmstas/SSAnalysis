{

  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L ../../classFiles/v8.04/SS.cc+");
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../CORE/Tools/dorky/dorky.cc+"); 
  gSystem->Load("../../software/tableMaker/libSimpleTable.so"); 

  gROOT->ProcessLine(".L plots.C+");
  gROOT->ProcessLine("plots()");
}
