#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD:$PWD/batch

git update-index --assume-unchanged main.cc

therelease=CMSSW_7_4_7_patch1
export SCRAM_ARCH=slc6_amd64_gcc491
if [ ! -d $therelease ]; then cmsrel $therelease; fi
cd $therelease/src
eval `scram ru -sh`
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout 74x-root6
cd ../..
scramv1 b vclean
scramv1 b -j 4
cd ../..

ARCH="$(root-config --arch)"
if [ $ARCH == "macosx64" ]
then
    echo "gc mac specific fixes"
    export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$PWD:$PWD/CORE/:$PWD/Tools/:$PWD/software/tableMaker/
    source $ROOTSYS/bin/thisroot.sh
fi
