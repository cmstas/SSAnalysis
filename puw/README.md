# steps to calculate PUw and PUw variation

1. Get data distribution of pu weights (make sure the recommended minBiasXsec is being used, as well as the latest physics JSON and PU JSON)
```pileupCalc.py -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec 71300 --maxPileupBin 50 --numPileupBins 50 pileup_central.root```

2. Now vary the MB xsec up and down by 5% and recalculate (make sure to change the root file name at the end)

3. Get a distribution of cms3.puInfo_trueNumInteractions().at(0) from Z+jets MC (with trigger requirement and/or other stuff?). Right now, just using vince's MC True PU distribution

4. Run makePUw.C to make the histo file and also print out the bins for use in commonUtils.h
