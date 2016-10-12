import ROOT as r
import os
import math
import numpy as np

def get_weight_hist(h1, syst=0.0):
    """
    if syst=0.5, then take stat errors in input hist and add in quadrature to 0.5*bin content
    for each bin. spit out a new histogram where 1/(these errors) are the bin contents.
    """
    h2 = h1.Clone()
    for ix in range(1,h2.GetNbinsX()+1):
        stat = h2.GetBinError(ix)
        cont = h2.GetBinContent(ix)
        unc = math.sqrt(stat*stat + (syst*cont)**2)
        # FIXME should be 1/unc**2, but this causes more problems
        weight = 1.0/unc if unc > 0.0001 else 0.0001
        h2.SetBinContent(ix, weight)
        print ix, stat, cont, unc, weight
    return h2

# bypass TFractionFitter and use manual toys instead
do_nick_fit = False

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

# draw stuff
c1 = r.TCanvas()
h_stack.Draw("hist")
h_data.Draw("pesame")
leg.Draw("same")
c1.SaveAs("plots/nbtags_prefit.pdf")

# instantiate TFractionFitter, excluding Nbtags==3 bin
mc = r.TObjArray(4)
mc.Add(h_wz)
mc.Add(h_ttz)
mc.Add(h_fakes)
mc.Add(h_rares)
fit = r.TFractionFitter(h_data, mc);
fit.ExcludeBin(4)

# constrain each MC fraction to be between 0 and 1
for i in range(4):
    fit.Constrain(i, 0.0, 1.0)

# # constrain hists by fixed fraction
# fit.Constrain(2, frac_rares-0.001, frac_rares+0.001)
# fit.Constrain(2, 0.7*frac_fakes, 1.3*frac_fakes)
# fit.Constrain(3, 0.5*frac_rares, 1.5*frac_rares)



# do the fit and get the results
if do_nick_fit:
    import manual
    nbins = 3

    vals_data = np.array([h_data.GetBinContent(i) for i in range(1,nbins+1)])
    errs_data = np.array([h_data.GetBinError(i) for i in range(1,nbins+1)])
    vals_wz = np.array([h_wz.GetBinContent(i) for i in range(1,nbins+1)])
    errs_wz = np.array([h_wz.GetBinError(i) for i in range(1,nbins+1)])
    vals_ttz = np.array([h_ttz.GetBinContent(i) for i in range(1,nbins+1)])
    errs_ttz = np.array([h_ttz.GetBinError(i) for i in range(1,nbins+1)])
    vals_fakes = np.array([h_fakes.GetBinContent(i) for i in range(1,nbins+1)])
    errs_fakes = np.array([h_fakes.GetBinError(i) for i in range(1,nbins+1)])
    vals_rares = np.array([h_rares.GetBinContent(i) for i in range(1,nbins+1)])
    errs_rares = np.array([h_rares.GetBinError(i) for i in range(1,nbins+1)])

    # FIXME change the syst values on lines 45-48 of manual.py depending on the considered cases
    sfs = manual.get_sfs(
            vals_data, vals_wz, vals_ttz, vals_fakes, vals_rares,
            errs_data, errs_wz, errs_ttz, errs_fakes, errs_rares,
            ntrials=100,ntoys=1000
            )
    sf_wz    , sf_err_wz = sfs[0]
    sf_ttz   , sf_err_ttz = sfs[1]
    sf_fakes , sf_err_fakes = sfs[2]
    sf_rares , sf_err_rares = sfs[3]
else:

    # constrain hists with bin-by-bin weights
    # FIXME change syst values appropriately
    fit.SetWeight(0,get_weight_hist(h_wz,syst=0.5))
    fit.SetWeight(1,get_weight_hist(h_ttz,syst=0.5))
    fit.SetWeight(2,get_weight_hist(h_fakes,syst=0.0)) # stat + 30% syst
    fit.SetWeight(3,get_weight_hist(h_rares,syst=0.0)) # stat + 50% syst

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

# fit.GetPlot().Draw()
# # for i in range(4):
# #     fit.GetMCPrediction(i).Draw()
# c1.SaveAs("test.pdf")

########################################
############### POSTFIT ################
########################################


# h_wz = fit.GetMCPrediction(0)
# h_ttz = fit.GetMCPrediction(1)
# h_fakes = fit.GetMCPrediction(2)
# h_rares = fit.GetMCPrediction(3)

if do_nick_fit:
    h_wz.Scale(sf_wz)
    h_ttz.Scale(sf_ttz)
    h_fakes.Scale(sf_fakes)
    h_rares.Scale(sf_rares)
else:
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
if not do_nick_fit:
    leg.AddEntry(h_wz, "WZ [= %.2f*data]" % frac_new_wz, "f")
    leg.AddEntry(h_ttz, "t#bar{t}Z [= %.2f*data]" % frac_new_ttz, "f")
    leg.AddEntry(h_fakes, "fakes [= %.2f*data]" % frac_new_fakes, "f")
    leg.AddEntry(h_rares, "rares MC [= %.2f*data]" % frac_new_rares, "f")
else:
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
if not do_nick_fit:
    sf_wz = frac_new_wz / frac_wz
    sf_ttz = frac_new_ttz / frac_ttz
    sf_fakes = frac_new_fakes / frac_fakes
    sf_rares = frac_new_rares / frac_rares
    sf_err_ttz = sf_ttz*frac_err_ttz/frac_new_ttz
    sf_err_wz = sf_wz*frac_err_wz/frac_new_wz
    sf_err_fakes = sf_fakes*frac_err_fakes/frac_new_fakes
    sf_err_rares = sf_rares*frac_err_rares/frac_new_rares
sfs.DrawLatexNDC(0.55,0.60, "SF WZ = %.2f #pm %.2f" % (sf_wz, sf_err_wz))
sfs.DrawLatexNDC(0.55,0.55, "SF t#bar{t}Z = %.2f #pm %.2f" % (sf_ttz, sf_err_ttz))
sfs.DrawLatexNDC(0.55,0.50, "SF fakes = %.2f #pm %.2f" % (sf_fakes, sf_err_fakes))
sfs.DrawLatexNDC(0.55,0.45, "SF rares = %.2f #pm %.2f" % (sf_rares, sf_err_rares))

c1.SaveAs("plots/nbtags_postfit.pdf")

os.system("niceplots plots plots_wzttzsf_Oct10")
