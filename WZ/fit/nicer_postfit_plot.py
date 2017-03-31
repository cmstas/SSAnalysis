from errors import E
import os
import sys
import ROOT as r
# export PYTHONPATH=/home/users/namin/2016/ss/master/SSAnalysis/software/dataMCplotMaker:$PYTHONPATH
from dataMCplotMaker import dataMCplot

# text = """
# bins_data: [496.0, 159.0, 69.0, 5.0]
# bins_wz: [353.807672016 +- 35.6716790256, 28.9570942149 +- 5.26612442767, 1.78065241958 +- 0.706353880717, 0.0 +- 0.0]
# bins_ttz: [44.1288788013 +- 11.6202693936, 89.3339448642 +- 15.1706931648, 50.9124912756 +- 16.0854926435, 3.92082743071 +- 1.87380632211]
# bins_fakes: [23.72396631 +- 13.0357155525, 12.75698764 +- 7.19196140469, 1.98555126543 +- 1.59577830117, 0.0615253269866 +- 0.0780805470605]
# bins_rares: [72.7313172382 +- 31.1904334336, 32.4357418114 +- 16.0553596213, 10.6997789347 +- 7.03917373975, 0.686639781235 +- 0.487871460707]
# """

# d_bins = {
#         }

# for line in text.splitlines():
#     line = line.strip().replace("]","").replace("[","")
#     if not line: continue
#     print line
#     if "bins_data" in line:
#         d_bins["data"] = map(int,map(float,line.split(":")[-1].split(",")))
#     else:
#         which,nums = line.split(":")
#         which = which.split("_")[-1]
#         d_bins[which] = map(lambda x: map(float,x.split("+-")), nums.split(","))
# print d_bins


# # blah = "72.7313172382 +- 31.1904334336, 32.4357418114 +- 16.0553596213, 10.6997789347 +- 7.03917373975, 0.686639781235 +- 0.487871460707"
# # print map(lambda x: map(float,x.split("+-")), blah.split(","))

# def fill_hist(h1, vals):
#     for ival,val in enumerate(vals):
#         if type(val) == list:
#             h1.SetBinContent(ival+1,val[0])
#             h1.SetBinError(ival+1,val[1])
#         else:
#             h1.SetBinContent(ival+1,val)

# h_data = r.TH1F("h_data","h_data",4,0,4)
# h_wz = r.TH1F("h_wz","h_wz",4,0,4)
# h_ttz = r.TH1F("h_ttz","h_ttz",4,0,4)
# h_fakes = r.TH1F("h_fakes","h_fakes",4,0,4)
# h_rares = r.TH1F("h_rares","h_rares",4,0,4)

# fill_hist(h_data, d_bins["data"])
# fill_hist(h_wz, d_bins["wz"])
# fill_hist(h_ttz, d_bins["ttz"])
# fill_hist(h_fakes, d_bins["fakes"])
# fill_hist(h_rares, d_bins["rares"])
fin = r.TFile("scaledfit.root")
h_data = fin.Get("h_data")
h_wz = fin.Get("h_wz")
h_ttz = fin.Get("h_ttz")
h_fakes = fin.Get("h_fakes")
h_rares = fin.Get("h_rares")

d_opts = {
        "lumi": 35.9,
        "outputName": "plots/postfit_nice.pdf",
        "xAxisLabel": "SR",
        "noXaxisUnit": True,
        "isLinear": True,
        "legendUp": -0.15,
        "legendRight": 0.05,
        "outOfFrame": True,
        "legendTaller": -0.05,
        "yTitleOffset": -0.5,
        "noRatioPlot": True,
        "type": "Preliminary",
        "outOfFrame": True,
        "type": "Preliminary",
        "dataName": "Data",
        "setMinimum": 0.03,
        }

# vector <Color_t> colors; // want this to match yieldMaker.C
# colors.push_back(18);  // fakes
# colors.push_back(kGreen-6);  // ttzh
# colors.push_back(kGreen+3);  // tw
# colors.push_back(kMagenta-7);  // rares
# colors.push_back(kOrange-3);  // ww
# colors.push_back(kViolet+2);  // xg
# colors.push_back(kOrange); // wz
# colors.push_back(kGreen-7);  // tth
#   std::cout << " nGoodEventsWeighted: " << nGoodEventsWeighted << " nGoodEvents: " << nGoodEvents << " nGoodEventsData: " << nGoodEventsData << " nEventsTotal: " << nEventsTotal << std::endl;
#   printCounter(true);
#   TH1F* data;
#   // TString comt = Form(" --errHistAtBottom --doCounts --colorTitle --lumi %.2f --lumiUnit fb --percentageInBox --legendRight 0.05 --legendUp 0.05 --noDivisionLabel --noType --outputName pdfs/",luminosity);
#   // std::string pct = " --showPercentage ";
#   TString comt = Form(" --errHistAtBottom --lumi %.2f --makeTable --lumiUnit fb --noDivisionLabel --legendTaller 0.15 --legendUp -0.15 --dataName Data --legendRight -0.08 --outOfFrame --type Preliminary --legendCounts --outputName pdfs/",luminosity);


title = "Post-fit"

colors = [r.kOrange,r.kGreen-6,18,r.kMagenta-7]
titles = [
        "WZ",
        "ttZ",
        "Fakes",
        "Rares",
        ]
bgs = [h_wz,h_ttz,h_fakes,h_rares]
dataMCplot(h_data, bgs=bgs, titles=titles, title=title, colors=colors, opts=d_opts)
os.system("ic " + d_opts["outputName"])
