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
        ["Raw $H_T$ ratio"       , "kinem_ht_raw__comparison_mc.pdf"],
        ["Lep1 $\\pt$ ratio" , "kinem_lep1pt__comparison_mc.pdf"],
        ["Lep2 $\\pt$ ratio" , "kinem_lep2pt__comparison_mc.pdf"],
        ["$\\met$ ratio"     , "kinem_met__comparison_mc.pdf"],
        ["Raw $\\met$ ratio"     , "kinem_rawmet__comparison_mc.pdf"],
        ["$\\mtmin$ ratio"   , "kinem_mtmin__comparison_mc.pdf"],
        ["Njets ratio"      , "kinem_njets__comparison_mc.pdf"],
        ["Raw Njets ratio"      , "kinem_njets_raw__comparison_mc.pdf"],
        ["Nbtags ratio"       , "kinem_nbtags__comparison_mc.pdf"],
        ["Raw Nbtags ratio"       , "kinem_nbtags_raw_comparison_mc.pdf"],
]

ratio_types_base = [
        ["Base: Raw $H_T$ ratio"       , "kinem_base_ht_raw__comparison_mc.pdf"],
        ["Base: Raw $\\met$ ratio"     , "kinem_base_rawmet__comparison_mc.pdf"],
        ["Base: Raw Njets ratio"      , "kinem_base_njets_raw__comparison_mc.pdf"],
        ["Base: Raw Nbtags ratio"       , "kinem_base_nbtags_raw_comparison_mc.pdf"],
]

sm.addObject("left7","text",(0.27,0.91),width=0.3, text="74X", color="red", size=1, bold=True)
sm.addObject("right7","text",(0.74,0.91),width=0.3, text="76X", color="red", size=1, bold=True)

sm.addObject("leftDMC","text",(0.27,0.91),width=0.3, text="MC", color="red", size=1, bold=True)
sm.addObject("rightDMC","text",(0.74,0.91),width=0.3, text="Data", color="red", size=1, bold=True)


sm.addSlide(title="Overview", text="""
        - Kinematic plots are of events falling into HH, HL, or LL SRs
        - 74/76 ratio plots at end
        """)
for label, plot74, plot76 in plot_types:
    sm.addSlide(title=label, p1=plot74, p2=plot76, objects=["left7","right7"])
for label, ratioplot in ratio_types:
    sm.addSlide(title=label, p1=ratioplot, p2=ratioplot.replace("_mc.pdf","_data.pdf"), objects=["leftDMC","rightDMC"])

sm.addSlide(title="base", text="- base = all hyp\\_class==3 events (no requirement that it falls into a SR)")

for label, ratioplot in ratio_types_base:
    sm.addSlide(title=label, p1=ratioplot, p2=ratioplot.replace("_mc.pdf","_data.pdf"), objects=["leftDMC","rightDMC"])


sm.writeSlides("SScomparison7476_Apr25.tex", opts="--compile --copy --dump")
