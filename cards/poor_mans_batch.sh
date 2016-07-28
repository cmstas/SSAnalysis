pfx="v8.04_cut"
postfix="_fix"
lumi="12.9"

# for cut in 300 400 500 600 700 800 900 1000 1125 1200 1300 1400 1500 1600 1700 1800 1900 2000; do
for cut in 1125 1200 1300 1400 1500 1600 1700 1800 1900 2000; do

    where="worker_cut${cut}"
    mkdir -p ${where}/${pfx}${cut}${postfix}
    cp ${pfx}${cut}${postfix}/* ${where}/${pfx}${cut}${postfix}/
    . worker.sh $where $cut $pfx $postfix $lumi >& ${where}/log.txt &

done
