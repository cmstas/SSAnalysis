#!/bin/bash

#Environment
export SCRAM_ARCH=slc6_amd64_gcc481

#Install the Pythia --> Baby part
cmsrel CMSSW_7_1_6
cd CMSSW_7_1_6/src
eval `scramv1 runtime -sh`
mkdir babyMaker
cd babyMaker
mkedprod babyMaker
cd babyMaker
cp ../../../../ConfFile_cfg.py python/
cp ../../../../CfiFile_cfi.py python/
cp ../../../../filter_cfg.py python/
cp ../../../../MCUtilities.h plugins/
cp ../../../../producer.cc plugins/
cp ../../../../filter.cc plugins/
cp ../../../../MCUtilities.cc plugins/
scram b -j 5
