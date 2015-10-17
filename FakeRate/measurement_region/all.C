{

    gROOT->ProcessLine(".L ../../commonUtils.h");
    gROOT->ProcessLine(".L normalizeZpeak.C");
    gROOT->ProcessLine(".L plotMTCR.C");
    gROOT->ProcessLine(".L make1DplotFR.C");
    gROOT->ProcessLine(".L plotEWKCorFR.C");

    float intlumi = getLumi();
    cout << "intlumi=" << intlumi << "/fb" << endl;

    cout << "non isolated triggers" << endl;

    pair<float,float> zsfs = normalizeZpeak(intlumi, false);
    float zsfEl = zsfs.first;
    float zsfMu = zsfs.second;
    cout << "ZP SF: " << zsfEl << " " << zsfMu << endl;

    pair<float,float> mtsfs = plotMTCR(zsfEl, zsfMu, false);
    float mtsfEl = mtsfs.first;
    float mtsfMu = mtsfs.second;
    cout << "MT SF: " << mtsfEl << " " << mtsfMu << endl;

    make1DplotFR(zsfEl, zsfMu, mtsfEl, mtsfMu, false);
    plotEWKCorFR(mtsfEl, mtsfMu, false);

    gROOT->ProcessLine(".x plotFR.C");
    gROOT->ProcessLine(".x trigStudyEl.C");
    gROOT->ProcessLine(".x trigStudyMu.C");

    cout << "isolated triggers" << endl;

    pair<float,float> zsfs_i = normalizeZpeak(intlumi, true);
    zsfEl = zsfs_i.first;
    zsfMu = zsfs_i.second;
    cout << "ZP SF: " << zsfEl << " " << zsfMu << endl;

    pair<float,float> mtsfs_i = plotMTCR(zsfEl, zsfMu, true);
    mtsfEl = mtsfs_i.first;
    mtsfMu = mtsfs_i.second;
    cout << "MT SF: " << mtsfEl << " " << mtsfMu << endl;

    make1DplotFR(zsfEl, zsfMu, mtsfEl, mtsfMu, true);
    plotEWKCorFR(mtsfEl, mtsfMu, true);

}
