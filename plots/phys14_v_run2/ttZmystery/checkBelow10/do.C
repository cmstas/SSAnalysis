{
  gROOT->ProcessLine(".L SS.cc+"); 
  gROOT->ProcessLine(".L ../../../../software/dataMCplotMaker/dataMCplotMaker.cc++"); 
  gROOT->ProcessLine(".L checkTTZbelow10.C+");
  gROOT->ProcessLine("checkTTZbelow10()"); 
}
