#!/bin/bash

xrootdbroken="1"

nSubmitted=0

SPPATH=""

#Source this stupid thing
pushd ..
. setup.sh
popd

#Copy main.C to main.cc
cp main.cc.norun main.cc
if [ "$xrootdbroken" == "1" ]
then
  sed -i 's,Open(Form("root://cmsxrootd.fnal.gov//%s/%s/%s/%s,Open(Form("/hadoop/cms/%s/%s/%s/%s,' main.cc
fi
make -j 10

#Start by checking proxy, get path to proxy file
voms-proxy-init -hours 168 -voms cms
voms-proxy-info --all &> voms_status.txt
if grep "Couldn't find a valid proxy." voms_status.txt &>/dev/null
then 
  echo "Error: couldn't find a valid proxy!  Aborting.  Create a proxy with voms-proxy-init."
  rm voms_status.txt
  return 1
fi
lineWithPath=`sed -n /path/= voms_status.txt`
pathToProxy=`awk -v var="$lineWithPath" 'NR==var {print $3}' voms_status.txt`

#Change the username
sed -i "s/cgeorge/$USER/" condorFileTemplate
sed -i "s/cgeorge/$USER/" condorExecutable.sh

#isSignal defaults to false
isSignal=0 

#Then submit jobs
nIter=0
for sname in "FASTSIMTEST" # "WWZ" "TG" "TTG"  "WZ"  "GGHtoZZto4L" "TTBAR" "TTW" "WJets_LO"  "ZG" "T1TTTT_1200" "T1TTTT_1500" "TG"  "T5QQQQWW_1200_1000_800" "T5QQQQWWDeg_1000_315_300" "T5ttttDeg_1000_300_285_280" "T6TTWW_600_425_50" "T6TTWW_650_150_50"  "SINGLETOP1" "SINGLETOP2" "SINGLETOP3" "SINGLETOP4" "SINGLETOP5" "TTWQQ" "TTZQ"  "ZZ" "QQWW" "TTBAR"  "DataDoubleMuonD_05oct" "DataDoubleEGD_05oct" "DataMuonEGD_05oct" "DataDoubleMuonD_v4" "DataDoubleEGD_v4" "DataMuonEGD_v4" "WJets" "TTZL" "DY_high" "DY_low" "WGToLNuG" "TTG" "TTHtoNonBB" "VHtoNonBB"  "TZQ"  "TTTT"  "WWDPS" "WZZ" "TTBAR_PH"  "DataDoubleMuonC_05oct" "DataDoubleEGC_05oct" "DataMuonEGC_05oct" "TTZlow" "WZMG"
do
  path="/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2"
  #Iter
  nIter=$(( $nIter + 1 ))

  #Get Name
  if   [ $sname == "TTBAR" ]
  then name="TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=0
  elif [ $sname == "TTW" ]
  then name="TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=1
  elif [ $sname == "TTZL" ]
  then name="TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2"
    tag=V07-04-11
    nameNu=2
  elif [ $sname == "WZ" ]
  then name="WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=3
  elif [ $sname == "DY_low" ]
    then name="DYJetsToLL_M-5to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=4
  elif [ $sname == "DY_high" ]
    then name="/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=5
  elif [ $sname == "WJets" ]; 
    then name="WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=6
  elif [ $sname == "TTWQQ" ]
  then name="TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=7
  elif [ $sname == "TTZQ" ]
  then name="TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=8
  elif [ $sname == "TTG" ] 
  then name="TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=9
  elif [ $sname == "SINGLETOP1"    ]; 
    then name="ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=10
  elif [ $sname == "SINGLETOP2"    ]; 
    then name="ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=11
  elif [ $sname == "SINGLETOP3"    ]; 
    then name="ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=12
  elif [ $sname == "SINGLETOP4"    ]; 
    then name="ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2"
    tag=V07-04-11
    nameNu=13
  elif [ $sname == "SINGLETOP5"    ]; 
    then name="ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=14
  elif [ $sname == "QQWW" ]
  then name=WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1
    tag=V07-04-11
    nameNu=15
  elif [ $sname == "TTTT" ] 
  then name="TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=16
  elif [ $sname == "WWDPS" ] 
  then name="WW_DoubleScattering_13TeV-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=17
  elif [ $sname == "TTBAR_PH" ] 
    then name="TT_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=18
  elif [ $sname == "WJets_LO" ]
    then name="WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=19
  elif [ $sname == "WWZ" ]
  then name="WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=20
  elif [ $sname == "ZZ" ]
  then name="ZZTo4L_13TeV_powheg_pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2"
    tag=V07-04-11
    nameNu=21
  elif [ $sname == "TG" ]; 
    then name="TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=22
  elif [ $sname == "ZG" ]
  then name="ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=23
  elif [ $sname == "WZZ" ] 
  then name="WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=24
  elif [ $sname == "WGToLNuG" ] 
  then name="WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=25
  elif [ $sname == "TZQ" ] 
  then name="tZq_ll_4f_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=26
  elif [ $sname == "TTHtoNonBB" ] 
  then name="ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=27
  elif [ $sname == "VHtoNonBB" ] 
  then name="VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=28
  elif [ $sname == "TTZlow" ]
  then name="TTZ_lowMM_petrucc"
    tag="V07-04-11"
    path="/hadoop/cms/store/group/snt/run2_25ns_privateMiniAOD"
    nameNu=29
  elif [ $sname == "GGHtoZZto4L" ] 
  then name="GluGluHToZZTo4L_M125_13TeV_powheg_JHUgen_pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=30
  elif [ $sname == "WZMG" ] 
  then name="WZJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=31

  #data
  elif [ $sname == "DataDoubleEGC_05oct"    ]; 
    then name="Run2015C_25ns_DoubleEG_MINIAOD_05Oct2015-v1";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1000
  elif [ $sname == "DataDoubleMuonC_05oct"  ]; 
    then name="Run2015C_25ns_DoubleMuon_MINIAOD_05Oct2015-v1";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1001
  elif [ $sname == "DataMuonEGC_05oct"    ]; 
    then name="Run2015C_25ns_MuonEG_MINIAOD_05Oct2015-v1";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1002
  elif [ $sname == "DataDoubleEGD_05oct"    ]; 
    then name="Run2015D_DoubleEG_MINIAOD_05Oct2015-v1";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1003
  elif [ $sname == "DataDoubleMuonD_05oct"  ]; 
    then name="Run2015D_DoubleMuon_MINIAOD_05Oct2015-v1";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1004
  elif [ $sname == "DataMuonEGD_05oct"    ]; 
    then name="Run2015D_MuonEG_MINIAOD_05Oct2015-v2";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1005
  elif [ $sname == "DataDoubleEGD_v4"    ]; 
    then name="Run2015D_DoubleEG_MINIAOD_PromptReco-v4";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1006
  elif [ $sname == "DataDoubleMuonD_v4"  ]; 
    then name="Run2015D_DoubleMuon_MINIAOD_PromptReco-v4";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1007
  elif [ $sname == "DataMuonEGD_v4"    ]; 
    then name="Run2015D_MuonEG_MINIAOD_PromptReco-v4";
    path="/hadoop/cms/store/group/snt/run2_data"
    tag=V07-04-11
    nameNu=1008

  #Signals
  elif [ $sname == "T1TTTT_1500" ] 
  then name="SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=100
  elif [ $sname == "T1TTTT_1200" ]  
  then name="SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1";
    tag=V07-04-11
    nameNu=101
  elif [ $sname == "T5QQQQWWDeg_1000_315_300" ] 
  then
    name=T5qqqqWWDeg_mGo1000_mCh315_mChi300_dilep
    path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
    tag=""
    nameNu=104
  elif [ $sname == "T5QQQQWW_1200_1000_800" ] 
  then
    name=T5qqqqWW_mGo1200_mCh1000_mChi800_dilep
    path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
    tag=""
    nameNu=102
  elif [ $sname == "T5ttttDeg_1000_300_285_280" ] 
  then
    name=T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_23bodydec
    path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
    tag=""
    nameNu=103
  elif [ $sname == "T6TTWW_600_425_50" ] 
  then
    name=T6ttWW_600_425_50
    path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
    tag=""
    nameNu=105
  elif [ $sname == "T6TTWW_650_150_50" ] 
  then
    name=T6ttWW_650_150_50
    path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
    tag=""
    nameNu=106

  #signal
  elif [ $sname == "T1TTTT_1950_750to950" ] 
  then
    name="SMS-T1tttt_mGluino-1950_mLSP-700to950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
    tag="V07-04-12"
    isSignal=1
    nameNu=10001 

  #default
  else 
    name=$sname 
    tag=V07-04-08
    nameNu=999999
  fi

  #fastsim path
  if [ "$isSignal" == "1" ]; then path="/hadoop/cms/store/group/snt/run2_fastsim"; tag="V07-04-12"; fi

  #Data vs. MC variables
  if [ `echo $name | tr '_' ' ' | awk '{print $1}' | cut -c 1-7` == "Run2015" ]
  then 
    infix="merged/"
    isData="1"
  else 
    infix="" 
    isData="0"
  fi

  #Get number of files
  numberOfFiles=$((`ls -l $path/$name/$infix$tag/merged_ntuple_*.root | wc -l`))
  echo "number of files: $numberOfFiles"
  echo "dir: $path/$name/$infix$tag"
  
  #Submit all of them
  for (( i=0; i<$numberOfFiles; i++))
  do
    sname_lower=`echo ${sname,,}`
    number=$(( $i + 1 ))

    #Except they've finished
    if [ -e /hadoop/cms/store/user/$USER/condor/ss_13_babies/$SPPATH/${sname_lower}_${number}.root ] 
    then 
      continue
    fi

    echo "-------------"
    echo "Working on $sname $number"
  
    #Or if they're still running
    if [ -e /data/tmp/cgeorge/logs/condorLog_${nameNu}_${number}.log ] 
    then
      while read line
      do
        if [ `echo $line | awk '{ print $1 }'` == "Cluster" ] 
        then
          cluster=`echo $line | awk '{ print $3 }'`
        elif [ `echo $line | awk '{ print $1 }'` == "Proc" ] 
        then
          process=`echo $line | awk '{ print $3 }'`
        fi
      done < /data/tmp/cgeorge/logs/condorLog_${nameNu}_${number}.log
      jobid="$cluster.$process"
      condor_q $jobid > temp.txt
      result=`more temp.txt | awk 'END{print $1}'`
      if [ "$result" == "$jobid" ] 
      then
        echo "already submitted!"
        let "nSubmitted=$nSubmitted+1"
        continue
      fi
    fi
    echo "submitting..."
    let "nSubmitted=$nSubmitted+1"

    #Make condor file
    cp condorFileTemplate condorFile
    if [ "$xrootdbroken" == "1" ]
    then
      sed -i 's/T2_US_UCSD,T2_US_WISCONSIN,T2_US_FLORIDA,T2_US_PURDUE,T2_US_NEBRASKA,T2_US_CALTECH/T2_US_UCSD/' condorFile
    fi
  
    #submit it
    sed -i s/ARG1/$nameNu/g condorFile
    sed -i s/ARG2/$number/g condorFile
    sed -i s/ARG3/$USER/g condorFile
    sed -i "s,USER_PROXY,$pathToProxy,g" condorFile
    condor_submit condorFile
  done
done

if [ "$nSubmitted" == "0" ]
then 
  return 1
else 
  return 0
fi
