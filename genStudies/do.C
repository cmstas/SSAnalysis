{

  //gROOT->ProcessLine(".L /home/users/cgeorge/CORE/CMS3.cc+"); 
  //gROOT->ProcessLine(".L /home/users/cgeorge/CORE/Tools/utils.cc+"); 
  gSystem->Load("/home/users/cgeorge/CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L /home/users/cgeorge/software/dataMCplotMaker/PlotMaker2D.cc+"); 
  gROOT->ProcessLine(".L genStudies.C+"); 
  gROOT->ProcessLine("genStudies()"); 

}
