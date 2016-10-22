from ROOT import *
import os
import time
import glob
from tqdm import tqdm

def main(name, fpatt):
    fnames = glob.glob(fpatt)

    print ">>> Started working on %s with %i files" % (name, len(fnames))

    basename = "points_%s" % name

    tfout = TFile("%s.root" % basename, "RECREATE")
    
    # book histos
    hists = []
    bins = "100,0,2500"
    for ifname, fname in enumerate(fnames):
        hists.append( TH1F("h"+str(ifname),"h"+str(ifname),100,0,2500) )

    def fill_histo(ifname, fname):
        f1 = TFile.Open(fname)
        histname = "h"+str(ifname)
        tree = f1.Get("Events")
        tree.Draw("genps_mass>>%s(%s)" % (histname, bins), "genps_id==25 && genps_status==22")
        hists[ifname].Add( gDirectory.Get(histname) )
        f1.Close()

    t0 = time.time()
    for ifname, fname in tqdm(list(enumerate(fnames))):
        fill_histo(ifname, fname)
    t1 = time.time()
    print "elapsed time: %.3f" % (t1-t0)

    # get first histogram and add the rest to it
    hall = hists[0]
    for hist in hists[1:]:
        hall.Add(hist)
    tfout.Write()

    cnt = 0
    with open("%s.txt" % basename, "w") as fhout:
        for ix in range(hall.GetNbinsX()):
            n = int(hall.GetBinContent(ix))
            if n == 0: continue
            x = int(hall.GetXaxis().GetBinLowEdge(ix))
            fhout.write("    if (mH == %i) return %i;\n" % (x,n))

if __name__ == "__main__":

    name_patts = [
    # ("ttH", "/hadoop/cms/store/group/snt/run2_25ns_80Private/TTH_ttH-scan-MINIAOD_Private80Xv2/V08-00-05/merged_ntuple_*.root"),
    ("ttH", "/hadoop/cms/store/group/snt/run2_25ns_80Private/TTH_ttH-scan-MINIAOD_Private80Xv2_v2/V08-00-05/merged_ntuple_*.root"),
    ("tHW", "/hadoop/cms/store/group/snt/run2_25ns_80Private/THW_tHW-scan-MINIAOD_Private80Xv2/V08-00-05/merged_ntuple_*.root"),
    ("tHq", "/hadoop/cms/store/group/snt/run2_25ns_80Private/THQ_tHq-scan-MINIAOD_Private80Xv2/V08-00-05/merged_ntuple_*.root"),
    ]

    for name, fpatt in name_patts:
        main(name, fpatt)
