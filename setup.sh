#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD

git update-index --assume-unchanged main.cc

cd CMSSW_7_4_1_patch1/
eval `scram ru -sh`
cd -
