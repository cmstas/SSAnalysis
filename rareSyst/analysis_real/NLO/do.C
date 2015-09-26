{
  gROOT->ProcessLine(".L SYST.cc+"); 
  gSystem->Load("../../../CORE/CMS3_CORE.so");
  gSystem->Load("../../../software/tableMaker/libSimpleTable.so"); 
  gROOT->ProcessLine(".L analysis.C++"); 
  gROOT->ProcessLine("analysis()"); 
}
