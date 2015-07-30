#include "/home/users/cgeorge/software/SlideMaker/SlideMaker.h"
#include "TString.h"

void makePres(){

  pres myTest("blue", true); 
  myTest.Underline("Alex");
  myTest.TitleSlide("Data vs. MC Plots in SS CRs");

  //Overview slide
  myTest.NewSlide();
  myTest.Title(R"( Overview \& Details )");
  myTest.AllText(); 
  myTest.Text(R"(
  \begin{itemize} 
  \item Here we present data vs. MC plots in several kinematic variables, useful as general validation.
  \item Events selected are SS (SS+SS or SS+FO) or OS events selected for any CR and any of HH/HL/LL.
  \begin{itemize}
  \item Our CRs require 2 jets and either 30 MET or 500 HT
  \item We also implicitly reject events without 2 selected leptons; selected leptons must have $p_T \geq 10$ GeV
  \end{itemize}
  \item Run-2 MC samples are used whenever available, otherwise PHYS14 samples are used.
  \item For data, we use DoubleEG, DoubleMuon, and MuonEG, with duplicate removal
  \item We apply ``our'' triggers to both data and MC (except for phys14 samples)
  \item Jet corrections are ``out of the box'' (not applied by us) -- I have the JECs stored though.
  \end{itemize} 
  )");
  myTest.FinishSlide();

  //MET
  myTest.NewSlide();
  myTest.Title("MET");
  myTest.TextPlotPlot("nominal/MET_OS.pdf", "nominal/MET.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item Consistently overpredict in OS channel; shape looks OK.
  \item SS channel looks fine, limited statistics
  \end{itemize} 
  )");
  myTest.FinishSlide();

  myTest.NewSlide();
  myTest.Title("MET -- Upstream");
  myTest.TextPlotPlot("upstream/MET_OS.pdf", "upstream/MET.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item We try going upstream -- no nJets, MET/HT cut
  \item OS looks good, everything seems shifted by one bin -- jet corrections?
  \item SS channel looks fine at high MET, probably missing samples at low MET 
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  myTest.NewSlide();
  myTest.Title("MET -- Upstream, OS");
  myTest.TextPlotPlot("upstream/MET_eemm_OS.pdf", "upstream/MET_em_OS.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item We break the OS down into ee+$\mu \mu$ and e$\mu$
  \item It is DY, so ee+$\mu \mu$ is same as previous slide.  e$\mu$ looks good.
  \end{itemize} 
  )");
  myTest.FinishSlide();

  //HT
  myTest.NewSlide();
  myTest.Title("HT");
  myTest.TextPlotPlot("nominal/HT_OS.pdf", "nominal/HT.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item Consistently overpredict in OS channel; shape looks OK.
  \item SS channel looks fine, limited statistics
  \end{itemize} 
  )");
  myTest.FinishSlide();

  myTest.NewSlide();
  myTest.Title("HT -- upstream");
  myTest.TextPlotPlot("upstream/HT_OS.pdf", "upstream/HT.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item We try going upstream -- no nJets, MET/HT cut
  \item OS looks great!  
  \item SS still suffers from missing samples
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  myTest.NewSlide();
  myTest.Title("HT OS looks worse downstream due to the MET $\\geq$ 30 requirement");
  myTest.TextPlotPlot("special/HT_OS_met30.pdf", "upstream/HT_OS_2jet.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item Apparently the MET discrepencies we noted previously are causing problems
  \item The nJets cut causes no problems 
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //Lepton pT
  myTest.NewSlide();
  myTest.Title("Lepton $p_{T}$");
  myTest.TextPlotPlot("nominal/leppt_OS.pdf", "nominal/leppt.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS overpredicts as before
  \item SS looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //nJets
  myTest.NewSlide();
  myTest.Title("nJets");
  myTest.TextPlotPlot("nominal/nJets_OS.pdf", "nominal/nJets.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS overpredicts as before
  \item SS looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //nBtags
  myTest.NewSlide();
  myTest.Title("nBtags");
  myTest.TextPlotPlot("nominal/nBtags_OS.pdf", "nominal/nBtags.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS overpredicts as before
  \item SS looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //mll
  myTest.NewSlide();
  myTest.Title(R"($\textrm{M}_{\ell \ell}$)");
  myTest.TextPlotPlot("nominal/mll_OS.pdf", "nominal/mll.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS overpredicts as before
  \item SS looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //mht
  myTest.NewSlide();
  myTest.Title("MHT");
  myTest.TextPlotPlot("nominal/mht_OS.pdf", "nominal/mht.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS overpredicts as before
  \item SS looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //mtmin
  myTest.NewSlide();
  myTest.Title(R"($\textrm{M}_{T}$ min)");
  myTest.TextPlotPlot("nominal/mtmin_OS.pdf", "nominal/mtmin.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS overpredicts as before
  \item SS looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //Conclusions
  myTest.AllText();
  myTest.Title(R"(Conclusions)");
  myTest.Text(R"(
  \begin{itemize}
  \item OS plots consistently underpredict with our selections, but look OK upstream.
  \item SS looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  vector <string> out;
  out.push_back("MET");
  out.push_back("HT");
  out.push_back("nJets");
  out.push_back("nBtags");
  out.push_back("mtmin");
  out.push_back("leppt");
  out.push_back("mll");
  out.push_back("mht");

  vector <string> type;
  type.push_back("");
  type.push_back("_eemm");
  type.push_back("_em");

  for (unsigned int i = 0; i < 8; i++){
    for (unsigned int j = 0; j < 3; j++){
      myTest.NewSlide();
      myTest.Plot(Form("nominal/%s%s_OS.pdf", out[i].c_str(), type[j].c_str() )); 
      myTest.FinishSlide();
    }
  }
  for (unsigned int i = 0; i < 8; i++){
    for (unsigned int j = 0; j < 3; j++){
      myTest.NewSlide();
      myTest.Plot(Form("nominal/%s%s.pdf", out[i].c_str(), type[j].c_str() )); 
      myTest.FinishSlide(); } }

}
