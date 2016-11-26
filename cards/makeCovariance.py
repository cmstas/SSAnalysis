#!/usr/bin/env python
import ROOT as r
r.gROOT.SetBatch(True)
import os, sys
import argparse
import numpy as np
from numpy.linalg import inv

# MAKE THE CARD VIA
# python createCard.py v8.04_Nov16_agg fs_t1tttt_m1500_m400 - agg

def main(args):

    

    print ">>> Running combine"

    os.system("combine -M MaxLikelihoodFit --saveShapes --saveWithUnc --saveOverall --numToysForShapes 1000 --preFitValue 0 %s" % args.card)

    print ">>> Running makeLHInputs.py"

    os.system("python makeLHInputs.py -i mlfit.root -o temp.root")

    f1 = r.TFile("temp.root")

    h = f1.Get("shapes_prefit/total_covar")

    c1 = r.TCanvas()
    r.gStyle.SetOptStat(0)
    c1.SetLogz(1)
    h.SetTitle(args.title)

    if args.correlation:
        print ">>> Converting covariance to correlation matrix"

        cov = []
        for iy in range(1,h.GetNbinsY()+1):
            tmp = []
            for ix in range(1,h.GetNbinsX()+1):
                val = h.GetBinContent(ix,iy)
                tmp.append(val)
            cov.append(tmp[:])
        cov = np.array(cov)

        D = np.sqrt(np.diag(np.diag(cov)))
        Dinv = inv(D)
        corr = np.dot(np.dot(Dinv,cov),Dinv)
        for irow,row in enumerate(corr):
            for icol,val in enumerate(row):
                h.SetBinContent(irow+1,icol+1,val)


        for i in range(1,h.GetNbinsX()+1):
            h.GetXaxis().SetBinLabel(i,str(i))
            h.GetYaxis().SetBinLabel(i,str(i))

        args.title = args.title.replace("covariance", "correlation")
        h.SetTitle(args.title)
        args.output = args.output.replace("covariance","correlation")

    h.Draw("colz")

    c1.SaveAs(args.output)

    print ">>> Saved %s" % args.output

if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("card", help="input card")
    parser.add_argument("-t", "--title", help="title of covariance plot", default="Total covariance")
    parser.add_argument("-o", "--output", help="output pdf name", default="covariance.pdf")
    parser.add_argument("-c", "--correlation", help="make correlation matrix instead", action="store_true")
    args = parser.parse_args()
    main(args)
