{
  gROOT->ProcessLine(".L SYST.cc+"); 
  gSystem->Load("../../../CORE/CMS3_CORE.so");
  gSystem->Load("../../../Tools/libTools.so");
  gROOT->ProcessLine(".L analysis.C++"); 
  gROOT->ProcessLine("analysis()"); 
}
