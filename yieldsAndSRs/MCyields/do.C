{
  gROOT->ProcessLine(".L SS.cc+"); 
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../CORE/Tools/dorky/dorky.cc+");
  gSystem->Load("../../software/tableMaker/libSimpleTable.so"); 
  gROOT->ProcessLine(".L yieldMaker.C++"); 
  gROOT->ProcessLine("getyields()"); 
}
