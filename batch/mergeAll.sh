#!/bin/bash

path1="/hadoop/cms/store/group/snt/phys14"
path2="/hadoop/cms/store/user/cgeorge/privateSusySignalsSS"
tag="V07-02-08"

pathCondor="/hadoop/cms/store/user/$USER/condor/ss_13_babies"

for sname in "TTW" "TTZ" "TTBAR" "WZ" "T1TTTT_1500" "T1TTTT_1200" "T5qqqqWW_1200_1000_800" "T5qqqqWW_deg_1000_315_300" "DY" "WJets"
do

  for (( counter=0; counter<=500; counter++ )); do file1[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file2[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file3[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file4[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file5[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file6[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file7[$counter]=""; done

  #Get Name, path
  path=$path1 
  if   [ $sname == "TTW"         ]; then name="TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  elif [ $sname == "TTZ"         ]; then name="TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  elif [ $sname == "TTBAR"       ]; then name="TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  elif [ $sname == "WZ"          ]; then name="WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  elif [ $sname == "T1TTTT_1500" ]; then name="SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
  elif [ $sname == "T1TTTT_1200" ]; then name="SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1";
  elif [ $sname == "DY"          ]; then name="DYJetsToLL_M-50_13TeV-madgraph-pythia8_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  elif [ $sname == "WJets"       ]; then name="WJetsToLNu_13TeV-madgraph-pythia8-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  else name=$sname; path=$path2
  fi
  
  sname_lower=`echo ${sname,,}`

  #Get number of files
  numberOfFiles=$((`ls -l $path/$name/$tag | wc -l` - 1))
  echo $sname $numberOfFiles

  #If only one, done
  if [ "$numberOfFiles" == "1" ]; then cp "$pathCondor/${sname_lower}_1" "${sname_lower}" ; continue; fi 
  
  #Write array with file names
  for (( counter=1; counter<=$numberOfFiles; counter++ ))
  do
    file5[$(( $counter - 1 )) ]="$pathCondor/${sname_lower}_${counter}.root"
  done

  hadd ${sname}.root ${file5[*]}

 done 
