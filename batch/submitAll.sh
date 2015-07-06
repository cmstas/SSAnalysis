#!/bin/bash

pathPublic="/hadoop/cms/store/group/snt/run2_50ns"
pathPrivate="/hadoop/cms/store/user/cgeorge/privateSusySignalsSS"
tag="V07-04-03"

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
ptrel="MultiIso"
for expt in "0" "1"
do
  nIter=0
  for sname in "TTBAR" "WZ" "DY_low" "DY_high" "WJets"
  do
    #Iter
    nIter=$(( $nIter + 1 ))

    #Get Path
    if [ "$nIter" -lt "17" ]; then path=$pathPublic; else path=$pathPrivate; fi 

    #Get Name
    if   [ $sname == "TTBAR"       ]; then name="TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
    elif [ $sname == "WZ"          ]; then name="WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2";
    elif [ $sname == "DY_low"      ]; then name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
    elif [ $sname == "DY_high"     ]; then name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2";
    elif [ $sname == "WJets"       ]; then name="WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
    else name=$sname 
    fi

    #Get pTRel suffix
    if [ "$ptrel" == "MultiIso" ] 
    then
      ptrelsuf=""
    fi
  
    #Get number of files
    numberOfFiles=$((`ls -l $path/$name/$tag | wc -l` - 1))
  
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
      sed -i s/ARG1/$sname/g condorFile
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
