import ROOT as r
import os
import sys
import math
import commands
import glob
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

def get_variations(h1, syst=0.0, do_var=False):
    for ix in range(1,h1.GetNbinsX()+1):
        if not do_var:
            hUp = r.TH1F("bin%i_%sUp" % (ix,h1.GetName()), "", 1, 0, 1)
            hDown = r.TH1F("bin%i_%sDown" % (ix,h1.GetName()), "", 1, 0, 1)
        hCent = r.TH1F("bin%i_%s" % (ix,h1.GetName()), "", 1, 0, 1)
        stat = h1.GetBinError(ix)
        cont = h1.GetBinContent(ix)
        unc = math.sqrt(stat*stat + (syst*cont)**2)
        if not do_var:
            hUp.SetBinContent(1, cont+unc)
            hDown.SetBinContent(1, max(cont-unc, 0.0001))
        hCent.SetBinContent(1, cont)
        if not do_var:
            yield hUp
            yield hDown
        yield hCent

def get_variations_systs(h1, syst=0.0):
    for ix in range(1,h1.GetNbinsX()+1):
        hUp = r.TH1F("bin%i_%sUp" % (ix,h1.GetName()), "", 1, 0, 1)
        hDown = r.TH1F("bin%i_%sDown" % (ix,h1.GetName()), "", 1, 0, 1)
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




def get_sfs(infile, lnNsig=2.0, lnNbg=1.5, shapeUnc=0.1):
    outfile = "forCard.root"
    card_filename = "card.txt"
    variations = ["btag","jes"]
    # variations = []
    procs = ["data", "wz", "ttz", "fakes", "rares"]
    # procs = ["data", "wz", "ttz", "fakes"]

    variationsud = sum([[v+"_up",v+"_dn"] for v in variations],[])
    # print variations

    files = {}
    files["central"] = r.TFile(infile)

    for var in variationsud:
        # print "%s_%s_%s.root" % (infile.split(".root")[0], var, ud)
        files[var] = r.TFile("%s_%s.root" % (infile.split(".root")[0], var))

    # print files


    # return 

    print ">>> Reading input histograms from %s" % infile


    procs_nodata = procs[1:]
    procs_signodata = ["sig"] + procs_nodata

    hnames = {}

    for var in ["central"] + variationsud:
        keys = files[var].GetListOfKeys()
        histnames = [key.GetName() for key in keys if key.ReadObj().InheritsFrom(r.TH1F.Class())]
        hnames[var] = {}
        hnames[var]["data"] = [hn for hn in histnames if "Data" in hn]
        hnames[var]["wz"] = [hn for hn in histnames if "WZ" in hn]
        hnames[var]["ttz"] = [hn for hn in histnames if "ttZ" in hn]
        hnames[var]["fakes"] = [hn for hn in histnames if "Fakes" in hn]
        hnames[var]["rares"] = [hn for hn in histnames if ("Fakes" not in hn) and ("WZ" not in hn) and ("Data" not in hn) and ("ttZ" not in hn)]

    hists = {}
    # for k,v in hnames.items():
    for proc in procs:
        # tmp = [files["central"].Get(hn) for hn in hnames["central"][proc]]
        # hists["central"][proc] = tmp[0].Clone("h_"+proc)
        # for h in tmp[1:]: hists["central"][proc].Add(h)
        for var in ["central"] + variationsud:
            if var not in hists: hists[var] = {}
            tmp = [files[var].Get(hn) for hn in hnames[var][proc]]
            name = "h_"+proc
            if "cent" not in var: name += var.replace("_up","Up").replace("_dn","Down")
            # print name
            hists[var][proc] = tmp[0].Clone(name)
            for h in tmp[1:]: hists[var][proc].Add(h)


    d_prefit = {}
    for proc in procs:
        d_prefit[proc] = get_bin_yields_and_errors(hists["central"][proc])


    print ">>> Writing output histograms for combine into %s" % outfile
    fout = r.TFile(outfile, "RECREATE")
    for proc in procs:
        for var in ["central"] + variationsud:
            map(lambda x: x.Write(), get_variations(hists[var][proc], syst=(shapeUnc if "data" not in proc.lower() else 0.0), do_var=not("cent" in var)))

    # fake signal with 1 event in each bin (arbitrary since we only care about BG only fit)
    hists["central"]["sig"] = hists["central"]["data"].Clone("h_sig")
    for ix in range(1,hists["central"]["sig"].GetNbinsX()+1): hists["central"]["sig"].SetBinContent(ix,1.0)
    map(lambda x: x.Write(), get_variations(hists["central"]["sig"], syst=0.0))

    Nbins = hists["central"]["sig"].GetNbinsX()
    Nproc = len(procs)
    counts  = {}
    for proc in procs_signodata + ["data"]:
        counts[proc] = [hists["central"][proc].GetBinContent(ix) for ix in range(1,Nbins+1)]

    bin_str = " ".join([("ch%i "%i) * Nproc for i in range(1,Nbins+1)])
    proc1_str = (" ".join(procs_signodata) + " ") * Nbins
    proc2_str = " ".join([" ".join(map(str,range(0,Nproc))) for _ in range(0,Nbins)])
    rate_list = sum(map(list,zip(*[counts[proc] for proc in procs_signodata])),[])

    buff = ""
    buff += "imax %i\n" % (Nbins)
    buff += "jmax %i\n" % (Nproc-1)
    buff += "kmax %i\n" % ((Nbins+1+len(variations))*(Nproc-1))
    buff += "------------\n"

    for i in range(1,Nbins+1):
        # for thing in ["data_obs","sig","wz","ttz","fakes","rares"]:
        for thing in ["data_obs"] + procs_signodata:
            buff += "shapes %s ch%i %s bin%i_h_%s bin%i_h_%s\n" % \
                    (thing, i, outfile, i, thing.replace("_obs",""), i, "data" if "data" in thing else "$SYSTEMATIC")

    buff += "------------\n"

    buff += "bin " + " ".join(["ch%i" % i for i in range(1,Nbins+1)]) + "\n"
    buff += "observation " + " ".join(["%.2f" % e for e in counts["data"]]) + "\n"

    buff += "------------\n"

    buff += "bin " + bin_str + "\n"
    buff += "process " + proc1_str + "\n"
    buff += "process " + proc2_str + "\n"
    buff += "rate " + " ".join(["%.2f" % e for e in rate_list]) + "\n"

    buff += "------------\n"

    # for ithing,thing in enumerate(["wz","ttz","fakes","rares"]):
    for ithing,thing in enumerate(procs_nodata):
        norm = lnNbg
        if thing in ["wz","ttz"]: norm = lnNsig
        for ibin in range(Nbins):
            buff += get_nuisance_line(thing, "shape", len(rate_list), 1.0, [ibin*(Nproc)+ithing+1])
        buff += get_nuisance_line(thing, "lnN", len(rate_list), norm, range(ithing+1,(Nbins+1)*Nproc,Nproc))
        for var in variations:
            buff += get_nuisance_line(thing+var, "shape", len(rate_list), 1.0, range(ithing+1,(Nbins+1)*Nproc,Nproc))

    card_txt = buff


    fout.Close()

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
    # print "{:>10} {:>21} {:>21} {:>21}".format("process", "prefit", "postfit", "SF")
    # print "-"*80
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
        # print "{:>10} {:>15,.2f} +-{:>6,.2f} {:>15,.2f} +-{:>6,.2f} {:>15,.2f} +-{:>6,.2f}".format(
        #         title, 
        #         p_val, p_err,
        #         norm_b.getVal(),norm_b.getError(),
        #         sf, sferr
        #         )
        d_sfs[title] = (sf, sferr)

    print "total SFs:"
    d_sfs["totals"] = {}
    for proc in d_cerrs:
        sf = sum(d_cerrs[proc]["postfit"])/sum(d_cerrs[proc]["prefit"])
        d_sfs["totals"][proc.split("/")[-1]] = (sf[0],sf[1])
        print "{:>10} {:>15,.2f} +-{:>6,.2f}".format(proc, sf[0], sf[1])


    return d_sfs

def make_single_bin_hist(fname, which_bin):
    fin = r.TFile(fname)
    fout = r.TFile("onebin_"+fname, "RECREATE")
    for key in fin.GetListOfKeys():
        if not key.ReadObj().InheritsFrom(r.TH1F.Class()): continue
        name = key.GetName()
        hist = fin.Get(name)
        h_temp = hist.Clone(name)
        h_temp.SetBins(1,0,1)
        h_temp.Reset()
        # h_temp = r.TH1F(name, name, 1, 0, 1)
        h_temp.SetBinContent(1, hist.GetBinContent(which_bin))
        h_temp.SetBinError(1, hist.GetBinError(which_bin))
        h_temp.Write()
    fout.Close()
    fin.Close()

if __name__ == "__main__":

    for fname in glob.glob("h1D_nbtags*root"):
        make_single_bin_hist(fname, 3)
    infile = "onebin_h1D_nbtags.root"
    print get_sfs(infile)

    # infile = "h1D_nbtags.root"
    # print get_sfs(infile)

