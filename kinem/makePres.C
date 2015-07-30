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
  \item Jet corrections are ``out of the box'' (not applied by us), except where indicated
  \end{itemize} 
  )");
  myTest.FinishSlide();

  //HT
  myTest.NewSlide();
  myTest.Title("HT OS");
  myTest.TextPlotPlot("nominal/HT_OS.pdf", "upstream/HT_OS.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item Left shows OS HT plot; it seems we overpredict
  \item Right shows same plot without nJets, MET requirement -- much better!
  \end{itemize} 
  )");
  myTest.FinishSlide();

  myTest.NewSlide();
  myTest.Title("HT OS -- between up- and down-stream");
  myTest.TextPlotPlot("special/HT_OS_met30.pdf", "special/HT_OS_2jet.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item Apparently the MET $\geq$ 30 cut causes problems (left)
  \item The nJets cut causes no problems  (right)
  \end{itemize} 
  )", "--size -2");
  myTest.FinishSlide();

  //MET
  myTest.NewSlide();
  myTest.Title("MET OS");
  myTest.TextPlotPlot("nominal/MET_OS.pdf", "upstream/MET_OS.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item Left shows OS MET -- underpredicts
  \item Right shows MET without MET $\geq$ 30 cut (and nJets cut) -- data is skewed to left!  Jet corrections?
  \end{itemize} 
  )");
  myTest.FinishSlide();

  myTest.NewSlide();
  myTest.Title("MET OS -- Upstream with MET corrections");
  myTest.TextPlot("jetcorr/MET_OS.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item Now we try MET corrections.  Only available for MC, but we try to use those for both data and MC.
  \item Agreement is terrible; these MET corrections aren't helping!
  \item Vince and Maria (and Indara, and me) are still discussing the details of this, possible our procedure is not perfect yet
  \item This solves the OS mystery: need better jet corrections
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  myTest.NewSlide();
  myTest.Title("HT SS ");
  myTest.TextPlotPlot("nominal/HT.pdf", "upstream/HT.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item As for SS, looks like things are OK, hard to tell with statistics, MET problem probably has same effect here but obfuscated
  \item Without nJets, MET cuts (right), we underpredict a lot.  
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  myTest.NewSlide();
  myTest.Title("MET SS");
  myTest.TextPlotPlot("nominal/MET.pdf", "upstream/MET.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item MET also looks OK to within statistics
  \item Without nJets, MET corrections (right), we see lots of missing events at low MET.  This is probably from missing low-MET samples (QCD, ....).  This explains the underprediction
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //Lepton pT
  myTest.NewSlide();
  myTest.Title("Lepton $p_{T}$");
  myTest.TextPlotPlot("nominal/leppt_OS.pdf", "nominal/leppt.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item The rest of the slides look good except for the two issues already solved.
  \item OS (left) overpredicts as before (due to bad MET corrections)
  \item SS (right) looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //nJets
  myTest.NewSlide();
  myTest.Title("nJets");
  myTest.TextPlotPlot("nominal/nJets_OS.pdf", "nominal/nJets.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS (left)  overpredicts as before (due to bad MET corrections)
  \item SS (right)  looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //nBtags
  myTest.NewSlide();
  myTest.Title("nBtags");
  myTest.TextPlotPlot("nominal/nBtags_OS.pdf", "nominal/nBtags.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS (left)  overpredicts as before (due to bad MET corrections)
  \item SS (right) looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //mll
  myTest.NewSlide();
  myTest.Title(R"($\textrm{M}_{\ell \ell}$)");
  myTest.TextPlotPlot("nominal/mll_OS.pdf", "nominal/mll.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS (left)  overpredicts as before (due to bad MET corrections)
  \item SS (right) looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //mht
  myTest.NewSlide();
  myTest.Title("MHT");
  myTest.TextPlotPlot("nominal/mht_OS.pdf", "nominal/mht.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS (left)  overpredicts as before (due to bad MET corrections)
  \item SS (right) looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //mtmin
  myTest.NewSlide();
  myTest.Title(R"($\textrm{M}_{T}$ min)");
  myTest.TextPlotPlot("nominal/mtmin_OS.pdf", "nominal/mtmin.pdf");
  myTest.Text(R"(
  \begin{itemize}
  \item OS (left)  overpredicts as before (due to bad MET corrections)
  \item SS (right) looks OK as before, limited statistics
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //Conclusions
  myTest.NewSlide();
  myTest.AllText();
  myTest.Title(R"(Conclusions)");
  myTest.Text(R"(
  \begin{itemize}
  \item OS has right shape, MET corrections need work
  \item SS looks OK, though statistics are limited.  Missing low-MET samples are critical, but this is suppressed by our CR cuts
  \end{itemize} 
  )", "--size -1");
  myTest.FinishSlide();

  //Start Backup
  myTest.StartBackup();

  //Backup
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
