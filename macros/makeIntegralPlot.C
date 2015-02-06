{

  gROOT.Reset();
  gStyle.SetOptStat(0);

  TString dir = "results_PHYS14";

  TString file1 = "T1ttttG1200_histos.root";
  TString file2 = "T1ttttG1200_histos.root";
  TString file3 = "T1ttttG1200_histos.root";
  file1 = "T1ttttG1500_histos.root";
  file2 = "T1ttttG1200_histos.root";
  file3 = "ttbar_histos.root";

  bool greater = true;
  bool doROC = false;

  TString plot1 = "hyp_hihi_gennobjets_csv";
  TString plot2 = "hyp_hihi_gennobjets_csv";
  TString plot3 = "hyp_hihi_genbjets_csv";
  plot1 = "hyp_ss_foFromWlead_mu_ptrel";
  plot3 = "hyp_ss_foFakelead_mu_ptrel";
  // plot1 = "hyp_ss_foFromWlead_mu_ptrelsub";
  // plot3 = "hyp_ss_foFakelead_mu_ptrelsub";
  // plot1 = "hyp_ss_foFromWlead_fail_mu_miniRelIso";greater = false;
  // plot3 = "hyp_ss_foFakelead_fail_mu_miniRelIso";
  // plot1 = "hyp_ss_foFromWlead_fail_mu_relIso02";
  // plot3 = "hyp_ss_foFakelead_fail_mu_relIso02";
  // plot1 = "hyp_ss_foFromWlead_fail_mu_relIso02DB";
  // plot3 = "hyp_ss_foFakelead_fail_mu_relIso02DB";
  plot2 = plot1;

  bool norm = true;
  bool save = true;
  bool logy = false;

  TCanvas c1;c1.SetGridy();c1.SetGridx();

  TFile *_file1 = TFile::Open(dir+"/"+file1);
  TH1F* h1 = (TH1F*) _file1->Get(plot1);
  h1->SetLineColor(kRed);
  h1->SetLineStyle(1);
  h1->SetLineWidth(2);
  if (norm) h1->Scale(1./h1->Integral());
  TH1F* h1_integral = h1->Clone("integral");
  if (greater) {
    for (unsigned int bin=1;bin<=h1->GetNbinsX();++bin) {
      h1_integral->SetBinContent(bin,h1->Integral(bin,h1->GetNbinsX()+1));
    }
  } else {
    for (unsigned int bin=h1->GetNbinsX();bin>=1;--bin) {
      h1_integral->SetBinContent(bin,h1->Integral(1,bin));
    }
  }
  h1_integral->GetYaxis()->SetRangeUser(0.,1.1);
  h1_integral->GetYaxis()->SetTitle("efficiency");
  h1_integral->GetXaxis()->SetTitle("cut value");
  h1_integral->Draw("HIST");

  TFile *_file2 = TFile::Open(dir+"/"+file2);
  TH1F* h2 = (TH1F*) _file2->Get(plot2);
  h2->SetLineColor(kRed);
  h2->SetLineStyle(2);
  h2->SetLineWidth(2);
  if (norm) h2->Scale(1./h2->Integral());
  TH1F* h2_integral = h2->Clone("integral");
  if (greater) {
    for (unsigned int bin=1;bin<=h2->GetNbinsX();++bin) {
      h2_integral->SetBinContent(bin,h2->Integral(bin,h2->GetNbinsX()+1));
    }
  } else {
    for (unsigned int bin=h2->GetNbinsX();bin>=1;--bin) {
      h2_integral->SetBinContent(bin,h2->Integral(1,bin));
    }
  }
  h2_integral->Draw("HIST,SAME");

  TFile *_file3 = TFile::Open(dir+"/"+file3);
  TH1F* h3 = (TH1F*) _file3->Get(plot3);
  h3->SetLineColor(kBlue);
  h3->SetLineStyle(1);
  h3->SetLineWidth(3);
  if (norm) h3->Scale(1./h3->Integral());
  TH1F* h3_integral = h3->Clone("integral");
  if (greater) {
    for (unsigned int bin=1;bin<=h3->GetNbinsX();++bin) {
      h3_integral->SetBinContent(bin,h3->Integral(bin,h3->GetNbinsX()+1));
    }
  } else {
    for (unsigned int bin=h3->GetNbinsX();bin>=1;--bin) {
      h3_integral->SetBinContent(bin,h3->Integral(1,bin));
    }
  }
  h3_integral->Draw("HIST,SAME");

  if (save) c1.SaveAs(dir+"/"+plot1+"_integral.png");

  if (doROC) {
    Double_t x1[100], x2[100], y[100];
    Int_t n = h1->GetNbinsX()+1;
    if (greater) {
      for (Int_t i=n;i>0;i--) {
	x1[i-1] = h1_integral->GetBinContent(i);
	x2[i-1] = h2_integral->GetBinContent(i);
	y[i-1] = 1.-h3_integral->GetBinContent(i);
	cout << "g->SetPoint("<<i-1<<","<<h2_integral->GetBinContent(i)<<","<<1.-h3_integral->GetBinContent(i)<<");"<< endl;
      }
    } else {
      for (Int_t i=0;i<n;i++) {
	x1[i] = h1_integral->GetBinContent(i);
	x2[i] = h2_integral->GetBinContent(i);
	y[i] = 1.-h3_integral->GetBinContent(i);
	cout << "g->SetPoint("<<i<<","<<h2_integral->GetBinContent(i)<<","<<1.-h3_integral->GetBinContent(i)<<");"<< endl;
      }
    }
    TGraph* gr1 = new TGraph(n,x1,y);
    TGraph* gr2 = new TGraph(n,x2,y);
    TCanvas c2;
    gr1->Draw("AL");
    gr2->Draw("L");
    c2.SaveAs(dir+"/"+plot1+"_roc.C");
  }

}
