#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD

git update-index --assume-unchanged main.cc

cd CMSSW_7_1_6/
eval `scram ru -sh`
cd -

ARCH="$(root-config --arch)"
if [ $ARCH == "macosx64" ]
then
    echo "gc mac specific fixes"
    export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$PWD:$PWD/CORE/:$PWD/Tools/:$PWD/software/tableMaker/
    source $ROOTSYS/bin/thisroot.sh
fi
