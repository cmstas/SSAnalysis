SSAnalysis
==========

###how to install
. install.sh

###how to set your environment
Every time, do
. setup.sh

###how to make babies
This PHYS14 branch is deprecated.  You may not be able to run interactively (out of date, and I didn't put in the JECs).  It's easiest to run this in the batch directory (root do.C\(TTBAR, 17, 4, 0\) for example); batch still works.  Yell at Alex if you actually need to run this interactively for some reason.

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
