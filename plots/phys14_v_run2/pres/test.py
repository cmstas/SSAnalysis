import slideMaker as sm
import utils

#To get the GUI
sm.addGlobalOptions("--makegui")

content = """
 - PHYS14 samples use v1.29
   -- Does not include triggers or MET filters, as these are not available for PHYS14
   -- DY sample only exists for $M_{ll} > 50$ GeV
 - Run2 samples use v3.05
   -- Does not include triggers or MET filters, to match PHYS14
   -- Use only DY sample with $M_{ll} > 50$ GeV, to match PHYS14
 - All plots use SS ``loose baseline'' selection:
   -- Require a SS (or OS) numer-numer pair 
   -- Do not impose a nJets or MET/HT requirement
 - All plots use most up-to-date JEC corrections (and propogated to MET), except where uncorrected
   -- PHYS14: PHYS14\\_V1\\_MC\\_L*\\_AK4PFchs collection
   -- Run2: Summer15\\_25nsV2\\_MC\\_L*\\_AK4PFchs.txt collection

"""

content2 = """
 - Clearly there are some cross-section differences
   -- WZ $\\rightarrow 3 \\ell$ used to be 2.17 pb, now it is 1.67.  
   -- ttZ used to be .842, now it is .789.
 - The W+Jets sample has very poor statistics
   -- We used the inclusive here, since the HT-binned is only for HT $\\geq$ 100 GeV
"""

content3 = """
   - WZJetsTo3LNu\\_Tune4C\\_13TeV-madgraph-tauola\\_Phys14DR-PU20bx25\\_PHYS14\\_25\\_V1-v1
   - WJetsToLNu\\_13TeV-madgraph-pythia8-tauola\\_Phys14DR-PU20bx25\\_PHYS14\\_25\\_V1-v1
   - DYJetsToLL\\_M-50\\_13TeV-madgraph-pythia8\\_Phys14DR-PU20bx25\\_PHYS14\\_25\\_V1-v1
   - TTJets\\_MSDecaysCKM\\_central \\_Tune4C\\_13TeV-madgraph-tauola\\_Phys14DR-PU20bx25\\_PHYS14\\_25\\_V1-v1
   - TTWJets\\_Tune4C\\_13TeV-madgraph-tauola\\_Phys14DR-PU20bx25\\_PHYS14\\_25\\_V1-v1
   - TTZJets\\_Tune4C\\_13TeV-madgraph-tauola\\_Phys14DR-PU20bx25\\_PHYS14\\_25\\_V1-v1
"""

content4 = """
   - TTWJetsToLNu\\_TuneCUETP8M1    \\_13TeV-amcatnloFXFX-madspin-pythia8\\_RunIISpring15DR74-Asympt25ns\\_MCRUN2\\_74\\_V9-v1
   - TTWJetsToQQ\\_TuneCUETP8M1\\_13TeV-amcatnloFXFX-madspin-pythia8\\_RunIISpring15DR74-Asympt25ns\\_MCRUN2\\_74\\_V9-v1
   - TTZToLLNuNu\\_M-10\\_TuneCUETP8M1\\_13TeV-amcatnlo-pythia8\\_RunIISpring15DR74-Asympt25ns\\_MCRUN2\\_74\\_V9-v1
   - TTZToQQ\\_TuneCUETP8M1\\_13TeV-amcatnlo-pythia8\\_RunIISpring15DR74-Asympt25ns\\_MCRUN2\\_74\\_V9-v1
   - WZTo3LNu\\_TuneCUETP8M1\\_13TeV-powheg-pythia8\\_RunIISpring15DR74-Asympt25ns\\_MCRUN2\\_74\\_V9-v1
   - WJetsToLNu\\_TuneCUETP8M1 \\_13TeV-amcatnloFXFX-pythia8\\_RunIISpring15DR74-Asympt25ns\\_MCRUN2\\_74\\_V9-v1
   - DYJetsToLL\\_M-50\\_TuneCUETP8M1 \\_13TeV-amcatnloFXFX-pythia8\\_RunIISpring15DR74-Asympt25ns\\_MCRUN2\\_74\\_V9-v3
   - TTJets\\_TuneCUETP8M1\\_13TeV-madgraphMLM-pythia8\\_RunIISpring15DR74-Asympt25ns\\_MCRUN2\\_74\\_V9-v2
"""

updates = """
   - Samples Updated to facilitate apples-to-apples comparison with PHYS14
     -- added hadronic channels to ttV samples in run2.  The ttW agreement is a lot better now (flips/fakes)
     -- moved to WZ $\\rightarrow \\ell \\nu$ instead of inclusive in run2 
   - MET plots changed:
     -- Removed corrected MET plot; the MET correction we were using in PHYS14 has been deprecated
     -- Added MET 3.0 plot
   - Fixed bug in jet corrections, updated to latest jet correction files
"""

content5 = """
   - Generally good agreement between PHYS14 and run2-25ns
   - The Jet Energy Corrections don't seem to help the agreement
"""

gridslides = []

#t1 = sm.textObject(x=0.25,y=0.15,width=0.3, text="testlabel", color="red", size=0, bold=False) 
#t2 = sm.textObject(x=0.75,y=0.15,width=0.3, text="testlabel", color="coolblue", size=0, bold=False) 

#a1 = sm.arrowObject( (0.31,0.15), (0.69,0.15) )
#a2 = sm.arrowObject( (0.31,0.15), (0.69,0.42) )

# supported themes are "nick", "alex", and "madrid"
t="alex"
# test2 and test3 can be folders with your plots, so you can execute this script anywhere really
# themecolor takes RGB. could put in explicit color names, but RGB more robust
# also try the --modernfont option
sm.initSlides(me="Alex",themeName=t,opts="--themecolor 51,51,179")
sm.addSlide(title="PHYS14 vs. Run2 ENDL Comparison", opts="")

#Label
sm.addObject("Phys14","text", (0.21,0.23),   text="\\underline{Phys14}",bold=True)
sm.addObject("Run2-25ns","text", (0.71,0.23),text="\\underline{Run2-25ns}",bold=True)
sm.addSlide(title="This is a check to see if the phys14 and run2-25ns samples are consistent", text=content)
sm.addSlide(title="Several updates have been made since last week", text=updates)
sm.addSlide(title="We use the relevant PHYS14 samples and the corresponding Run2-25ns samples", text=content3, text2=content4, opts="--textsize -4 --textbottom", objects=["Phys14","Run2-25ns"])
sm.addSlide(title="Lepton $p_T$ shows generally good agreement", p1="../lep_pt.pdf", p2="../lep_pt_OS.pdf", text=content2, opts="--plottop")
sm.addSlide(title="The $M_{\\ell \\ell}$ distribution is similar", p1="../mll.pdf"          , p2="../mll_OS.pdf"          , text="Hi")
sm.addSlide(title="Jet $p_T$ uncorrected", p1="../jetpt_uncorr.pdf" , p2="../jetpt_uncorr_OS.pdf" , text="Hi")
sm.addSlide(title="Jet $p_T$ corrected", p1="../jetpt.pdf"        , p2="../jetpt_OS.pdf"        , text="Corrections don't seem to do much, maybe slight improvement")
sm.addSlide(title="MET uncorrected",   p1="../metuncorr.pdf"    , p2="../metuncorr_OS.pdf" ) 
sm.addSlide(title="MET 3.0", p1="../met3p0.pdf"          , p2="../met3p0_OS.pdf"          , text="")
sm.addSlide(title="$M_T$ min", p1="../mtmin.pdf"        , p2="../mtmin_OS.pdf"        , text="Hi")
sm.addSlide(title="Conclusions", text=content5)
sm.startBackup()
sm.addSlide(title="HT uncorrected", p1="../htuncorr.pdf"     , p2="../htuncorr_OS.pdf"     , text="Hi")
sm.addSlide(title="HT corrected", p1="../ht.pdf"           , p2="../ht_OS.pdf"           , text="Hi")
sm.addSlide(title="nJets uncorrected", p1="../njets_uncorr.pdf" , p2="../njets_uncorr_OS.pdf" , text="Hi")
sm.addSlide(title="nJets corrected", p1="../njets.pdf"        , p2="../njets_OS.pdf"        , text="Hi")
sm.addSlide(title="nBjets uncorrected", p1="../nbtags_uncorr.pdf", p2="../nbtags_uncorr_OS.pdf", text="Hi")
sm.addSlide(title="nBjets corrected", p1="../nbtags.pdf"       , p2="../nbtags_OS.pdf"       , text="Hi")
sm.writeSlides("Phys14_vs_Run2-25ns_v2.tex", opts="--compile --copy")
