#!/bin/bash

path="/hadoop/cms/store/group/snt/phys14"
path2="/hadoop/cms/store/user/$USER/condor/ss_13_babies"
tag="V07-02-08"

for sname in "TTW" "TTZ" "TTBAR" "WZ" "T1TTTT_1500" "T1TTTT_1200"
do

  for (( counter=0; counter<=500; counter++ )); do file1[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file2[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file3[$counter]=""; done
  for (( counter=0; counter<=500; counter++ )); do file4[$counter]=""; done

  #Get Name
  if [ $sname == "TTW"         ]; then name="TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "TTZ"         ]; then name="TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "TTBAR"       ]; then name="TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "WZ"          ]; then name="WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "T1TTTT_1500" ]; then name="SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1"; fi
  if [ $sname == "T1TTTT_1200" ]; then name="SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1"; fi
  
  sname_lower=`echo ${sname,,}`

  #Get number of files
  numberOfFiles=$((`ls -l $path/$name/$tag | wc -l` - 1))
  echo $sname $numberOfFiles
  
  #Write array with file names
  for (( counter=1; counter<=$numberOfFiles; counter++ ))
  do
    file1[$(( $counter - 1 )) ]="$path2/${sname_lower}_${counter}.root"
    file2[$(( $counter - 1 )) ]="$path2/${sname_lower}_${counter}_ptRel.root"
    file3[$(( $counter - 1 )) ]="$path2/${sname_lower}_${counter}_miniIso.root"
    file4[$(( $counter - 1 )) ]="$path2/${sname_lower}_${counter}_newMiniIso.root"
  done

  hadd $sname.root ${file1[*]}
  hadd ${sname}_ptRel.root ${file2[*]}
  hadd ${sname}_miniIso.root ${file3[*]}
  hadd ${sname}_newMiniIso.root ${file4[*]}

 done 
