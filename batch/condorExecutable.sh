#!/bin/bash

WHICH=$1
FILE=$2
USER=$3

#Show where you are
hostname

#Environment
export CMS_PATH=/cvmfs/cms.cern.ch
export SCRAM_ARCH=slc6_amd64_gcc491
source /cvmfs/cms.cern.ch/cmsset_default.sh

#Set CMSSW environment
pushd /cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_12/src/
eval `scramv1 runtime -sh`
popd

#Specify name of output file and name of dierctory in /hadoop/...
export DIRNAME=ss_13_babies
export WHICH_SMALL=`echo ${WHICH,,}`

#backgrounds
if [ "$WHICH" == "0" ]; then WHICH_SMALL="ttbar"; fi
if [ "$WHICH" == "1" ]; then WHICH_SMALL="ttw"; fi
if [ "$WHICH" == "2" ]; then WHICH_SMALL="ttzl"; fi
if [ "$WHICH" == "3" ]; then WHICH_SMALL="wz"; fi
if [ "$WHICH" == "4" ]; then WHICH_SMALL="dy_low"; fi
if [ "$WHICH" == "5" ]; then WHICH_SMALL="dy_high"; fi
if [ "$WHICH" == "6" ]; then WHICH_SMALL="wjets"; fi
if [ "$WHICH" == "7" ]; then WHICH_SMALL="ttwqq"; fi
if [ "$WHICH" == "8" ]; then WHICH_SMALL="ttzq"; fi
if [ "$WHICH" == "9" ]; then WHICH_SMALL="ttg"; fi
if [ "$WHICH" == "10" ]; then WHICH_SMALL="singletop1"; fi
if [ "$WHICH" == "11" ]; then WHICH_SMALL="singletop2"; fi
if [ "$WHICH" == "12" ]; then WHICH_SMALL="singletop3"; fi
if [ "$WHICH" == "13" ]; then WHICH_SMALL="singletop4"; fi
if [ "$WHICH" == "14" ]; then WHICH_SMALL="singletop5"; fi
if [ "$WHICH" == "15" ]; then WHICH_SMALL="qqww"; fi
if [ "$WHICH" == "16" ]; then WHICH_SMALL="tttt"; fi
if [ "$WHICH" == "17" ]; then WHICH_SMALL="wwdps"; fi
if [ "$WHICH" == "18" ]; then WHICH_SMALL="ttbar_ph"; fi
if [ "$WHICH" == "19" ]; then WHICH_SMALL="wjets_lo"; fi
if [ "$WHICH" == "20" ]; then WHICH_SMALL="wwz"; fi
if [ "$WHICH" == "21" ]; then WHICH_SMALL="zz"; fi
if [ "$WHICH" == "22" ]; then WHICH_SMALL="tg"; fi
if [ "$WHICH" == "23" ]; then WHICH_SMALL="zg"; fi
if [ "$WHICH" == "24" ]; then WHICH_SMALL="wzz"; fi
if [ "$WHICH" == "25" ]; then WHICH_SMALL="wgtolnug"; fi
if [ "$WHICH" == "26" ]; then WHICH_SMALL="tzq"; fi
if [ "$WHICH" == "27" ]; then WHICH_SMALL="tthtononbb"; fi
if [ "$WHICH" == "28" ]; then WHICH_SMALL="vhtononbb"; fi
if [ "$WHICH" == "29" ]; then WHICH_SMALL="ttzlow"; fi
if [ "$WHICH" == "30" ]; then WHICH_SMALL="gghtozzto4l"; fi
if [ "$WHICH" == "31" ]; then WHICH_SMALL="wzmg"; fi

#signals
if [ "$WHICH" == "100" ]; then WHICH_SMALL="t1tttt_1500"; fi
if [ "$WHICH" == "101" ]; then WHICH_SMALL="t1tttt_1200"; fi
if [ "$WHICH" == "102" ]; then WHICH_SMALL="t5qqqqww_1200_1000_800"; fi
if [ "$WHICH" == "103" ]; then WHICH_SMALL="t5ttttdeg_1000_300_285_280"; fi
if [ "$WHICH" == "104" ]; then WHICH_SMALL="t5qqqqwwdeg_1000_315_300"; fi
if [ "$WHICH" == "105" ]; then WHICH_SMALL="t6ttww_600_425_50"; fi
if [ "$WHICH" == "106" ]; then WHICH_SMALL="t6ttww_650_150_50"; fi

#data
if [ "$WHICH" == "1000" ]; then WHICH_SMALL="datadoubleegc_05oct"; fi
if [ "$WHICH" == "1001" ]; then WHICH_SMALL="datadoublemuonc_05oct"; fi
if [ "$WHICH" == "1002" ]; then WHICH_SMALL="datamuonegc_05oct"; fi
if [ "$WHICH" == "1003" ]; then WHICH_SMALL="datadoubleegd_05oct"; fi
if [ "$WHICH" == "1004" ]; then WHICH_SMALL="datadoublemuond_05oct"; fi
if [ "$WHICH" == "1005" ]; then WHICH_SMALL="datamuonegd_05oct"; fi
if [ "$WHICH" == "1006" ]; then WHICH_SMALL="datadoubleegd_v4"; fi
if [ "$WHICH" == "1007" ]; then WHICH_SMALL="datadoublemuond_v4"; fi
if [ "$WHICH" == "1008" ]; then WHICH_SMALL="datamuonegd_v4"; fi

#signal
if [ "$WHICH" == "10001" ]; then WHICH_SMALL="t1tttt_1950_700to950"; fi
if [ "$WHICH" == "10002" ]; then WHICH_SMALL="t1tttt_1100_1to775"; fi
if [ "$WHICH" == "10003" ]; then WHICH_SMALL="t1tttt_1175_950"; fi
if [ "$WHICH" == "10004" ]; then WHICH_SMALL="t1tttt_1275_900to975"; fi
if [ "$WHICH" == "10005" ]; then WHICH_SMALL="t1tttt_1300_1to1075"; fi
if [ "$WHICH" == "10006" ]; then WHICH_SMALL="t1tttt_1300to1325_700to1100"; fi
if [ "$WHICH" == "10007" ]; then WHICH_SMALL="t1tttt_1350to1375_50to1025"; fi
if [ "$WHICH" == "10008" ]; then WHICH_SMALL="t1tttt_1500to1525_50to1125"; fi
if [ "$WHICH" == "10009" ]; then WHICH_SMALL="t1tttt_1800to1850_1to1450"; fi
if [ "$WHICH" == "10010" ]; then WHICH_SMALL="fastsim1"; fi
if [ "$WHICH" == "10011" ]; then WHICH_SMALL="t1tttt_625_275to375"; fi
if [ "$WHICH" == "10012" ]; then WHICH_SMALL="t1tttt_625to650_200to400"; fi
if [ "$WHICH" == "10013" ]; then WHICH_SMALL="t1tttt_650to675_250to425"; fi
if [ "$WHICH" == "10014" ]; then WHICH_SMALL="t1tttt_700to750_200to500"; fi
if [ "$WHICH" == "10015" ]; then WHICH_SMALL="fastsim2"; fi
if [ "$WHICH" == "10016" ]; then WHICH_SMALL="fastsim3"; fi
if [ "$WHICH" == "10017" ]; then WHICH_SMALL="fastsim4"; fi
if [ "$WHICH" == "10018" ]; then WHICH_SMALL="fastsim5"; fi
if [ "$WHICH" == "10019" ]; then WHICH_SMALL="fastsim6"; fi
if [ "$WHICH" == "10020" ]; then WHICH_SMALL="fastsim7"; fi
if [ "$WHICH" == "10021" ]; then WHICH_SMALL="fastsim8"; fi
if [ "$WHICH" == "10022" ]; then WHICH_SMALL="fastsim9"; fi
if [ "$WHICH" == "10023" ]; then WHICH_SMALL="fastsim10"; fi
if [ "$WHICH" == "10024" ]; then WHICH_SMALL="fastsim11"; fi
if [ "$WHICH" == "10025" ]; then WHICH_SMALL="fastsim12"; fi
if [ "$WHICH" == "10026" ]; then WHICH_SMALL="fastsim13"; fi
if [ "$WHICH" == "10027" ]; then WHICH_SMALL="fastsim14"; fi
if [ "$WHICH" == "10028" ]; then WHICH_SMALL="fastsim15"; fi
if [ "$WHICH" == "10029" ]; then WHICH_SMALL="fastsim16"; fi
if [ "$WHICH" == "10030" ]; then WHICH_SMALL="fastsim17"; fi
if [ "$WHICH" == "10031" ]; then WHICH_SMALL="fastsim18"; fi
if [ "$WHICH" == "10032" ]; then WHICH_SMALL="fastsim19"; fi
if [ "$WHICH" == "10033" ]; then WHICH_SMALL="fastsim20"; fi
if [ "$WHICH" == "10034" ]; then WHICH_SMALL="fastsim21"; fi
if [ "$WHICH" == "10035" ]; then WHICH_SMALL="fastsim22"; fi
if [ "$WHICH" == "10036" ]; then WHICH_SMALL="fastsim23"; fi
if [ "$WHICH" == "10037" ]; then WHICH_SMALL="fastsim24"; fi
if [ "$WHICH" == "10038" ]; then WHICH_SMALL="fastsim25"; fi
if [ "$WHICH" == "10039" ]; then WHICH_SMALL="fastsim26"; fi
if [ "$WHICH" == "10040" ]; then WHICH_SMALL="fastsim27"; fi
if [ "$WHICH" == "10041" ]; then WHICH_SMALL="fastsim28"; fi
if [ "$WHICH" == "10042" ]; then WHICH_SMALL="fastsim29"; fi
if [ "$WHICH" == "10043" ]; then WHICH_SMALL="fastsim30"; fi



#SMS-T1tttt_mGluino-1100to1125_mLSP-700to900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1150_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1150to1175_mLSP-750to925_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1200_mLSP-1to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1400_mLSP-1to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1750_mLSP-50to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-600_mLSP-250to325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-675_mLSP-325to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-700_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-750to775_mLSP-350to525_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-775_mLSP-475to550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-800to825_mLSP-1to575_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-825to850_mLSP-200to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-850to875_mLSP-450to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-875to900_mLSP-1to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-950to975_mLSP-350to725_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1
#SMS-T1tttt_mGluino-975_mLSP-600to750_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1

export OUTPUT=${WHICH_SMALL}_${FILE}

#This stuff to get output back
export COPYDIR=/hadoop/cms/store/user/cgeorge/condor/${DIRNAME}

#Untar the zip dir
tar xzvf CORE.tar.gz
tar xzvf btagsf.tar.gz

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
echo "running: ./main.exe $WHICH $FILE"
./main.exe $WHICH $FILE
ls -l `pwd`/${OUTPUT}.root

echo "copying.  LS is: "
ls

deleteMe=`find -name "$OUTPUT.root" -size -delete -1` 
if [ "$deleteMe" != "" ];
then 
  rm $deleteMe 
  echo "THIS BITCH IS EMPTY, DELETING IT!!"
else
  lcg-cp -b -D srmv2 --vo cms --connect-timeout 2400 --verbose file://`pwd`/${OUTPUT}.root srm://bsrm-3.t2.ucsd.edu:8443/srm/v2/server?SFN=${COPYDIR}/${OUTPUT}.root
fi
