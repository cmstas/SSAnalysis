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

FASTSIM="T1TTTT_1000to1050_1to800 T1TTTT_1000_1to700 T1TTTT_1400_1to1175 T1TTTT_1900to1950_0to1450 T1TTTT_1325to1350_1to1125 T1TTTT_1950_700to950 T1TTTT_1100_1to775 T1TTTT_1175_950 T1TTTT_1275_900to975 T1TTTT_1300_1to1075 T1TTTT_1300to1325_700to1100 T1TTTT_1350to1375_50to1025 T1TTTT_1500to1525_50to1125 T1TTTT_1800to1850_1to1450 T1TTTT_1100to1125_700to900 T1TTTT_625_275to375 T1TTTT_625to650_200to400 T1TTTT_650to675_250to425 T1TTTT_700to750_200to500 T1TTTT_1150_1to800 T1TTTT_1150to1175_750to925 T1TTTT_1200_1to825 T1TTTT_1200to1225_800to1000 T1TTTT_1250to1275_700to1050 T1TTTT_1325to1350_1to1125 T1TTTT_1400_1to1175 T1TTTT_1400to1425_50to1100 T1TTTT_1425to1450_1to1200 T1TTTT_1450to1475_50to1075 T1TTTT_1475to1500_1to1250 T1TTTT_1525to1550_1to1300 T1TTTT_1600to1650_1to1350 T1TTTT_1650to1700_1to1400 T1TTTT_1700to1750_1to1450 T1TTTT_1750_50to1450 T1TTTT_1850to1900_1to1450 T1TTTT_1900to1950_0to1450 T1TTTT_600_250to325 T1TTTT_675_325to450 T1TTTT_700_1to450 T1TTTT_750to775_350to525 T1TTTT_775_475to550 T1TTTT_800to825_1to575 T1TTTT_825to850_200to600 T1TTTT_850to875_450to625 T1TTTT_875to900_1to650 T1TTTT_950to975_350to725 T1TTTT_975_600to750"

#Then submit jobs
nIter=0
for sname in "TTHtoNonBBext" # "WGMG" # $FASTSIM # "WWZ" "TG" "TTG"  "WZ"  "GGHtoZZto4L" "TTBAR" "TTW" "WJets_LO"  "ZG" "T1TTTT_1200" "T1TTTT_1500" "TG"  "T5QQQQWW_1200_1000_800" "T5QQQQWWDeg_1000_315_300" "T5ttttDeg_1000_300_285_280" "T6TTWW_600_425_50" "T6TTWW_650_150_50"  "SINGLETOP1" "SINGLETOP2" "SINGLETOP3" "SINGLETOP4" "SINGLETOP5" "TTWQQ" "TTZQ"  "ZZ" "QQWW" "TTBAR"  "DataDoubleMuonD_05oct" "DataDoubleEGD_05oct" "DataMuonEGD_05oct" "DataDoubleMuonD_v4" "DataDoubleEGD_v4" "DataMuonEGD_v4" "WJets" "TTZL" "DY_high" "DY_low" "WGToLNuG" "TTG" "TTHtoNonBB" "VHtoNonBB"  "TZQ"  "TTTT"  "WWDPS" "WZZ" "TTBAR_PH"  "DataDoubleMuonC_05oct" "DataDoubleEGC_05oct" "DataMuonEGC_05oct" "TTZlow" "WZMG"
do
  isSignal=0 
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
  elif [ $sname == "WGMG" ] 
  then name="WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-12
    nameNu=32
  elif [ $sname == "TTHtoNonBBext" ] 
  then name="ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1"
    tag=V07-04-12 
    nameNu=33

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
  elif [ $sname == "T1TTTT_1950_700to950" ] 
  then
    name="SMS-T1tttt_mGluino-1950_mLSP-700to950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10001 
  elif [ $sname == "T1TTTT_1100_1to775" ] 
  then
    name="SMS-T1tttt_mGluino-1100_mLSP-1to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10002
  elif [ $sname == "T1TTTT_1175_950" ] 
  then
    name="SMS-T1tttt_mGluino-1175_mLSP-950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10003 
  elif [ $sname == "T1TTTT_1275_900to975" ] 
  then
    name="SMS-T1tttt_mGluino-1275_mLSP-900to975_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10004 
  elif [ $sname == "T1TTTT_1300_1to1075" ] 
  then
    name="SMS-T1tttt_mGluino-1300_mLSP-1to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10005 
  elif [ $sname == "T1TTTT_1300to1325_700to1100" ] 
  then
    name="SMS-T1tttt_mGluino-1300to1325_mLSP-700to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10006 
  elif [ $sname == "T1TTTT_1350to1375_50to1025" ] 
  then
    name="SMS-T1tttt_mGluino-1350to1375_mLSP-50to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10007 
  elif [ $sname == "T1TTTT_1500to1525_50to1125" ] 
  then
    name="SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10008 
  elif [ $sname == "T1TTTT_1800to1850_1to1450" ] 
  then
    name="SMS-T1tttt_mGluino-1800to1850_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10009
  elif [ $sname == "T1TTTT_1100to1125_700to900" ] 
  then
    name="SMS-T1tttt_mGluino-1100to1125_mLSP-700to900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10010
  elif [ $sname == "T1TTTT_625_275to375" ] 
  then
    name="SMS-T1tttt_mGluino-625_mLSP-275to375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10011 
  elif [ $sname == "T1TTTT_625to650_200to400" ] 
  then
    name="SMS-T1tttt_mGluino-625to650_mLSP-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10012 
  elif [ $sname == "T1TTTT_650to675_250to425" ] 
  then
    name="SMS-T1tttt_mGluino-650to675_mLSP-250to425_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10013 
  elif [ $sname == "T1TTTT_700to750_200to500" ] 
  then
    name="SMS-T1tttt_mGluino-700to750_mLSP-200to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10014 
  elif [ $sname == "T1TTTT_1150_1to800" ] 
  then
    name="SMS-T1tttt_mGluino-1150_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10015 
  elif [ $sname == "T1TTTT_1150to1175_750to925" ] 
  then
    name="SMS-T1tttt_mGluino-1150to1175_mLSP-750to925_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10016 
  elif [ $sname == "T1TTTT_1200_1to825" ] 
  then
    name="SMS-T1tttt_mGluino-1200_mLSP-1to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10017 
  elif [ $sname == "T1TTTT_1200to1225_800to1000" ] 
  then
    name="SMS-T1tttt_mGluino-1200to1225_mLSP-800to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10018 
  elif [ $sname == "T1TTTT_1250to1275_700to1050" ] 
  then
    name="SMS-T1tttt_mGluino-1250to1275_mLSP-700to1050_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10019 
  elif [ $sname == "T1TTTT_1325to1350_1to1125" ] 
  then
    name="SMS-T1tttt_mGluino-1325to1350_mLSP-1to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10020 
  elif [ $sname == "T1TTTT_1400_1to1175" ] 
  then
    name="SMS-T1tttt_mGluino-1400_mLSP-1to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10021 
  elif [ $sname == "T1TTTT_1400to1425_50to1100" ] 
  then
    name="SMS-T1tttt_mGluino-1400to1425_mLSP-50to1100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10022 
  elif [ $sname == "T1TTTT_1425to1450_1to1200" ] 
  then
    name="SMS-T1tttt_mGluino-1425to1450_mLSP-1to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10023 
  elif [ $sname == "T1TTTT_1450to1475_50to1075" ] 
  then
    name="SMS-T1tttt_mGluino-1450to1475_mLSP-50to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10024 
  elif [ $sname == "T1TTTT_1475to1500_1to1250" ] 
  then
    name="SMS-T1tttt_mGluino-1475to1500_mLSP-1to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10025 
  elif [ $sname == "T1TTTT_1525to1550_1to1300" ]  #bad
  then
    name="SMS-T1tttt_mGluino-1525to1550_mLSP-1to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10026 
  elif [ $sname == "T1TTTT_1600to1650_1to1350" ] 
  then
    name="SMS-T1tttt_mGluino-1600to1650_mLSP-1to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10027 
  elif [ $sname == "T1TTTT_1650to1700_1to1400" ] 
  then
    name="SMS-T1tttt_mGluino-1650to1700_mLSP-1to1400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10028 
  elif [ $sname == "T1TTTT_1700to1750_1to1450" ] 
  then
    name="SMS-T1tttt_mGluino-1700to1750_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10029 
  elif [ $sname == "T1TTTT_1750_50to1450" ] 
  then
    name="SMS-T1tttt_mGluino-1750_mLSP-50to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10030
  elif [ $sname == "T1TTTT_1850to1900_1to1450" ] 
  then
    name="SMS-T1tttt_mGluino-1850to1900_mLSP-1to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10031 
  elif [ $sname == "T1TTTT_1900to1950_0to1450" ]   #bad
  then
    name="SMS-T1tttt_mGluino-1900to1950_mLSP-0to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10032 
  elif [ $sname == "T1TTTT_600_250to325" ] 
  then
    name="SMS-T1tttt_mGluino-600_mLSP-250to325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10033 
  elif [ $sname == "T1TTTT_675_325to450" ] 
  then
    name="SMS-T1tttt_mGluino-675_mLSP-325to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10034 
  elif [ $sname == "T1TTTT_700_1to450" ] 
  then
    name="SMS-T1tttt_mGluino-700_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10035 
  elif [ $sname == "T1TTTT_750to775_350to525" ] 
  then
    name="SMS-T1tttt_mGluino-750to775_mLSP-350to525_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10036 
  elif [ $sname == "T1TTTT_775_475to550" ] 
  then
    name="SMS-T1tttt_mGluino-775_mLSP-475to550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10037 
  elif [ $sname == "T1TTTT_800to825_1to575" ] 
  then
    name="SMS-T1tttt_mGluino-800to825_mLSP-1to575_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10038 
  elif [ $sname == "T1TTTT_825to850_200to600" ] 
  then
    name="SMS-T1tttt_mGluino-825to850_mLSP-200to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10039 
  elif [ $sname == "T1TTTT_850to875_450to625" ] 
  then
    name="SMS-T1tttt_mGluino-850to875_mLSP-450to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10040 
  elif [ $sname == "T1TTTT_875to900_1to650" ] 
  then
    name="SMS-T1tttt_mGluino-875to900_mLSP-1to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10041 
  elif [ $sname == "T1TTTT_950to975_350to725" ] 
  then
    name="SMS-T1tttt_mGluino-950to975_mLSP-350to725_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10042 
  elif [ $sname == "T1TTTT_975_600to750" ] 
  then
    name="SMS-T1tttt_mGluino-975_mLSP-600to750_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10043 
  elif [ $sname == "T1TTTT_1375_950to1150" ] 
  then
    name="SMS-T1tttt_mGluino-1375_mLSP-950to1150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10044
  elif [ $sname == "T1TTTT_1000to1050_1to800" ] 
  then
    name="SMS-T1tttt_mGluino-1000to1050_mLSP-1to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10045
  elif [ $sname == "T1TTTT_1000_1to700" ] 
  then
    name="SMS-T1tttt_mGluino-1000_mLSP-1to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10046

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
