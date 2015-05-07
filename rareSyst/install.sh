#!/bin/bash

#Environment
export SCRAM_ARCH=slc6_amd64_gcc481

#Install the Pythia --> Baby part
cmsrel CMSSW_7_2_0
cd CMSSW_7_2_0/src
mkdir babyMaker
cd babyMaker
mkedprod babyMaker
cd babyMaker
mkdir interface
cp ../../../../ConfFile_cfg.py test/
cp ../../../../CfiFile_cfg.py test/
cp ../../../../MCUtilities.h interface/
cp ../../../../producer.cc plugins/
cp ../../../../filter.cc plugins/
cp ../../../../MCUtilities.cc plugins/
