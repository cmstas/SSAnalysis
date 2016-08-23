import tableMaker as tm
import os

template = r"""
\documentclass{article}
\usepackage{multirow}
\usepackage{slashed}
\usepackage{xspace}
\newcommand{\met}{\slashed{E}_\mathrm{T}}
\newcommand{\mt}{m_\mathrm{T}}
\newcommand{\pt}{p_\mathrm{T}}
\newcommand{\mtmin}{m_{T}^\mathrm{min}}
\newcommand{\Totttt}{\ensuremath{\mbox{T}1\mbox{tttt}}\xspace}
\newcommand{\TfqqqqWW}{\ensuremath{\mbox{T}5\mbox{qqqq}\PW\PW}\xspace}
\newcommand{\eslash}{\ensuremath{\hbox{$E$\kern-0.6em\lower-.05ex\hbox{/}\kern0.10em}}}}
\newcommand{\ETmiss}{\ensuremath{\eslash_{\rm T}}\xspace}
\newcommand{\HT}{\ensuremath{H_\text{T}}\xspace}
\newcommand{\dmtwenty}{$m_{ {\tilde{\chi}^\pm _1} } = m_{ {\tilde{\chi}^0_1} }$ + 20 GeV\xspace}
\newcommand{\dmhalf}{$m_{ {\tilde{\chi}^\pm _1} } = 0.5(m_{ \tilde{g} } + m_{ {\tilde{\chi}^0_1} })$\xspace}
\newcommand{\gev}{GeV~}
\begin{document}
\pagenumbering{gobble}
%s
\end{document}
"""

# content = template % r"""
# \begin{table}[!h]
# \begin{center}
# \caption{\label{tab:cutflowfs_t5qqqqvvdm20_m1400_m200} Cut flow table for the \TfqqqqWW model  with \dmhalf  assuming gluino and LSP masses equal to 1400 and 200 GeV, respectively.
# The last two lines correspond to the most populated search regions.
# The assumed cross section for this model is 0.0253 pb.}
# \begin{tabular}{lcc}
# \hline
# Selection & Yield & Efficiency (\%) \\
# \hline
# No selection                             & 326.3 & 100.00 \\
# Trigger and $\geq$2 leptons              &   4.3 &   1.30 \\
# At least one SS lepton pair              &   1.8 &   0.55 \\
# Baseline (two jets, and either \ETmiss $>$ 30\gev or \HT $>$ 500\gev) &   1.8 &   0.55 \\
# Any SR (\ETmiss $>$ 50\gev)              &   1.8 &   0.54 \\
# \hline
# HL SR26                                  &   0.5 &   0.17 \\
# HL SR24                                  &   0.2 &   0.06 \\
# \hline
# \end{tabular}
# \end{center}
# \end{table}
# """

everything = []
with open("cutflow_Aug2.txt", "r") as fhin:
    everything = fhin.read().split("\n\n")

for table in everything:
    # print table
    title = table.split("{tab:cutflow")[1].split("}",1)[0].replace("fs_","")
    tm.makePDF(template % table, "tables/table_%s.tex" % title)
# tm.makePDF(content,"tables/test.tex")
# os.system("web tables/test.pdf")
