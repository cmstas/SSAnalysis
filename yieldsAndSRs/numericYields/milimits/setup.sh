#!/bin/bash

# tag="v8.04_July28"

for cut in "300" "400" "500" "600" "700" "800" "900" "1000"; do
# for cut in "300" ; do
    echo $cut; 
    # mkdir -p ../../../cards/${tag}_cut${cut}/
    dir=MET_${cut}
    mkdir -p $dir
    cp yieldMaker.C $dir/
    cp do.C $dir/
    sed -i 's/DOMETCUTHERE/true/' $dir/yieldMaker.C
    sed -i 's/DOHTCUTHERE/false/' $dir/yieldMaker.C
    sed -i 's/THECUTHERE/'"$cut"'/' $dir/yieldMaker.C
    cd $dir
    root -b -q -l do.C >& log.txt &
    cd -
done

for cut in "1125" "1200" "1300" "1400" "1500" "1600" "1700" "1800" "1900" "2000"; do
    echo $cut; 
    # mkdir -p ../../../cards/${tag}_cut${cut}/
    dir=HT_${cut}
    mkdir -p $dir
    cp yieldMaker.C $dir/
    cp do.C $dir/
    sed -i 's/DOMETCUTHERE/false/' $dir/yieldMaker.C
    sed -i 's/DOHTCUTHERE/true/' $dir/yieldMaker.C
    sed -i 's/THECUTHERE/'"$cut"'/' $dir/yieldMaker.C
    cd $dir
    root -b -q -l do.C >& log.txt &
    cd -
done
