import ROOT as r
import os
import sys
import math
import commands
from errors import E

def get_nuisance_line(name, typ, ndashes, val, which_vals):
    tmp = ""
    tmp += "%s %s" % (name, typ)
    for idash in range(ndashes):
        if idash in which_vals:
            tmp += " %.1f " % val
        else:
            tmp += " - "
    return tmp+"\n"

# def get_variations(h1, syst=0.0):
#     hUp = h1.Clone(h1.GetName()+"Up")
#     hDown = h1.Clone(h1.GetName()+"Down")
#     for ix in range(1,h1.GetNbinsX()+1):
#         stat = h1.GetBinError(ix)
#         cont = h1.GetBinContent(ix)
#         unc = math.sqrt(stat*stat + (syst*cont)**2)
#         hUp.SetBinContent(ix, cont+unc)
#         hDown.SetBinContent(ix, cont-unc)
#     return hUp,h1,hDown

# def get_variations(h1, syst=0.0):
#     yield h1
#     for ix in range(1,h1.GetNbinsX()+1):
#         hUp = h1.Clone("bin%i_%sUp" % (ix,h1.GetName()))
#         hDown = h1.Clone("bin%i_%sDown" % (ix,h1.GetName()))
#         stat = h1.GetBinError(ix)
#         cont = h1.GetBinContent(ix)
#         unc = math.sqrt(stat*stat + (syst*cont)**2)
#         hUp.SetBinContent(ix, cont+unc)
#         hDown.SetBinContent(ix, cont-unc)
#         hUp.Scale(h1.Integral()/hUp.Integral())
#         hDown.Scale(h1.Integral()/hDown.Integral())
#         yield hUp
#         yield hDown

def get_variations(h1, syst=0.0):
    for ix in range(1,h1.GetNbinsX()+1):
        hUp = r.TH1F("bin%i_%sUp" % (ix,h1.GetName()), "", 1, 0, 1)
        hDown = r.TH1F("bin%i_%sDown" % (ix,h1.GetName()), "", 1, 0, 1)
        hCent = r.TH1F("bin%i_%s" % (ix,h1.GetName()), "", 1, 0, 1)
        stat = h1.GetBinError(ix)
        cont = h1.GetBinContent(ix)
        unc = math.sqrt(stat*stat + (syst*cont)**2)
        hUp.SetBinContent(1, cont+unc)
        hDown.SetBinContent(1, max(cont-unc, 0.0001))
        hCent.SetBinContent(1, cont)
        yield hUp
        yield hCent
        yield hDown

# def get_yield_and_error(h1):
#     err = r.Double(0.0)
#     integral = h1.IntegralAndError(0,-1,err)
#     return float(integral), float(err)
def get_bin_yields_and_errors(h1):
    return [[h1.GetBinContent(ix),h1.GetBinError(ix)] for ix in range(1,h1.GetNbinsX()+1)]




def get_sfs(infile, lnNsig=2.0, lnNbg=1.5, shapeUnc=0.3):
    outfile = "forCard.root"
    card_filename = "card.txt"
    f1 = r.TFile(infile)

    print ">>> Reading input histograms from %s" % infile
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

    # d_prefit = {
    #         "data": get_yield_and_error(h_data),
    #         "wz": get_yield_and_error(h_wz),
    #         "ttz": get_yield_and_error(h_ttz),
    #         "fakes": get_yield_and_error(h_fakes),
    #         "rares": get_yield_and_error(h_rares),
    #         }

    d_prefit = {
            "data": get_bin_yields_and_errors(h_data),
            "wz": get_bin_yields_and_errors(h_wz),
            "ttz": get_bin_yields_and_errors(h_ttz),
            "fakes": get_bin_yields_and_errors(h_fakes),
            "rares": get_bin_yields_and_errors(h_rares),
            }


    print ">>> Writing output histograms for combine into %s" % outfile
    # print h_rares.GetName()
    fout = r.TFile(outfile, "RECREATE")
    # map(lambda x: x.Write(), [h_data])
    map(lambda x: x.Write(), get_variations(h_data))
    map(lambda x: x.Write(), get_variations(h_wz, syst=shapeUnc))
    map(lambda x: x.Write(), get_variations(h_ttz, syst=shapeUnc))
    map(lambda x: x.Write(), get_variations(h_fakes, syst=shapeUnc))
    map(lambda x: x.Write(), get_variations(h_rares, syst=shapeUnc))

    # fake signal with 1 event in each bin (arbitrary since we only care about BG only fit)
    sig = h_data.Clone("h_sig")
    for ix in range(1,sig.GetNbinsX()+1): sig.SetBinContent(ix,1.0)
    map(lambda x: x.Write(), get_variations(sig, syst=0.0))


    Nbins = sig.GetNbinsX()
    Nproc = 5
    counts_data = [h_data.GetBinContent(ix) for ix in range(1,Nbins+1)]
    counts_wz = [h_wz.GetBinContent(ix) for ix in range(1,Nbins+1)]
    counts_ttz = [h_ttz.GetBinContent(ix) for ix in range(1,Nbins+1)]
    counts_fakes = [h_fakes.GetBinContent(ix) for ix in range(1,Nbins+1)]
    counts_rares = [h_rares.GetBinContent(ix) for ix in range(1,Nbins+1)]
    counts_sig = [sig.GetBinContent(ix) for ix in range(1,Nbins+1)]

    bin_str = " ".join([("ch%i "%i) * Nproc for i in range(1,Nbins+1)])
    proc1_str = "sig wz ttz fakes rares " * (Nbins)
    proc2_str = " ".join([" ".join(map(str,range(0,Nbins+1))) for _ in range(1,Nbins+1)])
    rate_list = sum(map(list,zip(*[counts_sig, counts_wz,counts_ttz,counts_fakes,counts_rares])),[])

    buff = ""
    buff += "imax 4\n"
    buff += "jmax 4\n"
    buff += "kmax %i\n" % (Nbins*Nproc)
    buff += "------------\n"

    for i in range(1,Nbins+1):
        for thing in ["data_obs","sig","wz","ttz","fakes","rares"]:
            buff += "shapes %s ch%i %s bin%i_h_%s bin%i_h_%s\n" % \
                    (thing, i, outfile, i, thing.replace("_obs",""), i, "data" if "data" in thing else "$SYSTEMATIC")

    buff += "------------\n"

    buff += "bin " + " ".join(["ch%i" % i for i in range(1,Nbins+1)]) + "\n"
    buff += "observation " + " ".join(["%.2f" % e for e in counts_data]) + "\n"

    buff += "------------\n"

    buff += "bin " + bin_str + "\n"
    buff += "process " + proc1_str + "\n"
    buff += "process " + proc2_str + "\n"
    buff += "rate " + " ".join(["%.2f" % e for e in rate_list]) + "\n"

    buff += "------------\n"

    for ithing,thing in enumerate(["wz","ttz","fakes","rares"]):
        norm = lnNbg
        if thing in ["wz","ttz"]: norm = lnNsig
        for ibin in range(Nbins):
            buff += get_nuisance_line(thing, "shape", len(rate_list), 1.0, [ibin*(Nproc)+ithing+1])
        buff += get_nuisance_line(thing, "lnN", len(rate_list), norm, range(ithing+1,(Nbins+1)*Nproc,Nproc))

    card_txt = buff


    fout.Close()

    # sys.exit()


    # card_txt = """
    # imax 1 number of channels
    # jmax 4 number of nonsignal processes
    # kmax 4 number of nuisances
    # ------------
    # shapes   data_obs   ch1   {rootfile}   h_data    h_data
    # shapes   sig        ch1   {rootfile}   h_sig     h_$SYSTEMATIC
    # shapes   wz         ch1   {rootfile}   h_wz      h_$SYSTEMATIC
    # shapes   ttz        ch1   {rootfile}   h_ttz     h_$SYSTEMATIC
    # shapes   fakes      ch1   {rootfile}   h_fakes   h_$SYSTEMATIC
    # shapes   rares      ch1   {rootfile}   h_rares   h_$SYSTEMATIC
    # ------------
    # bin           ch1
    # observation   {datacount:.0f}
    # ------------
    # bin       ch1    ch1    ch1    ch1     ch1
    # process   sig    wz     ttz    fakes   rares
    # process   0      1      2      3       4
    # rate      1      {wzcount:.2f}   {ttzcount:.2f}   {fakescount:.2f}    {rarescount:.2f}
    # ------------
    # wz      lnN   -     2.0   -     -     -
    # ttz     lnN   -     -     2.0   -     -
    # fakes   lnN   -     -     -     1.5   -
    # rares   lnN   -     -     -     -     1.5
    # """.format(
    #         rootfile=outfile, 
    #         datacount=h_data.Integral(),
    #         wzcount=h_wz.Integral(),
    #         ttzcount=h_ttz.Integral(),
    #         fakescount=h_fakes.Integral(),
    #         rarescount=h_rares.Integral(),
    #         )

    # wz      lnN     -     2.0   -     -     -
    # ttz     lnN     -     -     2.0   -     -

    print ">>> Writing card %s" % card_filename
    # write card
    with open(card_filename, "w") as fhout:
        fhout.write(card_txt)

    # run combine
    print ">>> Running combine with card %s" % card_filename
    output = commands.getoutput("combine -M MaxLikelihoodFit %s --saveNorm --saveWithUncertainties" % card_filename)
    if "Done in" in output:
        print ">>> Combine finished successfully"
    else:
        print ">>> [!] ERROR with combine. Output below:"
        print "-"*40
        print output
        print "-"*40
        sys.exit()

    print ">>> Examining fit results"
    # examine output
    fin = r.TFile.Open("mlfit.root");
    prefit = fin.Get("norm_prefit")
    fit_s = fin.Get("norm_fit_s")
    fit_b = fin.Get("norm_fit_b")
    iter = fit_b.createIterator()
    print "{:>10} {:>21} {:>21} {:>21}".format("process", "prefit", "postfit", "SF")
    print "-"*80
    d_sfs = {}
    d_cerrs = {}
    while True:
        norm_s = iter.Next()
        if norm_s == None: break;
        norm_b = fit_b.find(norm_s.GetName())
        norm_p = prefit.find(norm_s.GetName()) if prefit else None
        # title = norm_s.GetName().split("/")[-1]
        title = norm_s.GetName()
        binzi = int(title.split("/")[0].split("ch")[-1])-1 # zero indexed
        name = title.split("/")[-1]
        if "sig" in title: continue
        p_val, p_err = d_prefit[name][binzi][0],d_prefit[name][binzi][1]
        try:
            Epostfit = E(norm_b.getVal(), norm_b.getError())
            Eprefit = E(p_val, p_err)
            if name not in  d_cerrs: d_cerrs[name] = {"prefit": [], "postfit": []}
            d_cerrs[name]["prefit"].append(Eprefit)
            d_cerrs[name]["postfit"].append(Epostfit)

            sf, sferr = Epostfit/Eprefit
        except:
            sf, sferr = -1.0, 0.0
        # print "%s \t\t %.2f +- %.2f \t\t %.2f +- %.2f \t\t %.2f +- %.2f" % (title, d_prefit[title][0],d_prefit[title][1],norm_b.getVal(),norm_b.getError(), sf, sferr)
        print "{:>10} {:>15,.2f} +-{:>6,.2f} {:>15,.2f} +-{:>6,.2f} {:>15,.2f} +-{:>6,.2f}".format(
                title, 
                p_val, p_err,
                norm_b.getVal(),norm_b.getError(),
                sf, sferr
                )
        d_sfs[title] = (sf, sferr)

    print "total SFs:"
    d_sfs["totals"] = {}
    for proc in d_cerrs:
        sf = sum(d_cerrs[proc]["postfit"])/sum(d_cerrs[proc]["prefit"])
        d_sfs["totals"][proc.split("/")[-1]] = (sf[0],sf[1])
        print "{:>10} {:>15,.2f} +-{:>6,.2f}".format(proc, sf[0], sf[1])


    return d_sfs

if __name__ == "__main__":

    infile = "h1D_nbtags.root"
    print get_sfs(infile)

