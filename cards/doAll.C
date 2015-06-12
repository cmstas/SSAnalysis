#include "TROOT.h"
#include "TSystem.h"
void doAll(){
  gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
  gSystem->Load("../software/tableMaker/libSimpleTable.so");
  gSystem->Load("../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L SS.cc++");
  gROOT->ProcessLine(".L yields.C++");
  gROOT->ProcessLine("yields()");
}
