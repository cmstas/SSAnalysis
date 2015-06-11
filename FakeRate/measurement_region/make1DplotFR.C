{

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");

  bool saveDen = true;

  TString postfix = "";
  // postfix+="_LooseEMVA";
  // postfix+="_PtRatioCor";
  postfix+="_InvPtRatio";
  // postfix+="_doBonly";
  // postfix+="_doLightonly";
  TString var = "";
  var="_cone";
  // var="_jet";
  // var="_jet_highpt";
  // var="_jet_lowpt";

  TFile* f = TFile::Open("rate_histos_qcd"+postfix+".root");

  TH2F* mud = (TH2F*) f->Get("Nl"+var+"_histo_mu");  
  TH2F* eld = (TH2F*) f->Get("Nl"+var+"_histo_e");

  TH1D* mud1d = mud->ProjectionX();
  TH1D* eld1d = eld->ProjectionX();

  TH2F* mun = 0;
  TH2F* eln = 0;
  if (var.Contains("_jet")) {
    mun = (TH2F*) f->Get("Nt"+var+"_histo_mu");  
    eln = (TH2F*) f->Get("Nt"+var+"_histo_e");
  } else {
    mun = (TH2F*) f->Get("Nt_histo_mu");  
    eln = (TH2F*) f->Get("Nt_histo_e");
  }

  TH1D* mun1d = mun->ProjectionX();
  TH1D* eln1d = eln->ProjectionX();

  TH1D *muf = (TH1D*) mun1d->Clone("rate_histo_mu");
  muf->Divide(mun1d,mud1d,1,1,"B");
  TH1D *elf = (TH1D*) eln1d->Clone("rate_histo_el");
  elf->Divide(eln1d,eld1d,1,1,"B");

  muf->SetTitle("muon fake rate");
  muf->GetXaxis()->SetTitle("muon p_{T} [GeV]");
  if (var=="_cone") muf->GetXaxis()->SetTitle("cone corrected muon p_{T} [GeV]");
  if (var=="_jet") muf->GetXaxis()->SetTitle("jet p_{T} [GeV]");
  TCanvas c1("c1","c1",500,500);
  muf->GetYaxis()->SetRangeUser(0,0.7);
  muf->SetMarkerSize(1.5);
  muf->Draw("");
  c1.RedrawAxis();

  elf->SetTitle("electron fake rate");
  elf->GetXaxis()->SetTitle("electron p_{T} [GeV]");
  if (var=="_cone") elf->GetXaxis()->SetTitle("cone corrected electron p_{T} [GeV]");
  if (var=="_jet") elf->GetXaxis()->SetTitle("jet p_{T} [GeV]");
  TCanvas c2("c2","c2",500,500);
  elf->GetYaxis()->SetRangeUser(0,0.7);
  elf->SetMarkerSize(1.5);
  elf->Draw("");
  c2.RedrawAxis();

  c1.SaveAs("mu_1dfr"+var+postfix+".png");
  c2.SaveAs("el_1dfr"+var+postfix+".png");

}
