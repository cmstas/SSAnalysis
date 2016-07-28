{

  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L ../../classFiles/v6.02/SS.cc+");
  gROOT->ProcessLine(".L ../../CORE/Tools/dorky/dorky.cc+"); 
  gSystem->Load("../../software/tableMaker/libSimpleTable.so"); 

  gROOT->ProcessLine(".L plots.C+");
  gROOT->ProcessLine("plots()");
}
