import os, glob, ROOT

# tag = "v8.04"
# postfix=""
tag = "v9.06"
postfix="_new"

scans = [
    # "T1TTTT_main",
    # "T5QQQQVV_main",
    # "T5QQQQVV_dm20",
    # "T6TTWW_main",
    # "T5TTCC_main",
    # "T5TTTT_dm175",
    # "T1TTBB_main",
]

higgs_scans = [
        # "TTH_scan",
        # "THW_scan",
        # "THQ_scan",
        ]

for scan in higgs_scans:
    # files = glob.glob("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/"+scan+"*.root")
    files = glob.glob("/nfs-7/userdata/namin/tupler_babies/merged//SS/{0}/output/{1}*.root".format(tag,scan))
    print files

    outf = open("%s%s.h" % (scan.lower(),postfix), 'w')
    outf_ps = open("%s_ps%s.h" % (scan.lower(),postfix), 'w')

    for fn in files:
        #print fn
        f = ROOT.TFile.Open(fn, "read")
        t = f.Get("t")

        t.SetBranchStatus("*",0)
        t.SetBranchStatus("higgs_mass",1)
        t.SetEstimate(t.GetEntries());
        t.Draw("higgs_mass","","goff")
        v_masses = t.GetV1()
        masses = set({})
        for i in range(t.GetEntries()):
            masses.add(int(v_masses[i]))

        for mass in sorted(masses):
            outf.write("TChain *%s_%i_chain = new TChain(\"t\",\"%s_m%i\" );\n" % (scan.lower(), mass, scan.lower(), mass))
            outf.write("%s_%i_chain->Add(\"%s\");\n" % (scan.lower(), mass,  fn))
            outf.write("pair<yields_t, plots_t> results_%s_%i = run(%s_%i_chain, 0, 0, 0, 0, 1);\n" % (scan.lower(), mass,  scan.lower(), mass))
            outf.write("delete %s_%i_chain;\n\n" % (scan.lower(), mass))

            scan_ps = scan.lower()
            scan_ps = scan_ps.replace("tth","tta")
            scan_ps = scan_ps.replace("thw","taw")
            scan_ps = scan_ps.replace("thq","taq")
            outf_ps.write("TChain *%s_%i_chain = new TChain(\"t\",\"%s_m%i\" );\n" % (scan_ps, mass, scan_ps, mass))
            outf_ps.write("%s_%i_chain->Add(\"%s\");\n" % (scan_ps, mass,  fn))
            outf_ps.write("pair<yields_t, plots_t> results_%s_%i = run(%s_%i_chain, 0, 0, 0, 0, 1);\n" % (scan_ps, mass,  scan_ps, mass))
            outf_ps.write("delete %s_%i_chain;\n\n" % (scan_ps, mass))

        f.Close()
    outf.close()
    outf_ps.close()

# files = glob.glob("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/"+scan+"*.root")
the_dir = glob.glob("/nfs-7/userdata/namin/tupler_babies/merged//SS/{0}/output/".format(tag))[0]
scan = "higgs_scan"
outf = open("%s%s.h" % (scan.lower(),postfix), 'w')
outf_ps = open("%s_ps%s.h" % (scan.lower(),postfix), 'w')
for mass in range(350,570,20):
    outf.write("TChain *%s_%i_chain = new TChain(\"t\",\"%s_m%i\" );\n" % (scan.lower(), mass, scan.lower(), mass))
    for which in ["TTH_scan","THW_scan", "THQ_scan"]:
        outf.write("%s_%i_chain->Add(\"%s/%s.root\");\n" % (scan.lower(), mass,  the_dir,which))
    outf.write("pair<yields_t, plots_t> results_%s_%i = run(%s_%i_chain, 0, 0, 0, 0, 1);\n" % (scan.lower(), mass,  scan.lower(), mass))
    outf.write("delete %s_%i_chain;\n\n" % (scan.lower(), mass))
    scan_ps = scan.lower()
    scan_ps = "higgs_ps_scan"
    outf_ps.write("TChain *%s_%i_chain = new TChain(\"t\",\"%s_m%i\" );\n" % (scan_ps, mass, scan_ps, mass))
    for which in ["TTH_scan","THW_scan", "THQ_scan"]:
        outf_ps.write("%s_%i_chain->Add(\"%s/%s.root\");\n" % (scan_ps.lower(), mass,  the_dir,which))
    outf_ps.write("pair<yields_t, plots_t> results_%s_%i = run(%s_%i_chain, 0, 0, 0, 0, 1);\n" % (scan_ps, mass,  scan_ps, mass))
    outf_ps.write("delete %s_%i_chain;\n\n" % (scan_ps, mass))
outf.close()
outf_ps.close()

for scan in scans:
    # files = glob.glob("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/"+scan+"*.root")
    files = glob.glob("/nfs-7/userdata/namin/tupler_babies/merged//SS/{0}/output/{1}*.root".format(tag,scan))
    print files

    scan = scan.replace("_main","")

    outf = open("fs_%s%s.h" % (scan.lower(),postfix), 'w')

    for fn in files:
        #print fn
        f = ROOT.TFile.Open(fn, "read")
        t = f.Get("t")

        t.SetBranchStatus("*",0)
        t.SetBranchStatus("scale1fb",1)
        t.SetBranchStatus("sparms",1)
        t.SetEstimate(t.GetEntries());

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
