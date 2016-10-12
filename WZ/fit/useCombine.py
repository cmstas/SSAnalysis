import ROOT as r
import os
import sys
import math
import commands
from errors import E


def get_variations(h1, syst=0.0):
    hUp = h1.Clone(h1.GetName()+"Up")
    hDown = h1.Clone(h1.GetName()+"Down")
    for ix in range(1,h1.GetNbinsX()+1):
        stat = h1.GetBinError(ix)
        cont = h1.GetBinContent(ix)
        unc = math.sqrt(stat*stat + (syst*cont)**2)
        hUp.SetBinContent(ix, cont+unc)
        hDown.SetBinContent(ix, cont-unc)
    return hUp,h1,hDown

def get_yield_and_error(h1):
    err = r.Double(0.0)
    integral = h1.IntegralAndError(0,-1,err)
    return float(integral), float(err)


infile = "h1D_nbtags.root"
outfile = "forCard.root"
f1 = r.TFile(infile)

print ">>> Reading input histograms"
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

d_prefit = {
        "data": get_yield_and_error(h_data),
        "wz": get_yield_and_error(h_wz),
        "ttz": get_yield_and_error(h_ttz),
        "fakes": get_yield_and_error(h_fakes),
        "rares": get_yield_and_error(h_rares),
        }

print ">>> Writing output histograms for combine"
# print h_rares.GetName()
fout = r.TFile(outfile, "RECREATE")
map(lambda x: x.Write(), [h_data])
map(lambda x: x.Write(), get_variations(h_wz, syst=0.5))
map(lambda x: x.Write(), get_variations(h_ttz, syst=0.5))
map(lambda x: x.Write(), get_variations(h_fakes, syst=0.5))
map(lambda x: x.Write(), get_variations(h_rares, syst=0.5))

# fake signal with 1 event in first bin (arbitrary since we only care about BG only fit)
sig = h_data.Clone("h_sig")
for ix in range(1,sig.GetNbinsX()+1): sig.SetBinContent(ix,int(ix==1))
sig.Write()

fout.Close()

card_txt = """
imax 1
jmax 4
kmax 4
------------
shapes   data_obs   ch1   {rootfile}   h_data    h_data
shapes   sig        ch1   {rootfile}   h_sig     h_$SYSTEMATIC
shapes   wz         ch1   {rootfile}   h_wz      h_$SYSTEMATIC
shapes   ttz        ch1   {rootfile}   h_ttz     h_$SYSTEMATIC
shapes   fakes      ch1   {rootfile}   h_fakes   h_$SYSTEMATIC
shapes   rares      ch1   {rootfile}   h_rares   h_$SYSTEMATIC
------------
bin           ch1
observation   {datacount:.0f}
------------
bin       ch1    ch1    ch1    ch1     ch1
process   sig    wz     ttz    fakes   rares
process   0      1      2      3       4
rate      1      {wzcount:.2f}   {ttzcount:.2f}   {fakescount:.2f}    {rarescount:.2f}
------------
wz      lnN     -     1.5   -     -     -
ttz     lnN     -     -     1.5   -     -
fakes   shape   -     -     -     1.0   -
rares   shape   -     -     -     -     1.0
""".format(
        rootfile=outfile, 
        datacount=h_data.Integral(),
        wzcount=h_wz.Integral(),
        ttzcount=h_ttz.Integral(),
        fakescount=h_fakes.Integral(),
        rarescount=h_rares.Integral(),
        )

print ">>> Writing card"
# write card
with open("card.txt", "w") as fhout:
    fhout.write(card_txt)

# run combine
print ">>> Running combine"
output = commands.getoutput("combine -M MaxLikelihoodFit card.txt --saveNorm --saveWithUncertainties")

print ">>> Examining fit results"
# examine output
fin = r.TFile.Open("mlfit.root");
prefit = fin.Get("norm_prefit")
fit_s = fin.Get("norm_fit_s")
fit_b = fin.Get("norm_fit_b")
iter = fit_b.createIterator()
print "{:>10} {:>21} {:>21} {:>21}".format("process", "prefit", "postfit", "SF")
print "-"*80
while True:
    norm_s = iter.Next()
    if norm_s == None: break;
    norm_b = fit_b.find(norm_s.GetName())
    norm_p = prefit.find(norm_s.GetName()) if prefit else None
    title = norm_s.GetName().split("/")[-1]
    if "sig" in title: continue
    Epostfit = E(norm_b.getVal(), norm_b.getError())
    Eprefit = E(d_prefit[title][0],d_prefit[title][1])
    sf, sferr = Epostfit/Eprefit
    # print "%s \t\t %.2f +- %.2f \t\t %.2f +- %.2f \t\t %.2f +- %.2f" % (title, d_prefit[title][0],d_prefit[title][1],norm_b.getVal(),norm_b.getError(), sf, sferr)
    print "{:>10} {:>15,.2f} +-{:>6,.2f} {:>15,.2f} +-{:>6,.2f} {:>15,.2f} +-{:>6,.2f}".format(title, d_prefit[title][0],d_prefit[title][1],norm_b.getVal(),norm_b.getError(), sf, sferr)
