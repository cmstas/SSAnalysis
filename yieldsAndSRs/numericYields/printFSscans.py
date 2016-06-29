import os, glob, ROOT

scan = "T1TTTT_main"
#scan = "T6TTWW_50"
#scan = "T5QQQQVV"
#scan = "T5QQQQWW"
#scan = "T1TTBB"
#scan = "T5ttttDM175"
#scan = "T5tttt_degen"
#scan = "T5ttcc"

files = glob.glob("/nfs-7/userdata/ss2015/ssBabies/v8.02/"+scan+"*.root")

scan = scan.replace("_main","")

outf = open("fs_%s.h" % scan.lower(), 'w')

for fn in files:
    #print fn
    f = ROOT.TFile.Open(fn, "read")
    t = f.Get("t")
    for key in f.GetListOfKeys():
        kname = key.GetName()
        #print kname
        if "csErr" in kname:
            masses = kname.split('_')
            # print masses[1], masses[2]
            # if masses[1]  != "1300" or masses[2] != "100": continue
            # if masses[1]  != 1300: continue
            print "GOOD:",masses[1], masses[2]
            h = ROOT.TH1F("h_%s_%s"%(masses[1], masses[2]),"h_%s_%s"%(masses[1], masses[2]),2,-1,1)
            t.Draw("scale1fb>>h_%s_%s"%(masses[1], masses[2]),"sparms[0]==%s && sparms[1]==%s" % (masses[1], masses[2]),"goff")
            if h.GetMean()<0: 
                print "negative average weight for mass point %s, %s" % (masses[1], masses[2])
                continue
            outf.write("TChain *fs_%s_%s_%s_chain = new TChain(\"t\",\"fs_%s_m%s_m%s\" );\n" % (scan.lower(), masses[1], masses[2], scan.lower(), masses[1], masses[2]))
            outf.write("fs_%s_%s_%s_chain->Add(\"%s\");\n" % (scan.lower(), masses[1], masses[2], fn))
            outf.write("pair<yields_t, plots_t> results_fs_%s_%s_%s = run(fs_%s_%s_%s_chain, 0, 0, 0, 0, 1);\n" % (scan.lower(), masses[1], masses[2], scan.lower(), masses[1], masses[2]))
            outf.write("delete fs_%s_%s_%s_chain;\n\n" % (scan.lower(), masses[1], masses[2]))
    f.Close()
outf.close()
