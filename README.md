SSAnalysis
==========

##how to install
. install.sh

##how to run
edit main.cc

make

./main.exe

##how to make cards and get limits (see createCard.py for more options)

python createCard.py dir signalSample plot dir/card.txt

combine -M Asymptotic dir/card.txt --run expected --noFitAsimov
