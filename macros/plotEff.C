{

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");

  TString dir = "results_PHYS14";

  TString sample = "dy";
  //sample = "T1ttttG1500";
  //sample = "T5Full1200";
  //sample = "TTWJets";

  bool dofo = false;

  TFile* f = TFile::Open(dir+"/"+sample+"_histos_effic.root");

  TH2F* mud = (TH2F*) f->Get("ef_mu_den");
  TH2F* mun = (TH2F*) f->Get(dofo ? "ef_mu_num_fo" :  "ef_mu_num");
  TH2F* muf = (TH2F*) mud->Clone("ef_mu");
  muf->Reset();
  if (!muf->GetSumw2N()) muf->Sumw2();
  muf->SetTitle("muon efficiency");
  muf->GetXaxis()->SetTitle("p_{T} [GeV]");
  muf->GetYaxis()->SetTitle("|#eta|");
  muf->GetXaxis()->SetTitleSize(0.05);
  muf->GetYaxis()->SetTitleSize(0.05);
  muf->GetXaxis()->SetTitleOffset(0.85);
  muf->GetYaxis()->SetTitleOffset(0.8);
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

  TH2F* eld = (TH2F*) f->Get("ef_el_den");
  TH2F* eln = (TH2F*) f->Get(dofo ? "ef_el_num_fo" : "ef_el_num");
  TH2F* elf = (TH2F*) eld->Clone("ef_el");
  elf->Reset();
  if (!elf->GetSumw2N()) elf->Sumw2();
  elf->SetTitle("electron efficiency");
  elf->GetXaxis()->SetTitle("p_{T} [GeV]");
  elf->GetYaxis()->SetTitle("|#eta|");
  elf->GetXaxis()->SetTitleSize(0.05);
  elf->GetYaxis()->SetTitleSize(0.05);
  elf->GetXaxis()->SetTitleOffset(0.85);
  elf->GetYaxis()->SetTitleOffset(0.8);
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

  if (dofo) {
    c1.SaveAs(dir+"/"+sample+"_mu_ef_fo.png");
    c2.SaveAs(dir+"/"+sample+"_el_ef_fo.png");
  } else {
    c1.SaveAs(dir+"/"+sample+"_mu_ef.png");
    c2.SaveAs(dir+"/"+sample+"_el_ef.png");
  }

}
