#include <iostream> 
#include "TH1F.h"
#include "THStack.h"
#include "TChain.h"
#include "TCut.h"

using namespace std;

//Switches
bool doLatex = 1;
bool verbose = 0;

//Struct
struct entry {float SR; float value; float stat; float scale_up; float scale_dn; float pdf_up; float pdf_dn; float eff; float eff_stat; float eff_scale_up; float eff_scale_dn; float eff_pdf_up; float eff_pdf_dn;};
vector <entry> SR_fixed;
vector <entry> SR_dynam;
entry cs_fixed;
entry cs_dynam;

//Global Variables
float nTotal = 0;
float nTotal_stat = 0;
float nTotal_scaleup = 0;
float nTotal_scaledn = 0;
float pdf_error_fixed_all_up = 0;
float pdf_error_fixed_all_dn = 0;
TH1F* total_fixed = new TH1F("total_fixed","total_fixed", 1, 0, 1);
float nTotal_dynam = 0;
float nTotal_dynam_stat = 0;
float nTotal_scaleup_dynam = 0;
float nTotal_scaledn_dynam = 0;
float pdf_error_dynam_all_up = 0;
float pdf_error_dynam_all_dn = 0;
TH1F* total_dynam = new TH1F("total_dynam","total_dynam", 1, 0, 1);

void calcSR(int which, TCut cuts, TChain* chain, bool isFixed){

  //Locals
  float nTotal_local;
  float nTotal_local_scaleup;
  float nTotal_local_scaledn;
  float nTotal_local_stat;
  float pdf_error_local_up;
  float pdf_error_local_dn;
  if (isFixed){
    nTotal_local = nTotal;
    nTotal_local_stat = nTotal_stat;
    nTotal_local_scaleup = nTotal_scaleup;
    nTotal_local_scaledn = nTotal_scaledn;
    pdf_error_local_up = pdf_error_fixed_all_up;
    pdf_error_local_dn = pdf_error_fixed_all_dn;
  }
  else if (!isFixed){
    nTotal_local = nTotal_dynam;
    nTotal_local_stat = nTotal_dynam_stat;
    nTotal_local_scaleup = nTotal_scaleup_dynam;
    nTotal_local_scaledn = nTotal_scaledn_dynam;
    pdf_error_local_up = pdf_error_dynam_all_up;
    pdf_error_local_dn = pdf_error_dynam_all_dn;
  }

  //Weights
  TCut weight_normal("weight");
  TCut weight_up("weights[4]");
  TCut weight_dn("weights[8]");

  //Calculate number of passing events
  TH1F* SR = new TH1F("SR","SR", 1, 0, 1);
  SR->Sumw2();
  chain->Draw("0.5>>SR", weight_normal*cuts);
  float nPassing = SR->Integral();
  float nPassing_err = SR->GetBinError(1);

  //Calculate scale error on passing events
  TH1F* SR_scale_up = new TH1F("SR_scale_up","SR_scale_up", 1, 0, 1);
  SR_scale_up->Sumw2();
  chain->Draw("0.5>>SR_scale_up", weight_up*cuts);
  float nPassing_scaleup = SR_scale_up->Integral();
  TH1F* SR_scale_dn = new TH1F("SR_scale_dn","SR_scale_dn", 1, 0, 1);
  SR_scale_dn->Sumw2();
  chain->Draw("0.5>>SR_scale_dn", weight_dn*cuts);
  float nPassing_scaledn = SR_scale_dn->Integral();

  //Calculate PDF error on passing events
  float pdf_error_up = 0;
  float pdf_error_dn = 0;
  for (int i = 1; i < 21; i++){
    TH1F* temp1 = new TH1F("temp1", "temp1", 1, 0, 1);
    TH1F* temp2 = new TH1F("temp2", "temp2", 1, 0, 1);
    temp1->Sumw2();
    temp2->Sumw2();
    TCut tempWeight1 = TCut(Form("weights[%i]", 2*i-1+8));
    TCut tempWeight2 = TCut(Form("weights[%i]", 2*i+8));
    chain->Draw("0.5>>temp1", cuts*tempWeight1);
    chain->Draw("0.5>>temp2", cuts*tempWeight2);
    float pdf_error_temp_up = max(temp1->Integral() - nPassing, temp2->Integral() - nPassing);
    float pdf_error_temp_dn = -min(temp1->Integral() - nPassing, temp2->Integral() - nPassing);
    pdf_error_up += (pdf_error_temp_up > 0) ? pow(pdf_error_temp_up, 2) : 0;
    pdf_error_dn += (pdf_error_temp_dn > 0) ? pow(pdf_error_temp_dn, 2) : 0;
    delete temp1;
    delete temp2;
  }

  entry temp;
  temp.SR = which;
  temp.value = SR->Integral();
  temp.stat = 100*SR->GetBinError(1)/nPassing;
  temp.scale_up = 100*(nPassing_scaledn - nPassing)/nPassing;
  temp.scale_dn = 100*(nPassing_scaleup - nPassing)/nPassing;
  temp.pdf_up = 100*sqrt(pdf_error_up)/nPassing;
  temp.pdf_dn = 100*sqrt(pdf_error_dn)/nPassing;
  temp.eff = nPassing/nTotal_local; 
  temp.eff_stat = 100*(sqrt( ( (1-2*temp.eff)*pow(nPassing_err, 2) + pow(temp.eff*nTotal_local_stat, 2)) / pow(nTotal_local, 2)))/temp.eff;
  temp.eff_scale_up = 100*(nPassing_scaledn/nTotal_local_scaledn - temp.eff)/temp.eff;
  temp.eff_scale_dn = 100*(nPassing_scaleup/nTotal_local_scaleup - temp.eff)/temp.eff;
  temp.eff_pdf_up = 100*fabs( nPassing/nTotal_local - (nPassing + sqrt(pdf_error_up))/(nTotal_local + sqrt(pdf_error_local_up)) )/(nPassing/nTotal_local);
  temp.eff_pdf_dn = 100*fabs( nPassing/nTotal_local - (nPassing + sqrt(pdf_error_dn))/(nTotal_local + sqrt(pdf_error_local_dn)) )/(nPassing/nTotal_local);

  if (isFixed) SR_fixed.push_back(temp);
  else SR_dynam.push_back(temp);

  if (verbose) cout << "SR" << which << " yield is: " << temp.value << " pm " << temp.stat  << "\% pm " << temp.scale_up << "\%/" << temp.scale_dn << "\% pm " << temp.pdf_up << "%" << endl;
  if (verbose) cout << "SR" << which << " eff is:  " << temp.eff  << " pm " << temp.eff_stat << "\% pm " << temp.eff_scale_up << "\%/" << temp.eff_scale_dn << "\% pm " << temp.eff_pdf_up << "%"  << endl;

  delete SR;
  delete SR_scale_up;
  delete SR_scale_dn;

}

void doCrossSection(TChain* chain, bool isFixed){

  //Calculate cross-section
  TH1F* cs = new TH1F("cs","cs", 1, 0, 1);
  cs->Sumw2();
  chain->Draw("0.5>>cs", "weight");

  //Calculate cross-section scale uncertainties
  TH1F* cs_scaleup     = new TH1F("cs_scaleup","cs_scaleup", 1, 0, 1);
  cs_scaleup->Sumw2();
  chain->Draw("0.5>>cs_scaleup", "weights[4]");
  TH1F* cs_scaledn     = new TH1F("cs_scaledn","cs_scaledn", 1, 0, 1);
  cs_scaledn->Sumw2();
  chain->Draw("0.5>>cs_scaledn", "weights[8]");

  //Calculate cross-section PDF uncertainties
  float cs_pdf_error_up = 0;
  float cs_pdf_error_dn = 0;
  TH1F* temp0 = new TH1F("temp0", "temp0", 1, 0, 1);
  chain->Draw("0.5>>temp0", "weight");
  for (int i = 1; i < 21; i++){
    TH1F* temp1 = new TH1F("temp1", "temp1", 1, 0, 1);
    TH1F* temp2 = new TH1F("temp2", "temp2", 1, 0, 1);
    temp1->Sumw2();
    temp2->Sumw2();
    TCut tempWeight1 = TCut(Form("weights[%i]", 2*i-1+8));
    TCut tempWeight2 = TCut(Form("weights[%i]", 2*i+8));
    chain->Draw("0.5>>temp1", tempWeight1);
    chain->Draw("0.5>>temp2", tempWeight2);
    float pdf_error_up_sq_temp = max(temp1->Integral() - temp0->Integral(), temp2->Integral() - temp0->Integral());
    float pdf_error_dn_sq_temp = -min(temp1->Integral() - temp0->Integral(), temp2->Integral() - temp0->Integral());
    cs_pdf_error_up += (pdf_error_up_sq_temp > 0) ? pow(pdf_error_up_sq_temp, 2) : 0;
    cs_pdf_error_dn += (pdf_error_dn_sq_temp > 0) ? pow(pdf_error_dn_sq_temp, 2) : 0;
    delete temp1;
    delete temp2;
  }

  //Store results
  entry temp;
  temp.value = 1000*cs->Integral()/cs->GetEntries();
  temp.stat = 100*cs->GetBinError(1)/cs->Integral();
  temp.scale_up = 100*(cs_scaledn->Integral()-cs->Integral())/cs->Integral();
  temp.scale_dn = 100*(cs_scaleup->Integral()-cs->Integral())/cs->Integral();
  temp.pdf_up = 100*sqrt(cs_pdf_error_up)/cs->Integral();
  temp.pdf_dn = 100*sqrt(cs_pdf_error_dn)/cs->Integral();
  if (isFixed == true) cs_fixed = temp;
  else cs_dynam = temp;

  //Display cross-section
  if (verbose) cout << "cross section: " << 1000*cs->Integral()/cs->GetEntries() << " pm " << 100*cs->GetBinError(1)/cs->Integral() << "\% pm " << 100*(cs_scaleup->Integral()-cs->Integral())/cs->Integral() << "\%/" << 100*(cs_scaledn->Integral()-cs->Integral())/cs->Integral()  << "\% pm " << 100*sqrt(cs_pdf_error_up)/cs->Integral() << "%" << endl;

  delete cs;
  delete cs_scaleup;
  delete cs_scaledn;
  delete temp0;
}

void totalEvents(TChain* chain, bool isFixed){

  //Cuts
  TCut lumi("10.0");
  TCut scale1fb(Form("1000/1000000"));
  TCut weight_normal("weight");
  TCut weight_up("weights[4]");
  TCut weight_dn("weights[8]");

  //Calculate number of total events
  if (isFixed){
    total_fixed->Sumw2();
    chain->Draw("0.5>>total_fixed", weight_normal*scale1fb*lumi);
    nTotal = total_fixed->Integral();
    nTotal_stat = total_fixed->GetBinError(1);
  }
  else{
    total_dynam->Sumw2();
    chain->Draw("0.5>>total_dynam", weight_normal*scale1fb*lumi);
    nTotal_dynam = total_dynam->Integral();
    nTotal_dynam_stat = total_dynam->GetBinError(1);
  }

  //Calculate scale error on all events
  TH1F* total_scale_up = new TH1F("total_scale_up","total_scale_up", 1, 0, 1);
  total_scale_up->Sumw2();
  chain->Draw("0.5>>total_scale_up", weight_up*scale1fb*lumi);
  if (isFixed) nTotal_scaleup = total_scale_up->Integral();
  if (!isFixed) nTotal_scaleup_dynam = total_scale_up->Integral();
  TH1F* total_scale_dn = new TH1F("total_scale_dn","total_scale_dn", 1, 0, 1);
  total_scale_dn->Sumw2();
  chain->Draw("0.5>>total_scale_dn", weight_dn*scale1fb*lumi);
  if (isFixed) nTotal_scaledn = total_scale_dn->Integral();
  if (!isFixed) nTotal_scaledn_dynam = total_scale_dn->Integral();

  //Calculate PDF error on all events
  for (int i = 1; i < 21; i++){
    TH1F* temp1 = new TH1F("temp1", "temp1", 1, 0, 1);
    TH1F* temp2 = new TH1F("temp2", "temp2", 1, 0, 1);
    temp1->Sumw2();
    temp2->Sumw2();
    TCut tempWeight1 = TCut(Form("weights[%i]", 2*i-1+8));
    TCut tempWeight2 = TCut(Form("weights[%i]", 2*i+8));
    chain->Draw("0.5>>temp1", scale1fb*lumi*tempWeight1);
    chain->Draw("0.5>>temp2", scale1fb*lumi*tempWeight2);
    float pdf_error_temp_up = 0;
    float pdf_error_temp_dn = 0;
    if (isFixed) pdf_error_temp_up = max(temp1->Integral() - nTotal, temp2->Integral() - nTotal);
    if (!isFixed) pdf_error_temp_up = max(temp1->Integral() - nTotal_dynam, temp2->Integral() - nTotal_dynam);
    if (isFixed) pdf_error_fixed_all_up += (pdf_error_temp_up > 0) ? pow(pdf_error_temp_up, 2) : 0;
    if (!isFixed) pdf_error_dynam_all_up += (pdf_error_temp_up > 0) ? pow(pdf_error_temp_up, 2) : 0;
    if (isFixed) pdf_error_temp_dn = max(temp1->Integral() - nTotal, temp2->Integral() - nTotal);
    if (!isFixed) pdf_error_temp_dn = max(temp1->Integral() - nTotal_dynam, temp2->Integral() - nTotal_dynam);
    if (isFixed) pdf_error_fixed_all_dn += (pdf_error_temp_dn > 0) ? pow(pdf_error_temp_dn, 2) : 0;
    if (!isFixed) pdf_error_dynam_all_dn += (pdf_error_temp_dn > 0) ? pow(pdf_error_temp_dn, 2) : 0;
    delete temp1;
    delete temp2;
  }

  if (isFixed && verbose) cout << "Tot yield is: " << nTotal << " pm " << 100*total_fixed->GetBinError(1)/total_fixed->Integral() << "\% pm " << 100*(nTotal_scaleup - nTotal)/nTotal << "\%/" << 100*(nTotal_scaledn - nTotal)/nTotal << "\% pm " << 100*sqrt(pdf_error_fixed_all_up)/nTotal << "%" << endl;
  else if (!isFixed && verbose) cout << "Tot yield is: " << nTotal_dynam << " pm " << 100*total_dynam->GetBinError(1)/total_dynam->Integral() << "\% pm " << 100*(nTotal_scaleup_dynam - nTotal_dynam)/nTotal_dynam << "\%/" << 100*(nTotal_scaledn_dynam - nTotal_dynam)/nTotal_dynam << "\% pm " << 100*sqrt(pdf_error_dynam_all_up)/nTotal_dynam << "%" << endl;

  delete total_scale_up;
  delete total_scale_dn;

}

int analysis(){

  //Chains
  TChain *fixed = new TChain("Events");
  fixed->Add("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/myTTW_fixed.root");
  cout << fixed->GetEntries() << endl;

  TChain *fixed_noFilt = new TChain("Events");
  fixed_noFilt->Add("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/myTTW_fixed_noFilt.root");
  cout << fixed_noFilt->GetEntries() << endl;

  TChain *notFixed = new TChain("Events");
  notFixed->Add("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/myTTW_dynam.root");
  cout << notFixed->GetEntries() << endl;

  TChain *notFixed_noFilt = new TChain("Events");
  notFixed_noFilt->Add("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/myTTW_dynam_noFilt.root");
  cout << notFixed_noFilt->GetEntries() << endl;

  //Cuts
  TCut scale1fb(Form("1000/1000000"));
  TCut MET_low("met>50 && met<120");
  TCut MET_high("met>=120");
  TCut nJets_low("nJets40 == 2 || nJets40 == 3");
  TCut nJets_high("nJets40 >= 4");
  TCut HT_low("ht >= 200 && ht <= 400");
  TCut HT_high("ht > 400");
  TCut nBtag0("nBtags == 0");
  TCut nBtag1("nBtags == 1");
  TCut nBtag2("nBtags >= 2");
  TCut weight_normal("weight");
  TCut weight_up("weights[4]");
  TCut weight_dn("weights[8]");
  TCut SS("id1*id2 > 0");
  TCut lumi("10.0");
  
  //Calculate cross-section
  doCrossSection(fixed_noFilt, 1);
  doCrossSection(notFixed_noFilt, 0);
 
  //Calculate total number of events
  totalEvents(fixed_noFilt, 1);
  totalEvents(notFixed_noFilt, 0);

  //Calculate yields in each SR
  calcSR(1, SS*MET_low*nJets_low*HT_low*scale1fb*lumi, fixed, 1);
  calcSR(2, SS*MET_low*nJets_low*HT_high*scale1fb*lumi, fixed, 1);
  calcSR(3, SS*MET_low*nJets_high*HT_low*scale1fb*lumi, fixed, 1);
  calcSR(4, SS*MET_low*nJets_high*HT_high*scale1fb*lumi, fixed, 1);
  calcSR(5, SS*MET_high*nJets_low*HT_low*scale1fb*lumi, fixed, 1);
  calcSR(6, SS*MET_high*nJets_low*HT_high*scale1fb*lumi, fixed, 1);
  calcSR(7, SS*MET_high*nJets_high*HT_low*scale1fb*lumi, fixed, 1);
  calcSR(8, SS*MET_high*nJets_high*HT_high*scale1fb*lumi, fixed, 1);
  calcSR(1, SS*MET_low*nJets_low*HT_low*scale1fb*lumi, notFixed, 0);
  calcSR(2, SS*MET_low*nJets_low*HT_high*scale1fb*lumi, notFixed, 0);
  calcSR(3, SS*MET_low*nJets_high*HT_low*scale1fb*lumi, notFixed, 0);
  calcSR(4, SS*MET_low*nJets_high*HT_high*scale1fb*lumi, notFixed, 0);
  calcSR(5, SS*MET_high*nJets_low*HT_low*scale1fb*lumi, notFixed, 0);
  calcSR(6, SS*MET_high*nJets_low*HT_high*scale1fb*lumi, notFixed, 0);
  calcSR(7, SS*MET_high*nJets_high*HT_low*scale1fb*lumi, notFixed, 0);
  calcSR(8, SS*MET_high*nJets_high*HT_high*scale1fb*lumi, notFixed, 0);

  //Print out results -- latex
  if (doLatex == false) return 0;
  cout << "\\documentclass{article}" << endl;
  cout << "\\begin{document}" << endl;
  cout << "\\renewcommand{\\arraystretch}{1.5}" << endl;
  cout << "\\begin{table}" << endl;
  cout << "\\begin{tabular}{|c|c|c|}" << endl;
  cout << "\\hline" << endl;
  cout << " & Fixed Scales & Dynamic Scales\\\\" << endl;
  cout << "\\hline" << endl;
  cout << Form("$\\sigma$ & %.2f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%} $ & %.2f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\texrm{ }^{%.2f\\%%}_{%.2f\\%%} $ \\\\", cs_fixed.value, cs_fixed.stat, cs_fixed.scale_up, cs_fixed.scale_dn, cs_fixed.pdf_up, cs_fixed.pdf_dn, cs_dynam.value, cs_dynam.stat, cs_dynam.scale_up, cs_dynam.scale_dn, cs_dynam.pdf_up, cs_dynam.pdf_dn) << endl;
  cout << "\\hline" << endl;
  for (int i = 0; i < 8; i++){
    cout << Form("$\\sigma \\cdot \\varepsilon \\cdot \\textrm{lumi}$, SR%i & %.2f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%} $ & %.2f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%} $\\\\", i+1, SR_fixed[i].value, SR_fixed[i].stat, SR_fixed[i].scale_up, SR_fixed[i].scale_dn, SR_fixed[i].pdf_up, SR_fixed[i].pdf_dn, SR_dynam[i].value, SR_dynam[i].stat, SR_dynam[i].scale_up, SR_dynam[i].scale_dn, SR_dynam[i].pdf_up, SR_dynam[i].pdf_dn) << endl;
  cout << "\\hline" << endl;
  }
  for (int i = 0; i < 8; i++){
    cout << Form("$\\varepsilon$, SR%i & %.3f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%} $  & %.3f $\\pm$ $%.2f\\%%^{+%.2f\\%%}_{%.2f\\%%}$ $\\pm \\textrm{ }^{%.2f\\%%}_{%.2f\\%%}\\\\ $ ", i+1, SR_fixed[i].eff, fabs(SR_fixed[i].eff_stat), max(SR_fixed[i].eff_scale_up, SR_fixed[i].eff_scale_dn), min(SR_fixed[i].eff_scale_dn, SR_fixed[i].eff_scale_up), SR_fixed[i].eff_pdf_up, SR_fixed[i].eff_pdf_dn, SR_dynam[i].eff, fabs(SR_dynam[i].eff_stat), max(SR_dynam[i].eff_scale_up, SR_dynam[i].eff_scale_up), min(SR_dynam[i].eff_scale_dn, SR_dynam[i].eff_scale_up), SR_dynam[i].eff_pdf_up, SR_dynam[i].eff_pdf_dn) << endl;
  cout << "\\hline" << endl;
  }
  cout << "\\end{tabular}" << endl;
  cout << "\\caption{$\\sigma$ is in fb; lumi = $10.0\\textrm{ fb}^{-1}$; norm = 0.0001 (to normalize weights)}" << endl;
  cout << "\\end{table}" << endl;
  cout << "\\end{document}" << endl;


  return 0;


}
