#!/bin/bash

pathPublic="/hadoop/cms/store/group/snt/phys14"
pathPrivate="/hadoop/cms/store/user/cgeorge/privateSusySignalsSS"
tag="V07-02-08"

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
nIter=0
for ptrel in "4"
do
  for sname in "TTW" "TTZ" "TTBAR" "WZ" "T1TTTT_1500" "T1TTTT_1200" "T5qqqqWW_1200_1000_800" "T5qqqqWW_deg_1000_315_300"
  do
    #Iter
    nIter=$(( $nIter + 1 ))

    #Get Path
    if [ "$nIter" -lt "7" ]; then path=$pathPublic; else path=$pathPrivate; fi 

    #Get Name
    if   [ $sname == "TTW"         ]; then name="TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; 
    elif [ $sname == "TTZ"         ]; then name="TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; 
    elif [ $sname == "TTBAR"       ]; then name="TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; 
    elif [ $sname == "WZ"          ]; then name="WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; 
    elif [ $sname == "T1TTTT_1500" ]; then name="SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
    elif [ $sname == "T1TTTT_1200" ]; then name="SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
    else name=$sname 
    fi

    #Get pTRel suffix
    if [ "$ptrel" == "0" ] 
    then
      ptrelsuf=""
    elif [ "$ptrel" == "1" ] 
    then
      ptrelsuf="_ptRel"
    elif [ "$ptrel" == "2" ] 
    then
      ptrelsuf="_miniIso"
    elif [ "$ptrel" == "3" ] 
    then
      ptrelsuf="_newMiniIso"
    elif [ "$ptrel" == "4" ] 
    then
      ptrelsuf="_multiIso"
    fi
  
    #Get number of files
    numberOfFiles=$((`ls -l $path/$name/$tag | wc -l` - 1))
  
    #Submit all of them
    for (( i=0; i<$numberOfFiles; i++))
    do
      sname_lower=`echo ${sname,,}`
      number=$(( $i + 1 ))

      #Except they've finished
      if [ -e /hadoop/cms/store/user/$USER/condor/ss_13_babies/${sname_lower}_${number}$ptrelsuf.root ] 
      then 
        continue
      fi

      echo "-------------"
      echo "Working on $sname $number $ptrel"
  
      #Or if they're still running
      if [ -e logs/condorLog_${sname}_${number}_${ptrel}.log ] 
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
        done < logs/condorLog_${sname}_${number}_${ptrel}.log
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
