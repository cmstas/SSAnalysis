{

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");

  bool saveNum = false;
  bool saveDen = false;

  TString postfix = "";
  // postfix+="_LooseEMVA";//FO2
  // postfix+="_PtRatioCor";//FO3
  // postfix+="_InvPtRatio";//FO4
  // postfix+="_doBonly";
  // postfix+="_doLightonly";
  // postfix+="_noPtRel";
  // postfix+="_RelIso";
  // postfix+="_onlyMuPt15";
  // postfix+="_onlyEM30to80";
  TString var = "";
  // var="_cone";
  // var="_jet";
  // var="_jet_highpt";
  // var="_jet_lowpt";

  TFile* f = TFile::Open("rate_histos_qcd"+postfix+".root");

  TH2F* muf = (TH2F*) f->Get("rate"+var+"_histo_mu");
  muf->SetTitle("muon fake rate");
  muf->GetXaxis()->SetTitle("muon p_{T} [GeV]");
  if (var=="_cone") muf->GetXaxis()->SetTitle("cone corrected muon p_{T} [GeV]");
  if (var.Contains("_jet")) muf->GetXaxis()->SetTitle("jet p_{T} [GeV]");
  muf->GetYaxis()->SetTitle("|#eta|");
  if (postfix=="_extrPtRel") muf->GetYaxis()->SetTitle("H_{T} [GeV]");
  TCanvas c1;
  muf->GetZaxis()->SetRangeUser(0,0.7);
  muf->SetMarkerSize(1.5);
  if (postfix!="_extrPtRel") muf->GetXaxis()->SetNdivisions(6,0);
  muf->Draw("texte,colz");
  c1.RedrawAxis();

  TH2F* elf = (TH2F*) f->Get("rate"+var+"_histo_e");
  elf->SetTitle("electron fake rate");
  elf->GetXaxis()->SetTitle("electron p_{T} [GeV]");
  if (var=="_cone") elf->GetXaxis()->SetTitle("cone corrected electron p_{T} [GeV]");
  if (var.Contains("_jet")) elf->GetXaxis()->SetTitle("jet p_{T} [GeV]");
  elf->GetYaxis()->SetTitle("|#eta|");
  if (postfix=="_extrPtRel") elf->GetYaxis()->SetTitle("H_{T} [GeV]");
  TCanvas c2;
  elf->GetZaxis()->SetRangeUser(0,0.7);
  elf->SetMarkerSize(1.5);
  if (postfix!="_extrPtRel") elf->GetXaxis()->SetNdivisions(6,0);
  elf->Draw("texte,colz");
  c2.RedrawAxis();

  c1.SaveAs("mu_fr"+var+postfix+".png");
  c2.SaveAs("el_fr"+var+postfix+".png");

  if (saveDen) {

    TH2F* mud = (TH2F*) f->Get("Nl"+var+"_histo_mu");
    mud->SetTitle("muon fake rate denominator");
    mud->GetXaxis()->SetTitle("muon p_{T} [GeV]");
    if (var=="_cone") mud->GetXaxis()->SetTitle("cone corrected muon p_{T} [GeV]");
    if (var.Contains("_jet")) mud->GetXaxis()->SetTitle("jet p_{T} [GeV]");
    mud->GetYaxis()->SetTitle("|#eta|");
    if (postfix=="_extrPtRel") mud->GetYaxis()->SetTitle("H_{T} [GeV]");
    TCanvas c3;
    mud->SetMarkerSize(1.5);
    if (postfix!="_extrPtRel") mud->GetXaxis()->SetNdivisions(6,0);
    mud->Draw("texte");
    c3.RedrawAxis();
    
    TH2F* eld = (TH2F*) f->Get("Nl"+var+"_histo_e");
    eld->SetTitle("electron fake rate denominator");
    eld->GetXaxis()->SetTitle("electron p_{T} [GeV]");
    if (var=="_cone") eld->GetXaxis()->SetTitle("cone corrected electron p_{T} [GeV]");
    if (var.Contains("_jet")) eld->GetXaxis()->SetTitle("jet p_{T} [GeV]");
    eld->GetYaxis()->SetTitle("|#eta|");
    if (postfix=="_extrPtRel") eld->GetYaxis()->SetTitle("H_{T} [GeV]");
    TCanvas c4;
    eld->SetMarkerSize(1.5);
    if (postfix!="_extrPtRel") eld->GetXaxis()->SetNdivisions(6,0);
    eld->Draw("texte");
    c4.RedrawAxis();
    
    c3.SaveAs("mu_den_fr"+var+postfix+".png");
    c4.SaveAs("el_den_fr"+var+postfix+".png");

  }

  if (saveNum) {

    TH2F* mun = 0;
    if (var.Contains("_jet")) mun = (TH2F*) f->Get("Nt"+var+"_histo_mu");
    else mun = (TH2F*) f->Get("Nt_histo_mu");
    mun->SetTitle("muon fake rate numerator");
    mun->GetXaxis()->SetTitle("muon p_{T} [GeV]");
    if (var=="_cone") mun->GetXaxis()->SetTitle("cone corrected muon p_{T} [GeV]");
    if (var.Contains("_jet")) mun->GetXaxis()->SetTitle("jet p_{T} [GeV]");
    mun->GetYaxis()->SetTitle("|#eta|");
    if (postfix=="_extrPtRel") mun->GetYaxis()->SetTitle("H_{T} [GeV]");
    TCanvas c5;
    mun->SetMarkerSize(1.5);
    if (postfix!="_extrPtRel") mun->GetXaxis()->SetNdivisions(6,0);
    mun->Draw("texte");
    c5.RedrawAxis();
    
    TH2F* eln = 0;
    if (var.Contains("_jet")) eln = (TH2F*) f->Get("Nt"+var+"_histo_e");
    else eln = (TH2F*) f->Get("Nt_histo_e");
    eln->SetTitle("electron fake rate numerator");
    eln->GetXaxis()->SetTitle("electron p_{T} [GeV]");
    if (var=="_cone") eln->GetXaxis()->SetTitle("cone corrected electron p_{T} [GeV]");
    if (var.Contains("_jet")) eln->GetXaxis()->SetTitle("jet p_{T} [GeV]");
    eln->GetYaxis()->SetTitle("|#eta|");
    if (postfix=="_extrPtRel") eln->GetYaxis()->SetTitle("H_{T} [GeV]");
    TCanvas c6;
    eln->SetMarkerSize(1.5);
    if (postfix!="_extrPtRel") eln->GetXaxis()->SetNdivisions(6,0);
    eln->Draw("texte");
    c6.RedrawAxis();
    
    c5.SaveAs("mu_num_fr"+var+postfix+".png");
    c6.SaveAs("el_num_fr"+var+postfix+".png");

  }

}
