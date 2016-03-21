//root -b -q srPlot.C+
#include "../../software/dataMCplotMaker/dataMCplotMaker.cc"
#include "TFile.h"
#include "TH1F.h"
void srPlot() {

  TFile* f_m1200_m700 = TFile::Open("v5.07/fs_t1tttt_m1200_m700_histos_hihi_2.2ifb.root");
  TH1F* sr_m1200_m700 = (TH1F*)f_m1200_m700->Get("sr");

  TFile* f_t5qqqqww = TFile::Open("v5.07/t5qqqqww_1200_histos_hihi_2.2ifb.root");
  TH1F* sr_t5qqqqww = (TH1F*)f_t5qqqqww->Get("sr");

  TFile* f_t6ttww = TFile::Open("v5.07/t6ttww_600_histos_hihi_2.2ifb.root");
  TH1F* sr_t6ttww = (TH1F*)f_t6ttww->Get("sr");

  vector <string> titles;
  titles.push_back("T1tttt #scale[0.8]{m_{#tilde{g}}=1200, m_{LSP}=700 GeV}"); 
  titles.push_back("T5qqqqWW #scale[0.8]{m_{#tilde{g}}=1200, m_{#tilde{#chi}^{#pm}}=1000, m_{LSP}=800 GeV}"); 
  //titles.push_back("#splitline{T6ttWW}{#scale[0.8]{m_{#tilde{b}}=600, m_{#tilde{#chi}^{#pm}}=425, m_{LSP}=50 GeV}}"); 
  titles.push_back("T6ttWW #scale[0.8]{m_{#tilde{b}}=600, m_{#tilde{#chi}^{#pm}}=425, m_{LSP}=50 GeV}"); 

  vector <Color_t> colors;
  colors.push_back(kRed); 
  colors.push_back(kBlue); 
  colors.push_back(kBlack); 

  vector<TH1F*> SRHH_plots;
  SRHH_plots.push_back(sr_m1200_m700);
  SRHH_plots.push_back(sr_t5qqqqww);
  SRHH_plots.push_back(sr_t6ttww);
  dataMCplotMaker(new TH1F("data","data",32,1,33), SRHH_plots, titles, "H-H SRs", "", "--outputName HHSR-sig.pdf --xAxisLabel SR --noXaxisUnit --yAxisLabel a.u. --isLinear  --legendUp -0.02  --legendRight -0.40 --noOverflow --noRatioPlot --noStack --normalize --setMaximum 0.4 --noLumi --type Simulation --outOfFrame", vector <TH1F*>(), vector <string>(), colors); 

}
