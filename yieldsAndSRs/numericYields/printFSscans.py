import os, glob, ROOT

scan = "T1TTTT"
#scan = "T6TTWW_50"
#scan = "T5QQQQVV"

files = glob.glob("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/"+scan+"*.root")

outf = open("fs_%s.h" % scan.lower(), 'w')

for fn in files:
    #print fn
    f = ROOT.TFile.Open(fn, "read")
    for key in f.GetListOfKeys():
        kname = key.GetName()
        #print kname
        if "csErr" in kname:
            masses = kname.split('_')
            #print masses[1], masses[2]
            outf.write("TChain *fs_%s_%s_%s_chain = new TChain(\"t\",\"fs_%s_m%s_m%s\" );\n" % (scan.lower(), masses[1], masses[2], scan.lower(), masses[1], masses[2]))
            outf.write("fs_%s_%s_%s_chain->Add(\"%s\");\n" % (scan.lower(), masses[1], masses[2], fn))
            outf.write("pair<yields_t, plots_t> results_fs_%s_%s_%s = run(fs_%s_%s_%s_chain, 0, 0, 0, 0, 1);\n" % (scan.lower(), masses[1], masses[2], scan.lower(), masses[1], masses[2]))
            outf.write("delete fs_%s_%s_%s_chain;\n\n" % (scan.lower(), masses[1], masses[2]))

outf.close()