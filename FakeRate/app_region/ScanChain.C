#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <map>
#include <fstream>
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TH2D.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TString.h"
#include "../../CORE/SSSelections.h"
#include "../../CORE/Tools/utils.h"
#include "../../CORE/Tools/dorky/dorky.cc"
#include "../../classFiles/v4.00/SS.h"
#include "../../software/tableMaker/CTable.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../commonUtils.h"

using namespace std;
using namespace duplicate_removal;

CTable electrons;
CTable muons;

TH1D * evtCounter = new TH1D("","",1000,0,1000); 
map<TString, int> evtBinMap;
int evtBin = 0;
void initCounter() {
    evtCounter = new TH1D("","",1000,0,1000); 
    evtCounter->Sumw2();
    evtBinMap.clear();
}
void addToCounter(TString name, double weight=1.0) {
    if(evtBinMap.find(name) == evtBinMap.end() ) {
        evtBinMap[name] = evtBin;
        evtBin++;
    }
    evtCounter->Fill(evtBinMap[name], weight);
}
void printCounter(bool file = false) {
    ofstream outfile;
    if(file) outfile.open("counter.txt");
    cout << string(30, '-') << endl << "Counter totals: " << endl;
    for(map<TString,int>::iterator it = evtBinMap.begin(); it != evtBinMap.end(); it++) {
        int iBin = (it->second)+1;
        printf("%-15s %6.2f %6.2f\n",
                (it->first).Data(),
                evtCounter->GetBinContent(iBin),
                evtCounter->GetBinError(iBin) );
        if(file) outfile << (it->first).Data() << "  " << evtCounter->GetBinContent(iBin) << "  " << evtCounter->GetBinError(iBin) << endl;
    }
    cout << string(30, '-') << endl;
    if(file) outfile.close();
    if(file) cout << "Wrote counter to counter.txt" << endl;
}

bool isFakeLeg(int lep){
  if (lep == 1) return (ss::lep1_motherID() <= 0);
  if (lep == 2) return (ss::lep2_motherID() <= 0);
  return 0;
}

bool isGoodLeg(int lep){
  if (lep == 1) return (ss::lep1_motherID() == 1);
  if (lep == 2) return (ss::lep2_motherID() == 1);
  return 0;
}

float computePtRel(LorentzVector lepp4, LorentzVector jetp4, bool subtractLep){
  if (jetp4.pt()==0) return 0.;
  if (subtractLep) jetp4-=lepp4;
  float dot = lepp4.Vect().Dot( jetp4.Vect() );
  float ptrel = lepp4.P2() - dot*dot/jetp4.P2();
  ptrel = ptrel>0 ? sqrt(ptrel) : 0.0;
  return ptrel;
}

int number = 0;

void DrawPlots(TH1F *pred, TH1F *obs, TH2D **pred_err2_mu, TH2D **pred_err2_el, TH2D *rate_histo_mu,  TH2D *rate_histo_e, TCanvas *c, TPad *pad_h, TPad *pad_r, TLegend *leg){


  c->cd();
  pred->GetXaxis()->SetTitle("Signal Region"); 
  pred->GetYaxis()->SetTitle("Events");
  obs->GetXaxis()->SetTitle("Signal Region"); 
  obs->GetYaxis()->SetTitle("Events");
  obs->SetLineColor(kRed);

  pred->GetYaxis()->SetRangeUser(0,1.1*std::max(pred->GetMaximum(),obs->GetMaximum()));

  if (TString(pred->GetName()).Contains("HT")) {
    pred->GetXaxis()->SetTitle("HT [GeV]"); 
    obs->GetXaxis()->SetTitle("HT [GeV]"); 
  }
  if (TString(pred->GetName()).Contains("MET")) {
    pred->GetXaxis()->SetTitle("MET [GeV]"); 
    obs->GetXaxis()->SetTitle("MET [GeV]"); 
  }
  if (TString(pred->GetName()).Contains("MTMIN")) {
    pred->GetXaxis()->SetTitle("MTMIN [GeV]"); 
    obs->GetXaxis()->SetTitle("MTMIN [GeV]"); 
  }
  if (TString(pred->GetName()).Contains("L1PT")) {
    pred->GetXaxis()->SetTitle("L1PT [GeV]"); 
    obs->GetXaxis()->SetTitle("L1PT [GeV]"); 
  }
  if (TString(pred->GetName()).Contains("L2PT")) {
    pred->GetXaxis()->SetTitle("L2PT [GeV]"); 
    obs->GetXaxis()->SetTitle("L2PT [GeV]"); 
  }
  if (TString(pred->GetName()).Contains("_br_")) {
    pred->GetXaxis()->SetTitle("Baseline Region"); 
    obs->GetXaxis()->SetTitle("Baseline Region"); 
  }

  pred->SetLineWidth(2);
  obs->SetLineWidth(2);

  pad_h->Draw();
  pad_r->Draw();

  //Overlaid histos
  pad_h->cd();
  pred->SetMarkerStyle(kFullCircle);
  obs->SetFillColor(kCyan);
  pred->Draw("PE");
  obs->Draw("samehistE");
  obs->Draw("sameE");
  pred->Draw("samePE");

  //legend
  leg->Clear();
  leg->SetLineColor(kWhite);
  leg->SetTextFont(42); 
  leg->SetTextSize(0.06);
  leg->SetShadowColor(kWhite); 
  leg->SetFillColor(kWhite); 
  TString predname = pred->GetName();
  TString obsname = obs->GetName();
  leg->AddEntry(predname,"Predicted data","p");
  leg->AddEntry(obsname,"Observed MC","f");  
  leg->Draw();

  //ratio histo
  pad_r->cd();
  TH1F *ratio = (TH1F*) pred->Clone("ratio");
  ratio->Divide(ratio, obs);
  ratio->GetYaxis()->SetRangeUser(0,3);
  ratio->GetYaxis()->SetNdivisions(4);
  ratio->GetYaxis()->SetLabelSize(0.12);
  ratio->GetYaxis()->SetTitle("pred/obs");
  ratio->GetYaxis()->SetTitleSize(0.15);
  ratio->GetYaxis()->SetTitleOffset(0.2);
  ratio->GetXaxis()->SetLabelSize(0.);
  ratio->GetXaxis()->SetTitle("");
  ratio->SetTitle("");
  pad_r->SetGridy();
  ratio->Draw();

  bool print = pred->GetNbinsX() < 10;
  if (print) number++;

  int w = 18;
  if (print) cout << setw(5) << "BR" <<  setw(w) << "Pred" << setw(w) << "Obs" << setw(w) << "Pred/Obs" << setw(w) << "(p-o)/p" << endl;

  for (int bin=1;bin<=pred->GetNbinsX();++bin) {
    int sr = bin-1;
    float p = pred->GetBinContent(bin);
    float o = obs->GetBinContent(bin);
    //stat error on nFO's
    float pe = pred->GetBinError(bin);
    float oe = obs->GetBinError(bin);
    //stat error on FR (fixme, needs to added to plots as well!)
    float pefr2 = 0;
    if (pred_err2_mu[sr]!=0) { 
      for (int frbinx=1;frbinx<=rate_histo_mu->GetNbinsX();++frbinx) {
        for (int frbiny=1;frbiny<=rate_histo_mu->GetNbinsY();++frbiny) {
          float fr = rate_histo_mu->GetBinContent(frbinx,frbiny);
          float fre = rate_histo_mu->GetBinError(frbinx,frbiny);
          float tot = pred_err2_mu[sr]->GetBinContent(frbinx,frbiny);
          pefr2 += fre*fre*pow(1-fr,-4)*tot*tot;
        }
      }
    }
    if (pred_err2_el[sr]!=0) { 
      for (int frbinx=1;frbinx<=rate_histo_e->GetNbinsX();++frbinx) {
        for (int frbiny=1;frbiny<=rate_histo_e->GetNbinsY();++frbiny) {
          float fr = rate_histo_e->GetBinContent(frbinx,frbiny);
          float fre = rate_histo_e->GetBinError(frbinx,frbiny);
          float tot = pred_err2_el[sr]->GetBinContent(frbinx,frbiny);
          pefr2 += fre*fre*pow(1-fr,-4)*tot*tot;
        }
      }
    }
    //now combine the two
    pe = sqrt(pe*pe + pefr2);
    pred->SetBinError(bin, pe); //I think it works retroactively

    float ratioe = sqrt((p*p*oe*oe + o*o*pe*pe)/(o*o*o*o)); //error prop
    float laste = sqrt((p*p*oe*oe + o*o*pe*pe)/(p*p*p*p));  //error prop

    if (print) cout << setw(5) << sr <<  setw(w) << Form("%5.2f +/-%5.2f", p, pe) << setw(w) << Form("%5.2f +/-%5.2f", o, oe) << setw(w) << Form("%5.2f +/-%5.2f", (o>0?p/o:99.99),ratioe) << setw(w) << Form("%5.2f +/-%5.2f", (p>0?(p-o)/p:99.99), laste) << endl;
    if (print && number == 2) muons.setCell(Form("%5.2f $\\pm$ %5.2f", p, pe), bin-1, 0); 
    if (print && number == 2) muons.setCell(Form("%5.2f $\\pm$ %5.2f", o, oe), bin-1, 1); 
    if (print && number == 2) muons.setCell(Form("%5.2f $\\pm$ %5.2f", (o>0?p/o:99.99), ratioe), bin-1, 2); 
    if (print && number == 3) electrons.setCell(Form("%5.2f $\\pm$ %5.2f", p, pe), bin-1, 0); 
    if (print && number == 3) electrons.setCell(Form("%5.2f $\\pm$ %5.2f", o, oe), bin-1, 1); 
    if (print && number == 3) electrons.setCell(Form("%5.2f $\\pm$ %5.2f", (o>0?p/o:99.99), ratioe), bin-1, 2); 
 
  }
}

//getPt and getEta need to stay on sync with meas region
float getPt(float pt, bool extrPtRel = false){
  if(pt < 10.)  return 11.;  
  if(!extrPtRel && pt >= 70.) return 69.;
  if(extrPtRel && pt >= 150.) return 149.;
  return pt;
}

float getEta(float eta, float ht, bool extrPtRel = false){
  if (extrPtRel) {
    if(ht >= 800) return 799;
    return ht;
  }
  if(fabs(eta) >= 2.4) return 2.3;
  return fabs(eta);
}

float getFakeRate(TH2D* histo, float pt, float eta, float ht, bool extrPtRel = false){
  int id = 11;
  if (TString(histo->GetTitle()).Contains("_mu")) id = 13;
  return  fakeRate(id, pt, eta, ht); 
  // pt = getPt(pt,extrPtRel);
  // eta = getEta(eta,ht,extrPtRel);
  // return histo->GetBinContent( histo->FindBin(pt, fabs(eta) ));
}

TH1F* histCreator(string str1, string str2, int n1, int n2, int n3){
  TH1F *temp = new TH1F(str1.c_str(), str2.c_str(), n1, n2, n3);   
  return temp;
}

vector <TH1F*> hists; 

int getHist(string name){
  for (unsigned int i = 0; i < hists.size(); i++){
    if (hists[i]->GetName() == name) return i; 
  }
  cout << "getHist failed on: " << name << ".  Be careful of spurious spaces!" << endl;
  return -1;
}

int ScanChain( TChain* chain, TString fakeratefile, TString option = "", TString ptRegion = "HH", bool doData = false, int nEvents = -1){

  initCounter();

  //Make tables
  electrons.setTable() ("Pred", "Obs", "Pred/Obs");
  muons.setTable() ("Pred", "Obs", "Pred/Obs");
  electrons.setPrecision(2);
  muons.setPrecision(2);


  //Parse options
  bool coneCorr = option.Contains("coneCorr") ? true : false;
  bool jetCorr = option.Contains("jetCorr") ? true : false;
  bool usePtRatioCor = option.Contains("PtRatioCor") ? true :false;
  bool doBonly = option.Contains("doBonly") ? true : false;
  bool doConly = option.Contains("doConly") ? true : false;
  bool doLightonly = option.Contains("doLightonly") ? true : false;
  bool inSitu = option.Contains("inSitu") ? true : false;
  bool soup = option.Contains("soup") ? true : false;
  bool PCssZ = option.Contains("PCssZ") ? true : false;
  bool ssZ = (!PCssZ && option.Contains("ssZ")) ? true : false;
  bool PC = (!PCssZ && option.Contains("PC")) ? true : false;
  bool notCC = option.Contains("notCC") ? true : false;
  bool looseEMVA = option.Contains("LooseEMVA") ? true : false;
  bool highhigh = ptRegion.Contains("HH") ? true : false;
  bool highlow = ptRegion.Contains("HL") ? true : false;
  bool lowlow = ptRegion.Contains("LL") ? true : false;

  float luminosity = getLumi();

  //Dir
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  //Histograms
  hists.push_back( histCreator("Npn_histo_br_obs"       , "Observed Prompt-NonPrompt Background"              ,  4, 0,    4) ); 
  hists.push_back( histCreator("Npn_histo_br_pred"      , "Predicted Prompt-NonPrompt Background"             ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_br_obs_mu"    , "Observed Prompt-NonPrompt Background (Single mu)"  ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_br_pred_mu"   , "Predicted Prompt-NonPrompt Background (Single mu)" ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_br_obs_el"    , "Observed Prompt-NonPrompt Background (Single el)"  ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_br_pred_el"   , "Predicted Prompt-NonPrompt Background (Single el)" ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_sr_obs"       , "Observed Prompt-NonPrompt Background"              , 40, 0,   40) );
  hists.push_back( histCreator("Npn_histo_sr_pred"      , "Predicted Prompt-NonPrompt Background"             , 40, 0,   40) );
  hists.push_back( histCreator("Npn_histo_sr_obs_mu"    , "Observed Prompt-NonPrompt Background (Single mu)"  , 40, 0,   40) );
  hists.push_back( histCreator("Npn_histo_sr_pred_mu"   , "Predicted Prompt-NonPrompt Background (Single mu)" , 40, 0,   40) );
  hists.push_back( histCreator("Npn_histo_sr_obs_el"    , "Observed Prompt-NonPrompt Background (Single el)"  , 40, 0,   40) );
  hists.push_back( histCreator("Npn_histo_sr_pred_el"   , "Predicted Prompt-NonPrompt Background (Single el)" , 40, 0,   40) );
  hists.push_back( histCreator("Npn_histo_HT_obs"       , "Observed Prompt-NonPrompt Background"              , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_pred"      , "Predicted Prompt-NonPrompt Background"             , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_obs_mu"    , "Observed Prompt-NonPrompt Background (Single mu)"  , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_pred_mu"   , "Predicted Prompt-NonPrompt Background (Single mu)" , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_obs_el"    , "Observed Prompt-NonPrompt Background (Single el)"  , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_pred_el"   , "Predicted Prompt-NonPrompt Background (Single el)" , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_MET_obs"      , "Observed Prompt-NonPrompt Background"              , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MET_pred"     , "Predicted Prompt-NonPrompt Background"             , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MET_obs_mu"   , "Observed Prompt-NonPrompt Background (Single mu)"  , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MET_pred_mu"  , "Predicted Prompt-NonPrompt Background (Single mu)" , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MET_obs_el"   , "Observed Prompt-NonPrompt Background (Single el)"  , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MET_pred_el"  , "Predicted Prompt-NonPrompt Background (Single el)" , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MTMIN_obs"    , "Observed Prompt-NonPrompt Background"              , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MTMIN_pred"   , "Predicted Prompt-NonPrompt Background"             , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MTMIN_obs_mu" , "Observed Prompt-NonPrompt Background (Single mu)"  , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MTMIN_pred_mu", "Predicted Prompt-NonPrompt Background (Single mu)" , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MTMIN_obs_el" , "Observed Prompt-NonPrompt Background (Single el)"  , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_MTMIN_pred_el", "Predicted Prompt-NonPrompt Background (Single el)" , 20, 0,  200) );
  hists.push_back( histCreator("Npn_histo_L1PT_obs"     , "Observed Prompt-NonPrompt Background"              , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L1PT_pred"    , "Predicted Prompt-NonPrompt Background"             , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L1PT_obs_mu"  , "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L1PT_pred_mu" , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L1PT_obs_el"  , "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L1PT_pred_el" , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L2PT_obs"     , "Observed Prompt-NonPrompt Background"              , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L2PT_pred"    , "Predicted Prompt-NonPrompt Background"             , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L2PT_obs_mu"  , "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L2PT_pred_mu" , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L2PT_obs_el"  , "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,  150) );
  hists.push_back( histCreator("Npn_histo_L2PT_pred_el" , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,  150) );
  hists.push_back( histCreator("NBs_BR_histo_e"         , "Number of FOs from B's vs Nbtags (els)"            ,  4, 0,    4) );
  hists.push_back( histCreator("NBs_BR_histo_mu"        , "Number of FOs from B's vs Nbtags (muons)"          ,  4, 0,    4) );
  hists.push_back( histCreator("NnotBs_BR_histo_e"      , "Number of FOs NOT from B's vs Nbtags (els)"        ,  4, 0,    4) );
  hists.push_back( histCreator("NnotBs_BR_histo_mu"     , "Number of FOs NOT from B's vs Nbtags (muons)"      ,  4, 0,    4) );
  hists.push_back( histCreator("pTrel_histo_el"         , "pTrel (Electrons)"                                 , 15, 0,   30) );
  hists.push_back( histCreator("pTrel_histo_mu"         , "pTrel (Muons)"                                     , 15, 0,   30) );

  for (unsigned int i = 0; i < hists.size(); i++){
    hists[i]->SetDirectory(rootdir); 
    hists[i]->Sumw2(); 
  }

  //2D histos
  TH2D *pTrelvsIso_histo_mu = new TH2D("pTrelvsIso_histo_mu", "pTrel vs Iso (Muons)", 10, 0., 1., 15, 0., 30.);
  TH2D *pTrelvsIso_histo_el = new TH2D("pTrelvsIso_histo_el", "pTrel vs Iso (Electrons)", 10, 0., 1., 15, 0., 30.);
  TH2D *pTrelvsMiniIso_histo_mu = new TH2D("pTrelvsMiniIso_histo_mu", "pTrel vs MiniIso (Muons)", 10, 0., 1., 15, 0., 30.);
  TH2D *pTrelvsMiniIso_histo_el = new TH2D("pTrelvsMiniIso_histo_el", "pTrel vs MiniIso (Electrons)", 10, 0., 1., 15, 0., 30.);


  //---Load rate histos-----//
  TFile *InputFile = new TFile(fakeratefile,"read");
  cout << "using FR file=" << fakeratefile << endl;

  TH2D *rate_histo_e = 0, *rate_histo_mu = 0;

  if (inSitu){
    rate_histo_e = (TH2D*) InputFile->Get("elec")->Clone("rate_histo_e");
    rate_histo_mu = (TH2D*) InputFile->Get("muon")->Clone("rate_histo_mu");
  }
  else if (coneCorr){  
    rate_histo_e = (TH2D*) InputFile->Get("rate_cone_histo_e")->Clone("rate_cone_histo_e");
    rate_histo_mu = (TH2D*) InputFile->Get("rate_cone_histo_mu")->Clone("rate_cone_histo_mu");
  } 
  else if (jetCorr){ 
    if (highhigh){
      rate_histo_e = (TH2D*) InputFile->Get("rate_jet_highpt_histo_e")->Clone("rate_jet_highpt_histo_e");
      rate_histo_mu = (TH2D*) InputFile->Get("rate_jet_highpt_histo_mu")->Clone("rate_jet_highpt_histo_mu");
    } 
    else if (lowlow){
      rate_histo_e = (TH2D*) InputFile->Get("rate_jet_lowpt_histo_e")->Clone("rate_jet_lowpt_histo_e");
      rate_histo_mu = (TH2D*) InputFile->Get("rate_jet_lowpt_histo_mu")->Clone("rate_jet_lowpt_histo_mu");
    } 
    else {
      rate_histo_e = (TH2D*) InputFile->Get("rate_jet_histo_e")->Clone("rate_jet_histo_e");
      rate_histo_mu = (TH2D*) InputFile->Get("rate_jet_histo_mu")->Clone("rate_jet_histo_mu");
    } 
  } 
  else {  
    rate_histo_e = (TH2D*) InputFile->Get("rate_histo_e")->Clone("rate_histo_e");
    rate_histo_mu = (TH2D*) InputFile->Get("rate_histo_mu")->Clone("rate_histo_mu");
  }

  //Errors
  TH2D *Npn_histo_br_err2_pred_mu[4] = { 0 };
  TH2D *Npn_histo_br_err2_pred_el[4] = { 0 };
  for (int h=0;h<4;++h){
    Npn_histo_br_err2_pred_mu[h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_br_err2_pred_mu_br%i",h));
    Npn_histo_br_err2_pred_mu[h]->Reset();
    Npn_histo_br_err2_pred_mu[h]->SetDirectory(rootdir);
    Npn_histo_br_err2_pred_el[h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_br_err2_pred_el_br%i",h));
    Npn_histo_br_err2_pred_el[h]->Reset();
    Npn_histo_br_err2_pred_el[h]->SetDirectory(rootdir);
  }

  TH2D *Npn_histo_sr_err2_pred_mu[40] = {0};
  TH2D *Npn_histo_sr_err2_pred_el[40] = {0};
  for (int h=0;h<40;++h) {
    Npn_histo_sr_err2_pred_mu[h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_sr_err2_pred_mu_sr%i",h));
    Npn_histo_sr_err2_pred_mu[h]->Reset();
    Npn_histo_sr_err2_pred_mu[h]->SetDirectory(rootdir);
    Npn_histo_sr_err2_pred_el[h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_sr_err2_pred_el_sr%i",h));
    Npn_histo_sr_err2_pred_el[h]->Reset();
    Npn_histo_sr_err2_pred_el[h]->SetDirectory(rootdir);
  }

  TH2D *Npn_histo_HT_err2_pred_mu[50] = {0};
  TH2D *Npn_histo_HT_err2_pred_el[50] = {0};
  for (int h=0;h<50;++h) {
    Npn_histo_HT_err2_pred_mu[h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_HT_err2_pred_mu_HT%i",h));
    Npn_histo_HT_err2_pred_mu[h]->Reset();
    Npn_histo_HT_err2_pred_mu[h]->SetDirectory(rootdir);
    Npn_histo_HT_err2_pred_el[h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_HT_err2_pred_el_HT%i",h));
    Npn_histo_HT_err2_pred_el[h]->Reset();
    Npn_histo_HT_err2_pred_el[h]->SetDirectory(rootdir);
  }

  TH2D *Npn_histo_MET_err2_pred_mu[50] = {0};
  TH2D *Npn_histo_MET_err2_pred_el[50] = {0};
  for (int h=0;h<50;++h) {
    Npn_histo_MET_err2_pred_mu[h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_MET_err2_pred_mu_MET%i",h));
    Npn_histo_MET_err2_pred_mu[h]->Reset();
    Npn_histo_MET_err2_pred_mu[h]->SetDirectory(rootdir);
    Npn_histo_MET_err2_pred_el[h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_MET_err2_pred_el_MET%i",h));
    Npn_histo_MET_err2_pred_el[h]->Reset();
    Npn_histo_MET_err2_pred_el[h]->SetDirectory(rootdir);
  }

  TH2D *Npn_histo_MTMIN_err2_pred_mu[50] = {0};
  TH2D *Npn_histo_MTMIN_err2_pred_el[50] = {0};
  for (int h=0;h<50;++h) {
    Npn_histo_MTMIN_err2_pred_mu[h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_MTMIN_err2_pred_mu_MTMIN%i",h));
    Npn_histo_MTMIN_err2_pred_mu[h]->Reset();
    Npn_histo_MTMIN_err2_pred_mu[h]->SetDirectory(rootdir);
    Npn_histo_MTMIN_err2_pred_el[h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_MTMIN_err2_pred_el_MTMIN%i",h));
    Npn_histo_MTMIN_err2_pred_el[h]->Reset();
    Npn_histo_MTMIN_err2_pred_el[h]->SetDirectory(rootdir);
  }

  TH2D *Npn_histo_L1PT_err2_pred_mu[50] = {0};
  TH2D *Npn_histo_L1PT_err2_pred_el[50] = {0};
  for (int h=0;h<50;++h) {
    Npn_histo_L1PT_err2_pred_mu[h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_L1PT_err2_pred_mu_L1PT%i",h));
    Npn_histo_L1PT_err2_pred_mu[h]->Reset();
    Npn_histo_L1PT_err2_pred_mu[h]->SetDirectory(rootdir);
    Npn_histo_L1PT_err2_pred_el[h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_L1PT_err2_pred_el_L1PT%i",h));
    Npn_histo_L1PT_err2_pred_el[h]->Reset();
    Npn_histo_L1PT_err2_pred_el[h]->SetDirectory(rootdir);
  }

  TH2D *Npn_histo_L2PT_err2_pred_mu[50] = {0};
  TH2D *Npn_histo_L2PT_err2_pred_el[50] = {0};
  for (int h=0;h<50;++h) {
    Npn_histo_L2PT_err2_pred_mu[h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_L2PT_err2_pred_mu_L2PT%i",h));
    Npn_histo_L2PT_err2_pred_mu[h]->Reset();
    Npn_histo_L2PT_err2_pred_mu[h]->SetDirectory(rootdir);
    Npn_histo_L2PT_err2_pred_el[h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_L2PT_err2_pred_el_L2PT%i",h));
    Npn_histo_L2PT_err2_pred_el[h]->Reset();
    Npn_histo_L2PT_err2_pred_el[h]->SetDirectory(rootdir);
  }

  //----------------------
  float prompt2_gen = 0.;     //2 prompt leptons in ss pairs
  float prompt2_reco = 0.;    //2 prompt leptons in ss pairs
  float prompt1_gen = 0.;     //1 prompt leptons in ss pairs
  float prompt1_reco = 0.;    //1 prompt leptons in ss pairs
  float prompt0_gen = 0.;     //0 prompt leptons in ss pairs
  float prompt0_reco = 0.;    //0 prompt leptons in ss pairs
  float sign_misid_gen = 0.;  //number of reco ss that are gen os
  float sign_misid_reco = 0.; // =0
  float Nss_gen=0.;           //number ss events
  float Nss_reco=0.;          //number ss events
  float NpromptL1_reco=0.;    //events where lep1 is prompt
  float NpromptL1_gen=0.;     //events where lep1 is prompt
  float NpromptL2_reco=0.;    //events where lep2 is prompt
  float NpromptL2_gen=0.;     //events where lep2 is prompt
  //----------------------

  //e determination
  float Npn = 0.; //# of prompt-nonprompt tight-tight pairs
  float Npn_s = 0.; //signal contamination for # of prompt-nonprompt tight-tight pairs
  float Nnn = 0.; //# of nonprompt-nonprompt tight-tight pairs
  //float e = 0.;  //rate = Nt/Nl
  float e1 = 0.;  //rate = Nt/Nl
  float e2 = 0.;  //rate = Nt/Nl
  //----------------
  int counter = 0;  
  float Bs_e = 0;
  float notBs_e = 0;
  float Bs_mu = 0;
  float notBs_mu = 0;

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    
    TString filename(currentFile->GetTitle());

    // Get File Content
    TFile *file = new TFile( filename );
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

         if(filename.Contains("TTBAR")) { filename = "TTBAR";  }
    else if(filename.Contains("DY"))    { filename = "DY";     }
    else if(filename.Contains("TTW"))   { filename = "TTW";    }
    else if(filename.Contains("TTZ"))   { filename = "TTZ";    }
    else if(filename.Contains("WJets")) { filename = "WJets";  }
    else if(filename.Contains("WZ3L"))  { filename = "WZ";     }
    else if(filename.Contains("Data"))  { filename = "Data";   }

    // Loop over Events in current file
    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for(unsigned int eventAG = 0; eventAG < nEventsTree; ++eventAG){

      // Get Event Content
      if(nEventsTotal >= nEventsChain) continue;
      samesign.GetEntry(eventAG);
      ++nEventsTotal;

      //debug
      //if (ss::event() != 48767071) continue;

      // Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      if (ss::is_real_data() ) {
          DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
          if(is_duplicate(id)) continue;
      }

      // Analysis Code
      float weight = ss::is_real_data() ? 1.0 : ss::scale1fb()*luminosity*getPUw(ss::nGoodVertices());

      // // ignore MC part of chain when looking at data, except for contamination subtraction
      // if(doData && !ss::is_real_data()) weight = 0; 

      if( !(ss::njets() >= 2 && (ss::ht() > 500 ? 1 : ss::met() > 30) ) ) continue;

      //require triggers
      if (!ss::fired_trigger()) continue;

      if (doBonly) {
        //consider only prompt or bs
        if (ss::lep2_motherID()!=1 && ss::lep2_motherID()!=-1) continue;
        if (ss::lep1_motherID()!=1 && ss::lep1_motherID()!=-1) continue;
      }
      else if (doConly) {
        //consider only prompt or cs
        if (ss::lep2_motherID()!=1 && ss::lep2_motherID()!=-2) continue;
        if (ss::lep1_motherID()!=1 && ss::lep1_motherID()!=-2) continue;
      }
      else if (doLightonly) {
        //consider only prompt or lights
        if (ss::lep2_motherID()!=1 && ss::lep2_motherID()!=0) continue;
        if (ss::lep1_motherID()!=1 && ss::lep1_motherID()!=0) continue;
        //EMEnriched starts at 20 GeV
        if ( (abs(ss::lep1_id())==11 && ss::lep1_motherID()==0 && ss::lep1_p4().pt() < 20) || 
            (abs(ss::lep2_id())==11 && ss::lep2_motherID()==0 && ss::lep2_p4().pt() < 20) ) continue;
      }

      float lep1_ptrel_v1 = ss::lep1_ptrel_v1();
      float lep2_ptrel_v1 = ss::lep2_ptrel_v1();
      assert(fabs(lep1_ptrel_v1 - computePtRel(ss::lep1_p4(),ss::jet_close_lep1(), true))<0.0001);
      assert(fabs(lep2_ptrel_v1 - computePtRel(ss::lep2_p4(),ss::jet_close_lep2(), true))<0.0001);
      float lep1_closejetpt = ss::jet_close_lep1().pt();
      float lep2_closejetpt = ss::jet_close_lep2().pt();

      if (fabs(ss::lep1_ip3d()/ss::lep1_ip3d_err())>4.) continue;
      if (fabs(ss::lep2_ip3d()/ss::lep2_ip3d_err())>4.) continue;

      //Determine lep pT
      float lep1_pT = ss::lep1_p4().pt();
      float lep2_pT = ss::lep2_p4().pt();
      if (coneCorr){
        if (abs(ss::lep1_id())==11){
          if (lep1_ptrel_v1>7.0) lep1_pT = ss::lep1_p4().pt()*(1+std::max(0.,ss::lep1_miniIso()-0.10));
          else lep1_pT = std::max(ss::lep1_p4().pt(),ss::jet_close_lep1().pt()*float(0.70));
        } 
        else {
          if (lep1_ptrel_v1>6.7) lep1_pT = ss::lep1_p4().pt()*(1+std::max(0.,ss::lep1_miniIso()-0.14));
          else lep1_pT = std::max(ss::lep1_p4().pt(),ss::jet_close_lep1().pt()*float(0.68));
        }
        if (abs(ss::lep2_id())==11) {
          if (lep2_ptrel_v1>7.0) lep2_pT = ss::lep2_p4().pt()*(1+std::max(0.,ss::lep2_miniIso()-0.10));
          else lep2_pT = std::max(ss::lep2_p4().pt(),ss::jet_close_lep2().pt()*float(0.70));
        } 
        else {
          if (lep2_ptrel_v1>6.7) lep2_pT = ss::lep2_p4().pt()*(1+std::max(0.,ss::lep2_miniIso()-0.14));
          else lep2_pT = std::max(ss::lep2_p4().pt(),ss::jet_close_lep2().pt()*float(0.68));
        }
      }
      if (jetCorr){
        lep1_pT = ss::jet_close_lep1().pt();
        lep2_pT = ss::jet_close_lep2().pt();
      }

      //Determine passes ID
      bool lep1_passes_id = ss::lep1_passes_id();
      bool lep2_passes_id = ss::lep2_passes_id();

      if (ss::ht_corr()<300.) {
	//Skip if does not pass FO for isolated triggers
	if (passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA())) continue;
	if (passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA())) continue;
      } 

      //Determine mtMin
      float mtmin = ss::mt() > ss::mt_l2() ? ss::mt_l2() : ss::mt();
      if (coneCorr){
        float mtl1 = MT(lep1_pT, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
        float mtl2 = MT(lep2_pT, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
        mtmin = mtl1 > mtl2 ? mtl2 : mtl1;
      }

      //Determine SR and BR
      anal_type_t ac_base = analysisCategory(lep1_pT, lep2_pT);
      int br = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), lep1_pT, lep2_pT); 
      if (br<0) continue;
      int sr = signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, lep1_pT, lep2_pT);

      //lepton pT selection
      if(highhigh && ac_base!=HighHigh) continue;
      if(highlow  && ac_base!=HighLow ) continue;
      if(lowlow   && ac_base!=LowLow  ) continue;

      //pTrel plots
      if(!ss::is_real_data()) {
        if ( (lep1_pT > 25. && lep2_pT > 25.) ){
          if( ss::lep1_id()*ss::lep2_id() > 0 ) {
            if (  (ss::lep1_motherID()<=0 && /*ss::lep1_iso()>0.1 &&*/ fabs(ss::lep1_ip3d()/ss::lep1_ip3d_err())<4. && ss::lep2_motherID()==1)
               || (!lep1_passes_id && fabs(ss::lep1_ip3d()/ss::lep1_ip3d_err())<4. && lep2_passes_id ) ) {
              if (abs(ss::lep1_id())==11){
                pTrelvsIso_histo_el->Fill( std::min(ss::lep1_iso(),float(0.99)), std::min(lep1_ptrel_v1,float(29.9)) );
                pTrelvsMiniIso_histo_el->Fill( std::min(ss::lep1_miniIso(),float(0.99)), std::min(lep1_ptrel_v1,float(29.9)) );
                hists[getHist("pTrel_histo_el")]->Fill(std::min(lep1_ptrel_v1,float(29.9)) );
              } 
              else {
                pTrelvsIso_histo_mu->Fill( std::min(ss::lep1_iso(),float(0.99)), std::min(lep1_ptrel_v1,float(29.9)) );
                pTrelvsMiniIso_histo_mu->Fill( std::min(ss::lep1_miniIso(),float(0.99)), std::min(lep1_ptrel_v1,float(29.9)) );
                hists[getHist("pTrel_histo_mu")]->Fill(std::min(lep1_ptrel_v1,float(29.9)) );
              }
            }
            if (  (ss::lep2_motherID()<=0 && /*ss::lep2_iso()>0.1 &&*/ fabs(ss::lep2_ip3d()/ss::lep2_ip3d_err())<4. && ss::lep1_motherID()==1) 
               || (lep1_passes_id && fabs(ss::lep2_ip3d()/ss::lep2_ip3d_err())<4. && !lep2_passes_id ) ) {
              if (abs(ss::lep2_id())==11) {
                pTrelvsIso_histo_el->Fill( std::min(ss::lep2_iso(),float(0.99)), std::min(lep2_ptrel_v1,float(29.9)) );
                pTrelvsMiniIso_histo_el->Fill( std::min(ss::lep2_miniIso(),float(0.99)), std::min(lep2_ptrel_v1,float(29.9)) );
                hists[getHist("pTrel_histo_el")]->Fill(std::min(lep2_ptrel_v1,float(29.9)) );
              } 
              else {
                pTrelvsIso_histo_mu->Fill( std::min(ss::lep2_iso(),float(0.99)), std::min(lep2_ptrel_v1,float(29.9)) );
                pTrelvsMiniIso_histo_mu->Fill( std::min(ss::lep2_miniIso(),float(0.99)), std::min(lep2_ptrel_v1,float(29.9)) );
                hists[getHist("pTrel_histo_mu")]->Fill(std::min(lep2_ptrel_v1,float(29.9)) );
              }
            }
          }
        }
      }

      //////////////////////////////////////////////////////////////////////////////////////////
      //                         RECO SAME SIGN PAIRS                                         // 
      //////////////////////////////////////////////////////////////////////////////////////////
      // if we're doing data, we want to only fill prompt-nonprompt prediction (class 2) with data
      // and leave all MC in class 3 observations (if they have truth fakes)
      if (!ss::is_real_data() && ss::hyp_class() == 3) {

          bool isLep1Prompt = ss::lep1_motherID()==1;
          bool isLep2Prompt = ss::lep2_motherID()==1;
          bool isLep1NonPrompt = ss::lep1_motherID()<=0;
          bool isLep2NonPrompt = ss::lep2_motherID()<=0;

        //Counters
        counter++;
        Nss_reco = Nss_reco + weight;
        if( isLep1Prompt && isLep2Prompt ){
          prompt2_reco = prompt2_reco + weight;
          NpromptL1_reco = NpromptL1_reco + weight;
          NpromptL2_reco = NpromptL2_reco + weight;
        }
        else if ( ss::lep1_motherID()==2 || ss::lep2_motherID()==2 ) sign_misid_reco += weight; 


        //1) Lep 2 is non-prompt
        else if( isLep1Prompt && isLep2NonPrompt ){ 
          prompt1_reco += weight;  
          NpromptL2_reco += weight;  

          hists[getHist("Npn_histo_sr_obs")   ]->Fill(sr, weight);
          hists[getHist("Npn_histo_br_obs")   ]->Fill(br, weight);
          hists[getHist("Npn_histo_HT_obs")   ]->Fill(ss::ht(), weight);
          hists[getHist("Npn_histo_MET_obs")  ]->Fill(ss::met(), weight);
          hists[getHist("Npn_histo_MTMIN_obs")]->Fill(mtmin, weight);
          hists[getHist("Npn_histo_L1PT_obs") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_L2PT_obs") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);

          if(abs(ss::lep2_id()) == 11){
            hists[getHist("Npn_histo_sr_obs_el")   ]->Fill(sr, weight);
            hists[getHist("Npn_histo_br_obs_el")   ]->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_el")   ]->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_el")  ]->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_el")]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_L1PT_obs_el") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_el") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);

            addToCounter(filename+Form("_obs_el_BR%i", br), weight);
          } 
          else if(abs(ss::lep2_id()) == 13){
            hists[getHist("Npn_histo_sr_obs_mu")   ]->Fill(sr, weight);
            hists[getHist("Npn_histo_br_obs_mu")   ]->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_mu")   ]->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_mu")  ]->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_mu")]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_L1PT_obs_mu") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_mu") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);

            addToCounter(filename+Form("_obs_mu_BR%i", br), weight);
          }
        }

        //2) Lep 1 is non-prompt
        else if( isLep1NonPrompt && isLep2Prompt ){ 
          prompt1_reco += weight;
          NpromptL2_reco += weight;

          hists[getHist("Npn_histo_sr_obs")]   ->Fill(sr, weight);
          hists[getHist("Npn_histo_br_obs")]   ->Fill(br, weight);
          hists[getHist("Npn_histo_HT_obs")]   ->Fill(ss::ht(), weight);
          hists[getHist("Npn_histo_MET_obs")]  ->Fill(ss::met(), weight);
          hists[getHist("Npn_histo_MTMIN_obs")]->Fill(mtmin, weight);
          hists[getHist("Npn_histo_L1PT_obs") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_L2PT_obs") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
          
          if(abs(ss::lep1_id()) == 11){
            hists[getHist("Npn_histo_sr_obs_el")]   ->Fill(sr, weight);
            hists[getHist("Npn_histo_br_obs_el")]   ->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_el")]   ->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_el")]  ->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_el")]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_L1PT_obs_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            
            addToCounter(filename+Form("_obs_el_BR%i", br), weight);

          } 
          else if(abs(ss::lep1_id()) == 13){
            hists[getHist("Npn_histo_sr_obs_mu")]   ->Fill(sr, weight);
            hists[getHist("Npn_histo_br_obs_mu")]   ->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_mu")]   ->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_mu")]  ->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_mu")]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_L1PT_obs_mu")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_mu")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);

            addToCounter(filename+Form("_obs_mu_BR%i", br), weight);
          }
        }

        //Both are non-prompt
        else if( isLep1NonPrompt && isLep2NonPrompt ) prompt0_reco += weight;

        //check for charge misID on gen level.
        if (ss::lep1_motherID()==2 || ss::lep2_motherID()==2) sign_misid_gen += weight;
        else {
          Nss_gen += weight;
          if( ss::lep1_motherID()==1 && ss::lep2_motherID()==1 ){
            prompt2_gen += weight;
            NpromptL1_gen += weight;
            NpromptL2_gen += weight;
          }
          else if( ss::lep1_motherID()==1 && ss::lep2_motherID()<=0 ){
            prompt1_gen += weight;
            NpromptL1_gen += weight;
          }
          else if( ss::lep1_motherID()<=0 && ss::lep2_motherID()==1 ){
            prompt1_gen += weight;
            NpromptL2_gen += weight;
          }
          else if( (ss::lep1_motherID()!=1 && ss::lep2_motherID()!=1) ) prompt0_gen += weight;
        }

      }

      //////////////////////////////////////////////////////////////////////////////////////////
      //                         SINGLE FAKES                                                 // 
      //////////////////////////////////////////////////////////////////////////////////////////

      e1 = 0.; //rate for lep1
      e2 = 0.; //rate for lep2

      //prompt-nonprompt background
      // if we're doing data, we want to only fill prompt-nonprompt prediction (class 2) with data
      // and leave all MC in class 3 observations (if they have truth fakes)
      if( ( doData && (ss::is_real_data() && ss::hyp_class() == 2) ) ||
          (!doData && (ss::hyp_class() == 2) ) ) { 

        int nbjets = ss::nbtags();
        if (nbjets > 3) nbjets = 3; 

        //0) InSituFR variables
        float ptrel_cut_1 = (abs(ss::lep1_id()) == 11 ? 7.0 : 6.7); 
        float ptrel_cut_2 = (abs(ss::lep2_id()) == 11 ? 7.0 : 6.7); 
        float ptratio_cut_1 = (abs(ss::lep1_id()) == 11 ? 0.7 : 0.68); 
        float ptratio_cut_2 = (abs(ss::lep2_id()) == 11 ? 0.7 : 0.68); 
        bool lep1_denom_iso = ((ss::lep1_miniIso() < 0.4) && ((ss::lep1_ptrel_v1() > ptrel_cut_1) || ((ss::lep1_closeJet().pt()/ss::lep1_p4().pt()) < (1/ptratio_cut_1 + ss::lep1_miniIso())))); 
        bool lep2_denom_iso = ((ss::lep2_miniIso() < 0.4) && ((ss::lep2_ptrel_v1() > ptrel_cut_2) || ((ss::lep2_closeJet().pt()/ss::lep2_p4().pt()) < (1/ptratio_cut_2 + ss::lep2_miniIso())))); 
        //1) Lep1 is tight, lep2 is loose!tight
        if (lep1_passes_id && !lep2_passes_id){  


          //Baseline selections
          if (!inSitu && ss::hyp_class() != 2) continue;
          if (inSitu && (ss::lep2_multiIso() || !isFakeLeg(2) || !isGoodLeg(1) || !lep2_denom_iso)) continue;
          if (inSitu && (!ss::passed_id_inSituFR_lep1() || !ss::passed_id_inSituFR_lep2())) continue;

          ////Electron MVA -- enforce tight selections
          //if (abs(ss::lep1_id()) == 11 && fabs(ss::lep1_p4().eta()) < 0.8 && ss::lep1_MVA() < 0.73) continue;
          //if (abs(ss::lep1_id()) == 11 && fabs(ss::lep1_p4().eta()) >= 0.8 && fabs(ss::lep1_p4().eta()) <= 1.479 && ss::lep1_MVA() < 0.57) continue;
          //if (abs(ss::lep1_id()) == 11 && fabs(ss::lep1_p4().eta()) > 1.479 && ss::lep1_MVA() < 0.05) continue; 
          //if (abs(ss::lep2_id()) == 11 && fabs(ss::lep2_p4().eta()) < 0.8 && ss::lep2_MVA() < 0.73) continue;
          //if (abs(ss::lep2_id()) == 11 && fabs(ss::lep2_p4().eta()) >= 0.8 && fabs(ss::lep2_p4().eta()) <= 1.479 && ss::lep2_MVA() < 0.57) continue;
          //if (abs(ss::lep2_id()) == 11 && fabs(ss::lep2_p4().eta()) > 1.479 && ss::lep2_MVA() < 0.05) continue; 
        
          // if(doData && !ss::is_real_data() && isGoodLeg(2)) weight = -ss::scale1fb(); 
          // else {
          //     if(!ss::is_real_data()) weight = 0;
          // }
 
          if (usePtRatioCor){
            //this is a tighter FO than default, so skip if it does not pass
            if ( abs(ss::lep2_id())==11 ){
              float ptratiocor = lep2_closejetpt>0. ? ss::lep2_p4().pt()*(1+std::max(0.,ss::lep2_miniIso()-0.10))/lep2_closejetpt : 1.;
              if (!(ptratiocor > 0.70 || lep2_ptrel_v1 > 7.0)) continue;
            } 
            else {
              float ptratiocor = lep2_closejetpt>0. ? ss::lep2_p4().pt()*(1+std::max(0.,ss::lep2_miniIso()-0.14))/lep2_closejetpt : 1.;
              if (!(ptratiocor > 0.68 || lep2_ptrel_v1 > 6.7)) continue;
            }
          }
          if (highlow && jetCorr){
            if (ss::lep2_p4().pt()>25.){
              rate_histo_e = (TH2D*) InputFile->Get("rate_jet_highpt_histo_e");
              rate_histo_mu = (TH2D*) InputFile->Get("rate_jet_highpt_histo_mu");
            } 
            else {
              rate_histo_e = (TH2D*) InputFile->Get("rate_jet_lowpt_histo_e");
              rate_histo_mu = (TH2D*) InputFile->Get("rate_jet_lowpt_histo_mu");
            }
          }

          if (abs(ss::lep2_id()) == 11){  
            e2 = getFakeRate( rate_histo_e, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false );

            addToCounter(filename+Form("_pred_el_BR%i", br), (e2/(1-e2))*weight);

            hists[getHist("Npn_histo_sr_pred_el")]   ->Fill(sr, (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_br_pred_el")]   ->Fill(br, (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_HT_pred_el")]   ->Fill(ss::ht(), (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_MET_pred_el")]  ->Fill(ss::met(), (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_MTMIN_pred_el")]->Fill(mtmin, (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_L1PT_pred_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_L2PT_pred_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), (e2/(1-e2))*weight);
            if (sr>=0) Npn_histo_sr_err2_pred_el[sr]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_br_err2_pred_el[br]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_HT_err2_pred_el[hists[getHist("Npn_histo_HT_pred_el")]->FindBin(ss::ht())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_MET_err2_pred_el[hists[getHist("Npn_histo_MET_pred_el")]->FindBin(ss::met())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_MTMIN_err2_pred_el[hists[getHist("Npn_histo_MTMIN_pred_el")]->FindBin(mtmin)-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_L1PT_err2_pred_el[hists[getHist("Npn_histo_L1PT_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_L2PT_err2_pred_el[hists[getHist("Npn_histo_L2PT_pred_el")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            // fill el abundance histos here w/ nbtags
            if(ss::lep2_motherID() == -1) hists[getHist("NBs_BR_histo_e")]->Fill(nbjets, weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) hists[getHist("NnotBs_BR_histo_e")]->Fill(nbjets, weight);
            if(ss::lep2_motherID() == -1) Bs_e = Bs_e + weight;
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) notBs_e = notBs_e + weight;
          }
          else if (abs(ss::lep2_id()) == 13){ 
            e2 = getFakeRate( rate_histo_mu, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false );

            addToCounter(filename+Form("_pred_mu_BR%i", br), (e2/(1-e2))*weight);

            hists[getHist("Npn_histo_sr_pred_mu")]->Fill(sr, (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_br_pred_mu")]->Fill(br, (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_HT_pred_mu")]->Fill(ss::ht(), (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_MET_pred_mu")]->Fill(ss::met(), (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_MTMIN_pred_mu")]->Fill(mtmin, (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_L1PT_pred_mu")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), (e2/(1-e2))*weight);
            hists[getHist("Npn_histo_L2PT_pred_mu")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), (e2/(1-e2))*weight);
            if (sr>=0) Npn_histo_sr_err2_pred_mu[sr]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_br_err2_pred_mu[br]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_HT_err2_pred_mu[hists[getHist("Npn_histo_HT_pred_mu")]->FindBin(ss::ht())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_MET_err2_pred_mu[hists[getHist("Npn_histo_MET_pred_mu")]->FindBin(ss::met())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_MTMIN_err2_pred_mu[hists[getHist("Npn_histo_MTMIN_pred_mu")]->FindBin(mtmin)-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_L1PT_err2_pred_mu[hists[getHist("Npn_histo_L1PT_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            Npn_histo_L2PT_err2_pred_mu[hists[getHist("Npn_histo_L2PT_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            if(ss::lep2_motherID() == -1) hists[getHist("NBs_BR_histo_mu")]->Fill(nbjets, weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) hists[getHist("NnotBs_BR_histo_mu")]->Fill(nbjets, weight);
            if(ss::lep2_motherID() == -1) Bs_mu = Bs_mu + weight;
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) notBs_mu = notBs_mu + weight;
          }
          Npn = Npn + (e2/(1-e2))*weight;
          if (ss::lep2_motherID()==1) Npn_s = Npn_s + (e2/(1-e2))*weight;
          hists[getHist("Npn_histo_sr_pred")]->Fill(sr, (e2/(1-e2))*weight);
          hists[getHist("Npn_histo_br_pred")]->Fill(br, (e2/(1-e2))*weight);
          hists[getHist("Npn_histo_HT_pred")]->Fill(ss::ht(), (e2/(1-e2))*weight);
          hists[getHist("Npn_histo_MET_pred")]->Fill(ss::met(), (e2/(1-e2))*weight);
          hists[getHist("Npn_histo_MTMIN_pred")]->Fill(mtmin, (e2/(1-e2))*weight);
          hists[getHist("Npn_histo_L1PT_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), (e2/(1-e2))*weight);
          hists[getHist("Npn_histo_L2PT_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), (e2/(1-e2))*weight);
        }

        //2) Lep1 is loose!tight, lep2 is tight
        else if (!lep1_passes_id && lep2_passes_id){

          //Baseline selections
          if (!inSitu && ss::hyp_class() != 2) continue;
          if (inSitu && (ss::lep1_multiIso() || !isFakeLeg(1) || !isGoodLeg(2) || !lep1_denom_iso)) continue;
          if (inSitu && (!ss::passed_id_inSituFR_lep1() || !ss::passed_id_inSituFR_lep2())) continue;

          // if(doData && !ss::is_real_data() && isGoodLeg(1)) weight = -ss::scale1fb(); 
          // else weight = 0;

          if (usePtRatioCor){
            if ( abs(ss::lep1_id())==11 ){
              float ptratiocor = lep1_closejetpt>0. ? ss::lep1_p4().pt()*(1+std::max(0.,ss::lep1_miniIso()-0.10))/lep1_closejetpt : 1.;
              if (!(ptratiocor > 0.70 || lep1_ptrel_v1 > 7.0)) continue;
            } 
            else {
              float ptratiocor = lep1_closejetpt>0. ? ss::lep1_p4().pt()*(1+std::max(0.,ss::lep1_miniIso()-0.14))/lep1_closejetpt : 1.;
              if (!(ptratiocor > 0.68 || lep1_ptrel_v1 > 6.7)) continue;
            }
          }
          if (highlow && jetCorr){
            if (ss::lep1_p4().pt()>25.){
              rate_histo_e = (TH2D*) InputFile->Get("rate_jet_highpt_histo_e");
              rate_histo_mu = (TH2D*) InputFile->Get("rate_jet_highpt_histo_mu");
            } 
            else {
              rate_histo_e = (TH2D*) InputFile->Get("rate_jet_lowpt_histo_e");
              rate_histo_mu = (TH2D*) InputFile->Get("rate_jet_lowpt_histo_mu");
            }
          }

          if( abs(ss::lep1_id()) == 11 ){	//if el, use el rate.  FILL WITH NONPROMPT			  
            e1 = getFakeRate(rate_histo_e, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false );

            addToCounter(filename+Form("_pred_el_BR%i", br), (e1/(1-e1))*weight);

            hists[getHist("Npn_histo_sr_pred_el")]   ->Fill(sr, (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_br_pred_el")]   ->Fill(br, (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_HT_pred_el")]   ->Fill(ss::ht(), (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_MET_pred_el")]  ->Fill(ss::met(), (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_MTMIN_pred_el")]->Fill(mtmin, (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_L1PT_pred_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_L2PT_pred_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), (e1/(1-e1))*weight);

            if (sr>=0) Npn_histo_sr_err2_pred_el[sr]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_br_err2_pred_el[br]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_HT_err2_pred_el[hists[getHist("Npn_histo_HT_pred_el")]->FindBin(ss::ht())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_MET_err2_pred_el[hists[getHist("Npn_histo_MET_pred_el")]->FindBin(ss::met())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_MTMIN_err2_pred_el[hists[getHist("Npn_histo_MTMIN_pred_el")]->FindBin(mtmin)-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_L1PT_err2_pred_el[hists[getHist("Npn_histo_L1PT_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_L2PT_err2_pred_el[hists[getHist("Npn_histo_L2PT_pred_el")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            // fill el abundance histos here w/ nbtags
            if(ss::lep1_motherID() == -1) hists[getHist("NBs_BR_histo_e")]->Fill(nbjets, weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) hists[getHist("NnotBs_BR_histo_e")]->Fill(nbjets, weight);
            if(ss::lep1_motherID() == -1) Bs_e = Bs_e + weight;
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) notBs_e = notBs_e + weight;
          }
          else if( abs(ss::lep1_id()) == 13 ){ //if mu, use mu rate.  FILL WITH NONPROMPT				  
            e1 = getFakeRate(rate_histo_mu, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false );

            addToCounter(filename+Form("_pred_mu_BR%i", br), (e1/(1-e1))*weight);

            hists[getHist("Npn_histo_sr_pred_mu")]->Fill(sr, (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_br_pred_mu")]->Fill(br, (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_HT_pred_mu")]->Fill(ss::ht(), (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_MET_pred_mu")]->Fill(ss::met(), (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_MTMIN_pred_mu")]->Fill(mtmin, (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_L1PT_pred_mu")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), (e1/(1-e1))*weight);
            hists[getHist("Npn_histo_L2PT_pred_mu")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), (e1/(1-e1))*weight);

            if (sr>=0) Npn_histo_sr_err2_pred_mu[sr]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_br_err2_pred_mu[br]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_HT_err2_pred_mu[hists[getHist("Npn_histo_HT_pred_mu")]->FindBin(ss::ht())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_MET_err2_pred_mu[hists[getHist("Npn_histo_MET_pred_mu")]->FindBin(ss::met())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_MTMIN_err2_pred_mu[hists[getHist("Npn_histo_MTMIN_pred_mu")]->FindBin(mtmin)-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_L1PT_err2_pred_mu[hists[getHist("Npn_histo_L1PT_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            Npn_histo_L2PT_err2_pred_mu[hists[getHist("Npn_histo_L2PT_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            // fill el abundance histos here w/ nbtags
            if(ss::lep1_motherID() == -1) hists[getHist("NBs_BR_histo_mu")]->Fill(nbjets, weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) hists[getHist("NnotBs_BR_histo_mu")]->Fill(nbjets, weight);
            if(ss::lep1_motherID() == -1) Bs_mu = Bs_mu + weight;
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) notBs_mu = notBs_mu + weight;
          }
          Npn = Npn + (e1/(1-e1))*weight;
          if (ss::lep1_motherID()==1) Npn_s = Npn_s + (e1/(1-e1))*weight;
          hists[getHist("Npn_histo_sr_pred")]->Fill(sr, (e1/(1-e1))*weight);
          hists[getHist("Npn_histo_br_pred")]->Fill(br, (e1/(1-e1))*weight);
          hists[getHist("Npn_histo_HT_pred")]->Fill(ss::ht(), (e1/(1-e1))*weight);
          hists[getHist("Npn_histo_MET_pred")]->Fill(ss::met(), (e1/(1-e1))*weight);
          hists[getHist("Npn_histo_MTMIN_pred")]->Fill(mtmin, (e1/(1-e1))*weight);
          hists[getHist("Npn_histo_L1PT_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), (e1/(1-e1))*weight);
          hists[getHist("Npn_histo_L2PT_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), (e1/(1-e1))*weight);
        }
      } //end hyp = 2 if statement
      
      // // resume ignoring MC part of chain when looking at data, except for contamination subtraction
      // if(doData && !ss::is_real_data()) weight = 0; 

      //nonprompt-nonprompt background
      else if(ss::hyp_class() == 1){
        if( ss::lep1_id()*ss::lep2_id() > 0 ){
          if( abs(ss::lep2_id()) == 11 ) e2 = getFakeRate( rate_histo_e, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false );
          else if( abs(ss::lep2_id()) == 13 ) e2 = getFakeRate( rate_histo_mu, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false );
          if( abs(ss::lep1_id()) == 11)      e1 = getFakeRate( rate_histo_e, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false );
          else if( abs(ss::lep1_id()) == 13) e1 = getFakeRate( rate_histo_mu, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false );
          Nnn = Nnn + (e1/(1-e1))*(e2/(1-e2))*weight;
        }
      } //end hyp = 1 if statement

    }//end event loop

    // Clean Up
    delete tree;
    file->Close();
    delete file;

  }//file loop

  cout<<"\n"<<"Hyp_type = 3 (Both leptons pass tight selection)"<<endl;
  cout<<"---------------------------------------------"<<endl;
  cout<<setw(15)<<""<<setw(15)<<"Reco Level"<<setw(15)<<"Gen Level"<<endl;
  cout<<setw(15)<<"ss prompt2"<<setw(15)<<prompt2_reco<<setw(15)<<prompt2_gen<<endl;
  cout<<setw(15)<<"ss prompt1"<<setw(15)<<prompt1_reco<<setw(15)<<prompt1_gen<<endl;
  cout<<setw(15)<<"ss prompt0"<<setw(15)<<prompt0_reco<<setw(15)<<prompt0_gen<<endl;
  cout<<setw(15)<<"charge misID"<<setw(15)<<sign_misid_reco<<setw(15)<<sign_misid_gen<<endl;
  cout<<setw(15)<<"Nss"<<setw(15)<<Nss_reco<<setw(15)<<Nss_gen<<endl;
  cout<<"---------------------------------------------"<<endl;
  cout<<setw(15)<<"Nprompt L1"<<setw(15)<<NpromptL1_reco<<setw(15)<<NpromptL1_gen<<endl;
  cout<<setw(15)<<"Nprompt L2"<<setw(15)<<NpromptL2_reco<<setw(15)<<NpromptL2_gen<<endl;
  cout<<"---------------------------------------------"<<endl;

  cout<<setw(25)<<" "<<setw(10)<<"EST"<<setw(10)<<"OBS"<<setw(10)<<"EST/OBS"<<endl;
  cout<<setw(25)<<"Npn:"<<setw(10)<<Npn<<setw(10)<<prompt1_reco<<setw(10)<<Npn/prompt1_reco<<endl;
  cout<<setw(25)<<"Npn-Npn_s:"<<setw(10)<<Npn-Npn_s<<setw(10)<<prompt1_reco<<setw(10)<<(Npn-Npn_s)/prompt1_reco<<endl;
  cout<<setw(25)<<"Nnn:"<<setw(10)<<Nnn<<setw(10)<<prompt0_reco<<setw(10)<<Nnn/prompt0_reco<<endl;

  cout<<"\nCounter = "<<counter<<endl;
  cout<<"\nAve B abundance (els)= "<<Bs_e/(Bs_e + notBs_e)<<endl;
  cout<<"\nAve B abundance (mus)= "<<Bs_mu/(Bs_mu + notBs_mu)<<endl;

  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("1.3f");

  //Plot fake rate histos
  TCanvas *c1=new TCanvas("c1","Fake Rate vs Pt, eta (electron)",800,800);
  c1->cd();
  rate_histo_e->Draw("colz,texte");
  TCanvas *c2=new TCanvas("c2","Fake Rate vs Pt, eta (muon)",800,800);
  c2->cd();
  rate_histo_mu->Draw("colz,texte");

  //redefine option to save also ptRegion in output files
  option=option+"_"+ptRegion;

  std::string plotdir="plots/";

  //Signal region plots
  TCanvas *c3=new TCanvas("c3","Predicted and Observed Prompt-NonPrompt Background", 800,800);
  TPad *pad_h3 = new TPad("pad_h3","Histo Pad3",0., 0.2, 1., 1.0);
  TPad *pad_r3 = new TPad("pad_r3","Ratio Pad3",0., 0.0, 1., 0.2);
  pad_h3->Draw();
  pad_r3->Draw();
  TLegend *leg3 = new TLegend(0.65, 0.70, 0.85, 0.85); //(0.78, 0.63, 0.87, 0.89)
  cout << "\ndump BR all" << endl;
  DrawPlots(hists[getHist("Npn_histo_br_pred")], hists[getHist("Npn_histo_br_obs")], Npn_histo_br_err2_pred_mu, Npn_histo_br_err2_pred_el, rate_histo_mu, rate_histo_e, c3, pad_h3, pad_r3, leg3);

  TH2D *nullarr[40] = {0};

  TCanvas *c4=new TCanvas("c4","Predicted and Observed Prompt-NonPrompt Background (Single mu)", 800,800);
  TPad *pad_h4 = new TPad("pad_h4","Histo Pad4",0., 0.2, 1., 1.0);
  TPad *pad_r4 = new TPad("pad_r4","Ratio Pad4",0., 0.0, 1., 0.2);
  pad_h4->Draw();
  pad_r4->Draw();
  TLegend *leg4 = new TLegend(0.65, 0.70, 0.85, 0.85); //(0.78, 0.63, 0.87, 0.89)
  cout << "\ndump BR mu" << endl;
  DrawPlots(hists[getHist("Npn_histo_br_pred_mu")], hists[getHist("Npn_histo_br_obs_mu")], Npn_histo_br_err2_pred_mu, nullarr, rate_histo_mu, rate_histo_e, c4, pad_h4, pad_r4, leg4);

  TCanvas *c5=new TCanvas("c5","Predicted and Observed Prompt-NonPrompt Background (Single el)", 800,800);
  TPad *pad_h5 = new TPad("pad_h5","Histo Pad5",0., 0.2, 1., 1.0);
  TPad *pad_r5 = new TPad("pad_r5","Ratio Pad5",0., 0.0, 1., 0.2);
  pad_h5->Draw();
  pad_r5->Draw();
  TLegend *leg5 = new TLegend(0.65, 0.70, 0.85, 0.85); //(0.78, 0.63, 0.87, 0.89)
  cout << "\ndump BR ele" << endl;
  DrawPlots(hists[getHist("Npn_histo_br_pred_el")], hists[getHist("Npn_histo_br_obs_el")], nullarr, Npn_histo_br_err2_pred_el, rate_histo_mu, rate_histo_e, c5, pad_h5, pad_r5, leg5);
  c3->SaveAs(plotdir+"br_all"+option+".png");
  c4->SaveAs(plotdir+"br_mu"+option+".png");
  c5->SaveAs(plotdir+"br_el"+option+".png");

  DrawPlots(hists[getHist("Npn_histo_sr_pred")],    hists[getHist("Npn_histo_sr_obs")], Npn_histo_sr_err2_pred_mu, Npn_histo_sr_err2_pred_el, rate_histo_mu, rate_histo_e, c3, pad_h3, pad_r3, leg3);
  DrawPlots(hists[getHist("Npn_histo_sr_pred_mu")], hists[getHist("Npn_histo_sr_obs_mu")], Npn_histo_sr_err2_pred_mu, nullarr, rate_histo_mu, rate_histo_e, c4, pad_h4, pad_r4, leg4);
  DrawPlots(hists[getHist("Npn_histo_sr_pred_el")], hists[getHist("Npn_histo_sr_obs_el")], nullarr, Npn_histo_sr_err2_pred_el, rate_histo_mu, rate_histo_e, c5, pad_h5, pad_r5, leg5);
  c3->SaveAs(plotdir+"sr_all"+option+".png");
  c4->SaveAs(plotdir+"sr_mu"+option+".png");
  c5->SaveAs(plotdir+"sr_el"+option+".png");

  DrawPlots(hists[getHist("Npn_histo_HT_pred")], hists[getHist("Npn_histo_HT_obs")], Npn_histo_HT_err2_pred_mu, Npn_histo_HT_err2_pred_el, rate_histo_mu, rate_histo_e, c3, pad_h3, pad_r3, leg3);
  DrawPlots(hists[getHist("Npn_histo_HT_pred_mu")], hists[getHist("Npn_histo_HT_obs_mu")], Npn_histo_HT_err2_pred_mu, nullarr, rate_histo_mu, rate_histo_e, c4, pad_h4, pad_r4, leg4);
  DrawPlots(hists[getHist("Npn_histo_HT_pred_el")], hists[getHist("Npn_histo_HT_obs_el")], nullarr, Npn_histo_HT_err2_pred_el, rate_histo_mu, rate_histo_e, c5, pad_h5, pad_r5, leg5);
  c3->SaveAs(plotdir+"HT_all"+option+".png");
  c4->SaveAs(plotdir+"HT_mu"+option+".png");
  c5->SaveAs(plotdir+"HT_el"+option+".png");

  DrawPlots(hists[getHist("Npn_histo_MET_pred")], hists[getHist("Npn_histo_MET_obs")], Npn_histo_MET_err2_pred_mu, Npn_histo_MET_err2_pred_el, rate_histo_mu, rate_histo_e, c3, pad_h3, pad_r3, leg3);
  DrawPlots(hists[getHist("Npn_histo_MET_pred_mu")], hists[getHist("Npn_histo_MET_obs_mu")], Npn_histo_MET_err2_pred_mu, nullarr, rate_histo_mu, rate_histo_e, c4, pad_h4, pad_r4, leg4);
  DrawPlots(hists[getHist("Npn_histo_MET_pred_el")], hists[getHist("Npn_histo_MET_obs_el")], nullarr, Npn_histo_MET_err2_pred_el, rate_histo_mu, rate_histo_e, c5, pad_h5, pad_r5, leg5);
  c3->SaveAs(plotdir+"MET_all"+option+".png");
  c4->SaveAs(plotdir+"MET_mu"+option+".png");
  c5->SaveAs(plotdir+"MET_el"+option+".png");

  DrawPlots(hists[getHist("Npn_histo_MTMIN_pred")],    hists[getHist("Npn_histo_MTMIN_obs")], Npn_histo_MTMIN_err2_pred_mu, Npn_histo_MTMIN_err2_pred_el, rate_histo_mu, rate_histo_e, c3, pad_h3, pad_r3, leg3);
  DrawPlots(hists[getHist("Npn_histo_MTMIN_pred_mu")], hists[getHist("Npn_histo_MTMIN_obs_mu")], Npn_histo_MTMIN_err2_pred_mu, nullarr, rate_histo_mu, rate_histo_e, c4, pad_h4, pad_r4, leg4);
  DrawPlots(hists[getHist("Npn_histo_MTMIN_pred_el")], hists[getHist("Npn_histo_MTMIN_obs_el")], nullarr, Npn_histo_MTMIN_err2_pred_el, rate_histo_mu, rate_histo_e, c5, pad_h5, pad_r5, leg5);
  c3->SaveAs(plotdir+"MTMIN_all"+option+".png");
  c4->SaveAs(plotdir+"MTMIN_mu"+option+".png");
  c5->SaveAs(plotdir+"MTMIN_el"+option+".png");

  DrawPlots(hists[getHist("Npn_histo_L1PT_pred")],    hists[getHist("Npn_histo_L1PT_obs")], Npn_histo_L1PT_err2_pred_mu, Npn_histo_L1PT_err2_pred_el, rate_histo_mu, rate_histo_e, c3, pad_h3, pad_r3, leg3);
  DrawPlots(hists[getHist("Npn_histo_L1PT_pred_mu")], hists[getHist("Npn_histo_L1PT_obs_mu")], Npn_histo_L1PT_err2_pred_mu, nullarr, rate_histo_mu, rate_histo_e, c4, pad_h4, pad_r4, leg4);
  DrawPlots(hists[getHist("Npn_histo_L1PT_pred_el")], hists[getHist("Npn_histo_L1PT_obs_el")], nullarr, Npn_histo_L1PT_err2_pred_el, rate_histo_mu, rate_histo_e, c5, pad_h5, pad_r5, leg5);
  c3->SaveAs(plotdir+"L1PT_all"+option+".png");
  c4->SaveAs(plotdir+"L1PT_mu"+option+".png");
  c5->SaveAs(plotdir+"L1PT_el"+option+".png");

  DrawPlots(hists[getHist("Npn_histo_L2PT_pred")],    hists[getHist("Npn_histo_L2PT_obs")], Npn_histo_L2PT_err2_pred_mu, Npn_histo_L2PT_err2_pred_el, rate_histo_mu, rate_histo_e, c3, pad_h3, pad_r3, leg3);
  DrawPlots(hists[getHist("Npn_histo_L2PT_pred_mu")], hists[getHist("Npn_histo_L2PT_obs_mu")], Npn_histo_L2PT_err2_pred_mu, nullarr, rate_histo_mu, rate_histo_e, c4, pad_h4, pad_r4, leg4);
  DrawPlots(hists[getHist("Npn_histo_L2PT_pred_el")], hists[getHist("Npn_histo_L2PT_obs_el")], nullarr, Npn_histo_L2PT_err2_pred_el, rate_histo_mu, rate_histo_e, c5, pad_h5, pad_r5, leg5);
  c3->SaveAs(plotdir+"L2PT_all"+option+".png");
  c4->SaveAs(plotdir+"L2PT_mu"+option+".png");
  c5->SaveAs(plotdir+"L2PT_el"+option+".png");

  TH1F *total_BR_histo_e = (TH1F*)  hists[getHist("NBs_BR_histo_e")]->Clone("total_BR_histo_e");
  TH1F *total_BR_histo_mu = (TH1F*) hists[getHist("NBs_BR_histo_mu")]->Clone("total_BR_histo_mu");
  total_BR_histo_e->Add(hists[getHist("NnotBs_BR_histo_e")]);
  total_BR_histo_mu->Add(hists[getHist("NnotBs_BR_histo_mu")]);
  hists[getHist("NBs_BR_histo_e")]->Divide(hists[getHist("NBs_BR_histo_e")], total_BR_histo_e,1,1,"B");
  hists[getHist("NBs_BR_histo_mu")]->Divide(hists[getHist("NBs_BR_histo_mu")], total_BR_histo_mu,1,1,"B");
  hists[getHist("NBs_BR_histo_e")]->GetXaxis()->SetTitle("Nbtags"); 
  hists[getHist("NBs_BR_histo_e")]->GetYaxis()->SetTitle("Abundance");
  hists[getHist("NBs_BR_histo_e")]->GetYaxis()->SetRangeUser(0.,1.);
  hists[getHist("NBs_BR_histo_e")]->SetTitle("B Abundance vs Nbtags (Njets >= 2) (electrons)");
  hists[getHist("NBs_BR_histo_mu")]->GetXaxis()->SetTitle("Nbtags"); 
  hists[getHist("NBs_BR_histo_mu")]->GetYaxis()->SetTitle("Abundance");
  hists[getHist("NBs_BR_histo_mu")]->GetYaxis()->SetRangeUser(0.,1.);
  hists[getHist("NBs_BR_histo_mu")]->SetTitle("B Abundance vs Nbtags (Njets >= 2) (muons)");

  TCanvas *c6 = new TCanvas("c6","B Abundance vs Nbtags (Njets >= 2) (electrons)", 800,800);
  c6->cd();
  hists[getHist("NBs_BR_histo_e")]->Draw("histE");
  TCanvas *c7 = new TCanvas("c7","B Abundance vs Nbtags (Njets >= 2) (muons)", 800,800);
  c7->cd();
  hists[getHist("NBs_BR_histo_mu")]->Draw("histE");

  TCanvas c8;
  hists[getHist("pTrel_histo_mu")]->Draw();
  c8.SaveAs(plotdir+"pTrel_mu"+option+".png");
  hists[getHist("pTrel_histo_el")]->Draw();
  c8.SaveAs(plotdir+"pTrel_el"+option+".png");
  c8.SetLogz();
  pTrelvsIso_histo_mu->Draw("colz");
  c8.SaveAs(plotdir+"pTrelvsIso_mu"+option+".png");
  pTrelvsIso_histo_el->Draw("colz");
  c8.SaveAs(plotdir+"pTrelvsIso_el"+option+".png");
  pTrelvsMiniIso_histo_mu->Draw("colz");
  c8.SaveAs(plotdir+"pTrelvsMiniIso_mu"+option+".png");
  pTrelvsMiniIso_histo_el->Draw("colz");
  c8.SaveAs(plotdir+"pTrelvsMiniIso_el"+option+".png");

  //Print tables
  for (int i = 0; i < 4; i++){
    electrons.setRowLabel(Form("%i", i), i);
    muons.setRowLabel(Form("%i", i), i);
  }
  if (soup){
    cout << "setting soup" << endl;
    electrons.forSlideMaker("closure_elec_soup.tex");
    muons.forSlideMaker("closure_muon_soup.tex"); 
  }
  else if (PC){
    cout << "setting PC" << endl;
    electrons.forSlideMaker("closure_elec_PC.tex");
    muons.forSlideMaker("closure_muon_PC.tex"); 
  }
  else if (PCssZ){
    cout << "setting PCssZ" << endl;
    electrons.forSlideMaker("closure_elec_PCssZ.tex");
    muons.forSlideMaker("closure_muon_PCssZ.tex"); 
  }
  else if (ssZ){
    cout << "setting ssZ" << endl;
    electrons.forSlideMaker("closure_elec_ssZ.tex");
    muons.forSlideMaker("closure_muon_ssZ.tex"); 
  }
  else if (notCC){
    cout << "setting notCC" << endl;
    electrons.forSlideMaker("closure_elec_notCC.tex");
    muons.forSlideMaker("closure_muon_notCC.tex"); 
  }
  else if (looseEMVA){
    cout << "setting FO2pFO4" << endl;
    electrons.forSlideMaker("closure_elec_FO2pFO4.tex");
    muons.forSlideMaker("closure_muon_FO2pFO4.tex"); 
  }
  else {
    cout << "setting normal" << endl;
    electrons.forSlideMaker("closure_elec_normal.tex");
    muons.forSlideMaker("closure_muon_normal.tex"); 
  }

  printCounter(true);

  return 0;
}
