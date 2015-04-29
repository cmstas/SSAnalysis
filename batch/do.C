#include "TROOT.h"

enum IsolationMethods { Standard = 0, PtRel = 1, MiniIso = 2 , NewMiniIso = 3 , MultiIso = 4 };
enum sample_t { TTBAR, TTW, TTZ, WZ, T1TTTT_1500, T1TTTT_1200, T5qqqqWW_1200_1000_800, T5qqqqWW_deg_1000_315_300 };

int do(sample_t which, int file, IsolationMethods ptrel = 4){

  gSystem->Load("CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L CORE/Tools/utils.cc++");
  gROOT->ProcessLine(".L CORE/Tools/MT2/MT2Utility.cc++");
  gROOT->ProcessLine(".L CORE/Tools/MT2/MT2.cc++");
  gROOT->ProcessLine(".L helper_babymaker.cc++");
  gROOT->ProcessLine(".L run.C++");

  run(which, file, ptrel);
  
  return 0;
}
