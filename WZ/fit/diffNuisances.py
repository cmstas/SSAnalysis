#!/usr/bin/env python
import re
from sys import argv, stdout, stderr, exit
from optparse import OptionParser
import math

# import ROOT with a fix to get batch mode (http://root.cern.ch/phpBB3/viewtopic.php?t=3198)
hasHelp = False
for X in ("-h", "-?", "--help"):
    if X in argv:
        hasHelp = True
        argv.remove(X)
argv.append( '-b-' )
import ROOT
ROOT.gROOT.SetBatch(True)
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
argv.remove( '-b-' )
if hasHelp: argv.append("-h")

parser = OptionParser(usage="usage: %prog [options] in.root  \nrun with --help to get list of options")
parser.add_option("--vtol", "--val-tolerance", dest="vtol", default=0.30, type="float", help="Report nuisances whose value changes by more than this amount of sigmas")
parser.add_option("--stol", "--sig-tolerance", dest="stol", default=0.10, type="float", help="Report nuisances whose sigma changes by more than this amount")
parser.add_option("--vtol2", "--val-tolerance2", dest="vtol2", default=2.0, type="float", help="Report severely nuisances whose value changes by more than this amount of sigmas")
parser.add_option("--stol2", "--sig-tolerance2", dest="stol2", default=0.50, type="float", help="Report severely nuisances whose sigma changes by more than this amount")
parser.add_option("-a", "--all",      dest="all",    default=False,  action="store_true", help="Print all nuisances, even the ones which are unchanged w.r.t. pre-fit values.")
parser.add_option("-A", "--absolute", dest="abs",    default=False,  action="store_true", help="Report also absolute values of nuisance values and errors, not only the ones normalized to the input sigma")
parser.add_option("-p", "--poi",      dest="poi",    default="r",    type="string",  help="Name of signal strength parameter (default is 'r' as per text2workspace.py)")
parser.add_option("-f", "--format",   dest="format", default="text", type="string",  help="Output format ('text', 'latex', 'twiki'")
parser.add_option("-g", "--histogram", dest="plotfile", default=None, type="string", help="If true, plot the pulls of the nuisances to the given file.")

(options, args) = parser.parse_args()
if len(args) == 0:
    parser.print_usage()
    exit(1)

file = ROOT.TFile(args[0])
if file == None: raise RuntimeError, "Cannot open file %s" % args[0]
fit_s  = file.Get("fit_s")
fit_b  = file.Get("fit_b")
prefit = file.Get("nuisances_prefit")
if fit_s == None or fit_s.ClassName()   != "RooFitResult": raise RuntimeError, "File %s does not contain the output of the signal fit 'fit_s'"     % args[0]
if fit_b == None or fit_b.ClassName()   != "RooFitResult": raise RuntimeError, "File %s does not contain the output of the background fit 'fit_b'" % args[0]
if prefit == None or prefit.ClassName() != "RooArgSet":    raise RuntimeError, "File %s does not contain the prefit nuisances 'nuisances_prefit'"  % args[0]

isFlagged = {}
table = {}
fpf_b = fit_b.floatParsFinal()
fpf_s = fit_s.floatParsFinal()
pulls = []

nuis_p_i=0
# Also make histograms for pull distributions:
size = 0
for i in range(fpf_s.getSize()):
    nuis_s = fpf_s.at(i)
    name   = nuis_s.GetName();
#    if (name.find("CRstat")!=-1):
#        continue;
    size = size + 1

#hist_fit_b  = ROOT.TH1F("prefit_fit_b"   ,"B-only fit Nuisances;;#theta ",prefit.getSize(),0,prefit.getSize())
#hist_fit_s  = ROOT.TH1F("prefit_fit_s"   ,"S+B fit Nuisances   ;;#theta ",prefit.getSize(),0,prefit.getSize())
#hist_prefit = ROOT.TH1F("prefit_nuisances","Prefit Nuisances    ;;#theta ",prefit.getSize(),0,prefit.getSize())
hist_fit_b  = ROOT.TH1F("prefit_fit_b"   ,"B-only fit Nuisances;;#theta ",size,0,size)
hist_fit_s  = ROOT.TH1F("prefit_fit_s"   ,"S+B fit Nuisances   ;;#theta ",size,0,size)
hist_prefit = ROOT.TH1F("prefit_nuisances","Prefit Nuisances    ;;#theta ",size,0,size)
prev_shortname = ""
boundaries = []
for i in range(fpf_s.getSize()):
    nuis_s = fpf_s.at(i)
    name   = nuis_s.GetName();
#    if (name.find("CRstat")!=-1):
#        continue;
    nuis_b = fpf_b.find(name)
    nuis_p = prefit.find(name)
    row = []
    flag = False;
    mean_p, sigma_p = 0,0
    if nuis_p == None:
        if not options.abs: continue
        row += [ "[%.2f, %.2f]" % (nuis_s.getMin(), nuis_s.getMax()) ]
    else:
        mean_p, sigma_p = (nuis_p.getVal(), nuis_p.getError())
        if options.abs: row += [ "%.2f +/- %.2f" % (nuis_p.getVal(), nuis_p.getError()) ]
    for fit_name, nuis_x in [('b', nuis_b), ('s',nuis_s)]:
        if nuis_x == None:
            row += [ " n/a " ]
        else:
            row += [ "%+.2f +/- %.2f" % (nuis_x.getVal(), nuis_x.getError()) ]
	
            if nuis_p != None:
	        if options.plotfile: 
                    # NJA
                  shortname = nuis_x.GetName()
                  for thing in ["hihi", "hilow", "lowlow"]:
                      shortname = shortname.replace(thing,"")
                  while shortname[-1].isdigit() or shortname[-1] == "_":
                      shortname = shortname[:-1]
                  if prev_shortname != shortname:
                      prev_shortname = shortname
                      boundaries.append([nuis_p_i, shortname])
                    
	          if fit_name=='b':
	    	    nuis_p_i+=1
                    if(mean_p == 0):
                        hist_fit_b.SetBinContent(nuis_p_i,(nuis_x.getVal()-mean_p))
                        hist_fit_b.SetBinError(nuis_p_i,nuis_x.getError()/sigma_p)
                    else:
                        hist_fit_b.SetBinContent(nuis_p_i,(nuis_x.getVal()-mean_p)/sigma_p)
                        hist_fit_b.SetBinError(nuis_p_i,nuis_x.getError()/sigma_p)
#	      	    hist_fit_b.GetXaxis().SetBinLabel(nuis_p_i,name)
	          if fit_name=='s':
                      if(mean_p == 0):
                          hist_fit_s.SetBinContent(nuis_p_i,(nuis_x.getVal()-mean_p))
                          hist_fit_s.SetBinError(nuis_p_i,nuis_x.getError()/sigma_p)
                      else:
                          hist_fit_s.SetBinContent(nuis_p_i,(nuis_x.getVal()-mean_p)/sigma_p)
                          hist_fit_s.SetBinError(nuis_p_i,nuis_x.getError()/sigma_p)
#	      	    hist_fit_s.GetXaxis().SetBinLabel(nuis_p_i,name)
		  hist_prefit.SetBinContent(nuis_p_i,mean_p-mean_p)
		  hist_prefit.SetBinError(nuis_p_i,sigma_p/sigma_p)
#	      	  hist_prefit.GetXaxis().SetBinLabel(nuis_p_i,name)

                valShift = (nuis_x.getVal() - mean_p)/sigma_p
                if fit_name == 'b':
                    pulls.append(valShift)
                sigShift = nuis_x.getError()/sigma_p
                if options.abs:
                    row[-1] += " (%+4.2fsig, %4.2f)" % (valShift, sigShift)
                else:
                    row[-1] = " %+4.2f, %4.2f" % (valShift, sigShift)
                if (abs(valShift) > options.vtol2 or abs(sigShift-1) > options.stol2):
                    isFlagged[(name,fit_name)] = 2
                    flag = True
                elif (abs(valShift) > options.vtol  or abs(sigShift-1) > options.stol):
                    if options.all: isFlagged[(name,fit_name)] = 1
                    flag = True
                elif options.all:
                    flag = True
    row += [ "%+4.2f"  % fit_s.correlation(name, options.poi) ]
    if flag or options.all: table[name] = row
    # print name


fmtstring = "%-40s     %15s    %15s  %10s"
highlight = "*%s*"
morelight = "!%s!"
pmsub, sigsub = None, None
if options.format == 'text':
    if options.abs:
        fmtstring = "%-40s     %15s    %30s    %30s  %10s"
        print fmtstring % ('name', 'pre fit', 'b-only fit', 's+b fit', 'rho')
    else:
        print fmtstring % ('name', 'b-only fit', 's+b fit', 'rho')

elif options.format == 'latex':
    pmsub  = (r"(\S+) \+/- (\S+)", r"$\1 \\pm \2$")
    sigsub = ("sig", r"$\\sigma$")
    highlight = "\\textbf{%s}"
    morelight = "{{\\color{red}\\textbf{%s}}}"
    if options.abs:
        fmtstring = "%-40s &  %15s & %30s & %30s & %6s \\\\"
        print "\\begin{tabular}{|l|r|r|r|r|} \\hline ";
        print (fmtstring % ('name', 'pre fit', '$b$-only fit', '$s+b$ fit', r'$\rho(\theta, \mu)$')), " \\hline"
    else:
        fmtstring = "%-40s &  %15s & %15s & %6s \\\\"
        print "\\begin{tabular}{|l|r|r|r|} \\hline ";
        #what = r"$(x_\text{out} - x_\text{in})/\sigma_{\text{in}}$, $\sigma_{\text{out}}/\sigma_{\text{in}}$"
        what = r"\Delta x/\sigma_{\text{in}}$, $\sigma_{\text{out}}/\sigma_{\text{in}}$"
        print  fmtstring % ('',     '$b$-only fit', '$s+b$ fit', '')
        print (fmtstring % ('name', what, what, r'$\rho(\theta, \mu)$')), " \\hline"
elif options.format == 'twiki':
    pmsub  = (r"(\S+) \+/- (\S+)", r"\1 &plusmn; \2")
    sigsub = ("sig", r"&sigma;")
    highlight = "<b>%s</b>"
    morelight = "<b style='color:red;'>%s</b>"
    if options.abs:
        fmtstring = "| <verbatim>%-40s</verbatim>  | %-15s  | %-30s  | %-30s   | %-15s  |"
        print "| *name* | *pre fit* | *b-only fit* | *s+b fit* | "
    else:
        fmtstring = "| <verbatim>%-40s</verbatim>  | %-15s  | %-15s | %-15s  |"
        print "| *name* | *b-only fit* | *s+b fit* | *corr.* |"
elif options.format == 'html':
    pmsub  = (r"(\S+) \+/- (\S+)", r"\1 &plusmn; \2")
    sigsub = ("sig", r"&sigma;")
    highlight = "<b>%s</b>"
    morelight = "<strong>%s</strong>"
    print """
<html><head><title>Comparison of nuisances</title>
<style type="text/css">
    td, th { border-bottom: 1px solid black; padding: 1px 1em; }
    td { font-family: 'Consolas', 'Courier New', courier, monospace; }
    strong { color: red; font-weight: bolder; }
</style>
</head><body style="font-family: 'Verdana', sans-serif; font-size: 10pt;"><h1>Comparison of nuisances</h1>
<table>
"""
    if options.abs:
        print "<tr><th>nuisance</th><th>pre fit</th><th>background fit </th><th>signal fit</th><th>correlation</th></tr>"
        fmtstring = "<tr><td><tt>%-40s</tt> </td><td> %-15s </td><td> %-30s </td><td> %-30s </td><td> %-15s </td></tr>"
    else:
        what = "&Delta;x/&sigma;<sub>in</sub>, &sigma;<sub>out</sub>/&sigma;<sub>in</sub>";
        print "<tr><th>nuisance</th><th>background fit<br/>%s </th><th>signal fit<br/>%s</th><th>&rho;(&mu;, &theta;)</tr>" % (what,what)
        fmtstring = "<tr><td><tt>%-40s</tt> </td><td> %-15s </td><td> %-15s </td><td> %-15s </td></tr>"

names = table.keys()
names.sort()
highlighters = { 1:highlight, 2:morelight };
# print names
for n in names:
    v = table[n]

     # # FIXME -- SS-specific
    # if "_stat" in n: continue
    # if "isr" == n: continue
    # if "bshlt" == n: continue
    # if "fs_lep" in n: continue

    if options.format == "latex": n = n.replace(r"_", r"\_")
    if pmsub  != None: v = [ re.sub(pmsub[0],  pmsub[1],  i) for i in v ]
    if sigsub != None: v = [ re.sub(sigsub[0], sigsub[1], i) for i in v ]
    if (n,'b') in isFlagged: v[-3] = highlighters[isFlagged[(n,'b')]] % v[-3]
    if (n,'s') in isFlagged: v[-2] = highlighters[isFlagged[(n,'s')]] % v[-2]
    if options.abs:
       print fmtstring % (n, v[0], v[1], v[2], v[3])
    else:
       print fmtstring % (n, v[0], v[1], v[2])

if options.format == "latex":
    print " \\hline\n\end{tabular}"
elif options.format == "html":
    print "</table></body></html>"

def getGraph(hist,shift):

   gr = ROOT.TGraphErrors()
   gr.SetName(hist.GetName())
   for i in range(hist.GetNbinsX()):
     x = hist.GetBinCenter(i+1)+shift
     y = hist.GetBinContent(i+1)
     e = hist.GetBinError(i+1)
     gr.SetPoint(i,x,y)
     gr.SetPointError(i,float(abs(shift))*0.8,e)
   return gr

if options.plotfile:
    import ROOT
    fout = ROOT.TFile(options.plotfile,"RECREATE")
    ROOT.gROOT.SetStyle("Plain")
    ROOT.gStyle.SetOptFit(1)
    ROOT.gStyle.SetOptStat("sirmen")
    histogram = ROOT.TH1F("pulls", "Pulls", 60, -3, 3)
    for pull in pulls:
        histogram.Fill(pull)
    canvas = ROOT.TCanvas("pulls", "pulls", 800, 800)
    histogram.GetXaxis().SetTitle("pull")
    histogram.SetTitle("Post-fit nuisance pull distribution")
    histogram.SetMarkerStyle(20)
    histogram.SetMarkerSize(2)
    #histogram.Fit("gaus")
    histogram.Draw("pe")
    #canvas.SaveAs(options.plotfile)
    fout.WriteTObject(canvas)
    canvas.SaveAs("plots/pulls.pdf")

    canvas_nuis = ROOT.TCanvas("nuisances", "nuisances", 900, 600)
    ROOT.gStyle.SetOptFit(0)
    ROOT.gStyle.SetOptStat(0)
    hist_fit_e_s = hist_fit_s.Clone()
    hist_fit_e_b = hist_fit_b.Clone()
    hist_fit_s_g = getGraph(hist_fit_s,-0.1)
    hist_fit_b_g = getGraph(hist_fit_b, 0.1)
    hist_fit_s_g.SetLineColor(ROOT.kRed)
#    hist_fit_s_g.SetMarkerColor(ROOT.kRed)
    hist_fit_s_g.SetMarkerColor(ROOT.kBlack)
    hist_fit_b_g.SetLineColor(ROOT.kBlue)
#    hist_fit_b_g.SetMarkerColor(ROOT.kBlue)
    hist_fit_b_g.SetMarkerColor(ROOT.kBlack)
    hist_fit_b_g.SetMarkerStyle(20)
    hist_fit_s_g.SetMarkerStyle(20)
    hist_fit_b_g.SetMarkerSize(1.0)
    hist_fit_s_g.SetMarkerSize(1.0)
    hist_fit_b_g.SetLineWidth(2)
    hist_fit_s_g.SetLineWidth(2)
    hist_prefit.SetLineWidth(2)
    hist_prefit.SetTitle("Nuisance Parameters")
    hist_prefit.SetLineColor(ROOT.kBlack)
    hist_prefit.SetFillColor(ROOT.kGray)
    hist_prefit.SetMaximum(3)
    hist_prefit.SetMinimum(-3)
    hist_prefit.GetXaxis().SetTitle("Nuisance Parameters")
    hist_prefit.Draw("E2")
    hist_prefit.Draw("histsame")
    for ibin, nuisance in boundaries:
        line = ROOT.TLine()
        line.SetLineColor(ROOT.kGray+2)
        line.SetLineWidth(1)
        line.DrawLine(ibin,-3,ibin,3)
    hist_fit_b_g.Draw("EPsame")
#    hist_fit_s_g.Draw("EPsame")
    canvas_nuis.SetGridx()
    canvas_nuis.RedrawAxis()
    canvas_nuis.RedrawAxis('g')
    for ibin, nuisance in boundaries:
        text = ROOT.TLatex()
        text.SetTextAlign(13)
        text.SetTextFont(42)
        text.SetTextSize(0.015)
        # text.SetTextAngle(45)
        text.DrawLatex(ibin,-2.4+0.07*(ibin%10),nuisance)

    leg=ROOT.TLegend(0.6,0.7,0.89,0.89)
    leg.SetFillColor(0)
    leg.SetTextFont(42)
    leg.AddEntry(hist_prefit,"Prefit","FL")
    leg.AddEntry(hist_fit_b_g,"B-only fit","EPL")
#    leg.AddEntry(hist_fit_s_g,"S+B fit"   ,"EPL")
    leg.Draw()
    fout.WriteTObject(canvas_nuis)
    canvas_nuis.SaveAs("plots/nuisances.pdf")
    canvas_pferrs = ROOT.TCanvas("post_fit_errs", "post_fit_errs", 900, 600)
    hist_fit_e_s = hist_fit_s.Clone()
    hist_fit_e_b = hist_fit_b.Clone()
    for b in range(1,hist_fit_e_s.GetNbinsX()): 
      hist_fit_e_s.SetBinContent(b,hist_fit_s.GetBinError(b)/hist_prefit.GetBinError(b))
      hist_fit_e_b.SetBinContent(b,hist_fit_b.GetBinError(b)/hist_prefit.GetBinError(b))
      hist_fit_e_s.SetBinError(b,0)
      hist_fit_e_b.SetBinError(b,0)
    hist_fit_e_s.SetFillColor(ROOT.kRed)
    hist_fit_e_b.SetFillColor(ROOT.kBlue)
    hist_fit_e_s.SetBarWidth(0.4)
    hist_fit_e_b.SetBarWidth(0.4)
    hist_fit_e_b.SetBarOffset(0.45)
    hist_fit_e_b.GetYaxis().SetTitle("#sigma_{#theta}/(#sigma_{#theta} prefit)")
    hist_fit_e_b.SetTitle("Nuisance Parameter #Delta#sigma")
    hist_fit_e_s.GetYaxis().SetTitle("#sigma_{#theta}/(#sigma_{#theta} prefit)")
    hist_fit_e_s.SetTitle("Nuisance Parameter #Delta#sigma")
    hist_fit_e_b.SetMaximum(1.5)
    hist_fit_e_b.SetMinimum(0)
    hist_fit_e_b.Draw("bar")
##    hist_fit_e_s.Draw("barsame")
#    hist_fit_e_s.Draw("bar")
    leg_rat=ROOT.TLegend(0.6,0.7,0.89,0.89)
    leg_rat.SetFillColor(0)
    leg_rat.SetTextFont(42)
    leg_rat.AddEntry(hist_fit_e_b,"B-only fit","F")
#    leg_rat.AddEntry(hist_fit_e_s,"S+B fit"   ,"F")
    leg_rat.Draw()
    line_one = ROOT.TLine(0,1,hist_fit_e_s.GetXaxis().GetXmax(),1)
    line_one.SetLineColor(1); line_one.SetLineStyle(2); line_one.SetLineWidth(2)
    line_one.Draw()
    canvas_pferrs.RedrawAxis()

    fout.WriteTObject(canvas_pferrs)

   
