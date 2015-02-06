void setLabels(TH1F* h) {
  /*
    enum LeptonCategories { Prompt = 0, PromptWS = 1, PromptWF = 2, PromptFSR = 2, 
    FakeLightTrue = 3, FakeC = 4, FakeB = 5, FakeUnknown = 6, 
    FakeMatchLowPtGamma = 7, FakeMatchHiPtGamma = 8, All9999 = 9, FakeLightFake = 10,
    Other = 11, End = 12};
  */
  h->GetXaxis()->SetBinLabel(1,"Prompt");
  h->GetXaxis()->SetBinLabel(2,"PromptWS");
  h->GetXaxis()->SetBinLabel(3,"PromptFSR");
  h->GetXaxis()->SetBinLabel(4,"FakeLightTrue");
  h->GetXaxis()->SetBinLabel(5,"FakeC");
  h->GetXaxis()->SetBinLabel(6,"FakeB");
  h->GetXaxis()->SetBinLabel(7,"FakeLightFake");
  h->GetXaxis()->SetBinLabel(8,"FakeHiPtGamma");
  h->GetXaxis()->SetBinLabel(9,"FakeUnknown");
  h->GetXaxis()->SetBinLabel(10,"FakeLowPtGamma");
  h->GetXaxis()->SetBinLabel(11,"All9999");
  h->GetXaxis()->SetBinLabel(12,"Other");  
  h->GetXaxis()->LabelsOption("v");
  h->GetXaxis()->SetLabelOffset(-0.33);
  h->GetXaxis()->SetLabelSize(0.05);
  h->Scale(1./h->Integral(1,9));
  h->GetYaxis()->SetRangeUser(0.0001,8.);
}

void plotMCMatch() {

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  TFile* f = TFile::Open("test_histos.root");

  TCanvas c1("c1","c1",1600,1000);
  TVirtualPad* pad;
  c1.Divide(2,2);
  pad = c1.cd(1);
  TH1F* hyp_ss_lead_el_category = (TH1F*) f->Get("hyp_ss_lead_el_category");
  setLabels(hyp_ss_lead_el_category);
  hyp_ss_lead_el_category->Draw();
  pad->SetLogy();
  pad = c1.cd(2);
  TH1F* hyp_ss_trail_el_category = (TH1F*) f->Get("hyp_ss_trail_el_category");
  setLabels(hyp_ss_trail_el_category);
  pad->SetLogy();
  hyp_ss_trail_el_category->Draw();
  pad = c1.cd(3);
  TH1F* hyp_ss_lead_mu_category = (TH1F*) f->Get("hyp_ss_lead_mu_category");
  setLabels(hyp_ss_lead_mu_category);
  pad->SetLogy();
  hyp_ss_lead_mu_category->Draw();
  pad = c1.cd(4);
  TH1F* hyp_ss_trail_mu_category = (TH1F*) f->Get("hyp_ss_trail_mu_category");
  setLabels(hyp_ss_trail_mu_category);
  hyp_ss_trail_mu_category->Draw();
  pad->SetLogy();
  c1.SaveAs("lep_mc_cetagory.png");

  TCanvas ca;ca.SetLogy();
  hyp_ss_lead_el_category->Draw();
  ca.SaveAs("hyp_ss_lead_el_category.png");
  hyp_ss_lead_mu_category->Draw();
  ca.SaveAs("hyp_ss_lead_mu_category.png");
  hyp_ss_trail_el_category->Draw();
  ca.SaveAs("hyp_ss_trail_el_category.png");
  hyp_ss_trail_mu_category->Draw();
  ca.SaveAs("hyp_ss_trail_mu_category.png");
  TH1F* hyp_ss_trail_mu_ptGt10_category = (TH1F*) f->Get("hyp_ss_trail_mu_ptGt10_category");
  setLabels(hyp_ss_trail_mu_ptGt10_category);
  hyp_ss_trail_mu_ptGt10_category->Draw();
  ca.SaveAs("hyp_ss_trail_mu_ptGt10_category.png");
  TH1F* hyp_ss_trail_mu_ptLt10_category = (TH1F*) f->Get("hyp_ss_trail_mu_ptLt10_category");
  setLabels(hyp_ss_trail_mu_ptLt10_category);
  hyp_ss_trail_mu_ptLt10_category->Draw();
  ca.SaveAs("hyp_ss_trail_mu_ptLt10_category.png");

  TCanvas c2;
  TH1F* hyp_ss_trail_el_hiptgamma_mc_mother = (TH1F*) f->Get("hyp_ss_trail_el_hiptgamma_mc_mother");
  hyp_ss_trail_el_hiptgamma_mc_mother->SetLineWidth(2);
  hyp_ss_trail_el_hiptgamma_mc_mother->SetLineColor(kRed);
  hyp_ss_trail_el_hiptgamma_mc_mother->Draw();
  c2.SaveAs("hyp_ss_trail_el_hiptgamma_mc_mother.png");
  hyp_ss_trail_el_hiptgamma_mc_mother->GetXaxis()->SetRangeUser(-25,25);
  hyp_ss_trail_el_hiptgamma_mc_mother->Draw();
  c2.SaveAs("hyp_ss_trail_el_hiptgamma_mc_mother_zoom.png");

  TH1F* hyp_ss_trail_mu_hilowgamma_mc_mother = (TH1F*) f->Get("hyp_ss_trail_mu_hilowgamma_mc_mother");
  hyp_ss_trail_mu_hilowgamma_mc_mother->SetLineWidth(2);
  hyp_ss_trail_mu_hilowgamma_mc_mother->SetLineColor(kRed);
  hyp_ss_trail_mu_hilowgamma_mc_mother->Draw();
  c2.SaveAs("hyp_ss_trail_mu_hilowgamma_mc_mother.png");
  hyp_ss_trail_mu_hilowgamma_mc_mother->GetXaxis()->SetRangeUser(-25,25);
  hyp_ss_trail_mu_hilowgamma_mc_mother->Draw();
  c2.SaveAs("hyp_ss_trail_mu_hilowgamma_mc_mother_zoom.png");

  TCanvas c3;
  TH1F* hyp_ss_trail_mu_category_all9999_pt = (TH1F*) f->Get("hyp_ss_trail_mu_category_all9999_pt");
  hyp_ss_trail_mu_category_all9999_pt->Draw();
  c3.SaveAs("hyp_ss_trail_mu_category_all9999_pt.png");
  TH1F* hyp_ss_trail_mu_category_hilowgamma_pt = (TH1F*) f->Get("hyp_ss_trail_mu_category_hilowgamma_pt");
  hyp_ss_trail_mu_category_hilowgamma_pt->Draw();
  c3.SaveAs("hyp_ss_trail_mu_category_hilowgamma_pt.png");

  TCanvas cc;cc.SetLogy();
  TH1F* hyp_prompttype = (TH1F*) f->Get("hyp_prompttype");
  hyp_prompttype->GetXaxis()->SetBinLabel(1,"LP-TP");
  hyp_prompttype->GetXaxis()->SetBinLabel(2,"LP-TF");
  hyp_prompttype->GetXaxis()->SetBinLabel(3,"LF-TP");
  hyp_prompttype->GetXaxis()->SetBinLabel(4,"LF-TF");
  hyp_prompttype->GetXaxis()->SetRangeUser(0,3);
  hyp_prompttype->GetXaxis()->SetLabelSize(0.05);
  hyp_prompttype->Scale(1./hyp_prompttype->Integral());
  hyp_prompttype->Draw("hist");
  cc.SaveAs("hyp_prompttype.png");
  cc.SetLogy(0);  
  TH1F* hyp_leadprompttype = (TH1F*) f->Get("hyp_leadprompttype");
  hyp_leadprompttype->Scale(1./hyp_leadprompttype->Integral());
  hyp_leadprompttype->GetXaxis()->SetBinLabel(1,"PP");
  hyp_leadprompttype->GetXaxis()->SetBinLabel(2,"WS");
  hyp_leadprompttype->GetXaxis()->SetBinLabel(3,"FSR");
  hyp_leadprompttype->GetXaxis()->SetBinLabel(4,"Other");
  hyp_leadprompttype->GetXaxis()->SetRangeUser(0,3);
  hyp_leadprompttype->GetYaxis()->SetRangeUser(0,1);
  hyp_leadprompttype->GetXaxis()->SetLabelSize(0.05);
  hyp_leadprompttype->Draw("hist");
  cc.SaveAs("hyp_leadprompttype.png");

}
