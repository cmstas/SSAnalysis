{

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat(".1e");

  TString dir = "results_PHYS14";

  bool doFlip = true;

  TString sample = "dy";

  TFile* f = TFile::Open(dir+"/"+sample+"_histos_effic.root");

  TH2F* mud = (TH2F*) (doFlip ? f->Get("flip_mu_den") : f->Get("ef_mu_den"));
  TH2F* mun = (TH2F*) (doFlip ? f->Get("flip_mu_num") : f->Get("ef_mu_num"));
  TH2F* muf = (TH2F*) mud->Clone("ef_mu");
  muf->Reset();
  if (doFlip) muf->SetTitle("muon flip rate");
  else muf->SetTitle("muon efficiency");
  muf->GetXaxis()->SetTitle("p_{T} [GeV]");
  muf->GetYaxis()->SetTitle("|#eta|");
  for (int i=1;i<=muf->GetXaxis()->GetNbins();++i) {
    for (int j=1;j<=muf->GetYaxis()->GetNbins();++j) {
      if (mud->GetBinContent(i,j)>0) { 
	if (i!=1) {
	  float ef = mun->GetBinContent(i,j)/mud->GetBinContent(i,j);
	  muf->SetBinContent(i,j,ef);
	  muf->SetBinError(i,j,sqrt( ef*(1-ef)/mud->GetBinContent(i,j)) );
	} else muf->SetBinContent(i,j,0);
      }
    }
  }
  TCanvas c1;
  muf->GetZaxis()->SetRangeUser(0.,1.);
  if (doFlip) {
    muf->GetZaxis()->SetRangeUser(0.00001,1.);
    c1.SetLogz();
  }
  muf->SetMarkerSize(1.8);
  muf->Draw("texte,colz");
  c1.RedrawAxis();

  TH2F* eld = (TH2F*) (doFlip ? f->Get("flip_el_den") : f->Get("ef_el_den"));
  TH2F* eln = (TH2F*) (doFlip ? f->Get("flip_el_num") : f->Get("ef_el_num"));
  TH2F* elf = (TH2F*) eld->Clone("ef_el");
  elf->Reset();
  if (doFlip) elf->SetTitle("electron flip rate 13 TeV");
  else elf->SetTitle("electron efficiency");
  elf->GetXaxis()->SetTitle("p_{T} [GeV]");
  elf->GetYaxis()->SetTitle("|#eta|");
  for (int i=1;i<=elf->GetXaxis()->GetNbins();++i) {
    for (int j=1;j<=elf->GetYaxis()->GetNbins();++j) {
      if (eld->GetBinContent(i,j)>0) { 
	if (i!=1) {
	  float ef = eln->GetBinContent(i,j)/eld->GetBinContent(i,j);
	  elf->SetBinContent(i,j,ef);
	  elf->SetBinError(i,j,sqrt( ef*(1-ef)/eld->GetBinContent(i,j)) );
	} else elf->SetBinContent(i,j,0);
      }
    }
  }
  TCanvas c2;
  elf->GetZaxis()->SetRangeUser(0.,1.);
  if (doFlip) {
    elf->GetZaxis()->SetRangeUser(0.00001,1.);
    c2.SetLogz();
  }
  elf->SetMarkerSize(1.8);
  elf->Draw("texte,colz");
  c2.RedrawAxis();

  TString label = "ef";
  if (doFlip) label = "flip"; 
  c1.SaveAs(dir+"/"+sample+"_mu_"+label+".png");
  c2.SaveAs(dir+"/"+sample+"_el_"+label+".png");

  /*
  TCanvas c3;
  c3.SetLogz();
  TH2F* elf8TeV = (TH2F*) elf->Clone("ef_el8TeV");
  elf8TeV->Reset();
  elf8TeV->SetTitle("electron flip rate 8 TeV");
  elf8TeV->SetBinContent(2,1,3.44E-05);
  elf8TeV->SetBinError  (2,1,6.18E-06);
  elf8TeV->SetBinContent(2,2,1.81E-04);
  elf8TeV->SetBinError  (2,2,1.82E-05);
  elf8TeV->SetBinContent(2,3,2.96E-04);
  elf8TeV->SetBinError  (2,3,4.52E-05);
  elf8TeV->SetBinContent(3,1,2.06E-05);
  elf8TeV->SetBinError  (3,1,4.38E-06);
  elf8TeV->SetBinContent(3,2,2.16E-04);
  elf8TeV->SetBinError  (3,2,1.94E-05);
  elf8TeV->SetBinContent(3,3,3.96E-04);
  elf8TeV->SetBinError  (3,3,5.49E-05);
  elf8TeV->SetBinContent(4,1,1.03E-04);
  elf8TeV->SetBinError  (4,1,3.27E-05);
  elf8TeV->SetBinContent(4,2,5.31E-03);
  elf8TeV->SetBinError  (4,2,1.00E-04);
  elf8TeV->SetBinContent(4,3,1.34E-03);
  elf8TeV->SetBinError  (4,3,3.34E-04);
  elf8TeV->SetBinContent(5,1,9.16E-05);
  elf8TeV->SetBinError  (5,1,4.58E-05);
  elf8TeV->SetBinContent(5,2,9.87E-04);
  elf8TeV->SetBinError  (5,2,2.06E-04);
  elf8TeV->SetBinContent(5,3,8.79E-04);
  elf8TeV->SetBinError  (5,3,4.39E-04);
  elf8TeV->Draw("texte,colz");
  c3.SaveAs(dir+"/"+sample+"_el_"+label+"_8TeV.png");
  */
}
