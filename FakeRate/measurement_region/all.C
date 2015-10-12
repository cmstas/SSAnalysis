{
    gROOT->ProcessLine(".L normalizeZpeak.C");
    gROOT->ProcessLine(".L plotMTCR.C");
    gROOT->ProcessLine(".L make1DplotFR.C");
    gROOT->ProcessLine(".L plotEWKCorFR.C");

    pair<float,float> zsfs = normalizeZpeak();
    zsfEl = zsfs.first;
    zsfMu = zsfs.second;
    cout << "ZP SF: " << zsfEl << " " << zsfMu << endl;

    pair<float,float> mtsfs = plotMTCR(zsfEl, zsfMu);
    mtsfEl = mtsfs.first;
    mtsfMu = mtsfs.second;
    cout << "MT SF: " << mtsfEl << " " << mtsfMu << endl;

    make1DplotFR(zsfEl, zsfMu, mtsfEl, mtsfMu);
    plotEWKCorFR(mtsfEl, mtsfMu);

    gROOT->ProcessLine(".x plotFR.C");
    gROOT->ProcessLine(".x trigStudyEl.C");
    gROOT->ProcessLine(".x trigStudyMu.C");

}
