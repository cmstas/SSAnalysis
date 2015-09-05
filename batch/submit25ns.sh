#!/bin/bash

xrootdbroken="1"

pathPublic="/hadoop/cms/store/group/snt/run2_25ns"
pathPrivate="/hadoop/cms/store/user/cgeorge/privateSusySignalsSS"
pathData="/hadoop/cms/store/group/snt/run2_data"

nSubmitted=0

#Source this stupid thing
pushd ..
. setup.sh
popd

#Copy main.C to main.cc
cp main.cc.norun main.cc
if [ "$xrootdbroken" == "1" ]
then
  sed -i 's/T2_US_UCSD,T2_US_WISCONSIN,T2_US_FLORIDA,T2_US_PURDUE,T2_US_NEBRASKA,T2_US_MIT,T2_US_CALTECH/T2_US_UCSD/' condorFile
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
ptrel="4"
for expt in "0" # "1"
do
  nIter=0
  for sname in  "DATAMUONEG2" "DATAMUONEG" "DataDoubleEG2" "DataDoubleMuon2" "DY_low"  "SINGLETOP1" "SINGLETOP2" "SINGLETOP3" "SINGLETOP4" "SINGLETOP5" "TTWQQ" "WJets"  "TTBAR"  "TTZL" "TTW" "TTZQ" "WZ" "DY_high"   "DataDoubleEG"  "DataDoubleMuon" "WZ3LNu"  "DATAMUONEGC" "DataDoubleEGC" "DataDoubleMuonC"
  do
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
      tag=V07-04-08
      nameNu=9
    elif [ $sname == "DataDoubleEG"    ]; 
      then name="Run2015B_DoubleEG_MINIAOD_PromptReco-v1";
      tag=V07-04-08
      nameNu=10
    elif [ $sname == "DATAMUONEG"    ]; 
      then name="Run2015B_MuonEG_MINIAOD_PromptReco-v1";
      tag=V07-04-08
      nameNu=16
    elif [ $sname == "DataDoubleMuon2"  ]; 
      then name="Run2015B_DoubleMuon_MINIAOD_17Jul2015-v1";
      tag=V07-04-08
      nameNu=25
    elif [ $sname == "DataDoubleEG2"    ]; 
      then name="Run2015B_DoubleEG_MINIAOD_17Jul2015-v1";
      tag=V07-04-08
      nameNu=26
    elif [ $sname == "DATAMUONEG2"    ]; 
      then name="Run2015B_MuonEG_MINIAOD_17Jul2015-v1";
      tag=V07-04-08
      nameNu=24
    elif [ $sname == "DataDoubleMuonC"  ]; 
      then name="Run2015C_DoubleMuon_MINIAOD_PromptReco-v1";
      tag=V07-04-08
      nameNu=20
    elif [ $sname == "DataDoubleEGC"    ]; 
      then name="Run2015C_DoubleEG_MINIAOD_PromptReco-v1";
      tag=V07-04-08
      nameNu=21
    elif [ $sname == "DATAMUONEGC"    ]; 
      then name="Run2015C_MuonEG_MINIAOD_PromptReco-v1";
      tag=V07-04-08
      nameNu=19
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
    else 
      name=$sname 
      tag=V07-04-07
      nameNu=9999
    fi

    #Get pTRel suffix
    if [ "$ptrel" == "4" ] 
    then
      ptrelsuf=""
    fi
  
    #Data vs. MC variables
    if [ `echo $name | tr '_' ' ' | awk '{print $1}' | cut -c 1-7` == "Run2015" ]
    then 
      infix="merged/"
      path=$pathData
      isData="1"
    else 
      infix="" 
      isData="0"
      path=$pathPublic
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
      if [ -e /hadoop/cms/store/user/$USER/condor/ss_13_babies/${sname_lower}_${number}${ptrelsuf}_$expt.root ] 
      then 
        continue
      else
        "Redoing, this does not exist: /hadoop/cms/store/user/$USER/condor/ss_13_babies/${sname_lower}_${number}${ptrelsuf}_$expt.root"
      fi

      echo "-------------"
      echo "Working on $sname $number $ptrel $expt"
  
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
  
      #submit it
      sed -i s/ARG1/$nameNu/g condorFile
      sed -i s/ARG2/$number/g condorFile
      sed -i s/ARG3/$ptrel/g condorFile
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

