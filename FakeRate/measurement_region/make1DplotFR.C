#include "../../software/dataMCplotMaker/dataMCplotMaker.cc"
#include "TFile.h"
#include "TH2.h"
#include "TPaveText.h"

void make1DplotFR(){

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");

  TString postfix = "";
  postfix+="_LooseEMVA";
  // postfix+="_PtRatioCor";
  // postfix+="_InvPtRatio";
  // postfix+="_doBonly";
  // postfix+="_doLightonly";
  TString var = "";
  var="_cone";
  // var="_jet";
  // var="_jet_highpt";
  // var="_jet_lowpt";

  TFile* f_qcd_mu = TFile::Open("rate_histos_qcd_mu"+postfix+".root");
  TFile* f_qcd_el = TFile::Open("rate_histos_qcd_el"+postfix+".root");

  TH2F* mud_qcd = (TH2F*) f_qcd_mu->Get("Nl"+var+"_histo_mu");  
  TH2F* eld_qcd = (TH2F*) f_qcd_el->Get("Nl"+var+"_histo_e");

  TH1D* mud1d_qcd = mud_qcd->ProjectionX();
  TH1D* eld1d_qcd = eld_qcd->ProjectionX();

  TH2F* mun_qcd = 0;
  TH2F* eln_qcd = 0;
  if (var.Contains("_jet")) {
    mun_qcd = (TH2F*) f_qcd_mu->Get("Nt"+var+"_histo_mu");  
    eln_qcd = (TH2F*) f_qcd_el->Get("Nt"+var+"_histo_e");
  } else {
    mun_qcd = (TH2F*) f_qcd_mu->Get("Nt_histo_mu");  
    eln_qcd = (TH2F*) f_qcd_el->Get("Nt_histo_e");
  }

  TH1D* mun1d_qcd = mun_qcd->ProjectionX();
  TH1D* eln1d_qcd = eln_qcd->ProjectionX();

  TH1D *muf_qcd = (TH1D*) mun1d_qcd->Clone("rate_histo_mu");
  muf_qcd->Divide(mun1d_qcd,mud1d_qcd,1,1,"B");
  TH1D *elf_qcd = (TH1D*) eln1d_qcd->Clone("rate_histo_el");
  elf_qcd->Divide(eln1d_qcd,eld1d_qcd,1,1,"B");

  TFile* f_data = TFile::Open("rate_histos_data"+postfix+".root");
  TFile* f_dy = TFile::Open("rate_histos_dy"+postfix+".root");
  TFile* f_wj = TFile::Open("rate_histos_wj"+postfix+".root");

  TH2F* mud_data = (TH2F*) f_data->Get("Nl"+var+"_histo_mu");
  TH2F* mud_wj = (TH2F*) f_wj->Get("Nl"+var+"_histo_mu");
  TH2F* mud_dy = (TH2F*) f_dy->Get("Nl"+var+"_histo_mu");
  TH2F* eld_data = (TH2F*) f_data->Get("Nl"+var+"_histo_e");
  TH2F* eld_wj = (TH2F*) f_wj->Get("Nl"+var+"_histo_e");
  TH2F* eld_dy = (TH2F*) f_dy->Get("Nl"+var+"_histo_e");

  //scale factors from MT control region plot
  float muSF = 0.539734;//0.600114;
  float elSF = 0.416113;//0.351519;//0.682205;

  mud_data->Add(mud_wj,-1.*muSF);
  mud_data->Add(mud_dy,-1.*muSF);
  eld_data->Add(eld_wj,-1.*elSF);
  eld_data->Add(eld_dy,-1.*elSF);

  TH1D* mud1d_data = mud_data->ProjectionX();
  TH1D* eld1d_data = eld_data->ProjectionX();

  TH2F* mun_data = 0;
  TH2F* mun_wj   = 0;
  TH2F* mun_dy   = 0;
  TH2F* eln_data = 0;
  TH2F* eln_wj   = 0;
  TH2F* eln_dy   = 0;
  if (var.Contains("_jet")) {
    mun_data = (TH2F*) f_data->Get("Nt"+var+"_histo_mu");
    mun_wj   = (TH2F*) f_wj->Get("Nt"+var+"_histo_mu");
    mun_dy   = (TH2F*) f_dy->Get("Nt"+var+"_histo_mu");
    eln_data = (TH2F*) f_data->Get("Nt"+var+"_histo_e");
    eln_wj   = (TH2F*) f_wj->Get("Nt"+var+"_histo_e");
    eln_dy   = (TH2F*) f_dy->Get("Nt"+var+"_histo_e");
  } else {
    mun_data = (TH2F*) f_data->Get("Nt_histo_mu");
    mun_wj   = (TH2F*) f_wj->Get("Nt_histo_mu");
    mun_dy   = (TH2F*) f_dy->Get("Nt_histo_mu");
    eln_data = (TH2F*) f_data->Get("Nt_histo_e");
    eln_wj   = (TH2F*) f_wj->Get("Nt_histo_e");
    eln_dy   = (TH2F*) f_dy->Get("Nt_histo_e");
  }

  mun_data->Add(mun_wj,-1.*muSF);
  mun_data->Add(mun_dy,-1.*muSF);
  eln_data->Add(eln_wj,-1.*elSF);
  eln_data->Add(eln_dy,-1.*elSF);

  TH1D* mun1d_data = mun_data->ProjectionX();
  TH1D* eln1d_data = eln_data->ProjectionX();

  TH1D *muf_data = (TH1D*) mun1d_data->Clone("rate_histo_mu");
  muf_data->Divide(mun1d_data,mud1d_data,1,1,"B");
  muf_data->SetMarkerStyle(20);
  TH1D *elf_data = (TH1D*) eln1d_data->Clone("rate_histo_el");
  elf_data->Divide(eln1d_data,eld1d_data,1,1,"B");
  elf_data->SetMarkerStyle(20);

  std::vector <string> Titles;
  Titles.push_back("QCD MC");

  TH1F* muf_data_copy = new TH1F("data","data",1,0,1);
  muf_data->Copy(*muf_data_copy);
  TH1F* muf_qcd_copy = new TH1F("qcd","qcd",1,0,1);
  muf_qcd->Copy(*muf_qcd_copy);
  std::vector<TH1F*> mu_qcd_vec;
  mu_qcd_vec.push_back(muf_qcd_copy);
  dataMCplotMaker(muf_data_copy, mu_qcd_vec, Titles, "", "", Form("--outputName mu_1dfr%s%s --png --noStack --isLinear --drawDots --lumi 0.040 --xAxisLabel muon p_{T}^{corr.} --xAxisUnit GeV --setMaximum 0.5 --errHistAtBottom",var.Data(),postfix.Data())/*, std::vector<TH1F*>(), std::vector<string>(), std::vector<Color_t>()*/);

  TH1F* elf_data_copy = new TH1F("data","data",1,0,1);
  elf_data->Copy(*elf_data_copy);
  TH1F* elf_qcd_copy = new TH1F("qcd","qcd",1,0,1);
  elf_qcd->Copy(*elf_qcd_copy);
  std::vector<TH1F*> el_qcd_vec;
  el_qcd_vec.push_back(elf_qcd_copy);
  dataMCplotMaker(elf_data_copy, el_qcd_vec, Titles, "", "", Form("--outputName el_1dfr%s%s --png --noStack --isLinear --drawDots --lumi 0.040 --xAxisLabel electron p_{T}^{corr.} --xAxisUnit GeV --setMaximum 0.5 --errHistAtBottom",var.Data(),postfix.Data())/*, std::vector<TH1F*>(), std::vector<string>(), std::vector<Color_t>()*/);

  // muf_qcd->SetTitle("");
  // muf_qcd->GetYaxis()->SetTitleOffset(1.25);
  // muf_qcd->GetYaxis()->SetTitle("muon fake rate");
  // muf_qcd->GetXaxis()->SetTitle("muon p_{T} [GeV]");
  // if (var=="_cone") muf_qcd->GetXaxis()->SetTitle("muon p_{T}^{corr.} [GeV]");
  // if (var=="_jet") muf_qcd->GetXaxis()->SetTitle("jet p_{T} [GeV]");
  // TCanvas c1("c1","c1",600,600);
  // muf_qcd->GetYaxis()->SetRangeUser(0,0.7);
  // muf_qcd->SetMarkerStyle(21);
  // muf_qcd->SetMarkerColor(kRed);
  // muf_qcd->SetLineColor(kRed);
  // muf_qcd->Draw("PE");
  // muf_data->Draw("PEsame");
  // c1.RedrawAxis();

  // TLegend* leg = new TLegend(0.7,0.7,0.89,0.89);
  // leg->SetFillColor(kWhite);
  // leg->SetLineColor(kWhite);
  // // leg->SetHeader("CMS Preliminary, L~few/pb");
  // leg->AddEntry(muf_qcd,"QCD MC","ple");
  // leg->AddEntry(muf_data,"data","ple");
  // leg->Draw();

  // TPaveText* labelcms  = new TPaveText(0.12,0.75,0.5,0.89,"NDCBR");
  // labelcms->SetTextAlign(12);
  // labelcms->SetTextSize(0.035);
  // labelcms->SetFillColor(kWhite);
  // labelcms->AddText("CMS Preliminary");
  // labelcms->AddText("#sqrt{s} = 13 TeV");
  // labelcms->AddText("L = 43 pb^{-1}");
  // labelcms->SetBorderSize(0);
  // labelcms->SetTextFont(42);
  // labelcms->SetLineWidth(2);
  // labelcms->Draw();

  // elf_qcd->SetTitle("");
  // elf_qcd->GetYaxis()->SetTitleOffset(1.25);
  // elf_qcd->GetYaxis()->SetTitle("electron fake rate");
  // elf_qcd->GetXaxis()->SetTitle("electron p_{T} [GeV]");
  // if (var=="_cone") elf_qcd->GetXaxis()->SetTitle("electron p_{T}^{corr.} [GeV]");
  // if (var=="_jet") elf_qcd->GetXaxis()->SetTitle("jet p_{T} [GeV]");
  // TCanvas c2("c2","c2",600,600);
  // elf_qcd->GetYaxis()->SetRangeUser(0,0.7);
  // elf_qcd->SetMarkerStyle(21);
  // elf_qcd->SetMarkerColor(kRed);
  // elf_qcd->SetLineColor(kRed);
  // elf_qcd->Draw("PE");
  // elf_data->Draw("PEsame");
  // c2.RedrawAxis();
  // leg->Draw();
  // labelcms->Draw();

  // c1.SaveAs("mu_1dfr"+var+postfix+"_2.png");
  // c2.SaveAs("el_1dfr"+var+postfix+".png");

}
