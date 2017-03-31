#!/usr/bin/env python
import ROOT as r
r.gROOT.SetBatch(True)
import os, sys
import argparse
import numpy as np
from numpy.linalg import inv

# MAKE THE CARD VIA
# Note that these are exclusive regions, so you must have run makeAggregateHists.py first
# python makeCovariance.py -c v9.04_Jan31_36p8_t1tttt_exclagg/card_fs_t1tttt_m1500_m400_36.8ifb-all.txt
# python createCard.py v9.06_Mar6_35p9_reminiaod_t1ttttagg fs_t1tttt_m1500_m400 
# python makeCovariance.py -c v9.06_Mar6_35p9_reminiaod_t1ttttagg/card_fs_t1tttt_m1500_m400_35.9ifb-all.txt
# python createCard.py v9.06_Mar6_35p9_reminiaodfilt_nomhiggs_agg fs_t1tttt_m1500_m400


def main(args):

    print ">>> Running combine"

    os.system("combine -M MaxLikelihoodFit --saveShapes --saveWithUnc --saveOverall --numToysForShapes 1000 --preFitValue 0 %s" % args.card)

    print ">>> Running makeLHInputs.py"

    os.system("python makeLHInputs.py -i mlfit.root -o temp.root")

    f1 = r.TFile("temp.root")

    h = f1.Get("shapes_prefit/total_covar")

    c1 = r.TCanvas()
    r.gStyle.SetOptStat(0)
    c1.SetLogz(0)
    # h.SetTitle(args.title)
    h.SetTitle(" ")

    h_cov = h.Clone("covariance")

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
            h.GetXaxis().SetBinLabel(i,"ESR"+str(i))
            h.GetYaxis().SetBinLabel(i,"ESR"+str(i))

        h_corr = h.Clone("correlation")

        args.title = args.title.replace("covariance", "correlation")
        # h.SetTitle(args.title)
        h.SetTitle(" ")
        h.GetZaxis().SetRangeUser(0.0,1.0)
        # h.GetZaxis().SetRangeUser(0.5,1.0)
        args.output = args.output.replace("covariance","correlation")

    fout = r.TFile("SS2017_exclcorrcov.root", "RECREATE")
    h_cov.Write()
    h_corr.Write()
    fout.Write()
    fout.Close()

    lumi = args.card.split("ifb")[0].split("_")[-1]

    r.gStyle.SetPaintTextFormat(".3f")
    h.Draw("text colz")

    cmstex = r.TLatex(0.705,0.91, lumi+" fb^{-1} (13 TeV)" )
    cmstex.SetNDC()
    cmstex.SetTextSize(0.04)
    cmstex.SetLineWidth(2)
    cmstex.SetTextFont(42)
    cmstex.Draw()

    cmstexbold = r.TLatex(0.10,0.91, "CMS" )
    cmstexbold.SetNDC()
    cmstexbold.SetTextSize(0.05)
    cmstexbold.SetLineWidth(2)
    cmstexbold.SetTextFont(61)
    cmstexbold.Draw()

    cmstexprel = r.TLatex(0.18,0.91, "Preliminary" )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.04)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstexprel.Draw()

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
