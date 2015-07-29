#include "/home/users/cgeorge/software/SlideMaker/SlideMaker.h"
#include "TString.h"

void makePres(){

  pres myTest("blue"); 
  myTest.Underline("Alex");
  myTest.TitleSlide("Data vs. MC Plots in SS CRs");

  //Second slide
  myTest.NewSlide();
  myTest.Title("Technical Details.");
  myTest.AllText(); 
  myTest.Text(R"(
  \begin{itemize} 
  \item Events selected are SS (SS+SS or SS+FO) or OS events selected for any CR and any HH/HL/LL.
  \item Run-2 MC samples are used whenever available, otherwise PHYS14 samples are used (only ttV)
  \item We apply ``our'' triggers to both data and MC (except for phys14 samples)
  \item Jet corrections are ``out of the box'' (not applied by us) -- I have the JECs stored though.
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
      myTest.Plot(Form("%s%s_OS.pdf", out[i].c_str(), type[j].c_str() )); 
      myTest.FinishSlide();
    }
  }
  for (unsigned int i = 0; i < 8; i++){
    for (unsigned int j = 0; j < 3; j++){
      myTest.NewSlide();
      myTest.Plot(Form("%s%s.pdf", out[i].c_str(), type[j].c_str() )); 
      myTest.FinishSlide();
    }
  }

}
