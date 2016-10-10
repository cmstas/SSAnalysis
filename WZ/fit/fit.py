import ROOT as r
import os
import math


f1 = r.TFile("h1D_nbtags.root")

keys = f1.GetListOfKeys()
histnames = [key.GetName() for key in keys if key.ReadObj().InheritsFrom(r.TH1F.Class())]
hname_data = [hn for hn in histnames if "Data" in hn][0]
hname_wz = [hn for hn in histnames if "WZ" in hn][0]
hname_ttz = [hn for hn in histnames if "ttZ" in hn][0]
hname_fakes = [hn for hn in histnames if "Fakes" in hn][0]
hnames_rares = [hn for hn in histnames if ("Fakes" not in hn) and ("WZ" not in hn) and ("Data" not in hn) and ("ttZ" not in hn)]

h_data = f1.Get(hname_data).Clone("h_data")
h_wz = f1.Get(hname_wz).Clone("h_wz")
h_ttz = f1.Get(hname_ttz).Clone("h_tzz")
h_fakes = f1.Get(hname_fakes).Clone("h_fakes")
hists_rares = [f1.Get(hn) for hn in hnames_rares]

h_rares = hists_rares[0].Clone("h_rares")
for h in hists_rares[1:]:
    h_rares.Add(h)

# # set 50% error on nonWZ/ttZ
# for ix in range(1,h_rares.GetNbinsX()+1):
#     stat = h_rares.GetBinError(ix)
#     cont = h_rares.GetBinContent(ix)
#     syst = cont*0.5
#     unc = math.sqrt(stat*stat + syst*syst)
#     h_rares.SetBinError(ix, unc)

########################################
################ PREFIT ################
########################################

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

# draw stuff and upload it
c1 = r.TCanvas()
h_stack.Draw("hist")
h_data.Draw("pesame")
leg.Draw("same")
c1.SaveAs("plots/nbtags_prefit.pdf")

mc = r.TObjArray(3)
mc.Add(h_wz)
mc.Add(h_ttz)
mc.Add(h_fakes)
mc.Add(h_rares)
fit = r.TFractionFitter(h_data, mc);
# fit.Constrain(2, frac_rares-0.001, frac_rares+0.001)
fit.Constrain(2, 0.7*frac_fakes, 1.3*frac_fakes)
fit.Constrain(2, 0.5*frac_rares, 1.5*frac_rares)
fit.Fit()
val, err = r.Double(0.0), r.Double(0.0)
fit.GetResult(0, val, err)
frac_new_wz, frac_err_wz = 1.0*val, 1.0*err
fit.GetResult(1, val, err)
frac_new_ttz, frac_err_ttz = 1.0*val, 1.0*err
fit.GetResult(2, val, err)
frac_new_fakes, frac_err_fakes = 1.0*val, 1.0*err
fit.GetResult(3, val, err)
frac_new_rares, frac_err_rares = 1.0*val, 1.0*err

print frac_new_wz, frac_err_wz
print frac_new_ttz, frac_err_ttz
print frac_new_fakes, frac_err_fakes
print frac_new_rares, frac_err_rares

########################################
############### POSTFIT ################
########################################

# scale hists
h_wz.Scale(frac_new_wz/frac_wz)
h_ttz.Scale(frac_new_ttz/frac_ttz)
h_fakes.Scale(frac_new_fakes/frac_fakes)
h_rares.Scale(frac_new_rares/frac_rares)

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
leg.AddEntry(h_wz, "WZ [= %.2f*data]" % frac_new_wz, "f")
leg.AddEntry(h_ttz, "t#bar{t}Z [= %.2f*data]" % frac_new_ttz, "f")
leg.AddEntry(h_fakes, "fakes [= %.2f*data]" % frac_new_fakes, "f")
leg.AddEntry(h_rares, "rares MC [= %.2f*data]" % frac_new_rares, "f")


# draw stuff and upload it
c1 = r.TCanvas()
h_stack.Draw("hist")
h_data.Draw("pesame")
leg.Draw("same")


sfs = r.TLatex()
sfs.SetTextSize(0.035)
sfs.SetTextFont(42)
sf_ttz = frac_new_ttz / frac_ttz
sf_wz = frac_new_wz / frac_wz
sf_fakes = frac_new_fakes / frac_fakes
sf_rares = frac_new_rares / frac_rares
sfs.DrawLatexNDC(0.55,0.60, "SF t#bar{t}Z = %.2f #pm %.2f" % (sf_ttz, sf_ttz*frac_err_ttz/frac_new_ttz))
sfs.DrawLatexNDC(0.55,0.55, "SF WZ = %.2f #pm %.2f" % (sf_wz, sf_wz*frac_err_wz/frac_new_wz))
sfs.DrawLatexNDC(0.55,0.50, "SF fakes = %.2f #pm %.2f" % (sf_fakes, sf_fakes*frac_err_fakes/frac_new_fakes))
sfs.DrawLatexNDC(0.55,0.45, "SF rares = %.2f #pm %.2f" % (sf_rares, sf_rares*frac_err_rares/frac_new_rares))

c1.SaveAs("plots/nbtags_postfit.pdf")

os.system("niceplots plots plots_wzttzsf_Oct9")
