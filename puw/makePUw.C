{

    // central
    TFile *f_central = TFile::Open("pileup_central_4ifb.root");
    TH1F* vtx_central = (TH1F*) f_central->Get("pileup");
    vtx_central->Scale(1.0/vtx_central->Integral());
    std::cout << vtx_central->Integral() << std::endl;

    // up
    TFile *f_up = TFile::Open("pileup_up_4ifb.root");
    TH1F* vtx_up = (TH1F*) f_up->Get("pileup");
    vtx_up->Scale(1.0/vtx_up->Integral());
    std::cout << vtx_up->Integral() << std::endl;

    // down
    TFile *f_down = TFile::Open("pileup_down_4ifb.root");
    TH1F* vtx_down = (TH1F*) f_down->Get("pileup");
    vtx_down->Scale(1.0/vtx_down->Integral());
    std::cout << vtx_down->Integral() << std::endl;

    // MC
    TFile *f_mc = TFile::Open("nvtx_ratio_4p0fb_vince.root");
    TH1F* vtx_mc = (TH1F*) f_mc->Get("h_zjet");
    vtx_mc->Scale(1.0/vtx_mc->Integral());
    std::cout << vtx_mc->Integral() << std::endl;

    TFile *fout = new TFile("puw_4ifb_var.root", "RECREATE");

    TH1F* puw_central = (TH1F*) vtx_central->Clone("puw_central");
    TH1F* puw_up = (TH1F*) vtx_up->Clone("puw_up");
    TH1F* puw_down = (TH1F*) vtx_down->Clone("puw_down");

    puw_central->Divide(vtx_mc);
    puw_up->Divide(vtx_mc);
    puw_down->Divide(vtx_mc);
    fout->Write();

    cout << "float getTruePUw(int nvtx) {" << endl;
    for (int bin=1;bin<=puw_central->GetNbinsX();++bin) {
        cout << Form("   if (nvtx>=%f && nvtx<%f) return %f;",puw_central->GetBinLowEdge(bin),puw_central->GetBinLowEdge(bin+1),puw_central->GetBinContent(bin) ) << endl;
    }
    cout << "   return 0.;" << endl << "}" << endl;

    cout << "float getTruePUwUp(int nvtx) {" << endl;
    for (int bin=1;bin<=puw_up->GetNbinsX();++bin) {
        cout << Form("   if (nvtx>=%f && nvtx<%f) return %f;",puw_up->GetBinLowEdge(bin),puw_up->GetBinLowEdge(bin+1),puw_up->GetBinContent(bin) ) << endl;
    }
    cout << "   return 0.;" << endl << "}" << endl;

    cout << "float getTruePUwDn(int nvtx) {" << endl;
    for (int bin=1;bin<=puw_down->GetNbinsX();++bin) {
        cout << Form("   if (nvtx>=%f && nvtx<%f) return %f;",puw_down->GetBinLowEdge(bin),puw_down->GetBinLowEdge(bin+1),puw_down->GetBinContent(bin) ) << endl;
    }
    cout << "   return 0.;" << endl << "}" << endl;

}