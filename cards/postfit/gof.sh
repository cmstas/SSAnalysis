#!/usr/bin/env bash
# card=v9.04_Jan31_36p8_t1tttt/card_fs_t1tttt_m1300_m400_36.8ifb-all.txt
card=v9.06_Mar6_35p9_reminiaodfilt_nomhiggs/card_fs_t1tttt_m1500_m400_35.9ifb-all.txt

njobs=25
ntoys=5

# run toys
for seed in $(seq 1 ${njobs}); do
    echo "nice -10 combine -M GoodnessOfFit ${card} --algo=saturated -t ${ntoys} -s ${seed} >& log_gof_${seed}.txt &"
    nice -10 combine -M GoodnessOfFit ${card} --algo=saturated -t ${ntoys} -s ${seed} >& log_gof_${seed}.txt &
done

# get observed
combine -M GoodnessOfFit ${card} --algo=saturated >& log_gof_observed.txt &

# when done, do
# hadd -f all_toys.root higgsCombineTest.GoodnessOfFit.mH120.*.root
# ./plot_gof.py
