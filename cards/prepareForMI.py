import ROOT as r
import os
import glob

## MUST EDIT
met_folders = [
"v8.02_cut300",
"v8.02_cut400",
"v8.02_cut500",
"v8.02_cut600",
"v8.02_cut700",
"v8.02_cut800",
"v8.02_cut900",
"v8.02_cut1000",
]

ht_folders = [
"v8.02_cut1125",
"v8.02_cut1200",
"v8.02_cut1300",
"v8.02_cut1400",
"v8.02_cut1500",
"v8.02_cut1600",
"v8.02_cut1700",
"v8.02_cut1800",
"v8.02_cut1900",
"v8.02_cut2000",
]
sig_name = "fs_t1tttt_m1300_m100" # will rename this to "sig"
## END MUST EDIT



OFFSET = 30-1


def setBin(fname, isHT):
    fin = r.TFile(fname)
    fout = r.TFile(fname+"_new", "RECREATE")

    srin = fin.Get("sr")
    print srin.GetBinContent(30+isHT)
    print srin.GetBinContent(32+isHT)



    for name in [thing.GetName() for thing in list(fin.GetListOfKeys())]:
        fin.Get(name).Write()

    srin = fout.Get("sr")
    srin.SetBinContent(30+isHT-OFFSET, 1.0)
    srin.SetBinContent(32+isHT-OFFSET, 0.0)
    srin.Write()

    print srin.GetBinContent(30+isHT-OFFSET)
    print srin.GetBinContent(32+isHT-OFFSET)

    fout.Close()
    fin.Close()

def doReduceBins(fname):
    FIRST = 30

    fin = r.TFile(fname)
    fout = r.TFile(fname+"_new", "RECREATE")

    for name in [thing.GetName() for thing in list(fin.GetListOfKeys())]:
        hist = fin.Get(name)
        
        if "_hihi" in name: continue
        # print name

        h_temp = r.TH1F(name, name, 4, 0, 4)
        for i in range(FIRST,hist.GetNbinsX()+1):
            h_temp.SetBinContent(i-FIRST+1, hist.GetBinContent(i))
            h_temp.SetBinError(i-FIRST+1, hist.GetBinError(i))

        h_temp.Write()

    fout.Close()
    fin.Close()

def doCopy(fname):
    os.system("cp %s %s" % (fname, fname.replace(sig_name, "sig")))
    for other in ["hilow", "lowlow"]:
        os.system("cp %s %s" % (fname.replace("hihi",other), fname.replace("hihi",other).replace(sig_name, "sig")))

def doMove(fname):
    os.system("mv {0}_new {0}".format(fname))

def makeCard(folder):
    os.system("python createCard.py %s sig" % folder)

if __name__ == "__main__":

    for folder in met_folders+ht_folders:
        print folder
        fnames = glob.glob("%s/*hihi*.root" % folder)
        for fname in fnames:
            # print fname
            doReduceBins(fname)
            doMove(fname)

    for folder in met_folders:
        fnames = glob.glob("%s/%s*hihi*.root" % (folder, sig_name))
        if len(fnames) < 1: 
            print "Warning: DID NOT FIND FNAME FOR %s" % folder
            continue
        fname = fnames[0]
        print fname
        setBin(fname, isHT=False)
        doMove(fname)
        doCopy(fname)
        makeCard(folder)

    for folder in ht_folders:
        fnames = glob.glob("%s/%s*hihi*.root" % (folder, sig_name))
        if len(fnames) < 1: 
            print "Warning: DID NOT FIND FNAME FOR %s" % folder
            continue
        fname = fnames[0]
        print fname
        setBin(fname, isHT=True)
        doMove(fname)
        doCopy(fname)
        makeCard(folder)
