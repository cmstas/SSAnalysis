{
  gSystem->Load("~/CORE/libCMS2NtupleMacrosCORE.so");
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L fits.cc++");
  gROOT->ProcessLine(".L ScaleFactors.cc++");
  gROOT->ProcessLine(".L closure.C++");
  //gROOT->ProcessLine("closure(\"T1tttt_big.root\", -1, -1, false)");
  gROOT->ProcessLine("closure(\"T6ttww.root\")");
  
}
