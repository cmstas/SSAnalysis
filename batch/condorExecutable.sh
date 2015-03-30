#!/bin/bash

WHICH=$1
FILE=$2
PTREL=$3

#Show where you are
hostname

#Environment
export CMS_PATH=/cvmfs/cms.cern.ch
export SCRAM_ARCH=slc6_amd64_gcc481
source /cvmfs/cms.cern.ch/cmsset_default.sh
#source /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/root/5.34.18/bin/thisroot.sh
#export LD_LIBRARY_PATH=/cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/root/5.34.18/lib:/cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/gcc/4.8.1/lib:/home/users/cgeorge:/cvmfs/cms.cern.ch/crab3/slc6_amd64_gcc481/external/gcc/4.8.1/lib64:/cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw-patch/CMSSW_7_2_0_patch1/external/slc6_amd64_gcc481/lib
#export PATH=$ROOTSYS/bin:$PATH:${_CONDOR_SCRATCH_DIR}
#export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH:$ROOTSYS/bin
#
#Set CMSSW environment
pushd /cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_2_0/src/
eval `scramv1 runtime -sh`
popd

#Specify name of output file and name of dierctory in /hadoop/.../cgeorge/condor
export DIRNAME=ss_13_babies
export WHICH_SMALL=`echo ${WHICH,,}`
if [ "$PTREL" == "1" ] 
then
  PT="_ptRel"
else
  PT=""
fi
export OUTPUT=${WHICH_SMALL}${PT}_${FILE}

#This stuff to get output back
export COPYDIR=/hadoop/cms/store/user/cgeorge/condor/${DIRNAME}

#Untar the zip dir
tar xzvf CORE.tar.gz

root -b -q do.C\($WHICH,$FILE,$PTREL\)
ls -l `pwd`/${OUTPUT}.root

lcg-cp -b -D srmv2 --vo cms --connect-timeout 2400 --verbose file://`pwd`/${OUTPUT}.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/${OUTPUT}.root
