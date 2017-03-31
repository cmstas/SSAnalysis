
{
    // TString indir = "agg_test/";
    // TString indir = "v8.07_Dec1_17p3_rereco_agg/";
    // TString indir = "v8.07_Dec5_17p3_v2/";
    // TString indir = "v8.07_Dec15_36p5_nobtagsf/";
    // TString indir = "v9.04_Jan31_36p8_t1tttt_exclagg/";
    TString indir = "v9.04_Jan31_36p8_blah_exclagg/";
    // TString lumistr = "17.3";
    TString lumistr = "36.8";

    gROOT->ProcessLine(".L ../software/dataMCplotMaker/dataMCplotMaker.cc+");
    // gROOT->ProcessLine(".L testPlot.C+");
    vector <string> titles;
    titles.push_back("t#bar{t}W"); 
    titles.push_back("t#bar{t}Z/H"); 
    titles.push_back("WZ"); 
    titles.push_back("WW"); 
    titles.push_back("X+#gamma"); 
    titles.push_back("Rare SM"); 
    titles.push_back("Charge misid."); 
    titles.push_back("Nonprompt lep."); 

    vector <Color_t> colors;
    colors.push_back(kGreen+3); 
    colors.push_back(kGreen-6); 
    colors.push_back(kOrange); 
    colors.push_back(kOrange-3); 
    colors.push_back(kViolet+2); 
    colors.push_back(kMagenta-7); 
    colors.push_back(kGray+2); 
    colors.push_back(18); 

    TString extra = " --poissonErrorsNoZeros ";

    // for (int ikine = 0; ikine < 3; ikine++) {
    for (int ikine = 0; ikine < 2; ikine++) { // FIXME no LL?

        TString kine = "hihi";
        if (ikine == 1) kine = "hilow";
        if (ikine == 2) kine = "lowlow";

        float roughSystTTW   = 0.2;
        float roughSystTTZH  = 0.2;
        float roughSystWZ    = 0.3;
        float roughSystWW    = 0.3;
        float roughSystXG    = 0.5;
        float roughSystRARES = 0.5;
        float roughSystFLIPS = 0.3;
        float roughSystFAKES = 0.35;
        float roughSystFAKESHH = 0.40;
        float roughSystFAKESXL = 0.30;


        TFile *f1 = new TFile(Form("%s/data_histos_%s_%sifb.root", indir.Data(),kine.Data(),lumistr.Data()));
        TFile *f2 = new TFile(Form("%s/ttw_histos_%s_%sifb.root",  indir.Data(),kine.Data(),lumistr.Data()));
        TFile *f3 = new TFile(Form("%s/ttzh_histos_%s_%sifb.root", indir.Data(),kine.Data(),lumistr.Data()));
        TFile *f4 = new TFile(Form("%s/wz_histos_%s_%sifb.root",   indir.Data(),kine.Data(),lumistr.Data()));
        TFile *f5 = new TFile(Form("%s/ww_histos_%s_%sifb.root",   indir.Data(),kine.Data(),lumistr.Data()));
        TFile *f6 = new TFile(Form("%s/xg_histos_%s_%sifb.root",   indir.Data(),kine.Data(),lumistr.Data()));
        TFile *f7 = new TFile(Form("%s/rares_histos_%s_%sifb.root",indir.Data(),kine.Data(),lumistr.Data()));
        TFile *f8 = new TFile(Form("%s/flips_histos_%s_%sifb.root",indir.Data(),kine.Data(),lumistr.Data()));
        TFile *f9 = new TFile(Form("%s/fakes_histos_%s_%sifb.root",indir.Data(),kine.Data(),lumistr.Data()));
        TH1F *h1 = (TH1F*)f1->Get("sr");
        TH1F *h2 = (TH1F*)f2->Get("sr");
        TH1F *h3 = (TH1F*)f3->Get("sr");
        TH1F *h4 = (TH1F*)f4->Get("sr");
        TH1F *h5 = (TH1F*)f5->Get("sr");
        TH1F *h6 = (TH1F*)f6->Get("sr");
        TH1F *h7 = (TH1F*)f7->Get("sr");
        TH1F *h8 = (TH1F*)f8->Get("sr");
        TH1F *h9 = (TH1F*)f9->Get("sr");

        std::cout << h1->GetEntries() << std::endl;

        std::cout << h1 << std::endl;
        std::cout << h2 << std::endl;
        std::cout << h3 << std::endl;
        std::cout << h4 << std::endl;
        std::cout << h5 << std::endl;
        std::cout << h6 << std::endl;
        std::cout << h7 << std::endl;
        std::cout << h8 << std::endl;
        std::cout << h9 << std::endl;

        // // testPlot();
        // // gROOT->ProcessLine(".! web h1D_vec.pdf")
        // // Overlapping Aggregate Regions

        vector<pair<TH1F*, float> > SRAGG_plots;
        SRAGG_plots.push_back(pair<TH1F*, float>(h2  , roughSystTTW     ));
        SRAGG_plots.push_back(pair<TH1F*, float>(h3 , roughSystTTZH    ));
        SRAGG_plots.push_back(pair<TH1F*, float>(h4   , roughSystWZ      ));
        SRAGG_plots.push_back(pair<TH1F*, float>(h5   , roughSystWW      ));
        SRAGG_plots.push_back(pair<TH1F*, float>(h6   , roughSystXG      ));
        SRAGG_plots.push_back(pair<TH1F*, float>(h7, roughSystRARES   ));
        SRAGG_plots.push_back(pair<TH1F*, float>(h8, roughSystFLIPS   ));
        SRAGG_plots.push_back(pair<TH1F*, float>(h9, roughSystFAKESHH ));

        TString shortkine = "HH";
        if (kine == "hilow") shortkine = "HL";
        if (kine == "lowlow") shortkine = "LL";

        std::string opts  = Form("--lumi %s --outputName plots/%sexclAGGSR.pdf     --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08 --noOverflow --systInclStat --noRatioPlot --outOfFrame --legendTaller 0.15 --yTitleOffset -0.5 --dataName Data %s --type Preliminary   --xAxisBinLabels 1,2,3,4,5,6,7,8,9,10 ", lumistr.Data(), shortkine.Data(), extra.Data());
        std::string opts2 = Form("--lumi %s --outputName plots/%sexclAGGSR_log.pdf --xAxisLabel SR --noXaxisUnit            --legendUp -.15 --legendRight -0.08 --noOverflow --systInclStat --noRatioPlot --outOfFrame --legendTaller 0.15 --yTitleOffset -0.5 --dataName Data %s --type Preliminary  --xAxisBinLabels 1,2,3,4,5,6,7,8,9,10  ", lumistr.Data(), shortkine.Data(), extra.Data());
        dataMCplotMaker(h1, SRAGG_plots, titles, "Excl. Aggregate SRs", "", opts,  vector <TH1F*>(), vector <string>(), colors);
        dataMCplotMaker(h1, SRAGG_plots, titles, "Excl. Aggregate SRs", "", opts2, vector <TH1F*>(), vector <string>(), colors);

    }

}
