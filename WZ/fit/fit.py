import ROOT as r
import os
import math
import numpy as np
import commands
import ast
import sys





infile = "onebin_h1D_nbtags.root"
# infile = "h1D_nbtags_withbsf.root"
f1 = r.TFile(infile)

lumi = float(commands.getoutput(""" grep "^float getLumi()" ../../commonUtils.h | cut -d';' -f 1 | cut -d'n' -f2 """))

keys = f1.GetListOfKeys()
histnames = [key.GetName() for key in keys if key.ReadObj().InheritsFrom(r.TH1F.Class())]
hname_data = [hn for hn in histnames if "Data" in hn][0]
hname_wz = [hn for hn in histnames if "WZ" in hn][0]
hname_ttz = [hn for hn in histnames if "ttZ" in hn][0]
hname_fakes = [hn for hn in histnames if "Fakes" in hn][0]
hnames_rares = [hn for hn in histnames if ("Fakes" not in hn) and ("WZ" not in hn) and ("Data" not in hn) and ("ttZ" not in hn)]

h_data = f1.Get(hname_data).Clone("h_data")
h_wz = f1.Get(hname_wz).Clone("h_wz")
h_ttz = f1.Get(hname_ttz).Clone("h_ttz")
h_fakes = f1.Get(hname_fakes).Clone("h_fakes")
hists_rares = [f1.Get(hn) for hn in hnames_rares]

h_rares = hists_rares[0].Clone("h_rares")
for h in hists_rares[1:]:
    h_rares.Add(h)
h_rares.SetFillColor(r.kMagenta-7)


########################################
################ PREFIT ################
########################################

os.system("mkdir -p plots")

# get counts, fractions of data
tot_data = h_data.GetEntries()
tot_mc = h_wz.Integral() + h_ttz.Integral() + h_rares.Integral()
frac_wz = h_wz.Integral() / tot_data
frac_ttz = h_ttz.Integral() / tot_data
frac_fakes = h_fakes.Integral() / tot_data
frac_rares = h_rares.Integral() / tot_data

h_stack = r.THStack("st", "Nbtags [prefit];Nbtags;Entries")
h_stack.Add(h_ttz)
h_stack.Add(h_wz)
h_stack.Add(h_fakes)
h_stack.Add(h_rares)
h_stack.Draw()
h_stack.SetMaximum( 1.1*max(h_stack.GetMaximum(),h_data.GetMaximum()) )

# pretty legend
leg = r.TLegend(0.53,0.65,0.85,0.87)
leg.AddEntry(h_data, "Data", "lep")
leg.AddEntry(h_wz, "WZ [= %.2f*data]" % frac_wz, "f")
leg.AddEntry(h_ttz, "t#bar{t}Z [= %.2f*data]" % frac_ttz, "f")
leg.AddEntry(h_fakes, "fakes [= %.2f*data]" % frac_fakes, "f")
leg.AddEntry(h_rares, "rares MC [= %.2f*data]" % frac_rares, "f")

sfs = r.TLatex()
sfs.SetTextSize(0.035)
sfs.SetTextFont(42)

# draw stuff
c1 = r.TCanvas()
h_stack.Draw("hist")
h_data.Draw("pesame")
leg.Draw("same")
sfs.DrawLatexNDC(0.55,0.60, "L = %.2f fb^{-1}" % (lumi))
c1.SaveAs("plots/nbtags_prefit.pdf")

# sigh...interference between histogram names, so need to call this in a different process...can't just import it
lnNsig = 2.0
lnNbg = 1.50
shapeUnc = 0.1

if len(sys.argv) > 3:
    lnNsig = float(sys.argv[1])
    lnNbg = float(sys.argv[2])
    shapeUnc = float(sys.argv[3])

output = commands.getoutput(""" python -c 'd_sfs = __import__("useCombine").get_sfs("%s", %.2f, %.2f, %.2f); print "GREP",d_sfs' | grep "GREP" """ % (infile, lnNsig, lnNbg, shapeUnc))
d_sfs = ast.literal_eval(output.replace("GREP","").strip())

sf_wz    , sf_err_wz = d_sfs["totals"]["wz"]
sf_ttz   , sf_err_ttz = d_sfs["totals"]["ttz"]
sf_fakes , sf_err_fakes = d_sfs["totals"]["fakes"]
sf_rares , sf_err_rares = d_sfs["totals"]["rares"]

########################################
############### POSTFIT ################
########################################

h_wz.Scale(sf_wz)
h_ttz.Scale(sf_ttz)
h_fakes.Scale(sf_fakes)
h_rares.Scale(sf_rares)


h_stack = r.THStack("st", "Nbtags [postfit];Nbtags;Entries")
h_stack.Add(h_ttz)
h_stack.Add(h_wz)
h_stack.Add(h_fakes)
h_stack.Add(h_rares)
h_stack.Draw()
h_stack.SetMaximum( 1.1*max(h_stack.GetMaximum(),h_data.GetMaximum()) )

# pretty legend
leg = r.TLegend(0.53,0.65,0.85,0.87)
leg.AddEntry(h_data, "Data", "lep")
leg.AddEntry(h_wz, "WZ", "f")
leg.AddEntry(h_ttz, "t#bar{t}Z", "f")
leg.AddEntry(h_fakes, "fakes", "f")
leg.AddEntry(h_rares, "rares MC", "f")

# draw stuff
h_stack.Draw("hist")
h_data.Draw("pesame")
leg.Draw("same")

sfs = r.TLatex()
sfs.SetTextSize(0.035)
sfs.SetTextFont(42)
sfs.DrawLatexNDC(0.55,0.60, "SF WZ = %.2f #pm %.2f" % (sf_wz, sf_err_wz))
sfs.DrawLatexNDC(0.55,0.55, "SF t#bar{t}Z = %.2f #pm %.2f" % (sf_ttz, sf_err_ttz))
sfs.DrawLatexNDC(0.55,0.50, "SF fakes = %.2f #pm %.2f" % (sf_fakes, sf_err_fakes))
sfs.DrawLatexNDC(0.55,0.45, "SF rares = %.2f #pm %.2f" % (sf_rares, sf_err_rares))
sfs.DrawLatexNDC(0.55,0.40, "L = %.2f fb^{-1}" % (lumi))

c1.SaveAs("plots/nbtags_postfit_%.2f_%.2f_%.2f.pdf" % (lnNsig,lnNbg,shapeUnc))
os.system("web plots/nbtags_postfit_%.2f_%.2f_%.2f.pdf" % (lnNsig,lnNbg,shapeUnc))
print "lnNsig,lnNbg,shapeUnc,wz,ttz,fakes,rares", lnNsig,lnNbg,shapeUnc,sf_wz, sf_err_wz, sf_ttz, sf_err_ttz, sf_fakes, sf_err_fakes, sf_rares, sf_err_rares
print "wzsf: %.2f pm %.2f" % (sf_wz, sf_err_wz)
print "ttzsf: %.2f pm %.2f" % (sf_ttz, sf_err_ttz)
# os.system("niceplots plots plots_wzttzsf_Oct10")
