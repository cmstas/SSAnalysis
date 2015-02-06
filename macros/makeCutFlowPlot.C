{


  gROOT.Reset();
  gStyle.SetOptStat(0);
  TString dir = "results_PHYS14";
  TString plot = "cut_flow_ss";
  bool norm = true;
  bool save = true;
  bool logy = false;

  TCanvas c1;
  if (logy) c1.SetLogy();

  c1.SetGridx();
  c1.SetGridy();

  float maxy = -1.;

  TFile *T1ttttG1500_file = TFile::Open(dir+"/T1ttttG1500_histos.root");
  TH1F* T1ttttG1500_h = (TH1F*) T1ttttG1500_file->Get(plot);
  TH1F* h0 = T1ttttG1500_h;
  T1ttttG1500_h->SetLineColor(kRed);
  T1ttttG1500_h->SetLineStyle(1);
  T1ttttG1500_h->SetLineWidth(2);
  if (norm) T1ttttG1500_h->Scale(1./T1ttttG1500_h->GetBinContent(1));

  T1ttttG1500_h->GetXaxis()->SetRangeUser(0,11);
  T1ttttG1500_h->GetXaxis()->SetBinLabel(1,"AllSS");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(2,"GoodVtx");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(3,"N_{FO}>0");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(4,"MET30||HT500");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(5,"N_{FO}>=2");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(6,"MLL8");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(7,"BASELINE");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(8,"3LVETO");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(9,"FULL ID");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(10,"PT20/20");
  T1ttttG1500_h->GetXaxis()->SetBinLabel(11,"SAMESIGN");

  T1ttttG1500_h->Draw("HIST");
  if (T1ttttG1500_h && T1ttttG1500_h->GetMaximum()>maxy) maxy = T1ttttG1500_h->GetMaximum();

  TFile *T1ttttG1200_file = TFile::Open(dir+"/T1ttttG1200_histos.root");
  TH1F* T1ttttG1200_h = (TH1F*) T1ttttG1200_file->Get(plot);
  T1ttttG1200_h->SetLineColor(kRed);
  T1ttttG1200_h->SetLineStyle(7);
  T1ttttG1200_h->SetLineWidth(2);
  if (norm) T1ttttG1200_h->Scale(1./T1ttttG1200_h->GetBinContent(1));
  T1ttttG1200_h->Draw("SAME,HIST");
  if (T1ttttG1200_h && T1ttttG1200_h->GetMaximum()>maxy) maxy = T1ttttG1200_h->GetMaximum();

  // TFile *T5Full1500_file = TFile::Open(dir+"/T5Full1500_histos.root");
  // TH1F* T5Full1500_h = (TH1F*) T5Full1500_file->Get(plot);
  // if (T5Full1500_h && T5Full1500_h->GetEntries()>20) {
  //   T5Full1500_h->SetLineColor(kBlack);
  //   T5Full1500_h->SetLineStyle(1);
  //   T5Full1500_h->SetLineWidth(2);
  //   if (norm) T5Full1500_h->Scale(1./T5Full1500_h->GetBinContent(1));
  //   T5Full1500_h->Draw("SAME,HIST");
  //   if (T5Full1500_h && T5Full1500_h->GetMaximum()>maxy) maxy = T5Full1500_h->GetMaximum();
  // }

  // TFile *T5Full1200_file = TFile::Open(dir+"/T5Full1200_histos.root");
  // TH1F* T5Full1200_h = (TH1F*) T5Full1200_file->Get(plot);
  // if (T5Full1200_h && T5Full1200_h->GetEntries()>20) {
  //   T5Full1200_h->SetLineColor(kBlack);
  //   T5Full1200_h->SetLineStyle(7);
  //   T5Full1200_h->SetLineWidth(2);
  //   T5Full1200_h->Draw("SAME,HIST");
  //   if (norm) T5Full1200_h->Scale(1./T5Full1200_h->GetBinContent(1));
  //   T5Full1200_h->Draw("SAME,HIST");
  //   if (T5Full1200_h && T5Full1200_h->GetMaximum()>maxy) maxy = T5Full1200_h->GetMaximum();
  // }

  TFile *TTWJets_file = TFile::Open(dir+"/TTWJets_histos.root");
  TH1F* TTWJets_h = (TH1F*) TTWJets_file->Get(plot);
  if (TTWJets_h && TTWJets_h->GetEntries()>20) {
    TTWJets_h->SetLineColor(kOrange+1);
    TTWJets_h->SetLineStyle(1);
    TTWJets_h->SetLineWidth(2);
    TTWJets_h->Draw("SAME,HIST");
    if (norm) TTWJets_h->Scale(1./TTWJets_h->GetBinContent(1));
    TTWJets_h->Draw("SAME,HIST");
    if (TTWJets_h && TTWJets_h->GetMaximum()>maxy) maxy = TTWJets_h->GetMaximum();
  }

  if (norm) {
    if (logy) h0->GetYaxis()->SetRangeUser(0.001,1.1*maxy);
    else h0->GetYaxis()->SetRangeUser(0.,1.1*maxy);
  }

  TLegend* leg  = new TLegend(0.73,0.54,0.89,0.89);
  leg->SetNColumns(1);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  if (TTWJets_h)  leg->AddEntry(TTWJets_h,"TTW","L");
  if (T1ttttG1500_h) leg->AddEntry(T1ttttG1500_h,"T1_1500","L");
  if (T1ttttG1200_h) leg->AddEntry(T1ttttG1200_h,"T1_1200","L");
  // if (T5Full1500_h)  leg->AddEntry(T5Full1500_h,"T5_1500","L");
  // if (T5Full1200_h)  leg->AddEntry(T5Full1200_h,"T5_1200","L");
  leg->Draw();

  if (save) c1.SaveAs(dir+"/"+plot+".png");



}
