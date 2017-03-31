TH1F* reduceBins(TH1F* h_in, int nbins) {
    TH1F* h_tmp = new TH1F(h_in->GetName(), h_in->GetTitle(), nbins, 0.5, nbins+0.5);
    for (int ibin = 1; ibin < h_in->GetNbinsX()+1; ibin++) {
        h_tmp->SetBinContent( ibin, h_in->GetBinContent(ibin) );
        h_tmp->SetBinError( ibin, h_in->GetBinError(ibin) );
    }
    return h_tmp;
}


void makePlots() {

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


    TFile *f1 = new TFile("mlfitname.root");
    TString lumistr = "35.9";


    std::vector<TString> types = {"shapes_fit_b", "shapes_prefit"};
    for(auto type : types) {
    
    for (int ikine = 0; ikine < 3; ikine++) {

        TString kine = "hihi"; TString shortkine = "HH"; int Nbins = 51;

        if (ikine == 0) { kine = "hihi";   shortkine = "HH"; Nbins = 51; }
        if (ikine == 1) { kine = "hilow";  shortkine = "HL"; Nbins = 41; }
        if (ikine == 2) { kine = "lowlow"; shortkine = "LL"; Nbins = 8;  }

        std::cout << ">>> Making plots from " << type << " for " << shortkine << std::endl;

        // TString type = "shapes_fit_b";
        // TString type = "shapes_prefit";
        TGraphAsymmErrors* g_data = (TGraphAsymmErrors*)(f1->Get(Form("%s/ch%i/data", type.Data(), ikine+1)));
        TH1F* h_bg_total = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/total_background", type.Data(), ikine+1))), Nbins);
        TH1F* h_fakes = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/fakes", type.Data(), ikine+1))), Nbins);
        TH1F* h_flips = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/flips", type.Data(), ikine+1))), Nbins);
        TH1F* h_rares = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/rares", type.Data(), ikine+1))), Nbins);
        TH1F* h_ttw = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/ttw", type.Data(), ikine+1))), Nbins);
        TH1F* h_ttzh = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/ttzh", type.Data(), ikine+1))), Nbins);
        TH1F* h_ww = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/ww", type.Data(), ikine+1))), Nbins);
        TH1F* h_wz = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/wz", type.Data(), ikine+1))), Nbins);
        TH1F* h_xg = reduceBins((TH1F*)(f1->Get(Form("%s/ch%i/xg", type.Data(), ikine+1))), Nbins);

        h_bg_total->SetLineColor(0);
        h_bg_total->SetMarkerSize(0);

        TH1F* h_data = new TH1F("h_data"+kine, "h_data", Nbins, 0.5, Nbins+0.5);
        for (int ipt = 0; ipt < g_data->GetN(); ipt++) {
            double x,y;
            g_data->GetPoint(ipt, x, y);
            h_data->SetBinContent(ipt+1, y);
        }

        TString extra = " ";
        vector<TH1F*> sr_plots = {
            h_ttw,
            h_ttzh,
            h_wz,
            h_ww,
            h_xg,
            h_rares,
            h_flips,
            h_fakes,
        };

        TString prepost = type.Contains("prefit") ? "prefit": "postfit";

        std::vector<std::string> opts = {
            Form("--lumi %s --outputName plots/%s%sSR.pdf     --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08 --noOverflow --systInclStat  --outOfFrame --legendTaller 0.15 --yTitleOffset -0.5 --dataName Data %s --type Preliminary  --topYaxisTitle Pull --doPull ", lumistr.Data(), prepost.Data(), shortkine.Data(), extra.Data()),
            Form("--lumi %s --outputName plots/%s%sSR_log.pdf   --setMinimum 0.03  --xAxisLabel SR --noXaxisUnit  --legendUp -.15 --legendRight -0.08 --noOverflow --systInclStat  --outOfFrame --legendTaller 0.15 --yTitleOffset -0.5 --dataName Data %s --type Preliminary  --topYaxisTitle Pull --doPull ", lumistr.Data(), prepost.Data(), shortkine.Data(), extra.Data()),
        };

        for (auto opt : opts) {
        dataMCplotMaker(h_data, sr_plots, titles, Form("%s SR", type.Contains("prefit")?"Pre-fit":"Post-fit"), "", opt,  vector <TH1F*>(), vector <string>(), colors, h_bg_total);
        }

    }

}

}
