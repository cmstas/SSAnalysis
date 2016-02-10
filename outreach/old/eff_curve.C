#include "TTree.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TStyle.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLine.h"

#include "eff.h"

void eff_curve(const char* name){
  TFile *file = new TFile(Form("%s", name));
  TTree *tree = (TTree*)file->Get("tree");
  
  gStyle->SetOptStat(0);

  const int size = 9;
  float binx[size] = {10,15,20,25,30,40,50,80,200};

  //Electron by pT
  TH1F* elec_eff_numer = new TH1F("elec_eff_numer", "elec_eff_numer", size-1, binx);
  tree->Draw("pt1_gen>>elec_eff_numer","abs(id1_gen)==11 && ht_gen>200 && met_gen>50 && isNumeratorLepton_1 && njets_reco >= 2 && pt1_gen >= 20 && pt2_gen >= 20 && pt1_reco >= 20 && pt2_reco >= 20");
  tree->Draw("pt2_gen>>+elec_eff_numer","abs(id2_gen)==11 && ht_gen>200 && met_gen>50 && isNumeratorLepton_2 && njets_reco >= 2 && pt1_gen >= 20 && pt2_gen >= 20 && pt1_reco >= 20 && pt2_reco >= 20");

  TH1F* elec_eff_denom = new TH1F("elec_eff_denom", "elec_eff_denom", size-1, binx);
  tree->Draw("pt1_gen>>elec_eff_denom","abs(id1_gen)==11 && ht_gen>200 && met_gen>50 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");
  tree->Draw("pt2_gen>>+elec_eff_denom","abs(id2_gen)==11 && ht_gen>200 && met_gen>50 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

  TH1F* elec_eff;
  elec_eff = eff(elec_eff_denom, elec_eff_numer, "elec_pt");
  elec_eff->SetTitle("Reco Efficiency by pT for Electrons");

  //Muon by pT
  TH1F* muon_eff_numer = new TH1F("muon_eff_numer", "muon_eff_numer", size-1, binx);
  tree->Draw("pt1_gen>>muon_eff_numer","abs(id1_gen)==13 && ht_gen>200 && met_gen>50 && isNumeratorLepton_1 && njets_reco >= 2 && pt1_gen >= 20 && pt2_gen >= 20 && pt1_reco >= 20 && pt2_reco >= 20");
  tree->Draw("pt2_gen>>+muon_eff_numer","abs(id2_gen)==13 && ht_gen>200 && met_gen>50 && isNumeratorLepton_2 && njets_reco >= 2 && pt1_gen >= 20 && pt2_gen >= 20 && pt1_reco >= 20 && pt2_reco >= 20");

  TH1F* muon_eff_denom = new TH1F("muon_eff_denom", "muon_eff_denom", size-1, binx);
  tree->Draw("pt1_gen>>muon_eff_denom","abs(id1_gen)==13 && ht_gen>200 && met_gen>50 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");
  tree->Draw("pt2_gen>>+muon_eff_denom","abs(id2_gen)==13 && ht_gen>200 && met_gen>50 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

  TH1F* muon_eff;
  muon_eff = eff(muon_eff_denom, muon_eff_numer, "muon_pt");
  muon_eff->SetTitle("Reco Efficiency by pT for Muons");

  const int size2 = 18;
  float binx2[size2] = {1.99,2.01,2.99,3.01,3.99,4.01,4.99,5.01,5.99,6.01,6.99,7.01,7.99,8.01,8.99,9.01,9.99,10.01};

  //njets < 4
  TH1F* njets_eff_numer_2 = new TH1F("njets_eff_numer_2", "njets_eff_numer_2", size2-1, binx2);
  tree->Draw("njets_gen>>njets_eff_numer_2","ht_gen>200 && met_gen>50 && njets_reco<4");

  TH1F* njets_eff_denom_2 = new TH1F("njets_eff_denom_2", "njets_eff_denom_2", size2-1, binx2);
  tree->Draw("njets_gen>>njets_eff_denom_2","met_gen>50 && ht_gen>200");

  TH1F* njets_eff_2;
  njets_eff_2 = eff(njets_eff_denom_2, njets_eff_numer_2, "njets");
  njets_eff_2->SetTitle("Efficiency of njets < 4 cut");

  //njets >= 4
  TH1F* njets_eff_numer = new TH1F("njets_eff_numer", "njets_eff_numer", size2-1, binx2);
  tree->Draw("njets_gen>>njets_eff_numer","ht_gen>200 && met_gen>50 && njets_reco>=4");

  TH1F* njets_eff_denom = new TH1F("njets_eff_denom", "njets_eff_denom", size2-1, binx2);
  tree->Draw("njets_gen>>njets_eff_denom","met_gen>50 && ht_gen>200");

  TH1F* njets_eff;
  njets_eff = eff(njets_eff_denom, njets_eff_numer, "njets");
  njets_eff->SetTitle("Efficiency of njets >= 4 cut");

  //hT > 200
  TH1F* ht1_eff_numer = new TH1F("ht1_eff_numer", "ht1_eff_numer", 56, 100, 800);
  tree->Draw("ht_gen>>ht1_eff_numer","ht_reco>200 && met_gen>50 && njets_gen>=2");

  TH1F* ht1_eff_denom = new TH1F("ht1_eff_denom", "ht1_eff_denom", 56, 100, 800);
  tree->Draw("ht_gen>>ht1_eff_denom","met_gen>50 && njets_gen>=2");

  TH1F* ht1_eff;
  ht1_eff = eff(ht1_eff_denom, ht1_eff_numer, "ht1");
  ht1_eff->SetTitle("Efficiency of Reco hT > 200 cut");

  //hT > 400
  TH1F* ht2_eff_numer = new TH1F("ht2_eff_numer", "ht2_eff_numer", 56, 100, 800);
  tree->Draw("ht_gen>>ht2_eff_numer","ht_reco>400 && met_gen>50 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

  TH1F* ht2_eff_denom = new TH1F("ht2_eff_denom", "ht2_eff_denom", 56, 100, 800);
  tree->Draw("ht_gen>>ht2_eff_denom","met_gen>50 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

  TH1F* ht2_eff;
  ht2_eff = eff(ht2_eff_denom, ht2_eff_numer, "ht2");
  ht2_eff->SetTitle("Efficiency of Reco hT > 400 cut");

  //hT > 200 (direct)
  TH1F* ht3_eff_numer = new TH1F("ht3_eff_numer", "ht3_eff_numer", 56, 100, 800);
  tree->Draw("ht_jets_gen>>ht3_eff_numer","ht_direct_reco>200 && met_gen>50 && njets_jets_gen>=2");

  TH1F* ht3_eff_denom = new TH1F("ht3_eff_denom", "ht3_eff_denom", 56, 100, 800);
  tree->Draw("ht_jets_gen>>ht3_eff_denom","met_gen>50 && njets_jets_gen>=2");

  TH1F* ht3_eff;
  ht3_eff = eff(ht3_eff_denom, ht3_eff_numer, "ht3");
  ht3_eff->SetTitle("Efficiency of Reco hT > 200 cut");

  //hT > 400 (direct)
  TH1F* ht4_eff_numer = new TH1F("ht4_eff_numer", "ht4_eff_numer", 56, 100, 800);
  tree->Draw("ht_jets_gen>>ht4_eff_numer","ht_direct_reco>400 && met_gen>50 && njets_jets_gen >= 2");

  TH1F* ht4_eff_denom = new TH1F("ht4_eff_denom", "ht4_eff_denom", 56, 100, 800);
  tree->Draw("ht_jets_gen>>ht4_eff_denom","met_gen>50 && njets_jets_gen >= 2");

  TH1F* ht4_eff;
  ht4_eff = eff(ht4_eff_denom, ht4_eff_numer, "ht4");
  ht4_eff->SetTitle("Efficiency of Reco hT > 400 cut");

  //met > 30
  TH1F* met1_eff_numer = new TH1F("met1_eff_numer", "met1_eff_numer", 25, 0, 500);
  tree->Draw("met_gen>>met1_eff_numer","met_reco>30 && ht_gen>200 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

  TH1F* met1_eff_denom = new TH1F("met1_eff_denom", "met1_eff_denom", 25, 0, 500);
  tree->Draw("met_gen>>met1_eff_denom","ht_gen>200 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met1_eff;
  met1_eff = eff(met1_eff_denom, met1_eff_numer, "met1");
  met1_eff->SetTitle("Efficiency of Reco met > 30 cut");

  //met > 50
  TH1F* met2_eff_numer = new TH1F("met2_eff_numer", "met2_eff_numer", 50, 0, 500);
  tree->Draw("met_gen>>met2_eff_numer","met_reco>50 && ht_gen>200 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met2_eff_denom = new TH1F("met2_eff_denom", "met2_eff_denom", 50, 0, 500);
  tree->Draw("met_gen>>met2_eff_denom","ht_gen>200 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met2_eff;
  met2_eff = eff(met2_eff_denom, met2_eff_numer, "met2");
  met2_eff->SetTitle("Efficiency of Reco met > 30 cut");

  //met > 120
  TH1F* met3_eff_numer = new TH1F("met3_eff_numer", "met3_eff_numer", 50, 0, 500);
  tree->Draw("met_gen>>met3_eff_numer","met_reco>120 && ht_gen>200 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met3_eff_denom = new TH1F("met3_eff_denom", "met3_eff_denom", 50, 0, 500);
  tree->Draw("met_gen>>met3_eff_denom","ht_gen>200 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met3_eff;
  met3_eff = eff(met3_eff_denom, met3_eff_numer, "met3");
  met3_eff->SetTitle("Efficiency of Reco met > 30 cut");

  //pT of each btag  -- from jets
  TH1F* btag_eff_numer = new TH1F("btag_eff_numer", "btag_eff_numer", 50, 0, 400);
  tree->Draw("btags_direct_gen.pt()>>btag_eff_numer","met_gen>50 && ht_jets_gen>200 && njets_jets_gen >= 2 && gen_btag_is_matched_direct");

  TH1F* btag_eff_denom = new TH1F("btag_eff_denom", "btag_eff_denom", 50, 0, 400);
  tree->Draw("btags_direct_gen.pt()>>btag_eff_denom","met_gen>50 && ht_jets_gen>200 && njets_jets_gen >= 2");

  TH1F* btag_eff;
  btag_eff = eff(btag_eff_denom, btag_eff_numer, "btag");
  btag_eff->SetTitle("Efficiency of btag cut (jets)");

  //pT of each btag -- from partons
  TH1F* btag2_eff_numer = new TH1F("btag2_eff_numer", "btag2_eff_numer", 50, 0, 400);
  tree->Draw("btags_gen.pt()>>btag2_eff_numer","met_gen>50 && ht_gen>200 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 && gen_btag_is_matched");

  TH1F* btag2_eff_denom = new TH1F("btag2_eff_denom", "btag2_eff_denom", 50, 0, 400);
  tree->Draw("btags_gen.pt()>>btag2_eff_denom","met_gen>50 && ht_gen>200 && njets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

  TH1F* btag2_eff;
  btag2_eff = eff(btag2_eff_denom, btag2_eff_numer, "btag2");
  btag2_eff->SetTitle("Efficiency of btag cut (partons)");

  TCanvas *c0 = new TCanvas;
  THStack *s0 = new THStack("stack", "Lepton Efficiency by pT");
  elec_eff->SetMaximum(.7);
  elec_eff->SetLineColor(kBlue);
  muon_eff->SetLineColor(kRed);
  elec_eff->SetLineWidth(5);
  muon_eff->SetLineWidth(5);
  s0->Add(elec_eff);
  s0->Add(muon_eff);
  TLegend *leg0 = new TLegend(.86, .86, 1.0, .76);
  leg0->AddEntry(elec_eff, "electrons"); 
  leg0->AddEntry(muon_eff, "muons"); 
  leg0->SetBorderSize(0);//remove border and shadow box.
  leg0->SetTextSize(0.03);
  leg0->SetFillStyle(0); //make legend transparent
  c0->SetRightMargin(0.15);
  s0->SetMinimum(.3);
  s0->Draw("nostack");
  leg0->Draw();
  s0->SetTitle("ID & Iso Efficiency by pT");
  s0->GetYaxis()->SetTitle("Efficiency");
  s0->GetXaxis()->SetTitle("pT (GeV)");

  TCanvas *c1 = new TCanvas;
  THStack *s1 = new THStack("stack", "Efficiency of hT Cut");
  ht1_eff->SetMaximum(1.05);
  ht1_eff->SetLineColor(kBlue);
  ht2_eff->SetLineColor(kRed);
  ht1_eff->SetLineWidth(5);
  ht2_eff->SetLineWidth(5);
  s1->Add(ht1_eff);
  s1->Add(ht2_eff);
  TLegend *leg1 = new TLegend(.86, .86, 1.0, .76);
  leg1->AddEntry(ht1_eff, "hT > 200"); 
  leg1->AddEntry(ht2_eff, "hT > 400"); 
  leg1->SetBorderSize(0);//remove border and shadow box.
  leg1->SetTextSize(0.03);
  leg1->SetFillStyle(0); //make legend transparent
  TLine *line4 = new TLine(400,0,400,1.05);
  line4->SetLineColor(15);
  line4->SetLineStyle(3);
  TLine *line5 = new TLine(200,0,200,1.05);
  line5->SetLineColor(15);
  line5->SetLineStyle(3);
  c1->SetRightMargin(0.15);
  s1->SetMinimum(0);
  s1->Draw("nostack");
  leg1->Draw();
  line4->Draw();
  line5->Draw();
  s1->SetTitle("Efficiency of hT Cut (partons)");
  s1->GetYaxis()->SetTitle("Efficiency");
  s1->GetXaxis()->SetTitle("gen hT (GeV)");

  TCanvas *c5 = new TCanvas;
  THStack *s5 = new THStack("stack", "Efficiency of hT Cut");
  ht3_eff->SetMaximum(1.05);
  ht3_eff->SetLineColor(kBlue);
  ht4_eff->SetLineColor(kRed);
  ht3_eff->SetLineWidth(5);
  ht4_eff->SetLineWidth(5);
  s5->Add(ht3_eff);
  s5->Add(ht4_eff);
  TLegend *leg5 = new TLegend(.86, .86, 1.0, .76);
  leg5->AddEntry(ht3_eff, "hT > 200"); 
  leg5->AddEntry(ht4_eff, "hT > 400"); 
  leg5->SetBorderSize(0);//remove border and shadow box.
  leg5->SetTextSize(0.03);
  leg5->SetFillStyle(0); //make legend transparent
  TLine *line6 = new TLine(400,0,400,1.05);
  line6->SetLineColor(15);
  line6->SetLineStyle(3);
  TLine *line7 = new TLine(200,0,200,1.05);
  line7->SetLineColor(15);
  line7->SetLineStyle(3);
  c5->SetRightMargin(0.15);
  s5->SetMinimum(0);
  s5->Draw("nostack");
  leg5->Draw();
  line6->Draw();
  line7->Draw();
  s5->SetTitle("Efficiency of hT Cut (jets)");
  s5->GetYaxis()->SetTitle("Efficiency");
  s5->GetXaxis()->SetTitle("gen hT (GeV)");

  TCanvas *c2 = new TCanvas;
  THStack *s2 = new THStack("stack", "Efficiency of met Cut");
  met1_eff->SetMaximum(1);
  met1_eff->SetLineColor(kBlue);
  met2_eff->SetLineColor(kRed);
  met3_eff->SetLineColor(kGreen+2);
  met1_eff->SetLineWidth(5);
  met2_eff->SetLineWidth(5);
  met3_eff->SetLineWidth(5);
  s2->Add(met1_eff);
  s2->Add(met2_eff);
  s2->Add(met3_eff);
  TLegend *leg2 = new TLegend(.86, .86, 1.0, .76);
  leg2->AddEntry(met1_eff, "met > 30"); 
  leg2->AddEntry(met2_eff, "met > 50"); 
  leg2->AddEntry(met3_eff, "met > 120"); 
  leg2->SetBorderSize(0);//remove border and shadow box.
  leg2->SetTextSize(0.03);
  leg2->SetFillStyle(0); //make legend transparent
  TLine *line1 = new TLine(120,0,120,1.05);
  line1->SetLineColor(15);
  line1->SetLineStyle(3);
  TLine *line2 = new TLine(50,0,50,1.05);
  line2->SetLineColor(15);
  line2->SetLineStyle(3);
  TLine *line3 = new TLine(30,0,30,1.05);
  line3->SetLineColor(15);
  line3->SetLineStyle(3);
  c2->SetRightMargin(0.15);
  s2->SetMinimum(0);
  s2->Draw("nostack");
  leg2->Draw();
  line1->Draw();
  line2->Draw();
  line3->Draw();
  s2->SetTitle("Efficiency of MET Cut");
  s2->GetYaxis()->SetTitle("Efficiency");
  s2->GetXaxis()->SetTitle("gen met (GeV)");

  TCanvas *c3 = new TCanvas;
  THStack *s3 = new THStack("stack", "Efficiency of nbtags Cut");
  btag_eff->SetMaximum(1);
  btag_eff->SetLineColor(kBlue);
  btag_eff->SetLineWidth(5);
  s3->Add(btag_eff);
  s3->SetMinimum(0);
  s3->Draw("nostack");
  s3->SetTitle("Efficiency of nBtags Cut (jets)");
  s3->GetYaxis()->SetTitle("Efficiency");
  s3->GetXaxis()->SetTitle("gen btag pT (GeV)");

  TCanvas *c4 = new TCanvas;
  THStack *s4 = new THStack("stack", "Efficiency of nbtags Cut");
  btag2_eff->SetMaximum(1);
  btag2_eff->SetLineColor(kBlue);
  btag2_eff->SetLineWidth(5);
  s4->Add(btag2_eff);
  s4->SetMinimum(0);
  s4->Draw("nostack");
  s4->SetTitle("Efficiency of nBtags Cut (partons)");
  s4->GetYaxis()->SetTitle("Efficiency");
  s4->GetXaxis()->SetTitle("gen btag pT (GeV)");

  TCanvas *c6 = new TCanvas;
  THStack *s6 = new THStack("stack", "Efficiency of njets Cut");
  njets_eff->SetMaximum(1);
  njets_eff->SetLineColor(kBlue);
  njets_eff->SetLineWidth(5);
  s6->Add(njets_eff);
  njets_eff_2->SetMaximum(1);
  njets_eff_2->SetLineColor(kRed);
  njets_eff_2->SetLineWidth(5);
  s6->Add(njets_eff_2);
  s6->SetMinimum(0);
  s6->Draw("nostack");
  s6->SetTitle("Efficiency of nJets Cut (partons)");
  s6->GetYaxis()->SetTitle("Efficiency");
  s6->GetXaxis()->SetTitle("num gen jets");

  TFile *out_file = new TFile("eff_curves.root", "RECREATE");
  out_file->cd();
  gStyle->SetOptStat(0);
  elec_eff->Write();
  muon_eff->Write();
  c0->Write();
  c0->Print("id_iso_eff.pdf");
  c1->Write();
  c1->Print("ht_eff_part.pdf");
  c2->Write();
  c2->Print("met_eff.pdf");
  c3->Write();
  c3->Print("nbtags_eff_jets.pdf");
  c4->Write();
  c4->Print("nbtags_eff_part.pdf");
  c5->Write();
  c5->Print("ht_eff_jets.pdf");
  c6->Write();
  c6->Print("njets_eff.pdf");
  out_file->Close();
}
