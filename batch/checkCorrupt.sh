#!/bin/bash

dir="/hadoop/cms/store/user/namin/condor/ss_babies_Sept22/"

patterns="R__unzip
Error in <TBasket::ReadBasketBuffers>
Error in <TBranchElement::GetBasket>
No servers are available to read the file" # may need to remove this if xrootd is not used
corrupts=$(grep -F "${patterns}" /data/tmp/namin/logs/condorLog_10[0-9][0-9]_* | cut -d: -f1 | sort | uniq)

for corrupt in $corrupts; do
    echo $corrupt
    grep "hadoop" $corrupt | grep "nerrors=1," | awk '{print $5}'
    isample=$(echo $corrupt | cut -d '_' -f2);
    imerged=$(echo $corrupt | cut -d '_' -f3 | cut -d '.' -f1);

    sample=$(grep "\b$isample\b" base.h | awk '{print $1}')
    shortname=$(grep "\b$sample\b" main.cc.norun -A 5 | grep shortname | cut -d'"' -f2)

    # ls -l ${dir}/${shortname}_${imerged}.root
    file=${dir}/${shortname}_${imerged}.root
    [[ -f $file ]] || continue;

    echo $file
    echo "not deleting just yet"
    # rm $file


done
