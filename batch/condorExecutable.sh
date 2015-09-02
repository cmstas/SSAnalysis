#!/bin/bash

WHICH=$1
FILE=$2
PTREL=$3
USER=$4
EXPT=$5

#Show where you are
hostname

#Environment
export CMS_PATH=/cvmfs/cms.cern.ch
export SCRAM_ARCH=slc6_amd64_gcc491
source /cvmfs/cms.cern.ch/cmsset_default.sh

#Set CMSSW environment
pushd /cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_6/src/
eval `scramv1 runtime -sh`
popd

#Specify name of output file and name of dierctory in /hadoop/...
export DIRNAME=ss_13_babies
export WHICH_SMALL=`echo ${WHICH,,}`
if [ "$WHICH" == "0" ]; then WHICH_SMALL="ttbar"; fi
if [ "$WHICH" == "1" ]; then WHICH_SMALL="ttw"; fi
if [ "$WHICH" == "2" ]; then WHICH_SMALL="ttzl"; fi
if [ "$WHICH" == "3" ]; then WHICH_SMALL="wz"; fi
if [ "$WHICH" == "4" ]; then WHICH_SMALL="t1tttt_1500"; fi
if [ "$WHICH" == "5" ]; then WHICH_SMALL="t1tttt_1200"; fi
if [ "$WHICH" == "6" ]; then WHICH_SMALL="dy_low"; fi
if [ "$WHICH" == "7" ]; then WHICH_SMALL="dy_high"; fi
if [ "$WHICH" == "8" ]; then WHICH_SMALL="wjets"; fi
if [ "$WHICH" == "9" ]; then WHICH_SMALL="datadoublemuon"; fi
if [ "$WHICH" == "10" ]; then WHICH_SMALL="datadoubleeg"; fi
if [ "$WHICH" == "11" ]; then WHICH_SMALL="ttpowheg"; fi
if [ "$WHICH" == "12" ]; then WHICH_SMALL="singletop1"; fi
if [ "$WHICH" == "13" ]; then WHICH_SMALL="singletop2"; fi
if [ "$WHICH" == "14" ]; then WHICH_SMALL="singletop3"; fi
if [ "$WHICH" == "15" ]; then WHICH_SMALL="singletop4"; fi
if [ "$WHICH" == "16" ]; then WHICH_SMALL="datamuoneg"; fi
if [ "$WHICH" == "17" ]; then WHICH_SMALL="singletop5"; fi
if [ "$WHICH" == "18" ]; then WHICH_SMALL="ttzq"; fi

if [ "$PTREL" == "4" ] 
then
  PT=""
fi
export OUTPUT=${WHICH_SMALL}_${FILE}${PT}_$EXPT

#This stuff to get output back
export COPYDIR=/hadoop/cms/store/user/cgeorge/condor/${DIRNAME}

#Untar the zip dir
tar xzvf CORE.tar.gz

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
echo "running: ./main.exe $WHICH $FILE $PTREL $EXPT"
./main.exe $WHICH $FILE $PTREL $EXPT
ls -l `pwd`/${OUTPUT}.root

echo "copying.  LS is: "
ls

lcg-cp -b -D srmv2 --vo cms --connect-timeout 2400 --verbose file://`pwd`/${OUTPUT}.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/${OUTPUT}.root
