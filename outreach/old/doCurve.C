{
  gSystem->Load("~/CORE/libCMS2NtupleMacrosCORE.so");
  gROOT->ProcessLine(".L eff.cc+");
  gROOT->ProcessLine(".L eff_curve.C++");
  //gROOT->ProcessLine("eff_curve(\"T1tttt.root\")");
  gROOT->ProcessLine("eff_curve(\"T1tttt_big.root\")");
  //gROOT->ProcessLine("eff_curve(\"T1tttt_big_better.root\")");
  
}
