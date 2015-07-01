#include "TChain.h"
#include "looper.h"

int main() {

  bool useSkim   = false;
  bool skimAll   = false;
  bool runAll    = false;
  bool runLepEff = false;
  bool runSync   = false;
  bool expt      = false;
  string runBaby = "33";

  runMain(useSkim, skimAll, runAll, runLepEff, runSync, runBaby, expt);

  return 0;

}

