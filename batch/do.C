#include "TROOT.h"
#include "CORE/CMS3.h"

enum sample_t { TTBAR, TTW, TTZ, WZ, T1TTTT_1500, T1TTTT_1200 };

int do(sample_t which, int file, bool ptrel = 0){

  gSystem->Load("CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L CORE/Tools/utils.cc+");
  gROOT->ProcessLine(".L CORE/Tools/MT2/MT2Utility.cc+");
  gROOT->ProcessLine(".L CORE/Tools/MT2/MT2.cc+");
  gROOT->ProcessLine(".L helper_babymaker.cc+");
  gROOT->ProcessLine(".L run.C+");

  run(which, file, ptrel);
}
