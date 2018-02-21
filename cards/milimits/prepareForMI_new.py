import ROOT as r
import os
import glob

####  MUST EDIT
met_folders = [
 "v9.06_cut300",
 "v9.06_cut400",
 "v9.06_cut500",
 "v9.06_cut600",
 "v9.06_cut700",
 "v9.06_cut800",
 "v9.06_cut900",
 "v9.06_cut1000",
]
ht_folders = [
"v9.06_cut1125",
"v9.06_cut1200",
"v9.06_cut1300",
"v9.06_cut1400",
"v9.06_cut1500",
"v9.06_cut1600",
"v9.06_cut1700",
"v9.06_cut1800",
"v9.06_cut1900",
"v9.06_cut2000",
]
sig_name = "fs_t1tttt_m1150_m500" # will rename this to "sig"
#### END MUST EDIT

def setBin(fname):
    # Set the single reduced bin to 1 for signal root file
    fin = r.TFile(fname)
    fout = r.TFile(fname+"_new", "RECREATE")

    srin = fin.Get("sr")
    print "before", srin.GetBinContent(1)

    for name in [thing.GetName() for thing in list(fin.GetListOfKeys())]:
        fin.Get(name).Write()

    srin = fout.Get("sr")
    srin.SetBinContent(1, 1.0)
    srin.Write()

    print "after", srin.GetBinContent(1)

    fout.Close()
    fin.Close()

def doReduceBins(fname, isHT):
    # Reduce 33 bins to 1 bin

    fin = r.TFile(fname)
    fout = r.TFile(fname+"_new", "RECREATE")

    for name in [thing.GetName() for thing in list(fin.GetListOfKeys())]:
        hist = fin.Get(name)
        if "_hihi" in name: continue

        tot_val = 0.0
        tot_err = 0.0
        bins_to_use = range(42,45+1) # MET
        if isHT:
            bins_to_use = range(46,51+1) # HT
        # don't reduce an already reduced histogram, dummy!!
        if hist.GetNbinsX() == 1: bins_to_use = [1]

        for bin_to_use in bins_to_use:
            val = hist.GetBinContent(bin_to_use)
            err = hist.GetBinError(bin_to_use)

            if "_extr_hth" in name or "xg" in fname:
                if hist.GetBinContent(bin_to_use) < 0.00001:
                    val = 0.00001
                    print fname, name, hist.GetBinContent(bin_to_use), hist.GetBinError(bin_to_use)

            tot_val += val
            tot_err += err

        h_temp = r.TH1F(name, name, 1, 0, 1)
        h_temp.SetBinContent(1, tot_val)
        h_temp.SetBinError(1, tot_err)
        h_temp.Write()

    fout.Close()
    fin.Close()

def doCopy(fname):
    # Copy the signal sample name to sig and do the same for hilow and lowlow as for hihi
    os.system("cp %s %s" % (fname, fname.replace(sig_name, "sig")))
    for other in ["hilow", "lowlow"]:
        os.system("cp %s %s" % (fname.replace("hihi",other), fname.replace("hihi",other).replace(sig_name, "sig")))

def doMove(fname):
    # move new hists over old ones
    os.system("mv {0}_new {0}".format(fname))

def makeCard(folder):
    # call createCard.py with 'sig' so that it knows to do the correct thing for MI limits
    # this will throw an error when trying to combine the cards, but that's ok since we only
    # care about the hihi card.
    os.system("export NOCOMBINE=true; python createCard.py %s sig" % folder)

if __name__ == "__main__":

    for  folder, isHT in [(x,False) for x in met_folders]+[(x,True) for x in ht_folders]:
        print folder, isHT

        fnames = glob.glob("%s/*hihi*.root" % folder)
        for fname in fnames:
            doReduceBins(fname, isHT)
            doMove(fname)

        fnames = glob.glob("%s/%s*hihi*.root" % (folder, sig_name))
        if len(fnames) < 1: 
            print "Warning: DID NOT FIND FNAME FOR %s" % folder
            continue
        fname = fnames[0]
        setBin(fname)
        doMove(fname)
        doCopy(fname)
        makeCard(folder)

