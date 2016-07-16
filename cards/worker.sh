where=$1
cut=$2
pfx=$3
postfix=$4
lumi=$5

cd $where

combine -M HybridNew --frequentist --testStat LHC -H ProfileLikelihood --seed -1 -T 500 -m ${cut} ${pfx}${cut}${postfix}/card_sig_${lumi}ifb-hihi.txt  -v 0 --rAbsAcc=0.05 --rRelAcc=0.015
for q in 0.025 0.16 0.5 0.84 0.975; do 
    combine -M HybridNew --frequentist --testStat LHC -H ProfileLikelihood --seed -1 -T 500 -m ${cut} ${pfx}${cut}${postfix}/card_sig_${lumi}ifb-hihi.txt  -v 0 --rAbsAcc=0.05 --rRelAcc=0.015 --expectedFromGrid=${q}
done
wait
