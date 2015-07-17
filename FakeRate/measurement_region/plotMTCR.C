{

  gROOT->Reset();

  gStyle->SetOptStat(0);

  TCanvas c1("c1","c1",600,600);

  bool doMu = false;

  TString plot = (doMu ? "histo_mt_cr_mu" : "histo_mt_cr_el");

  TFile *_file_wj = TFile::Open("rate_histos_wj_LooseEMVA.root");
  TFile *_file_dy = TFile::Open("rate_histos_dy_LooseEMVA.root");

  TFile *_file_data = TFile::Open("rate_histos_data_LooseEMVA.root");

  TH1F* h_data = (TH1F*) _file_data->Get(plot);
  TH1F* h_wj = (TH1F*) _file_wj->Get(plot);
  TH1F* h_dy = (TH1F*) _file_dy->Get(plot);

  if (doMu) {
    h_wj->Scale(4.5);
    h_dy->Scale(4.5);
  } else{
    h_wj->Scale(0.8);
    h_dy->Scale(0.8);
  }

  h_wj->SetFillColor(kRed+2);
  h_dy->SetFillColor(kYellow+2);

  THStack* h_ewk = new THStack("hs","hs");
  h_ewk->Add(h_wj);
  h_ewk->Add(h_dy);

  h_data->SetMarkerStyle(kFullCircle);

  h_data->SetTitle("");
  h_data->GetXaxis()->SetTitle("MT [GeV]");
  h_data->GetYaxis()->SetTitle("events/bin");
  h_data->GetYaxis()->SetTitleOffset(1.2);

  h_data->Draw("PE");
  h_ewk->Draw("same,hist");
  h_data->Draw("same");

  TLegend* leg = new TLegend(0.7,0.7,0.89,0.89);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetHeader("L~O(10)/pb");
  leg->AddEntry(h_data,"data","pe");
  leg->AddEntry(h_wj  ,"W+jets","f");
  leg->AddEntry(h_dy  ,"DY","f");
  leg->Draw();

  c1.SaveAs(plot+".png");

}
