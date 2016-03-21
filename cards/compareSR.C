{

  gStyle->SetOptStat(0);
  gStyle->SetHistLineWidth(2);
  gStyle->SetLabelSize(0.03, "XY");
  gStyle->SetTitleSize(0.03, "XY");
  gStyle->SetTitleOffset(1.5, "XY");

  TCanvas c1("c1","c1",800,800);

  TString kr = "hihi";
  TString mglu = "900";
  TString mlsp = "700";

  TFile* f_orig = TFile::Open("v6.02-statunc/fs_t5tttt_degen_m"+mglu+"_m"+mlsp+"_histos_"+kr+"_2.3ifb.root");
  TH1F* sr_orig = (TH1F*) f_orig->Get("sr");

  TFile* f_test = TFile::Open("v6.02/fs_t5tttt_degen_test_m"+mglu+"_m"+mlsp+"_histos_"+kr+"_2.3ifb.root");
  TH1F* sr_test = (TH1F*) f_test->Get("sr");

  sr_orig->SetLineColor(kRed);
  sr_orig->SetTitle("");
  sr_orig->GetXaxis()->SetTitle("sr "+kr+"");
  sr_orig->GetYaxis()->SetRangeUser(0,1.4*TMath::Max(sr_orig->GetMaximum(),sr_test->GetMaximum()));
  sr_orig->Draw();

  sr_test->SetLineColor(kBlack);
  sr_test->Draw("same");

  TLegend* l = new TLegend(0.45, 0.70, 0.80, 0.90);
  l->SetTextFont(42);
  l->SetShadowColor(kWhite);
  l->SetFillColor(kWhite);
  l->SetHeader("m(#tilde{g})="+mglu+" GeV, m(#tilde{#chi^{0}})="+mlsp+" GeV");
  l->AddEntry(sr_orig , "official", "l");
  l->AddEntry(sr_test , "private", "l");
  l->Draw();

  c1.SaveAs("t5tttt_degen_m"+mglu+"_m"+mlsp+"_histos_"+kr+".png");

}
