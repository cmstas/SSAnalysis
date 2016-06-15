//root -b -q srPlot.C+
#include "../../software/dataMCplotMaker/dataMCplotMaker.cc"
#include "TFile.h"
#include "TH1F.h"
void srPlot() {

  TFile* f_m1200_m700 = TFile::Open("v6.02-statunc/fs_t1tttt_m1200_m800_histos_hihi_2.3ifb.root");
  TH1F* sr_m1200_m700 = (TH1F*)f_m1200_m700->Get("sr");
  TH1F* h1 = new TH1F("h1","h1",32,0.5,32.5);
  for (int bin = 1; bin<=32; bin++) h1->SetBinContent(bin,sr_m1200_m700->GetBinContent(bin));

  TFile* f_t5qqqqww = TFile::Open("v6.02-statunc/fs_t5qqqqvv_m1000_m700_histos_hihi_2.3ifb.root");
  TH1F* sr_t5qqqqww = (TH1F*)f_t5qqqqww->Get("sr");
  TH1F* h2 = new TH1F("h2","h2",32,0.5,32.5);
  for (int bin = 1; bin<=32; bin++) h2->SetBinContent(bin,sr_t5qqqqww->GetBinContent(bin));

  TFile* f_t6ttww = TFile::Open("v6.02-statunc/fs_t6ttww_50_m650_m550_histos_hihi_2.3ifb.root");
  TH1F* sr_t6ttww = (TH1F*)f_t6ttww->Get("sr");
  TH1F* h3 = new TH1F("h3","h3",32,0.5,32.5);
  for (int bin = 1; bin<=32; bin++) h3->SetBinContent(bin,sr_t6ttww->GetBinContent(bin));

  vector <string> titles;
  titles.push_back("T1tttt #scale[0.8]{m_{#tilde{g}}=1200, m_{#tilde{#chi}^{0}_{1}}=800 GeV}"); 
  titles.push_back("T5qqqqWW #scale[0.8]{m_{#tilde{g}}=1000, m_{#tilde{#chi}^{#pm}_{1}}=850, m_{#tilde{#chi}^{0}_{1}}=700 GeV}"); 
  //titles.push_back("#splitline{T6ttWW}{#scale[0.8]{m_{#tilde{b}}=600, m_{#tilde{#chi}^{#pm}}=425, m_{LSP}=50 GeV}}"); 
  titles.push_back("T6ttWW #scale[0.8]{m_{#tilde{b}}=650, m_{#tilde{#chi}^{#pm}_{1}}=550, m_{#tilde{#chi}^{0}_{1}}=50 GeV}"); 

  vector <Color_t> colors;
  colors.push_back(kRed); 
  colors.push_back(kBlue); 
  colors.push_back(kBlack); 

  vector<TH1F*> SRHH_plots;
  SRHH_plots.push_back(h1);
  SRHH_plots.push_back(h2);
  SRHH_plots.push_back(h3);
  dataMCplotMaker(new TH1F("data","data",32,0.5,32.5), SRHH_plots, titles, "HH SRs", "", "--outputName HHSR-sig.pdf --xAxisLabel SR --noXaxisUnit --yAxisLabel a.u. --isLinear --yTitleOffset -0.2 --legendUp -0.05  --legendRight -0.45 --noOverflow --noRatioPlot --noStack --normalize --setMaximum 0.5 --noLumi --type Supplementary (Simulation) --outOfFrame", vector <TH1F*>(), vector <string>(), colors); 

}
