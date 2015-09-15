{

  gROOT->Reset();
  gStyle->SetOptStat(0);

  TString dataf = "/nfs-7/userdata/leptonTree/v0.14-16p1pb/2015CDoubleMuon.root";

  TFile *_file = TFile::Open(dataf);

  TTree* t = (TTree*) _file->Get("t");

  TH1F* total_pt_Mu8 = new TH1F("total_pt_Mu8","total_pt_Mu8",20,0,100);
  total_pt_Mu8->Sumw2();  
  TH1F* total_pt_Mu17 = (TH1F*) total_pt_Mu8->Clone("total_pt_Mu17");
  TH1F* total_pt_Mu24 = (TH1F*) total_pt_Mu8->Clone("total_pt_Mu24");
  TH1F* total_pt_Mu34 = (TH1F*) total_pt_Mu8->Clone("total_pt_Mu34");

  TH1F* fo_pt_Mu8 = new TH1F("fo_pt_Mu8","fo_pt_Mu8",20,0,100);
  fo_pt_Mu8->Sumw2();  
  TH1F* fo_pt_Mu17 = (TH1F*) fo_pt_Mu8->Clone("fo_pt_Mu17");
  TH1F* fo_pt_Mu24 = (TH1F*) fo_pt_Mu8->Clone("fo_pt_Mu24");
  TH1F* fo_pt_Mu34 = (TH1F*) fo_pt_Mu8->Clone("fo_pt_Mu34");

  TH1F* tight_pt_Mu8 = new TH1F("tight_pt_Mu8","tight_pt_Mu8",20,0,100);
  tight_pt_Mu8->Sumw2();  
  TH1F* tight_pt_Mu17 = (TH1F*) tight_pt_Mu8->Clone("tight_pt_Mu17");
  TH1F* tight_pt_Mu24 = (TH1F*) tight_pt_Mu8->Clone("tight_pt_Mu24");
  TH1F* tight_pt_Mu34 = (TH1F*) tight_pt_Mu8->Clone("tight_pt_Mu34");

  TH1F* unw_fo_pt_Mu8 = new TH1F("unw_fo_pt_Mu8","unw_fo_pt_Mu8",20,0,100);
  unw_fo_pt_Mu8->Sumw2();  
  TH1F* unw_fo_pt_Mu17 = (TH1F*) unw_fo_pt_Mu8->Clone("unw_fo_pt_Mu17");
  TH1F* unw_fo_pt_Mu24 = (TH1F*) unw_fo_pt_Mu8->Clone("unw_fo_pt_Mu24");
  TH1F* unw_fo_pt_Mu34 = (TH1F*) unw_fo_pt_Mu8->Clone("unw_fo_pt_Mu34");

  t->Draw("p4.pt()>>total_pt_Mu8" ,"HLT_Mu8*(HLT_Mu8>0  && abs(id)==13)","goff");// && passes_SS_tight_v3 && HLT_Mu24>0 && tag_p4.pt()>30. && p4.pt()>25.
  t->Draw("p4.pt()>>total_pt_Mu17","HLT_Mu17*(HLT_Mu17>0 && abs(id)==13)","goff");
  t->Draw("p4.pt()>>total_pt_Mu24","HLT_Mu24*(HLT_Mu24>0 && abs(id)==13)","goff");
  t->Draw("p4.pt()>>total_pt_Mu34","HLT_Mu34*(HLT_Mu34>0 && abs(id)==13)","goff");
  
  t->Draw("p4.pt()>>fo_pt_Mu8" ,"HLT_Mu8 *(HLT_Mu8>0  && abs(id)==13 && passes_SS_fo_v3)","goff");
  t->Draw("p4.pt()>>fo_pt_Mu17","HLT_Mu17*(HLT_Mu17>0 && abs(id)==13 && passes_SS_fo_v3)","goff");
  t->Draw("p4.pt()>>fo_pt_Mu24","HLT_Mu24*(HLT_Mu24>0 && abs(id)==13 && passes_SS_fo_v3)","goff");
  t->Draw("p4.pt()>>fo_pt_Mu34","HLT_Mu34*(HLT_Mu34>0 && abs(id)==13 && passes_SS_fo_v3)","goff");
  
  t->Draw("p4.pt()>>tight_pt_Mu8" ,"HLT_Mu8 *(HLT_Mu8>0  && abs(id)==13 && passes_SS_tight_v3)","goff");
  t->Draw("p4.pt()>>tight_pt_Mu17","HLT_Mu17*(HLT_Mu17>0 && abs(id)==13 && passes_SS_tight_v3)","goff");
  t->Draw("p4.pt()>>tight_pt_Mu24","HLT_Mu24*(HLT_Mu24>0 && abs(id)==13 && passes_SS_tight_v3)","goff");
  t->Draw("p4.pt()>>tight_pt_Mu34","HLT_Mu34*(HLT_Mu34>0 && abs(id)==13 && passes_SS_tight_v3)","goff");
  
  t->Draw("p4.pt()>>unw_fo_pt_Mu8" ,"(HLT_Mu8>0  && abs(id)==13 && passes_SS_fo_v3)","goff");
  t->Draw("p4.pt()>>unw_fo_pt_Mu17","(HLT_Mu17>0 && abs(id)==13 && passes_SS_fo_v3)","goff");
  t->Draw("p4.pt()>>unw_fo_pt_Mu24","(HLT_Mu24>0 && abs(id)==13 && passes_SS_fo_v3)","goff");
  t->Draw("p4.pt()>>unw_fo_pt_Mu34","(HLT_Mu34>0 && abs(id)==13 && passes_SS_fo_v3)","goff");

  TCanvas c1;
  c1.SetLogy();

  TH1F* paths_total = new TH1F("paths_total","paths_total",4,0,4);
  paths_total->Fill(0.0,total_pt_Mu8->GetEntries());
  paths_total->Fill(1.0,total_pt_Mu17->GetEntries());
  paths_total->Fill(2.0,total_pt_Mu24->GetEntries());
  paths_total->Fill(3.0,total_pt_Mu34->GetEntries());

  TH1F* paths_fo = new TH1F("paths_fo","paths_fo",40,0,40);
  paths_fo->Fill(0.0,fo_pt_Mu8->GetEntries());
  paths_fo->Fill(1.0,fo_pt_Mu17->GetEntries());
  paths_fo->Fill(2.0,fo_pt_Mu24->GetEntries());
  paths_fo->Fill(3.0,fo_pt_Mu34->GetEntries());
  paths_fo->SetLineColor(kMagenta);

  TH1F* paths_tight = new TH1F("paths_tight","paths_tight",40,0,40);
  paths_tight->Fill(0.0,tight_pt_Mu8->GetEntries());
  paths_tight->Fill(1.0,tight_pt_Mu17->GetEntries());
  paths_tight->Fill(2.0,tight_pt_Mu24->GetEntries());
  paths_tight->Fill(3.0,tight_pt_Mu34->GetEntries());
  paths_tight->SetLineColor(kRed);

  TLatex *label = new TLatex();

  paths_total->GetXaxis()->SetBinLabel(1,"Mu8");
  paths_total->GetXaxis()->SetBinLabel(2,"Mu17");
  paths_total->GetXaxis()->SetBinLabel(3,"Mu24");
  paths_total->GetXaxis()->SetBinLabel(4,"Mu34");
  paths_total->GetYaxis()->SetRangeUser(1000,1000000);
  paths_total->Draw("hist,text0");
  paths_fo->Draw("hist,text0,same");
  paths_tight->Draw("hist,text0,same");

  label->SetTextColor(kMagenta); label->DrawLatexNDC(0.7,0.85, "pass FO");
  label->SetTextColor(kRed); label->DrawLatexNDC(0.7,0.80, "pass Tight");
  label->SetTextColor(kBlue); label->DrawLatexNDC(0.7,0.75, "all");

  c1.SaveAs( "pdfs/paths_mu.pdf" );

  TCanvas c2;
  c2.SetLogy();
  unw_fo_pt_Mu8->SetLineColor(kBlack);
  unw_fo_pt_Mu17->SetLineColor(kBlue);
  unw_fo_pt_Mu24->SetLineColor(kRed);
  unw_fo_pt_Mu34->SetLineColor(kMagenta);
  unw_fo_pt_Mu8->GetYaxis()->SetRangeUser(1,100000);
  unw_fo_pt_Mu8->Draw("hist");
  unw_fo_pt_Mu17->Draw("hist,same");
  unw_fo_pt_Mu24->Draw("hist,same");
  unw_fo_pt_Mu34->Draw("hist,same");

  label->SetTextColor(kMagenta); label->DrawLatexNDC(0.8,0.85, "Mu34");
  label->SetTextColor(kRed); label->DrawLatexNDC(0.8,0.80, "Mu24");
  label->SetTextColor(kBlue); label->DrawLatexNDC(0.8,0.75, "Mu17");
  label->SetTextColor(kBlack); label->DrawLatexNDC(0.8,0.70, "Mu8");

  c2.SaveAs( "pdfs/unw_pt_mu.pdf" );

}
