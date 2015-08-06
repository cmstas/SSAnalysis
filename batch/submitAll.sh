#!/bin/bash

pathPublic="/hadoop/cms/store/group/snt/run2_50ns"
pathPrivate="/hadoop/cms/store/user/cgeorge/privateSusySignalsSS"
pathData="/hadoop/cms/store/group/snt/run2_data"
tag_data="V07-04-06"
tag_MC="V07-04-03"

nSubmitted=0

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

#Then submit jobs
ptrel="4"
for expt in "0" "1"
do
  nIter=0
  for sname in "DY_high" "DATAMUEG" "TTPOWHEG" "TTBAR"  "WZ" "DY_low" "WJets" "DataDoubleEG" "DataDoubleMuon" # "SINGLETOP1" "SINGLETOP2" "SINGLETOP3" "SINGLETOP4"  
  do
    #Iter
    nIter=$(( $nIter + 1 ))

    #Get Name
    if   [ $sname == "TTBAR"           ]
    then 
      name="TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1"
      nameNu=0
    elif [ $sname == "WZ"              ]
    then 
      name="WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2";
      nameNu=3
    elif [ $sname == "DY_low"          ]; 
      then name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
      nameNu=6
    elif [ $sname == "DY_high"         ]; 
      then name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2";
      nameNu=7
    elif [ $sname == "WJets"           ]; 
      then name="WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
      nameNu=8
    elif [ $sname == "TTPOWHEG"        ]; 
      then name="TT_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v4";
      nameNu=11
    elif [ $sname == "DataDoubleMuon"  ]; 
      then name="Run2015B_DoubleMuon_MINIAOD_PromptReco-v1";
      nameNu=9
    elif [ $sname == "DataDoubleEG"    ]; 
      then name="Run2015B_DoubleEG_MINIAOD_PromptReco-v1";
      nameNu=10
    elif [ $sname == "SINGLETOP1"    ]; 
      then name="ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1"
      nameNu=12
    elif [ $sname == "SINGLETOP2"    ]; 
      then name="ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2"
      nameNu=13
    elif [ $sname == "SINGLETOP3"    ]; 
      then name="ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1"
      nameNu=14
    elif [ $sname == "SINGLETOP4"    ]; 
      then name="ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1"
      nameNu=15
    elif [ $sname == "DATAMUONEG"    ]; 
      then name="Run2015B_MuonEG_MINIAOD_PromptReco-v1";
      nameNu=16
    else 
      name=$sname 
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
      tag=$tag_data
    else 
      infix="" 
      isData="0"
      path=$pathPublic
      tag=$tag_MC
    fi

    #Get number of files
    numberOfFiles=$((`ls -l $path/$name/$infix$tag | wc -l` - 1))
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
  
      #submit it
      cp condorFileTemplate condorFile
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
