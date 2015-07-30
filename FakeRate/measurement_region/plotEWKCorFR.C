{

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");

  bool doMu = 0;

  TString suffix = (doMu ? "_mu" : "_e");
  TString lepname = (doMu ? "muon" : "electron");

  TCanvas c1("c1","c1",600,600);

  TFile *_file_data = TFile::Open("rate_histos_data_LooseEMVA.root");
  TFile *_file_wj = TFile::Open("rate_histos_wj_LooseEMVA.root");
  TFile *_file_dy = TFile::Open("rate_histos_dy_LooseEMVA.root");

  TH2F* den_data = (TH2F*) _file_data->Get("Nl_cone_histo"+suffix);
  TH2F* den_wj = (TH2F*) _file_wj->Get("Nl_cone_histo"+suffix);
  TH2F* den_dy = (TH2F*) _file_dy->Get("Nl_cone_histo"+suffix);

  //scale factors from MT control region plot
  float muSF = 0.54;
  float elSF = 0.42;
  float ewkSF = (doMu ? muSF : elSF);

  den_data->Add(den_wj,-1.*ewkSF);
  den_data->Add(den_dy,-1.*ewkSF);

  TH2F* num_data = (TH2F*) _file_data->Get("Nt_histo"+suffix);
  TH2F* num_wj = (TH2F*) _file_wj->Get("Nt_histo"+suffix);
  TH2F* num_dy = (TH2F*) _file_dy->Get("Nt_histo"+suffix);

  num_data->Add(num_wj,-1.*ewkSF);
  num_data->Add(num_dy,-1.*ewkSF);

  num_data->Divide(num_data, den_data, 1, 1, "B");

  num_data->SetTitle("EWK-corrected "+lepname+" fake rate");
  num_data->GetXaxis()->SetTitle(""+lepname+" p_{T}^{corr.} [GeV]");
  num_data->GetYaxis()->SetTitle(""+lepname+" |#eta|");
  num_data->GetYaxis()->SetTitleOffset(1.2);

  num_data->GetZaxis()->SetRangeUser(0,0.5);
  num_data->GetZaxis()->SetLabelSize(0.025);

  num_data->SetMarkerSize(1.5);

  num_data->Draw("textecolz");

  c1.SaveAs("ewkCorFR_"+lepname+".png");

}
