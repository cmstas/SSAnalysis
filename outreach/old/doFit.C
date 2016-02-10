{
  gSystem->Load("~/CORE/libCMS2NtupleMacrosCORE.so");
  gROOT->ProcessLine(".L eff.cc+");
  gROOT->ProcessLine(".L ScaleFactors.cc++");
  gROOT->ProcessLine(".L TDRStyle.cc++");
  gROOT->ProcessLine("rt::SetTDRStyle()");
  gROOT->ProcessLine(".L fit.C++");
  //gROOT->ProcessLine("eff_curve(\"T1tttt.root\")");
  gROOT->ProcessLine("eff_curve(\"T1tttt_all.root\")"); //use this to make jet pt figure
  //gROOT->ProcessLine("eff_curve(\"T1tttt_big.root\")"); //use this to make the other ones
  
}
