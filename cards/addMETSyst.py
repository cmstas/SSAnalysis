import ROOT as r
import os
import glob
from tqdm import tqdm

####  MUST EDIT
# folder = "v8.04_Sept21"
# sig_name = "fs_t1ttbb_m"
# folder = "v9.06_Mar6_35p9_reminiaodfilt_t5qqqqt6ttww_met"
# folder = "v9.06_Mar6_35p9_reminiaodfilt_t5tttt_met"
# folder = "v9.06_Mar6_35p9_reminiaodfilt_t1ttbb_met"
folder = "v9.06_Mar6_35p9_reminiaodfilt_t5ttcc_met"
# sig_name = "fs_t1tttt_m"
# sig_name = "fs_t5qqqqvv_dm20_m"
# sig_name = "fs_t5qqqqvv_m"
# sig_name = "fs_t1ttbb_m"
sig_name = "fs_t5ttcc_m"
#### END MUST EDIT

def doAddSyst(fname):
    fin = r.TFile(fname)
    histnames = [thing.GetName() for thing in list(fin.GetListOfKeys())]

    # already have modified this file
    if "metUncUp" in histnames: 
        print "already added metUncUp/Down to %s...skipping" % fname
        return

    fout = r.TFile(fname+"_new", "RECREATE")

    for name in histnames:
        hist = fin.Get(name)
        if name == "sr": continue
        hist.Write()

    pfmet = fin.Get("sr")
    genmet = fin.Get("genMet")

    nb = pfmet.GetNbinsX()
    xlow = pfmet.GetXaxis().GetXmin()
    xhigh = pfmet.GetXaxis().GetXmax()

    sr_out = r.TH1F("sr", "sr", nb,xlow,xhigh)
    met_up = r.TH1F("metUncUp", "metUncUp", nb,xlow,xhigh)
    met_dn = r.TH1F("metUncDown", "metUncDown", nb,xlow,xhigh)

    for ibin in range(0,pfmet.GetNbinsX()+1):
        pf = pfmet.GetBinContent(ibin)
        gen = genmet.GetBinContent(ibin)

        cent = 0.5*(pf+gen)
        lower = min(pf,gen)
        upper = max(pf,gen)

        sr_out.SetBinContent(ibin, cent)
        met_up.SetBinContent(ibin, upper)
        met_dn.SetBinContent(ibin, lower)

    sr_out.Write()
    met_up.Write()
    met_dn.Write()

    fout.Close()
    fin.Close()

    # print "modified %s" % fname
    os.system("mv {0}_new {0}".format(fname))


if __name__ == "__main__":

    fnames = glob.glob("%s/%s*.root" % (folder, sig_name))
    # print fnames
    # fnames = [fn for fn in fnames if "hihi" in fn]
    for fname in tqdm(fnames):
            if "agg" in fname: continue
            # print fname
        # try:
            doAddSyst(fname)
        # except:
        #     print "[!] Failed to add syst to %s" % fname
