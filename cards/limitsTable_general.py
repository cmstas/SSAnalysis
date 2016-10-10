import os, ROOT, array, re
from multiprocessing.dummy import Pool as ThreadPool 
have_tqdm = False
try:
    from tqdm import tqdm
    have_tqdm = True
except:
    print "You don't have the tqdm python module, but it's ok."
    print "If you want a pretty progress bar, put the following file in your path: /home/users/namin/syncfiles/pyfiles/tqdm.py"

def plot_limits(d):

    if d["doSignificance"]:
        minz = -2.0
        maxz =  2.0

    # make things less cluttered
    step = d["step"]
    minx = d["minx"]
    maxx = d["maxx"]
    miny = d["miny"]
    maxy = d["maxy"]
    minz = d["minz"]
    maxz = d["maxz"]
    maxyh = d["maxyh"]
    ybinsfirstxbin = d["ybinsfirstxbin"]

    npx = (maxx-minx)/step
    npy = (maxy-miny)/step
    npyh = (maxyh-miny)/step

    lumi_str = d["mylumi"].replace(".","p")+"ifb"

    def smooth(g,h):
        h_tmp = g.GetHistogram()
        print g, h, h_tmp
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
        # h.Smooth()
        # h.Smooth(1,"k5b")
        h.Smooth(1,"k5b")
        h.Smooth(1,"k5b")

        #now cleanup above the diagonal
        for xbin in range(1,h.GetNbinsX()+1):
            for ybin in range(1,h.GetNbinsY()+1):
                if ybin>xbin+ybinsfirstxbin: h.SetBinContent(xbin,ybin,-999 if d["doSignificance"] else 0)

    sigs = []

    for mglu in range (minx,maxx,step):
        for mlsp in range (miny,maxy,step):
            if mlsp == 0: mlsp = 1

            if os.path.isfile(("%s/fs_%s_m%i_m%i_histos_hihi_%sifb.root" % (d["mydir"],d["modeltag"],mglu,mlsp,d["mylumi"]))) is False: continue
            if d["verbose"]:
                print "found sig = fs_%s_m%i_m%i" % (d["modeltag"], mglu,mlsp)
            sigs.append(("fs_%s_m%i_m%i" % (d["modeltag"], mglu,mlsp)))

    limit_obs = []
    limit_exp = []
    limit_sm1 = []
    limit_sp1 = []
    limit_sm2 = []
    limit_sp2 = []

    miny = miny - step/2.
    maxy = maxy - step/2.
    minx = minx - step/2.
    maxx = maxx - step/2.

    def run_sig(sig):
        if d["verbose"]:
            print sig
        if not d["doSignificance"]: 
            if os.path.isfile(d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.log"): return
        else: 
            if os.path.isfile(d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all_significance.log"): return

        if d["redolimits"]: 
            os.system("python createCard.py "+d["mydir"]+" "+sig)
            os.system("combine -M Asymptotic "+d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.txt >& "+d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.log") # --run expected --noFitAsimov
        if d["redosignificances"]: os.system("combine -M ProfileLikelihood --uncapped 1 --significance --rMin -5 "+d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.txt >& "+d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all_significance.log") 

    os.nice(10)
    if d["redolimits"] or d["redosignificances"]:

        pool = ThreadPool(40)
        vals = []
        if have_tqdm:
            for result in tqdm(pool.imap_unordered(run_sig, sigs), total=len(sigs)):
                vals.append(result)
        else:
            for result in pool.imap_unordered(run_sig, sigs):
                vals.append(result)
        pool.close()
        pool.join()

        # pool = ThreadPool(40)
        # vals = pool.map(run_sig, sigs)
        # pool.close()
        # pool.join()

    new_sigs = []
    if d["doSignificance"]:
        for sig in sigs:
            foundObs = False
            if not os.path.isfile(d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all_significance.log"): continue
            with open(d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all_significance.log") as f:
                obs = 0.0 # actually this is just significance, but sig is already taken :(
                cputime = 0.0
                for line in f:
                    if "Significance" in line:
                        obs = float(line.split()[1])
                        foundObs = True
                    elif "Done in" in line:
                        cputime = float(line.split("min (cpu)")[0].split("Done in")[1].strip())

                if cputime > 0.20: foundObs = False
                # if obs <= -3.0: print sig, obs
                # if we want to look at cpu time:
                # obs = cputime
                obs = max(min(obs,maxz),minz)
            if not foundObs: print "significance not found for", sig
            else:
                new_sigs.append(sig)
                limit_obs.append(obs)
    else:
        for sig in sigs:
            foundObs = False
            with open(d["mydir"]+"/card_"+sig+"_"+d["mylumi"]+"ifb-all.log") as f:
                obs = 0.0
                exp = 0.0
                sm1 = 0.0
                sp1 = 0.0
                sm2 = 0.0
                sp2 = 0.0
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
            # print sig, foundObs
            if not foundObs or not foundExp:
                print "obs not found for", sig
                # sigs.remove(sig)
            else:
                new_sigs.append(sig)
                limit_exp.append(exp)
                limit_obs.append(obs)
                limit_sm1.append(sm1)
                limit_sp1.append(sp1)
                limit_sm2.append(sm2)
                limit_sp2.append(sp2)

    sigs = new_sigs[:]

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
    if not d["doSignificance"]: padt.SetLogz()

    fxsec = ROOT.TFile("xsec_susy_13tev.root")
    hxsec = fxsec.Get("h_xsec_gluino" if d["is_gluino"] else "h_xsec_stop")

    # h_xsec_test = ROOT.TH2D("h_xsec_test","",npx,minx,maxx,npy,miny,maxy)
    # h_sobs_test = ROOT.TH2D("h_sobs_test","",npx,minx,maxx,npy,miny,maxy)

    count = 0
    mglus = array.array('d')
    mlsps = array.array('d')
    v_xsec = array.array('d')
    v_sexp = array.array('d')
    v_ssm1 = array.array('d')
    v_ssp1 = array.array('d')
    v_ssm2 = array.array('d')
    v_ssp2 = array.array('d')
    v_sobs = array.array('d')
    v_som1 = array.array('d')
    v_sop1 = array.array('d')
    for sig in sigs:
        smod = sig.split("_m",1)[0]
        mglu, mlsp = map(int,re.findall(r"_m([0-9]+)", sig))
        if mlsp == 1: mlsp = 0
        mglus.append(mglu)
        mlsps.append(mlsp)

        if not d["doSignificance"]:
            v_xsec.append( limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
            v_sobs.append( limit_obs[count] )
            v_som1.append( limit_obs[count]*(hxsec.GetBinContent(hxsec.FindBin(mglu))-hxsec.GetBinError(hxsec.FindBin(mglu)))/hxsec.GetBinContent(hxsec.FindBin(mglu)) )
            v_sop1.append( limit_obs[count]*(hxsec.GetBinContent(hxsec.FindBin(mglu))+hxsec.GetBinError(hxsec.FindBin(mglu)))/hxsec.GetBinContent(hxsec.FindBin(mglu)) )
            v_sexp.append( limit_exp[count] )
            v_ssm1.append( limit_sm1[count] )
            v_ssp1.append( limit_sp1[count] )
            v_ssm2.append( limit_sm2[count] )
            v_ssp2.append( limit_sp2[count] )

            if d["verbose"]:
                print ("%-8s %4i %4i | %.1f | %.1f (-1s %.1f, +1s %.1f) | %.3f " % (smod , mglu, mlsp , limit_obs[count]  , limit_exp[count] , limit_sm1[count] , limit_sp1[count], hxsec.GetBinContent(hxsec.FindBin(mglu)) ) )
        else:
            v_xsec.append( limit_obs[count] )


        # h_xsec_test.SetBinContent(h_xsec_test.FindBin(mglu,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)))
        # h_sobs_test.SetBinContent(h_sobs_test.FindBin(mglu,mlsp), limit_obs[count])
        count = count+1

    # h_xsec_test.GetZaxis().SetRangeUser(1e-2,10e0)
    # h_xsec_test.Draw("colz")
    # print h_xsec_test.GetXaxis().GetBinLowEdge(1), h_xsec_test.GetXaxis().GetBinUpEdge(1)
    # c1.SaveAs("xsec_test.pdf")
    # os.system("web xsec_test.pdf")

    g_xsec = ROOT.TGraph2D("g_xsec","",count,mglus,mlsps,v_xsec)
    g_xsec.SetNpx(npx);
    g_xsec.SetNpy(npy);
    h_xsec = ROOT.TH2D("h_xsec","",npx,minx,maxx,npyh,miny,maxyh)
    smooth(g_xsec,h_xsec)

    if not d["doSignificance"]:
        g_sexp = ROOT.TGraph2D("g_sexp","",count,mglus,mlsps,v_sexp)
        g_ssm1 = ROOT.TGraph2D("g_ssm1","",count,mglus,mlsps,v_ssm1)
        g_ssp1 = ROOT.TGraph2D("g_ssp1","",count,mglus,mlsps,v_ssp1)
        g_ssm2 = ROOT.TGraph2D("g_ssm2","",count,mglus,mlsps,v_ssm2)
        g_ssp2 = ROOT.TGraph2D("g_ssp2","",count,mglus,mlsps,v_ssp2)
        g_sobs = ROOT.TGraph2D("g_sobs","",count,mglus,mlsps,v_sobs)
        g_som1 = ROOT.TGraph2D("g_som1","",count,mglus,mlsps,v_som1)
        g_sop1 = ROOT.TGraph2D("g_sop1","",count,mglus,mlsps,v_sop1)

        g_sexp.SetNpx(npx);
        g_sexp.SetNpy(npy);
        g_ssm1.SetNpx(npx);
        g_ssm1.SetNpy(npy);
        g_ssp1.SetNpx(npx);
        g_ssp1.SetNpy(npy);
        g_ssm2.SetNpx(npx);
        g_ssm2.SetNpy(npy);
        g_ssp2.SetNpx(npx);
        g_ssp2.SetNpy(npy);
        g_sobs.SetNpx(npx);
        g_sobs.SetNpy(npy);
        g_som1.SetNpx(npx);
        g_som1.SetNpy(npy);
        g_sop1.SetNpx(npx);
        g_sop1.SetNpy(npy);

        h_sexp = ROOT.TH2D("h_sexp","",npx,minx,maxx,npy,miny,maxy)
        smooth(g_sexp,h_sexp)
        h_ssm1 = ROOT.TH2D("h_ssm1","",npx,minx,maxx,npy,miny,maxy)
        smooth(g_ssm1,h_ssm1)
        h_ssp1 = ROOT.TH2D("h_ssp1","",npx,minx,maxx,npy,miny,maxy)
        smooth(g_ssp1,h_ssp1)
        h_ssm2 = ROOT.TH2D("h_ssm2","",npx,minx,maxx,npy,miny,maxy)
        smooth(g_ssm2,h_ssm2)
        h_ssp2 = ROOT.TH2D("h_ssp2","",npx,minx,maxx,npy,miny,maxy)
        smooth(g_ssp2,h_ssp2)
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
        k_ssm2 = ROOT.TGraph2D(h_ssm2)
        h_ssm2 = k_ssm2.GetHistogram()
        k_ssp2 = ROOT.TGraph2D(h_ssp2)
        h_ssp2 = k_ssp2.GetHistogram()
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
    h_xsec.GetXaxis().SetTitle(d["label_xaxis"])
    h_xsec.GetYaxis().SetTitle(d["label_yaxis"])
    h_xsec.GetZaxis().SetTitle("95% CL upper limit on cross section (pb)" if not d["doSignificance"] else "Significance [#sigma]")
    h_xsec.GetXaxis().SetTitleOffset(1.0)
    h_xsec.GetYaxis().SetTitleOffset(1.6)
    h_xsec.GetZaxis().SetTitleOffset(1.3)
    h_xsec.GetZaxis().SetRangeUser(minz,maxz)

    if not d["doSignificance"]: k_sexp.Draw()
    h_xsec.Draw("colz")

    if not d["doSignificance"]:
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

        # >>
        csm2list = k_ssm2.GetContourList(1.)
        max_points = -1
        for i in range(0,len(csm2list)):
            n_points = csm2list[i].GetN()
            if n_points > max_points:
                csm2 = csm2list[i]
                max_points = n_points
        csm2.SetLineWidth(2)
        csm2.SetLineStyle(3)
        csm2.SetLineColor(ROOT.kRed)
        csm2.Draw("L same");

        csp2list = k_ssp2.GetContourList(1.)
        max_points = -1
        for i in range(0,len(csp2list)):
            n_points = csp2list[i].GetN()
            if n_points > max_points:
                csp2 = csp2list[i]
                max_points = n_points
        csp2.SetLineWidth(2)
        csp2.SetLineStyle(3)
        csp2.SetLineColor(ROOT.kRed)
        csp2.Draw("L same");
        # <<

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

    diag = ROOT.TLine(*d["diag_points"])
    diag.SetLineWidth(1)
    diag.SetLineStyle(2)
    diag.Draw("same")

    diagtex = ROOT.TLatex(d["diag_labelpoints"][0],d["diag_labelpoints"][1], d["label_diag"] )
    diagtex.SetNDC()
    diagtex.SetTextSize(0.03)
    diagtex.SetTextAngle(d["diag_labelangle"])
    diagtex.SetLineWidth(2)
    diagtex.SetTextFont(42)
    diagtex.Draw()

    l1 = ROOT.TLegend(0.15, 0.70, 0.83, 0.90)
    l1.SetTextFont(42)
    l1.SetTextSize(0.036)
    l1.SetShadowColor(ROOT.kWhite)
    l1.SetFillColor(ROOT.kWhite)
    if not d["doSignificance"]:
        l1.SetHeader("%s  NLO+NLL exclusion" % d["label_process"])
        l1.AddEntry(cobs , "Observed #pm 1 #sigma_{theory}", "l")
        l1.AddEntry(cexp , "Expected #pm 1 and 2 #sigma_{experiment}", "l")
    else:
        l1.SetHeader("%s  NLO+NLL cross section" % d["label_process"])
        l1.AddEntry(ROOT.TH1F() , "", "")
        l1.AddEntry(ROOT.TH1F() , "", "")
        l1.Draw()

    cmstex = ROOT.TLatex(0.575,0.91, d["mylumi"]+" fb^{-1} (13 TeV)" )
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

    cmstexprel = ROOT.TLatex(0.28,0.91, "Preliminary" )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.04)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstexprel.Draw()

    if d["mass_label"]:
        masstex = ROOT.TLatex(0.18,0.67, d["mass_label"])
        masstex.SetNDC()
        masstex.SetTextSize(0.03)
        masstex.SetLineWidth(2)
        masstex.SetTextFont(42)
        masstex.Draw()

    if not d["doSignificance"]:

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

        # >>
        LExpP2 = ROOT.TGraph(2)
        LExpP2.SetName("LExpP")
        LExpP2.SetTitle("LExpP")
        LExpP2.SetLineColor(ROOT.kRed)
        LExpP2.SetLineStyle(3)
        LExpP2.SetLineWidth(2)
        LExpP2.SetMarkerStyle(20)
        LExpP2.SetPoint(0,minx+ 3.8*(maxx-minx)/100, maxyh-1.83*(maxyh-miny)/100*10)
        LExpP2.SetPoint(1,minx+21.2*(maxx-minx)/100, maxyh-1.83*(maxyh-miny)/100*10)
        LExpP2.Draw("LSAME")

        LExpM2 = ROOT.TGraph(2)
        LExpM2.SetName("LExpM")
        LExpM2.SetTitle("LExpM")
        LExpM2.SetLineColor(ROOT.kRed)
        LExpM2.SetLineStyle(3)
        LExpM2.SetLineWidth(2)
        LExpM2.SetMarkerStyle(20)
        LExpM2.SetPoint(0,minx+ 3.8*(maxx-minx)/100, maxyh-2.33*(maxyh-miny)/100*10)
        LExpM2.SetPoint(1,minx+21.2*(maxx-minx)/100, maxyh-2.33*(maxyh-miny)/100*10)
        LExpM2.Draw("LSAME")
        # <<

    c1.SaveAs("%s_scan_xsec_%s%s%s.pdf" % (d["modeltag"], lumi_str,d["postfix"], "_sig" if d["doSignificance"] else ""))

    if not d["doSignificance"]:

        h_sobs.GetXaxis().SetLabelSize(0.035)
        h_sobs.GetYaxis().SetLabelSize(0.035)
        h_sobs.GetXaxis().SetTitle(d["label_xaxis"])
        h_sobs.GetYaxis().SetTitle(d["label_yaxis"])
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
        # c1.SaveAs("sobs.pdf")

        h_sexp.GetXaxis().SetLabelSize(0.035)
        h_sexp.GetYaxis().SetLabelSize(0.035)
        h_sexp.GetXaxis().SetTitle(d["label_xaxis"])
        h_sexp.GetYaxis().SetTitle(d["label_yaxis"])
        h_sexp.GetZaxis().SetTitle("95% CL expected upper limit on signal strength")
        h_sexp.GetZaxis().SetRangeUser(0.01,10.0)
        h_sexp.GetZaxis().SetLabelSize(0.035)
        h_sexp.GetXaxis().SetTitleOffset(1.1)
        h_sexp.GetYaxis().SetTitleOffset(1.6)
        h_sexp.GetZaxis().SetTitleOffset(1.6)
        h_sexp.Draw("colz")
        cexp.Draw("samecont2");
        # c1.SaveAs("sexp.pdf")

        fout = ROOT.TFile("SS2016_%s_%s%s.root" % (d["modeltag"], lumi_str,d["postfix"]),"RECREATE")

        hxsecwrite = h_xsec.Clone("xsec")

        if d["modeltag"] == "t5qqqqvv_dm20":
            for xbin in range(1,hxsecwrite.GetNbinsX()+1):
                for ybin in range(1,hxsecwrite.GetNbinsY()+1):
                    if hxsecwrite.GetXaxis().GetBinUpEdge(xbin)>1700: hxsecwrite.SetBinContent(xbin,ybin,0)

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
        csm2write = csm2.Clone("ssexp_m2s")
        csm2write.Write()
        csp2write = csp2.Clone("ssexp_p2s")
        csp2write.Write()
        fout.Close()

    os.system("web %s_scan_xsec_%s%s%s.pdf" % (d["modeltag"], lumi_str,d["postfix"],"_sig" if d["doSignificance"] else ""))

if __name__ == "__main__":
    d_t1tttt = {
        "mydir": "v8.04_July28",
        "mylumi": "12.9",
        # "mydir": "v8.04_Oct1",
        # "mylumi": "17.3",
        "postfix": "",
        "redolimits": False,

        "doSignificance": False,
        "redosignificances": False,
        "verbose": False,
        "modeltag": "t1tttt",
        "is_gluino": True,
        "mass_label": "",
        "label_diag": "m_{#tilde{g}}-m_{#tilde{#chi}_{1}^{0}}: 2 #upoint (m_{W} + m_{b})",
        "label_xaxis": "m_{#tilde{g}} (GeV)",
        "label_yaxis": "m_{#tilde{#chi}_{1}^{0}} (GeV)",
        "label_process": "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow t#bar{t}#tilde{#chi}^{0}_{1}          ",
        "diag_points": [600,430,1700,1530],
        "diag_labelpoints": [0.18,0.365],
        "diag_labelangle": 42,
        "step": 25,
        "minx": 600,
        "maxx": 2000+25,
        "miny": 0,
        "maxy": 1800+25,
        "minz": 1e-3,
        "maxz": 2.0,
        "maxyh": 1800,
        "ybinsfirstxbin": 15,
    }

    d_t5qqqqvv = d_t1tttt.copy()
    d_t5qqqqvv["modeltag"] = "t5qqqqvv"
    d_t5qqqqvv["label_diag"] = "m_{#tilde{g}} = m_{#tilde{#chi}_{1}^{0}}"
    d_t5qqqqvv["label_xaxis"] = "m_{#tilde{g}} (GeV)"
    d_t5qqqqvv["label_yaxis"] = "m_{#tilde{#chi}_{1}^{0}} (GeV)"
    d_t5qqqqvv["label_process"] = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow q#bar{q}'W#tilde{#chi}^{0}_{1}      "
    d_t5qqqqvv["mass_label"] = "m_{#tilde{#chi}^{#pm}_{1}} = 0.5(m_{#tilde{g}}+m_{#tilde{#chi}^{0}_{1}})"
    d_t5qqqqvv["diag_points"] = [600,600,1600,1600]
    d_t5qqqqvv["diag_labelpoints"] = [0.2,0.445]
    d_t5qqqqvv["diag_labelangle"] = 42
    d_t5qqqqvv["step"] = 25
    d_t5qqqqvv["minx"] = 600
    d_t5qqqqvv["maxx"] = 2000+25
    d_t5qqqqvv["miny"] = 0
    d_t5qqqqvv["maxy"] = 1800+25
    d_t5qqqqvv["maxyh"] = 1800
    d_t5qqqqvv["ybinsfirstxbin"] = 19

    d_t5qqqqvv_dm20 = d_t5qqqqvv.copy()
    # d_t5qqqqvv_dm20["mydir"] = "v8.04_Sept19"
    d_t5qqqqvv_dm20["mydir"] = "v8.04_t5qqqqdm20_July28" # FIXME
    d_t5qqqqvv_dm20["modeltag"] = "t5qqqqvv_dm20"
    d_t5qqqqvv_dm20["postfix"] = "beforefix"
    d_t5qqqqvv_dm20["maxy"] = 1825
    d_t5qqqqvv_dm20["ybinsfirstxbin"] = 21
    d_t5qqqqvv_dm20["mass_label"] = "m_{#tilde{#chi}^{#pm}_{1}} = m_{#tilde{#chi}^{0}_{1}} + 20 GeV"

    d_t6ttww = d_t1tttt.copy()
    d_t6ttww["mydir"] = "v8.04_Sept19"
    d_t6ttww["modeltag"] = "t6ttww"
    d_t6ttww["is_gluino"] = False
    d_t6ttww["label_diag"] = "m_{#tilde{b}_{1}} = m_{#tilde{#chi}_{1}^{#pm}}"
    d_t6ttww["label_xaxis"] = "m_{#tilde{b}_{1}} (GeV)"
    d_t6ttww["label_yaxis"] = "m_{#tilde{#chi}_{1}^{#pm}} (GeV)"
    d_t6ttww["label_process"] = "pp #rightarrow #tilde{b}_{1}#bar{#tilde{b}}_{1}, #tilde{b}_{1}#rightarrow tW#tilde{#chi}^{0}_{1}        "
    d_t6ttww["mass_label"] = "m_{#tilde{#chi}^{0}_{1}} = 50 GeV"
    d_t6ttww["diag_points"] = [300,300,900,910]
    d_t6ttww["diag_labelpoints"] = [0.2,0.325]
    d_t6ttww["diag_labelangle"] = 38
    d_t6ttww["step"] = 25
    d_t6ttww["minx"] = 300
    d_t6ttww["maxx"] = 950+25
    d_t6ttww["miny"] = 75
    d_t6ttww["maxy"] = 875+25
    d_t6ttww["maxyh"] = 1175
    d_t6ttww["ybinsfirstxbin"] = 5

    d_t5ttcc = d_t1tttt.copy()
    d_t5ttcc["mydir"] = "v8.04_Sept21"
    d_t5ttcc["modeltag"] = "t5ttcc"
    d_t5ttcc["label_diag"] = "m_{#tilde{g}} - m_{#tilde{#chi}^{0}_{1}} = m_{W} + m_{b}"
    d_t5ttcc["label_xaxis"] = "m_{#tilde{b}_{1}} (GeV)"
    d_t5ttcc["label_yaxis"] = "m_{#tilde{#chi}_{1}^{#pm}} (GeV)"
    d_t5ttcc["label_process"] = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow #tilde{t}_{1}#kern[0.3]{t}, #tilde{t}_{1}#rightarrow c#tilde{#chi}^{0}_{1}  "
    d_t5ttcc["mass_label"] = "m_{#kern[0.5]{#tilde{t}_{1}}} = m_{#tilde{#chi}^{0}_{1}} + 20 GeV"
    d_t5ttcc["diag_points"] = [600,515,1600,1515]
    d_t5ttcc["diag_labelpoints"] = [0.2,0.405]
    d_t5ttcc["diag_labelangle"] = 37
    d_t5ttcc["step"] = 25
    d_t5ttcc["minx"] = 600
    d_t5ttcc["maxx"] = 1700+25
    d_t5ttcc["miny"] = 0
    d_t5ttcc["maxy"] = 1350+25
    d_t5ttcc["maxyh"] = 1700
    d_t5ttcc["ybinsfirstxbin"] = 19

    d_t5tttt_dm175 = d_t5ttcc.copy()
    d_t5tttt_dm175["mydir"] = "v8.04_Sept21"
    d_t5tttt_dm175["modeltag"] = "t5tttt_dm175"
    d_t5tttt_dm175["label_diag"] = "m_{#tilde{g}} - m_{#tilde{#chi}^{0}_{1}} = m_{t} + m_{W} + m_{b}"
    d_t5tttt_dm175["label_xaxis"] = "m_{#tilde{g}} (GeV)"
    d_t5tttt_dm175["label_yaxis"] = "m_{#tilde{#chi}_{1}^{0}} (GeV)"
    d_t5tttt_dm175["label_process"] = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow #tilde{t}_{1}#kern[0.3]{t}, #tilde{t}_{1}#rightarrow t#tilde{#chi}^{0}_{1}  "
    d_t5tttt_dm175["mass_label"] = "m_{#kern[0.5]{#tilde{t}_{1}}} = m_{#tilde{#chi}^{0}_{1}} + m_{#kern[0.3]{t}}"
    d_t5tttt_dm175["diag_points"] = [600,340,1700,1440]
    d_t5tttt_dm175["diag_labelpoints"] = [0.2,0.325]
    d_t5tttt_dm175["diag_labelangle"] = 37
    d_t5tttt_dm175["step"] = 25
    d_t5tttt_dm175["minx"] = 600
    d_t5tttt_dm175["maxx"] = 1700+25
    d_t5tttt_dm175["miny"] = 0
    d_t5tttt_dm175["maxy"] = 1250+25
    d_t5tttt_dm175["maxyh"] = 1700
    d_t5tttt_dm175["ybinsfirstxbin"] = 13

    d_t1ttbb = d_t1tttt.copy()
    d_t1ttbb["mydir"] = "v8.04_Sept21"
    d_t1ttbb["redolimits"] = True
    d_t1ttbb["modeltag"] = "t1ttbb"
    d_t1ttbb["label_diag"] = "m_{#tilde{g}}-m_{#tilde{#chi}_{1}^{0}} = 2 #upoint (m_{W} + m_{b})"
    d_t1ttbb["label_xaxis"] = "m_{#tilde{g}} (GeV)"
    d_t1ttbb["label_yaxis"] = "m_{#tilde{#chi}_{1}^{0}} (GeV)"
    d_t1ttbb["label_process"] = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow tb#tilde{#chi}^{#pm}_{1}          "
    d_t1ttbb["mass_label"] = "m_{#tilde{#chi}^{#pm}_{1}} = m_{#tilde{#chi}^{0}_{1}} + 5 GeV"
    d_t1ttbb["diag_points"] = [600,430,1700,1530]
    d_t1ttbb["diag_labelpoints"] = [0.2,0.365]
    d_t1ttbb["diag_labelangle"] = 37
    d_t1ttbb["step"] = 25
    d_t1ttbb["minx"] = 600
    d_t1ttbb["maxx"] = 1700+25
    d_t1ttbb["miny"] = 0
    d_t1ttbb["maxy"] = 1400+25
    d_t1ttbb["maxyh"] = 1700
    d_t1ttbb["ybinsfirstxbin"] = 15

    plot_limits(d_t5qqqqvv_dm20)
    # plot_limits(d_t5qqqqvv)
    # plot_limits(d_t1tttt)
    # plot_limits(d_t6ttww)
    # plot_limits(d_t5ttcc)
    # plot_limits(d_t5tttt_dm175)
    # plot_limits(d_t1ttbb)
