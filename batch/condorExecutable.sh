#!/bin/bash

WHICH=$1
FILE=$2
PTREL=$3

#This stuff to get it to run
export CMS_PATH=/cvmfs/cms.cern.ch
export SCRAM_ARCH=slc6_amd64_gcc481
source /cvmfs/cms.cern.ch/cmsset_default.sh
source /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/root/5.34.18/bin/thisroot.sh
export LD_LIBRARY_PATH=$PWD/lib/slc6_amd64_gcc481:$LD_LIBRARY_PATH
export PATH=$PWD:$PATH
export PYTHONPATH=$PWD/python:$PYTHONPATH:$ROOTSYS/bin:$HOME/bin

#Set CMSSW environment
pushd /cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_2_0/src/
eval `scramv1 runtime -sh`
popd

#Specify name of output file and name of dierctory in /hadoop/.../cgeorge/condor
export DIRNAME=ss_13_babies
export WHICH_SMALL=`echo ${WHICH,,}`
export OUTPUT=${WHICH_SMALL}_${FILE}

#This stuff to get output back
export COPYDIR=/hadoop/cms/store/user/cgeorge/condor/${DIRNAME}

root -b -q do.C\($WHICH,$FILE,$PTREL\)
#ls -l `pwd`/${OUTPUT}.root
#lcg-cp -b -D srmv2 --vo cms --connect-timeout 2400 --verbose file://`pwd`/${OUTPUT}.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/${OUTPUT}.root
