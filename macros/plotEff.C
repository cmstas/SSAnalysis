{

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");

  TString dir = "results_PHYS14";

  TString sample = "dy";
  //sample = "T1ttttG1200";
  //sample = "T5Full1200";
  //sample = "TTWJets";

  bool doht = false;
  bool dofo = false;

  TString prefix = "";
  TString postfix = "";
  if (doht)  prefix  = "_ht";
  if (dofo)  postfix = "_vlnoiso";

  TFile* f = TFile::Open(dir+"/"+sample+"_histos_effic.root");

  TH2F* mud = (TH2F*) f->Get("ef"+prefix+"_mu_den");
  mud->GetXaxis()->SetTitle("p_{T} [GeV]");
  mud->GetYaxis()->SetTitle("|#eta|");
  if (doht) {
    mud->GetYaxis()->SetTitle("H_{T} [GeV]");
    mud->GetXaxis()->SetNdivisions(5,0);
    mud->GetYaxis()->SetNdivisions(6,0);
  }
  mud->GetXaxis()->SetTitleSize(0.05);
  mud->GetYaxis()->SetTitleSize(0.05);
  mud->GetXaxis()->SetTitleOffset(0.85);
  mud->GetYaxis()->SetTitleOffset(0.8);
  TH2F* mun = (TH2F*) f->Get("ef"+prefix+"_mu_num"+postfix);
  TH2F* muf = (TH2F*) mud->Clone("ef_mu");
  muf->Reset();
  if (!muf->GetSumw2N()) muf->Sumw2();
  muf->SetTitle("muon efficiency");
  muf->Divide(mun,mud,1.0,1.0,"B");
  /*
  for (int i=1;i<=muf->GetXaxis()->GetNbins();++i) {
    for (int j=1;j<=muf->GetYaxis()->GetNbins();++j) {
      if (mud->GetBinContent(i,j)>0) { 
	float ef = mun->GetBinContent(i,j)/mud->GetBinContent(i,j);
	muf->SetBinContent(i,j,ef);
	muf->SetBinError(i,j,sqrt( ef*(1-ef)/mud->GetBinContent(i,j)) );
      }
    }
  }
  */
  TCanvas c1;
  muf->GetZaxis()->SetRangeUser(0.,1.);
  muf->SetMarkerSize(1.5);
  muf->Draw("texte,colz");
  c1.RedrawAxis();
  TCanvas c1_1;
  mud->SetMarkerSize(1.5);
  mud->Draw("texte,colz");
  c1_1.RedrawAxis();

  TH2F* eld = (TH2F*) f->Get("ef"+prefix+"_el_den");
  eld->GetXaxis()->SetTitle("p_{T} [GeV]");
  eld->GetYaxis()->SetTitle("|#eta|");
  if (doht) {
    eld->GetYaxis()->SetTitle("H_{T} [GeV]");
    eld->GetXaxis()->SetNdivisions(5,0);
    eld->GetYaxis()->SetNdivisions(6,0);
  }
  eld->GetXaxis()->SetTitleSize(0.05);
  eld->GetYaxis()->SetTitleSize(0.05);
  eld->GetXaxis()->SetTitleOffset(0.85);
  eld->GetYaxis()->SetTitleOffset(0.8);
  TH2F* eln = (TH2F*) f->Get("ef"+prefix+"_el_num"+postfix);
  TH2F* elf = (TH2F*) eld->Clone("ef_el");
  elf->Reset();
  if (!elf->GetSumw2N()) elf->Sumw2();
  elf->SetTitle("electron efficiency");
  elf->Divide(eln,eld,1.0,1.0,"B");
  /*
  for (int i=1;i<=elf->GetXaxis()->GetNbins();++i) {
    for (int j=1;j<=elf->GetYaxis()->GetNbins();++j) {
      if (eld->GetBinContent(i,j)>0) { 
	float ef = eln->GetBinContent(i,j)/eld->GetBinContent(i,j);
	elf->SetBinContent(i,j,ef);
	elf->SetBinError(i,j,sqrt( ef*(1-ef)/eld->GetBinContent(i,j)) );
      }
    }
  }
  */
  TCanvas c2;
  elf->GetZaxis()->SetRangeUser(0.,1.);
  elf->SetMarkerSize(1.5);
  elf->Draw("texte,colz");
  c2.RedrawAxis();
  TCanvas c2_1;
  eld->SetMarkerSize(1.5);
  eld->Draw("texte,colz");
  c2_1.RedrawAxis();

  c1.SaveAs(dir+"/"+sample+"_mu_ef"+prefix+postfix+".png");
  c2.SaveAs(dir+"/"+sample+"_el_ef"+prefix+postfix+".png");
  c1_1.SaveAs(dir+"/"+sample+"_mu_ef"+prefix+postfix+"_den.png");
  c2_1.SaveAs(dir+"/"+sample+"_el_ef"+prefix+postfix+"_den.png");

}
