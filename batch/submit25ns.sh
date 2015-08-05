#!/bin/bash

pathPublic="/hadoop/cms/store/group/snt/run2_25ns"
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
  for sname in "DY_high_25" "TTBAR_25"  "WZ_25" "DY_low_25" "WJets_25" "SINGLETOP1_25" "SINGLETOP2_25" "SINGLETOP3_25" "SINGLETOP4_25" # "DataDoubleEG" "DataDoubleMuon" "DATAMUEG" 
  do
    #Iter
    nIter=$(( $nIter + 1 ))

    #Get Name
    if   [ $sname == "TTBAR_25"           ]
    then 
      name="TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2"
      nameNu=20
    elif [ $sname == "WZ_25"              ]
    then 
      name="WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      nameNu=21
    elif [ $sname == "DY_low_25"          ]; 
      then name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      nameNu=22
    elif [ $sname == "DY_high_25"         ]; 
      then name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3"
      nameNu=23
    elif [ $sname == "WJets_25"           ]; 
      then name="WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      nameNu=24
    elif [ $sname == "SINGLETOP1_25"    ]; 
      then name="ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      nameNu=12
    elif [ $sname == "SINGLETOP2_25"    ]; 
      then name="ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      nameNu=13
    elif [ $sname == "SINGLETOP3_25"    ]; 
      then name="ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      nameNu=14
    elif [ $sname == "SINGLETOP4_25"    ]; 
      then name="ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      nameNu=15
    elif [ $sname == "SINGLETOP5_25"    ]; 
      then name="ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1"
      nameNu=17
    else 
      name=$sname 
      nameNu=9999
    fi

    #Get pTRel suffix
    if [ "$ptrel" == "4" ] 
    then
      ptrelsuf="_25"
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
