{

  gROOT->ProcessLine(".L ../../software/dataMCplotMaker/PlotMaker2D.cc++");
  gROOT->ProcessLine(".L ../../classFiles/v4.00/SS.cc+");
  gROOT->ProcessLine(".L Flip.C+");
  flip(true); // mu
  flip(false);  // ele

  // flip(false, true, false);  // ele DY only
  // flip(false, false, true);  // ele TTbar only

}
