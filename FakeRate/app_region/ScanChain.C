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
#include "TPaveText.h"
#include "../../CORE/SSSelections.h"
#include "../../CORE/IsolationTools.h"
#include "../../CORE/Tools/utils.h"
#include "../../CORE/Tools/dorky/dorky.cc"
#include "../../classFiles/v6.02/SS.h"
#include "../../software/tableMaker/CTable.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../commonUtils.h"

using namespace std;
using namespace duplicate_removal;

CTable electrons;
CTable muons;

bool doLatex = true;
bool doRatio = false;

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

float getFakeRate(int id, float pt, float eta, float ht, bool extrPtRel = false, bool doData = false, bool doInSitu = false){
   // ht = 100; // FIXME - this is so we force the usage of iso triggers
    if (doInSitu) return fakeRateInSitu(id, pt, eta);
    else if (doData) return fakeRate(id, pt, eta, ht);
    else return qcdMCFakeRate(id, pt, eta, ht);
}

float getFakeRateError(int id, float pt, float eta, float ht, bool doInSitu = false) { 
    if (doInSitu) return fakeRateErrorInSitu(id, pt, eta);
    else return fakeRateError(id, pt, eta, ht);
}

float getFakeRate2(int id, float pt, float eta, float ht, bool extrPtRel = false, bool doData = false){
  if (doData) return fakeRateNoCC(id, pt, eta, ht);
  else return qcdMCFakeRateNoCC(id, pt, eta, ht);
}

void GetErrorPlot(TH1F *pred, vector< vector<TH2D*> > pred_err2_mu, vector< vector<TH2D*> > pred_err2_el, bool inSitu){

  for (int bin=1;bin<=pred->GetNbinsX();++bin) {
    int sr = bin-1;
    float p = pred->GetBinContent(bin);
    //stat error on nFOs
    float pe = pred->GetBinError(bin);
    float pefr2 = 0;
    //stat error on FR 
    for (unsigned int i = 0; i < 2; i++){
      if (pred_err2_mu[i][sr]!=0) { 
        for (int frbinx=1;frbinx<=pred_err2_mu[i][sr]->GetNbinsX();++frbinx) {
          for (int frbiny=1;frbiny<=pred_err2_mu[i][sr]->GetNbinsY();++frbiny) {
            float fr = getFakeRate(13, pred_err2_mu[i][sr]->GetXaxis()->GetBinLowEdge(frbinx), pred_err2_mu[i][sr]->GetYaxis()->GetBinLowEdge(frbiny), i == 0 ? 500 : 150, false, inSitu); 
            float fre = getFakeRateError(13, pred_err2_mu[i][sr]->GetXaxis()->GetBinLowEdge(frbinx), pred_err2_mu[i][sr]->GetYaxis()->GetBinLowEdge(frbiny), i == 0 ? 500 : 150,  inSitu); 
            float tot = pred_err2_mu[i][sr]->GetBinContent(frbinx,frbiny);
            pefr2 += fre*fre*pow(1-fr,-4)*tot*tot;
          }
        }
      }
      if (pred_err2_el[i][sr]!=0) { 
        for (int frbinx=1;frbinx<=pred_err2_el[i][sr]->GetNbinsX();++frbinx) {
          for (int frbiny=1;frbiny<=pred_err2_el[i][sr]->GetNbinsY();++frbiny) {
            float fr = getFakeRate(11, pred_err2_el[i][sr]->GetXaxis()->GetBinLowEdge(frbinx), pred_err2_el[i][sr]->GetYaxis()->GetBinLowEdge(frbiny), i == 0 ? 500 : 150, false, inSitu);
            float fre = getFakeRateError(11, pred_err2_el[i][sr]->GetXaxis()->GetBinLowEdge(frbinx), pred_err2_el[i][sr]->GetYaxis()->GetBinLowEdge(frbiny), i == 0 ? 500 : 150,  inSitu);
            float tot = pred_err2_el[i][sr]->GetBinContent(frbinx,frbiny);
            pefr2 += fre*fre*pow(1-fr,-4)*tot*tot;
          }
        }
      }
    }
    //now combine the two
    pe = sqrt(pe*pe + pefr2);
    pred->SetBinError(bin, pe); 
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

TH1F* histCreator(string str1, string str2, int n1, float n2, float n3){
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

int ScanChain( TChain* chain, TString option = "", TString ptRegion = "HH", bool doData = false, int nEvents = -1){

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

  bool doLowHT = option.Contains("IsoTrigs") ? true : false;
  bool doHighHT = option.Contains("HTTrigs") ? true : false;

  bool weightOne = false;
  bool bypass = false;
  bool doSubtractContamination = false;
  bool requireIsoTriggers = false;

  float luminosity = doData ? getLumi() : 12.9;

  //Dir
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  int nsr = 33;
  if (highlow) nsr = 27;
  if (lowlow)  nsr = 8;

  //Histograms
  hists.push_back( histCreator("Npn_histo_br_obs"       , "Observed Prompt-NonPrompt Background"              ,  4, 0,    4) ); 
  hists.push_back( histCreator("Npn_histo_br_pred"      , "Predicted Prompt-NonPrompt Background"             ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_br_obs_mu"    , "Observed Prompt-NonPrompt Background (Single mu)"  ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_br_pred_mu"   , "Predicted Prompt-NonPrompt Background (Single mu)" ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_br_obs_el"    , "Observed Prompt-NonPrompt Background (Single el)"  ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_br_pred_el"   , "Predicted Prompt-NonPrompt Background (Single el)" ,  4, 0,    4) );
  hists.push_back( histCreator("Npn_histo_sr_obs"       , "Observed Prompt-NonPrompt Background"              , nsr, 0.5,   nsr+0.5) );
  hists.push_back( histCreator("Npn_histo_sr_pred"      , "Predicted Prompt-NonPrompt Background"             , nsr, 0.5,   nsr+0.5) );
  hists.push_back( histCreator("Npn_histo_sr_obs_mu"    , "Observed Prompt-NonPrompt Background (Single mu)"  , nsr, 0.5,   nsr+0.5) );
  hists.push_back( histCreator("Npn_histo_sr_pred_mu"   , "Predicted Prompt-NonPrompt Background (Single mu)" , nsr, 0.5,   nsr+0.5) );
  hists.push_back( histCreator("Npn_histo_sr_obs_el"    , "Observed Prompt-NonPrompt Background (Single el)"  , nsr, 0.5,   nsr+0.5) );
  hists.push_back( histCreator("Npn_histo_sr_pred_el"   , "Predicted Prompt-NonPrompt Background (Single el)" , nsr, 0.5,   nsr+0.5) );
  hists.push_back( histCreator("Npn_histo_HT_obs"       , "Observed Prompt-NonPrompt Background"              , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_pred"      , "Predicted Prompt-NonPrompt Background"             , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_obs_mu"    , "Observed Prompt-NonPrompt Background (Single mu)"  , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_pred_mu"   , "Predicted Prompt-NonPrompt Background (Single mu)" , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_obs_el"    , "Observed Prompt-NonPrompt Background (Single el)"  , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_HT_pred_el"   , "Predicted Prompt-NonPrompt Background (Single el)" , 20, 0, 1000) );
  hists.push_back( histCreator("Npn_histo_MET_obs"      , "Observed Prompt-NonPrompt Background"              , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MET_pred"     , "Predicted Prompt-NonPrompt Background"             , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MET_obs_mu"   , "Observed Prompt-NonPrompt Background (Single mu)"  , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MET_pred_mu"  , "Predicted Prompt-NonPrompt Background (Single mu)" , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MET_obs_el"   , "Observed Prompt-NonPrompt Background (Single el)"  , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MET_pred_el"  , "Predicted Prompt-NonPrompt Background (Single el)" , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MTMIN_obs"    , "Observed Prompt-NonPrompt Background"              , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MTMIN_pred"   , "Predicted Prompt-NonPrompt Background"             , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MTMIN_obs_mu" , "Observed Prompt-NonPrompt Background (Single mu)"  , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MTMIN_pred_mu", "Predicted Prompt-NonPrompt Background (Single mu)" , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MTMIN_obs_el" , "Observed Prompt-NonPrompt Background (Single el)"  , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_MTMIN_pred_el", "Predicted Prompt-NonPrompt Background (Single el)" , 20, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L1PT_obs"     , "Observed Prompt-NonPrompt Background"              , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L1PT_pred"    , "Predicted Prompt-NonPrompt Background"             , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L1PT_obs_mu"  , "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L1PT_pred_mu" , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L1PT_obs_el"  , "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L1PT_pred_el" , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L2PT_obs"     , "Observed Prompt-NonPrompt Background"              , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L2PT_pred"    , "Predicted Prompt-NonPrompt Background"             , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L2PT_obs_mu"  , "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L2PT_pred_mu" , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L2PT_obs_el"  , "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_L2PT_pred_el" , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,   250) );
  hists.push_back( histCreator("Npn_histo_LTrue_obs"     , "Observed Prompt-NonPrompt Background"              , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LTrue_pred"    , "Predicted Prompt-NonPrompt Background"             , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LTrue_obs_mu"  , "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LTrue_pred_mu" , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LTrue_obs_el"  , "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LTrue_pred_el" , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LFake_obs"     , "Observed Prompt-NonPrompt Background"              , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LFake_pred"    , "Predicted Prompt-NonPrompt Background"             , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LFake_obs_mu"  , "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LFake_pred_mu" , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LFake_obs_el"  , "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,  250) );
  hists.push_back( histCreator("Npn_histo_LFake_pred_el" , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,  250) );
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

  //Make rate histo (for shape only)
  float xbins[6] = {10, 15, 25, 35, 50, 1000};
  float ybinse[4] = {0, 0.8, 1.479, 2.5}; 
  float ybinsm[4] = {0, 1.2, 2.1, 2.4}; 
  TH2D *rate_histo_e  = new TH2D("elec", "elec", 5, xbins, 3, ybinse);
  TH2D *rate_histo_mu = new TH2D("muon", "muon", 5, xbins, 3, ybinsm);

  //Errors
  vector< vector<TH2D*> > Npn_histo_br_err2_pred_mu(2, vector<TH2D*>(4,0));  
  vector< vector<TH2D*> > Npn_histo_br_err2_pred_el(2, vector<TH2D*>(4,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<4;++h){
      Npn_histo_br_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_br_err2_pred_mu_br%i",h));
      Npn_histo_br_err2_pred_mu[i][h]->Reset();
      Npn_histo_br_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_br_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_br_err2_pred_el_br%i",h));
      Npn_histo_br_err2_pred_el[i][h]->Reset();
      Npn_histo_br_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_sr_err2_pred_mu(2, vector<TH2D*>(40,0));  
  vector< vector<TH2D*> > Npn_histo_sr_err2_pred_el(2, vector<TH2D*>(40,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<40;++h) {
      Npn_histo_sr_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_sr_err2_pred_mu_sr%i",h));
      Npn_histo_sr_err2_pred_mu[i][h]->Reset();
      Npn_histo_sr_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_sr_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_sr_err2_pred_el_sr%i",h));
      Npn_histo_sr_err2_pred_el[i][h]->Reset();
      Npn_histo_sr_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_HT_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_HT_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_HT_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_HT_err2_pred_mu_HT%i",h));
      Npn_histo_HT_err2_pred_mu[i][h]->Reset();
      Npn_histo_HT_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_HT_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_HT_err2_pred_el_HT%i",h));
      Npn_histo_HT_err2_pred_el[i][h]->Reset();
      Npn_histo_HT_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_MET_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_MET_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_MET_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_MET_err2_pred_mu_MET%i",h));
      Npn_histo_MET_err2_pred_mu[i][h]->Reset();
      Npn_histo_MET_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_MET_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_MET_err2_pred_el_MET%i",h));
      Npn_histo_MET_err2_pred_el[i][h]->Reset();
      Npn_histo_MET_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_MTMIN_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_MTMIN_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_MTMIN_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_MTMIN_err2_pred_mu_MTMIN%i",h));
      Npn_histo_MTMIN_err2_pred_mu[i][h]->Reset();
      Npn_histo_MTMIN_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_MTMIN_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_MTMIN_err2_pred_el_MTMIN%i",h));
      Npn_histo_MTMIN_err2_pred_el[i][h]->Reset();
      Npn_histo_MTMIN_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_L1PT_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_L1PT_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_L1PT_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_L1PT_err2_pred_mu_L1PT%i",h));
      Npn_histo_L1PT_err2_pred_mu[i][h]->Reset();
      Npn_histo_L1PT_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_L1PT_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_L1PT_err2_pred_el_L1PT%i",h));
      Npn_histo_L1PT_err2_pred_el[i][h]->Reset();
      Npn_histo_L1PT_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_L2PT_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_L2PT_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_L2PT_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_L2PT_err2_pred_mu_L2PT%i",h));
      Npn_histo_L2PT_err2_pred_mu[i][h]->Reset();
      Npn_histo_L2PT_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_L2PT_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_L2PT_err2_pred_el_L2PT%i",h));
      Npn_histo_L2PT_err2_pred_el[i][h]->Reset();
      Npn_histo_L2PT_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_LTrue_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_LTrue_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_LTrue_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_LTrue_err2_pred_mu_LTrue%i",h));
      Npn_histo_LTrue_err2_pred_mu[i][h]->Reset();
      Npn_histo_LTrue_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_LTrue_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_LTrue_err2_pred_el_LTrue%i",h));
      Npn_histo_LTrue_err2_pred_el[i][h]->Reset();
      Npn_histo_LTrue_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_LFake_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_LFake_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_LFake_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_LFake_err2_pred_mu_LFake%i",h));
      Npn_histo_LFake_err2_pred_mu[i][h]->Reset();
      Npn_histo_LFake_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_LFake_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_LFake_err2_pred_el_LFake%i",h));
      Npn_histo_LFake_err2_pred_el[i][h]->Reset();
      Npn_histo_LFake_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
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
  float e1a = 0.;  //rate = Nt/Nl
  float e2 = 0.;  //rate = Nt/Nl
  float e2a = 0.;  //rate = Nt/Nl
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

    
    TString fname(currentFile->GetTitle());

    // Get File Content
    TFile *file = new TFile( fname );
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

         if(fname.Contains("TTBAR")) { fname = "TTBAR";  }
    else if(fname.Contains("DY"))    { fname = "DY";     }
    else if(fname.Contains("TTW"))   { fname = "TTW";    }
    else if(fname.Contains("TTZ"))   { fname = "TTZ";    }
    else if(fname.Contains("WJets")) { fname = "WJets";  }
    else if(fname.Contains("WZ3L"))  { fname = "WZ";     }
    else if(fname.Contains("Data"))  { fname = "Data";   }

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
      float weight = ss::is_real_data() ? 1.0 : ss::scale1fb()*luminosity*getTruePUw(ss::trueNumInt()[0]);

      TString filename = fname;

      // // ignore MC part of chain when looking at data, except for contamination subtraction
      // if(doData && !ss::is_real_data()) weight = 0; 

      if( !(ss::njets() >= 2 && (ss::ht() > 500 ? 1 : ss::met() > 30) ) ) continue;


      //require triggers
      if (!ss::fired_trigger() && ss::is_real_data()) continue;
      // if (!ss::fired_trigger()) continue;
      if (doLowHT) {
	if (ss::ht()>300.) continue;
      }
      if (doHighHT) {
	if (ss::ht()<300.) continue;
      }


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
          if (lep1_ptrel_v1>7.2) lep1_pT = ss::lep1_p4().pt()*(1+std::max(0.,ss::lep1_miniIso()-0.12));
          else lep1_pT = std::max(ss::lep1_p4().pt(),ss::jet_close_lep1().pt()*float(0.80));
        } 
        else {
          if (lep1_ptrel_v1>7.2) lep1_pT = ss::lep1_p4().pt()*(1+std::max(0.,ss::lep1_miniIso()-0.16));
          else lep1_pT = std::max(ss::lep1_p4().pt(),ss::jet_close_lep1().pt()*float(0.76));
        }
        if (abs(ss::lep2_id())==11) {
          if (lep2_ptrel_v1>7.2) lep2_pT = ss::lep2_p4().pt()*(1+std::max(0.,ss::lep2_miniIso()-0.12));
          else lep2_pT = std::max(ss::lep2_p4().pt(),ss::jet_close_lep2().pt()*float(0.80));
        } 
        else {
          if (lep2_ptrel_v1>7.2) lep2_pT = ss::lep2_p4().pt()*(1+std::max(0.,ss::lep2_miniIso()-0.16));
          else lep2_pT = std::max(ss::lep2_p4().pt(),ss::jet_close_lep2().pt()*float(0.76));
        }
      }
      if (jetCorr){
        lep1_pT = ss::jet_close_lep1().pt();
        lep2_pT = ss::jet_close_lep2().pt();
      }


      if (abs(ss::lep1_id())==11 && lep1_pT<15.) continue;
      if (abs(ss::lep2_id())==11 && lep2_pT<15.) continue;

      //Determine passes ID
      bool lep1_passes_id = ss::lep1_passes_id();
      bool lep2_passes_id = ss::lep2_passes_id();


      if (ss::ht()<300.) {
	//Skip if does not pass FO for isolated triggers
	if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA())) continue;
	if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA())) continue;
	//do not do LL for LowHT
	if (lep1_pT < 25. && lep2_pT < 25.) continue;
      } 


      //Determine mtMin
      float mtmin = ss::mt() > ss::mt_l2() ? ss::mt_l2() : ss::mt();
      if (coneCorr){
        float mtl1 = MT(lep1_pT, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
        float mtl2 = MT(lep2_pT, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
        mtmin = mtl1 > mtl2 ? mtl2 : mtl1;
      }

      //Determine SR and BR
      anal_type_t ac_base = analysisCategory(ss::lep1_id(), ss::lep2_id(), lep1_pT, lep2_pT);
      int br = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pT, lep2_pT);
      if (br<0) continue;
      int sr = signalRegion2016(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pT, lep2_pT);


      //lepton pT selection
      if(highhigh && ac_base!=HighHigh) continue;
      if(highlow  && ac_base!=HighLow ) continue;
      if(lowlow   && ac_base!=LowLow  ) continue;

      // if(sr < 30) continue; // FIXME -- this is for checking the tail SR excess
      // if(ss::event() == 58699579) continue; // SUPER FIXME
      // bypass = (ss::event() == 1110509127); // SUPER FIXME
      // weightOne = false; // SUPER-DUPER FIXME
      // requireIsoTriggers = false;
      // if(requireIsoTriggers && ss::is_real_data()) {
      //     // std::cout << " ss::triggers(): " << ss::triggers() << std::endl;
      //     if(!((ss::triggers() & (1<<2)) || (ss::triggers() & (1<<3)) || (ss::triggers() & (1<<6)))) continue;
      // }

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

        // e2 = getFakeRate(abs(ss::lep2_id()), lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, false, inSitu );
        // if(!weightOne) weight *= (e2/(1-e2));

          prompt1_reco += weight;  
          NpromptL2_reco += weight;  

          hists[getHist("Npn_histo_sr_obs")   ]->Fill(sr, weight);
          hists[getHist("Npn_histo_br_obs")   ]->Fill(br, weight);
          hists[getHist("Npn_histo_HT_obs")   ]->Fill(ss::ht(), weight);
          hists[getHist("Npn_histo_MET_obs")  ]->Fill(ss::met(), weight);
          hists[getHist("Npn_histo_MTMIN_obs")]->Fill(mtmin, weight);
          hists[getHist("Npn_histo_L1PT_obs") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_L2PT_obs") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
          hists[getHist("Npn_histo_LTrue_obs") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_LFake_obs") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);

          if(abs(ss::lep2_id()) == 11){
            hists[getHist("Npn_histo_sr_obs_el")   ]->Fill(sr, weight);
            hists[getHist("Npn_histo_br_obs_el")   ]->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_el")   ]->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_el")  ]->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_el")]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_L1PT_obs_el") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_el") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            hists[getHist("Npn_histo_LTrue_obs_el") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_LFake_obs_el") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);


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
            hists[getHist("Npn_histo_LTrue_obs_mu") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_LFake_obs_mu") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);

            addToCounter(filename+Form("_obs_mu_BR%i", br), weight);
          }
        }

        //2) Lep 1 is non-prompt
        else if( isLep1NonPrompt && isLep2Prompt ){ 

        // e1 = getFakeRate(abs(ss::lep1_id()), lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, false, inSitu );
        // if(!weightOne) weight *= (e1/(1-e1));

          prompt1_reco += weight;
          NpromptL2_reco += weight;

          hists[getHist("Npn_histo_sr_obs")]   ->Fill(sr, weight);
          hists[getHist("Npn_histo_br_obs")]   ->Fill(br, weight);
          hists[getHist("Npn_histo_HT_obs")]   ->Fill(ss::ht(), weight);
          hists[getHist("Npn_histo_MET_obs")]  ->Fill(ss::met(), weight);
          hists[getHist("Npn_histo_MTMIN_obs")]->Fill(mtmin, weight);
          hists[getHist("Npn_histo_L1PT_obs") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_L2PT_obs") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
          hists[getHist("Npn_histo_LFake_obs") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_LTrue_obs") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
          
          if(abs(ss::lep1_id()) == 11){
            hists[getHist("Npn_histo_sr_obs_el")]   ->Fill(sr, weight);
            hists[getHist("Npn_histo_br_obs_el")]   ->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_el")]   ->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_el")]  ->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_el")]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_L1PT_obs_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            hists[getHist("Npn_histo_LFake_obs_el") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_LTrue_obs_el") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            
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
            hists[getHist("Npn_histo_LFake_obs_mu") ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_LTrue_obs_mu") ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);

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
      
      // Contamination subtraction: Subtract prompt-prompt MC 
      // (which ends up as class 2 instead of 3) from data prediction by assigning negative weight
      bool subtractContamination = false;
      float mult = 1.0;
      if(doData && doSubtractContamination && !ss::is_real_data() && ss::hyp_class() == 2 && ss::lep1_motherID()==1 && ss::lep2_motherID()==1) {
        mult = -1.0; // invert weight to subtract from the prediction
        subtractContamination = true;
        filename = "Data"; // when filling counter below, want it to think we are filling in negative data
      }

      //////////////////////////////////////////////////////////////////////////////////////////
      //                         SINGLE FAKES                                                 // 
      //////////////////////////////////////////////////////////////////////////////////////////

      e1 = 0.; //rate for lep1
      e2 = 0.; //rate for lep2
      float w = 0.;
      int nonIso = 0; 


      //prompt-nonprompt background
      // if we're doing data, we want to only fill prompt-nonprompt prediction (class 2) with data
      // and leave all MC in class 3 observations (if they have truth fakes)
      if( ( doData && (ss::is_real_data() && (ss::hyp_class() == 2 || inSitu)) ) || // if doing data, only fill pred with data
          (!doData && (ss::hyp_class() == 2 || inSitu) ) || // if not doing data, fill pred with MC (old behaviour)
          (doData && subtractContamination && !ss::is_real_data()) ) { // do contamination subtraction if doData and MC
          
          // FIXME
      // std::cout << ss::run() << ":" << ss::lumi() << ":" << ss::event() << " " << ss::lep1_id() << " " << lep1_pT << " " << ss::lep2_id() << " " << lep2_pT << " " << sr << std::endl;

        int nbjets = ss::nbtags();
        if (nbjets > 3) nbjets = 3; 

        //0) InSituFR variables
      float ptrel_cut_1    = (abs(ss::lep1_id()) == 11 ? 7.20 : 7.20);
      float ptrel_cut_2    = (abs(ss::lep2_id()) == 11 ? 7.20 : 7.20);
      float ptratio_cut_1  = (abs(ss::lep1_id()) == 11 ? 0.80 : 0.76);
      float ptratio_cut_2  = (abs(ss::lep2_id()) == 11 ? 0.80 : 0.76);
      float mini_cut_1     = (abs(ss::lep1_id()) == 11 ? 0.12 : 0.16);
      float mini_cut_2     = (abs(ss::lep2_id()) == 11 ? 0.12 : 0.16);
      bool lep1_denom_iso  = ((ss::lep1_miniIso() < 0.4) && ((ss::lep1_ptrel_v1() > ptrel_cut_1) || ((ss::lep1_closeJet().pt()/ss::lep1_p4().pt()) < (1.0/ptratio_cut_1 + ss::lep1_miniIso()))));
      bool lep2_denom_iso  = ((ss::lep2_miniIso() < 0.4) && ((ss::lep2_ptrel_v1() > ptrel_cut_2) || ((ss::lep2_closeJet().pt()/ss::lep2_p4().pt()) < (1.0/ptratio_cut_2 + ss::lep2_miniIso()))));


      // doData ss::is_real_data() inSitu ss::hyp_class() isFakeLeg(2) isGoodLeg(1) lep2_denom_iso ss::lep2_multiIso() ss::passed_id_inSituFR_lep1() ss::passed_id_inSituFR_lep2()
      // std::cout << " doData: " << doData << " ss::is_real_data(): " << ss::is_real_data() << " inSitu: " << inSitu << " ss::hyp_class(): " << ss::hyp_class() << " isFakeLeg(2): " << isFakeLeg(2) << " isGoodLeg(1): " << isGoodLeg(1) << " lep2_denom_iso: " << lep2_denom_iso << " ss::lep2_multiIso(): " << ss::lep2_multiIso() << " ss::passed_id_inSituFR_lep1(): " << ss::passed_id_inSituFR_lep1() << " ss::passed_id_inSituFR_lep2(): " << ss::passed_id_inSituFR_lep2() << std::endl;

        //1) Lep1 is tight, lep2 is loose!tight
        if (lep1_passes_id && !lep2_passes_id){

          //Baseline selections
          if (!inSitu && (ss::hyp_class() != 2)) continue;
          if (inSitu && (ss::lep2_multiIso() || !isFakeLeg(2) || !isGoodLeg(1) || !lep2_denom_iso)) continue;
          if (inSitu && (!ss::passed_id_inSituFR_lep1() || !ss::passed_id_inSituFR_lep2())) continue;

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

          if (abs(ss::lep2_id()) == 11){  
            e2 = getFakeRate(11, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, doData, inSitu );
            e2a = getFakeRate2(11, ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::ht(), false, doData); 
            w = coneCorr ? (e2/(1-e2))*weight : (e2a/(1-e2a))*weight;
            // std::cout << "lep2 is L!T with pt,eta,fr = " << lep2_pT << "," << fabs(ss::lep2_p4().eta()) << "," << e2 << std::endl; // FIXME
            if(weightOne) w = 1.0;
            if(subtractContamination) w = mult*weight;

            addToCounter(filename+Form("_pred_el_BR%i", br), w);

            hists[getHist("Npn_histo_sr_pred_el")]   ->Fill(sr, w);
            hists[getHist("Npn_histo_br_pred_el")]   ->Fill(br, w);
            hists[getHist("Npn_histo_HT_pred_el")]   ->Fill(ss::ht(), w);
            hists[getHist("Npn_histo_MET_pred_el")]  ->Fill(ss::met(), w);
            hists[getHist("Npn_histo_MTMIN_pred_el")]->Fill(mtmin, w);
            hists[getHist("Npn_histo_L1PT_pred_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_L2PT_pred_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            hists[getHist("Npn_histo_LTrue_pred_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            if(isFakeLeg(2)) hists[getHist("Npn_histo_LFake_pred_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            if (sr>=0) Npn_histo_sr_err2_pred_el[(ss::ht() > 300)][sr-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_br_err2_pred_el[ss::ht() > 300][br]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_HT_err2_pred_el[(ss::ht() > 300)][hists[getHist("Npn_histo_HT_pred_el")]->FindBin(ss::ht())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MET_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MET_pred_el")]->FindBin(ss::met())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MTMIN_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MTMIN_pred_el")]->FindBin(mtmin)-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L1PT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L1PT_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L2PT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L2PT_pred_el")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_LTrue_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_LTrue_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_LFake_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_LFake_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            // fill el abundance histos here w/ nbtags
            if(ss::lep2_motherID() == -1) hists[getHist("NBs_BR_histo_e")]->Fill(nbjets, w);
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) hists[getHist("NnotBs_BR_histo_e")]->Fill(nbjets, w);
            if(ss::lep2_motherID() == -1) Bs_e = Bs_e + mult*weight;
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) notBs_e = notBs_e + mult*weight;
          }
          else if (abs(ss::lep2_id()) == 13){ 
            e2 = getFakeRate(13, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, doData, inSitu );
            e2a = getFakeRate2(13, ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), ss::ht(), false, doData); 
            w = coneCorr ? (e2/(1-e2))*weight : (e2a/(1-e2a))*weight;
            // std::cout << "lep2 is L!T with pt,eta,fr = " << lep2_pT << "," << fabs(ss::lep2_p4().eta()) << "," << e2 << std::endl; // FIXME
            if(weightOne) w = 1.0;
            if(subtractContamination) w = mult*weight;

            addToCounter(filename+Form("_pred_mu_BR%i", br), w);

            hists[getHist("Npn_histo_sr_pred_mu")]->Fill(sr, w);
            hists[getHist("Npn_histo_br_pred_mu")]->Fill(br, w);
            hists[getHist("Npn_histo_HT_pred_mu")]->Fill(ss::ht(), w);
            hists[getHist("Npn_histo_MET_pred_mu")]->Fill(ss::met(), w);
            hists[getHist("Npn_histo_MTMIN_pred_mu")]->Fill(mtmin, w);
            hists[getHist("Npn_histo_L1PT_pred_mu")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_L2PT_pred_mu")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            hists[getHist("Npn_histo_LTrue_pred_mu")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            if(isFakeLeg(2)) hists[getHist("Npn_histo_LFake_pred_mu")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            if (sr>=0) Npn_histo_sr_err2_pred_mu[(ss::ht() > 300)][sr-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_br_err2_pred_mu[ss::ht() > 300][br]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_HT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_HT_pred_mu")]->FindBin(ss::ht())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MET_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MET_pred_mu")]->FindBin(ss::met())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MTMIN_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MTMIN_pred_mu")]->FindBin(mtmin)-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L1PT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L1PT_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L2PT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L2PT_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_LTrue_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_LTrue_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_LFake_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_LFake_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            if(ss::lep2_motherID() == -1) hists[getHist("NBs_BR_histo_mu")]->Fill(nbjets, mult*weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) hists[getHist("NnotBs_BR_histo_mu")]->Fill(nbjets, w);
            if(ss::lep2_motherID() == -1) Bs_mu = Bs_mu + mult*weight;
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) notBs_mu = notBs_mu + mult*weight;
          }
          Npn = Npn + w;
          if (ss::lep2_motherID()==1) Npn_s = Npn_s + w;
          hists[getHist("Npn_histo_sr_pred")]->Fill(sr, w);
          hists[getHist("Npn_histo_br_pred")]->Fill(br, w);
          hists[getHist("Npn_histo_HT_pred")]->Fill(ss::ht(), w);
          hists[getHist("Npn_histo_MET_pred")]->Fill(ss::met(), w);
          hists[getHist("Npn_histo_MTMIN_pred")]->Fill(mtmin, w);
          hists[getHist("Npn_histo_L1PT_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
          hists[getHist("Npn_histo_L2PT_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
          hists[getHist("Npn_histo_LTrue_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
          if(isFakeLeg(2)) hists[getHist("Npn_histo_LFake_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
        }

        //2) Lep1 is loose!tight, lep2 is tight
        if (!lep1_passes_id && lep2_passes_id){

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

          if( abs(ss::lep1_id()) == 11 ){	//if el, use el rate.  FILL WITH NONPROMPT			  
            e1 = getFakeRate(11, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, doData, inSitu );
            e1a = getFakeRate2(11, ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::ht(), false, doData); 
            w = coneCorr ? (e1/(1-e1))*weight : (e1a/(1-e1a))*weight;
            // std::cout << "lep1 is L!T with pt,eta,fr = " << lep1_pT << "," << fabs(ss::lep1_p4().eta()) << "," << e1 << std::endl; // FIXME
            if(weightOne) w = 1.0;
            if(subtractContamination) w = mult*weight;

            addToCounter(filename+Form("_pred_el_BR%i", br), w);

            hists[getHist("Npn_histo_sr_pred_el")]   ->Fill(sr, w);
            hists[getHist("Npn_histo_br_pred_el")]   ->Fill(br, w);
            hists[getHist("Npn_histo_HT_pred_el")]   ->Fill(ss::ht(), w);
            hists[getHist("Npn_histo_MET_pred_el")]  ->Fill(ss::met(), w);
            hists[getHist("Npn_histo_MTMIN_pred_el")]->Fill(mtmin, w);
            hists[getHist("Npn_histo_L1PT_pred_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_L2PT_pred_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            if(isFakeLeg(1)) hists[getHist("Npn_histo_LFake_pred_el")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_LTrue_pred_el")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);

            if (sr>=0) Npn_histo_sr_err2_pred_el[(ss::ht() > 300)][sr-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_br_err2_pred_el[ss::ht() > 300][br]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_HT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_HT_pred_el")]->FindBin(ss::ht())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MET_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MET_pred_el")]->FindBin(ss::met())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MTMIN_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MTMIN_pred_el")]->FindBin(mtmin)-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L1PT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L1PT_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L2PT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L2PT_pred_el")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_LFake_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_LFake_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_LTrue_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_LTrue_pred_el")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            // fill el abundance histos here w/ nbtags
            if(ss::lep1_motherID() == -1) hists[getHist("NBs_BR_histo_e")]->Fill(nbjets, mult*weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) hists[getHist("NnotBs_BR_histo_e")]->Fill(nbjets, w);
            if(ss::lep1_motherID() == -1) Bs_e = Bs_e + mult*weight;
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) notBs_e = notBs_e + mult*weight;
          }
          else if( abs(ss::lep1_id()) == 13 ){ //if mu, use mu rate.  FILL WITH NONPROMPT				  
            e1 = getFakeRate(13, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, doData, inSitu );
            e1a = getFakeRate2(13, ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), ss::ht(), false, doData); 
            w = coneCorr ? (e1/(1-e1))*weight : (e1a/(1-e1a))*weight;
            // std::cout << "lep1 is L!T with pt,eta,fr = " << lep1_pT << "," << fabs(ss::lep1_p4().eta()) << "," << e1 << std::endl; // FIXME
            if(weightOne) w = 1.0;
            if(subtractContamination) w = mult*weight;

            addToCounter(filename+Form("_pred_mu_BR%i", br), w);

            hists[getHist("Npn_histo_sr_pred_mu")]->Fill(sr, w);
            hists[getHist("Npn_histo_br_pred_mu")]->Fill(br, w);
            hists[getHist("Npn_histo_HT_pred_mu")]->Fill(ss::ht(), w);
            hists[getHist("Npn_histo_MET_pred_mu")]->Fill(ss::met(), w);
            hists[getHist("Npn_histo_MTMIN_pred_mu")]->Fill(mtmin, w);
            hists[getHist("Npn_histo_L1PT_pred_mu")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_L2PT_pred_mu")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            if(isFakeLeg(1)) hists[getHist("Npn_histo_LFake_pred_mu")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_LTrue_pred_mu")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);

            if (sr>=0) Npn_histo_sr_err2_pred_mu[(ss::ht() > 300)][sr-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_br_err2_pred_mu[ss::ht() > 300][br]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_HT_err2_pred_mu[(ss::ht() > 300)][hists[getHist("Npn_histo_HT_pred_mu")]->FindBin(ss::ht())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MET_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MET_pred_mu")]->FindBin(ss::met())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MTMIN_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MTMIN_pred_mu")]->FindBin(mtmin)-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L1PT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L1PT_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L2PT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L2PT_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_LFake_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_LFake_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_LTrue_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_LTrue_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            // fill el abundance histos here w/ nbtags
            if(ss::lep1_motherID() == -1) hists[getHist("NBs_BR_histo_mu")]->Fill(nbjets, mult*weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) hists[getHist("NnotBs_BR_histo_mu")]->Fill(nbjets, w);
            if(ss::lep1_motherID() == -1) Bs_mu = Bs_mu + mult*weight;
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) notBs_mu = notBs_mu + mult*weight;
          }
          Npn = Npn + w;
          if (ss::lep1_motherID()==1) Npn_s = Npn_s + w;
          hists[getHist("Npn_histo_sr_pred")]->Fill(sr, w);
          hists[getHist("Npn_histo_br_pred")]->Fill(br, w);
          hists[getHist("Npn_histo_HT_pred")]->Fill(ss::ht(), w);
          hists[getHist("Npn_histo_MET_pred")]->Fill(ss::met(), w);
          hists[getHist("Npn_histo_MTMIN_pred")]->Fill(mtmin, w);
          hists[getHist("Npn_histo_L1PT_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
          hists[getHist("Npn_histo_L2PT_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
          if(isFakeLeg(1)) hists[getHist("Npn_histo_LFake_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
          hists[getHist("Npn_histo_LTrue_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
        }
      } //end hyp = 2 if statement
      
      // // resume ignoring MC part of chain when looking at data, except for contamination subtraction
      // if(doData && !ss::is_real_data()) weight = 0; 

      //nonprompt-nonprompt background
      else if(ss::hyp_class() == 1){
        if( ss::lep1_id()*ss::lep2_id() > 0 ){
          if( abs(ss::lep2_id()) == 11 ) e2 = getFakeRate(11, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, doData, inSitu );
          else if( abs(ss::lep2_id()) == 13 ) e2 = getFakeRate(13, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, doData, inSitu );
          if( abs(ss::lep1_id()) == 11)      e1 = getFakeRate(11, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, doData, inSitu );
          else if( abs(ss::lep1_id()) == 13) e1 = getFakeRate(13, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, doData, inSitu );
          Nnn = Nnn + w*weight;
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

  //redefine option to save also ptRegion in output files
  option=option+"_"+ptRegion;

  std::string plotdir="plots/";

  // TString commonOptions = Form(" --isLinear --outOfFrame --type Supplementary (Simulation) --dataName Data --noDivisionLabel --noRatioPlot --lumi %.2f --yTitleOffset -0.2", luminosity);// --systBlack --systFillStyle 3345
  TString commonOptions = Form(" --isLinear --outOfFrame --type Supplementary (Simulation) --dataName Data --noDivisionLabel --lumi %.2f --yTitleOffset -0.2", luminosity);// --systBlack --systFillStyle 3345
  // std::string mc_type = "t#bar{t} MC";
  std::string mc_type = "Data";

  //BR plots
  GetErrorPlot(hists[getHist("Npn_histo_br_pred")], Npn_histo_br_err2_pred_mu, Npn_histo_br_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_br_pred_mu")], Npn_histo_br_err2_pred_mu, Npn_histo_br_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_br_pred_el")], Npn_histo_br_err2_pred_mu, Npn_histo_br_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_br_pred")], { make_pair(hists[getHist("Npn_histo_br_obs")], hists[getHist("Npn_histo_br_obs")] ) }, {"t#bar{t}"}, "BRs", mc_type+"", Form("--outputName %s --xAxisLabel Baseline Region --noXaxisUnit"+commonOptions, (plotdir+"br_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_br_pred_mu")], { make_pair(hists[getHist("Npn_histo_br_obs_mu")], hists[getHist("Npn_histo_br_obs_mu")] ) }, {"t#bar{t}"}, "BRs", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel Baseline Region --noXaxisUnit"+commonOptions, (plotdir+"br_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_br_pred_el")], { make_pair(hists[getHist("Npn_histo_br_obs_el")], hists[getHist("Npn_histo_br_obs_el")]) }, {"t#bar{t}"}, "BRs", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel Baseline Region --noXaxisUnit"+commonOptions, (plotdir+"br_el"+option).Data()), {}, {}, { kYellow }); 

  //SR plots
  string typeAG = "HH";
  if (highlow) typeAG = "HL";
  if (lowlow)  typeAG = "LL";
  GetErrorPlot(hists[getHist("Npn_histo_sr_pred")], Npn_histo_sr_err2_pred_mu, Npn_histo_sr_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_sr_pred_mu")], Npn_histo_sr_err2_pred_mu, Npn_histo_sr_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_sr_pred_el")], Npn_histo_sr_err2_pred_mu, Npn_histo_sr_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_sr_pred")], { make_pair(hists[getHist("Npn_histo_sr_obs")], hists[getHist("Npn_histo_sr_obs")]) }, {"t#bar{t}"}, Form("%s SRs", typeAG.c_str()), mc_type+"", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptions, (plotdir+"sr_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_sr_pred_mu")], { make_pair(hists[getHist("Npn_histo_sr_obs_mu")], hists[getHist("Npn_histo_sr_obs_mu")]) }, {"t#bar{t}"}, Form("%s SRs", typeAG.c_str()), mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptions, (plotdir+"sr_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_sr_pred_el")], { make_pair(hists[getHist("Npn_histo_sr_obs_el")], hists[getHist("Npn_histo_sr_obs_el")]) }, {"t#bar{t}"}, Form("%s SRs", typeAG.c_str()), mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptions, (plotdir+"sr_el"+option).Data()), {}, {}, { kYellow }); 

  //HT plots
  GetErrorPlot(hists[getHist("Npn_histo_HT_pred")], Npn_histo_HT_err2_pred_mu, Npn_histo_HT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_HT_pred_mu")], Npn_histo_HT_err2_pred_mu, Npn_histo_HT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_HT_pred_el")], Npn_histo_HT_err2_pred_mu, Npn_histo_HT_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_HT_pred")], { make_pair(hists[getHist("Npn_histo_HT_obs")], hists[getHist("Npn_histo_HT_obs")]) }, {"t#bar{t}"}, "H_{T}", mc_type+"", Form("--outputName %s --xAxisLabel H_{T}"+commonOptions, (plotdir+"HT_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_HT_pred_mu")], { make_pair(hists[getHist("Npn_histo_HT_obs_mu")], hists[getHist("Npn_histo_HT_obs_mu")]) }, {"t#bar{t}"}, "H_{T}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel H_{T}"+commonOptions, (plotdir+"HT_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_HT_pred_el")], { make_pair(hists[getHist("Npn_histo_HT_obs_el")], hists[getHist("Npn_histo_HT_obs_el")]) }, {"t#bar{t}"}, "H_{T}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel H_{T}"+commonOptions, (plotdir+"HT_el"+option).Data()), {}, {}, { kYellow }); 

  //MET plots
  GetErrorPlot(hists[getHist("Npn_histo_MET_pred")], Npn_histo_MET_err2_pred_mu, Npn_histo_MET_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MET_pred_mu")], Npn_histo_MET_err2_pred_mu, Npn_histo_MET_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MET_pred_el")], Npn_histo_MET_err2_pred_mu, Npn_histo_MET_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_MET_pred")], { make_pair(hists[getHist("Npn_histo_MET_obs")], hists[getHist("Npn_histo_MET_obs")]) }, {"t#bar{t}"}, "MET", mc_type+"", Form("--outputName %s --xAxisLabel MET"+commonOptions, (plotdir+"MET_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_MET_pred_mu")], { make_pair(hists[getHist("Npn_histo_MET_obs_mu")], hists[getHist("Npn_histo_MET_obs_mu")]) }, {"t#bar{t}"}, "MET", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel MET"+commonOptions, (plotdir+"MET_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_MET_pred_el")], { make_pair(hists[getHist("Npn_histo_MET_obs_el")], hists[getHist("Npn_histo_MET_obs_el")]) }, {"t#bar{t}"}, "MET", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel MET"+commonOptions, (plotdir+"MET_el"+option).Data()), {}, {}, { kYellow }); 

  //MTMIN plots
  GetErrorPlot(hists[getHist("Npn_histo_MTMIN_pred")], Npn_histo_MTMIN_err2_pred_mu, Npn_histo_MTMIN_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MTMIN_pred_mu")], Npn_histo_MTMIN_err2_pred_mu, Npn_histo_MTMIN_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MTMIN_pred_el")], Npn_histo_MTMIN_err2_pred_mu, Npn_histo_MTMIN_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_MTMIN_pred")], { make_pair(hists[getHist("Npn_histo_MTMIN_obs")], hists[getHist("Npn_histo_MTMIN_obs")]) }, {"t#bar{t}"}, "M_{T}^{min}", mc_type+"", Form("--outputName %s --xAxisLabel M_{T}^{min}"+commonOptions, (plotdir+"MTMIN_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_MTMIN_pred_mu")], { make_pair(hists[getHist("Npn_histo_MTMIN_obs_mu")], hists[getHist("Npn_histo_MTMIN_obs_mu")]) }, {"t#bar{t}"}, "M_{T}^{min}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel M_{T}^{min}"+commonOptions, (plotdir+"MTMIN_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_MTMIN_pred_el")], { make_pair(hists[getHist("Npn_histo_MTMIN_obs_el")], hists[getHist("Npn_histo_MTMIN_obs_el")]) }, {"t#bar{t}"}, "M_{T}^{min}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel M_{T}^{min}"+commonOptions, (plotdir+"MTMIN_el"+option).Data()), {}, {}, { kYellow }); 

  //L1PT plots
  GetErrorPlot(hists[getHist("Npn_histo_L1PT_pred")], Npn_histo_L1PT_err2_pred_mu, Npn_histo_L1PT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_L1PT_pred_mu")], Npn_histo_L1PT_err2_pred_mu, Npn_histo_L1PT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_L1PT_pred_el")], Npn_histo_L1PT_err2_pred_mu, Npn_histo_L1PT_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_L1PT_pred")], { make_pair(hists[getHist("Npn_histo_L1PT_obs")], hists[getHist("Npn_histo_L1PT_obs")]) }, {"t#bar{t}"}, "L1 p_{T}", mc_type+"", Form("--outputName %s --xAxisLabel L1 p_{T}"+commonOptions, (plotdir+"L1PT_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_L1PT_pred_mu")], { make_pair(hists[getHist("Npn_histo_L1PT_obs_mu")], hists[getHist("Npn_histo_L1PT_obs_mu")]) }, {"t#bar{t}"}, "L1 p_{T}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel L1 p_{T}"+commonOptions, (plotdir+"L1PT_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_L1PT_pred_el")], { make_pair(hists[getHist("Npn_histo_L1PT_obs_el")], hists[getHist("Npn_histo_L1PT_obs_el")]) }, {"t#bar{t}"}, "L1 p_{T}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel L1 p_{T}"+commonOptions, (plotdir+"L1PT_el"+option).Data()), {}, {}, { kYellow }); 

  //L2PT plots
  GetErrorPlot(hists[getHist("Npn_histo_L2PT_pred")], Npn_histo_L2PT_err2_pred_mu, Npn_histo_L2PT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_L2PT_pred_mu")], Npn_histo_L2PT_err2_pred_mu, Npn_histo_L2PT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_L2PT_pred_el")], Npn_histo_L2PT_err2_pred_mu, Npn_histo_L2PT_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_L2PT_pred")], { make_pair(hists[getHist("Npn_histo_L2PT_obs")], hists[getHist("Npn_histo_L2PT_obs")]) }, {"t#bar{t}"}, "L2 p_{T}", mc_type+"", Form("--outputName %s --xAxisLabel L2 p_{T}"+commonOptions, (plotdir+"L2PT_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_L2PT_pred_mu")], { make_pair(hists[getHist("Npn_histo_L2PT_obs_mu")], hists[getHist("Npn_histo_L2PT_obs_mu")]) }, {"t#bar{t}"}, "L2 p_{T}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel L2 p_{T}"+commonOptions, (plotdir+"L2PT_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_L2PT_pred_el")], { make_pair(hists[getHist("Npn_histo_L2PT_obs_el")], hists[getHist("Npn_histo_L2PT_obs_el")]) }, {"t#bar{t}"}, "L2 p_{T}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel L2 p_{T}"+commonOptions, (plotdir+"L2PT_el"+option).Data()), {}, {}, { kYellow }); 
  
  //LTrue plots
  GetErrorPlot(hists[getHist("Npn_histo_LTrue_pred")], Npn_histo_LTrue_err2_pred_mu, Npn_histo_LTrue_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_LTrue_pred_mu")], Npn_histo_LTrue_err2_pred_mu, Npn_histo_LTrue_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_LTrue_pred_el")], Npn_histo_LTrue_err2_pred_mu, Npn_histo_LTrue_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_LTrue_pred")], { make_pair(hists[getHist("Npn_histo_LTrue_obs")], hists[getHist("Npn_histo_LTrue_obs")]) }, {"t#bar{t}"}, "LTight p_{T}", mc_type+", all", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LTight p_{T} --noDivisionLabel  --isLinear --noOverflow --lumi 2.26 --noXaxisUnit --systBlack --systFillStyle 3345 --type Simulation --lumiPrec 1", (plotdir+"LTrue_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_LTrue_pred_mu")], { make_pair(hists[getHist("Npn_histo_LTrue_obs_mu")], hists[getHist("Npn_histo_LTrue_obs_mu")]) }, {"t#bar{t}"}, "LTight p_{T}", mc_type+", mu", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LTight p_{T} --noDivisionLabel  --isLinear --noOverflow --lumi 2.26 --noXaxisUnit --systBlack --systFillStyle 3345 --type Simulation --lumiPrec 1", (plotdir+"LTrue_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_LTrue_pred_el")], { make_pair(hists[getHist("Npn_histo_LTrue_obs_el")], hists[getHist("Npn_histo_LTrue_obs_el")]) }, {"t#bar{t}"}, "LTight p_{T}", mc_type+", el", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LTight p_{T} --noDivisionLabel  --isLinear --noOverflow --lumi 2.26 --noXaxisUnit --systBlack --systFillStyle 3345 --type Simulation --lumiPrec 1", (plotdir+"LTrue_el"+option).Data()), {}, {}, { kYellow }); 

  //LFake plots
  GetErrorPlot(hists[getHist("Npn_histo_LFake_pred")], Npn_histo_LFake_err2_pred_mu, Npn_histo_LFake_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_LFake_pred_mu")], Npn_histo_LFake_err2_pred_mu, Npn_histo_LFake_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_LFake_pred_el")], Npn_histo_LFake_err2_pred_mu, Npn_histo_LFake_err2_pred_el, inSitu);
  dataMCplotMaker(hists[getHist("Npn_histo_LFake_pred")], { make_pair(hists[getHist("Npn_histo_LFake_obs")], hists[getHist("Npn_histo_LFake_obs")]) }, {"t#bar{t}"}, "LLoose p_{T}", mc_type+", all", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LLoose p_{T} --noDivisionLabel  --isLinear --noOverflow --lumi 2.26 --noXaxisUnit --systBlack --systFillStyle 3345 --type Simulation --lumiPrec 1", (plotdir+"LFake_all"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_LFake_pred_mu")], { make_pair(hists[getHist("Npn_histo_LFake_obs_mu")], hists[getHist("Npn_histo_LFake_obs_mu")]) }, {"t#bar{t}"}, "LLoose p_{T}", mc_type+", mu", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LLoose p_{T} --noDivisionLabel  --isLinear --noOverflow --lumi 2.26 --noXaxisUnit --systBlack --systFillStyle 3345 --type Simulation --lumiPrec 1", (plotdir+"LFake_mu"+option).Data()), {}, {}, { kYellow }); 
  dataMCplotMaker(hists[getHist("Npn_histo_LFake_pred_el")], { make_pair(hists[getHist("Npn_histo_LFake_obs_el")], hists[getHist("Npn_histo_LFake_obs_el")]) }, {"t#bar{t}"}, "LLoose p_{T}", mc_type+", el", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LLoose p_{T} --noDivisionLabel  --isLinear --noOverflow --lumi 2.26 --noXaxisUnit --systBlack --systFillStyle 3345 --type Simulation --lumiPrec 1", (plotdir+"LFake_el"+option).Data()), {}, {}, { kYellow }); 


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
