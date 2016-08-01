## Note that it is implied that each script should be edited with the appropriate parameters before running them

## Various scripts
* `comparison.C` was used to compare 2015 and 2016 cards (but can be repurposed)
* `dumpAllShapes.sh` was used to dump all the shapes for each background sample
* `makeTable.C` makes the yield table that goes into AN and PAS
* `printShapes.C` and `printTotalSyst.C` produce the red-blue band plots and gray band plots, respectively

## Limits
* run the yieldMaker with an #include directive for one of the `fs_t1tttt.h`-like files. These are created via the `printFSscans.py` script (~2 hours)
* all the root files should end up here
* run `addMETSyst.py` to add the MET systematics for the fastsim root files
* run `limitsTable_XXX.py` for the appropriate model (with redocards and redolimits set to True) to make the cards in parallel (takes ~20 mins)
* next time, you do not need to set redocards and redolimits to True since we can just reuse them

## SSTT limits
* `python createCard.py v8.03 ttbar_os br sr card-ttos.txt` with the appropriate directory instead of `v8.03`
* `combine -M Asymptotic v8.03_ttos/card-ttos.txt  --noFitAsimov`
* the limit value here is signal strength, so must multiply it by the xsec of ttbar (831.762pb) and divide by 1000 because of https://github.com/cmstas/SSAnalysis/blob/42e6e83e932d670d36b421e498862113aed513c1/yieldsAndSRs/numericYields/yieldMaker.C#L1596 (note, obs limit was 1.5 pb in 2015 paper with 2.3/fb of data)

## TTTT limits
* run the yieldMaker after commenting out the TTTT *rares* chain (keep the tttt_chain, since we will be using this as a signal) (10 mins)
* `python createCard.py v8.03_tttt tttt`
* `combine -M Asymptotic v8.03_tttt/card_tttt_6.3ifb-all.txt --noFitAsimov`
* same story as for SSTT (signal strength is output, so need to multiply by xsec of TTTT (0.009103 pb) to get limit
* note: was 119 fb (obs) in 2015 with 2.3/fb of data

## MI limits
* see the `milimits` area inside the yieldMaker area. (takes 20 mins)
* after running that, there should be a series of folders inside this directory.
* edit `createCard.py` to use the correct luminosity
* run `python prepareForMI_new.py` to prepare the cards and histograms to be one bin. Note that this assumes the new binning where we have two high ht and met regions (takes 5 mins, and ignore 'Bin SS has no processes contributing to it')
* run `. poor_mans_batch.sh` with the proper parameters to run combine in parallel (takes 1.5 hrs)
* copy the root files generated in all the worker folders into a single folder called <whatever>
* run `python makeMIlimitsnick.py` after editing dir to be <whatever>
* If all goes well (5% chance), you should end up with two beautiful plots

## validation of signal points
* inside `validation`, run `copy.sh` after editing in a few appropriate signal mass points to dump
