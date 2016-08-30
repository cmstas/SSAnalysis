{
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../CORE/Tools/dorky/dorky.cc+"); 
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/PlotMaker2D.cc+");
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L ../classFiles/v8.04/SS.cc+");
  gROOT->ProcessLine(".L FR.C+");
  cout << "remember to enable other 2" << endl;
  cout << "remember to enable other 2" << endl;
  cout << "remember to enable other 2" << endl;
  cout << "remember to enable other 2" << endl;
  cout << "make sure sip3d inequalities are in right directions" << endl;
  cout << "make sure sip3d inequalities are in right directions" << endl;
  cout << "make sure sip3d inequalities are in right directions" << endl;
  cout << "make sure sip3d inequalities are in right directions" << endl;
  // FR();
  FR(0); // do low HT
  FR(1); // do high HT
}
