{

  int updown = 2;

  TFile *_fdata = TFile::Open("MyDataPileupHistogram_69.root");
  if (updown==1) _fdata = TFile::Open("MyDataPileupHistogram_72.root");
  if (updown==2) _fdata = TFile::Open("MyDataPileupHistogram_65.root");
  TH1D* hdata = (TH1D*) _fdata->Get("pileup");
  hdata->Scale(1./hdata->Integral());

  TFile *_fmc = TFile::Open("pu_mc.root");
  TH1F* hmc = (TH1F*) _fmc->Get("h_pu_mc");
  hmc->Scale(1./hmc->Integral());

  for (int bin=1;bin<=50;++bin) {
    if (hmc->GetBinContent(bin)>0) hdata->SetBinContent(bin,hdata->GetBinContent(bin)/hmc->GetBinContent(bin));
    else hdata->SetBinContent(bin,0.);
  }

  //hdata->Draw();

  if (updown==1) cout << "float getTruePUwUp(float nvtx) {" << endl;
  if (updown==2) cout << "float getTruePUwDn(float nvtx) {" << endl;
  else cout << "float getTruePUw(float nvtx) {" << endl;
  for (int bin=1;bin<=hdata->GetNbinsX();++bin) {
    cout << Form("   if (nvtx>=%f && nvtx<%f) return %f;",hdata->GetBinLowEdge(bin),hdata->GetBinLowEdge(bin+1),hdata->GetBinContent(bin) ) << endl;
  }
  cout << "   return 0.;" << endl;
  cout << "}" << endl;


}
