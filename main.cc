#include "TChain.h"
#include "looper.h"

int main() {

  bool useSkim   = false;
  bool skimAll   = false;
  bool runAll    = false;
  bool runLepEff = false;
  bool runSync   = false;
  string runBaby = "5,14";

  runMain(useSkim, skimAll, runAll, runLepEff, runSync, runBaby, 0);

  return 0;

}

