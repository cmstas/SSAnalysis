import ROOT as r
import os
import glob

####  MUST EDIT
met_folders = [
 "v8.04_July28_cut300",
 "v8.04_July28_cut400",
 "v8.04_July28_cut500",
 "v8.04_July28_cut600",
 "v8.04_July28_cut700",
 "v8.04_July28_cut800",
 "v8.04_July28_cut900",
"v8.04_July28_cut1000",
]
ht_folders = [
"v8.04_July28_cut1125",
"v8.04_July28_cut1200",
"v8.04_July28_cut1300",
"v8.04_July28_cut1400",
"v8.04_July28_cut1500",
"v8.04_July28_cut1600",
"v8.04_July28_cut1700",
"v8.04_July28_cut1800",
"v8.04_July28_cut1900",
"v8.04_July28_cut2000",
]
sig_name = "fs_t1tttt_m1300_m100" # will rename this to "sig"
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


        bin_to_use = 30+isHT
        # don't reduce an already reduced histogram, dummy!!
        if hist.GetNbinsX() == 1: bin_to_use =1

        val = hist.GetBinContent(bin_to_use)
        err = hist.GetBinError(bin_to_use)

        if bin_to_use != 1 and val < 0.00001:
            # maybe we're accidentally putting it in the extreme bin, so try to pick that up
            bin_to_use += 2
            val = hist.GetBinContent(bin_to_use)
            err = hist.GetBinError(bin_to_use)

        if "_extr_hth" in name or "xg" in fname:
            if hist.GetBinContent(bin_to_use) < 0.00001:
                val = 0.00001
                print fname, name, hist.GetBinContent(bin_to_use), hist.GetBinError(bin_to_use)

        h_temp = r.TH1F(name, name, 1, 0, 1)
        h_temp.SetBinContent(1, val)
        h_temp.SetBinError(1, err)
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
    os.system("python createCard.py %s sig" % folder)

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

