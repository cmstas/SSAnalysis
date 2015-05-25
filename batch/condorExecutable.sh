#!/bin/bash

WHICH=$1
FILE=$2
PTREL=$3
USERNAME=$4

#Show where you are
hostname

#Environment
source /nfs-7/cmssoft/cms.cern.ch/cmssw/cmsset_default.sh
pushd /nfs-7/cmssoft/cms.cern.ch/cmssw/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_2_0/src/
eval `scramv1 runtime -sh`
popd

#export CMS_PATH=/cvmfs/cms.cern.ch
#export SCRAM_ARCH=slc6_amd64_gcc481
#source /cvmfs/cms.cern.ch/cmsset_default.sh
#
##Set CMSSW environment
#pushd /cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_2_0/src/
#eval `scramv1 runtime -sh`
#popd

#Specify name of output file and name of dierctory in /hadoop/...
export DIRNAME=ss_13_babies
export WHICH_SMALL=`echo ${WHICH,,}`
if [ "$PTREL" == "0" ] 
then
  PT=""
elif [ "$PTREL" == "1" ] 
then
  PT="_ptRel"
elif [ "$PTREL" == "2" ] 
then
  PT="_miniIso"
elif [ "$PTREL" == "3" ] 
then
  PT="_newMiniIso"
elif [ "$PTREL" == "4" ] 
then
  PT="_multiIso"
fi
export OUTPUT=${WHICH_SMALL}_${FILE}${PT}

#This stuff to get output back
export COPYDIR=/hadoop/cms/store/user/$USERNAME/condor/${DIRNAME}

#Untar the zip dir
tar xzvf CORE.tar.gz

root -b -q do.C\($WHICH,$FILE,$PTREL\)
ls -l `pwd`/${OUTPUT}.root

echo "copying.  LS is: "
ls

lcg-cp -b -D srmv2 --vo cms --connect-timeout 2400 --verbose file://`pwd`/${OUTPUT}.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/${OUTPUT}.root
