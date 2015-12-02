import os, ROOT, array

mydir = "v5.06"
mylumi = "2.1"

#the first time you need to make both cards and limits
#if you no not delete logs, then next time you can skip cards and limits
redocards = True
redolimits = True
deletelogs = False

sigs = []
for mglu in range (625,1975,25):
    for mlsp in range (0,1500,50):
        if os.path.isfile(("%s/fs_t1tttt_m%i_m%i_histos_hihi_%sifb.root" % (mydir,mglu,mlsp,mylumi))) is False: continue
        #print "found sig = fs_t1tttt_m%i_m%i" % (mglu,mlsp)
        sigs.append(("fs_t1tttt_m%i_m%i" % (mglu,mlsp)))

limit_obs = []
limit_exp = []
limit_sm1 = []
limit_sp1 = []

#print sigs

for sig in sigs:
    #print sig
    if redocards: os.system("python createCard.py "+mydir+" "+sig)
    if redolimits: os.system("combine -M Asymptotic "+mydir+"/card_"+sig+"_"+mylumi+"ifb-all.txt >& "+mydir+"/card_"+sig+"_"+mylumi+"ifb-all.log") # --run expected --noFitAsimov
    with open(mydir+"/card_"+sig+"_"+mylumi+"ifb-all.log") as f:
        for line in f:
            #print line
            if "Observed Limit" in line:
                val = line.split()[4]
                limit_obs.append(float(val))
            if "Expected 50" in line:
                val = line.split()[4]
                limit_exp.append(float(val))
            if "Expected 16" in line:
                val = line.split()[4]
                limit_sm1.append(float(val))
            if "Expected 84" in line:
                val = line.split()[4]
                limit_sp1.append(float(val))
    if deletelogs: os.system("rm "+mydir+"/card_"+sig+"_"+mylumi+"ifb-all.log")

#for sig in sigs:
#    os.system("combine -M ProfileLikelihood --significance gc.v1.24/card_"+sig+"_3.0ifb-all.txt -t -1 --expectSignal=1 >& tmp.log")
#    with open('tmp.log') as f:
#        for line in f:
#            if "Significance" in line:
#                val = line.split()[1]
#                significance3.append(float(val))
#    os.system("rm tmp.log")

#for sig in sigs:
#    os.system("combine -M BayesianToyMC gc.v1.24/card_"+sig+"_3.0ifb-all.txt >& tmp.log")
#    with open('tmp.log') as f:
#        for line in f:
#            if "Limit:" in line:
#                val = line.split()[3]
#                limit_b.append(float(val))
#    os.system("rm tmp.log")

fxsec = ROOT.TFile("xsec_susy_13tev.root")
hxsec = fxsec.Get("h_xsec_gluino")

ROOT.gStyle.SetOptStat(0)

h_xsec = ROOT.TH2F("h_xsec","",54,625,1975,30,0,1500)
h_sobs = ROOT.TH2F("h_sobs","",54,625,1975,30,0,1500)
h_sexp = ROOT.TH2F("h_sexp","",54,625,1975,30,0,1500)

count = 0
print "limits "
for sig in sigs:
    smod = sig.split("_")[1]
    mglu = int(sig.split("_")[2][1:])
    mlsp = int(sig.split("_")[3][1:])
    h_xsec.SetBinContent( h_xsec.FindBin(mglu,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
    h_sobs.SetBinContent( h_xsec.FindBin(mglu,mlsp), limit_obs[count] )
    h_sexp.SetBinContent( h_xsec.FindBin(mglu,mlsp), limit_exp[count] )
    if mglu-mlsp>425:
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_exp[count] )
    print ("%-8s %4i %4i | %.1f | %.1f (-1s %.1f, +1s %.1f) | %.3f " % (smod , mglu, mlsp , limit_obs[count]  , limit_exp[count] , limit_sm1[count] , limit_sp1[count], hxsec.GetBinContent(hxsec.FindBin(mglu)) ) )
    count = count + 1

#for xbin in range(1,h_sobs.GetNbinsX(),2):
#    for ybin in range(1,h_sobs.GetNbinsY()+1):
#        if ((600+xbin*25)-(-50+50*ybin))==175 and (600+xbin*25)<1100: 
#            print (600+xbin*25),(-50+50*ybin)
#            h_sobs.SetBinContent(xbin,ybin,1.0)
#            h_sobs.SetBinContent(xbin+1,ybin,1.0)
#            h_sobs.SetBinContent(xbin+1,ybin+1,1.0)

c1 = ROOT.TCanvas("c1", "", 800, 800)
c1.cd()
padt = ROOT.TPad("p_tex", "p_tex", 0.0, 0.0, 1.0, 1.0);
padt.SetTopMargin(0.08)
padt.SetBottomMargin(0.16)
padt.SetRightMargin(0.17)
padt.SetLeftMargin(0.18)
padt.Draw()
padt.cd()
padt.SetLogz()
h_xsec.GetXaxis().SetLabelSize(0.035)
h_xsec.GetYaxis().SetLabelSize(0.035)
h_xsec.GetXaxis().SetTitle("gluino mass [GeV]")
h_xsec.GetYaxis().SetTitle("#chi_{1}^{0} mass [GeV]")
h_xsec.GetZaxis().SetTitle("95% CL upper limit on #sigma [pb]")
h_xsec.GetZaxis().SetRangeUser(1e-2,1e0)
h_xsec.GetZaxis().SetLabelSize(0.035)
h_xsec.GetXaxis().SetTitleOffset(1.1)
h_xsec.GetYaxis().SetTitleOffset(1.6)
h_xsec.GetZaxis().SetTitleOffset(1.6)
#contourplot = h_sobs.Clone("contourplot");
#contours = array.array('d')
#contours.append(1.0)
#contourplot.SetContour(1, contours);
#contourplot.SetLineWidth(4);
#contourplot.SetLineStyle(2);
#contourplot.SetLineColor(ROOT.kRed);
#contourplot.Smooth();
h_xsec.Draw("colz")
#contourplot.Draw("samecont2");
c1.SaveAs("xsec.png")

h_sobs.GetXaxis().SetLabelSize(0.035)
h_sobs.GetYaxis().SetLabelSize(0.035)
h_sobs.GetXaxis().SetTitle("gluino mass [GeV]")
h_sobs.GetYaxis().SetTitle("#chi_{1}^{0} mass [GeV]")
h_sobs.GetZaxis().SetTitle("95% CL observed upper limit on signal strength")
h_sobs.GetZaxis().SetRangeUser(0.2,5.0)
h_sobs.GetZaxis().SetLabelSize(0.035)
h_sobs.GetXaxis().SetTitleOffset(1.1)
h_sobs.GetYaxis().SetTitleOffset(1.6)
h_sobs.GetZaxis().SetTitleOffset(1.6)
h_sobs.Draw("colz")
#contourplot.Draw("samecont2");
c1.SaveAs("sobs.png")

h_sexp.GetXaxis().SetLabelSize(0.035)
h_sexp.GetYaxis().SetLabelSize(0.035)
h_sexp.GetXaxis().SetTitle("gluino mass [GeV]")
h_sexp.GetYaxis().SetTitle("#chi_{1}^{0} mass [GeV]")
h_sexp.GetZaxis().SetTitle("95% CL expected upper limit on signal strength")
h_sexp.GetZaxis().SetRangeUser(0.2,5.0)
h_sexp.GetZaxis().SetLabelSize(0.035)
h_sexp.GetXaxis().SetTitleOffset(1.1)
h_sexp.GetYaxis().SetTitleOffset(1.6)
h_sexp.GetZaxis().SetTitleOffset(1.6)
h_sexp.Draw("colz")
c1.SaveAs("sexp.png")
