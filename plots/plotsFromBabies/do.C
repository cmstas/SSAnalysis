{
  gROOT->ProcessLine(".L /home/users/cgeorge/software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L /home/users/cgeorge/SSAnalysis/classFiles/v6.02/SS.cc+");
  gROOT->ProcessLine(".L /home/users/cgeorge/CORE/Tools/dorky/dorky.cc+"); 
  gSystem->Load("/home/users/cgeorge/software/tableMaker/libSimpleTable.so"); 
  gROOT->ProcessLine(".L plots.C+");
  gROOT->ProcessLine("plots()");
}
