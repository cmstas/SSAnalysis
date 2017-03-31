{
    gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
    gSystem->Load("../CORE/CMS3_CORE.so");
    gROOT->ProcessLine(".L ../classFiles/v6.02/SS.cc+");

    gROOT->ProcessLine(".L scan.C+");
    // gROOT->ProcessLine(".L scan_ra7.C+");
    // gROOT->ProcessLine(".L blah.C+");
    // gROOT->ProcessLine(".L debug.C+");
    // gROOT->ProcessLine(".L debug_layered.C+");
    // gROOT->ProcessLine(".L scan_rares.C+");
    // gROOT->ProcessLine(".L scan_synch.C+");
    // gROOT->ProcessLine(".L scan_synchMoriond.C+");
    scan();

    // gROOT->ProcessLine(".L srscan.C+");
    // srscan();

}
