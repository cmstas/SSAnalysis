SSAnalysis
==========

git clone git@github.com:cmstas/SSAnalysis.git

cd SSAnalysis/

git clone git@github.com:cmstas/CORE.git

git clone git@github.com:cmstas/Tools.git

cmsrel CMSSW_7_1_6

cd CMSSW_7_1_6/src

cmsenv

git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit

cd HiggsAnalysis/CombinedLimit

git fetch origin

git checkout v5.0.1

scramv1 b clean

scramv1 b -j 4

cd ../../../../

#how to run
edit main.cc

make

./main.exe

#how to make cards and get limits (see createCard.py for more options)

python createCard.py dir signalSample plot dir/card.txt

combine -M Asymptotic dir/card.txt --run expected --noFitAsimov
