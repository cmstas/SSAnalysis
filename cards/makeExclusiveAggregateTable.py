# import ROOT as r
import os
import glob
import re
import commands
from math import log10, floor

def round_sig(x, sig=2):
    if x < 0.001: return x
    return round(x, sig-int(floor(log10(x)))-1)

def transform_err(x):
    if "$pm$" not in x: return x

    val, err = x.split("$pm$")
    val = float(val)
    err = float(err)
    val = round_sig(val,3)
    err = round_sig(err,2)
    return "{} $pm$ {}".format(val,err)

if __name__ == "__main__":

    # in_dir = "v9.06_Mar6_35p9_reminiaod_testagg"
    in_dir = "v9.06_Mar6_35p9_reminiaodfilt_nomhiggs_agg"
    lumi = "35.9"

    cmd = """root -b -q -l makeTable.C'(\"{in_dir}\",\"{lumi}\")' """.format(in_dir=in_dir,lumi=lumi)
    stat, output = commands.getstatusoutput(cmd)

    found = False
    hh = []
    hl = []
    ll = []
    for line in output.splitlines():
        if "Event yields in signal regions" in line: found = True
        if not found: continue
        # print line
        if not line.startswith("SR"): continue

        line = line.replace("\\","")
        parts = map(lambda x: x.strip(), line.split("&"))
        sr = parts[0]
        hh_exp = parts[1]
        hh_obs = parts[2]
        hl_exp = parts[3]
        hl_obs = parts[4]
        ll_exp = parts[5]
        ll_obs = parts[6]

        hh_exp = transform_err(hh_exp)
        hl_exp = transform_err(hl_exp)
        ll_exp = transform_err(ll_exp)

        # print hh_exp,hh_obs,hl_exp,hl_obs,ll_exp,ll_obs
        if hh_exp and not hh_exp.startswith("0.0"): hh.append([hh_exp,hh_obs])
        if hl_exp and not hl_exp.startswith("0.0"): hl.append([hl_exp,hl_obs])
        if ll_exp and not ll_exp.startswith("0.0"): ll.append([ll_exp,ll_obs])

    print r"""
\begin{table}
\begin{center}
\footnotesize
\caption{Event yields in exclusive aggregate regions.}
\label{tab:yieldsExclAgg}
\begin{tabular}{c|c|c}\hline
    & $N_\mathrm{exp}$ &  $N_\mathrm{obs}$  \\
\hline\hline """

    for isr, (exp,obs) in enumerate(hh+hl+ll):

        exp = exp.replace("pm","\\pm")
        print "{0:3} & {1:>20} & {2:>10} \\\\".format(isr+1,exp,obs)

    print r"""\hline
\end{tabular}
\end{center}
\end{table}
    """

    
