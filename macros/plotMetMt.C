{

  gROOT->Reset();

  TString sample = "qcd";
  TString postfix = "_pt-80to120";

  TFile* f = TFile::Open(sample+"_histos"+postfix+".root");

  gStyle->SetOptStat(0);
  TCanvas c1;
  c1.SetLogz();
  TH2F* evt_mt_vs_met = (TH2F*) f->Get("evt_mt_vs_met");
  evt_mt_vs_met->Scale(1./evt_mt_vs_met->Integral());
  evt_mt_vs_met->GetZaxis()->SetRangeUser(0.0001,0.1);
  evt_mt_vs_met->Draw("colz");
  c1.SaveAs("mt_vs_met_13TeV.png");

  gStyle->SetOptStat(1);
  TCanvas c2;
  TH1F* evt_mt = (TH1F*) f->Get("evt_mt");
  evt_mt->Scale(1./evt_mt->Integral());
  evt_mt->Draw("hist");
  c2.SaveAs("mt_13TeV.png");

  TCanvas c3;
  TH1F* evt_met = (TH1F*) f->Get("evt_met");
  evt_met->Scale(1./evt_met->Integral());
  evt_met->Draw("hist");
  c3.SaveAs("met_13TeV.png");

  cout << "cut_met effic=" << evt_met->Integral(1,2)/evt_met->Integral() << endl;
  cout << "cut_mt effic="  << evt_mt->Integral(1,2)/evt_mt->Integral() << endl;

  TH1F* pass_metmt = (TH1F*) f->Get("pass_metmt");
  cout << "cut effic=" << pass_metmt->GetMean() << endl;

  TCanvas c4;
  TH1F* npu_true = (TH1F*) f->Get("npu_true");
  npu_true->Scale(1./npu_true->Integral());
  npu_true->Draw("hist");
  c4.SaveAs("npu_13TeV.png");

}
