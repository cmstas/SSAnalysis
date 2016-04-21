import sys
import glob

import slideMaker as sm

# test2 and test3 can be folders with your plots, so you can execute this script anywhere really
sm.addGlobalOptions("--graphicspaths ../yield_plots/")

# sm.initSlides(me="Nick",themeName="alex",opts="--casual Nick --themecolor random --font gillius")
sm.initSlides(me="Nick",themeName="alex",opts="--casual Nick --themecolor 81,73,69 --font gillius")
sm.addSlide(title="SS: 74X vs 76X ", opts="")

# samples = [
# "TChiChi_mChi-150_mLSP-1",
# "TChiNeu_mChi-300_mLSP-285",
# "TChiNeu_mChi-300_mLSP-290",
# "TChiNeu_mChi-300_mLSP-295",
# "TChiNeu_mChi-300_mLSP-297",
# "TChiNeu_mChi-300_mLSP-299",
# ]

# plot_types = [
#         "mChi", "chargino daughters", "W daughters", "met", "pfjet pt", "pfjet eta"
#         ]



plot_types = [
        ["HH"          , "high_yields.pdf"   , "high_yields_76x.pdf"]   ,
        ["HL"          , "hl_yields.pdf"     , "hl_yields_76x.pdf"]     ,
        ["$H_T$"       , "kinem_ht_.pdf"     , "kinem_ht__76x.pdf"]     ,
        ["Lep1 $\\pt$" , "kinem_lep1pt_.pdf" , "kinem_lep1pt__76x.pdf"] ,
        ["Lep2 $\\pt$" , "kinem_lep2pt_.pdf" , "kinem_lep2pt__76x.pdf"] ,
        ["$\\met$"     , "kinem_met_.pdf"    , "kinem_met__76x.pdf"]    ,
        ["$\\mtmin$"   , "kinem_mtmin_.pdf"  , "kinem_mtmin__76x.pdf"]  ,
        ["Njets"       , "kinem_njets_.pdf"  , "kinem_njets__76x.pdf"]  ,
        ["Nbtags"      , "kinem_nbtags_.pdf" , "kinem_nbtags__76x.pdf"] ,
    ]

files = glob.glob("../yield_plots/*.pdf")
# files = [f for f in files if "_all" not in f]

ratio_types = [
        ["HH ratio"          , "high_yields_comparison_mc.pdf"],
        ["HL ratio"          , "hl_yields_comparison_mc.pdf"],
        ["$H_T$ ratio"       , "kinem_ht__comparison_mc.pdf"],
        ["Lep1 $\\pt$ ratio" , "kinem_lep1pt__comparison_mc.pdf"],
        ["Lep2 $\\pt$ ratio" , "kinem_lep2pt__comparison_mc.pdf"],
        ["$\\met$ ratio"     , "kinem_met__comparison_mc.pdf"],
        ["$\\mtmin$ ratio"   , "kinem_mtmin__comparison_mc.pdf"],
        ["Njets ratio"      , "kinem_njets__comparison_mc.pdf"],
        ["Nbtags ratio"       , "kinem_nbtags__comparison_mc.pdf"],
]

sm.addObject("left7","text",(0.27,0.91),width=0.3, text="74X", color="red", size=1, bold=True)
sm.addObject("right7","text",(0.74,0.91),width=0.3, text="76X", color="red", size=1, bold=True)

sm.addObject("leftDMC","text",(0.27,0.91),width=0.3, text="MC", color="red", size=1, bold=True)
sm.addObject("rightDMC","text",(0.74,0.91),width=0.3, text="Data", color="red", size=1, bold=True)

# print files
# qcuts = sorted(list(set(map(lambda x: int(x.split("_")[1]), files))))
# print qcuts

sm.addSlide(title="Overview", text="""
        - Kinematic plots are of events falling into HH, HL, or LL SRs
        - 74/76 ratio plots at end
        """)
for label, plot74, plot76 in plot_types:
    sm.addSlide(title=label, p1=plot74, p2=plot76, objects=["left7","right7"])
for label, ratioplot in ratio_types:
    sm.addSlide(title=label, p1=ratioplot, p2=ratioplot.replace("_mc.pdf","_data.pdf"), objects=["leftDMC","rightDMC"])

# for iplot_type,plot_type in enumerate(plot_types):
#     for sample in samples:
#         plots = sorted([f for f in files if sample in f])

#         if "TChiChi" in sample and plot_type == "chargino daughters": continue
#         if "TChiNeu" in sample and plot_type == "W daughters": continue

#     # print plots
#     # for plot_type,plot in zip(plot_types,plots):
#         title = "[%s] %s" % (plot_type, sample.replace("-","").replace("_","\\_"))
#         print title
#         sm.addSlide(title=title, p1=plots[iplot_type])

# # for qcut in qcuts:
# #     plots = sorted([f for f in files if "_"+str(qcut)+"_" in f])
# #     print plots

# #     if len(plots) == 1:
# #         sm.addSlide(title="qcut = %i" % qcut, p1=plots[0])
# #     else:
# #         sm.addSlide(title="qcut = %i" % qcut, plots=plots)


sm.writeSlides("SScomparison7476_Apr20.tex", opts="--compile --copy --dump")
