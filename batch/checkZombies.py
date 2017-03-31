from ROOT import *
import os
import glob

def main(fpatt):
    fnames = glob.glob(fpatt)

    for fname in fnames:
        f1 = TFile(fname)

        if f1.IsZombie(): print "%s is BAD!" % fname
        else: print "%s is good." % fname

        f1.Close()

if __name__ == "__main__":

    main("/hadoop/cms/store/user/namin/condor/ss_babies_July27_isr/t5qqqqvv_dm20_*.root")

