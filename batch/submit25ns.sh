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
sed -i "s/cgeorge/$USER/" condorExecutable.sh

#Then submit jobs
for expt in "0" # "1"
do
  nIter=0
  for sname in "TTBAR"  "DataDoubleMuonD" "DataDoubleEGD" "DATAMUONEGD" "WJets" "WZ3LNu" "TTZL" "TTBAR" "TTW" "DY_high" "DY_low" "T1TTTT_1200" "T1TTTT_1500" "WGToLNuG" "TTG" "TTHtoNonBB" "VHtoNonBB"  "TZQ"  "TTTT"  "WWDPS"  "WZZ" "T5QQQQWZ_1200_1000_800" "T5QQQQDeg_1000_300_285_280" "T5QQQQWZ_1500_800_100" "T5ttttDeg_1000_300_280" "T2ttDeg_350_330" "T5QQQQZZ_1200_1000_800" "T6TTWW_600_425_50" "T2TT_ARXIV" "T5QQQQZZ_1500_800_100" "T6TTWW_650_150_50" "SINGLETOP1" "SINGLETOP2" "SINGLETOP3" "SINGLETOP4" "SINGLETOP5" "TTWQQ" "TTZQ" "WWZ" "ZZ"
  do
    path="/hadoop/cms/store/group/snt/run2_25ns"
    #Iter
    nIter=$(( $nIter + 1 ))

    #Get Name
    if   [ $sname == "TTBAR"           ]
    then 
      name="TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2"
      tag=V07-04-08
      nameNu=0
    elif   [ $sname == "TTWQQ"           ]
    then 
      name="TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08 
      nameNu=23
    elif   [ $sname == "WZ3LNu"          ]
    then
      name="WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=22
    elif [ $sname == "WZ"              ]
    then 
      name="WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=3
    elif [ $sname == "WWZ"             ]
    then 
      name="WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=49
    elif [ $sname == "ZZ"              ]
    then 
      name="ZZTo4L_13TeV_powheg_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=50
    elif [ $sname == "DY_low"          ]; 
      then name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=6
    elif [ $sname == "DY_high"         ]; 
      then name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3"
      tag=V07-04-08
      nameNu=7
    elif [ $sname == "WJets"           ]; 
      then name="WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=8
    elif [ $sname == "SINGLETOP1"    ]; 
      then name="ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=12
    elif [ $sname == "SINGLETOP2"    ]; 
      then name="ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=13
    elif [ $sname == "SINGLETOP3"    ]; 
      then name="ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=14
    elif [ $sname == "SINGLETOP4"    ]; 
      then name="ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=15
    elif [ $sname == "SINGLETOP5"    ]; 
      then name="ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=17
    elif [ $sname == "DataDoubleMuon"  ]; 
      then name="Run2015B_DoubleMuon_MINIAOD_PromptReco-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=9
    elif [ $sname == "DataDoubleEG"    ]; 
      then name="Run2015B_DoubleEG_MINIAOD_PromptReco-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=10
    elif [ $sname == "DATAMUONEG"    ]; 
      then name="Run2015B_MuonEG_MINIAOD_PromptReco-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=16
    elif [ $sname == "DataDoubleMuon2"  ]; 
      then name="Run2015B_DoubleMuon_MINIAOD_17Jul2015-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=25
    elif [ $sname == "DataDoubleEG2"    ]; 
      then name="Run2015B_DoubleEG_MINIAOD_17Jul2015-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=26
    elif [ $sname == "DATAMUONEG2"    ]; 
      then name="Run2015B_MuonEG_MINIAOD_17Jul2015-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=24
    elif [ $sname == "DataDoubleMuonC"  ]; 
      then name="Run2015C_DoubleMuon_MINIAOD_PromptReco-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=20
    elif [ $sname == "DataDoubleEGC"    ]; 
      then name="Run2015C_DoubleEG_MINIAOD_PromptReco-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=21
    elif [ $sname == "DATAMUONEGC"    ]; 
      then name="Run2015C_MuonEG_MINIAOD_PromptReco-v1";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-08
      nameNu=19
    elif [ $sname == "DataDoubleMuonD"  ]; 
      then name="Run2015D_DoubleMuon_MINIAOD_PromptReco-v3";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-09
      nameNu=46
    elif [ $sname == "DataDoubleEGD"    ]; 
      then name="Run2015D_DoubleEG_MINIAOD_PromptReco-v3";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-09
      nameNu=45
    elif [ $sname == "DATAMUONEGD"    ]; 
      then name="Run2015D_MuonEG_MINIAOD_PromptReco-v3";
      path="/hadoop/cms/store/group/snt/run2_data"
      tag=V07-04-09
      nameNu=47
    elif [ $sname == "TTW" ]
    then
      name="TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1" 
      tag=V07-04-08
      nameNu=1
    elif [ $sname == "TTZL" ]
    then
      name="TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=2
    elif [ $sname == "TTZQ" ]
    then
      name="TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=18
    elif [ $sname == "WGToLNuG" ] 
    then
      name="WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=27
    elif [ $sname == "TTG" ] 
    then
      name="TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      tag=V07-04-08
      nameNu=28
    elif [ $sname == "TTHtoNonBB" ] 
    then
      name=ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1
      tag=V07-04-08
      nameNu=29
    elif [ $sname == "VHtoNonBB" ] 
    then
      name=VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1
      tag=V07-04-08
      nameNu=30
    elif [ $sname == "TZQ" ] 
    then
      name=tZq_ll_4f_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2
      tag=V07-04-08
      nameNu=31
    elif [ $sname == "TTTT" ] 
    then
      name=TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1
      tag=V07-04-08
      nameNu=32
    elif [ $sname == "WWDPS" ] 
    then
      name=WW_DoubleScattering_13TeV-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1
      tag=V07-04-08
      nameNu=33
    elif [ $sname == "WZZ" ] 
    then
      name=WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1
      tag=V07-04-08
      nameNu=34
    elif [ $sname == "QQWW" ] 
    then
      name=WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1
      tag=V07-04-08
      nameNu=48
    elif [ $sname == "T1TTTT_1500" ] 
    then
      name=SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1
      tag=V07-04-08
      nameNu=4
    elif [ $sname == "T1TTTT_1200" ] 
    then
      name=SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1
      tag=V07-04-08
      nameNu=5
    elif [ $sname == "T5QQQQWZ_1200_1000_800" ] 
    then
      name=T5qqqqWZ_mGo1200_mCh1000_mChi800_lep  
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=35
    elif [ $sname == "T5QQQQDeg_1000_300_285_280" ] 
    then
      name=T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_23bodydec
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=36
    elif [ $sname == "T5QQQQWZ_1500_800_100" ] 
    then
      name=T5qqqqWZ_mGo1500_mCh800_mChi100_lep   
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=37
    elif [ $sname == "T5ttttDeg_1000_300_280" ] 
    then
      name=T5ttttDeg_mGo1000_mStop300_mChi280_4bodydec
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=38
    elif [ $sname == "T2ttDeg_350_330" ] 
    then
      name=T2ttDeg_mStop350_mChi330_4bodydec  
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=39
    elif [ $sname == "T5QQQQZZ_1200_1000_800" ] 
    then
      name=T5qqqqZZ_mGo1200_mCh1000_mChi800_lep  
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=40
    elif [ $sname == "T6TTWW_600_425_50" ] 
    then
      name=T6ttWW_600_425_50
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=41
    elif [ $sname == "T2TT_ARXIV" ] 
    then
      name=T2tt_arxiv1507_01601               
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=42
    elif [ $sname == "T5QQQQZZ_1500_800_100" ] 
    then
      name=T5qqqqZZ_mGo1500_mCh800_mChi100_lep   
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=43
    elif [ $sname == "T6TTWW_650_150_50" ] 
    then
      name=T6ttWW_650_150_50
      path="/nfs-7/userdata/ss2015/signalSamples/V07-04-08/"
      tag=""
      nameNu=44
    else 
      name=$sname 
      tag=V07-04-08
      nameNu=9999
    fi

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
      if [ -e /hadoop/cms/store/user/$USER/condor/ss_13_babies/$SPPATH/${sname_lower}_${number}_$expt.root ] 
      then 
        continue
      else
        "Redoing, this does not exist: /hadoop/cms/store/user/$USER/condor/ss_13_babies/$SPPATH/${sname_lower}_${number}_$expt.root"
      fi

      echo "-------------"
      echo "Working on $sname $number $expt"
  
      #Or if they're still running
      if [ -e logs/condorLog_${sname}_${number}_$expt.log ] 
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
        done < logs/condorLog_${sname}_${number}_$expt.log
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
      sed -i s/ARG4/$USER/g condorFile
      sed -i s/ARG5/$expt/g condorFile
      sed -i "s,USER_PROXY,$pathToProxy,g" condorFile
      condor_submit condorFile
    done
  done
done

if [ "$nSubmitted" == "0" ]
then 
  return 1
else 
  return 0
fi

