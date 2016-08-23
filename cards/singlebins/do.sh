sig="fs_t1tttt_m1500_m200"

orig_dir="v8.04_July28/"
# orig_dir="v8.04_t5qqqqdm20_July28/"

new_dir="v8.04_July28_singlebin/"
lumi="12.9"
kine="hihi"
sr=30

mkdir -p logs

# for sig in fs_t1tttt_m1400_m1000 fs_t1tttt_m1500_m200 fs_t5qqqqvv_m1000_m700 fs_t5qqqqvv_m1200_m400 fs_t5qqqqvv_dm20_m1400_m200 fs_t5qqqqvv_dm20_m1000_m700 ; do
# for sig in fs_t5qqqqvv_dm20_m1400_m200 fs_t5qqqqvv_dm20_m1000_m700 ; do
# for sig in fs_t1tttt_m1500_m200; do
for sig in fs_t1tttt_m1400_m1000; do

# for sig in fs_t1tttt_m1500_m200 fs_t1tttt_m1400_m1000; do

    # for kine in hihi hilow lowlow; do
    for kine in hihi; do

        srs=$(seq 1 33)
        [[ $kine == "hilow" ]] && srs=$(seq 1 27)
        [[ $kine == "lowlow" ]] && srs=$(seq 1 8)

        # for sr in $srs; do 
        for sr in 24; do 

            card=$new_dir/card_${sig}_${lumi}ifb-all.txt
            log=logs/log_${sig}_${lumi}_${kine}_${sr}.txt

            # copy over full histograms to new directory
            echo ". copy.sh $sig $orig_dir $new_dir $lumi"
            . copy.sh $sig $orig_dir $new_dir $lumi

            rename $sig sig $new_dir/${sig}*.root # FIXME

            # reduce hists to single bin
            echo "python binKiller.py $new_dir $kine $sr"
            python binKiller.py $new_dir $kine $sr

            # make card
            # echo "python ../createCard.py $new_dir $sig - $kine"
            # python ../createCard.py $new_dir $sig - $kine
            python ../createCard.py $new_dir sig - $kine # FIXME
            card=$new_dir/card_sig_${lumi}ifb-all.txt # FIXME

            # get limits
            echo "combine -M Asymptotic $card > $log"
            combine -M Asymptotic $card > $log

            # parse card to get pred, obs
            echo "python ../parseCard.py $card >> $log"
            python ../parseCard.py $card >> $log

            # # clean up
            # echo "rm -rf $new_dir"
            # rm -rf $new_dir

        done



    done
done


