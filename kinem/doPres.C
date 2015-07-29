{
  gROOT->ProcessLine(".L ../software/SlideMaker/SlideMaker.cc+"); 
  gROOT->ProcessLine(".L makePres.C+");
  gROOT->ProcessLine("makePres()");
}
