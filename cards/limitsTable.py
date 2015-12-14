import os, ROOT, array

mydir = "v5.07-lumi216-newfs-statunc"
mylumi = "2.2"

#the first time you need to make both cards and limits
#if you no not delete logs, then next time you can skip cards and limits
redocards = False
redolimits = False
deletelogs = False

sigs = []

for mglu in range (625,1975,25):
    for mlsp in range (0,1500,25):
        if os.path.isfile(("%s/fs_t1tttt_m%i_m%i_histos_hihi_%sifb.root" % (mydir,mglu,mlsp,mylumi))) is False: continue
        print "found sig = fs_t1tttt_m%i_m%i" % (mglu,mlsp)
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

ROOT.gStyle.SetOptStat(0)
#"Official" SUSY palette
mypalette = array.array('i')
NRGBs = 5
NCont = 255
stops = array.array('d', [0.00, 0.34, 0.61, 0.84, 1.00])
red   = array.array('d', [0.50, 0.50, 1.00, 1.00, 1.00])
green = array.array('d', [0.50, 1.00, 1.00, 0.60, 0.50])
blue  = array.array('d', [1.00, 1.00, 0.50, 0.40, 0.50])
FI = ROOT.TColor.CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont)
for i in range (0,NCont): mypalette.append(FI+i)
ROOT.gStyle.SetPalette(NCont,mypalette)
ROOT.gStyle.SetNumberContours(NCont)

c1 = ROOT.TCanvas("c1", "", 800, 800)
c1.cd()
padt = ROOT.TPad("p_tex", "p_tex", 0.0, 0.0, 1.0, 1.0);
padt.SetTopMargin(0.1)
padt.SetBottomMargin(0.1)
padt.SetRightMargin(0.17)
padt.SetLeftMargin(0.15)
padt.Draw()
padt.cd()
padt.SetLogz()

fxsec = ROOT.TFile("xsec_susy_13tev.root")
hxsec = fxsec.Get("h_xsec_gluino")

#h_xsec_test = ROOT.TH2D("h_xsec_test","",54,625,1975,80,0,2000)

count = 0
mglus = array.array('d')
mlsps = array.array('d')
v_xsec = array.array('d')
v_sexp = array.array('d')
v_ssm1 = array.array('d')
v_ssp1 = array.array('d')
v_sobs = array.array('d')
v_som1 = array.array('d')
v_sop1 = array.array('d')
for sig in sigs:
    #print sig
    smod = sig.split("_")[1]
    mglu = int(sig.split("_")[2][1:])
    mglus.append(mglu)
    mlsp = int(sig.split("_")[3][1:])
    mlsps.append(mlsp)
    v_xsec.append( limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
    v_sobs.append( limit_obs[count] )
    v_som1.append( limit_obs[count]*(hxsec.GetBinContent(hxsec.FindBin(mglu))-hxsec.GetBinError(hxsec.FindBin(mglu)))/hxsec.GetBinContent(hxsec.FindBin(mglu)) )
    v_sop1.append( limit_obs[count]*(hxsec.GetBinContent(hxsec.FindBin(mglu))+hxsec.GetBinError(hxsec.FindBin(mglu)))/hxsec.GetBinContent(hxsec.FindBin(mglu)) )
    v_sexp.append( limit_exp[count] )
    v_ssm1.append( limit_sm1[count] )
    v_ssp1.append( limit_sp1[count] )
    #h_xsec_test.SetBinContent(h_xsec_test.FindBin(mglu,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)))
    print ("%-8s %4i %4i | %.1f | %.1f (-1s %.1f, +1s %.1f) | %.3f " % (smod , mglu, mlsp , limit_obs[count]  , limit_exp[count] , limit_sm1[count] , limit_sp1[count], hxsec.GetBinContent(hxsec.FindBin(mglu)) ) )
    count = count+1

#g_xsec = ROOT.TGraph2D(h_xsec_test)
g_xsec = ROOT.TGraph2D("g_xsec","",count,mglus,mlsps,v_xsec)
g_sexp = ROOT.TGraph2D("g_sexp","",count,mglus,mlsps,v_sexp)
g_ssm1 = ROOT.TGraph2D("g_ssm1","",count,mglus,mlsps,v_ssm1)
g_ssp1 = ROOT.TGraph2D("g_ssp1","",count,mglus,mlsps,v_ssp1)
g_sobs = ROOT.TGraph2D("g_sobs","",count,mglus,mlsps,v_sobs)
g_som1 = ROOT.TGraph2D("g_som1","",count,mglus,mlsps,v_som1)
g_sop1 = ROOT.TGraph2D("g_sop1","",count,mglus,mlsps,v_sop1)

#h_xsec_test.GetZaxis().SetRangeUser(5e-3,2e0)
#h_xsec_test.Draw("colz")
#c1.SaveAs("xsec.png")

g_xsec.SetNpx(54);
g_xsec.SetNpy(60);

h_xsec_tmp = g_xsec.GetHistogram()

h_xsec = ROOT.TH2D("h_xsec","",54,625,1975,80,0,2000)
for xbin in range(1,h_xsec_tmp.GetNbinsX()+1):
    for ybin in range(1,h_xsec_tmp.GetNbinsY()+1):
        value = h_xsec_tmp.GetBinContent(xbin,ybin)
        #h_xsec.FindBin(h_xsec_tmp.GetXaxis().GetBinCenter(xbin),h_xsec_tmp.GetYaxis().GetBinCenter(ybin))
        h_xsec.SetBinContent(xbin,ybin,value)
h_sexp = g_sexp.GetHistogram()
h_ssm1 = g_ssm1.GetHistogram()
h_ssp1 = g_ssp1.GetHistogram()
h_sobs = g_sobs.GetHistogram()
h_som1 = g_som1.GetHistogram()
h_sop1 = g_sop1.GetHistogram()

#hacks for aesthetics
h_xsec.SetBinContent(1,1,0.09)
h_xsec.SetBinContent(2,1,0.09)
h_xsec.SetBinContent(3,1,0.09)
h_xsec.SetBinContent(1,2,0.09)
h_xsec.SetBinContent(2,2,0.09)
h_xsec.SetBinContent(3,2,0.09)
h_xsec.SetBinContent(1,3,0.10)
h_xsec.SetBinContent(2,3,0.10)
h_xsec.SetBinContent(1,4,0.10)
h_xsec.SetBinContent(2,4,0.10)
h_xsec.SetBinContent(1,5,0.10)
h_xsec.SetBinContent(2,5,0.10)
h_xsec.SetBinContent(1,6,0.10)
h_xsec.SetBinContent(2,6,0.10)
h_xsec.SetBinContent(1,7,0.11)
h_xsec.SetBinContent(1,8,0.11)
h_xsec.SetBinContent(1,9,0.12)
h_xsec.SetBinContent(30,47,0)
h_xsec.SetBinContent(31,48,0)
h_xsec.SetBinContent(32,49,0)
#
h_xsec.GetXaxis().SetLabelSize(0.035)
h_xsec.GetYaxis().SetLabelSize(0.035)
h_xsec.GetXaxis().SetTitle("m_{#tilde{g}} (GeV)")
h_xsec.GetYaxis().SetTitle("m_{#tilde{#chi_{1}^{0}}} (GeV)")
h_xsec.GetZaxis().SetTitle("95% CL upper limit on cross section (pb)")
h_xsec.GetZaxis().SetLabelSize(0.035)
h_xsec.GetXaxis().SetTitleOffset(1.2)
h_xsec.GetYaxis().SetTitleOffset(1.7)
h_xsec.GetZaxis().SetTitleOffset(1.6)

h_xsec.GetXaxis().SetRangeUser(700,1950)
h_xsec.GetYaxis().SetRangeUser(0,1900)
h_xsec.GetZaxis().SetRangeUser(1e-3,2)

h_xsec.Draw("colz")

contarray = [1.0]

h_sexp.Clone().SetContour(1,array.array('d',contarray))
cexplist = g_sexp.GetContourList(1.)
max_points = -1
for i in range(0,len(cexplist)):
    n_points = cexplist[i].GetN()
    if n_points > max_points:
        cexp = cexplist[i]
        max_points = n_points
cexp.SetLineWidth(4)
cexp.SetLineStyle(2)
cexp.SetLineColor(ROOT.kRed)
cexp.Draw("L same");

h_ssm1.Clone().SetContour(1,array.array('d',contarray))
csm1list = g_ssm1.GetContourList(1.)
max_points = -1
for i in range(0,len(csm1list)):
    n_points = csm1list[i].GetN()
    if n_points > max_points:
        csm1 = csm1list[i]
        max_points = n_points
csm1.SetLineWidth(2)
csm1.SetLineStyle(2)
csm1.SetLineColor(ROOT.kRed)
csm1.Draw("L same");

h_ssp1.Clone().SetContour(1,array.array('d',contarray))
csp1list = g_ssp1.GetContourList(1.)
max_points = -1
for i in range(0,len(csp1list)):
    n_points = csp1list[i].GetN()
    if n_points > max_points:
        csp1 = csp1list[i]
        max_points = n_points
csp1.SetLineWidth(2)
csp1.SetLineStyle(2)
csp1.SetLineColor(ROOT.kRed)
csp1.Draw("L same");

h_sobs.Clone().SetContour(1,array.array('d',contarray))
cobslist = g_sobs.GetContourList(1.)
max_points = -1
for i in range(0,len(cobslist)):
    n_points = cobslist[i].GetN()
    if n_points > max_points:
        cobs = cobslist[i]
        max_points = n_points
cobs.SetLineWidth(4)
cobs.SetLineStyle(1)
cobs.SetLineColor(ROOT.kBlack)
#cobs.Print()
cobs.Draw("L same");

h_som1.Clone().SetContour(1,array.array('d',contarray))
com1list = g_som1.GetContourList(1.)
max_points = -1
for i in range(0,len(com1list)):
    n_points = com1list[i].GetN()
    if n_points > max_points:
        com1 = com1list[i]
        max_points = n_points
com1.SetLineWidth(2)
com1.SetLineStyle(1)
com1.SetLineColor(ROOT.kBlack)
com1.Draw("L same");

h_sop1.Clone().SetContour(1,array.array('d',contarray))
cop1list = g_sop1.GetContourList(1.)
max_points = -1
for i in range(0,len(cop1list)):
    n_points = cop1list[i].GetN()
    if n_points > max_points:
        cop1 = cop1list[i]
        max_points = n_points
cop1.SetLineWidth(2)
cop1.SetLineStyle(1)
cop1.SetLineColor(ROOT.kBlack)
cop1.Draw("L same");

diag = ROOT.TLine(700,530,1700,1530)
diag.SetLineWidth(1)
diag.SetLineStyle(2)
diag.Draw("same")

diagtex = ROOT.TLatex(0.20,0.38, "m_{#tilde{g}}-m_{#tilde{#chi_{1}^{0}}} = 2 #upoint (m_{W} + m_{b})" )
diagtex.SetNDC()
diagtex.SetTextSize(0.02)
diagtex.SetTextAngle(38)
diagtex.SetLineWidth(2)
diagtex.SetTextFont(42)
diagtex.Draw()

l1 = ROOT.TLegend(0.15, 0.70, 0.83, 0.90)
l1.SetTextFont(42)
l1.SetTextSize(0.038)
l1.SetShadowColor(ROOT.kWhite)
l1.SetFillColor(ROOT.kWhite)
l1.SetHeader("pp #rightarrow #tilde{g} #tilde{g}, #tilde{g}#rightarrow t#bar{t}#tilde{#chi}^{0}_{1}     NLO+NLL Exclusions")
l1.AddEntry(cobs , "Observed #pm 1 #sigma_{theory}", "l")
l1.AddEntry(cexp , "Expected #pm 1 #sigma_{experiment}", "l")

cmstex = ROOT.TLatex(0.575,0.91, "2.2 fb^{-1} (13 TeV)" )
cmstex.SetNDC()
cmstex.SetTextSize(0.04)
cmstex.SetLineWidth(2)
cmstex.SetTextFont(42)
cmstex.Draw()

cmstexbold = ROOT.TLatex(0.17,0.91, "CMS" )
cmstexbold.SetNDC()
cmstexbold.SetTextSize(0.04)
cmstexbold.SetLineWidth(2)
cmstexbold.SetTextFont(61)
cmstexbold.Draw()

cmstexprel = ROOT.TLatex(0.26,0.91, "Preliminary" )
cmstexprel.SetNDC()
cmstexprel.SetTextSize(0.03)
cmstexprel.SetLineWidth(2)
cmstexprel.SetTextFont(52)
cmstexprel.Draw()

padt.Update()
padt.RedrawAxis();
l1.Draw("same")

LObsP = ROOT.TGraph(2)
LObsP.SetName("LObsP")
LObsP.SetTitle("LObsP")
LObsP.SetLineColor(ROOT.kBlack)
LObsP.SetLineStyle(1)
LObsP.SetLineWidth(2)
LObsP.SetMarkerStyle(20)
LObsP.SetPoint(0,700+ 2.4*1950/100, 1900-1.10*1900/100*10)
LObsP.SetPoint(1,700+13.6*1950/100, 1900-1.10*1900/100*10)
LObsP.Draw("LSAME")

LObsM = ROOT.TGraph(2)
LObsM.SetName("LObsM")
LObsM.SetTitle("LObsM")
LObsM.SetLineColor(ROOT.kBlack)
LObsM.SetLineStyle(1)
LObsM.SetLineWidth(2)
LObsM.SetMarkerStyle(20)
LObsM.SetPoint(0,700+ 2.4*1950/100, 1900-1.40*1900/100*10)
LObsM.SetPoint(1,700+13.6*1950/100, 1900-1.40*1900/100*10)
LObsM.Draw("LSAME")

LExpP = ROOT.TGraph(2)
LExpP.SetName("LExpP")
LExpP.SetTitle("LExpP")
LExpP.SetLineColor(ROOT.kRed)
LExpP.SetLineStyle(2)
LExpP.SetLineWidth(2)
LExpP.SetMarkerStyle(20)
LExpP.SetPoint(0,700+ 2.4*1950/100, 1900-1.93*1900/100*10)
LExpP.SetPoint(1,700+13.6*1950/100, 1900-1.93*1900/100*10)
LExpP.Draw("LSAME")

LExpM = ROOT.TGraph(2)
LExpM.SetName("LExpM")
LExpM.SetTitle("LExpM")
LExpM.SetLineColor(ROOT.kRed)
LExpM.SetLineStyle(2)
LExpM.SetLineWidth(2)
LExpM.SetMarkerStyle(20)
LExpM.SetPoint(0,700+ 2.4*1950/100, 1900-2.23*1900/100*10)
LExpM.SetPoint(1,700+13.6*1950/100, 1900-2.23*1900/100*10)
LExpM.Draw("LSAME")

c1.SaveAs("t1tttt_scan_xsec.pdf")

h_sobs.GetXaxis().SetLabelSize(0.035)
h_sobs.GetYaxis().SetLabelSize(0.035)
h_sobs.GetXaxis().SetTitle("m_{#tilde{g}} (GeV)")
h_sobs.GetYaxis().SetTitle("m_{#tilde{#chi_{1}^{0}}} (GeV)")
h_sobs.GetZaxis().SetTitle("95% CL observed upper limit on signal strength")
h_sobs.GetZaxis().SetRangeUser(0.01,10.0)
h_sobs.GetZaxis().SetLabelSize(0.035)
h_sobs.GetXaxis().SetTitleOffset(1.1)
h_sobs.GetYaxis().SetTitleOffset(1.6)
h_sobs.GetZaxis().SetTitleOffset(1.6)
h_sobs.Draw("colz")
cobs.Draw("samecont2");
c1.SaveAs("sobs.png")

h_sexp.GetXaxis().SetLabelSize(0.035)
h_sexp.GetYaxis().SetLabelSize(0.035)
h_sexp.GetXaxis().SetTitle("m_{#tilde{g}} (GeV)")
h_sexp.GetYaxis().SetTitle("m_{#tilde{#chi_{1}^{0}}} (GeV)")
h_sexp.GetZaxis().SetTitle("95% CL expected upper limit on signal strength")
h_sexp.GetZaxis().SetRangeUser(0.01,10.0)
h_sexp.GetZaxis().SetLabelSize(0.035)
h_sexp.GetXaxis().SetTitleOffset(1.1)
h_sexp.GetYaxis().SetTitleOffset(1.6)
h_sexp.GetZaxis().SetTitleOffset(1.6)
h_sexp.Draw("colz")
cexp.Draw("samecont2");
c1.SaveAs("sexp.png")

fout = ROOT.TFile("ss_t1tttt_scan_xsec.root","RECREATE")
cobswrite = cobs.Clone("ssobs")
cobswrite.Write()
com1write = com1.Clone("ssobs_m1s")
com1write.Write()
cop1write = cop1.Clone("ssobs_p1s")
cop1write.Write()
cexpwrite = cexp.Clone("ssexp")
cexpwrite.Write()
csm1write = csm1.Clone("ssexp_m1s")
csm1write.Write()
csp1write = csp1.Clone("ssexp_p1s")
csp1write.Write()
fout.Close()
