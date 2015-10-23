#!/bin/bash

babiesVersion=v4.01

git clone git@github.com:cmstas/SSAnalysis.git
export SCRAM_ARCH=slc6_amd64_gcc481
cd SSAnalysis/
if [ ! -d babies ]; then mkdir babies; fi
if [ $USER == "cgeorge" ]
then
  ln -s /home/users/cgeorge/CORE/ .
  ln -s /home/users/cgeorge/software/ . 
  ln -s /nfs-7/userdata/ss2015/ssBabies/$babiesVersion babies/$babiesVersion
else
  git clone git@github.com:cmstas/CORE.git
  git clone git@github.com:cmstas/software.git
  cp /nfs-7/userdata/ss2015/ssBabies/$babiesVersion babies/$babiesVersion/
fi
git clone git@github.com:cmstas/LeptonBabyMaker.git
cmsrel CMSSW_7_4_7_patch1
cd CMSSW_7_4_7_patch1/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout 74x-root6
scramv1 b vclean
scramv1 b -j 4
cd ../../../../
