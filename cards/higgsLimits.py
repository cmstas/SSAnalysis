import os, ROOT, array, re
from multiprocessing.dummy import Pool as ThreadPool 
import time,random
have_tqdm = False
try:
    from tqdm import tqdm
    have_tqdm = True
except:
    print "You don't have the tqdm python module, but it's ok."
    print "If you want a pretty progress bar, put the following file in your path: /home/users/namin/syncfiles/pyfiles/tqdm.py"

d_xsec = {
        "tth": {
         350: 0.001*24.2,
         370: 0.001*20.1,
         390: 0.001*17.8,
         410: 0.001*15.8,
         430: 0.001*14.9,
         450: 0.001*11.9,
         470: 0.001*11.2,
         490: 0.001*9.87,
         510: 0.001*9.75,
         530: 0.001*7.78,
         550: 0.001*7.49,
         600: 0.001*7.49,
         650: 1.0,
         700: 1.0,
         750: 1.0,
         800: 1.0,
         850: 1.0,
         900: 1.0,
            },
        "thw": {
            350: 0.001*8.37,
            370: 0.001*7.61,
            390: 0.001*7.07,
            410: 0.001*6.58,
            430: 0.001*5.72,
            450: 0.001*5.16,
            470: 0.001*4.81,
            490: 0.001*4.34,
            510: 0.001*4.19,
            530: 0.001*3.86,
            550: 0.001*3.41,
            },
        "thq": {
            350: 0.001*15.8,
            370: 0.001*13.5,
            390: 0.001*11.7,
            410: 0.001*10.2,
            430: 0.001*8.82,
            450: 0.001*7.73,
            470: 0.001*6.78,
            490: 0.001*5.93,
            510: 0.001*5.23,
            530: 0.001*4.59,
            550: 0.001*4.11,
            },
        "tta": {
            350: 0.001*33.7,
            370: 0.001*28.6,
            390: 0.001*25.3,
            410: 0.001*22.0,
            430: 0.001*19.9,
            450: 0.001*17.7,
            470: 0.001*15.1,
            490: 0.001*14.9,
            510: 0.001*12.6,
            530: 0.001*11.5,
            550: 0.001*10.0,
            600: 0.001*10.0,
         650: 1.0,
         700: 1.0,
         750: 1.0,
         800: 1.0,
         850: 1.0,
         900: 1.0,
            },
        "taw": {
                350: 0.001*7.97,
                370: 0.001*7.09,
                390: 0.001*6.88,
                410: 0.001*5.92,
                430: 0.001*5.39,
                450: 0.001*4.97,
                470: 0.001*4.59,
                490: 0.001*4.27,
                510: 0.001*3.87,
                530: 0.001*3.82,
                550: 0.001*3.26,
            },
        "taq": {
                350: 0.001*9.28,
                370: 0.001*8.14,
                390: 0.001*7.19,
                410: 0.001*6.43,
                430: 0.001*5.72,
                450: 0.001*5.07,
                470: 0.001*4.55,
                490: 0.001*4.09,
                510: 0.001*3.67,
                530: 0.001*3.27,
                550: 0.001*2.94,
            }
        }




d_xsec["combined"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tth","thq","thw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tth","thq","thw"]],[])) }
d_xsec["combined_ps"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tta","taq","taw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tta","taq","taw"]],[])) }
d_xsec["higgs_scan"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tth","thq","thw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tth","thq","thw"]],[])) }
d_xsec["higgs_ps_scan"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tta","taq","taw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tta","taq","taw"]],[])) }
# print d_xsec
# print d_xsec.keys()
# print " s",sorted(d_xsec["combined"].items())
# print "ps",sorted(d_xsec["combined_ps"].items())
# d_xsec["combined_ps"] = {}
masses = set(sum( [vs.keys() for vs in d_xsec.values()],[]))
# for mass in masses:
#     val = 0.0
#     for proc in ["tth","thw","thq"]:
#         val += d_xsec[proc].get(mass,0.0)*d_xsec_scale[proc].get(mass,1.0)
#     d_xsec["combined_ps"][mass] = val

def make_plot(d):

    if "combined" in d["sig"]:
        typ = "combined"
    elif "higgs_ps" in d["sig"]:
        typ = "combined_ps"
    elif "higgs" in d["sig"]:
        typ = "combined"
    else:
        typ = d["sig"].split("_")[0]

    def run_sig(sig):

        print sig

        time.sleep(60.0*random.random()) 


        if d["verbose"]:
            print sig

        cardname = d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.txt"
        cardname_nodir = "card_"+sig+"_"+d["mylumi"]+"ifb-all.txt"

        if os.path.isfile(d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.log"):
            print "Log already exists, skipping"
            return

        if "combined" in sig:
            if "_ps" in typ:
                # print "HERE"
                # print cardname_nodir
                # card_higgs_ps_scan_m470_35.9ifb-all.txt
                tth_card = cardname_nodir.replace("higgs_ps_scan", "tta_scan")
                thw_card = cardname_nodir.replace("higgs_ps_scan", "taw_scan")
                thq_card = cardname_nodir.replace("higgs_ps_scan", "taq_scan")
                # print cardname_nodir
            else:
                tth_card = cardname_nodir.replace("higgs_scan", "tth_scan")
                thw_card = cardname_nodir.replace("higgs_scan", "thw_scan")
                thq_card = cardname_nodir.replace("higgs_scan", "thq_scan")

            # print "-->",tth_card, thw_card, thq_card
            # --> card_higgs_ps_scan_m550_35.9ifb-all.txt card_higgs_ps_scan_m550_35.9ifb-all.txt card_higgs_ps_scan_m550_35.9ifb-all.txt

            cmd = "(cd %s ; combineCards.py %s %s %s > %s)" % (d["mydir"], tth_card, thw_card, thq_card, cardname_nodir)
            print cmd
            os.system(cmd)
        else:
            cmd = "python createCard.py "+d["mydir"]+" "+sig
            print cmd
            os.system(cmd)
        cmd = "combine -M Asymptotic "+cardname+" >& "+d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.log"
        print cmd
        os.system(cmd) # --run expected --noFitAsimov


    sigs  = []
    if "combined" in d["sig"]:
        for mass in range(350, 570, 20):
            sigs.append("%s_m%i" % (d["sig"],mass))
    else:
        # for mass in range(350, 910, 10):
        for mass in range(350, 570, 20):
            fname = "%s/%s_m%i_histos_hihi_%sifb.root" % (d["mydir"],d["sig"],mass,d["mylumi"])
            print fname
            if os.path.isfile(fname):
                sigs.append("%s_m%i" % (d["sig"],mass))

    # print sigs

    if d["redolimits"]:
        os.nice(10)
        pool = ThreadPool(11)
        # pool = ThreadPool(1)
        vals = []
        if have_tqdm:
            for result in tqdm(pool.imap_unordered(run_sig, sigs), total=len(sigs)):
                vals.append(result)
        else:
            for result in pool.imap_unordered(run_sig, sigs):
                vals.append(result)

        pool.close()
        pool.join()

    new_sigs = []
    lims = []
    for sig in sigs:
        foundObs = False
        fname = d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.log"
        mass = int(sig.split("_m")[-1])
        if not os.path.isfile(fname): continue
        with open(fname) as f:
            obs, exp, sm1, sp1, sm2, sp2 = 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
            foundExp = False
            for line in f:
                if "Observed Limit" in line:
                    val = line.split()[4]
                    obs = float(val)
                    foundObs = True
                if "Expected 50" in line:
                    val = line.split()[4]
                    exp = float(val)
                    foundExp = True
                if "Expected 16" in line:
                    val = line.split()[4]
                    sm1 = float(val)
                if "Expected 84" in line:
                    val = line.split()[4]
                    sp1 = float(val)
                if "Expected  2.5" in line:
                    val = line.split()[4]
                    sm2 = float(val)
                if "Expected 97" in line:
                    val = line.split()[4]
                    sp2 = float(val)
        if not foundObs or not foundExp:
            print "obs not found for", sig
        else:
            print typ, int(mass), sig
            lims.append({
                "sig":  sig,
                "mass":mass,
                "exp":  exp*1000.0*d_xsec[typ][int(mass)],
                "obs":  obs*1000.0*d_xsec[typ][int(mass)],
                "sm1":  sm1*1000.0*d_xsec[typ][int(mass)],
                "sm2":  sm2*1000.0*d_xsec[typ][int(mass)],
                "sp1":  sp1*1000.0*d_xsec[typ][int(mass)],
                "sp2":  sp2*1000.0*d_xsec[typ][int(mass)],
                })

    lims = sorted(lims, key=lambda x: x.get("mass",9999))
    # print lims

    lumi = float(d["mylumi"])
    if d["overridelumi"]:
        lumi = float(d["overridelumi"])


    scalelumi = 1.0
    if d["scaletolumi"]:
        scalelumi = float(d["scaletolumi"])/lumi

    # xmin,xmax = 250,1050
    # ymax = 30

    xmin = d["xmin"]
    xmax = d["xmax"]
    ymax = d["ymax"]

    obs = [lim["obs"]/(scalelumi**0.5) for lim in lims]
    exp = [lim["exp"]/(scalelumi**0.5) for lim in lims]
    sm2 = [lim["sm2"]/(scalelumi**0.5) for lim in lims]
    sm1 = [lim["sm1"]/(scalelumi**0.5) for lim in lims]
    sp1 = [lim["sp1"]/(scalelumi**0.5) for lim in lims]
    sp2 = [lim["sp2"]/(scalelumi**0.5) for lim in lims]
    mhs = [lim["mass"] for lim in lims]
    x0e = [0. for lim in lims]

    theory = [1000.0*d_xsec[typ][int(mass)]/(scalelumi**0.5) for mass in mhs]
    # print theory
    if "combined" in typ:
        theory_ps = [1000.0*d_xsec[typ][int(mass)]/(scalelumi**0.5) for mass in mhs]
        # print theory_ps

    # print mhs
    # print obs
    # print exp

    #TGraphAsymmErrors needs the delta
    for i in range(0,len(exp)):
        sm2[i]=exp[i]-sm2[i]
        sm1[i]=exp[i]-sm1[i]
        sp1[i]=sp1[i]-exp[i]
        sp2[i]=sp2[i]-exp[i]

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

    h = ROOT.TH2F("h","h",16,xmin,xmax,ymax,0,ymax)
    h.SetTitle("")
    # h.GetXaxis().SetTitle("m_{H/A} [GeV]")
    h.GetXaxis().SetTitle(d.get("xaxistitle","m_{H/A} [GeV]"))
    # h.GetYaxis().SetTitle("N^{UL}/N^{exp} limit at 95% CL")
    h.GetYaxis().SetTitle(d["yaxistitle"])
    h.GetXaxis().SetLabelSize(0.04)
    h.GetYaxis().SetLabelSize(0.04)
    h.GetXaxis().SetTitleSize(0.045)
    h.GetYaxis().SetTitleSize(0.045)
    h.GetXaxis().SetTitleOffset(1.0)
    h.GetYaxis().SetTitleOffset(1.5)
    h.Draw()

    gr_s2b = ROOT.TGraphAsymmErrors(len(mhs),array.array('d', mhs),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', sm2),array.array('d', sp2))
    gr_s2b.SetFillColor(ROOT.kYellow)
    gr_s2b.Draw("3")
      
    gr_s1b = ROOT.TGraphAsymmErrors(len(mhs),array.array('d', mhs),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', sm1),array.array('d', sp1))
    gr_s1b.SetFillColor(ROOT.kGreen)
    gr_s1b.Draw("3")

    gexp = ROOT.TGraph(len(mhs), array.array('d', mhs), array.array('d', exp))
    gexp.SetLineStyle(7)
    gexp.SetLineWidth(3)
    gexp.SetLineColor(ROOT.kBlack)
    gexp.Draw("L")

    gtheory = ROOT.TGraph(len(mhs), array.array('d', mhs), array.array('d', theory))
    # gexp.SetLineStyle(7)
    gtheory.SetLineWidth(4)
    gtheory.SetLineColor(ROOT.kRed)
    gtheory.Draw("L")

    if "combined" in typ:
        if not d.get("hidepseudo",False):
            gtheory2 = ROOT.TGraph(len(mhs), array.array('d', mhs), array.array('d', theory_ps))
            gtheory2.SetLineStyle(7)
            gtheory2.SetLineWidth(4)
            gtheory2.SetLineColor(ROOT.kRed)
            gtheory2.Draw("L")

    if d["showobserved"]:
        gobs = ROOT.TGraph(len(mhs), array.array('d', mhs), array.array('d', obs))
    else:
        gobs = ROOT.TGraph()
    gobs.SetMarkerStyle(ROOT.kFullCircle)
    gobs.SetMarkerSize(1.5)
    gobs.SetMarkerColor(ROOT.kBlack)
    gobs.SetLineWidth(3)
    gobs.SetLineColor(ROOT.kBlack)
    if d["showobserved"]: gobs.Draw("PL")

    if d["nolegendheader"]:
        # l1 = ROOT.TLegend(0.15, 0.75, 0.95, 0.90)
        l1 = ROOT.TLegend(0.15, 0.70, 0.95, 0.90)
        l1.AddEntry(None , "", "")
        l1.AddEntry(None , "", "")
    else:
        l1 = ROOT.TLegend(0.15, 0.70, 0.95, 0.90)
    l1.SetTextFont(42)
    l1.SetTextSize(0.036)
    l1.SetNColumns(2)
    l1.SetShadowColor(ROOT.kWhite)
    l1.SetFillColor(ROOT.kWhite)
    if not d["nolegendheader"]:
        l1.SetHeader(d["legendheader"])
    l1.AddEntry(gobs , "95% CL Observed", "lp")
    if "_ps_" in d["sig"]:
        l1.AddEntry(gtheory , "#sigma_{theory}^{pseudoscalar}", "l")
    else:
        l1.AddEntry(gtheory , "#sigma_{theory}^{scalar}", "l")
    l1.AddEntry(gexp , "95% CL Expected #pm 1 #sigma #pm 2 #sigma", "l")
    if "combined" in typ:
        if not d.get("hidepseudo",False):
            l1.AddEntry(gtheory2 , "#sigma_{theory}^{pseudoscalar}", "l")
    l1.Draw()
    
    magic1 = 1.8
    if d["nolegendheader"]:
        magic2 = 1.38
    else:
        magic2 = 2.08
    magic3 = 0.15
    magic4 = 0.08
    magic5 = 11.2
    
    LExp1 = ROOT.TGraphAsymmErrors(2)
    LExp1.SetFillColor(ROOT.kYellow)
    LExp1.SetPoint(0,xmin+ magic1*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp1.SetPointError(0,0.,0.,magic3*ymax/5,magic3*ymax/5)
    LExp1.SetPoint(1,xmin+magic5*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp1.SetPointError(1,0.,0.,magic3*ymax/5,magic3*ymax/5)
    LExp1.Draw("3")

    LExp2 = ROOT.TGraphAsymmErrors(2)
    LExp2.SetFillColor(ROOT.kGreen)
    LExp2.SetPoint(0,xmin+ magic1*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp2.SetPointError(0,0.,0.,magic4*ymax/5,magic4*ymax/5)
    LExp2.SetPoint(1,xmin+magic5*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp2.SetPointError(1,0.,0.,magic4*ymax/5,magic4*ymax/5)
    LExp2.Draw("L3")

    LExp = ROOT.TGraph(2)
    LExp.SetLineColor(ROOT.kBlack)
    LExp.SetLineStyle(7)
    LExp.SetLineWidth(3)
    LExp.SetPoint(0,xmin+ magic1*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp.SetPoint(1,xmin+magic5*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp.Draw("L")

    offset = 0.058
    cmstex = ROOT.TLatex(0.670,0.91-offset, "%.1f fb^{-1} (13 TeV)" % (lumi*scalelumi))
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

    cmstexprel = ROOT.TLatex(0.28,0.91-offset, "Preliminary" )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.04)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstexprel.Draw()


    # masstex = ROOT.TLatex(0.60,0.79, "pp#rightarrow t#bar{t}H, H #rightarrow t#bar{t}" )
    # masstex.SetNDC()
    # masstex.SetTextSize(0.036)
    # masstex.SetLineWidth(2)
    # masstex.SetTextFont(42)
    # masstex.Draw()
    
    # exclLine = ROOT.TLine()
    # exclLine.SetLineColor(ROOT.kRed)
    # exclLine.DrawLine(xmin,1.0,xmax,1.0)

    fname_out = "%s_limit%s.pdf" % (d["sig"],d["postfix"])
    c1.SaveAs(fname_out)
    # os.system("web %s" % fname_out)
    os.system("ic %s" % fname_out)

if __name__ == "__main__":

    tth = {
            "sig": "tth_scan",
            "redolimits": False,
            "mydir": "v9.06_Mar6_35p9_reminiaod_higgs",
            "legendheader": "pp#rightarrow t#bar{t}H, BR(H #rightarrow t#bar{t})=1: exclusion limit",
            "nolegendheader": False,
            "mylumi": "17.3",
            "verbose": False,
            "scaletolumi": None,
            "showobserved": True,
            "postfix": "",
            # "xmin": 250,
            # "xmax": 1050,
            # "ymax": 200,
            "xmin": 300,
            "xmax": 550,
            "ymax": 200,
            "overridelumi": None,
            "yaxistitle": "#sigma^{UL} limit at 95% CL [fb]",
            }

    # tth["mylumi"] = "36.5"
    # tth["mydir"] = "v8.07_Dec14_36p5_nobtagsf_higgst1tttt"
    # tth["mylumi"] = "36.8"
    # tth["mydir"] = "v9.03_Jan26_36p8_nobtagsf_higgs"
    # tth["mydir"] = "v8.07_Dec14_36p5_nobtagsf_higgst1tttt_redo_test"

    ### SCALAR

    tth["mylumi"] = "35.9"
    tth["redolimits"] = False


    thw = tth.copy()
    thw["sig"] = "thw_scan"
    thw["legendheader"] = "pp#rightarrow tHW, H #rightarrow t#bar{t}: exclusion limit"
    thw["redolimits"] = False

    thq = tth.copy()
    thq["sig"] = "thq_scan"
    thq["legendheader"] = "pp#rightarrow tHq, H #rightarrow t#bar{t}: exclusion limit"
    thq["redolimits"] = False

    ### PSEUDOSCALAR

    tta = tth.copy()
    tta["sig"] = "tta_scan"
    tta["legendheader"] = "pp#rightarrow t#bar{t}A, A #rightarrow t#bar{t}: exclusion limit"
    tta["redolimits"] = False

    taw = tth.copy()
    taw["sig"] = "taw_scan"
    taw["legendheader"] = "pp#rightarrow tAW, A #rightarrow t#bar{t}: exclusion limit"
    taw["redolimits"] = False

    taq = tth.copy()
    taq["sig"] = "taq_scan"
    taq["legendheader"] = "pp#rightarrow tAq, A #rightarrow t#bar{t}: exclusion limit"
    taq["redolimits"] = False

    higgs = tth.copy()
    higgs["sig"] = "higgs_scan"
    higgs["mydir"] = "v9.06_Mar6_35p9_reminiaodfilt_nomhiggs"
    higgs["xmin"] = 300
    higgs["xmax"] = 550
    higgs["ymax"] = 200
    higgs["legendheader"] = "pp#rightarrow (t#bar{t}H/A,tWH/A,tqH/A), BR(H/A #rightarrow t#bar{t})=1: exclusion"
    higgs["nolegendheader"] = True
    # higgs["yaxistitle"] = "#sigma(pp#rightarrow (t#bar{t},tW,tq)+H/A) #times BR(H/A#rightarrow t#bar{t}) [fb]"
    higgs["yaxistitle"] = "#sigma(pp#rightarrow (t#bar{t},tW,tq)+H) #times BR(H#rightarrow t#bar{t}) [fb]"
    higgs["xaxistitle"] = "m_{H} [GeV]"
    higgs["hidepseudo"] = True
    higgs["redolimits"] = False

    higgs_ps = tth.copy()
    higgs_ps["sig"] = "higgs_ps_scan"
    higgs_ps["mydir"] = "v9.06_Mar6_35p9_reminiaodfilt_nomhiggs"
    higgs_ps["xmin"] = 300
    higgs_ps["xmax"] = 550
    higgs_ps["ymax"] = 200
    higgs_ps["legendheader"] = "pp#rightarrow (t#bar{t}H/A,tWH/A,tqH/A), BR(H/A #rightarrow t#bar{t})=1: exclusion"
    higgs_ps["nolegendheader"] = True
    # higgs_ps["yaxistitle"] = "#sigma(pp#rightarrow (t#bar{t},tW,tq)+H/A) #times BR(H/A#rightarrow t#bar{t}) [fb]"
    higgs_ps["yaxistitle"] = "#sigma(pp#rightarrow (t#bar{t},tW,tq)+A) #times BR(A#rightarrow t#bar{t}) [fb]"
    higgs_ps["xaxistitle"] = "m_{A} [GeV]"
    higgs_ps["hidepseudo"] = True
    higgs_ps["redolimits"] = False


    # tth["overridelumi"] = "36.8"
    # thw["overridelumi"] = "36.8"
    # thq["overridelumi"] = "36.8"

    # higgs["overridelumi"] = "36.8"

    # combined = tth.copy()
    # combined["sig"] = "higgs_scan_combined"
    # combined["legendheader"] = "pp#rightarrow (t#bar{t}H/A,tWH/A,tqH/A), H/A #rightarrow t#bar{t}: exclusion limit"
    # # combined["scaletolumi"] = 35.0
    # # combined["showobserved"] = False
    # combined["xmin"] = 300
    # combined["xmax"] = 600
    # combined["ymax"] = 200



    # make_plot(tth)
    # make_plot(thw)
    # make_plot(thq)

    # make_plot(tta)
    # make_plot(taw)
    # make_plot(taq)

    # make_plot(higgs)
    # make_plot(higgs_ps)

    # FIXME remove
    higgs["redolimits"] = False
    higgs["showobserved"] = False
    higgs["postfix"] = "_double"
    higgs["overridelumi"] = "71.8"
    higgs["mydir"] = "v9.06_Mar24_test_doublelumi_higgs"
    make_plot(higgs)
    higgs["postfix"] = "_triple"
    higgs["overridelumi"] = "107.7"
    higgs["mydir"] = "v9.06_Mar24_test_triplelumi_higgs"
    make_plot(higgs)
    higgs_ps["redolimits"] = False
    higgs_ps["showobserved"] = False
    higgs_ps["postfix"] = "_double"
    higgs_ps["overridelumi"] = "71.8"
    higgs_ps["mydir"] = "v9.06_Mar24_test_doublelumi_higgs"
    make_plot(higgs_ps)
    higgs_ps["postfix"] = "_triple"
    higgs_ps["overridelumi"] = "107.7"
    higgs_ps["mydir"] = "v9.06_Mar24_test_triplelumi_higgs"
    make_plot(higgs_ps)

    # make_plot(higgs)
    # make_plot(higgs_ps)
