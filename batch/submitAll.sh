#!/bin/bash

path="/hadoop/cms/store/group/snt/phys14"
tag="V07-02-08"

for sname in "TTW" "TTZ" "TTBAR"
do
  #Get Name
  if [ $sname == "TTW"   ]; then name="TTWJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "TTZ"   ]; then name="TTZJets_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi
  if [ $sname == "TTBAR" ]; then name="TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1"; fi

  #Get number of files
  numberOfFiles=$((`ls -l $path/$name/$tag | wc -l` - 1))

  #Submit all of them
  for (( i=0; i<$numberOfFiles; i++))
  do
    #Except they've finished
    sname_lower=`echo ${sname,,}`
    number=$(( $i + 1 ))
    if [ -e /hadoop/cms/store/user/cgeorge/condor/ss_13_babies/${sname_lower}_$number.root ]; then continue; fi

    #submit it
    cp condorFileTemplate condorFile
    sed -i s/ARG1/$sname/g condorFile
    sed -i s/ARG2/$number/g condorFile
    sed -i s/ARG3/0/g condorFile
    condor_submit condorFile
  done
done
