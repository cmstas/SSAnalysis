#!/bin/bash

path="/hadoop/cms/store/group/snt/phys14"
tag="V07-02-08"

nSubmitted=0

for ptrel in "0" "1"
do
  for sname in "TTW" "TTZ" "TTBAR" "WZ" "T1TTTT_1500" "T1TTTT_1200"
  do
    #Get Name
    if [ $sname == "TTW"         ]; then name="TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
    if [ $sname == "TTZ"         ]; then name="TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
    if [ $sname == "TTBAR"       ]; then name="TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
    if [ $sname == "WZ"          ]; then name="WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
    if [ $sname == "T1TTTT_1500" ]; then name="SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1"; fi
    if [ $sname == "T1TTTT_1200" ]; then name="SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1"; fi

    #Get pTRel suffix
    if [ "$ptrel" == 0 ]; then ptrelsuf=""; else ptrelsuf="_ptRel"; fi
  
    #Get number of files
    numberOfFiles=$((`ls -l $path/$name/$tag | wc -l` - 1))
  
    #Submit all of them
    for (( i=0; i<$numberOfFiles; i++))
    do
      sname_lower=`echo ${sname,,}`
      number=$(( $i + 1 ))
  
      #Except they've finished
      if [ -e /hadoop/cms/store/user/cgeorge/condor/ss_13_babies/${sname_lower}${ptrelsuf}_${number}.root ] 
      then 
        continue
      fi
  
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
        echo "-------------"
        echo "Working on $sname $number $ptrel"
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
