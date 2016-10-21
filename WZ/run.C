{
    gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
    gSystem->Load("../CORE/CMS3_CORE.so");
    gROOT->ProcessLine(".L ../classFiles/v6.02/SS.cc+");

    gROOT->ProcessLine(".L scan.C+");
    // gROOT->ProcessLine(".L scan_rares.C+");
    scan();

    // gROOT->ProcessLine(".L srscan.C+");
    // srscan();

}
