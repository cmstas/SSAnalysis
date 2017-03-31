import ROOT as r
import os
import glob




fname = "wz_histos_hihi_4.0ifb.root"

def doReduceBins(fname, isHT):
    fin = r.TFile(fname)
    fout = r.TFile(fname+"_new", "RECREATE")

    # srin = fin.Get("sr")
    # print srin.GetBinContent(30+isHT)
    # print srin.GetBinContent(32+isHT)



    for name in [thing.GetName() for thing in list(fin.GetListOfKeys())]:
        hist = fin.Get(name)
        
        if "_hihi" in name: continue
        print name

        h_temp = r.TH1F(name, name, 4, 0, 4)
        # for i in range(1,hist.GetNbinsX()+1):
        for i in range(FIRST,hist.GetNbinsX()+1):
            # print i, hist.GetBinContent(i), hist.GetBinError(i)
            h_temp.SetBinContent(i-FIRST+1, hist.GetBinContent(i))
            h_temp.SetBinError(i-FIRST+1, hist.GetBinError(i))

        h_temp.Write()

    # print srin.GetBinContent(30+isHT)
    # print srin.GetBinContent(32+isHT)

    fout.Close()
    fin.Close()

def doMove(fname):
    os.system("mv {0}_new {0}".format(fname))

if __name__ == "__main__":

    fnames = glob.glob("v8.02_cut700/*hihi*.root")
    for fname in fnames:
        print fname
        doReduceBins(fname, False)
        doMove(fname)
, False
