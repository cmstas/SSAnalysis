#NLO Calculations

###Technical details
  - QCUT not set for NLO, as it doesn't matter.  We showed last fall that QCUT = c*XQCUT is pretty stable for 1 < c < 2. 
  - The numbers in "old" dir (662.62, 16.34) are what we showed at FNAL.  
  - If event is not dilepton, we only "keep" some of the variables.
  - PDF Uncertainty description is given here: https://www.hep.ucl.ac.uk/pdf4lhc/PDF4LHC_practical_guide.pdf
  - Efficiencies show frac of all events in the given SR
  - We put in protection against HT = nan, this seems to happen a lot, not sure why
  - A number of small bugs were discovered in the "old" way, nothing consequential.  We have a "doOld" mode that reproduces the old numbers using the old babies.  Running in "normal" mode with the old babies will produce slightly different numbers due to these bugs
  - weights[4] is with both up x4, weights[8] is with both down x 1/4.  The resulting weight is not always in the same direction, so it is possible that both errors have the same sign.  For proof, see below:
    ````
root [8] Events->Scan("weights[4]:weights[8]","weights[4]>0")
************************************
*    Row   * weights[4 * weights[8 *
************************************
*        0 * 0.5667099 * 2.5341000 *
*        1 * 0.5949599 * 2.3733000 *
*        2 * 0.8261899 * 1.6592999 *
*        3 * 0.7831400 * 1.6816999 *
*        4 * 0.8780400 * 1.3564000 *
*        5 * 0.6115999 * 2.4528999 *
*        6 * 0.5601000 * 2.5739998 *
*        9 * 1.0132000 * 0.8384500 *
*       11 * 0.8122799 * 1.6888999 *
*       12 * 0.5245100 *     2.773 *
````
