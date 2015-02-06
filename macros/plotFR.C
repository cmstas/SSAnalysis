{

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");

  // TString sample = "ttbar";
  // TString postfix = "";
  // TString type = "";

  // TString sample = "qcd";
  // TString postfix = "_pt-50to170";
  // TString type = "_gen";

  // TString sample = "qcd";
  // TString postfix = "";
  // TString type = "_now";

  TString sample = "qcd_mupt5";
  TString postfix = "";
  TString type = "_gen";

  bool doEWK = false;

  TFile* f = TFile::Open(sample+"_histos"+postfix+".root");

  TFile* fout = TFile::Open("fakeRates_"+sample+postfix+".root","RECREATE");

  TH2F* mud = (TH2F*) f->Get("fr_mu_den"+type);
  TH2F* mun = (TH2F*) f->Get("fr_mu_num"+type);
  if (doEWK) {
    TFile* f_wj = TFile::Open("wj_histos_fakes.root");
    TH2F* mud_wj = (TH2F*) f_wj->Get("fr_mu_den"+type);
    TH2F* mun_wj = (TH2F*) f_wj->Get("fr_mu_num"+type);
    TFile* f_dy = TFile::Open("dy_histos_fakes.root");
    TH2F* mud_dy = (TH2F*) f_dy->Get("fr_mu_den"+type);
    TH2F* mun_dy = (TH2F*) f_dy->Get("fr_mu_num"+type);
    mud.Add(mud_wj);
    mud.Add(mud_dy);
    mun.Add(mun_wj);
    mun.Add(mun_dy);
  }
  TH2F* muf = (TH2F*) mud->Clone("fr_mu"+type);
  muf->Reset();
  if (!muf->GetSumw2N()) muf->Sumw2();
  muf->SetTitle("muon fake rate");
  muf->GetXaxis()->SetTitle("p_{T} [GeV]");
  muf->GetYaxis()->SetTitle("|#eta|");
  muf->Divide(mun,mud,1.0,1.0,"B");
  /*
  for (int i=1;i<=muf->GetXaxis()->GetNbins();++i) {
    for (int j=1;j<=muf->GetYaxis()->GetNbins();++j) {
      if (mud->GetBinContent(i,j)>0) { 
	float fr = mun->GetBinContent(i,j)/mud->GetBinContent(i,j);
	muf->SetBinContent(i,j,fr);
	muf->SetBinError(i,j,sqrt( fr*(1-fr)/mud->GetBinContent(i,j)) );
      }
    }
  }
  */
  TCanvas c1;
  muf->GetZaxis()->SetRangeUser(0,0.5);
  muf->SetMarkerSize(1.5);
  muf->Draw("texte,colz");
  c1.RedrawAxis();

  TH2F* eld = (TH2F*) f->Get("fr_el_den"+type);
  TH2F* eln = (TH2F*) f->Get("fr_el_num"+type);
  if (doEWK) {
    TFile* f_wj = TFile::Open("wj_histos_fakes.root");
    TH2F* eld_wj = (TH2F*) f_wj->Get("fr_el_den"+type);
    TH2F* eln_wj = (TH2F*) f_wj->Get("fr_el_num"+type);
    TFile* f_dy = TFile::Open("dy_histos_fakes.root");
    TH2F* eld_dy = (TH2F*) f_dy->Get("fr_el_den"+type);
    TH2F* eln_dy = (TH2F*) f_dy->Get("fr_el_num"+type);
    eld.Add(eld_wj);
    eld.Add(eld_dy);
    eln.Add(eln_wj);
    eln.Add(eln_dy);
  }
  TH2F* elf = (TH2F*) eld->Clone("fr_el"+type);
  elf->Reset();
  if (!elf->GetSumw2N()) elf->Sumw2();
  elf->SetTitle("electron fake rate");
  elf->GetXaxis()->SetTitle("p_{T} [GeV]");
  elf->GetYaxis()->SetTitle("|#eta|");
  elf->Divide(eln,eld,1.0,1.0,"B");
  /*
  for (int i=1;i<=elf->GetXaxis()->GetNbins();++i) {
    for (int j=1;j<=elf->GetYaxis()->GetNbins();++j) {
      if (eld->GetBinContent(i,j)>0) { 
	float fr = eln->GetBinContent(i,j)/eld->GetBinContent(i,j);
	elf->SetBinContent(i,j,fr);
	//elf->SetBinError(i,j,sqrt( fr*(1-fr)/eld->GetBinContent(i,j)) );
	elf->SetBinError(i,j,sqrt( fr*(1-fr)/pow(eld->GetBinContent(i,j)/eld->GetBinError(i,j),2)) );
      }
    }
  }
  */
  TCanvas c2;
  elf->GetZaxis()->SetRangeUser(0,0.5);
  elf->SetMarkerSize(1.5);
  elf->Draw("texte,colz");
  c2.RedrawAxis();

  if (doEWK) postfix="_ewk"+postfix;
  c1.SaveAs(sample+"_mu_fr"+postfix+type+".png");
  c2.SaveAs(sample+"_el_fr"+postfix+type+".png");

  muf->Write();
  elf->Write();
  fout->Close();
}
