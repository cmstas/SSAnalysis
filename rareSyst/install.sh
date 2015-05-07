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
cp ../../../../ConfFile.py test/
cp ../../../../CfiFile.py test/
cp ../../../../MCUtilities.h interface/
cp ../../../../producer.cc src/
cp ../../../../filter.cc src/
cp ../../../../MCUtilities.cc src/
