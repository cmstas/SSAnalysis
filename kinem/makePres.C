#include "/home/users/cgeorge/software/SlideMaker/SlideMaker.h"

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

  //Third slide
  myTest.NewSlide();
  myTest.Plot("leppt_OS.pdf"); 
  myTest.FinishSlide();

  //Fourth slide
  myTest.NewSlide();
  myTest.Plot("MET_OS.pdf"); 
  myTest.FinishSlide();

  //Fifth slide
  myTest.NewSlide();
  myTest.Plot("HT_OS.pdf"); 
  myTest.FinishSlide();

  //Sixth slide
  myTest.NewSlide();
  myTest.Plot("mtmin_OS.pdf"); 
  myTest.FinishSlide();

  //Seventh slide
  myTest.NewSlide();
  myTest.Plot("nJets_OS.pdf"); 
  myTest.FinishSlide();

  //Eighth slide
  myTest.NewSlide();
  myTest.Plot("nBtags_OS.pdf"); 
  myTest.FinishSlide();

  //mll slide
  myTest.NewSlide();
  myTest.Plot("mll_OS.pdf"); 
  myTest.FinishSlide();

  //mht slide
  myTest.NewSlide();
  myTest.Plot("mht_OS.pdf"); 
  myTest.FinishSlide();

 //////////////////////
 //        SS        //
 //////////////////////

  myTest.NewSlide();
  myTest.Plot("leppt.pdf"); 
  myTest.FinishSlide();

  //Fourth slide
  myTest.NewSlide();
  myTest.Plot("MET.pdf"); 
  myTest.FinishSlide();

  //Fifth slide
  myTest.NewSlide();
  myTest.Plot("HT.pdf"); 
  myTest.FinishSlide();

  //Sixth slide
  myTest.NewSlide();
  myTest.Plot("mtmin.pdf"); 
  myTest.FinishSlide();

  //Seventh slide
  myTest.NewSlide();
  myTest.Plot("nJets.pdf"); 
  myTest.FinishSlide();

  //Eighth slide
  myTest.NewSlide();
  myTest.Plot("nBtags.pdf"); 
  myTest.FinishSlide();

  //mll slide
  myTest.NewSlide();
  myTest.Plot("mll.pdf"); 
  myTest.FinishSlide();

  //mht slide
  myTest.NewSlide();
  myTest.Plot("mht.pdf"); 
  myTest.FinishSlide();

}
