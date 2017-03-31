import os
import sys
import ROOT as r
from dataMCplotMaker import dataMCplot



if __name__ == "__main__":

    r.gROOT.SetBatch(1)

    systs = [0.2, 0.2, 0.3, 0.3, 0.5, 0.5, 0.3, 0.35]
    colors = [r.kGreen+3, r.kGreen-6, r.kOrange, r.kOrange-3, r.kViolet+2, r.kMagenta-7, r.kGray+2, 18]
    titles = ["t#bar{t}W", "t#bar{t}Z/H", "WZ", "WW", "X+#gamma", "Rare SM", "Charge misid.", "Nonprompt lep."]
    # f1 = r.TFile("histos_Mar29.root")
    f1 = r.TFile("histos.root")

    d_opts_br = {
          "ht": "--lumi 35.9     --ratioUpperBound 4.0 --xAxisLabel H_{T} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred. --legendTaller 0.15 --yTitleOffset -0.0 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "met": "--lumi 35.9     --xAxisLabel E_{T}^{miss} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred. --legendTaller 0.15 --yTitleOffset -0.0 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "njets": "--lumi 35.9   --xAxisLabel N_{jets} --noXaxisUnit --nDivisions 6 --noDivisionLabel --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred. --legendTaller 0.15 --yTitleOffset 0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros   --outOfFrame ",
          "nbtags": "--lumi 35.9  --noDivisionLabel --noXaxisUnit --xAxisLabel N_{b} --nDivisions 4 --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset 0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros   --outOfFrame ",
          "mtmin": "--lumi 35.9   --xAxisLabel m_{T}^{min} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",

          "mll": "--lumi 35.9   --xAxisLabel m_{ll} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "mlle": "--lumi 35.9   --xAxisLabel m_{ee} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "mllmu": "--lumi 35.9   --xAxisLabel m_{#mu#mu} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "mllem": "--lumi 35.9   --xAxisLabel m_{e#mu} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "type": "--lumi 35.9   --xAxisLabel type --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "charge": "--lumi 35.9   --xAxisLabel charge --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "mt2real": "--lumi 35.9   --xAxisLabel m_{T}^{2} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",

          "nleps": "--lumi 35.9   --xAxisLabel Nleps --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "wcands": "--lumi 35.9   --xAxisLabel Wcands --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "topcands": "--lumi 35.9   --xAxisLabel Topcands --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame ",
          "SR_TOTAL": "--lumi 35.9 --isLinear  --xAxisLabel SR --noOverflow --legendUp -.01 --legendRight -0.05 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.05 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame --makeTable ",
          }
    for name in ["met", "ht", "mtmin", "njets", "nbtags","nleps","wcands", "topcands", \
            "mll", "mlle", "mllmu", "mllem", "type", "charge", "mt2real", "SR_TOTAL"]:

        oname = "plots/%s_all_SS.pdf" % name.replace("_TOTAL","")

        title = "Baseline selection"
        subtitle = "(a)"
        d_newopts = {
            "outputName": oname,
        }

        opts_str = d_opts_br[name]

        bgs = map(lambda x: f1.Get("{0}_{1}".format(name,x)), ["data","tttt","ttw","ttzh","wz","ww","xg","rares","flips","fakes"])
        h_data,h_tttt,bgs = bgs[0], bgs[1], bgs[2:]

        print name

        h_tttt.Scale(10.0)

        dataMCplot(h_data, bgs=bgs, sigs=[h_tttt], sigtitles=["t#bar{t}t#bar{t} x 10"], systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

        # os.system("ic " + d_newopts["outputName"])

os.system("niceplots plots plots_tttt_Mar30_wcand20")
