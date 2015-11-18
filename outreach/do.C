{

  gSystem->Load("../CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L /home/users/cgeorge/software/makeCMS3ClassFiles/OUT.cc+");
  gROOT->ProcessLine(".L outreachPlots.C+");
  gROOT->ProcessLine("outreachPlots()"); 

}
