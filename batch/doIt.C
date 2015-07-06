#include "TROOT.h"
#include "base.h"
#include "CORE/SSSelections.h"

int doIt(sample_t which, int file, IsolationMethods ptrel, bool expt){

  gSystem->Load("CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L CORE/Tools/utils.cc++");
  gROOT->ProcessLine(".L CORE/Tools/MT2/MT2Utility.cc++");
  gROOT->ProcessLine(".L CORE/Tools/MT2/MT2.cc++");
  gROOT->ProcessLine(".L helper_babymaker.cc++");
  gROOT->ProcessLine(".L run.C++");
  gROOT->ProcessLine(Form("run(%i, %i, %i, %i)", which, file, ptrel, expt)); 

  return 0;
}
