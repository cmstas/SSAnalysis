#!/bin/bash

path1="/hadoop/cms/store/group/snt/phys14"
path2="/hadoop/cms/store/user/cgeorge/privateSusySignalsSS"
tag="V07-04-03"

pathCondor="/hadoop/cms/store/user/$USER/condor/ss_13_babies"

for i in "0" "1"
do
for sname in "TTBAR" "WZ" "DY_low" "DY_high" "WJets"
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
 #if   [ $sname == "TTW"         ]; then name="TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
 #elif [ $sname == "TTZ"         ]; then name="TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1";
  if   [ $sname == "TTBAR"       ]; then name="TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
  elif [ $sname == "WZ"          ]; then name="WZ_TuneCUETP8M1_13TeV-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2";
  elif [ $sname == "DY_low"      ]; then name="DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
  elif [ $sname == "DY_high"     ]; then name="DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2";
  elif [ $sname == "WJets"       ]; then name="WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1";
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
    file5[$(( $counter - 1 )) ]="$pathCondor/${sname_lower}_${counter}_$i.root"
  done

  hadd ${sname}_$i.root ${file5[*]}

 done 
done
