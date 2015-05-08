#!/bin/bash

#Environment
export SCRAM_ARCH=slc6_amd64_gcc481

#Install the Pythia --> Baby part
cmsrel CMSSW_7_2_0
cd CMSSW_7_2_0/src
eval `scramv1 runtime -sh`
mkdir producer
cd producer
mkedprod producer
cd producer
cp ../../../../ConfFile_cfg.py python/
cp ../../../../CfiFile_cfi.py python/
cp ../../../../filter_cfg.py python/
cp ../../../../MCUtilities.h plugins/
cp ../../../../producer.cc plugins/
cp ../../../../filter.cc plugins/
cp ../../../../MCUtilities.cc plugins/
cp ../../../../BuildFile.xml plugins/
scram b -j 5
