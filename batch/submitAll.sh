#!/bin/bash

path="/hadoop/cms/store/group/snt/phys14"
tag="V07-02-08"
counter=0

for sname in "TTW" "TTZ" "TTBAR" "WZ" "T1TTTT_1500" "T1TTTT_1200"
do
  #Tracking
  let "counter=$counter+1"
#  if [ "${isDone[$counter]}" == "" ]; then isDone[$counter]="0"; elif [ "${isDone[$counter]}" == "1" ]; then continue; fi 

  #Get Name
  if [ $sname == "TTW"         ]; then name="TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "TTZ"         ]; then name="TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "TTBAR"       ]; then name="TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "WZ"          ]; then name="WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "T1TTTT_1500" ]; then name="SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1"; fi
  if [ $sname == "T1TTTT_1200" ]; then name="SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1"; fi

  #Get number of files
  numberOfFiles=$((`ls -l $path/$name/$tag | wc -l` - 1))

  #Submit all of them
  for (( i=0; i<$numberOfFiles; i++))
  do
    sname_lower=`echo ${sname,,}`
    number=$(( $i + 1 ))

    #Except they've finished
    if [ -e /hadoop/cms/store/user/cgeorge/condor/ss_13_babies/${sname_lower}_${number}.root ] 
    then 
      #./sweepRoot -o t -t t /hadoop/cms/store/user/cgeorge/condor/ss_13_babies/${sname_lower}_${number}.root > validFileOutput.txt
      #readarray -t results < validFileOutput.txt
      #if [ "$i" == "SUMMARY: 0 bad, 1 good" ]; then
#        isDone[$counter]="1"
      #elif [ "$i" == "SUMMARY: 1 bad, 0 good" ]; then
      #  rm /hadoop/cms/store/user/cgeorge/condor/ss_13_babies/${sname_lower}_${number}.root
      #fi
      continue
    fi

    #Or if it's still running
    if [ -e logs/condorLog_${sname}_${number}_0.log ] 
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
      done < logs/condorLog_${sname}_${number}_0.log
      jobid="$cluster.$process"
      condor_q $jobid > temp.txt
      result=`more temp.txt | awk 'END{print $1}'`
      echo "-------------"
      echo "Working on $sname $number"
      if [ "$result" == "$jobid" ] 
      then
        echo "already submitted!"
        continue
      fi
    fi
    echo "submitting..."

    #submit it
    cp condorFileTemplate condorFile
    sed -i s/ARG1/$sname/g condorFile
    sed -i s/ARG2/$number/g condorFile
    sed -i s/ARG3/0/g condorFile
    condor_submit condorFile
  done
done
