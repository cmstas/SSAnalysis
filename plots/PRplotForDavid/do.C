{

  //gSystem->Load("/home/users/cgeorge/CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L /home/users/cgeorge/CORE/Tools/dorky/dorky.cc++");
  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+"); 
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L PRplot.C++");
  gROOT->ProcessLine("closure()"); 

}
