#!/bin/bash

xrootdbroken="1"

nSubmitted=0

# SPPATH="ss_babies_Nov25"
SPPATH="ss_babies_Jan9"

DOCOMPILE="1" # compile stuff
NOSUBMIT="0" # don't actually submit jobs
DOONE="0" # submit one file per sample
if [ $# -gt 0 ]; then
    if [[ $1 == *fast* ]]; then 
      DOCOMPILE="0"
    fi
    if [[ $1 == *test* ]]; then 
      NOSUBMIT="1"
    fi
    if [[ $1 == *one* ]]; then 
      DOONE="1"
    fi
fi

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

if [ "$DOCOMPILE" == "1" ]; then
  make -j 10
fi
if [ "$NOSUBMIT" == "1" ]; then
  return
fi

#Start by checking proxy, get path to proxy file
# voms-proxy-init -hours 168 -voms cms
# voms-proxy-info --all &> voms_status.txt
# if grep "Couldn't find a valid proxy." voms_status.txt &>/dev/null
# then 
#   echo "Error: couldn't find a valid proxy!  Aborting.  Create a proxy with voms-proxy-init."
#   rm voms_status.txt
#   return 1
# fi
# lineWithPath=`sed -n /path/= voms_status.txt`
# pathToProxy=`awk -v var="$lineWithPath" 'NR==var {print $3}' voms_status.txt`

certFile="/home/users/${USER}/.globus/proxy_for_${USER}.file"
if [ ! -e $certFile ] ; then
  echo "didn't find $certFile. making it"
  voms-proxy-init -hours 9876543:0 -out=${certFile}
else
  echo "extending proxy in $certFile"
  voms-proxy-init -q -voms cms -hours 120 -valid=120:0 -cert=${certFile}
fi
pathToProxy=`voms-proxy-info -path`

condor_q -w $USER | grep "condorExecutable" > jobssubmitted.txt

#Change the username
sed -i "s/namin/$USER/" condorFileTemplate
sed -i "s/namin/$USER/" condorExecutable.sh

#Make some directories that will be used
mkdir -p /data/tmp/${USER}/logs/

FORSYNC="TG"

# NOT IN GOLDEN JSON: DataDoubleMuonv1 DataDoubleEGv1 DataMuonEGv1 
DATA="
DataDoubleMuonv2 DataDoubleEGv2 DataMuonEGv2 
DataDoubleMuonC DataDoubleEGC DataMuonEGC 
DataDoubleMuonD DataDoubleEGD DataMuonEGD 
DataDoubleMuonE DataDoubleEGE DataMuonEGE 
DataDoubleMuonF DataDoubleEGF DataMuonEGF 
DataDoubleMuonG DataDoubleEGG DataMuonEGG
DataDoubleMuonH DataDoubleEGH DataMuonEGH
DataDoubleMuonHv3 DataDoubleEGHv3 DataMuonEGHv3
DataDoubleMuonRerecoB DataDoubleEGRerecoB DataMuonEGRerecoB 
DataDoubleMuonRerecoC DataDoubleEGRerecoC DataMuonEGRerecoC 
DataDoubleMuonRerecoD DataDoubleEGRerecoD DataMuonEGRerecoD 
DataDoubleMuonRerecoE DataDoubleEGRerecoE DataMuonEGRerecoE 
DataDoubleMuonRerecoF DataDoubleEGRerecoF DataMuonEGRerecoF 
DataDoubleMuonRerecoG DataDoubleEGRerecoG DataMuonEGRerecoG
DataJetHTH DataJetHTHv3
"
CENTRAL="WZ ZZZ WZZ WWZ WWW WWDPS QQWW WGToLNuG ZG TG TTG VHtoNonBB TTHtoNonBB TZQ TTTT WJets DY_high DY_low ZZ TTBAR_PH GGHtoZZto4L TTZlow TTZnlo TTWnlo TWZ"
HIGGS="THQ_SCAN THW_SCAN TTH_SCAN"
SIG="T1TTTT_main T5QQQQVV_dm20 T5TTCC_main T5QQQQVV_main T6TTWW_main T5TTTT_dm175 T1TTBB_main"
INSITU="WJets100To200 WJets200To400 WJets400To600 WJets600To800 WJets800To1200"

TEST="WZ ZZZ WZZ WWZ WWW WWDPS QQWW WGToLNuG TG TTG VHtoNonBB TTHtoNonBB TZQ TTTT WJets DY_high ZZ TTBAR_PH GGHtoZZto4L TTZnlo TTWnlo TWZ" # ZG TTTT DY_low

# ALL="$DATA $CENTRAL $HIGGS $SIG $INSITU"
ALL="$TEST"

#Then submit jobs
nIter=0
for sname in $ALL
do
  isSignal=0 
  path="/hadoop/cms/store/group/snt/run2_25ns_MiniAODv2"
  #Iter
  nIter=$(( $nIter + 1 ))

  #Get Name
  if   [ $sname == "SYNCH_TTW" ] ; then 
    name="TTW76_TTW76-SSDL2016-forSynch_Private76X";
    path="/hadoop/cms/store/group/snt/run2_ss_synch/";
    tag="V07-06-03_MC";
    nameNu=101010
  elif   [ $sname == "SYNCH_TTW80" ] ; then 
    name="TTW80_TTW80-SSDL2016-forSynch_Private80Xv2";
    path="/hadoop/cms/store/group/snt/run2_ss_synch/";
    tag="V08-00-05";
    nameNu=101011
  elif   [ $sname == "TTH_M350" ] ; then 
    name="TTH_ttH-m350-MINIAOD_Private80Xv2";
    path="/hadoop/cms/store/group/snt/run2_25ns_80Private/";
    tag="V08-00-05";
    nameNu=150000
  elif   [ $sname == "THW_M350" ] ; then 
    name="THW_tHW-m350-MINIAOD_Private80Xv2";
    path="/hadoop/cms/store/group/snt/run2_25ns_80Private/";
    tag="V08-00-05";
    nameNu=150001
  elif   [ $sname == "TTH_SCAN" ] ; then 
    name="TTH_ttH-scan-MINIAOD_Private80Xv2_v2";
    path="/hadoop/cms/store/group/snt/run2_25ns_80Private/";
    tag="V08-00-05";
    nameNu=150002
  elif   [ $sname == "THW_SCAN" ] ; then 
    name="THW_tHW-scan-MINIAOD_Private80Xv2";
    path="/hadoop/cms/store/group/snt/run2_25ns_80Private/";
    tag="V08-00-05";
    nameNu=150003
  elif   [ $sname == "THQ_SCAN" ] ; then 
    name="THQ_tHq-scan-MINIAOD_Private80Xv2_v2";
    path="/hadoop/cms/store/group/snt/run2_25ns_80Private/";
    tag="V08-00-09";
    nameNu=150004
  elif   [ $sname == "TTBAR" ] ; then 
    name="TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v2"
    path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv1/";
    tag="V08-00-01"
    nameNu=0
  elif [ $sname == "TTW" ]; then 
      name="ttWJets_13TeV_madgraphMLM_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
      path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
      tag="V08-00-05";
    nameNu=1
  elif [ $sname == "TTZ" ]; then 
      name="ttZJets_13TeV_madgraphMLM_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
      path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
      tag="V08-00-05";
    nameNu=2
  elif [ $sname == "WZ" ]; then 
      name="WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=3
  elif [ $sname == "DY_low" ]; then # FIXME FIXME
    name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
      path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
      tag="V08-00-05";
    nameNu=4
  elif [ $sname == "DY_high" ]; then 
      name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=5
  elif [ $sname == "DY_high_LO" ]; then
    name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1"; 
    tag="V07-06-03_MC";
    path="/hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/";
    nameNu=101012
  elif [ $sname == "WJets" ]; then 
      name="WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=6
  elif [ $sname == "TTWQQ" ]; then
    name="TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8";
    path="/hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/";
    tag="V07-06-03_MC";
    nameNu=7
  elif [ $sname == "TTZQ" ]; then
    name="tZq_ll_4f_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1";
    tag="V07-06-03_MC";
    path="/hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/";
    nameNu=8
  elif [ $sname == "TTG" ] ;then 
      name="TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=9
  elif [ $sname == "SINGLETOP1"    ]; then 
    name="ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1";
    path="/hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/T/";
    tag="CMS3_V07-06-03";
    nameNu=10
  elif [ $sname == "SINGLETOP2"    ]; then 
    name="ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1";
    tag="CMS3_V07-06-03";
    path="/hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/T/";
    nameNu=11
  elif [ $sname == "SINGLETOP3"    ]; then 
    name="ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=12
  elif [ $sname == "SINGLETOP4"    ]; then 
    name="ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1";
    tag="V07-06-03_MC";
    path="/hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/";
    nameNu=13
  elif [ $sname == "SINGLETOP5"    ]; then 
    name="ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1";
    tag="V07-06-03_MC";
    path="/hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/";
    nameNu=14
  elif [ $sname == "QQWW" ]; then 
      name="WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=15
  elif [ $sname == "TTTT" ] ;then # FIXME FIXME
      name="TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=16
  elif [ $sname == "WWDPS" ] ;then 
      name="WWTo2L2Nu_DoubleScattering_13TeV-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=17
  elif [ $sname == "TTBAR_PH" ] ;then 
    name="TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=18
  elif [ $sname == "WWZ" ]; then 
      name="WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=20
  elif [ $sname == "ZZ" ]; then 
      name="ZZTo4L_13TeV_powheg_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=21
  elif [ $sname == "TG" ]; then 
      name="TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=22
  elif [ $sname == "ZG" ]; then # FIXME FIXME
      name="ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
      path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
      tag="V08-00-05";
    nameNu=23
  elif [ $sname == "WZZ" ] ;then 
      name="WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=24
  elif [ $sname == "WGToLNuG" ] ;then 
      name="WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=25
  elif [ $sname == "TZQ" ] ;then 
      name="tZq_ll_4f_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=26
  elif [ $sname == "TTHtoNonBB" ] ;then 
    name="ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=27
  elif [ $sname == "VHtoNonBB" ] ;then 
    name="VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1"
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=28
  elif [ $sname == "TTZlow" ]; then 
      name="TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=29
  elif [ $sname == "GGHtoZZto4L" ] ; then 
    name="GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=30
  elif [ $sname == "WZMG" ] ; then
    name="WZJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"
    tag=V07-04-11
    nameNu=31
  elif [ $sname == "WGMG" ] ; then
    name="WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8"; 
    tag="CMS3_V07-06-03";
    path="/hadoop/cms/store/group/snt/run2_25ns_76MiniAODv2/VV/";
    nameNu=32
  elif [ $sname == "TTHtoNonBBext" ] ; then
    name="ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix_RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1"
    tag=V07-04-12 
    nameNu=33
  elif [ $sname == "ZZZ" ] ; then 
      name="ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=34
  elif [ $sname == "WWW" ] ; then 
      name="WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=35

  elif [ $sname == "TTZnlo" ] ; then 
    name="TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=36
  elif [ $sname == "TTWnlo" ] ; then 
    name="TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=37

  elif [ $sname == "WJets100To200" ] ; then 
    name="WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=38
  elif [ $sname == "WJets200To400" ] ; then 
    name="WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=39
  elif [ $sname == "WJets400To600" ] ; then 
    name="WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=40
  elif [ $sname == "WJets600To800" ] ; then 
    name="WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=41
  elif [ $sname == "WJets800To1200" ] ; then 
    name="WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=42

  elif [ $sname == "WJets1" ] ; then # 
    name="W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
    tag="V08-00-05";
    path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
    nameNu=43
  elif [ $sname == "WJets2" ] ; then # 
    name="W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
    tag="V08-00-05";
    path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
    nameNu=44
  elif [ $sname == "WJets3" ] ; then # 
    name="W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
    tag="V08-00-05";
    path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
    nameNu=45
  elif [ $sname == "WJets4" ] ; then # 
    name="W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
    tag="V08-00-05";
    path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
    nameNu=46

  elif [ $sname == "WZLO" ] ; then
    name="WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
    tag="V08-00-05";
    path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2/";
    nameNu=50

  elif [ $sname == "TWZ" ] ; then 
    name="ST_tWll_5f_LO_13TeV-MadGraph-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
      tag="V08-00-16";
    nameNu=60

  elif [ $sname == "TTZlofix" ] ; then
      name="ttZJets_13TeV_madgraphMLM_RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1";
      tag="V08-00-16";
      path="/hadoop/cms/store/group/snt/run2_moriond17/";
    nameNu=61

    #data
########################################
################ Era B #################
########################################
  elif [ $sname == "DataDoubleEGv1"    ]; then 
    isData="1"
    name="Run2016B_DoubleEG_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-04";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1000
  elif [ $sname == "DataDoubleMuonv1"  ]; then 
    isData="1"
    name="Run2016B_DoubleMuon_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-04";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1001
  elif [ $sname == "DataMuonEGv1"    ]; then 
    isData="1"
    name="Run2016B_MuonEG_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-04";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1002

  elif [ $sname == "DataDoubleEGv2"    ]; then 
    isData="1"
    name="Run2016B_DoubleEG_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-06";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1003
  elif [ $sname == "DataDoubleMuonv2"  ]; then 
    isData="1"
    name="Run2016B_DoubleMuon_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-06";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1004
  elif [ $sname == "DataMuonEGv2"    ]; then 
    isData="1"
    name="Run2016B_MuonEG_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-06";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1005

########################################
################ Era C #################
########################################
  elif [ $sname == "DataDoubleEGC"    ]; then 
    isData="1"
    name="Run2016C_DoubleEG_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-07";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1006
  elif [ $sname == "DataDoubleMuonC"  ]; then 
    isData="1"
    name="Run2016C_DoubleMuon_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-07";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1007
  elif [ $sname == "DataMuonEGC"    ]; then 
    isData="1"
    name="Run2016C_MuonEG_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-07";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1008
    

########################################
################ Era D #################
########################################
  elif [ $sname == "DataDoubleEGD"    ]; then 
    isData="1"
    name="Run2016D_DoubleEG_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-08";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1009
  elif [ $sname == "DataDoubleMuonD"  ]; then 
    isData="1"
    name="Run2016D_DoubleMuon_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-08";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1010
  elif [ $sname == "DataMuonEGD"    ]; then 
    isData="1"
    name="Run2016D_MuonEG_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-08";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1011

########################################
################ Era E #################
########################################
  elif [ $sname == "DataDoubleEGE"    ]; then 
    isData="1"
    name="Run2016E_DoubleEG_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-10";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1012
  elif [ $sname == "DataDoubleMuonE"  ]; then 
    isData="1"
    name="Run2016E_DoubleMuon_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-10";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1013
  elif [ $sname == "DataMuonEGE"    ]; then 
    isData="1"
    name="Run2016E_MuonEG_MINIAOD_PromptReco-v2/merged/";
    tag="V08-00-10";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1014

########################################
################ Era F #################
########################################
  elif [ $sname == "DataDoubleEGF"    ]; then 
    isData="1"
    name="Run2016F_DoubleEG_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-11";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1015
  elif [ $sname == "DataDoubleMuonF"  ]; then 
    isData="1"
    name="Run2016F_DoubleMuon_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-11";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1016
  elif [ $sname == "DataMuonEGF"    ]; then 
    isData="1"
    name="Run2016F_MuonEG_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-11";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1017

########################################
################ Era G #################
########################################
  elif [ $sname == "DataDoubleEGG"    ]; then 
    isData="1"
    name="Run2016G_DoubleEG_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-12";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1018
  elif [ $sname == "DataDoubleMuonG"  ]; then 
    isData="1"
    name="Run2016G_DoubleMuon_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-12";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1019
  elif [ $sname == "DataMuonEGG"    ]; then 
    isData="1"
    name="Run2016G_MuonEG_MINIAOD_PromptReco-v1/merged/";
    tag="V08-00-12";
    path="/hadoop/cms/store/group/snt/run2_data/";
    nameNu=1020

######################################
############## Era H #################
######################################
elif [ $sname == "DataDoubleEGH" ]; then
      name="Run2016H_DoubleEG_MINIAOD_PromptReco-v2/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=1021
      elif [ $sname == "DataDoubleMuonH" ]; then
      name="Run2016H_DoubleMuon_MINIAOD_PromptReco-v2/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=1022
      elif [ $sname == "DataMuonEGH" ]; then
      name="Run2016H_MuonEG_MINIAOD_PromptReco-v2/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=1023

      elif [ $sname == "DataDoubleEGHv3" ]; then
      name="Run2016H_DoubleEG_MINIAOD_PromptReco-v3/merged/";
      tag="V08-00-15";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=1024
      elif [ $sname == "DataDoubleMuonHv3" ]; then
      name="Run2016H_DoubleMuon_MINIAOD_PromptReco-v3/merged/";
      tag="V08-00-15";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=1025
      elif [ $sname == "DataMuonEGHv3" ]; then
      name="Run2016H_MuonEG_MINIAOD_PromptReco-v3/merged/";
      tag="V08-00-15";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=1026

      elif [ $sname == "DataJetHTH" ]; then
      name="Run2016H_JetHT_MINIAOD_PromptReco-v2/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=3000
      elif [ $sname == "DataJetHTHv3" ]; then
      name="Run2016H_JetHT_MINIAOD_PromptReco-v3/merged/";
      tag="V08-00-15";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=3001

######################################
############## Rereco #################
######################################

elif [ $sname == "DataDoubleEGRerecoB" ]; then
      name="Run2016B_DoubleEG_MINIAOD_23Sep2016-v3/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2000
      elif [ $sname == "DataDoubleMuonRerecoB" ]; then
      name="Run2016B_DoubleMuon_MINIAOD_23Sep2016-v3/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2001
      elif [ $sname == "DataMuonEGRerecoB" ]; then
      name="Run2016B_MuonEG_MINIAOD_23Sep2016-v3/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2002

      elif [ $sname == "DataDoubleEGRerecoC" ]; then
      name="Run2016C_DoubleEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2003
      elif [ $sname == "DataDoubleMuonRerecoC" ]; then
      name="Run2016C_DoubleMuon_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2004
      elif [ $sname == "DataMuonEGRerecoC" ]; then
      name="Run2016C_MuonEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2005

      elif [ $sname == "DataDoubleEGRerecoD" ]; then
      name="Run2016D_DoubleEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2006
      elif [ $sname == "DataDoubleMuonRerecoD" ]; then
      name="Run2016D_DoubleMuon_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2007
      elif [ $sname == "DataMuonEGRerecoD" ]; then
      name="Run2016D_MuonEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2008

      elif [ $sname == "DataDoubleEGRerecoE" ]; then
      name="Run2016E_DoubleEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2009
      elif [ $sname == "DataDoubleMuonRerecoE" ]; then
      name="Run2016E_DoubleMuon_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2010
      elif [ $sname == "DataMuonEGRerecoE" ]; then
      name="Run2016E_MuonEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2011

      elif [ $sname == "DataDoubleEGRerecoF" ]; then
      name="Run2016F_DoubleEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2012
      elif [ $sname == "DataDoubleMuonRerecoF" ]; then
      name="Run2016F_DoubleMuon_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2013
      elif [ $sname == "DataMuonEGRerecoF" ]; then
      name="Run2016F_MuonEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2014

      elif [ $sname == "DataDoubleEGRerecoG" ]; then
      name="Run2016G_DoubleEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2015
      elif [ $sname == "DataDoubleMuonRerecoG" ]; then
      name="Run2016G_DoubleMuon_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2016
      elif [ $sname == "DataMuonEGRerecoG" ]; then
      name="Run2016G_MuonEG_MINIAOD_23Sep2016-v1/merged/";
      tag="V08-00-14";
      path="/hadoop/cms/store/group/snt/run2_data/";
      isData="1";
      nameNu=2017

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
  elif [ $sname == "T1TTTT_main" ] 
  then
    name="SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1"
    isSignal=1
    nameNu=10001 

  elif [ $sname == "T1TTTT_main" ] 
  then
    name="SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1"
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

  #---------

  elif [ $sname == "T1TTTT_1050_50to775" ]
  then
    name="SMS-T1tttt_mGluino-1050_mLSP-50to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10047
  elif [ $sname == "T1TTTT_1050to1075_650to850"  ]
  then
    name="SMS-T1tttt_mGluino-1050to1075_mLSP-650to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10048 
  elif [ $sname == "T1TTTT_1225to1250_1to1025"   ]
  then
    name="SMS-T1tttt_mGluino-1225to1250_mLSP-1to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10049
  elif [ $sname == "T1TTTT_1550to1575_500to1175" ]
  then
    name="SMS-T1tttt_mGluino-1550to1575_mLSP-500to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10050
  elif [ $sname == "T1TTTT_600_1to225"           ]
  then
    name="SMS-T1tttt_mGluino-600_mLSP-1to225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10051
  elif [ $sname == "T1TTTT_600to625_250to375"    ]
  then
    name="SMS-T1tttt_mGluino-600to625_mLSP-250to375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10052
  elif [ $sname == "T1TTTT_900to950_200to700"    ]
  then
    name="SMS-T1tttt_mGluino-900to950_mLSP-200to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=10053

  elif [ $sname == "T5QQQQVV_main"  ]
  then
    name="SMS-T5qqqqVV_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1"
    isSignal=1
    nameNu=11000
  elif [ $sname == "T5QQQQVV_dm20" ]
  then
    name="SMS-T5qqqqVV_dM20_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1"
    isSignal=1
    nameNu=11001
  elif [ $sname == "T5QQQQVV_1200To1275_1to1150" ]
  then
    name="SMS-T5qqqqVV_mGluino-1200To1275_mLSP-1to1150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=11002
  elif [ $sname == "T5QQQQVV_1300To1375_1to1250" ]
  then
    name="SMS-T5qqqqVV_mGluino-1300To1375_mLSP-1to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=11003
  elif [ $sname == "T5QQQQVV_1400To1550_1To1275" ]
  then
    name="SMS-T5qqqqVV_mGluino-1400To1550_mLSP-1To1275_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=11004
  elif [ $sname == "T5QQQQVV_1600To1750_1To950"  ]
  then
    name="SMS-T5qqqqVV_mGluino-1600To1750_mLSP-1To950_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=11005
  elif [ $sname == "T5QQQQVV_600To675_1to550"    ]
  then
    name="SMS-T5qqqqVV_mGluino-600To675_mLSP-1to550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=11006
  elif [ $sname == "T5QQQQVV_700To775_1To650"    ]
  then
    name="SMS-T5qqqqVV_mGluino-700To775_mLSP-1To650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=11007
  elif [ $sname == "T5QQQQVV_800To975_1To850"    ]
  then
    name="SMS-T5qqqqVV_mGluino-800To975_mLSP-1To850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=11008

  elif [ $sname == "T6TTWW_main"  ]
  then
      name="SMS-T6ttWW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1"
      isSignal=1
      nameNu=12000

  elif [ $sname == "T6TTWW_50_300to600_75to125"  ]
  then
    name="SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-75to125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=12001
  elif [ $sname == "T6TTWW_50_350to600_150to400" ]
  then
    name="SMS-T6ttWW_mLSP50_mSbottom-350to600_mChargino-150to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=12002
  elif [ $sname == "T6TTWW_50_625to950_500to850" ]
  then
    name="SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-500to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=12003
  elif [ $sname == "T6TTWW_50_625to950_550to875" ]
  then
    name="SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-550to875_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=12004
  elif [ $sname == "T6TTWW_50_625to950_75to125"  ]
  then
    name="SMS-T6ttWW_mLSP50_mSbottom-625to950_mChargino-75to125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=12005
  elif [ $sname == "T6TTWW_50_650to950_150to750" ]
  then
    name="SMS-T6ttWW_mLSP50_mSbottom-650to950_mChargino-150to750_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=12006
  elif [ $sname == "T6TTWW_50_300to600_150to500" ]
  then
    name="SMS-T6ttWW_mLSP50_mSbottom-300to600_mChargino-150to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1
    nameNu=12007

   elif [ $sname == "T5QQQQWW_1025to1200_0to1175" ]
   then
     nameNu=13001;
    isSignal=1
     name="SMS-T5qqqqWW_mGl-1025to1200_mLSP-0to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T5QQQQWW_1225to1400_0to1175" ]
   then
     nameNu=13002;
     isSignal=1
     name="SMS-T5qqqqWW_mGl-1225to1400_mLSP-0to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T5QQQQWW_1425to1600_0to1175" ] 
   then
     nameNu=13003;
     isSignal=1
     name="SMS-T5qqqqWW_mGl-1425to1600_mLSP-0to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T5QQQQWW_1650to1700_0to1150" ]  
   then
     nameNu=13004;
     isSignal=1
     name="SMS-T5qqqqWW_mGl-1650to1700_mLSP-0to1150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T5QQQQWW_600to800_0to725" ]    
   then
     nameNu=13005;
     isSignal=1
     name="SMS-T5qqqqWW_mGl-600to800_mLSP-0to725_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T5QQQQWW_825to1000_0to925" ]   
   then
     nameNu=13006;
     isSignal=1
     name="SMS-T5qqqqWW_mGl-825to1000_mLSP-0to925_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T1TTBB_main" ]     
   then
     nameNu=14001;
     isSignal=1
     name="SMS-T1ttbb_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1"
   elif [ $sname == "T1TTBB_1425to1600_0to1250" ]     
   then
     nameNu=14002;
     isSignal=1
     name="SMS-T1ttbb_mGl-1425to1600_mLSP-0to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T1TTBB_1425to1600_1100to1350" ]  
   then
     nameNu=14003;
     isSignal=1
     name="SMS-T1ttbb_mGl-1425to1600_mLSP-1100to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T1TTBB_600to800_275to575" ]      
   then
     nameNu=14004;
     isSignal=1
     name="SMS-T1ttbb_mGl-600to800_mLSP-275to575_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
   elif [ $sname == "T1TTBB_1225to1400_900to1175" ]      
   then
     name="SMS-T1ttbb_mGl-1225to1400_mLSP-900to1175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
     isSignal=1;
     nameNu=14005;
   elif [ $sname == "T1TTBB_1650to2000_0to1450" ]      
   then
     name="SMS-T1ttbb_mGl-1650to2000_mLSP-0to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
     isSignal=1;
     nameNu=14006;
   elif [ $sname == "T1TTBB_825to1000_500to775" ]      
   then
     name="SMS-T1ttbb_mGl-825to1000_mLSP-500to775_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
     isSignal=1;
     nameNu=14007;
  elif [ $sname == "T1TTBB_1650to1750_1350to1450" ]    
    then
    nameNu=14008; 
    name="SMS-T1ttbb_mGl-1650to1750_mLSP-1350to1450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"; 
    isSignal=1;
  elif [ $sname == "T1TTBB_825to1000_0to625" ]         
    then
    nameNu=14009; 
    name="SMS-T1ttbb_mGl-825to1000_mLSP-0to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"; 
    isSignal=1;
  elif [ $sname == "T1TTBB_600to800_0to450" ]          
    then
    nameNu=14010; 
    name="SMS-T1ttbb_mGl-600to800_mLSP-0to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"; 
    isSignal=1;
   elif [ $sname == "T5TTTT_dm175" ]      
   then
     nameNu=15001;
     isSignal=1
     name="SMS-T5tttt_dM175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1";
  elif [ $sname == "T5ttttDM175_1025to1200_700to925" ]    
    then
    nameNu=15002;
    name="SMS-T5ttttDM175_mGl-1025to1200_mLSP-700to925_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"; 
    isSignal=1;
  elif [ $sname == "T5ttttDM175_1225to1400_0to1050" ]     
    then
    nameNu=15003;
    name="SMS-T5ttttDM175_mGl-1225to1400_mLSP-0to1050_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"; 
    isSignal=1;
  elif [ $sname == "T5ttttDM175_1225to1400_900to1125" ]   
    then
    nameNu=15004;
    name="SMS-T5ttttDM175_mGl-1225to1400_mLSP-900to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
    isSignal=1;
  elif [ $sname == "T5ttttDM175_1425to1625_1100to1300" ]  
    then
    nameNu=15005;
    name="SMS-T5ttttDM175_mGl-1425to1625_mLSP-1100to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
    isSignal=1;
  elif [ $sname == "T5ttttDM175_600to800_275to525" ]      
    then
    nameNu=15006;
    name="SMS-T5ttttDM175_mGl-600to800_mLSP-275to525_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
    isSignal=1;
  elif [ $sname == "T5ttttDM175_825to1000_0to650" ]       
    then
    nameNu=15007;
    name="SMS-T5ttttDM175_mGl-825to1000_mLSP-0to650_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
    isSignal=1;
  elif [ $sname == "T5tttt_degen_1225to1400_1075to1275" ]       
    then
    nameNu=16001;
    name="SMS-T5tttt_degen_mGl-1225to1400_mLSP-1075to1275_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1;
  elif [ $sname == "T5tttt_degen_1425to1600_1275to1375" ]       
    then
    nameNu=16002;
    name="SMS-T5tttt_degen_mGl-1425to1600_mLSP-1275to1375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1;
  elif [ $sname == "T5tttt_degen_825to1000_0to825" ]       
    then
    nameNu=16003;
    name="SMS-T5tttt_degen_mGl-825to1000_mLSP-0to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1;
  elif [ $sname == "T5tttt_degen_825to1000_675to875" ]       
    then
    nameNu=16004;
    name="SMS-T5tttt_degen_mGl-825to1000_mLSP-675to875_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1;
  elif [ $sname == "T5tttt_degen_1225to1400_0to1225" ]
    then
    nameNu=16005;
    name="SMS-T5tttt_degen_mGl-1225to1400_mLSP-0to1225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
    isSignal=1;
  elif [ $sname == "T5tttt_degen_600to800_450to675" ]  
    then
    nameNu=16006;
    name="SMS-T5tttt_degen_mGl-600to800_mLSP-450to675_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"; 
    isSignal=1;
  elif [ $sname == "T5tttt_degen_1425to1600_0to1350" ] 
    then
    nameNu=16007;
    name="SMS-T5tttt_degen_mGl-1425to1600_mLSP-0to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";
    isSignal=1;
  elif [ $sname == "T5tttt_degen_1650to1700_0to1350" ] 
    then
    nameNu=16008;
    name="SMS-T5tttt_degen_mGl-1650to1700_mLSP-0to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"; 
    isSignal=1;
  elif [ $sname == "T5tttt_degen_1025to1200_0to1025" ] 
    then
    nameNu=16009;
    isSignal=1;
    name="SMS-T5tttt_degen_mGl-1025to1200_mLSP-0to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1";

  elif [ $sname == "T5TTCC_main" ] 
    then
    nameNu=17001
    isSignal=1;
    name="SMS-T5ttcc_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v3";
  elif [ $sname == "T5ttcc_1025to1200_875to1075" ] 
    then
    nameNu=17002
    isSignal=1;
    name="SMS-T5ttcc_mGl-1025to1200_mLSP-875to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttcc_825to1000_0to825" ] 
    then
    nameNu=17003
    isSignal=1;
    name="SMS-T5ttcc_mGl-825to1000_mLSP-0to825_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttcc_600to800_450to675" ] 
    then
    nameNu=17004
    isSignal=1;
    name="SMS-T5ttcc_mGl-600to800_mLSP-450to675_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttcc_1225to1400_1075to1225" ] 
    then
    nameNu=17005
    isSignal=1;
    name="SMS-T5ttcc_mGl-1225to1400_mLSP-1075to1225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttcc_1225to1400_0to1225" ] 
    then
    nameNu=17006
    isSignal=1;
    name="SMS-T5ttcc_mGl-1225to1400_mLSP-0to1225_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttcc_1650to1700_0to1350" ] 
    then
    nameNu=17007
    isSignal=1;
    name="SMS-T5ttcc_mGl-1650to1700_mLSP-0to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttcc_1425to1525_1275to1375" ] 
    then
    nameNu=17008
    isSignal=1;
    name="SMS-T5ttcc_mGl-1425to1525_mLSP-1275to1375_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttcc_1025to1200_0to1025" ] 
    then
    nameNu=17009
    isSignal=1;
    name="SMS-T5ttcc_mGl-1025to1200_mLSP-0to1025_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttcc_600to800_0to625" ] 
    then
    nameNu=17010
    isSignal=1;
    name="SMS-T5ttcc_mGl-600to800_mLSP-0to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15FSPremix-MCRUN2_74_V9-v1"
  elif [ $sname == "T5ttcc_1425to1600_0to1350" ] 
    then
    nameNu=17011
    isSignal=1;
    name="SMS-T5ttcc_mGl-1425to1600_mLSP-0to1350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15FSPremix-MCRUN2_74_V9-v1"
  elif [ $sname == "T5ttttDM175_1425to1600_0to1250" ] 
    then
    nameNu=15008 
    isSignal=1;
    name="SMS-T5ttttDM175_mGl-1425to1600_mLSP-0to1250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttttDM175_1025to1200_0to850" ] 
    then
    nameNu=15009
    isSignal=1;
    name="SMS-T5ttttDM175_mGl-1025to1200_mLSP-0to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttttDM175_1625to1700_0to1300" ] 
    then
    nameNu=15010
    isSignal=1;
    name="SMS-T5ttttDM175_mGl-1625to1700_mLSP-0to1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T5ttttDM175_600to800_0to450" ] 
    then
    nameNu=15011
    isSignal=1;
    name="SMS-T5ttttDM175_mGl-600to800_mLSP-0to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
  elif [ $sname == "T1TTBB_1025to1200_700to975" ] 
    then
    nameNu=14011
    name="SMS-T1ttbb_mGl-1025to1200_mLSP-700to975_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1;
  elif [ $sname == "T5tttt_degen_600to800_0to625" ]
    then
    nameNu=16010;
    name="SMS-T5tttt_degen_mGl-600to800_mLSP-0to625_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1"
    isSignal=1;
  elif [ $sname == "T5tttt_degen_1025to1200_875to1075" ]
    then
    nameNu=16011;
    name="SMS-T5tttt_degen_mGl-1025to1200_mLSP-875to1075_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15MiniAODv2-FastAsympt25ns_74X_mcRun2_asymptotic_v2-v2"
    isSignal=1;
  elif [ $sname == "T1TTBB_1025to1200_0to850" ]
    then
    nameNu=14012;
    name="SMS-T1ttbb_mGl-1025to1200_mLSP-0to850_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15FSPremix-MCRUN2_74_V9-v1"
    isSignal=1;
  elif [ $sname == "ANA1" ] 
    then 
    nameNu=100001;
    name="ana_test1"
    isSignal=100;
  elif [ $sname == "ANA2" ] 
    then 
    nameNu=100002;
    name="ana_test2"
    isSignal=101;
  elif [ $sname == "ANA3" ] 
    then 
    nameNu=100003;
    name="ana_test3"
    isSignal=102;

  
  #default
  else 
    name=$sname 
    tag=V07-04-08
    nameNu=999999
  fi

  #fastsim path
  if [ "$isSignal" == "1" ]; then path="/hadoop/cms/store/group/snt/run2_25ns_80MiniAODv2_fastsim"; tag="V08-00-09"; fi

  # #Data vs. MC variables
  # if [ `echo $name | tr '_' ' ' | awk '{print $1}' | cut -c 1-7` == "Run2015" ]
  # then 
  #   infix="merged/"
  #   isData="1"
  # else 
  #   infix="" 
  #   isData="0"
  # fi

  #Get number of files
  numberOfFiles=$((`ls -l $path/$name/$tag/merged_ntuple_*.root | wc -l`))
  echo "number of files: $numberOfFiles"
  echo "dir: $path/$name/$tag"
  
  if [ "$DOONE" == "1" ]; then
    numberOfFiles=1
  fi

  #Submit all of them
  for (( i=0; i<$numberOfFiles; i++))
  do
    sname_lower=`echo ${sname,,}`
    number=$(( $i + 1 ))

    #Except they've finished
    if [ -e /hadoop/cms/store/user/$USER/condor/$SPPATH/${sname_lower}_${number}.root ] 
    then 
      continue
    fi

    echo "-------------"
    echo "Working on $sname $number"


    # Or if they're still running
    result=$( grep " ${nameNu} ${number} ${USER}" jobssubmitted.txt | wc -l )
    if [ "$result" == "1" ] ; then
        if [ "$verbose" == "true" ]; then echo "already submitted!"; fi
        let "nSubmitted=$nSubmitted+1"
        continue
    fi
    # condorExecutable.sh 2 147 namin
    # condorExecutable.sh 1003 116 namin


  
    # #Or if they're still running
    # if [ -e /data/tmp/${USER}/logs/condorLog_${nameNu}_${number}.log ] 
    # then
    #   while read line
    #   do
    #     if [ `echo $line | awk '{ print $1 }'` == "Cluster" ] 
    #     then
    #       cluster=`echo $line | awk '{ print $3 }'`
    #     elif [ `echo $line | awk '{ print $1 }'` == "Proc" ] 
    #     then
    #       process=`echo $line | awk '{ print $3 }'`
    #     fi
    #   done < /data/tmp/${USER}/logs/condorLog_${nameNu}_${number}.log
    #   jobid="$cluster.$process"
    #   condor_q $jobid > temp.txt
    #   result=`more temp.txt | awk 'END{print $1}'`
    #   if [ "$result" == "$jobid" ] 
    #   then
    #     echo "already submitted!"
    #     let "nSubmitted=$nSubmitted+1"
    #     continue
    #   fi
    # fi

    echo "submitting..."
    let "nSubmitted=$nSubmitted+1"

    #Make condor file
    cp condorFileTemplate condorFile
    if [ "$xrootdbroken" == "1" ]
    then
      sed -i 's/T2_US_UCSD,T2_US_Wisconsin,T2_US_Florida,T2_US_Purdue,T2_US_Nebraska,T2_US_Caltech/T2_US_UCSD/' condorFile
    fi
  
    #submit it
    sed -i s/ARG1/$nameNu/g condorFile
    sed -i s/ARG2/$number/g condorFile
    sed -i s/ARG3/$USER/g condorFile
    sed -i s/ARG4/$SPPATH/g condorFile
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

# vim: set sw=2:
