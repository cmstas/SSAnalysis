import os, ROOT, array

def interpolate(h2):
    for xbin in range(2,h2.GetNbinsX()): #ok we do not consider the edges
        for ybin in range(2,h2.GetNbinsY()):
            if h2.GetBinContent(xbin,ybin)>0: continue #let's do it only for empty bins
            myxbinL = 0
            myxbinLVal = 0
            myxbinR = 0
            myxbinRVal = 0
            for xbinL in range(xbin-1,0,-1): 
                if h2.GetBinContent(xbinL,ybin-1)>0: 
                    myxbinL = xbinL
                    myxbinLVal = h2.GetBinContent(xbinL,ybin-1)
                    break 
            for xbinR in range(xbin+1,h2.GetNbinsX()+1): 
                if h2.GetBinContent(xbinR,ybin+1)>0: 
                    myxbinR = xbinR
                    myxbinRVal = h2.GetBinContent(xbinR,ybin+1)
                    break 
            myybinD = 0
            myybinDVal = 0
            myybinU = 0
            myybinUVal = 0
            for ybinD in range(ybin-1,0,-1): 
                if h2.GetBinContent(xbin-1,ybinD)>0: 
                    myybinD = ybinD
                    myybinDVal = h2.GetBinContent(xbin,ybinD)
                    break 
            for ybinU in range(ybin+1,h2.GetNbinsY()+1): 
                if h2.GetBinContent(xbin,ybinU)>0: 
                    myybinU = ybinU
                    myybinUVal = h2.GetBinContent(xbin,ybinU)
                    break 
            if myxbinL!=0 and myxbinR!=0 and myybinD!=0 and myybinU!=0:
                h2.SetBinContent(xbin,ybin,((1./(xbin-myxbinL))*myxbinLVal+(1./(myxbinR-xbin))*myxbinRVal+(1./(ybin-myybinD))*myybinDVal+(1./(myybinU-ybin))*myybinUVal)/(1./(xbin-myxbinL)+1./(myxbinR-xbin)+1./(ybin-myybinD)+1./(myybinU-ybin)))
            elif myxbinL!=0 and myxbinR!=0:
                h2.SetBinContent(xbin,ybin,((1./(xbin-myxbinL))*myxbinLVal+(1./(myxbinR-xbin))*myxbinRVal)/(1./(xbin-myxbinL)+1./(myxbinR-xbin)))
            else: continue
    return

mydir = "v5.06"
mylumi = "2.1"

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

#mpoints = []
#mpoints.append("m1325_m925")
#mpoints.append("m1350_m100")
#mpoints.append("m1350_m0")
#mpoints.append("m1350_m1075")
#mpoints.append("m1350_m1000")
#mpoints.append("m1350_m250")
#mpoints.append("m1325_m950")
#mpoints.append("m1350_m1025")
#mpoints.append("m1350_m1125")
#mpoints.append("m1350_m200")
#mpoints.append("m1325_m975")
#mpoints.append("m1350_m1100")
#mpoints.append("m1350_m150")
#mpoints.append("m1350_m1050")
#mpoints.append("m1350_m300")
#mpoints.append("m1400_m1125")
#mpoints.append("m1400_m1175")
#mpoints.append("m1400_m200")
#mpoints.append("m1400_m150")
#mpoints.append("m1400_m1150")
#mpoints.append("m1400_m250")
#mpoints.append("m1400_m300")
#mpoints.append("m1400_m350")
#mpoints.append("m1400_m400")
#mpoints.append("m1400_m1025")
#mpoints.append("m1400_m1050")
#mpoints.append("m1400_m1075")
#mpoints.append("m1400_m0")
#mpoints.append("m1400_m100")
#mpoints.append("m1400_m1000")
#mpoints.append("m1400_m1100")
#for mpoint in mpoints:
        #if os.path.isfile(("%s/fs_t1tttt_%s_histos_hihi_%sifb.root" % (mydir,mpoint,mylumi))) is False: continue
        #print "found sig = fs_t1tttt_"+mpoint
        #sigs.append(("fs_t1tttt_%s" % (mpoint)))

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

h_xsec = ROOT.TH2F("h_xsec","",56,600,2000,80,0,2000)
h_sobs = ROOT.TH2F("h_sobs","",56,600,2000,80,0,2000)
h_sexp = ROOT.TH2F("h_sexp","",56,600,2000,80,0,2000)
h_ssm1 = ROOT.TH2F("h_ssm1","",56,600,2000,80,0,2000)
h_ssp1 = ROOT.TH2F("h_ssp1","",56,600,2000,80,0,2000)

count = 0
print "limits "
for sig in sigs:
    #print sig
    smod = sig.split("_")[1]
    mglu = int(sig.split("_")[2][1:])
    mlsp = int(sig.split("_")[3][1:])
    h_xsec.SetBinContent( h_xsec.FindBin(mglu,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
    h_sobs.SetBinContent( h_xsec.FindBin(mglu,mlsp), limit_obs[count] )
    h_sexp.SetBinContent( h_xsec.FindBin(mglu,mlsp), limit_exp[count] )
    h_ssm1.SetBinContent( h_xsec.FindBin(mglu,mlsp), limit_sm1[count] )
    h_ssp1.SetBinContent( h_xsec.FindBin(mglu,mlsp), limit_sp1[count] )
    if mglu<1000 and mlsp<200:
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+50,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+50,mlsp), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+50,mlsp), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+50,mlsp), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+50,mlsp), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+75,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+75,mlsp), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+75,mlsp), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+75,mlsp), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+75,mlsp), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+25), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+25), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+25), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+25), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+25), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+25), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+25), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+25), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+25), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+25), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu,mlsp+50), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu,mlsp+50), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu,mlsp+50), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu,mlsp+50), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu,mlsp+50), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+50), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+50), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+50), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+50), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+50), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+50), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+50), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+50), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+50), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+50), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+50), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+50), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+50), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+50), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+50), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu,mlsp+75), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu,mlsp+75), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu,mlsp+75), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu,mlsp+75), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu,mlsp+75), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+75), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+75), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+75), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+75), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+75), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+75), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+75), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+75), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+75), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+50,mlsp+75), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+75), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+75), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+75), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+75), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+75,mlsp+75), limit_sp1[count] )
    if mglu-mlsp>425 or mlsp>=1200:
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu,mlsp+25), limit_sp1[count] )
        h_xsec.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_obs[count]*hxsec.GetBinContent(hxsec.FindBin(mglu)) )
        h_sobs.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_obs[count] )
        h_sexp.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_exp[count] )
        h_ssm1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_sm1[count] )
        h_ssp1.SetBinContent( h_xsec.FindBin(mglu+25,mlsp+25), limit_sp1[count] )

    print ("%-8s %4i %4i | %.1f | %.1f (-1s %.1f, +1s %.1f) | %.3f " % (smod , mglu, mlsp , limit_obs[count]  , limit_exp[count] , limit_sm1[count] , limit_sp1[count], hxsec.GetBinContent(hxsec.FindBin(mglu)) ) )
    count = count + 1

interpolate( h_xsec )
interpolate( h_sobs )
interpolate( h_sexp )
interpolate( h_ssm1 )
interpolate( h_ssp1 )

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
h_xsec.GetXaxis().SetLabelSize(0.035)
h_xsec.GetYaxis().SetLabelSize(0.035)
h_xsec.GetXaxis().SetTitle("gluino mass [GeV]")
h_xsec.GetYaxis().SetTitle("#tilde{#chi}^{0}_{1} mass [GeV]")
h_xsec.GetZaxis().SetTitle("95% CL upper limit on #sigma [pb]")
h_xsec.GetZaxis().SetRangeUser(1e-3,2e0)
h_xsec.GetZaxis().SetLabelSize(0.035)
h_xsec.GetXaxis().SetTitleOffset(1.2)
h_xsec.GetYaxis().SetTitleOffset(1.7)
h_xsec.GetZaxis().SetTitleOffset(1.6)

cobs = h_sobs.Clone("cobs");
#trick for driving the contour
for xbin in range(1,cobs.GetNbinsX()+1):
    for ybin in range(1,cobs.GetNbinsY()+1):
        if (ybin*25-xbin*25)>325:
             cobs.SetBinContent(xbin,ybin,1.)
        else: 
            if (600+xbin*25)>1500 or (ybin*25>900):
                cobs.SetBinContent(xbin,ybin,10.)
contours = array.array('d')
contours.append(1.0)
cobs.SetContour(1, contours);
cobs.SetLineWidth(4);
cobs.SetLineStyle(1);
cobs.SetLineColor(ROOT.kBlack);
cobs.Smooth();

cexp = h_sexp.Clone("cexp");
#trick for driving the contour
for xbin in range(1,cexp.GetNbinsX()+1):
    for ybin in range(1,cexp.GetNbinsY()+1):
        if (ybin*25-xbin*25)>325:
             cexp.SetBinContent(xbin,ybin,1.)
        else: 
            if (600+xbin*25)>1500 or (ybin*25>900):
                cexp.SetBinContent(xbin,ybin,10.)
contours = array.array('d')
contours.append(1.0)
cexp.SetContour(1, contours);
cexp.SetLineWidth(4);
cexp.SetLineStyle(1);
cexp.SetLineColor(ROOT.kRed);
cexp.Smooth();

csm1 = h_ssm1.Clone("csm1");
#trick for driving the contour
for xbin in range(1,csm1.GetNbinsX()+1):
    for ybin in range(1,csm1.GetNbinsY()+1):
        if (ybin*25-xbin*25)>325:
             csm1.SetBinContent(xbin,ybin,1.)
        else: 
            if (600+xbin*25)>1500 or (ybin*25>900):
                csm1.SetBinContent(xbin,ybin,10.)
contours = array.array('d')
contours.append(1.0)
csm1.SetContour(1, contours);
csm1.SetLineWidth(2);
csm1.SetLineStyle(3);
csm1.SetLineColor(ROOT.kRed);
csm1.Smooth();

csp1 = h_ssp1.Clone("csp1");
#trick for driving the contour
for xbin in range(1,csp1.GetNbinsX()+1):
    for ybin in range(1,csp1.GetNbinsY()+1):
        if (ybin*25-xbin*25)>325:
             csp1.SetBinContent(xbin,ybin,1.)
        else: 
            if (600+xbin*25)>1500 or (ybin*25>900):
                csp1.SetBinContent(xbin,ybin,10.)
contours = array.array('d')
contours.append(1.0)
csp1.SetContour(1, contours);
csp1.SetLineWidth(2);
csp1.SetLineStyle(3);
csp1.SetLineColor(ROOT.kRed);
csp1.Smooth();

h_xsec.Draw("colz")
cobs.Draw("samecont2");
cexp.Draw("samecont2");
csm1.Draw("samecont3");
csp1.Draw("samecont3");

diag = ROOT.TLine(625,425,1700,1500)
diag.SetLineWidth(2)
diag.SetLineStyle(2)
diag.Draw("same")

l1 = ROOT.TLegend(0.15, 0.70, 0.83, 0.90)
l1.SetTextFont(42)
l1.SetTextSize(0.038)
l1.SetShadowColor(ROOT.kWhite)
l1.SetFillColor(ROOT.kWhite)
l1.SetHeader("pp #rightarrow #tilde{g} #tilde{g}, #tilde{g}#rightarrow t#bar{t}#tilde{#chi}^{0}_{1} 95% CL NLO Exclusions")
l1.AddEntry(cexp , "Expected limit, #pm 1 #sigma_{exp.}", "l")
l1.AddEntry(cobs , "Observed limit"            , "l")


cmstex = ROOT.TLatex(0.575,0.91, "2.1 fb^{-1} (13 TeV)" )
cmstex.SetNDC()
cmstex.SetTextSize(0.04)
cmstex.SetLineWidth(2)
cmstex.SetTextFont(42)
cmstex.Draw()

cmstexbold = ROOT.TLatex(0.17,0.91, "CMS Preliminary" )
cmstexbold.SetNDC()
cmstexbold.SetTextSize(0.04)
cmstexbold.SetLineWidth(2)
cmstexbold.SetTextFont(62)
cmstexbold.Draw()

padt.Update()
padt.RedrawAxis();
l1.Draw("same")

c1.SaveAs("xsec.png")
c1.SaveAs("t1ttt_scan_xsec.pdf")

h_sobs.GetXaxis().SetLabelSize(0.035)
h_sobs.GetYaxis().SetLabelSize(0.035)
h_sobs.GetXaxis().SetTitle("gluino mass [GeV]")
h_sobs.GetYaxis().SetTitle("#chi_{1}^{0} mass [GeV]")
h_sobs.GetZaxis().SetTitle("95% CL observed upper limit on signal strength")
h_sobs.GetZaxis().SetRangeUser(0.1,10.0)
h_sobs.GetZaxis().SetLabelSize(0.035)
h_sobs.GetXaxis().SetTitleOffset(1.1)
h_sobs.GetYaxis().SetTitleOffset(1.6)
h_sobs.GetZaxis().SetTitleOffset(1.6)
h_sobs.Draw("colz")
cobs.Draw("samecont2");
c1.SaveAs("sobs.png")

h_sexp.GetXaxis().SetLabelSize(0.035)
h_sexp.GetYaxis().SetLabelSize(0.035)
h_sexp.GetXaxis().SetTitle("gluino mass [GeV]")
h_sexp.GetYaxis().SetTitle("#chi_{1}^{0} mass [GeV]")
h_sexp.GetZaxis().SetTitle("95% CL expected upper limit on signal strength")
h_sexp.GetZaxis().SetRangeUser(0.1,10.0)
h_sexp.GetZaxis().SetLabelSize(0.035)
h_sexp.GetXaxis().SetTitleOffset(1.1)
h_sexp.GetYaxis().SetTitleOffset(1.6)
h_sexp.GetZaxis().SetTitleOffset(1.6)
h_sexp.Draw("colz")
cexp.Draw("samecont2");
c1.SaveAs("sexp.png")
