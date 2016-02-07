#ifndef EFF_H
#define EFF_H

#include "TH1.h"

TH1F* eff(TH1F* h1, TH1F* h2, const char* name="eff");
TH1F* eff(const char* name1, const char* name2, const char* name="eff");

#endif
