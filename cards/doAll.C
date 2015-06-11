#include "TROOT.h"
#include "TSystem.h"
void doAll(){
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gROOT->ProcessLine(".L ../software/tableMaker/CTable.cpp+");
  gROOT->ProcessLine(".L ../CORE/CMS3.cc+");
  gROOT->ProcessLine(".L ../CORE/Base.cc+");
  gROOT->ProcessLine(".L ../CORE/IsolationTools.cc+");
  gROOT->ProcessLine(".L ../CORE/ElectronSelections.cc+");
  gROOT->ProcessLine(".L ../CORE/MuonSelections.cc+");
  gROOT->ProcessLine(".L ../CORE/MCSelections.cc+");
  gROOT->ProcessLine(".L ../CORE/JetSelections.cc+");
  gROOT->ProcessLine(".L ../CORE/SSSelections.cc+");
  gROOT->ProcessLine(".L SS.cc++");
  gROOT->ProcessLine(".L yields.C++");
  gROOT->ProcessLine("yields()");
}
