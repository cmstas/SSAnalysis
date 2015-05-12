#NLO Calculations

###Technical details
  - QCUT is 60 for ++, 65 for --
  - PDF Uncertainty description is given here: https://www.hep.ucl.ac.uk/pdf4lhc/PDF4LHC_practical_guide.pdf
  - Efficiencies show frac of all events in the given SR
  - We put in protection against HT = nan, this seems to happen a lot, not sure why
  - The yields with draw statements and fill statements are different by <1%.  I've tracked the discrepency to the weights, but they seem to be exactly equal, must be some sort of rounding error.  
  - The numbers in "old" dir are what we showed at FNAL.  I kept this directory due to the <1% effects
