import ROOT, array

dir="v6.02-mi"
lumi=3.99

met=[300,400,500,600,700,800,900,1000]
ht=[1125,1200,1300,1400,1500,1600,1700,1800,1900,2000]

obs=[]
exp=[]
m2s=[]
m1s=[]
p1s=[]
p2s=[]
x0e=[]

files_obs=[]
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH300.1055246857.root")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH400.1230859910.root")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH500.-1163351178.root")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH600.-2019227517.root")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH700.2094051393.root")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH800.-1600239908.root")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH900.449350061.root")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1000.-2012955751.root")
for fn in files_obs:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        obs.append(entry.limit/lumi)  
        x0e.append(0.)

files_exp=[]
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH300.933834827.quant0.500.root  ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH400.1576345934.quant0.500.root ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH500.-1072993202.quant0.500.root")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH600.1331906520.quant0.500.root ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH700.154502427.quant0.500.root  ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH800.-1219041915.quant0.500.root")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH900.-783424788.quant0.500.root ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1000.1085533504.quant0.500.root")
for fn in files_exp:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        exp.append(entry.limit/lumi)  

files_m2s=[]
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH300.-252547084.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH400.-1428335110.quant0.025.root")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH500.1622078588.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH600.1443741644.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH700.821663130.quant0.025.root  ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH800.-633553797.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH900.2022401024.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1000.-149935760.quant0.025.root")
for fn in files_m2s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        m2s.append(entry.limit/lumi)  

files_m1s=[]
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH300.-1036663579.quant0.160.root")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH400.-514753103.quant0.160.root ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH500.942899326.quant0.160.root  ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH600.76285539.quant0.160.root   ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH700.-1166684867.quant0.160.root")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH800.-360267007.quant0.160.root ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH900.2124738992.quant0.160.root ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1000.1913562600.quant0.160.root")
for fn in files_m1s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        m1s.append(entry.limit/lumi)  

files_p1s=[]
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH300.-1200664757.quant0.840.root ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH400.-2090875840.quant0.840.root ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH500.497753482.quant0.840.root   ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH600.2105939079.quant0.840.root  ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH700.-1293205450.quant0.840.root ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH800.851976575.quant0.840.root   ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH900.1817674162.quant0.840.root  ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1000.-1797958744.quant0.840.root")
for fn in files_p1s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        p1s.append(entry.limit/lumi)  

files_p2s=[]
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH300.129536121.quant0.975.root  ")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH400.-712780005.quant0.975.root ")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH500.-1557601378.quant0.975.root")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH600.-778116707.quant0.975.root ")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH700.-2117428643.quant0.975.root")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH800.1858161290.quant0.975.root ")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH900.1662863840.quant0.975.root ")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1000.-998003919.quant0.975.root")
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

cmstexprel = ROOT.TLatex(0.26,0.91, "Preliminary" )
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

c1.SaveAs("SR31_MIXSEC.pdf")

#####################################################################################################


obs=[]
exp=[]
m2s=[]
m1s=[]
p1s=[]
p2s=[]
x0e=[]

files_obs=[]
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1125.421931988.root   ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1200.-1994769184.root ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1300.-1780180844.root ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1400.-1197446448.root ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1500.-1847439251.root ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1600.-196328912.root  ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1700.-1466234442.root ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1800.936113736.root   ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH1900.47260678.root    ")
files_obs.append(dir+"/higgsCombineTest.HybridNew.mH2000.-963769095.root  ")
for fn in files_obs:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        obs.append(entry.limit/lumi)  
        x0e.append(0.)

files_exp=[]
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1125.-182462425.quant0.500.root ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1200.438834775.quant0.500.root  ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1300.-1194809231.quant0.500.root")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1400.-1425359371.quant0.500.root")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1500.1994921680.quant0.500.root ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1600.-328146502.quant0.500.root ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1700.-2086602109.quant0.500.root")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1800.2041385904.quant0.500.root ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH1900.-757093643.quant0.500.root ")
files_exp.append(dir+"/higgsCombineTest.HybridNew.mH2000.70642719.quant0.500.root   ")
for fn in files_exp:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        exp.append(entry.limit/lumi)  

files_m2s=[]
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1125.-831553152.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1200.-181131058.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1300.-774231459.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1400.464157351.quant0.025.root  ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1500.293773012.quant0.025.root  ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1600.1067393466.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1700.-827921994.quant0.025.root ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1800.125714726.quant0.025.root  ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH1900.687310325.quant0.025.root  ")
files_m2s.append(dir+"/higgsCombineTest.HybridNew.mH2000.964245984.quant0.025.root  ")
for fn in files_m2s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        print entry.limit/lumi, entry.limitErr/entry.limit/lumi
        m2s.append(entry.limit/lumi)  

files_m1s=[]
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1125.1723870869.quant0.160.root ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1200.703949239.quant0.160.root  ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1300.-665321842.quant0.160.root ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1400.-204598552.quant0.160.root ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1500.-820033554.quant0.160.root ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1600.-1837510878.quant0.160.root")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1700.32699023.quant0.160.root   ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1800.236887131.quant0.160.root  ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH1900.9399007.quant0.160.root    ")
files_m1s.append(dir+"/higgsCombineTest.HybridNew.mH2000.847229035.quant0.160.root  ")
for fn in files_m1s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        m1s.append(entry.limit/lumi)  

files_p1s=[]
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1125.1999564852.quant0.840.root ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1200.-797832854.quant0.840.root ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1300.981962595.quant0.840.root  ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1400.-1339456059.quant0.840.root")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1500.-1586365909.quant0.840.root")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1600.1327910416.quant0.840.root ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1700.-1314291141.quant0.840.root")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1800.1727497139.quant0.840.root ")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH1900.-1478389588.quant0.840.root")
files_p1s.append(dir+"/higgsCombineTest.HybridNew.mH2000.-176787656.quant0.840.root ")
for fn in files_p1s:
    f = ROOT.TFile.Open(fn)
    for entry in f.limit:
        #print entry.limit/lumi
        p1s.append(entry.limit/lumi)  

files_p2s=[]
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1125.-1120954457.quant0.975.root")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1200.1316136570.quant0.975.root ")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1300.-1575594365.quant0.975.root")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1400.-973993225.quant0.975.root ") #2.1749950
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1500.-371594167.quant0.975.root ") #1.9885212
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1600.-640252412.quant0.975.root ") #1.9315906
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1700.-2054291277.quant0.975.root") #1.8101379
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1800.105214299.quant0.975.root  ") #1.6246484
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH1900.-1187800579.quant0.975.root")
files_p2s.append(dir+"/higgsCombineTest.HybridNew.mH2000.-2014766644.quant0.975.root")
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

cmstexprel = ROOT.TLatex(0.26,0.91, "Preliminary" )
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

c2.SaveAs("SR32_MIXSEC.pdf")
