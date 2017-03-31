{
    gROOT->ProcessLine(".L ../../software/dataMCplotMaker/dataMCplotMaker.cc+");
    gROOT->ProcessLine(".L /home/users/namin/scripts/cat.C");
    gROOT->ProcessLine(".L makePlots.C");
    makePlots();
    // gSystem->Exec("ic plots/LLAGGSR.pdf");

}
