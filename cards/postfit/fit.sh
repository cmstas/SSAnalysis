#!/usr/bin/env bash

# lumi=36.8
# dir=v9.04_Jan31_36p8_t1tttt
lumi=35.9
dir=v9.06_Mar6_35p9_reminiaodfilt_nomhiggs
sig=fs_t1tttt_m1500_m400

echo ">>> Making card"
python ../createCard.py $dir $sig

echo ">>> Doing fit"
combine -M MaxLikelihoodFit ${dir}/card_${sig}_${lumi}ifb-all.txt --saveShapes --saveWithUncertainties -n name

echo ">>> Making pre/postfit pull plots"
root -b -q -l -n do.C

echo ">>> Running diffNuisances.py"
python diffNuisances.py -a -g nuisance_output.root mlfitname.root > nuisance_output.txt

echo ">>> And again for latex output"
python diffNuisances.py -a -g nuisance_output.root -f latex mlfitname.root > nuisance_output.tex

echo ">>> Done running diffNuisances.py. Output plots in nuisance_output.root and names in nuisance_output.txt"
