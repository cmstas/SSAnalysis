from ROOT import *
import os
import time
import glob
from tqdm import tqdm

def main(name, fname):
    print ">>> Started working on %s" % (name)

    bins = "100,0,2500,100,0,2500"
    hcnt = TH2F("hcnt","hcnt",100,0,2500,100,0,2500)
    hup = TH2F("hup","hup",100,0,2500,100,0,2500)
    hdn = TH2F("hdn","hdn",100,0,2500,100,0,2500)
    hcent = TH2F("hcent","hcent",100,0,2500,100,0,2500)

    f1 = TFile.Open(fname)
    tree = f1.Get("t")
    tree.Draw("sparms[0]:sparms[1]>>hcnt(%s)" % (bins), "1")
    print ">>> Scanned count"
    tree.Draw("sparms[0]:sparms[1]>>hup(%s)" % (bins), "weight_isr_UP")
    print ">>> Scanned isr up"
    tree.Draw("sparms[0]:sparms[1]>>hcent(%s)" % (bins), "weight_isr")
    print ">>> Scanned isr central"
    tree.Draw("sparms[0]:sparms[1]>>hdn(%s)" % (bins), "weight_isr_DN")
    print ">>> Scanned isr down"

    hcnt.Add( gDirectory.Get("hcnt") )
    hup.Add( gDirectory.Get("hup") )
    hcent.Add( gDirectory.Get("hcent") )
    hdn.Add( gDirectory.Get("hdn") )

    hup.Divide(hcnt)
    hcent.Divide(hcnt)
    hdn.Divide(hcnt)

    f1.Close()

    outname = "isr_norm_%s.h" % name
    with open(outname, "w") as fhout:
        fhout.write("float isr_norm_%s(int mSp1, int mSp2, int which){\n" % name)
        for ix in range(hup.GetNbinsX()):
            for iy in range(hup.GetNbinsY()):
                n_up = hup.GetBinContent(ix,iy)
                n_dn = hdn.GetBinContent(ix,iy)
                n_cent = hcent.GetBinContent(ix,iy)
                if n_cent == 0: continue
                x,y = int(hup.GetXaxis().GetBinLowEdge(ix)), int(hup.GetYaxis().GetBinLowEdge(iy))
                if x == 0: x = 1 # LSP is mass 1 in pythia :(
                if y == 0: y = 1 # LSP is mass 1 in pythia :(
                fhout.write( "    if (mSp1 == %i && mSp2 == %i && which ==  1) return %f;\n" % (y,x,1.0/n_up) )
                fhout.write( "    if (mSp1 == %i && mSp2 == %i && which ==  0) return %f;\n" % (y,x,1.0/n_cent) )
                fhout.write( "    if (mSp1 == %i && mSp2 == %i && which == -1) return %f;\n" % (y,x,1.0/n_dn) )
        fhout.write("    return 0.0;\n")
        fhout.write("}\n")

if __name__ == "__main__":


    # name,fname = "t5qqqq_dm20", "../T5QQQQVV_dm20.root"
    # name,fname = "t1tttt", "../T1TTTT_main.root"

    for name, fname in [
        # ("t1tttt", "../T1TTTT_main.root"),
        # ("t5qqqq_dm20", "../T5QQQQVV_dm20.root"),
        # ("t5qqqq", "../T5QQQQVV_main.root"),
        # ("t6ttww", "/nfs-7/userdata/ss2015/ssBabies/v8.04/T6TTWW_main.root"),
        # ("t5ttcc", "/nfs-7/userdata/ss2015/ssBabies/v8.04/T5TTCC_main.root"),
        # ("t5tttt_dm175", "/nfs-7/userdata/ss2015/ssBabies/v8.04/T5TTTT_dm175.root"),
        ("t1ttbb", "/nfs-7/userdata/ss2015/ssBabies/v8.04/T1TTBB_main.root"),
            ]:
        main(name, fname)
