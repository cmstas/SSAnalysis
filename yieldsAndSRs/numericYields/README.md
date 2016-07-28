### Instructions for yieldMaker

1. Make samples in `SSAnalysis/batch`, and make FR babies with LeptonBabyMaker
 * remember that the scripts in `points/` will get the mass point counts for the sparm scans

2. Update PUw, lepton SFs, trigger SFs, btagSFs, JECs, etc. in the appropriate `*.h` files

3. Get WZ SF in `SSAnalysis/WZ` and put this in `commonUtils.h`

4. Get fake rate maps in `SSAnalysis/FakeRate/measurement_region`. Update `commonUtils.h`

5. Get flip rate maps in `SSAnalysis/FlipRate`. Update `commonUtils.h`

6. Normalize the ISR weights with the script in `SSAnalysis/batch/points` and copy over the `*.h` files to this folder

7. Using `printFSscans.py` here to make the include files for the sparm scans (make sure these are included in the `yieldMaker.C` file)

8. Run the yieldMaker

9. Check cards

10. Follow instructions in the `SSAnalysis/cards` directory

11. Done

