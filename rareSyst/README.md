#Rare Systematic Measurements

This is to make more accurate calculations of the systematic uncertainties on the rare backgrounds.

Last time we privately produced TTW and QQWW samples in Madgraph and decayed them in Pythia to produce AODSIM (this code is buried in Alex's repository).  In the next iteration, we will use the official MC samples.  

###BabyMaker
  - This is used to make a baby (actually an EDProducer) holding the systematics information for each event.  
  - Install with ". install.sh", compile with "scram b -j 20", and run with "cmsRun python/ConfFile_cfg.py"
  - Run two ways: with filter (only keep dilepton events) and without (so we can measure efficiencies).

###Analyzer
  - This is used to run on the babies and produce useful systematics
  - Still needs resurrected...
