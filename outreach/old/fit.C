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
#include "TF1.h"
#include "TDRStyle.h"

#include "eff.h"
#include "ScaleFactors.h"


double fitf_btag(double *x, double *par){
  double fitval = 0;
  if (x[0] >= 120) fitval = par[0] + par[1]*x[0];
  if (x[0] < 120) fitval = par[2]*x[0]*x[0]*x[0] + par[3]*x[0]*x[0] + par[4]*x[0] + par[5];
  return fitval;
}

double fitf(double *x, double *par){
  double fitval = par[1] > 0 ? par[0]*erf((x[0]-10)/par[1]) + par[2]*(1-erf((x[0]-10)/par[1])) : 0;
  return fitval;
}

double fitf_jets(double *x, double *par){
  double fitval = .5*par[0]*(1 + erf((x[0]-par[1])/par[2]));
  return fitval;
}

double scale_factor(int id_gen, float pt_gen, float eta_gen, bool isElec){
  double factor = 1.0;
  factor *= TagAndProbeScaleFactor(id_gen, pt_gen, eta_gen);
  if (isElec == true) factor *= DileptonTriggerScaleFactor(11, pt_gen, eta_gen); 
  if (isElec == false) factor *= DileptonTriggerScaleFactor(13, pt_gen, eta_gen); 
  return factor;
}

void eff_curve(const char* name){
  TFile *file = new TFile(Form("%s", name));
  TTree *tree = (TTree*)file->Get("tree");
  
  gStyle->SetOptStat(0);

  const int size = 11;
  float binx[size] = {10,15,20,25,30,40,50,65,80,100,200};
  //float binx[size] = {10,15,20,25,30,40,60,80,100,120};

  //Electron by pT
  TH1F* elec_eff_numer = new TH1F("elec_eff_numer", "elec_eff_numer", size-1, binx);
  tree->Draw("pt1_gen>>elec_eff_numer","(abs(id1_gen)==11 && ht_jets_gen>200 && met_gen>50 && isNumeratorLepton_1 && njets_jets_gen >= 2)*scale_factor(id1_gen, pt1_gen, eta1_gen, 1)"); 
  tree->Draw("pt2_gen>>+elec_eff_numer","(abs(id2_gen)==11 && ht_jets_gen>200 && met_gen>50 && isNumeratorLepton_2 && njets_jets_gen >= 2)*scale_factor(id2_gen, pt2_gen, eta2_gen, 1)");

  TH1F* elec_eff_denom = new TH1F("elec_eff_denom", "elec_eff_denom", size-1, binx);
  tree->Draw("pt1_gen>>elec_eff_denom"," (abs(id1_gen)==11 && ht_jets_gen>200 && met_gen>50 && njets_jets_gen >= 2)");
  tree->Draw("pt2_gen>>+elec_eff_denom","(abs(id2_gen)==11 && ht_jets_gen>200 && met_gen>50 && njets_jets_gen >= 2)");

  TH1F* elec_eff;
  elec_eff = eff(elec_eff_denom, elec_eff_numer, "elec_pt");
  elec_eff->GetYaxis()->SetLimits(0.2, 0.7);
  elec_eff->SetTitle("Reco Efficiency by pT for Electrons");

  //Muon by pT
  TH1F* muon_eff_numer = new TH1F("muon_eff_numer", "muon_eff_numer", size-1, binx);
  tree->Draw("pt1_gen>>muon_eff_numer"," (abs(id1_gen)==13 && ht_jets_gen>200 && met_gen>50 && isNumeratorLepton_1 && njets_jets_gen >= 2)*scale_factor(id1_gen, pt1_gen, eta1_gen, 0)");
  tree->Draw("pt2_gen>>+muon_eff_numer","(abs(id2_gen)==13 && ht_jets_gen>200 && met_gen>50 && isNumeratorLepton_2 && njets_jets_gen >= 2)*scale_factor(id2_gen, pt2_gen, eta2_gen, 0)");

  TH1F* muon_eff_denom = new TH1F("muon_eff_denom", "muon_eff_denom", size-1, binx);
  tree->Draw("pt1_gen>>muon_eff_denom"," (abs(id1_gen)==13 && ht_jets_gen>200 && met_gen>50 && njets_jets_gen >= 2)");
  tree->Draw("pt2_gen>>+muon_eff_denom","(abs(id2_gen)==13 && ht_jets_gen>200 && met_gen>50 && njets_jets_gen >= 2)");

  TH1F* muon_eff;
  muon_eff = eff(muon_eff_denom, muon_eff_numer, "muon_pt");
  muon_eff->GetYaxis()->SetLimits(0.2, 0.7);
  cout << "min: " <<  muon_eff->GetMinimum() << endl;
  muon_eff->SetTitle("Reco Efficiency by pT for Muons");

  //jet is matched
  TH1F* njets_eff_numer = new TH1F("njets_eff_numer", "njets_eff_numer", 40,0,400);
  tree->Draw("jets_gen.pt()>>njets_eff_numer","ht_jets_gen>200 && met_gen>50 && jet_is_matched");

  TH1F* njets_eff_denom = new TH1F("njets_eff_denom", "njets_eff_denom", 40, 0, 400);
  tree->Draw("jets_gen.pt()>>njets_eff_denom","met_gen>50 && ht_jets_gen>200");

  TH1F* njets_eff;
  njets_eff = eff(njets_eff_denom, njets_eff_numer, "njets_part");
  njets_eff->SetTitle("Efficiency of jet reconstruction");

  //jet is matched
  TH1F* njets_eff_numer2 = new TH1F("njets_eff_numer2", "njets_eff_numer2", 40,0,400);
  tree->Draw("jets_direct_gen.pt()>>njets_eff_numer2","ht_jets_gen>200 && met_gen>50 && jet_is_matched_direct");

  TH1F* njets_eff_denom2 = new TH1F("njets_eff_denom2", "njets_eff_denom2", 40, 0, 400);
  tree->Draw("jets_direct_gen.pt()>>njets_eff_denom2","met_gen>50 && ht_jets_gen>200");

  TH1F* njets_eff2;
  njets_eff2 = eff(njets_eff_denom2, njets_eff_numer2, "njets_jets");
  njets_eff2->SetTitle("Efficiency of jet reconstruction");

  //hT > 200
  TH1F* ht1_eff_numer = new TH1F("ht1_eff_numer", "ht1_eff_numer", 56, 100, 500);
  tree->Draw("ht_jets_gen>>ht1_eff_numer","ht_reco>200 && met_gen>50 && njets_jets_gen>=2");

  TH1F* ht1_eff_denom = new TH1F("ht1_eff_denom", "ht1_eff_denom", 56, 100, 500);
  tree->Draw("ht_jets_gen>>ht1_eff_denom","met_gen>50 && njets_jets_gen>=2");

  TH1F* ht1_eff;
  ht1_eff = eff(ht1_eff_denom, ht1_eff_numer, "ht1");
  ht1_eff->SetTitle("Efficiency of Reco hT > 200 cut");

  //hT > 400
  TH1F* ht2_eff_numer = new TH1F("ht2_eff_numer", "ht2_eff_numer", 56, 100, 500);
  tree->Draw("ht_jets_gen>>ht2_eff_numer","ht_reco>400 && met_gen>50 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

  TH1F* ht2_eff_denom = new TH1F("ht2_eff_denom", "ht2_eff_denom", 56, 100, 500);
  tree->Draw("ht_jets_gen>>ht2_eff_denom","met_gen>50 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

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

  //hT > 250 (direct)
  TH1F* ht5_eff_numer = new TH1F("ht5_eff_numer", "ht5_eff_numer", 56, 100, 800);
  tree->Draw("ht_jets_gen>>ht5_eff_numer","ht_direct_reco>250 && met_gen>50 && njets_jets_gen>=2");

  TH1F* ht5_eff_denom = new TH1F("ht5_eff_denom", "ht5_eff_denom", 56, 100, 800);
  tree->Draw("ht_jets_gen>>ht5_eff_denom","met_gen>50 && njets_jets_gen>=2");

  TH1F* ht5_eff;
  ht5_eff = eff(ht5_eff_denom, ht5_eff_numer, "ht5");
  ht5_eff->SetTitle("Efficiency of Reco hT > 250 cut");

  //hT > 500 (direct)
  TH1F* ht6_eff_numer = new TH1F("ht6_eff_numer", "ht6_eff_numer", 56, 100, 800);
  tree->Draw("ht_jets_gen>>ht6_eff_numer","ht_direct_reco>500 && met_gen>50 && njets_jets_gen >= 2");

  TH1F* ht6_eff_denom = new TH1F("ht6_eff_denom", "ht6_eff_denom", 56, 100, 800);
  tree->Draw("ht_jets_gen>>ht6_eff_denom","met_gen>50 && njets_jets_gen >= 2");

  TH1F* ht6_eff;
  ht6_eff = eff(ht6_eff_denom, ht6_eff_numer, "ht6");
  ht6_eff->SetTitle("Efficiency of Reco hT > 500 cut");

  //met > 30
  TH1F* met1_eff_numer = new TH1F("met1_eff_numer", "met1_eff_numer", 25, 0, 500);
  tree->Draw("met_gen>>met1_eff_numer","met_reco>30 && ht_jets_gen>500 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20");

  TH1F* met1_eff_denom = new TH1F("met1_eff_denom", "met1_eff_denom", 25, 0, 500);
  tree->Draw("met_gen>>met1_eff_denom","ht_jets_gen>500 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met1_eff;
  met1_eff = eff(met1_eff_denom, met1_eff_numer, "met1");
  met1_eff->SetTitle("Efficiency of Reco met > 30 cut");

  //met > 50
  TH1F* met2_eff_numer = new TH1F("met2_eff_numer", "met2_eff_numer", 50, 0, 500);
  tree->Draw("met_gen>>met2_eff_numer","met_reco>50 && ht_jets_gen>500 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met2_eff_denom = new TH1F("met2_eff_denom", "met2_eff_denom", 50, 0, 500);
  tree->Draw("met_gen>>met2_eff_denom","ht_jets_gen>500 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met2_eff;
  met2_eff = eff(met2_eff_denom, met2_eff_numer, "met2");
  met2_eff->SetTitle("Efficiency of Reco met > 30 cut");

  //met > 120
  TH1F* met3_eff_numer = new TH1F("met3_eff_numer", "met3_eff_numer", 50, 0, 500);
  tree->Draw("met_gen>>met3_eff_numer","met_reco>120 && ht_jets_gen>500 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met3_eff_denom = new TH1F("met3_eff_denom", "met3_eff_denom", 50, 0, 500);
  tree->Draw("met_gen>>met3_eff_denom","ht_jets_gen>500 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 ");

  TH1F* met3_eff;
  met3_eff = eff(met3_eff_denom, met3_eff_numer, "met3");
  met3_eff->SetTitle("Efficiency of Reco met > 30 cut");

  //pT of each btag  -- from jets
  TH1F* btag_eff_numer = new TH1F("btag_eff_numer", "btag_eff_numer", 50, 0, 600);
  tree->Draw("btags_direct_gen.pt()>>btag_eff_numer","met_gen>50 && ht_jets_gen>200 && njets_jets_gen >= 2 && gen_btag_is_matched_direct");

  TH1F* btag_eff_denom = new TH1F("btag_eff_denom", "btag_eff_denom", 50, 0, 600);
  tree->Draw("btags_direct_gen.pt()>>btag_eff_denom","met_gen>50 && ht_jets_gen>200 && njets_jets_gen >= 2");

  TH1F* btag_eff;
  btag_eff = eff(btag_eff_denom, btag_eff_numer, "btag");
  btag_eff->SetTitle("Efficiency of btag cut (jets)");

  //pT of each btag -- from partons
  TH1F* btag2_eff_numer = new TH1F("btag2_eff_numer", "btag2_eff_numer", 50, 0, 400);
  tree->Draw("btags_direct_gen.pt()>>btag2_eff_numer","met_gen>50 && ht_jets_gen>200 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 && gen_btag_is_matched && btags_direct_gen.pt()>40 && btags_direct_gen.pt()<400");

  TH1F* btag2_eff_denom = new TH1F("btag2_eff_denom", "btag2_eff_denom", 50, 0, 400);
  tree->Draw("btags_direct_gen.pt()>>btag2_eff_denom","met_gen>50 && ht_jets_gen>200 && njets_jets_gen >= 2 && pt1_gen >= 20 && pt2_gen >= 20 && btags_direct_gen.pt()>40 && btags_direct_gen.pt() < 400");

  TH1F* btag2_eff;
  btag2_eff = eff(btag2_eff_denom, btag2_eff_numer, "btag2");
  btag2_eff->SetTitle("Efficiency of btag cut (partons)");

  //Print needed line in case you're printing to file
  cout << "#include \"fits.h\"" << endl; 
  cout << " " << endl;

  //Fit electron id & iso eff
  TF1 *func = new TF1("fit", fitf, -3, 3, 3);
  func->SetParameters(1,20,0);
  func->SetParNames("A","B","C");
  func->SetLineColor(kBlack);
  elec_eff->Fit("fit","Q");
  cout << "float electron_prob(float x){ " << endl;
  cout << "  return " << func->GetParameter(0)  << "*erf((x-10)/" << func->GetParameter(1)  << ") + " << func->GetParameter(2)  << "*(1-erf((x-10)/" << func->GetParameter(1)  << "));" << endl;
  cout << func->GetParameter(0) << " " << func->GetParError(0) << endl;
  cout << func->GetParameter(1) << " " << func->GetParError(1) << endl;
  cout << func->GetParameter(2) << " " << func->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  func->SetParameters(.66,45,0.33);
  muon_eff->Fit("fit","Q");
  cout << "float muon_prob(float x){ " << endl;
  cout << "  return " << func->GetParameter(0)  << "*erf((x-10)/" << func->GetParameter(1)  << ") + " << func->GetParameter(2)  << "*(1-erf((x-10)/" << func->GetParameter(1)  << "));" << endl;
  cout << func->GetParameter(0) << " " << func->GetParError(0) << endl;
  cout << func->GetParameter(1) << " " << func->GetParError(1) << endl;
  cout << func->GetParameter(2) << " " << func->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;

  //Fit btag
  TF1 *func2 = new TF1("fit_btag", fitf_btag, -6,6,6);
  func2->SetParameters(0,0,0,0,0,0);
  func2->SetParNames("A","B","C","D","E","F");
  func2->SetLineColor(kBlack);
  btag2_eff->Fit("fit_btag","Q","",40,400);
  cout << "float btag_prob_part(float x){ " << endl;
  cout << "  float fitval = 0;" << endl;
  cout << "  if (x > 120) fitval = " << func2->GetParameter(0) << " + " << func2->GetParameter(1) << "*x" << ";" << endl;
  cout << "  if (x <= 120) fitval = " << func2->GetParameter(2) << " *x*x*x +  " << func2->GetParameter(3) << "*x*x + " << func2->GetParameter(4) << "*x + " << func2->GetParameter(5) << ";" << endl;
  cout << func2->GetParameter(0) << " " << func2->GetParError(0) << endl;
  cout << func2->GetParameter(1) << " " << func2->GetParError(1) << endl;
  cout << func2->GetParameter(2) << " " << func2->GetParError(2) << endl;
  cout << func2->GetParameter(3) << " " << func2->GetParError(3) << endl;
  cout << func2->GetParameter(4) << " " << func2->GetParError(4) << endl;
  cout << func2->GetParameter(5) << " " << func2->GetParError(5) << endl;
  cout << "  return fitval;" << endl;
  cout << "}" << endl;
  cout << " " << endl;
  func2->SetParameters(0,0,0,0,0);
  //btag_eff->Fit("fit_btag","Q","",40,400);
  btag_eff->Fit("fit_btag","","",40,600);
  cout << "float btag_prob_jets(float x){ " << endl;
  cout << "  float fitval = 0;" << endl;
  cout << "  if (x > 120) fitval = " << func2->GetParameter(0) << " + " << func2->GetParameter(1) << "*x" << ";" << endl;
  cout << "  if (x <= 120) fitval = " << func2->GetParameter(2) << " *x*x*x +  " << func2->GetParameter(3) << "*x*x + " << func2->GetParameter(4) << "*x + " << func2->GetParameter(5) << ";" << endl;
  cout << func2->GetParameter(0) << " " << func2->GetParError(0) << endl;
  cout << func2->GetParameter(1) << " " << func2->GetParError(1) << endl;
  cout << func2->GetParameter(2) << " " << func2->GetParError(2) << endl;
  cout << func2->GetParameter(3) << " " << func2->GetParError(3) << endl;
  cout << func2->GetParameter(4) << " " << func2->GetParError(4) << endl;
  cout << func2->GetParameter(5) << " " << func2->GetParError(5) << endl;
  cout << "  return fitval;" << endl;
  cout << "}" << endl;
  cout << " " << endl;

  //Fit ht & met & njets
  TF1 *fun3 = new TF1("fit_jets", fitf_jets, -3, 3, 3);
  fun3->SetParameters(1,200,150);
  fun3->SetParNames("A","B");
  fun3->SetLineColor(kBlack);
  met1_eff->Fit("fit_jets","Q");
  cout << "float met_prob_30(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  met2_eff->Fit("fit_jets","Q");
  cout << "float met_prob_50(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  met3_eff->Fit("fit_jets","Q");
  cout << "float met_prob_120(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  ht1_eff->Fit("fit_jets","Q");
  cout << "float ht_prob_part_200(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  ht2_eff->Fit("fit_jets","Q");
  cout << "float ht_prob_part_400(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  fun3->SetParameters(1,200,150);
  ht3_eff->Fit("fit_jets","Q");
  cout << "float ht_prob_jets_200(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  fun3->SetParameters(1,200,150);
  ht4_eff->Fit("fit_jets","Q");
  cout << "float ht_prob_jets_400(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  fun3->SetParameters(1,200,150);
  ht5_eff->Fit("fit_jets","Q");
  cout << "float ht_prob_jets_250(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  fun3->SetParameters(1,200,150);
  ht6_eff->Fit("fit_jets","Q");
  cout << "float ht_prob_jets_500(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  fun3->SetParameters(1,200,150);
  njets_eff->Fit("fit_jets","Q");
  cout << "float jet_prob_part(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;
  fun3->SetParameters(1,200,150);
  njets_eff->Fit("fit_jets","Q");
  njets_eff2->Fit("fit_jets","Q");
  cout << "float jet_prob_jets(float x){" << endl;
  cout << "return .5*" << fun3->GetParameter(0) << "*(1 + erf((x-" << fun3->GetParameter(1) << ")/" << fun3->GetParameter(2) << "));" << endl;
  cout << fun3->GetParameter(0) << " " << fun3->GetParError(0) << endl;
  cout << fun3->GetParameter(1) << " " << fun3->GetParError(1) << endl;
  cout << fun3->GetParameter(2) << " " << fun3->GetParError(2) << endl;
  cout << "}" << endl;
  cout << " " << endl;

  TCanvas *c0 = new TCanvas;
  THStack *s0 = new THStack("stack", "Lepton Efficiency by pT");
  elec_eff->SetMaximum(.7);
  elec_eff->SetMinimum(.2);
  elec_eff->SetLineColor(kBlue);
  muon_eff->SetLineColor(kRed);
  elec_eff->SetMarkerColor(kBlue);
  muon_eff->SetMarkerColor(kRed);
  elec_eff->SetLineWidth(2);
  muon_eff->SetLineWidth(2);
  s0->Add(elec_eff);
  s0->Add(muon_eff);
  TLegend *leg0 = new TLegend(.65, .61, 0.93, .46);
  leg0->AddEntry(elec_eff, "electrons"); 
  leg0->AddEntry(muon_eff, "muons"); 
  leg0->SetBorderSize(0);//remove border and shadow box.
  leg0->SetTextSize(0.05);
  leg0->SetFillStyle(0); //make legend transparent
  c0->SetRightMargin(0.05);
  s0->SetMinimum(0);
  s0->Draw("nostack");
  s0->GetXaxis()->SetLimits(0.0, 120.0);
  leg0->Draw();
  TLine *line100 = new TLine(100,elec_eff->GetBinContent(10),120,elec_eff->GetBinContent(10));
  line100->SetLineColor(kBlue);
  line100->SetLineWidth(2);
  line100->Draw();
  TLine *line101 = new TLine(100,muon_eff->GetBinContent(10),120,muon_eff->GetBinContent(10));
  line101->SetLineColor(kRed);
  line101->SetLineWidth(2);
  line101->Draw();
  s0->SetTitle("CMS Simulation #sqrt{s} = 8 TeV");
  s0->GetYaxis()->SetTitle("Efficiency");
  s0->GetXaxis()->SetTitle("gen lepton p_{T} (GeV)");

  TCanvas *c1 = new TCanvas;
  THStack *s1 = new THStack("stack", "Efficiency of hT Cut");
  ht1_eff->SetMaximum(1.05);
  ht1_eff->SetLineColor(kBlue);
  ht2_eff->SetLineColor(kRed);
  ht1_eff->SetMarkerColor(kBlue);
  ht2_eff->SetMarkerColor(kRed);
  ht1_eff->SetLineWidth(5);
  ht2_eff->SetLineWidth(5);
  s1->Add(ht1_eff);
  s1->Add(ht2_eff);
  TLegend *leg1 = new TLegend(.86, .46, 1.0, .26);
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
  s1->SetTitle("CMS Simulation #sqrt{s} = 8 TeV");
  s1->GetYaxis()->SetTitle("Efficiency");
  s1->GetXaxis()->SetTitle("H_{T}^{gen} (GeV)");

  TCanvas *c5 = new TCanvas;
  THStack *s5 = new THStack("stack", "Efficiency of hT Cut");
  ht3_eff->SetMaximum(1.05);
  ht5_eff->SetMaximum(1.05);
  ht3_eff->SetLineColor(kBlue);
  ht5_eff->SetLineColor(kGreen);
  ht4_eff->SetLineColor(kRed);
  ht6_eff->SetLineColor(kMagenta);
  ht3_eff->SetLineWidth(2);
  ht5_eff->SetLineWidth(2);
  ht4_eff->SetLineWidth(2);
  ht6_eff->SetLineWidth(2);
  ht3_eff->SetMarkerColor(kBlue);
  ht5_eff->SetMarkerColor(kGreen);
  ht4_eff->SetMarkerColor(kRed);
  ht6_eff->SetMarkerColor(kMagenta);
  s5->Add(ht3_eff);
  s5->Add(ht5_eff);
  s5->Add(ht4_eff);
  s5->Add(ht6_eff);
  TLegend *leg5 = new TLegend(.64, .40, 0.84, .21);
  leg5->AddEntry(ht3_eff, "H_{T} > 200 GeV"); 
  leg5->AddEntry(ht5_eff, "H_{T} > 250 GeV"); 
  leg5->AddEntry(ht4_eff, "H_{T} > 400 GeV"); 
  leg5->AddEntry(ht6_eff, "H_{T} > 500 GeV"); 
  leg5->SetBorderSize(0);//remove border and shadow box.
  leg5->SetTextSize(0.05);
  leg5->SetFillStyle(0); //make legend transparent
  TLine *line6 = new TLine(400,0,400,1.05);
  line6->SetLineColor(15);
  line6->SetLineStyle(3);
  TLine *line7 = new TLine(200,0,200,1.05);
  line7->SetLineColor(15);
  line7->SetLineStyle(3);
  TLine *line102 = new TLine(250,0,250,1.05);
  line102->SetLineColor(15);
  line102->SetLineStyle(3);
  TLine *line103 = new TLine(500,0,500,1.05);
  line103->SetLineColor(15);
  line103->SetLineStyle(3);
  //c5->SetRightMargin(0.05);
  s5->SetMinimum(0);
  s5->Draw("nostack");
  //s5->GetXaxis()->SetLimits(100.0,600.0);
  leg5->Draw();
  line6->Draw();
  line7->Draw();
  line102->Draw();
  line103->Draw();
  s5->SetTitle("CMS Simulation #sqrt{s} = 8 TeV");
  s5->GetYaxis()->SetTitle("Efficiency");
  s5->GetXaxis()->SetTitle("gen H_{T} (GeV)");

  TCanvas *c2 = new TCanvas;
  THStack *s2 = new THStack("stack", "Efficiency of met Cut");
  met1_eff->SetMaximum(1);
  met1_eff->SetLineColor(kBlue);
  met2_eff->SetLineColor(kRed);
  met3_eff->SetLineColor(kGreen+2);
  met1_eff->SetMarkerColor(kBlue);
  met2_eff->SetMarkerColor(kRed);
  met3_eff->SetMarkerColor(kGreen+2);
  met1_eff->SetLineWidth(2);
  met2_eff->SetLineWidth(2);
  met3_eff->SetLineWidth(2);
  s2->Add(met1_eff);
  s2->Add(met2_eff);
  s2->Add(met3_eff);
  TLegend *leg2 = new TLegend(.61, .47, 0.91, .17);
  leg2->AddEntry(met1_eff, "E_{T}^{miss} > 30 GeV"); 
  leg2->AddEntry(met2_eff, "E_{T}^{miss} > 50 GeV"); 
  leg2->AddEntry(met3_eff, "E_{T}^{miss} > 120 GeV"); 
  leg2->SetBorderSize(0);//remove border and shadow box.
  leg2->SetTextSize(0.05);
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
  c2->SetRightMargin(0.05);
  s2->SetMinimum(0);
  s2->Draw("nostack");
  s2->GetXaxis()->SetLimits(0.0,200.0);
  leg2->Draw();
  line1->Draw();
  line2->Draw();
  line3->Draw();
  s2->SetTitle("CMS Simulation #sqrt{s} = 8 TeV");
  s2->GetYaxis()->SetTitle("Efficiency");
  s2->GetXaxis()->SetTitle("gen E_{T}^{miss} (GeV)");

  TCanvas *c3 = new TCanvas;
  THStack *s3 = new THStack("stack", "Efficiency of nbtags Cut");
  btag_eff->SetMaximum(1);
  btag_eff->SetLineColor(kBlue);
  btag_eff->SetLineWidth(5);
  btag_eff->SetMarkerColor(kBlue);
  btag_eff->SetLineWidth(5);
  s3->Add(btag_eff);
  s3->SetMinimum(0);
//  s3->GetXaxis()->SetLimits(0.0, 400.0);
  s3->Draw("nostack");
  s3->SetTitle("CMS Simulation #sqrt{s} = 8 TeV");
  s3->GetYaxis()->SetTitle("b-tagging Efficiency");
  s3->GetXaxis()->SetTitle("gen jet p_{T} (GeV)");

  TCanvas *c4 = new TCanvas;
  THStack *s4 = new THStack("stack", "Efficiency of nbtags Cut");
  btag2_eff->SetMaximum(1);
  btag2_eff->SetLineColor(kBlue);
  btag2_eff->SetLineWidth(5);
  btag2_eff->SetMarkerColor(kBlue);
  btag2_eff->SetLineWidth(2);
  s4->Add(btag2_eff);
  s4->SetMinimum(0);
  s4->Draw("nostack");
  s4->SetTitle("Efficiency of nBtags Cut (partons)");
  s4->GetYaxis()->SetTitle("Efficiency");
  s4->GetXaxis()->SetTitle("b-flavor matched jet pT^{gen} (GeV)");

  TCanvas *c6 = new TCanvas;
  THStack *s6 = new THStack("stack", "Efficiency of jet Reco");
  njets_eff->SetMaximum(1);
  njets_eff->SetLineColor(kBlue);
  njets_eff->SetMarkerColor(kBlue);
  njets_eff->SetLineWidth(2);
  TLine *line8 = new TLine(40,0,40,1.05);
  line8->SetLineColor(15);
  line8->SetLineStyle(3);
  s6->Add(njets_eff);
  s6->SetMinimum(0);
  s6->Draw("nostack");
  line8->Draw();
  s6->SetTitle("Efficiency of jet Matching (partons)");
  s6->GetYaxis()->SetTitle("Efficiency");
  s6->GetXaxis()->SetTitle("gen jet pT");

  TCanvas *c7 = new TCanvas;
  THStack *s7 = new THStack("stack", "Efficiency of jet Reco");
  njets_eff2->SetMaximum(1);
  njets_eff2->SetLineColor(kBlue);
  njets_eff2->SetMarkerColor(kBlue);
  njets_eff2->SetLineWidth(2);
  TLine *line9 = new TLine(40,0,40,1.05);
  line9->SetLineColor(15);
  line9->SetLineStyle(3);
  s7->Add(njets_eff2);
  s7->SetMinimum(0);
  s7->Draw("nostack");
  s7->GetXaxis()->SetLimits(0.0,100.0);
  line9->Draw();
  s7->SetTitle("CMS Simulation #sqrt{s} = 8 TeV");
  s7->GetYaxis()->SetTitle("Efficiency");
  s7->GetXaxis()->SetTitle("gen jet p_{T} (GeV)");

  TFile *out_file = new TFile("eff_curves.root", "RECREATE");
  out_file->cd();
  gStyle->SetOptStat(0);
  elec_eff->Write();
  muon_eff->Write();
  c0->Write();
  c0->Print("id_iso_fit.pdf");
  c1->Write();
  c1->Print("ht_fit_part.pdf");
  c2->Write();
  c2->Print("met_fit.pdf");
  c3->Write();
  c3->Print("nbtags_fit_jets.pdf");
  c4->Write();
  c4->Print("nbtags_fit_part.pdf");
  c5->Write();
  c5->Print("ht_fit_jets.pdf");
  c6->Write();
  c6->Print("jet_fit_part.pdf");
  c7->Write();
  c7->Print("jet_fit_jets.pdf");
  out_file->Close();
}







