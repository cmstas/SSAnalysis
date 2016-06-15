import os, ROOT, array

mydir = "v6.02-statunc"
mylumi = "2.3"

#the first time you need to make both cards and limits
#if you no not delete logs, then next time you can skip cards and limits
redocards = False
redolimits = False
deletelogs = False

step = 25
minx = 600
maxx = 1700+step
miny = 0
maxy = 1150+step
minz = 1e-2
maxz = 50
maxyh = 1500
ybinsfirstxbin = 21

npx = (maxx-minx)/step
npy = (maxy-miny)/step
npyh = (maxyh-miny)/step

def smooth(g,h):
    h_tmp = g.GetHistogram()
    #set histo values from graph (interpolated)
    for xbin in range(1,h_tmp.GetNbinsX()+1):
        for ybin in range(1,h_tmp.GetNbinsY()+1):
            value = h_tmp.GetBinContent(xbin,ybin)
            h.SetBinContent(xbin,ybin,value)
    #trick to avoid zeros above the diagonal that would affect the smoothing
    for xbin in range(1,h.GetNbinsX()+1):
        for ybin in range(2,h.GetNbinsY()+1):
            if ybin>xbin+ybinsfirstxbin: 
                h.SetBinContent(xbin,ybin,h.GetBinContent(xbin,ybin-1))
    #smooth
    h.Smooth(1,"k5b")
    #h.Smooth(1,"k5b")
    #h.Smooth(1,"k5b")
    #now cleanup above the diagonal
    for xbin in range(1,h.GetNbinsX()+1):
        for ybin in range(1,h.GetNbinsY()+1):
            if ybin>xbin+ybinsfirstxbin: h.SetBinContent(xbin,ybin,0)

sigs = []

for mglu in range (minx,maxx,step):
    for mlsp in range (miny,maxy,step):
        if os.path.isfile(("%s/fs_t5qqqqww_m%i_m%i_histos_hihi_%sifb.root" % (mydir,mglu,mlsp,mylumi))) is False: continue
        if mglu==1600 and mlsp==1100: continue
        if mglu==1650 and mlsp==750 : continue
        if mglu==1650 and mlsp==800 : continue
        if mglu==1650 and mlsp==900 : continue
        if mglu==1650 and mlsp==950 : continue
        if mglu==1650 and mlsp==1050: continue
        if mglu==1650 and mlsp==1100: continue
        if mglu==1650 and mlsp==1150: continue
        if mglu==1700 and mlsp==600: continue
        if mglu==1700 and mlsp==700: continue
        if mglu==1700 and mlsp>=800: continue
        #print "found sig = fs_t5qqqqww_m%i_m%i" % (mglu,mlsp)
        sigs.append(("fs_t5qqqqww_m%i_m%i" % (mglu,mlsp)))

limit_obs = []
limit_exp = []
limit_sm1 = []
limit_sp1 = []

miny = miny - step/2.
maxy = maxy - step/2.
minx = minx - step/2.
maxx = maxx - step/2.

#print sigs

for sig in sigs:
    #print sig
    if redocards: os.system("python createCard.py "+mydir+" "+sig)
    if redolimits: os.system("combine -M Asymptotic "+mydir+"/card_"+sig+"_"+mylumi+"ifb-all.txt >& "+mydir+"/card_"+sig+"_"+mylumi+"ifb-all.log") # --run expected --noFitAsimov
    foundObs = False
    with open(mydir+"/card_"+sig+"_"+mylumi+"ifb-all.log") as f:
        for line in f:
            #print line
            if "Observed Limit" in line:
                val = line.split()[4]
                limit_obs.append(float(val))
                foundObs = True
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
    if not foundObs: print "obs not found for", sig

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

h_xsec_test = ROOT.TH2D("h_xsec_test","",npx,minx,maxx,npy,miny,maxy)
h_sobs_test = ROOT.TH2D("h_sobs_test","",npx,minx,maxx,npy,miny,maxy)

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
    h_xsec_test.SetBinContent(h_xsec_test.FindBin(mglu,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)))
    h_sobs_test.SetBinContent(h_sobs_test.FindBin(mglu,mlsp), limit_obs[count])
    print ("%-8s %4i %4i | %.1f | %.1f (-1s %.1f, +1s %.1f) | %.3f " % (smod , mglu, mlsp , limit_obs[count]  , limit_exp[count] , limit_sm1[count] , limit_sp1[count], hxsec.GetBinContent(hxsec.FindBin(mglu)) ) )
    count = count+1

#h_xsec_test.GetZaxis().SetRangeUser(1e-2,10e0)
#h_xsec_test.Draw("colz")
#print h_xsec_test.GetXaxis().GetBinLowEdge(1), h_xsec_test.GetXaxis().GetBinUpEdge(1)
#c1.SaveAs("xsec_test.pdf")

#c1.SetLogz(0)
#h_sobs_test.GetZaxis().SetRangeUser(0.1,2)
#h_sobs_test.Draw("colz")
#print h_sobs_test.GetXaxis().GetBinLowEdge(1), h_sobs_test.GetXaxis().GetBinUpEdge(1)
#c1.SaveAs("sobs_test.pdf")

g_xsec = ROOT.TGraph2D("g_xsec","",count,mglus,mlsps,v_xsec)
g_sexp = ROOT.TGraph2D("g_sexp","",count,mglus,mlsps,v_sexp)
g_ssm1 = ROOT.TGraph2D("g_ssm1","",count,mglus,mlsps,v_ssm1)
g_ssp1 = ROOT.TGraph2D("g_ssp1","",count,mglus,mlsps,v_ssp1)
g_sobs = ROOT.TGraph2D("g_sobs","",count,mglus,mlsps,v_sobs)
g_som1 = ROOT.TGraph2D("g_som1","",count,mglus,mlsps,v_som1)
g_sop1 = ROOT.TGraph2D("g_sop1","",count,mglus,mlsps,v_sop1)

g_xsec.SetNpx(npx);
g_xsec.SetNpy(npy);
g_sexp.SetNpx(npx);
g_sexp.SetNpy(npy);
g_ssm1.SetNpx(npx);
g_ssm1.SetNpy(npy);
g_ssp1.SetNpx(npx);
g_ssp1.SetNpy(npy);
g_sobs.SetNpx(npx);
g_sobs.SetNpy(npy);
g_som1.SetNpx(npx);
g_som1.SetNpy(npy);
g_sop1.SetNpx(npx);
g_sop1.SetNpy(npy);

h_xsec = ROOT.TH2D("h_xsec","",npx,minx,maxx,npyh,miny,maxyh)
smooth(g_xsec,h_xsec)
h_sexp = ROOT.TH2D("h_sexp","",npx,minx,maxx,npy,miny,maxy)
smooth(g_sexp,h_sexp)
h_ssm1 = ROOT.TH2D("h_ssm1","",npx,minx,maxx,npy,miny,maxy)
smooth(g_ssm1,h_ssm1)
h_ssp1 = ROOT.TH2D("h_ssp1","",npx,minx,maxx,npy,miny,maxy)
smooth(g_ssp1,h_ssp1)
h_sobs = ROOT.TH2D("h_sobs","",npx,minx,maxx,npy,miny,maxy)
smooth(g_sobs,h_sobs)
h_som1 = ROOT.TH2D("h_som1","",npx,minx,maxx,npy,miny,maxy)
smooth(g_som1,h_som1)
h_sop1 = ROOT.TH2D("h_sop1","",npx,minx,maxx,npy,miny,maxy)
smooth(g_sop1,h_sop1)

k_sexp = ROOT.TGraph2D(h_sexp)
h_sexp = k_sexp.GetHistogram()
k_ssm1 = ROOT.TGraph2D(h_ssm1)
h_ssm1 = k_ssm1.GetHistogram()
k_ssp1 = ROOT.TGraph2D(h_ssp1)
h_ssp1 = k_ssp1.GetHistogram()
k_sobs = ROOT.TGraph2D(h_sobs)
j_sobs = k_sobs.GetHistogram()
k_som1 = ROOT.TGraph2D(h_som1)
j_som1 = k_som1.GetHistogram()
k_sop1 = ROOT.TGraph2D(h_sop1)
j_sop1 = k_sop1.GetHistogram()

h_xsec.GetXaxis().SetLabelSize(0.04)
h_xsec.GetYaxis().SetLabelSize(0.04)
h_xsec.GetZaxis().SetLabelSize(0.04)
h_xsec.GetXaxis().SetTitleSize(0.045)
h_xsec.GetYaxis().SetTitleSize(0.045)
h_xsec.GetZaxis().SetTitleSize(0.045)
h_xsec.GetXaxis().SetTitle("m_{#tilde{g}} (GeV)")
h_xsec.GetYaxis().SetTitle("m_{#tilde{#chi}_{1}^{0}} (GeV)")
h_xsec.GetZaxis().SetTitle("95% CL upper limit on cross section (pb)")
h_xsec.GetXaxis().SetTitleOffset(1.0)
h_xsec.GetYaxis().SetTitleOffset(1.6)
h_xsec.GetZaxis().SetTitleOffset(1.3)

h_xsec.GetZaxis().SetRangeUser(minz,maxz)

#hack to fix hole
for xbin in range(42,47):
    for ybin in range(32,49):
        h_xsec.SetBinContent(xbin,ybin,h_xsec.GetBinContent(xbin-1,ybin-1))

h_xsec.Draw("colz")

cexplist = k_sexp.GetContourList(1.)
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

csm1list = k_ssm1.GetContourList(1.)
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

csp1list = k_ssp1.GetContourList(1.)
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

cobslist = k_sobs.GetContourList(1.)
max_points = -1
for i in range(0,len(cobslist)):
    n_points = cobslist[i].GetN()
    if n_points > max_points:
        cobs = cobslist[i]
        max_points = n_points

cobs.SetLineWidth(4)
cobs.SetLineStyle(1)
cobs.SetLineColor(ROOT.kBlack)
#for i in range(62,51,-1): cobs.RemovePoint(i)
#cobs.Print()
cobs.Draw("L same");

com1list = k_som1.GetContourList(1.)
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

cop1list = k_sop1.GetContourList(1.)
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

diag = ROOT.TLine(600,600,1300,1300)
diag.SetLineWidth(1)
diag.SetLineStyle(2)
diag.Draw("same")

diagtex = ROOT.TLatex(0.20,0.485, "m_{#tilde{g}} = m_{#tilde{#chi}_{1}^{0}}" )
diagtex.SetNDC()
diagtex.SetTextSize(0.03)
diagtex.SetTextAngle(41)
diagtex.SetLineWidth(2)
diagtex.SetTextFont(42)
diagtex.Draw()

l1 = ROOT.TLegend(0.15, 0.70, 0.83, 0.90)
l1.SetTextFont(42)
l1.SetTextSize(0.036)
l1.SetShadowColor(ROOT.kWhite)
l1.SetFillColor(ROOT.kWhite)
l1.SetHeader("pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow q#bar{q}'W#tilde{#chi}^{0}_{1}      NLO+NLL exclusion")
l1.AddEntry(cobs , "Observed #pm 1 #sigma_{theory}", "l")
l1.AddEntry(cexp , "Expected #pm 1 #sigma_{experiment}", "l")

cmstex = ROOT.TLatex(0.575,0.91, mylumi+" fb^{-1} (13 TeV)" )
cmstex.SetNDC()
cmstex.SetTextSize(0.04)
cmstex.SetLineWidth(2)
cmstex.SetTextFont(42)
cmstex.Draw()

cmstexbold = ROOT.TLatex(0.17,0.91, "CMS" )
cmstexbold.SetNDC()
cmstexbold.SetTextSize(0.05)
cmstexbold.SetLineWidth(2)
cmstexbold.SetTextFont(61)
cmstexbold.Draw()

#cmstexprel = ROOT.TLatex(0.26,0.91, "Preliminary" )
#cmstexprel.SetNDC()
#cmstexprel.SetTextSize(0.03)
#cmstexprel.SetLineWidth(2)
#cmstexprel.SetTextFont(52)
#cmstexprel.Draw()

masstex = ROOT.TLatex(0.18,0.67, "m_{#tilde{#chi}^{#pm}_{1}} = m_{#tilde{#chi}^{0}_{1}} + 20 GeV" )
masstex.SetNDC()
masstex.SetTextSize(0.03)
masstex.SetLineWidth(2)
masstex.SetTextFont(42)
masstex.Draw()

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
LObsP.SetPoint(0,minx+ 3.8*(maxx-minx)/100, maxyh-1.10*(maxyh-miny)/100*10)
LObsP.SetPoint(1,minx+21.2*(maxx-minx)/100, maxyh-1.10*(maxyh-miny)/100*10)
LObsP.Draw("LSAME")

LObsM = ROOT.TGraph(2)
LObsM.SetName("LObsM")
LObsM.SetTitle("LObsM")
LObsM.SetLineColor(ROOT.kBlack)
LObsM.SetLineStyle(1)
LObsM.SetLineWidth(2)
LObsM.SetMarkerStyle(20)
LObsM.SetPoint(0,minx+ 3.8*(maxx-minx)/100, maxyh-1.40*(maxyh-miny)/100*10)
LObsM.SetPoint(1,minx+21.2*(maxx-minx)/100, maxyh-1.40*(maxyh-miny)/100*10)
LObsM.Draw("LSAME")

LExpP = ROOT.TGraph(2)
LExpP.SetName("LExpP")
LExpP.SetTitle("LExpP")
LExpP.SetLineColor(ROOT.kRed)
LExpP.SetLineStyle(2)
LExpP.SetLineWidth(2)
LExpP.SetMarkerStyle(20)
LExpP.SetPoint(0,minx+ 3.8*(maxx-minx)/100, maxyh-1.93*(maxyh-miny)/100*10)
LExpP.SetPoint(1,minx+21.2*(maxx-minx)/100, maxyh-1.93*(maxyh-miny)/100*10)
LExpP.Draw("LSAME")

LExpM = ROOT.TGraph(2)
LExpM.SetName("LExpM")
LExpM.SetTitle("LExpM")
LExpM.SetLineColor(ROOT.kRed)
LExpM.SetLineStyle(2)
LExpM.SetLineWidth(2)
LExpM.SetMarkerStyle(20)
LExpM.SetPoint(0,minx+ 3.8*(maxx-minx)/100, maxyh-2.23*(maxyh-miny)/100*10)
LExpM.SetPoint(1,minx+21.2*(maxx-minx)/100, maxyh-2.23*(maxyh-miny)/100*10)
LExpM.Draw("LSAME")

c1.SaveAs("t5qqqqww_scan_xsec.pdf")

h_sobs.GetXaxis().SetLabelSize(0.035)
h_sobs.GetYaxis().SetLabelSize(0.035)
h_sobs.GetXaxis().SetTitle("m_{#tilde{g}} (GeV)")
h_sobs.GetYaxis().SetTitle("m_{#tilde{#chi}_{1}^{0}} (GeV)")
h_sobs.GetZaxis().SetTitle("95% CL observed upper limit on signal strength")
#h_sobs.GetXaxis().SetRangeUser(600,1750)
#h_sobs.GetYaxis().SetRangeUser(0,1200)
h_sobs.GetZaxis().SetRangeUser(0.01,10.0)
h_sobs.GetZaxis().SetLabelSize(0.035)
h_sobs.GetXaxis().SetTitleOffset(1.1)
h_sobs.GetYaxis().SetTitleOffset(1.6)
h_sobs.GetZaxis().SetTitleOffset(1.6)
h_sobs.Draw("colz")
cobs.Draw("samecont2");
c1.SaveAs("sobs.pdf")

h_sexp.GetXaxis().SetLabelSize(0.035)
h_sexp.GetYaxis().SetLabelSize(0.035)
h_sexp.GetXaxis().SetTitle("m_{#tilde{g}} (GeV)")
h_sexp.GetYaxis().SetTitle("m_{#tilde{#chi}_{1}^{0}} (GeV)")
h_sexp.GetZaxis().SetTitle("95% CL expected upper limit on signal strength")
h_sexp.GetZaxis().SetRangeUser(0.01,10.0)
h_sexp.GetZaxis().SetLabelSize(0.035)
h_sexp.GetXaxis().SetTitleOffset(1.1)
h_sexp.GetYaxis().SetTitleOffset(1.6)
h_sexp.GetZaxis().SetTitleOffset(1.6)
h_sexp.Draw("colz")
cexp.Draw("samecont2");
c1.SaveAs("sexp.pdf")

fout = ROOT.TFile("SUS15008_t5qqqqww.root","RECREATE")
hxsecwrite = h_xsec.Clone("xsec")
for xbin in range(1,hxsecwrite.GetNbinsX()+1):
    for ybin in range(1,hxsecwrite.GetNbinsY()+1):
        if hxsecwrite.GetYaxis().GetBinUpEdge(ybin)>1125: hxsecwrite.SetBinContent(xbin,ybin,0)
hxsecwrite.Write()
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
