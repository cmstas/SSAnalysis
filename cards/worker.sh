where=$1
cut=$2
pfx=$3
postfix=$4
lumi=$5

cd $where

# # orig
# Ntoys=500
# AbsAcc=0.05
# RelAcc=0.015

# 3-4x longer, but more consistent values
Ntoys=1500
AbsAcc=0.025
RelAcc=0.01

combine -M HybridNew --frequentist --testStat LHC -H ProfileLikelihood --seed -1 -T $Ntoys -m ${cut} ${pfx}${cut}${postfix}/card_sig_${lumi}ifb-hihi.txt  -v 0 --rAbsAcc=$AbsAcc --rRelAcc=$RelAcc
for q in 0.025 0.16 0.5 0.84 0.975; do 
    combine -M HybridNew --frequentist --testStat LHC -H ProfileLikelihood --seed -1 -T $Ntoys -m ${cut} ${pfx}${cut}${postfix}/card_sig_${lumi}ifb-hihi.txt  -v 0 --rAbsAcc=$AbsAcc --rRelAcc=$RelAcc --expectedFromGrid=${q}
done
wait
