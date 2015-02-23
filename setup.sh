#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD

cd CMSSW_7_1_6/
eval `scram ru -sh`
cd -
