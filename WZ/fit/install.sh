#!/bin/bash

# source this, not ./install.sh
cmsrel CMSSW_7_4_7_patch1
cd CMSSW_7_4_7_patch1/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout 74x-root6
git checkout 43ddee7a162a322013d0c0f94151bc9dccb72d26 .
# scramv1 b vclean
scramv1 b -j 15
cd ../../../../
