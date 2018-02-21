import ROOT, array
import sys, glob
import os

ROOT.gROOT.SetBatch(1)

#### EDIT ME
# dir="MIlimits_12p9"
# dir="whatever2"
# dir="total"
# dir="whatever77"
# lumi=36.8
# dir="Mar6"
# dir="Mar7"
dir="Mar7_met300400" # <-- HT is good in this one
# dir="Mar7_filt"
# dir="Mar8_met"
lumi=35.9
#### END EDIT

met=[300,400,500,600,700,800,900,1000]
ht=[1125,1200,1300,1400,1500,1600,1700,1800,1900,2000]

# magic = 1.5
magic = 1.5
magic2 = 2.08
# magic2 = 2.70

doMET = True
doHT = True


get_cut = lambda x: int(x.split(".mH")[1].split(".")[0])

files = glob.glob(dir+"/higgsCombineTest.HybridNew.mH*")
# print files
met_files = filter(lambda x: int(x.split(".mH")[1].split(".")[0]) <= 1000+1, files)
ht_files = filter(lambda x: int(x.split(".mH")[1].split(".")[0]) >= 1125-1, files)

# print met_files

files_obs = sorted([f for f in met_files if len(f.split("/")[-1].split(".")) == 5], key=get_cut)
# print files_obs
files_exp = sorted([f for f in met_files if "quant0.500" in f], key=get_cut)
files_m2s = sorted([f for f in met_files if "quant0.025" in f], key=get_cut)
files_m1s = sorted([f for f in met_files if "quant0.160" in f], key=get_cut)
files_p1s = sorted([f for f in met_files if "quant0.840" in f], key=get_cut)
files_p2s = sorted([f for f in met_files if "quant0.975" in f], key=get_cut)

# print files_exp


if doMET:

    obs=[]
    exp=[]
    m2s=[]
    m1s=[]
    p1s=[]
    p2s=[]
    x0e=[]

    for fn in files_obs:
        f = ROOT.TFile.Open(fn)
        # print fn
        val = 0.
        try:
            for entry in f.limit:
                val = entry.limit/lumi
        except: pass
        obs.append(val)  
        x0e.append(0.)
            
    for fn in files_exp:
        f = ROOT.TFile.Open(fn)
        val = 0.
        try:
            for entry in f.limit:
                #print entry.limit/lumi
                val = entry.limit/lumi
        except: pass
        exp.append(val)  

    for fn in files_m2s:
        f = ROOT.TFile.Open(fn)
        val = 0.
        try:
            for entry in f.limit:
                #print entry.limit/lumi
                val = entry.limit/lumi
        except: pass
        m2s.append(val)  

    for fn in files_m1s:
        f = ROOT.TFile.Open(fn)
        val = 0.
        try:
            for entry in f.limit:
                #print entry.limit/lumi
                val = entry.limit/lumi
        except: pass
        m1s.append(val)  

    for fn in files_p1s:
        f = ROOT.TFile.Open(fn)
        val = 0.
        try:
            for entry in f.limit:
                #print entry.limit/lumi
                val = entry.limit/lumi
        except: pass
        p1s.append(val)  

    for fn in files_p2s:
        f = ROOT.TFile.Open(fn)
        val = 0.
        try:
            for entry in f.limit:
                #print entry.limit/lumi
                val = entry.limit/lumi
        except: pass
        p2s.append(val)  

    print p2s
    print p1s
    print exp
    print m1s
    print m2s
    print "obs:",obs

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

    h = ROOT.TH2F("h","h",8,250,1050,10,0,magic)
    h.SetTitle("")
    h.GetXaxis().SetTitle("E_{T}^{miss} (GeV)")
    h.GetYaxis().SetTitle("#sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} limit at 95% CL (fb)")
    h.GetXaxis().SetLabelSize(0.04)
    h.GetYaxis().SetLabelSize(0.04)
    h.GetXaxis().SetTitleSize(0.045)
    h.GetYaxis().SetTitleSize(0.045)
    h.GetXaxis().SetTitleOffset(1.0)
    h.GetYaxis().SetTitleOffset(1.5)

    xx = [">"+str(r) for r in range(300,1100,100)]
    for ib in range(1,h.GetNbinsX()+1):
        print ib-1,xx[ib-1]
        h.GetXaxis().SetBinLabel(ib,xx[ib-1])
    h.GetXaxis().SetLabelSize(0.045)
    h.Draw()
    first=350
    dx=100

    crappyLines = []
    for i in range(h.GetNbinsX()):
        crappyLines.append ( ROOT.TLine(first+i*dx,0,first+i*dx,0.025) )
        crappyLines[-1].Draw()


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


    l1 = ROOT.TLegend(0.15, 0.70, 0.95, 0.90)
    # l1 = ROOT.TLegend(0.15, 0.65, 0.95, 0.90)
    l1.SetTextFont(42)
    l1.SetTextSize(0.036)
    l1.SetShadowColor(ROOT.kWhite)
    l1.SetFillColor(ROOT.kWhite)
    l1.SetHeader("Model-independent #sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} exclusion limit")
    # l1.AddEntry(None,"","")
    # l1.AddEntry(None,"","")
    l1.AddEntry(gobs , "Observed", "lp")
    l1.AddEntry(gexp , "Expected #pm 1 and #pm 2 #sigma_{experiment}", "l")
    l1.Draw()

    # lheader = ROOT.TLatex(0.16,0.91-2*0.058, "Model-independent #sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} exclusion limit" )
    # lheader.SetNDC()
    # lheader.SetTextSize(0.036)
    # lheader.SetTextFont(42)
    # lheader.Draw()

    LExp1 = ROOT.TGraphAsymmErrors(2)
    LExp1.SetFillColor(ROOT.kYellow)
    LExp1.SetPoint(0,250+ 3.8*(1050-250)/100, magic-magic2*(magic-0)/100*10)
    LExp1.SetPointError(0,0.,0.,0.15*magic/5,0.15*magic/5)
    LExp1.SetPoint(1,250+21.2*(1050-250)/100, magic-magic2*(magic-0)/100*10)
    LExp1.SetPointError(1,0.,0.,0.15*magic/5,0.15*magic/5)
    LExp1.Draw("3")

    LExp2 = ROOT.TGraphAsymmErrors(2)
    LExp2.SetFillColor(ROOT.kGreen)
    LExp2.SetPoint(0,250+ 3.8*(1050-250)/100, magic-magic2*(magic-0)/100*10)
    LExp2.SetPointError(0,0.,0.,0.08*magic/5,0.08*magic/5)
    LExp2.SetPoint(1,250+21.2*(1050-250)/100, magic-magic2*(magic-0)/100*10)
    LExp2.SetPointError(1,0.,0.,0.08*magic/5,0.08*magic/5)
    LExp2.Draw("L3")

    LExp = ROOT.TGraph(2)
    LExp.SetLineColor(ROOT.kBlack)
    LExp.SetLineStyle(7)
    LExp.SetLineWidth(3)
    LExp.SetPoint(0,250+ 3.8*(1050-250)/100, magic-magic2*(magic-0)/100*10)
    LExp.SetPoint(1,250+21.2*(1050-250)/100, magic-magic2*(magic-0)/100*10)
    LExp.Draw("L")

    # offset = 0.058
    offset = 0.00
    # masstex = ROOT.TLatex(0.70,0.79, "H_{T} > 300 GeV" )
    masstex = ROOT.TLatex(0.70,0.79-offset, "H_{T} > 300 GeV" )
    masstex.SetNDC()
    masstex.SetTextSize(0.036)
    masstex.SetLineWidth(2)
    masstex.SetTextFont(42)
    masstex.Draw()

    cmstex = ROOT.TLatex(0.670,0.91-offset, "%.1f fb^{-1} (13 TeV)" % lumi)
    cmstex.SetNDC()
    cmstex.SetTextSize(0.04)
    cmstex.SetLineWidth(2)
    cmstex.SetTextFont(42)
    cmstex.Draw()

    cmstexbold = ROOT.TLatex(0.16,0.91-offset, "CMS" )
    cmstexbold.SetNDC()
    cmstexbold.SetTextSize(0.05)
    cmstexbold.SetLineWidth(2)
    cmstexbold.SetTextFont(61)
    cmstexbold.Draw()

    cmstexprel = ROOT.TLatex(0.28,0.91-offset, "" )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.04)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstexprel.Draw()

    stupidtex = ROOT.TLatex(0.19,0.59, "(a)")
    stupidtex.SetNDC()
    stupidtex.SetTextSize(0.045)
    stupidtex.SetLineWidth(2)
    stupidtex.SetTextFont(42)
    stupidtex.Draw()

    fname = "MET_MIXSEC_%s.pdf" % (str(lumi).replace(".","p"))
    c1.SaveAs(fname)
    c1.SaveAs(fname.replace(".pdf",".C"))
    os.system("web " + fname)
    os.system("ic " + fname)

    fout = ROOT.TFile("SS2017_%s_%s.root" % ("MIMET", str(lumi)),"RECREATE")

    h_write = h.Clone("hwrite")
    h_write.Write()

    gr_s2b_write = gr_s2b.Clone("s2b")
    gr_s2b_write.Write()
    
    gr_s1b_write = gr_s1b.Clone("s1b")
    gr_s1b_write.Write()

    gobs_write = gobs.Clone("lobs")
    gobs_write.Write()

    gexp_write = gexp.Clone("lexp")
    gexp_write.Write()

    fout.Close()

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
            try:
                print fn
                for entry in f.limit:
                    #print entry.limit/lumi
                    obs.append(entry.limit/lumi)  
                    x0e.append(0.)
            except:
                obs.append(0.0)
                x0e.append(0.)

    for fn in files_exp:
            f = ROOT.TFile.Open(fn)
            try:
                for entry in f.limit: exp.append(entry.limit/lumi)  
            except: exp.append(0.0)  

    for fn in files_m2s:
            f = ROOT.TFile.Open(fn)
            try:
                for entry in f.limit: m2s.append(entry.limit/lumi)  
            except: m2s.append(0.0)  

    for fn in files_m1s:
            f = ROOT.TFile.Open(fn)
            try:
                for entry in f.limit: m1s.append(entry.limit/lumi)  
            except: m1s.append(0.0)  

    for fn in files_p1s:
            f = ROOT.TFile.Open(fn)
            try:
                for entry in f.limit: p1s.append(entry.limit/lumi)  
            except: p1s.append(0.0)  

    for fn in files_p2s:
            f = ROOT.TFile.Open(fn)
            try:
                for entry in f.limit: p2s.append(entry.limit/lumi)  
            except: p2s.append(0.0)  

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

    h = ROOT.TH2F("h2","h2",10,1050,2050,10,0,magic)
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
    h.GetXaxis().SetTickLength(0.00);

    # for ib in range(1,h.GetNbinsX()):
    #     print ib, h.GetXaxis().GetBinLabel(ib)
    #     h.GetXaxis().SetBinLabel( ib, "#geq"+h.GetXaxis().GetBinLabel(ib) )

    ht=[1125,1200,1300,1400,1500,1600,1700,1800,1900,2000]
    xx = [">"+str(r) for r in ht]
    for ib in range(1,h.GetNbinsX()+1):
        print ib-1,xx[ib-1]
        h.GetXaxis().SetBinLabel(ib,xx[ib-1])
    h.GetXaxis().SetLabelSize(0.040)
    h.Draw()
    first=1125
    dx=100

    # dxs = zip(ht[:-1],ht[1:])
    # print dxs
    crappyLines = []
    # for i in range(h.GetNbinsX()):
    for htv in ht:
        # if i == 2: dx = 75
        # else: dx = 100
        # crappyLines.append ( ROOT.TLine(first+i*dx,0,first+i*dx,0.025) )
        crappyLines.append ( ROOT.TLine(htv,0,htv,0.040) )
        crappyLines[-1].Draw()

    print ht

    gr_s2b = ROOT.TGraphAsymmErrors(len(ht),array.array('d', ht),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', m2s),array.array('d', p2s))
    gr_s2b.SetFillColor(ROOT.kYellow)

    # binNumber= gr_s2b.GetXaxis().FindBin(1125);
    # gr_s2b.GetXaxis().SetBinLabel(binNumber,"myLabel1");

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

    l1 = ROOT.TLegend(0.15, 0.70, 0.95, 0.90)
    # l1 = ROOT.TLegend(0.15, 0.65, 0.95, 0.90)
    l1.SetTextFont(42)
    l1.SetTextSize(0.036)
    l1.SetShadowColor(ROOT.kWhite)
    l1.SetFillColor(ROOT.kWhite)
    l1.SetHeader("Model-independent #sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} exclusion limit")
    # l1.AddEntry(None,"","")
    # l1.AddEntry(None,"","")
    l1.AddEntry(gobs , "Observed", "lp")
    l1.AddEntry(gexp , "Expected #pm 1 and #pm 2 #sigma_{experiment}", "l")
    l1.Draw()

    # lheader = ROOT.TLatex(0.16,0.91-2*0.058, "Model-independent #sigma#kern[0.1]{A}#kern[0.1]{#varepsilon} exclusion limit" )
    # lheader.SetNDC()
    # lheader.SetTextSize(0.036)
    # lheader.SetTextFont(42)
    # lheader.Draw()



    LExp1 = ROOT.TGraphAsymmErrors(2)
    LExp1.SetFillColor(ROOT.kYellow)
    LExp1.SetPoint(0,1050+ 3.8*(2050-1050)/100, magic-magic2*(magic-0)/100*10)
    LExp1.SetPointError(0,0.,0.,0.15*magic/5,0.15*magic/5)
    LExp1.SetPoint(1,1050+21.2*(2050-1050)/100, magic-magic2*(magic-0)/100*10)
    LExp1.SetPointError(1,0.,0.,0.15*magic/5,0.15*magic/5)
    LExp1.Draw("3")

    LExp2 = ROOT.TGraphAsymmErrors(2)
    LExp2.SetFillColor(ROOT.kGreen)
    LExp2.SetPoint(0,1050+ 3.8*(2050-1050)/100, magic-magic2*(magic-0)/100*10)
    LExp2.SetPointError(0,0.,0.,0.08*magic/5,0.08*magic/5)
    LExp2.SetPoint(1,1050+21.2*(2050-1050)/100, magic-magic2*(magic-0)/100*10)
    LExp2.SetPointError(1,0.,0.,0.08*magic/5,0.08*magic/5)
    LExp2.Draw("L3")

    LExp = ROOT.TGraph(2)
    LExp.SetLineColor(ROOT.kBlack)
    LExp.SetLineStyle(7)
    LExp.SetLineWidth(3)
    LExp.SetPoint(0,1050+ 3.8*(2050-1050)/100, magic-magic2*(magic-0)/100*10)
    LExp.SetPoint(1,1050+21.2*(2050-1050)/100, magic-magic2*(magic-0)/100*10)
    LExp.Draw("L")

    # offset = 0.058
    offset = 0.0
    # masstex = ROOT.TLatex(0.62,0.79, "50 < E_{T}^{miss} < 300 GeV" )
    masstex = ROOT.TLatex(0.62,0.79-offset, "50 < E_{T}^{miss} < 300 GeV" )
    masstex.SetNDC()
    masstex.SetTextSize(0.036)
    masstex.SetLineWidth(2)
    masstex.SetTextFont(42)
    masstex.Draw()

    cmstex = ROOT.TLatex(0.670,0.91-offset, "%.1f fb^{-1} (13 TeV)" % lumi)
    cmstex.SetNDC()
    cmstex.SetTextSize(0.04)
    cmstex.SetLineWidth(2)
    cmstex.SetTextFont(42)
    cmstex.Draw()

    cmstexbold = ROOT.TLatex(0.16,0.91-offset, "CMS" )
    cmstexbold.SetNDC()
    cmstexbold.SetTextSize(0.05)
    cmstexbold.SetLineWidth(2)
    cmstexbold.SetTextFont(61)
    cmstexbold.Draw()

    cmstexprel = ROOT.TLatex(0.28,0.91-offset, " " )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.04)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstexprel.Draw()

    stupidtex = ROOT.TLatex(0.19,0.59, "(b)")
    stupidtex.SetNDC()
    stupidtex.SetTextSize(0.045)
    stupidtex.SetLineWidth(2)
    stupidtex.SetTextFont(42)
    stupidtex.Draw()

    fname = "HT_MIXSEC_%s.pdf" % (str(lumi).replace(".","p"))
    c2.SaveAs(fname)
    c2.SaveAs(fname.replace(".pdf",".C"))
    os.system("ic " + fname)
    os.system("web " + fname)

    fout = ROOT.TFile("SS2017_%s_%s.root" % ("MIHT", str(lumi)),"RECREATE")

    h_write = h.Clone("hwrite")
    h_write.Write()

    gr_s2b_write = gr_s2b.Clone("s2b")
    gr_s2b_write.Write()
    
    gr_s1b_write = gr_s1b.Clone("s1b")
    gr_s1b_write.Write()

    gobs_write = gobs.Clone("lobs")
    gobs_write.Write()

    gexp_write = gexp.Clone("lexp")
    gexp_write.Write()

    fout.Close()
