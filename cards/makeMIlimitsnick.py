import ROOT, array
import sys, glob

#### EDIT ME
dir="MIlimits_new_6p26"
lumi=6.26
#### END EDIT

met=[300,400,500,600,700,800,900,1000]
ht=[1125,1200,1300,1400,1500,1600,1700,1800,1900,2000]

doHT = True

obs=[]
exp=[]
m2s=[]
m1s=[]
p1s=[]
p2s=[]
x0e=[]

get_cut = lambda x: int(x.split(".mH")[1].split(".")[0])

files = glob.glob(dir+"/higgsCombineTest.HybridNew.mH*")
met_files = filter(lambda x: int(x.split(".mH")[1].split(".")[0]) <= 1000+1, files)
ht_files = filter(lambda x: int(x.split(".mH")[1].split(".")[0]) >= 1125-1, files)

files_obs = sorted([f for f in met_files if len(f.split("/")[-1].split(".")) == 5], key=get_cut)
files_exp = sorted([f for f in met_files if "quant0.500" in f], key=get_cut)
files_m2s = sorted([f for f in met_files if "quant0.025" in f], key=get_cut)
files_m1s = sorted([f for f in met_files if "quant0.160" in f], key=get_cut)
files_p1s = sorted([f for f in met_files if "quant0.840" in f], key=get_cut)
files_p2s = sorted([f for f in met_files if "quant0.975" in f], key=get_cut)


for fn in files_obs:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        obs.append(entry.limit/lumi)  
        x0e.append(0.)
        
for fn in files_exp:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        exp.append(entry.limit/lumi)  

for fn in files_m2s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        m2s.append(entry.limit/lumi)  

for fn in files_m1s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        m1s.append(entry.limit/lumi)  

for fn in files_p1s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        p1s.append(entry.limit/lumi)  

for fn in files_p2s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        p2s.append(entry.limit/lumi)  

print p2s
print p1s
print exp
print m1s
print m2s
print obs

#TGraphAsymmErrors needs the delta
for i in range(0,len(exp)):
    m2s[i]=exp[i]-m2s[i]
    m1s[i]=exp[i]-m1s[i]
    p1s[i]=p1s[i]-exp[i]
    p2s[i]=p2s[i]-exp[i]

ROOT.gStyle.SetOptStat(0)
c1 = ROOT.TCanvas("c1", "", 800, 800)
c1.cd()
padt = ROOT.TPad("p_tex", "p_tex", 0.0, 0.0, 1.0, 1.0)
padt.SetTopMargin(0.1)
padt.SetBottomMargin(0.1)
padt.SetRightMargin(0.05)
padt.SetLeftMargin(0.15)
padt.SetTickx()
padt.SetTicky()
padt.Draw()
padt.cd()

h = ROOT.TH2F("h","h",16,250,1050,5,0,5)
h.SetTitle("")
h.GetXaxis().SetTitle("E_{T}^{miss} (GeV)")
h.GetYaxis().SetTitle("#sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} limit at 95% CL (fb)")
h.GetXaxis().SetLabelSize(0.04)
h.GetYaxis().SetLabelSize(0.04)
h.GetXaxis().SetTitleSize(0.045)
h.GetYaxis().SetTitleSize(0.045)
h.GetXaxis().SetTitleOffset(1.0)
h.GetYaxis().SetTitleOffset(1.5)
h.Draw()

gr_s2b = ROOT.TGraphAsymmErrors(len(met),array.array('d', met),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', m2s),array.array('d', p2s))
gr_s2b.SetFillColor(ROOT.kYellow)
gr_s2b.Draw("3")
  
gr_s1b = ROOT.TGraphAsymmErrors(len(met),array.array('d', met),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', m1s),array.array('d', p1s))
gr_s1b.SetFillColor(ROOT.kGreen)
gr_s1b.Draw("3")

gexp = ROOT.TGraph(len(met), array.array('d', met), array.array('d', exp))
gexp.SetLineStyle(7)
gexp.SetLineWidth(3)
gexp.SetLineColor(ROOT.kBlack)
gexp.Draw("L")

gobs = ROOT.TGraph(len(met), array.array('d', met), array.array('d', obs))
gobs.SetMarkerStyle(ROOT.kFullCircle)
gobs.SetMarkerSize(1.5)
gobs.SetMarkerColor(ROOT.kBlack)
gobs.SetLineWidth(3)
gobs.SetLineColor(ROOT.kBlack)
gobs.Draw("PL")

cmstex = ROOT.TLatex(0.675,0.91, "%.2f fb^{-1} (13 TeV)" % lumi)
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

cmstexprel = ROOT.TLatex(0.29,0.91, "Preliminary" )
cmstexprel.SetNDC()
cmstexprel.SetTextSize(0.03)
cmstexprel.SetLineWidth(2)
cmstexprel.SetTextFont(52)
cmstexprel.Draw()

l1 = ROOT.TLegend(0.15, 0.70, 0.95, 0.90)
l1.SetTextFont(42)
l1.SetTextSize(0.036)
l1.SetShadowColor(ROOT.kWhite)
l1.SetFillColor(ROOT.kWhite)
l1.SetHeader("Model independent #sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} exclusion limit")
l1.AddEntry(gobs , "Observed", "lp")
l1.AddEntry(gexp , "Expected #pm 1 #sigma #pm 2 #sigma", "l")
l1.Draw()

LExp1 = ROOT.TGraphAsymmErrors(2)
LExp1.SetFillColor(ROOT.kYellow)
LExp1.SetPoint(0,250+ 3.8*(1050-250)/100, 5-2.08*(5-0)/100*10)
LExp1.SetPointError(0,0.,0.,0.15,0.15)
LExp1.SetPoint(1,250+21.2*(1050-250)/100, 5-2.08*(5-0)/100*10)
LExp1.SetPointError(1,0.,0.,0.15,0.15)
LExp1.Draw("3")

LExp2 = ROOT.TGraphAsymmErrors(2)
LExp2.SetFillColor(ROOT.kGreen)
LExp2.SetPoint(0,250+ 3.8*(1050-250)/100, 5-2.08*(5-0)/100*10)
LExp2.SetPointError(0,0.,0.,0.08,0.08)
LExp2.SetPoint(1,250+21.2*(1050-250)/100, 5-2.08*(5-0)/100*10)
LExp2.SetPointError(1,0.,0.,0.08,0.08)
LExp2.Draw("L3")

LExp = ROOT.TGraph(2)
LExp.SetLineColor(ROOT.kBlack)
LExp.SetLineStyle(7)
LExp.SetLineWidth(3)
LExp.SetPoint(0,250+ 3.8*(1050-250)/100, 5-2.08*(5-0)/100*10)
LExp.SetPoint(1,250+21.2*(1050-250)/100, 5-2.08*(5-0)/100*10)
LExp.Draw("L")

masstex = ROOT.TLatex(0.70,0.79, "H_{T} > 300 GeV" )
masstex.SetNDC()
masstex.SetTextSize(0.036)
masstex.SetLineWidth(2)
masstex.SetTextFont(42)
masstex.Draw()

c1.SaveAs("MET_MIXSEC_%s.pdf" % (str(lumi).replace(".","p")))

#####################################################################################################

if doHT:

    obs=[]
    exp=[]
    m2s=[]
    m1s=[]
    p1s=[]
    p2s=[]
    x0e=[]

    files_obs = sorted([f for f in ht_files if len(f.split("/")[-1].split(".")) == 5], key=get_cut)
    files_exp = sorted([f for f in ht_files if "quant0.500" in f], key=get_cut)
    files_m2s = sorted([f for f in ht_files if "quant0.025" in f], key=get_cut)
    files_m1s = sorted([f for f in ht_files if "quant0.160" in f], key=get_cut)
    files_p1s = sorted([f for f in ht_files if "quant0.840" in f], key=get_cut)
    files_p2s = sorted([f for f in ht_files if "quant0.975" in f], key=get_cut)

    print files_obs

    for fn in files_obs:
        f = ROOT.TFile.Open(fn)
        for entry in f.limit:
            #print entry.limit/lumi
            print f,entry.limit, lumi, entry.limit/lumi
            obs.append(entry.limit/lumi)  
            x0e.append(0.)

    for fn in files_exp:
        f = ROOT.TFile.Open(fn)
        for entry in f.limit:
            #print entry.limit/lumi
            exp.append(entry.limit/lumi)  

    for fn in files_m2s:
        f = ROOT.TFile.Open(fn)
        for entry in f.limit:
            # print entry.limit/lumi, entry.limitErr/entry.limit/lumi
            m2s.append(entry.limit/lumi)  

    for fn in files_m1s:
        f = ROOT.TFile.Open(fn)
        for entry in f.limit:
            #print entry.limit/lumi
            m1s.append(entry.limit/lumi)  

    for fn in files_p1s:
        f = ROOT.TFile.Open(fn)
        for entry in f.limit:
            #print entry.limit/lumi
            p1s.append(entry.limit/lumi)  

    for fn in files_p2s:
        f = ROOT.TFile.Open(fn)
        for entry in f.limit:
            #print entry.limit/lumi
            p2s.append(entry.limit/lumi)  

    print p2s
    print p1s
    print exp
    print m1s
    print m2s
    print "HT obs"
    print obs

    #TGraphAsymmErrors needs the delta
    for i in range(0,len(exp)):
        m2s[i]=exp[i]-m2s[i]
        m1s[i]=exp[i]-m1s[i]
        p1s[i]=p1s[i]-exp[i]
        p2s[i]=p2s[i]-exp[i]

    ROOT.gStyle.SetOptStat(0)
    c2 = ROOT.TCanvas("c2", "", 800, 800)
    c2.cd()
    padt2 = ROOT.TPad("p_tex", "p_tex", 0.0, 0.0, 1.0, 1.0)
    padt2.SetTopMargin(0.1)
    padt2.SetBottomMargin(0.1)
    padt2.SetRightMargin(0.05)
    padt2.SetLeftMargin(0.15)
    padt2.SetTickx()
    padt2.SetTicky()
    padt2.Draw()
    padt2.cd()

    h = ROOT.TH2F("h2","h2",20,1050,2050,5,0,5)
    h.SetTitle("")
    h.GetXaxis().SetTitle("H_{T} (GeV)")
    h.GetYaxis().SetTitle("#sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} limit at 95% CL (fb)")
    h.GetXaxis().SetLabelSize(0.04)
    h.GetYaxis().SetLabelSize(0.04)
    h.GetXaxis().SetTitleSize(0.045)
    h.GetYaxis().SetTitleSize(0.045)
    h.GetXaxis().SetTitleOffset(1.0)
    h.GetYaxis().SetTitleOffset(1.5)
    h.GetXaxis().SetNdivisions(305)
    h.Draw()

    gr_s2b = ROOT.TGraphAsymmErrors(len(ht),array.array('d', ht),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', m2s),array.array('d', p2s))
    gr_s2b.SetFillColor(ROOT.kYellow)
    gr_s2b.Draw("3")
      
    gr_s1b = ROOT.TGraphAsymmErrors(len(ht),array.array('d', ht),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', m1s),array.array('d', p1s))
    gr_s1b.SetFillColor(ROOT.kGreen)
    gr_s1b.Draw("3")

    gexp = ROOT.TGraph(len(ht), array.array('d', ht), array.array('d', exp))
    gexp.SetLineStyle(7)
    gexp.SetLineWidth(3)
    gexp.SetLineColor(ROOT.kBlack)
    gexp.Draw("L")

    gobs = ROOT.TGraph(len(ht), array.array('d', ht), array.array('d', obs))
    gobs.SetMarkerStyle(ROOT.kFullCircle)
    gobs.SetMarkerSize(1.5)
    gobs.SetMarkerColor(ROOT.kBlack)
    gobs.SetLineWidth(3)
    gobs.SetLineColor(ROOT.kBlack)
    gobs.Draw("PL")

    cmstex = ROOT.TLatex(0.675,0.91, "%.2f fb^{-1} (13 TeV)" % lumi)
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

    cmstexprel = ROOT.TLatex(0.29,0.91, "Preliminary" )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.03)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstexprel.Draw()

    l1 = ROOT.TLegend(0.15, 0.70, 0.95, 0.90)
    l1.SetTextFont(42)
    l1.SetTextSize(0.036)
    l1.SetShadowColor(ROOT.kWhite)
    l1.SetFillColor(ROOT.kWhite)
    l1.SetHeader("Model independent #sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} exclusion limit")
    l1.AddEntry(gobs , "Observed", "lp")
    l1.AddEntry(gexp , "Expected #pm 1 #sigma #pm 2 #sigma", "l")
    l1.Draw()

    LExp1 = ROOT.TGraphAsymmErrors(2)
    LExp1.SetFillColor(ROOT.kYellow)
    LExp1.SetPoint(0,1050+ 3.8*(2050-1050)/100, 5-2.08*(5-0)/100*10)
    LExp1.SetPointError(0,0.,0.,0.15,0.15)
    LExp1.SetPoint(1,1050+21.2*(2050-1050)/100, 5-2.08*(5-0)/100*10)
    LExp1.SetPointError(1,0.,0.,0.15,0.15)
    LExp1.Draw("3")

    LExp2 = ROOT.TGraphAsymmErrors(2)
    LExp2.SetFillColor(ROOT.kGreen)
    LExp2.SetPoint(0,1050+ 3.8*(2050-1050)/100, 5-2.08*(5-0)/100*10)
    LExp2.SetPointError(0,0.,0.,0.08,0.08)
    LExp2.SetPoint(1,1050+21.2*(2050-1050)/100, 5-2.08*(5-0)/100*10)
    LExp2.SetPointError(1,0.,0.,0.08,0.08)
    LExp2.Draw("L3")

    LExp = ROOT.TGraph(2)
    LExp.SetLineColor(ROOT.kBlack)
    LExp.SetLineStyle(7)
    LExp.SetLineWidth(3)
    LExp.SetPoint(0,1050+ 3.8*(2050-1050)/100, 5-2.08*(5-0)/100*10)
    LExp.SetPoint(1,1050+21.2*(2050-1050)/100, 5-2.08*(5-0)/100*10)
    LExp.Draw("L")

    masstex = ROOT.TLatex(0.62,0.79, "50 < E_{T}^{miss} < 300 GeV" )
    masstex.SetNDC()
    masstex.SetTextSize(0.036)
    masstex.SetLineWidth(2)
    masstex.SetTextFont(42)
    masstex.Draw()

    c2.SaveAs("HT_MIXSEC_%s.pdf" % (str(lumi).replace(".","p")))
