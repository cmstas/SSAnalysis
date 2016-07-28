{
  // gROOT->ProcessLine(".L ../../classFiles/v7.00/SS.cc+"); 
  gROOT->ProcessLine(".L ../../classFiles/v8.02/SS.cc+"); 
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../CORE/Tools/dorky/dorky.cc+");
  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+"); 
  // gROOT->ProcessLine(".L ../../hlt/HLTEfficiency.cc+"); 
  gSystem->Load("../../software/tableMaker/libSimpleTable.so"); 
  gROOT->ProcessLine(".L yieldMaker.C++"); 
  gROOT->ProcessLine("getyields()"); 
}
