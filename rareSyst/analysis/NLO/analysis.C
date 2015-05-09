#include <iostream> 
#include "TH1F.h"
#include "THStack.h"
#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "SYST.h"
#include "../../../Tools/utils.h"
#include "../../../CORE/SSSelections.h"

using namespace std;

//Switches
bool doOld = true;

int signalRegionAG(int njets, int nbtags, float met, float ht, float mt_min, float lep1_pt, float lep2_pt){
  if (ht != ht) return -1; //this is to protect against NANs
  if (!doOld) return signalRegion(njets, nbtags, met, ht, mt_min, lep1_pt, lep2_pt);
  else {
    if (met < 50 || njets < 2 || lep1_pt < 20 || lep2_pt < 20 || ht < 200) return 0;
    if (met >  50 && met < 120 && ht >= 200 && ht <= 400 && njets >= 2 && njets < 4) return 1;
    if (met >  50 && met < 120 && ht >  400 &&              njets >= 2 && njets < 4) return 2;
    if (met >  50 && met < 120 && ht >= 200 && ht <= 400 && njets >= 4             ) return 3;
    if (met >  50 && met < 120 && ht >  400 &&              njets >= 4             ) return 4;
    if (met > 120 &&              ht >= 200 && ht <= 400 && njets >= 2 && njets < 4) return 5;
    if (met > 120 &&              ht >  400 &&              njets >= 2 && njets < 4) return 6;
    if (met > 120 &&              ht >= 200 && ht <= 400 && njets >= 4             ) return 7;
    if (met > 120 &&              ht >  400 &&              njets >= 4             ) return 8;
  }
  cout << "WARNING!  No old SR found! Should never get here!" << endl;
  return -1;
}

//bool doLatex = 1;
//bool verbose = 0;
//
////Struct
//struct entry {float SR; float value; float stat; float scale_up; float scale_dn; float pdf_up; float pdf_dn; float eff; float eff_stat; float eff_scale_up; float eff_scale_dn; float eff_pdf_up; float eff_pdf_dn;};
//vector <entry> SR_fixed;
//vector <entry> SR_dynam;
//entry cs_fixed;
//entry cs_dynam;
//
////Global Variables
//float nTotal = 0;
//float nTotal_stat = 0;
//float nTotal_scaleup = 0;
//float nTotal_scaledn = 0;
//float pdf_error_fixed_all_up = 0;
//float pdf_error_fixed_all_dn = 0;
//TH1F* total_fixed = new TH1F("total_fixed","total_fixed", 1, 0, 1);
//float nTotal_dynam = 0;
//float nTotal_dynam_stat = 0;
//float nTotal_scaleup_dynam = 0;
//float nTotal_scaledn_dynam = 0;
//float pdf_error_dynam_all_up = 0;
//float pdf_error_dynam_all_dn = 0;
//TH1F* total_dynam = new TH1F("total_dynam","total_dynam", 1, 0, 1);
//
//void calcSR(int which, TCut cuts, TChain* chain, bool isFixed){
//
//  //Locals
//  float nTotal_local;
//  float nTotal_local_scaleup;
//  float nTotal_local_scaledn;
//  float nTotal_local_stat;
//  float pdf_error_local_up;
//  float pdf_error_local_dn;
//  if (isFixed){
//    nTotal_local = nTotal;
//    nTotal_local_stat = nTotal_stat;
//    nTotal_local_scaleup = nTotal_scaleup;
//    nTotal_local_scaledn = nTotal_scaledn;
//    pdf_error_local_up = pdf_error_fixed_all_up;
//    pdf_error_local_dn = pdf_error_fixed_all_dn;
//  }
//  else if (!isFixed){
//    nTotal_local = nTotal_dynam;
//    nTotal_local_stat = nTotal_dynam_stat;
//    nTotal_local_scaleup = nTotal_scaleup_dynam;
//    nTotal_local_scaledn = nTotal_scaledn_dynam;
//    pdf_error_local_up = pdf_error_dynam_all_up;
//    pdf_error_local_dn = pdf_error_dynam_all_dn;
//  }
//
//  //Weights
//  TCut weight_normal("weight");
//  TCut weight_up("weights[4]");
//  TCut weight_dn("weights[8]");
//
//  //Calculate number of passing events
//  TH1F* SR = new TH1F("SR","SR", 1, 0, 1);
//  SR->Sumw2();
//  chain->Draw("0.5>>SR", weight_normal*cuts);
//  float nPassing = SR->Integral();
//  float nPassing_err = SR->GetBinError(1);
//
//  //Calculate scale error on passing events
//  TH1F* SR_scale_up = new TH1F("SR_scale_up","SR_scale_up", 1, 0, 1);
//  SR_scale_up->Sumw2();
//  chain->Draw("0.5>>SR_scale_up", weight_up*cuts);
//  float nPassing_scaleup = SR_scale_up->Integral();
//  TH1F* SR_scale_dn = new TH1F("SR_scale_dn","SR_scale_dn", 1, 0, 1);
//  SR_scale_dn->Sumw2();
//  chain->Draw("0.5>>SR_scale_dn", weight_dn*cuts);
//  float nPassing_scaledn = SR_scale_dn->Integral();
//
//  //Calculate PDF error on passing events
//  float pdf_error_up = 0;
//  float pdf_error_dn = 0;
//  for (int i = 1; i < 21; i++){
//    TH1F* temp1 = new TH1F("temp1", "temp1", 1, 0, 1);
//    TH1F* temp2 = new TH1F("temp2", "temp2", 1, 0, 1);
//    temp1->Sumw2();
//    temp2->Sumw2();
//    TCut tempWeight1 = TCut(Form("weights[%i]", 2*i-1+8));
//    TCut tempWeight2 = TCut(Form("weights[%i]", 2*i+8));
//    chain->Draw("0.5>>temp1", cuts*tempWeight1);
//    chain->Draw("0.5>>temp2", cuts*tempWeight2);
//    float pdf_error_temp_up = max(temp1->Integral() - nPassing, temp2->Integral() - nPassing);
//    float pdf_error_temp_dn = -min(temp1->Integral() - nPassing, temp2->Integral() - nPassing);
//    pdf_error_up += (pdf_error_temp_up > 0) ? pow(pdf_error_temp_up, 2) : 0;
//    pdf_error_dn += (pdf_error_temp_dn > 0) ? pow(pdf_error_temp_dn, 2) : 0;
//    delete temp1;
//    delete temp2;
//  }
//
//  entry temp;
//  temp.SR = which;
//  temp.value = SR->Integral();
//  temp.stat = 100*SR->GetBinError(1)/nPassing;
//  temp.scale_up = 100*(nPassing_scaledn - nPassing)/nPassing;
//  temp.scale_dn = 100*(nPassing_scaleup - nPassing)/nPassing;
//  temp.pdf_up = 100*sqrt(pdf_error_up)/nPassing;
//  temp.pdf_dn = 100*sqrt(pdf_error_dn)/nPassing;
//  temp.eff = nPassing/nTotal_local; 
//  temp.eff_stat = 100*(sqrt( ( (1-2*temp.eff)*pow(nPassing_err, 2) + pow(temp.eff*nTotal_local_stat, 2)) / pow(nTotal_local, 2)))/temp.eff;
//  temp.eff_scale_up = 100*(nPassing_scaledn/nTotal_local_scaledn - temp.eff)/temp.eff;
//  temp.eff_scale_dn = 100*(nPassing_scaleup/nTotal_local_scaleup - temp.eff)/temp.eff;
//  temp.eff_pdf_up = 100*fabs( nPassing/nTotal_local - (nPassing + sqrt(pdf_error_up))/(nTotal_local + sqrt(pdf_error_local_up)) )/(nPassing/nTotal_local);
//  temp.eff_pdf_dn = 100*fabs( nPassing/nTotal_local - (nPassing + sqrt(pdf_error_dn))/(nTotal_local + sqrt(pdf_error_local_dn)) )/(nPassing/nTotal_local);
//
//  if (isFixed) SR_fixed.push_back(temp);
//  else SR_dynam.push_back(temp);
//
//  if (verbose) cout << "SR" << which << " yield is: " << temp.value << " pm " << temp.stat  << "\% pm " << temp.scale_up << "\%/" << temp.scale_dn << "\% pm " << temp.pdf_up << "%" << endl;
//  if (verbose) cout << "SR" << which << " eff is:  " << temp.eff  << " pm " << temp.eff_stat << "\% pm " << temp.eff_scale_up << "\%/" << temp.eff_scale_dn << "\% pm " << temp.eff_pdf_up << "%"  << endl;
//
//  delete SR;
//  delete SR_scale_up;
//  delete SR_scale_dn;
//
//}
//
//void doCrossSection(TChain* chain, bool isFixed){
//
//  //Calculate cross-section
//  TH1F* cs = new TH1F("cs","cs", 1, 0, 1);
//  cs->Sumw2();
//  chain->Draw("0.5>>cs", "weight");
//
//  //Calculate cross-section scale uncertainties
//  TH1F* cs_scaleup     = new TH1F("cs_scaleup","cs_scaleup", 1, 0, 1);
//  cs_scaleup->Sumw2();
//  chain->Draw("0.5>>cs_scaleup", "weights[4]");
//  TH1F* cs_scaledn     = new TH1F("cs_scaledn","cs_scaledn", 1, 0, 1);
//  cs_scaledn->Sumw2();
//  chain->Draw("0.5>>cs_scaledn", "weights[8]");
//
//  //Calculate cross-section PDF uncertainties
//  float cs_pdf_error_up = 0;
//  float cs_pdf_error_dn = 0;
//  TH1F* temp0 = new TH1F("temp0", "temp0", 1, 0, 1);
//  chain->Draw("0.5>>temp0", "weight");
//  for (int i = 1; i < 21; i++){
//    TH1F* temp1 = new TH1F("temp1", "temp1", 1, 0, 1);
//    TH1F* temp2 = new TH1F("temp2", "temp2", 1, 0, 1);
//    temp1->Sumw2();
//    temp2->Sumw2();
//    TCut tempWeight1 = TCut(Form("weights[%i]", 2*i-1+8));
//    TCut tempWeight2 = TCut(Form("weights[%i]", 2*i+8));
//    chain->Draw("0.5>>temp1", tempWeight1);
//    chain->Draw("0.5>>temp2", tempWeight2);
//    float pdf_error_up_sq_temp = max(temp1->Integral() - temp0->Integral(), temp2->Integral() - temp0->Integral());
//    float pdf_error_dn_sq_temp = -min(temp1->Integral() - temp0->Integral(), temp2->Integral() - temp0->Integral());
//    cs_pdf_error_up += (pdf_error_up_sq_temp > 0) ? pow(pdf_error_up_sq_temp, 2) : 0;
//    cs_pdf_error_dn += (pdf_error_dn_sq_temp > 0) ? pow(pdf_error_dn_sq_temp, 2) : 0;
//    delete temp1;
//    delete temp2;
//  }
//
//  //Store results
//  entry temp;
//  temp.value = 1000*cs->Integral()/cs->GetEntries();
//  temp.stat = 100*cs->GetBinError(1)/cs->Integral();
//  temp.scale_up = 100*(cs_scaledn->Integral()-cs->Integral())/cs->Integral();
//  temp.scale_dn = 100*(cs_scaleup->Integral()-cs->Integral())/cs->Integral();
//  temp.pdf_up = 100*sqrt(cs_pdf_error_up)/cs->Integral();
//  temp.pdf_dn = 100*sqrt(cs_pdf_error_dn)/cs->Integral();
//  if (isFixed == true) cs_fixed = temp;
//  else cs_dynam = temp;
//
//  //Display cross-section
//  if (verbose) cout << "cross section: " << 1000*cs->Integral()/cs->GetEntries() << " pm " << 100*cs->GetBinError(1)/cs->Integral() << "\% pm " << 100*(cs_scaleup->Integral()-cs->Integral())/cs->Integral() << "\%/" << 100*(cs_scaledn->Integral()-cs->Integral())/cs->Integral()  << "\% pm " << 100*sqrt(cs_pdf_error_up)/cs->Integral() << "%" << endl;
//
//  delete cs;
//  delete cs_scaleup;
//  delete cs_scaledn;
//  delete temp0;
//}

//int oldAnalysis(){
//
//  //Chains
//  TChain *fixed = new TChain("Events");
//  fixed->Add("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/myTTW_fixed.root");
//  cout << fixed->GetEntries() << endl;
//
//  TChain *fixed_noFilt = new TChain("Events");
//  fixed_noFilt->Add("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/myTTW_fixed_noFilt.root");
//  cout << fixed_noFilt->GetEntries() << endl;
//
//  TChain *notFixed = new TChain("Events");
//  notFixed->Add("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/myTTW_dynam.root");
//  cout << notFixed->GetEntries() << endl;
//
//  TChain *notFixed_noFilt = new TChain("Events");
//  notFixed_noFilt->Add("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/myTTW_dynam_noFilt.root");
//  cout << notFixed_noFilt->GetEntries() << endl;
//
//  //Cuts
//  TCut scale1fb(Form("1000/1000000"));
//  TCut MET_low("met>50 && met<120");
//  TCut MET_high("met>=120");
//  TCut nJets_low("nJets40 == 2 || nJets40 == 3");
//  TCut nJets_high("nJets40 >= 4");
//  TCut HT_low("ht >= 200 && ht <= 400");
//  TCut HT_high("ht > 400");
//  TCut nBtag0("nBtags == 0");
//  TCut nBtag1("nBtags == 1");
//  TCut nBtag2("nBtags >= 2");
//  TCut weight_normal("weight");
//  TCut weight_up("weights[4]");
//  TCut weight_dn("weights[8]");
//  TCut SS("id1*id2 > 0");
//  TCut lumi("10.0");
//  
//  //Calculate cross-section
//  doCrossSection(fixed_noFilt, 1);
//  doCrossSection(notFixed_noFilt, 0);
// 
//  //Calculate total number of events
//  totalEvents(fixed_noFilt, 1);
//  totalEvents(notFixed_noFilt, 0);
//
//  //Calculate yields in each SR
//  calcSR(1, SS*MET_low*nJets_low*HT_low*scale1fb*lumi, fixed, 1);
//  calcSR(2, SS*MET_low*nJets_low*HT_high*scale1fb*lumi, fixed, 1);
//  calcSR(3, SS*MET_low*nJets_high*HT_low*scale1fb*lumi, fixed, 1);
//  calcSR(4, SS*MET_low*nJets_high*HT_high*scale1fb*lumi, fixed, 1);
//  calcSR(5, SS*MET_high*nJets_low*HT_low*scale1fb*lumi, fixed, 1);
//  calcSR(6, SS*MET_high*nJets_low*HT_high*scale1fb*lumi, fixed, 1);
//  calcSR(7, SS*MET_high*nJets_high*HT_low*scale1fb*lumi, fixed, 1);
//  calcSR(8, SS*MET_high*nJets_high*HT_high*scale1fb*lumi, fixed, 1);
//  calcSR(1, SS*MET_low*nJets_low*HT_low*scale1fb*lumi, notFixed, 0);
//  calcSR(2, SS*MET_low*nJets_low*HT_high*scale1fb*lumi, notFixed, 0);
//  calcSR(3, SS*MET_low*nJets_high*HT_low*scale1fb*lumi, notFixed, 0);
//  calcSR(4, SS*MET_low*nJets_high*HT_high*scale1fb*lumi, notFixed, 0);
//  calcSR(5, SS*MET_high*nJets_low*HT_low*scale1fb*lumi, notFixed, 0);
//  calcSR(6, SS*MET_high*nJets_low*HT_high*scale1fb*lumi, notFixed, 0);
//  calcSR(7, SS*MET_high*nJets_high*HT_low*scale1fb*lumi, notFixed, 0);
//  calcSR(8, SS*MET_high*nJets_high*HT_high*scale1fb*lumi, notFixed, 0);
//
//  //Print out results -- latex
//  if (doLatex == false) return 0;
//  cout << "\\documentclass{article}" << endl;
//  cout << "\\begin{document}" << endl;
//  cout << "\\renewcommand{\\arraystretch}{1.5}" << endl;
//  cout << "\\begin{table}" << endl;
//  cout << "\\begin{tabular}{|c|c|c|}" << endl;
//  cout << "\\hline" << endl;
//  cout << " & Fixed Scales & Dynamic Scales\\\\" << endl;
//  cout << "\\hline" << endl;
//  cout << Form("$\\sigma$ & %.2f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%} $ & %.2f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\texrm{ }^{%.2f\\%%}_{%.2f\\%%} $ \\\\", cs_fixed.value, cs_fixed.stat, cs_fixed.scale_up, cs_fixed.scale_dn, cs_fixed.pdf_up, cs_fixed.pdf_dn, cs_dynam.value, cs_dynam.stat, cs_dynam.scale_up, cs_dynam.scale_dn, cs_dynam.pdf_up, cs_dynam.pdf_dn) << endl;
//  cout << "\\hline" << endl;
//  for (int i = 0; i < 8; i++){
//    cout << Form("$\\sigma \\cdot \\varepsilon \\cdot \\textrm{lumi}$, SR%i & %.2f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%} $ & %.2f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%} $\\\\", i+1, SR_fixed[i].value, SR_fixed[i].stat, SR_fixed[i].scale_up, SR_fixed[i].scale_dn, SR_fixed[i].pdf_up, SR_fixed[i].pdf_dn, SR_dynam[i].value, SR_dynam[i].stat, SR_dynam[i].scale_up, SR_dynam[i].scale_dn, SR_dynam[i].pdf_up, SR_dynam[i].pdf_dn) << endl;
//  cout << "\\hline" << endl;
//  }
//  for (int i = 0; i < 8; i++){
//    cout << Form("$\\varepsilon$, SR%i & %.3f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%} $  & %.3f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%}\\\\ $ ", i+1, SR_fixed[i].eff, fabs(SR_fixed[i].eff_stat), max(SR_fixed[i].eff_scale_up, SR_fixed[i].eff_scale_dn), min(SR_fixed[i].eff_scale_dn, SR_fixed[i].eff_scale_up), SR_fixed[i].eff_pdf_up, SR_fixed[i].eff_pdf_dn, SR_dynam[i].eff, fabs(SR_dynam[i].eff_stat), max(SR_dynam[i].eff_scale_up, SR_dynam[i].eff_scale_up), min(SR_dynam[i].eff_scale_dn, SR_dynam[i].eff_scale_up), SR_dynam[i].eff_pdf_up, SR_dynam[i].eff_pdf_dn) << endl;
//  cout << "\\hline" << endl;
//  }
//  cout << "\\end{tabular}" << endl;
//  cout << "\\caption{$\\sigma$ is in fb; lumi = $10.0\\textrm{ fb}^{-1}$; norm = 0.0001 (to normalize weights)}" << endl;
//  cout << "\\end{table}" << endl;
//  cout << "\\end{document}" << endl;
//
//  return 0;
//
//}

void run(TChain* chain){

  //Constants
  float lumi = 10.0;
  float scale1fb = 1000.0/1000000.0; //weight provided must be provided by nEvents (denom).  Numer is to convert to fb.

  //Histograms for cross-section calculation
  TH1F* cs = new TH1F("cs","cs", 1, 0, 1);
  cs->Sumw2();

  //Histograms for cross-section scale errors
  float cs_scale_up = 0;
  float cs_scale_dn = 0;

  //Histograms for total number of events
  TH1F *total = new TH1F("total", "total", 1, 0, 1);
  total->Sumw2();
 
  //Floats for total nEvents scale errors
  float nTotal_scale_up = 0;
  float nTotal_scale_dn = 0;

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //PDF floats for nTotal
  float cs_pdf[40] = { 0 }; 
  float nTotalPDF[40] = { 0 }; 

  //SR Histos
  TH1F* HHSR[(doOld ? 8 : 32)];
  TH1F* HLSR[26];
  TH1F* LLSR[8];
  for (unsigned int i = 0; i < (doOld ? 8 : 32); i++) HHSR[i] = new TH1F(Form("histo3%i", i), "hist", 1, 0, 1);
  for (unsigned int i = 0; i < 26; i++) HLSR[i] = new TH1F(Form("histo4%i", i), "hist", 1, 0, 1);
  for (unsigned int i = 0; i <  8; i++) LLSR[i] = new TH1F(Form("histo5%i", i), "hist", 1, 0, 1);
  for (unsigned int i = 0; i < (doOld ? 8 : 32); i++) HHSR[i]->Sumw2();
  for (unsigned int i = 0; i < 26; i++) HLSR[i]->Sumw2();
  for (unsigned int i = 0; i <  8; i++) LLSR[i]->Sumw2();

  //Scale Histos for SR
  float HHSR_scale_up[(doOld ? 8 : 32)];
  float HLSR_scale_up[26];
  float LLSR_scale_up[8];
  float HHSR_scale_dn[(doOld ? 8 : 32)];
  float HLSR_scale_dn[26];
  float LLSR_scale_dn[8];

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("Events");
    systematics.Init(tree);

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      systematics.GetEntry(event);
      nEventsTotal++;
    
      //Progress
      SYST::progress(nEventsTotal, nEventsChain);

      //Fill c-s histo
      cs->Fill(0.5, syst::weight());  
 
      //Fill c-s scale error histos
      cs_scale_up += syst::weights().at(4);
      cs_scale_dn += syst::weights().at(8);
  
      //Fill c-s PDF error histos
      for (int i = 0; i < 40; i++) cs_pdf[i] += syst::weights().at(i+9);  

      //Fill total nEvents histos
      total->Fill(0.5, syst::weight()*scale1fb*lumi);
      nTotal_scale_up += syst::weights().at(4)*scale1fb*lumi;
      nTotal_scale_dn += syst::weights().at(8)*scale1fb*lumi;
      for (int i = 0; i < 40; i++) nTotalPDF[i] += syst::weights().at(i+9)*scale1fb*lumi;  

      //Reject all events that are not "true SS"
      if (!syst::keep()) continue;
      if (syst::id1()*syst::id2() <= 0) continue;
    
      //Determine SR for the event
      float mt_l1 = MT(syst::lep1().pt(), syst::lep1().phi(), syst::met(), syst::metphi());
      float mt_l2 = MT(syst::lep2().pt(), syst::lep2().phi(), syst::met(), syst::metphi());
      float mt_min = (mt_l1 > mt_l2 ? mt_l2 : mt_l1); 
      int SR = signalRegionAG(syst::nJets40(), syst::nBtags(), syst::met(), syst::ht(), mt_min, syst::lep1().pt(), syst::lep2().pt());
      anal_type_t hyp_type = doOld ? HighHigh : analysisCategory(syst::lep1().pt(), syst::lep2().pt());
      if (SR <= 0) continue;

      //Fill SR histograms -- yields
      if (hyp_type == HighHigh) HHSR[SR-1]->Fill(0.5, syst::weight()*scale1fb*lumi);
      if (hyp_type == HighLow)  HLSR[SR-1]->Fill(0.5, syst::weight()*scale1fb*lumi);
      if (hyp_type == LowLow)   LLSR[SR-1]->Fill(0.5, syst::weight()*scale1fb*lumi);

      //Fill SR histograms -- scale errors
      if (hyp_type == HighHigh) HHSR_scale_up[SR-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == HighLow)  HLSR_scale_up[SR-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == LowLow)   LLSR_scale_up[SR-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == HighHigh) HHSR_scale_dn[SR-1] += syst::weights().at(8)*scale1fb*lumi;
      if (hyp_type == HighLow)  HLSR_scale_dn[SR-1] += syst::weights().at(8)*scale1fb*lumi;
      if (hyp_type == LowLow)   LLSR_scale_dn[SR-1] += syst::weights().at(8)*scale1fb*lumi;

    }//event loop
  }//file loop

  //Calculate c-s PDF errors
  float cs_pdf_up = 0;
  float cs_pdf_dn = 0;
  for (int i = 1; i <= 20; i++){
    float pdf_up_temp = max(cs_pdf[2*i-2]- cs->Integral(), cs_pdf[2*i-1] - cs->Integral());
    float pdf_dn_temp = max(cs->Integral() - cs_pdf[2*i-2], cs->Integral() - cs_pdf[2*i-1]);
    cs_pdf_up += (pdf_up_temp > 0) ? pow(pdf_up_temp, 2) : 0;
    cs_pdf_dn += (pdf_dn_temp > 0) ? pow(pdf_dn_temp, 2) : 0;
  }

  //Print results 
  cout << "Cross-Section: " << 1000*cs->Integral()/cs->GetEntries() << endl;
  cout << "  --> Stat err (%): " << 100*cs->GetBinError(1)/cs->Integral() << endl;
  cout << "  --> Scale up (%): " <<  100*(cs_scale_dn - cs->Integral())/cs->Integral() << endl;
  cout << "  --> Scale dn (%): " << -100*(cs_scale_up - cs->Integral())/cs->Integral() << endl;
  cout << "  --> PDF up (%):   " << 100*sqrt(cs_pdf_up)/cs->Integral() << endl;
  cout << "  --> PDF dn (%):   " << 100*sqrt(cs_pdf_dn)/cs->Integral() << endl;

  //Delete c-s histos
  delete cs; 

  //Calculate nTotal with statistical error
  float nTotal      = total->Integral();
  float nTotal_stat = total->GetBinError(1)/nTotal;

  //Calculate nTotal PDF errors
  float nTotal_pdf_up = 0;
  float nTotal_pdf_dn = 0;
  for (int i = 1; i <= 20; i++){
    float pdf_up_temp = max(nTotalPDF[2*i-2] - nTotal, nTotalPDF[2*i-1] - nTotal);
    float pdf_dn_temp = max(nTotal - nTotalPDF[2*i-2], nTotal - nTotalPDF[2*i-1]);
    nTotal_pdf_up += (pdf_up_temp > 0) ? pow(pdf_up_temp, 2) : 0;
    nTotal_pdf_dn += (pdf_dn_temp > 0) ? pow(pdf_dn_temp, 2) : 0;
  }

  //Print results
  cout << " " << endl;
  cout << "nTotal: " << nTotal << endl;
  cout << "  --> Stat err (%): " <<  100*nTotal_stat                       << endl;
  cout << "  --> Scale up (%): " <<  100*(nTotal_scale_dn - nTotal)/nTotal << endl;
  cout << "  --> Scale dn (%): " << -100*(nTotal_scale_up - nTotal)/nTotal << endl;
  cout << "  --> PDF up (%):   " <<  100*sqrt(nTotal_pdf_up)/nTotal        << endl;
  cout << "  --> PDF dn (%):   " <<  100*sqrt(nTotal_pdf_dn)/nTotal        << endl;

  //Print SR results (HH)
  for (unsigned int i = 0; i < (doOld ? 8 : 32); i++){
    cout << " " << endl;
    cout << "SR " << i+1 << ": " << HHSR[i]->Integral() << endl;
  }

//  calcSR(1, SS*MET_low*nJets_low*HT_low*scale1fb*lumi, fixed, 1);
//  //Calculate number of passing events
//  TH1F* SR = new TH1F("SR","SR", 1, 0, 1);
//  SR->Sumw2();
//  chain->Draw("0.5>>SR", weight_normal*cuts);
//  float nPassing = SR->Integral();
//  float nPassing_err = SR->GetBinError(1);
//
//  //Calculate scale error on passing events
//  TH1F* SR_scale_up = new TH1F("SR_scale_up","SR_scale_up", 1, 0, 1);
//  SR_scale_up->Sumw2();
//  chain->Draw("0.5>>SR_scale_up", weight_up*cuts);
//  float nPassing_scaleup = SR_scale_up->Integral();
//  TH1F* SR_scale_dn = new TH1F("SR_scale_dn","SR_scale_dn", 1, 0, 1);
//  SR_scale_dn->Sumw2();
//  chain->Draw("0.5>>SR_scale_dn", weight_dn*cuts);
//  float nPassing_scaledn = SR_scale_dn->Integral();
//
//  //Calculate PDF error on passing events
//  float pdf_error_up = 0;
//  float pdf_error_dn = 0;
//  for (int i = 1; i < 21; i++){
//    TH1F* temp1 = new TH1F("temp1", "temp1", 1, 0, 1);
//    TH1F* temp2 = new TH1F("temp2", "temp2", 1, 0, 1);
//    temp1->Sumw2();
//    temp2->Sumw2();
//    TCut tempWeight1 = TCut(Form("weights[%i]", 2*i-1+8));
//    TCut tempWeight2 = TCut(Form("weights[%i]", 2*i+8));
//    chain->Draw("0.5>>temp1", cuts*tempWeight1);
//    chain->Draw("0.5>>temp2", cuts*tempWeight2);
//    float pdf_error_temp_up = max(temp1->Integral() - nPassing, temp2->Integral() - nPassing);
//    float pdf_error_temp_dn = -min(temp1->Integral() - nPassing, temp2->Integral() - nPassing);
//    pdf_error_up += (pdf_error_temp_up > 0) ? pow(pdf_error_temp_up, 2) : 0;
//    pdf_error_dn += (pdf_error_temp_dn > 0) ? pow(pdf_error_temp_dn, 2) : 0;
//    delete temp1;
//    delete temp2;
//  }

}

int analysis(){

  //Declare Chains
  TChain *fixed = new TChain("Events");
  TChain *dynam = new TChain("Events");

  //Fill Chains
  string infix = "fall2014";
  string suffix = "_noFilt";
  fixed->Add(Form("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/%s/myTTW_fixed%s.root", doOld ? infix.c_str() : "", doOld ? "" : suffix.c_str()));
  dynam->Add(Form("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/%s/myTTW_dynam%s.root", doOld ? infix.c_str() : "", doOld ? "" : suffix.c_str()));

  //Do everything
  run(fixed);
  //run(dynam);

  return 0;

}
