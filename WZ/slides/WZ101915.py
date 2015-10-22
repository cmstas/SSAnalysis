import sys, os
import slideMaker as sm
import tableMaker as tm

sm.addGlobalOptions("--graphicspaths ../pdfs/")

sm.initSlides(me="Nick",themeName="alex",opts="--themecolor 13,26,129")
sm.addSlide(title="WZ Scale Factor ENDL With 832.7 pb$^{-1} (OLD)$", opts="")

sm.addSlide(title="Overview", text="""
        - Using latest json with 832 pb$^{-1}$
        - Construct \\coolblue{WZ control region} with baseline requirement of $\\met > 30$ and Njets$>2$ and
          -- 3 tight leptons
          -- Pair of leptons has $|m_{\\ell\\ell}-m_Z|\\leq15$
          -- Njets$<5$
          -- 0 btags
          -- in the HighHigh region (each lepton has $\\pt>25$)
""")

sm.addSlide(p1="h1D_njets.pdf", p2="h1D_nbtags.pdf", title="N-1 plots: njets, nbtags")
sm.addSlide(p1="h1D_lep1pt.pdf", p2="h1D_lep2pt.pdf", title="N-1 plots: lepton $\\pt$")

sm.addSlide(p1="h1D_met.pdf", p2="h1D_mtmin.pdf", title="CR plots: $\\met$, $\\mtmin$")
sm.addSlide(p1="h1D_ht.pdf", p2="h1D_zmass.pdf", title="CR plots: $\\Ht$, $m_{\\ell\\ell}$")

sm.addSlide(title="CR scale factor",text="""
        -- Put 100%% systematic uncertainty on nonWZ contributions
        -- From this CR, after subtracting out nonWZ contributions, I find a WZ data/MC scale factor of $\\mathbf{1.45\\pm 0.50}$.
        """)

sm.writeSlides("WZ101915.tex", opts="--compile --copy --dump")
