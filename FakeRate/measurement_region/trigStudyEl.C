{

  gROOT->Reset();
  gStyle->SetOptStat(0);

  TString dataf = "/nfs-7/userdata/leptonTree/v0.08/2015DoubleEG.root";

  TFile *_file = TFile::Open(dataf);

  TTree* t = (TTree*) _file->Get("t");

  TH1F* total_pt_Ele8 = new TH1F("total_pt_Ele8","total_pt_Ele8",20,0,100);
  total_pt_Ele8->Sumw2();  
  TH1F* total_pt_Ele12 = (TH1F*) total_pt_Ele8->Clone("total_pt_Ele12");
  TH1F* total_pt_Ele18 = (TH1F*) total_pt_Ele8->Clone("total_pt_Ele18");
  TH1F* total_pt_Ele23 = (TH1F*) total_pt_Ele8->Clone("total_pt_Ele23");
  TH1F* total_pt_Ele33 = (TH1F*) total_pt_Ele8->Clone("total_pt_Ele33");

  TH1F* fo_pt_Ele8 = new TH1F("fo_pt_Ele8","fo_pt_Ele8",20,0,100);
  fo_pt_Ele8->Sumw2();  
  TH1F* fo_pt_Ele12 = (TH1F*) fo_pt_Ele8->Clone("fo_pt_Ele12");
  TH1F* fo_pt_Ele18 = (TH1F*) fo_pt_Ele8->Clone("fo_pt_Ele18");
  TH1F* fo_pt_Ele23 = (TH1F*) fo_pt_Ele8->Clone("fo_pt_Ele23");
  TH1F* fo_pt_Ele33 = (TH1F*) fo_pt_Ele8->Clone("fo_pt_Ele33");

  TH1F* tight_pt_Ele8 = new TH1F("tight_pt_Ele8","tight_pt_Ele8",20,0,100);
  tight_pt_Ele8->Sumw2();  
  TH1F* tight_pt_Ele12 = (TH1F*) tight_pt_Ele8->Clone("tight_pt_Ele12");
  TH1F* tight_pt_Ele18 = (TH1F*) tight_pt_Ele8->Clone("tight_pt_Ele18");
  TH1F* tight_pt_Ele23 = (TH1F*) tight_pt_Ele8->Clone("tight_pt_Ele23");
  TH1F* tight_pt_Ele33 = (TH1F*) tight_pt_Ele8->Clone("tight_pt_Ele33");

  TH1F* unw_fo_pt_Ele8 = new TH1F("unw_fo_pt_Ele8","unw_fo_pt_Ele8",20,0,100);
  unw_fo_pt_Ele8->Sumw2();  
  TH1F* unw_fo_pt_Ele12 = (TH1F*) unw_fo_pt_Ele8->Clone("unw_fo_pt_Ele12");
  TH1F* unw_fo_pt_Ele18 = (TH1F*) unw_fo_pt_Ele8->Clone("unw_fo_pt_Ele18");
  TH1F* unw_fo_pt_Ele23 = (TH1F*) unw_fo_pt_Ele8->Clone("unw_fo_pt_Ele23");
  TH1F* unw_fo_pt_Ele33 = (TH1F*) unw_fo_pt_Ele8->Clone("unw_fo_pt_Ele33");

  //all fired (matched, weighted)
  t->Draw("p4.pt()>>total_pt_Ele8" ,"HLT_Ele8_CaloIdM_TrackIdM_PFJet30*(HLT_Ele8_CaloIdM_TrackIdM_PFJet30>0   && abs(id)==11)","goff");
  t->Draw("p4.pt()>>total_pt_Ele12","HLT_Ele12_CaloIdM_TrackIdM_PFJet30*(HLT_Ele12_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11)","goff");
  t->Draw("p4.pt()>>total_pt_Ele18","HLT_Ele18_CaloIdM_TrackIdM_PFJet30*(HLT_Ele18_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11)","goff");
  t->Draw("p4.pt()>>total_pt_Ele23","HLT_Ele23_CaloIdM_TrackIdM_PFJet30*(HLT_Ele23_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11)","goff");
  t->Draw("p4.pt()>>total_pt_Ele33","HLT_Ele33_CaloIdM_TrackIdM_PFJet30*(HLT_Ele33_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11)","goff");
  
  //fo fired (matched, weighted)
  t->Draw("p4.pt()>>fo_pt_Ele8" ,"HLT_Ele8_CaloIdM_TrackIdM_PFJet30 *(HLT_Ele8_CaloIdM_TrackIdM_PFJet30>0  && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  t->Draw("p4.pt()>>fo_pt_Ele12","HLT_Ele12_CaloIdM_TrackIdM_PFJet30*(HLT_Ele12_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  t->Draw("p4.pt()>>fo_pt_Ele18","HLT_Ele18_CaloIdM_TrackIdM_PFJet30*(HLT_Ele18_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  t->Draw("p4.pt()>>fo_pt_Ele23","HLT_Ele23_CaloIdM_TrackIdM_PFJet30*(HLT_Ele23_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  t->Draw("p4.pt()>>fo_pt_Ele33","HLT_Ele33_CaloIdM_TrackIdM_PFJet30*(HLT_Ele33_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  
  //tight fired (matched, weighted)
  t->Draw("p4.pt()>>tight_pt_Ele8" ,"HLT_Ele8_CaloIdM_TrackIdM_PFJet30 *(HLT_Ele8_CaloIdM_TrackIdM_PFJet30>0  && abs(id)==11 && passes_SS_tight_v3)","goff");
  t->Draw("p4.pt()>>tight_pt_Ele12","HLT_Ele12_CaloIdM_TrackIdM_PFJet30*(HLT_Ele12_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_tight_v3)","goff");
  t->Draw("p4.pt()>>tight_pt_Ele18","HLT_Ele18_CaloIdM_TrackIdM_PFJet30*(HLT_Ele18_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_tight_v3)","goff");
  t->Draw("p4.pt()>>tight_pt_Ele23","HLT_Ele23_CaloIdM_TrackIdM_PFJet30*(HLT_Ele23_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_tight_v3)","goff");
  t->Draw("p4.pt()>>tight_pt_Ele33","HLT_Ele33_CaloIdM_TrackIdM_PFJet30*(HLT_Ele33_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_tight_v3)","goff");
  
  //fo fired (matched, unweighted)
  t->Draw("p4.pt()>>unw_fo_pt_Ele8" ,"(HLT_Ele8_CaloIdM_TrackIdM_PFJet30>0  && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  t->Draw("p4.pt()>>unw_fo_pt_Ele12","(HLT_Ele12_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  t->Draw("p4.pt()>>unw_fo_pt_Ele18","(HLT_Ele18_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  t->Draw("p4.pt()>>unw_fo_pt_Ele23","(HLT_Ele23_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");
  t->Draw("p4.pt()>>unw_fo_pt_Ele33","(HLT_Ele33_CaloIdM_TrackIdM_PFJet30>0 && abs(id)==11 && passes_SS_fo_looseMVA_v3)","goff");

  TCanvas c1;
  c1.SetLogy();

  TH1F* paths_total = new TH1F("paths_total","paths_total",5,0,5);
  paths_total->Fill(0,total_pt_Ele8->GetEntries());
  paths_total->Fill(1,total_pt_Ele12->GetEntries());
  paths_total->Fill(2,total_pt_Ele18->GetEntries());
  paths_total->Fill(3,total_pt_Ele23->GetEntries());
  paths_total->Fill(4,total_pt_Ele33->GetEntries());

  TH1F* paths_fo = new TH1F("paths_fo","paths_fo",40,0,40);
  paths_fo->Fill(0,fo_pt_Ele8->GetEntries());
  paths_fo->Fill(1,fo_pt_Ele12->GetEntries());
  paths_fo->Fill(2,fo_pt_Ele18->GetEntries());
  paths_fo->Fill(3,fo_pt_Ele23->GetEntries());
  paths_fo->Fill(4,fo_pt_Ele33->GetEntries());
  paths_fo->SetLineColor(kMagenta);

  TH1F* paths_tight = new TH1F("paths_tight","paths_tight",40,0,40);
  paths_tight->Fill(0,tight_pt_Ele8->GetEntries());
  paths_tight->Fill(1,tight_pt_Ele12->GetEntries());
  paths_tight->Fill(2,tight_pt_Ele18->GetEntries());
  paths_tight->Fill(3,tight_pt_Ele23->GetEntries());
  paths_tight->Fill(4,tight_pt_Ele33->GetEntries());
  paths_tight->SetLineColor(kRed);

  paths_total->GetXaxis()->SetBinLabel(1,"Ele8");
  paths_total->GetXaxis()->SetBinLabel(2,"Ele12");
  paths_total->GetXaxis()->SetBinLabel(3,"Ele18");
  paths_total->GetXaxis()->SetBinLabel(4,"Ele23");
  paths_total->GetXaxis()->SetBinLabel(5,"Ele33");
  paths_total->GetYaxis()->SetRangeUser(10,1000000);
  paths_total->Draw("hist,text0");
  paths_fo->Draw("hist,text0,same");
  paths_tight->Draw("hist,text0,same");
  c1->SaveAs( "paths_el.png" );

  TCanvas c2;
  c2.SetLogy();
  unw_fo_pt_Ele8->SetLineColor(kBlack);
  unw_fo_pt_Ele12->SetLineColor(kGreen);
  unw_fo_pt_Ele18->SetLineColor(kBlue);
  unw_fo_pt_Ele23->SetLineColor(kRed);
  unw_fo_pt_Ele33->SetLineColor(kMagenta);
  unw_fo_pt_Ele8->GetYaxis()->SetRangeUser(1,100000);
  unw_fo_pt_Ele8->Draw("hist");
  unw_fo_pt_Ele12->Draw("hist,same");
  unw_fo_pt_Ele18->Draw("hist,same");
  unw_fo_pt_Ele23->Draw("hist,same");
  unw_fo_pt_Ele33->Draw("hist,same");
  c2.SaveAs( "unw_pt_el.png" );

  TCanvas c3;
  c3.SetLogy();
  fo_pt_Ele8->SetLineColor(kBlack);
  fo_pt_Ele12->SetLineColor(kGreen);
  fo_pt_Ele18->SetLineColor(kBlue);
  fo_pt_Ele23->SetLineColor(kRed);
  fo_pt_Ele33->SetLineColor(kMagenta);
  fo_pt_Ele8->GetYaxis()->SetRangeUser(50,5000000);
  fo_pt_Ele8->Draw("hist");
  fo_pt_Ele12->Draw("hist,same");
  fo_pt_Ele18->Draw("hist,same");
  fo_pt_Ele23->Draw("hist,same");
  fo_pt_Ele33->Draw("hist,same");
  c3.SaveAs( "pt_el.png" );

}