SSAnalysis
==========

###how to install
. install.sh

###how to run
This will make babies, plots, etc.  
edit main.cc
make
./main.exe

###how to make cards 

Create a directory, and fill it with these files:
  - WZJets_histos.root
  - TTWJets_histos.root
  - TTZJets_histos.root
  - ttbar_histos.root
  - t1tttt_1500_100_histos.root

Each of these files should have 3 histograms inside, with the names (You are allowed to append a suffix to these names, in that case, this will be the suffix below):
  - hyp_hihi 
  - hyp_hilow
  - hyp_lowlow

Finally, create the cards with:
python createCard.py dir signalSample suffix

###how to get limits 
combine -M Asymptotic dir/card.txt --run expected --noFitAsimov
