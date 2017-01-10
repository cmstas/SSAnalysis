import ROOT as r
import os
import glob
import re
import sys

def doReduceBins(fname, kine, SR):
    SR = int(SR)
    print "Reducing %s to only have %sSR%i" % (fname, kine.replace("i","").replace("o","").replace("w","").upper(), SR)
    # Reduce 33 bins to 1 bin

    fin = r.TFile(fname)
    fout = r.TFile(fname+"_new", "RECREATE")


    for name in [thing.GetName() for thing in list(fin.GetListOfKeys())]:
        hist = fin.Get(name)

        # identify the hists like "ww_stat_hihi30Up", "ww_stat_hihi30Down" and skip them if the SR in the name doesn't match the one specified here
        match = re.match(".*_(agg|hihi|hilow|lowlow)([0-9]{1,2})(Up|Down)$", name)
        if match and len(match.groups()) == 3:
            thekine = match.group(1)
            thesr = match.group(2)
            if int(thesr) != int(SR): continue
            if str(thekine) != str(kine): continue


        # don't reduce an already reduced histogram, dummy!!
        if hist.GetNbinsX() == 1: SR = 1

        val = hist.GetBinContent(SR)
        err = hist.GetBinError(SR)

        # print name, val
        if "jesDown" in name and val == 0:
            val = 0.0001
        if "extr" in name and val == 0:
            val = 0.0001

        h_temp = r.TH1F(name, name, 1, 0, 1)
        h_temp.SetBinContent(1, val)
        h_temp.SetBinError(1, err)
        h_temp.Write()

    # FIXME write new SR hist with nsig=1
    if "sig_" in fname:
        srin = fout.Get("sr")
        print "NSIGNAL:"+str(srin.GetBinContent(1))
        srin.SetBinContent(1, 1.0)
        srin.Write()

    fout.Close()
    fin.Close()


def doMove(fname):
    # move new hists over old ones
    os.system("mv {0}_new {0}".format(fname))

if __name__ == "__main__":

    # doReduceBins("v8.04_July28/ww_histos_hihi_12.9ifb.root", "hihi", 30)
    if len(sys.argv) != 4: sys.exit()

    _, folder, kine, sr = sys.argv

    # folder = "v8.04_July28_singlebin"
    # kine = "lowlow"
    # sr = 3

    fnames = glob.glob("%s/*%s*.root" % (folder,kine))
    for fname in fnames:
        doReduceBins(fname, kine, sr)
        doMove(fname)

    # for  folder, isHT in [(x,False) for x in met_folders]+[(x,True) for x in ht_folders]:
    #     print folder, isHT

    #     fnames = glob.glob("%s/*hihi*.root" % folder)
    #     for fname in fnames:
    #         doReduceBins(fname, isHT)
    #         doMove(fname)

    #     fnames = glob.glob("%s/%s*hihi*.root" % (folder, sig_name))
    #     if len(fnames) < 1: 
    #         print "Warning: DID NOT FIND FNAME FOR %s" % folder
    #         continue
    #     fname = fnames[0]
    #     setBin(fname)
    #     doMove(fname)
    #     doCopy(fname)
    #     makeCard(folder)

