#NLO Calculations

###How to run this?
   - You need the genweights from CMS3.  You can run directly on the CMS3 or on a baby
   - You need to look on DAS to find the number of events IN THE LHE FILES for the samples you are analyzing.  This will convert the multiplicative weights to an additive weight, so that you can calculate your yields in each SR.  
   - Then just run it.  It will output tables with the results both to the screen and to the tables subdirectory.  

###Signs seem messed up?
  - weights[4] is with both up x2, weights[8] is with both down x 1/2.  The resulting weight is not always in the same direction, so it is possible that both errors have the same sign.  For proof, see below:

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
