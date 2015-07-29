{

  gROOT->Reset();

  gStyle->SetOptStat(0);
  TGaxis::SetMaxDigits(3);

  TCanvas c1("c1","c1",600,600);

  bool doMu = 1;
  TString plot = "met_all";
  bool applyInputSF  = 1;
  bool applyOutputSF = 0;

  if (doMu) plot = "histo_"+plot+"_mu";
  else plot = "histo_"+plot+"_el";

  TFile *_file_wj = TFile::Open("rate_histos_wj_LooseEMVA.root");
  TFile *_file_dy = TFile::Open("rate_histos_dy_LooseEMVA.root");

  TFile *_file_data = TFile::Open("rate_histos_data_LooseEMVA.root");

  TH1F* h_data = (TH1F*) _file_data->Get(plot);
  TH1F* h_wj = (TH1F*) _file_wj->Get(plot);
  TH1F* h_dy = (TH1F*) _file_dy->Get(plot);

  float inputSF_mu = 0.93;
  float inputSF_el = 0.44;

  if (applyInputSF) {
    if (doMu) {
      h_wj->Scale(inputSF_mu);
      h_dy->Scale(inputSF_mu);
    } else {
      h_wj->Scale(inputSF_el);
      h_dy->Scale(inputSF_el);
    }
  }

  if (applyOutputSF) {
    float ewk60mt100 = h_wj->Integral(h_wj->FindBin(60),h_wj->FindBin(100)) + h_dy->Integral(h_dy->FindBin(60),h_dy->FindBin(100));
    float data60mt100 = h_data->Integral(h_data->FindBin(60),h_data->FindBin(100));
    cout << "ewk=" << ewk60mt100 << " data=" << data60mt100 << " sf=" << data60mt100/ewk60mt100 << endl;
    h_wj->Scale(data60mt100/ewk60mt100);
    h_dy->Scale(data60mt100/ewk60mt100);
  }

  h_wj->SetFillColor(kRed+2);
  h_dy->SetFillColor(kYellow+2);

  THStack* h_ewk = new THStack("hs","hs");
  h_ewk->Add(h_wj);
  h_ewk->Add(h_dy);

  h_data->SetMarkerStyle(kFullCircle);

  h_data->SetTitle("");
  h_data->GetXaxis()->SetTitle("MT [GeV]");
  if (plot.Contains("met")) h_data->GetXaxis()->SetTitle("MET [GeV]");
  h_data->GetYaxis()->SetTitle("events/bin");
  h_data->GetYaxis()->SetTitleOffset(1.2);

  h_data->Draw("PE");
  h_ewk->Draw("same,hist");
  h_data->Draw("same");  

  TLegend* leg = new TLegend(0.7,0.7,0.89,0.89);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetHeader("L=40/pb");
  leg->AddEntry(h_data,"data","pe");
  leg->AddEntry(h_wj  ,"W+jets","f");
  leg->AddEntry(h_dy  ,"DY","f");
  leg->Draw();

  c1.SaveAs(plot+".png");

}
