# pfx="v8.07_cut"
# postfix="/" # if postfix is blank, then arguments to worker get screwed up
# lumi="36.5"

pfx="v9.06_cut"
postfix="/" # if postfix is blank, then arguments to worker get screwed up
lumi="35.9"

# for cut in 300 400 500 600 700 800 900 1000 1125 1200 1300 1400 1500 1600 1700 1800 1900 2000; do
for cut in 300 400 500 600 700 800 900 1000 ; do
# for cut in 300 400 ; do
# for cut in 1125 1200 1300 1400 1500 1600 1700 1800 1900 2000; do
# for cut in 2000; do

    where="worker_cut${cut}"
    mkdir -p ${where}/${pfx}${cut}${postfix}
    cp ${pfx}${cut}${postfix}/* ${where}/${pfx}${cut}${postfix}/
    echo ". worker.sh $where $cut $pfx $postfix $lumi >& ${where}/log.txt"
    . worker.sh $where $cut $pfx $postfix $lumi >& ${where}/log.txt &

done
