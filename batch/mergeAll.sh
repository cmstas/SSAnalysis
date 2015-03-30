#!/bin/bash

path="/hadoop/cms/store/group/snt/phys14"
path2="/hadoop/cms/store/user/cgeorge/condor/ss_13_babies"
tag="V07-02-08"

for sname in "TTW" "TTZ" "TTBAR" "WZ" "T1TTTT_1500" "T1TTTT_1200"
do

  for (( counter=0; counter<=500; counter++ )); do file[$counter]=""; done

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
    file[$(( $counter - 1 )) ]="$path2/${sname_lower}_${counter}.root"
    file[$(( $counter - 1 )) ]="$path2/${sname_lower}_ptRel_${counter}.root"
  done

  #hadd $sname.root ${file[*]}
  hadd ${sname}_ptRel.root ${file[*]}

 done 
