import os, sys, commands
from pprint import pprint
import glob

from ROOT import *
fxsec = TFile("xsec_susy_13tev.root")
hxsec = fxsec.Get("h_xsec_gluino")
def get_xsec(mglu):
    return hxsec.GetBinContent(hxsec.FindBin(mglu))
# for mglu in range(400,700,25):
#     print mglu, get_xsec(mglu)


# lumi = 12.9
# model = "t5qqqqvv"
# mglu, mlsp = 1000, 700
# kine = "hihi"
# sr = 31
# sig = "fs_%s_m%i_m%i" % (model, mglu, mlsp)
# fname = "logs/log_%s_%s_%s_%i.txt" % (sig, str(lumi), kine, sr)
# print fname

fname = "logs/log_fs_t5qqqqvv_m1000_m700_12.9_hihi_31.txt"

# fnames = glob.glob("logs_bad/*.txt")
# fnames = glob.glob("logs_Aug6/*.txt")
fnames = glob.glob("logs_Aug24/*.txt")

def get_info(fname):
    sig, lumi, kine, sr = fname.split("log_")[-1].replace(".txt","").rsplit("_",3)
    sr = int(sr)
    mglu, mlsp = map(int,sig.replace("_m","_").split("_")[-2:])

    lim_obs, lim_exp, obs, pred, nsig = None, None, None, None, None
    with open(fname, "r") as fhin:
        for line in fhin:
            try:
                line = line.strip()
                if "Observed Limit" in line:
                    lim_obs = float(line.split()[-1])
                elif "Expected 50" in line:
                    lim_exp = float(line.split()[-1])
                elif "Obs:" in line:
                    obs = int(line.split()[-1])
                elif "Pred:" in line:
                    pred = float(line.split()[-1])
                elif sig+": " in line:
                    nsig = float(line.split()[-1])
                elif "NSIGNAL" in line:
                    nsig = float(line.split(":")[-1])
            except:
                pass


    if lim_exp is None: return {}

    xsec = get_xsec(mglu)
    # FIXME
    if nsig < 0.0001: nsig = 0.00001
    # print fname, nsig/xsec, xsec, nsig, lim_obs, lim_exp
    # xsec = 1
    ret = {"mglu": mglu, "mlsp": mlsp, "sr": sr, "kine": kine, "lumi": lumi, 
            "sig": sig, "lim_obs": xsec/nsig*lim_obs, "lim_exp": xsec/nsig*lim_exp, "obs": obs,
            # "sig": sig, "lim_obs": xsec*lim_obs, "lim_exp": xsec*lim_exp, "obs": obs,
            "pred": pred, "nsig": nsig, "fname": fname}
    return ret

d_sigs = {}
for fname in fnames:
    info = get_info(fname)
    if not info: continue

    sig = info["sig"]
    if sig not in d_sigs: d_sigs[sig] = []
    d_sigs[sig].append(info)

    # print info

# sort each mass point in order of decreasing expected limit
for sig in d_sigs:
    d_sigs[sig] = sorted(d_sigs[sig], key=lambda x: x.get("lim_exp",99999.0))
    # d_sigs[sig] = sorted(d_sigs[sig], key=lambda x: -x.get("lim_exp",-99999.0)) # FIXME

# for thing in d_sigs["fs_t5qqqqvv_m1000_m700"]:
#     print thing["sr"], thing["kine"], thing["lim_exp"]


# For analyses with many bins, select a handful of representative points (compressed, uncompressed, high mass, low mass etc) in each relevant simplified model. Show a table of the few most sensitive bins with the following information
# Event yield in this bin
# Expected background in this bin
# Expected upper limit from this bin only
# Observed upper limit from this bin only
# Signal acceptance in this bin

# for sig in d_sigs:
#     print "#### %s #####" % sig
#     # take top 5 most sensitive bins
#     for thing in d_sigs[sig][:10]:
#         print thing["sr"], thing["kine"], thing["obs"], thing["lim_exp"], thing["lim_obs"], thing["nsig"]


# LATEX
import tableMaker as tm
import os

template = r"""
\documentclass{article}
\usepackage{multirow}
\usepackage{slashed}
\usepackage{xspace}
\newcommand{\met}{\slashed{E}_\mathrm{T}}
\newcommand{\mt}{m_\mathrm{T}}
\newcommand{\pt}{p_\mathrm{T}}
\newcommand{\mtmin}{m_{T}^\mathrm{min}}
\newcommand{\Totttt}{\ensuremath{\mbox{T}1\mbox{tttt}}\xspace}
\newcommand{\TfqqqqWW}{\ensuremath{\mbox{T}5\mbox{qqqq}\PW\PW}\xspace}
\newcommand{\eslash}{\ensuremath{\hbox{$E$\kern-0.6em\lower-.05ex\hbox{/}\kern0.10em}}}}
\newcommand{\ETmiss}{\ensuremath{\eslash_{\rm T}}\xspace}
\newcommand{\HT}{\ensuremath{H_\text{T}}\xspace}
\newcommand{\dmtwenty}{$m_{ {\tilde{\chi}^\pm _1} } = m_{ {\tilde{\chi}^0_1} }$ + 20 GeV\xspace}
\newcommand{\dmhalf}{$m_{ {\tilde{\chi}^\pm _1} } = 0.5(m_{ \tilde{g} } + m_{ {\tilde{\chi}^0_1} })$\xspace}
\newcommand{\gev}{GeV~}
\begin{document}
\pagenumbering{gobble}
%s
\end{document}
"""

for sig in d_sigs:
    model = "\\Totttt"
    extra = ""
    if "t5qqqqvv" in sig:
        model = "\\TfqqqqWW"
        extra = " with \\dmhalf "
    if "t5qqqqvv_dm20" in sig:
        model = "\\TfqqqqWW"
        extra = " with \\dmtwenty "

    print sig, model, extra

    mglu = d_sigs[sig][0]["mglu"]
    mlsp = d_sigs[sig][0]["mlsp"]

    buff = ""

    buff += "\\begin{table}[!h] \n"
    buff += "\\begin{center} \n"
    buff += "\\caption{\\label{tab:bins%s} Single bin limits for most sensitive bins for the %s model %s assuming gluino and LSP masses equal to %i and %i GeV, respectively.}\n" % (sig, model, extra, mglu, mlsp)
    buff += "\\begin{tabular}{lccccc} \n"
    buff += "\\hline \n"
    buff += "SR & N_\\mathrm{obs} & N_\\mathrm{bkg} & Expected limit (pb) & Observed limit (pb) & N_\\mathrm{sig} \\\\ \n"
    # buff += "SR & N_\\mathrm{obs} & N_\\mathrm{bkg} & Exp. sig. strength & Obs. sig. strength & N_\\mathrm{sig} \\\\ \n" # FIXME
    buff += "\\hline \n"
    for thing in d_sigs[sig][:10]:
        sr = "HH"
        if thing["kine"] == "hilow": sr = "HL"
        elif thing["kine"] == "lowlow": sr = "LL"
        sr += str(thing["sr"])
        buff += "%s & %i & %.2f & %.2f & %.2f & %.2f \\\\ \n" % (sr, thing["obs"], thing["pred"], thing["lim_exp"], thing["lim_obs"], thing["nsig"])
    buff += "\\hline \n"
    buff += "\\end{tabular} \n"
    buff += "\\end{center} \n"
    buff += "\\end{table} \n"

    tm.makePDF(template % buff, "tables/table_%s.tex" % sig)
    

os.system("niceplots tables tables_sig_Aug24")
