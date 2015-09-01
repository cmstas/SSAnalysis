{

  gROOT->ProcessLine(".L /home/users/cgeorge/CORE/Tools/dorky/dorky.cc+");
  gROOT->ProcessLine(".L ../../../software/dataMCplotMaker/dataMCplotMaker.cc+"); 
  gROOT->ProcessLine(".L SS.cc+");
  gROOT->ProcessLine(".L compare_74x_75x.C++");
  gROOT->ProcessLine("compare_74x_75x()"); 

}
