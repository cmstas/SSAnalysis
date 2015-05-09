#NLO Calculations

###Technical details
  - QCUT not set for NLO, as it doesn't matter.  We showed last fall that QCUT = c*XQCUT is pretty stable for 1 < c < 2. 
  - The numbers in "old" dir (662.62, 16.34) are what we showed at FNAL.  
  - If event is not dilepton, we only "keep" some of the variables.
  - PDF Uncertainty description is given here: https://www.hep.ucl.ac.uk/pdf4lhc/PDF4LHC_practical_guide.pdf
  - Efficiencies show frac of all events in the given SR
  - We put in protection against HT = nan, this seems to happen a lot, not sure why
  - A number of small bugs were discovered in the "old" way, nothing consequential.  We have a "doOld" mode that reproduces the old numbers using the old babies.  Running in "normal" mode with the old babies will produce slightly different numbers due to these bugs
