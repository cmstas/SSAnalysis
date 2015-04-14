// Usage:
// > root -b doAll.C

// C++
#include <iostream>
#include <iomanip>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TH2D.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TColor.h"
#include "TString.h"

#include "../../CORE/SSSelections.h"

// lepfilter
#include "lepfilter.cc"

using namespace std;
using namespace samesign;

// #ifdef __MAKECINT__ 
// #pragma link C++ class ROOT::Math::PxPyPzE4D<float>+; 
// #pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >+;
// #endif 

void DrawPlots(TH1F *pred, TH1F *obs, TH2D **pred_err2_mu, TH2D **pred_err2_el, TH2D *rate_histo_mu,  TH2D *rate_histo_e, TCanvas *c, TPad *pad_h, TPad *pad_r, TLegend *leg)
{
  pred->GetXaxis()->SetTitle("Signal Region"); 
  pred->GetYaxis()->SetTitle("Events");
  obs->GetXaxis()->SetTitle("Signal Region"); 
  obs->GetYaxis()->SetTitle("Events");
  obs->SetLineColor(kRed);

  pred->SetLineWidth(2);
  obs->SetLineWidth(2);

  pad_h->Draw();
  pad_r->Draw();
 
  //Overlaid histos
  pad_h->cd();
  pred->Draw("histE");
  obs->Draw("samehistE");

  //legend
  leg->SetLineColor(kWhite);
  leg->SetTextFont(42); 
  leg->SetTextSize(0.06);
  leg->SetShadowColor(kWhite); 
  leg->SetFillColor(kWhite); 
  TString predname = pred->GetName();
  TString obsname = obs->GetName();
  leg->AddEntry(predname,"Predicted","l");
  leg->AddEntry(obsname,"Observed","l");  
  leg->Draw();

  //ratio histo
  pad_r->cd();
  TH1F *ratio = (TH1F*) pred->Clone("ratio");
  ratio->Divide(ratio, obs);
  ratio->GetXaxis()->SetRangeUser(0,40); 
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
  
  int w = 18;
  cout << setw(5) << "SR" <<  setw(w) << "Pred" << setw(w) << "Obs" << setw(w) << "Pred/Obs" << setw(w) << "(p-o)/p" << endl;

  for (int bin=1;bin<=pred->GetNbinsX();++bin) {
    if (bin%10 == 0) continue;
    if (bin%10 != 1) continue;
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

    //cout << Form("SR=%2i - pred=%5.2f +/- %5.2f - obs=%5.2f +/- %5.2f - pred/obs=%5.2f +/- %5.2f - (p-o)/p=%5.2f +/- %5.2f", sr, p, pe, o, oe, (o>0?p/o:99.99),ratioe, (p>0?(p-o)/p:99.99), laste ) << endl;	
	cout << setw(5) << sr <<  setw(w) << Form("%5.2f +/-%5.2f", p, pe) << setw(w) << Form("%5.2f +/-%5.2f", o, oe) << setw(w) << Form("%5.2f +/-%5.2f", (o>0?p/o:99.99),ratioe) << setw(w) << Form("%5.2f +/-%5.2f", (p>0?(p-o)/p:99.99), laste) << endl;
  }


}

//getPt and getEta need to stay on sync with meas region
float getPt(float pt, bool extrPtRel) {
  if(pt < 10.)  return 11.;   //use this if lower FR histo bound is 10.
  if(!extrPtRel && pt >= 70.) return 69.;
  if(extrPtRel && pt >= 150.) return 149.;
  return pt;
}

float getEta(float eta, float ht, bool extrPtRel) {
  if (extrPtRel) {
    if(ht >= 800) return 799;
    return ht;
  }
  if(fabs(eta) >= 2.4) return 2.3;
  return fabs(eta);
}

float getFakeRate(TH2D* histo, float pt, float eta, float ht, bool extrPtRel) //change if bounds of histo change
{
  float e = 0;

  //if above or below bounds, reassign so pick rate from closest bin.
  pt = getPt(pt,extrPtRel);
  eta = getEta(eta,ht,extrPtRel);
 
  e = histo->GetBinContent( histo->FindBin(pt, fabs(eta) ));

  return e;
}


int ScanChain( TChain* chain, TString fakeratefile, TString option = "", TString ptRegion = "HH", bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  bool coneCorr = false;
  if (option.Contains("coneCorr")) coneCorr=true;
  bool noSIP = false;
  if (option.Contains("noSIP")) noSIP = true;
  bool unIso = false;
  if (option.Contains("unIso")) unIso = true;
  bool doBonly = false;
  if (option.Contains("doBonly")) doBonly = true;
  bool doConly = false;
  if (option.Contains("doConly")) doConly = true;
  bool doLightonly = false;
  if (option.Contains("doLightonly")) doLightonly = true;
  bool lowPtRel14 = false;
  if (option.Contains("lowPtRel14")) lowPtRel14 = true;
  bool lowPtRel6 = false;
  if (option.Contains("lowPtRel6")) lowPtRel6 = true;
  bool useMiniIso = false;
  if (option.Contains("useMiniIso")) useMiniIso = true;
  bool useNewMiniIso = false;
  if (option.Contains("useNewMiniIso")) useNewMiniIso = true;
  bool extrPtRel = false;
  if (option.Contains("extrPtRel")) {
    extrPtRel = true;
    unIso = true;
    noSIP = true;
  }
  bool highhigh = false;
  if (ptRegion.Contains("HH")) highhigh = true;
  bool highlow = false;
  if (ptRegion.Contains("HL")) highlow = true;
  bool lowlow = false;
  if (ptRegion.Contains("LL")) lowlow = true;

  //histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  TH1F *Npn_histo_obs = new TH1F("Npn_histo_obs", "Observed Prompt-NonPrompt Background", 40, 0, 40);
  Npn_histo_obs->SetDirectory(rootdir);
  Npn_histo_obs->Sumw2();

  TH1F *Npn_histo_pred = new TH1F("Npn_histo_pred", "Predicted Prompt-NonPrompt Background", 40, 0, 40);
  Npn_histo_pred->SetDirectory(rootdir);
  Npn_histo_pred->Sumw2();

  TH1F *Npn_histo_obs_mu = new TH1F("Npn_histo_obs_mu", "Observed Prompt-NonPrompt Background (Single mu)", 40, 0, 40);
  Npn_histo_obs_mu->SetDirectory(rootdir);
  Npn_histo_obs_mu->Sumw2();

  TH1F *Npn_histo_pred_mu = new TH1F("Npn_histo_pred_mu", "Predicted Prompt-NonPrompt Background (Single mu)", 40, 0, 40);
  Npn_histo_pred_mu->SetDirectory(rootdir);
  Npn_histo_pred_mu->Sumw2();

  TH1F *Npn_histo_obs_el = new TH1F("Npn_histo_obs_el", "Observed Prompt-NonPrompt Background (Single el)", 40, 0, 40);
  Npn_histo_obs_el->SetDirectory(rootdir);
  Npn_histo_obs_el->Sumw2();

  TH1F *Npn_histo_pred_el = new TH1F("Npn_histo_pred_el", "Predicted Prompt-NonPrompt Background (Single el)", 40, 0, 40);
  Npn_histo_pred_el->SetDirectory(rootdir);
  Npn_histo_pred_el->Sumw2();

  TH1F *NBs_BR_histo_e = new TH1F("NBs_BR_histo_e", "Number of FO's from B's vs Nbtags (els)", 4,0,4);
  NBs_BR_histo_e->SetDirectory(rootdir);
  NBs_BR_histo_e->Sumw2();

  TH1F *NBs_BR_histo_mu = new TH1F("NBs_BR_histo_mu", "Number of FO's from B's vs Nbtags (muons)", 4,0,4);
  NBs_BR_histo_mu->SetDirectory(rootdir);
  NBs_BR_histo_mu->Sumw2();

  TH1F *NnotBs_BR_histo_e = new TH1F("NnotBs_BR_histo_e", "Number of FO's NOT from B's vs Nbtags (els)", 4,0,4);
  NnotBs_BR_histo_e->SetDirectory(rootdir);
  NnotBs_BR_histo_e->Sumw2();

  TH1F *NnotBs_BR_histo_mu = new TH1F("NnotBs_BR_histo_mu", "Number of FO's NOT from B's vs Nbtags (muons)", 4,0,4);
  NnotBs_BR_histo_mu->SetDirectory(rootdir);
  NnotBs_BR_histo_mu->Sumw2();

  TH2D *pTrelvsIso_histo_mu = new TH2D("pTrelvsIso_histo_mu", "pTrel vs Iso (Muons)", 10, 0., 1., 15, 0., 30.);
  pTrelvsIso_histo_mu->SetDirectory(rootdir);
  pTrelvsIso_histo_mu->Sumw2();

  TH2D *pTrelvsIso_histo_el = new TH2D("pTrelvsIso_histo_el", "pTrel vs Iso (Electrons)", 10, 0., 1., 15, 0., 30.);
  pTrelvsIso_histo_el->SetDirectory(rootdir);
  pTrelvsIso_histo_el->Sumw2();

  TH1D *pTrel_histo_el = new TH1D("pTrel_histo_el", "pTrel (Electrons)", 15, 0., 30.);
  pTrel_histo_el->SetDirectory(rootdir);
  pTrel_histo_el->Sumw2();

  TH1D *pTrel_histo_mu = new TH1D("pTrel_histo_mu", "pTrel (Muons)", 15, 0., 30.);
  pTrel_histo_mu->SetDirectory(rootdir);
  pTrel_histo_mu->Sumw2();

  //---Load rate histos-----//
  TFile *InputFile = new TFile(fakeratefile,"read");

  TH2D *rate_histo_e = 0, *rate_histo_mu = 0;
  // if (extrPtRel) {
  //   //test same for e and mu
  //   rate_histo_e  = (TH2D*) InputFile->Get("rate_histo")->Clone("rate_histo");
  //   rate_histo_mu = (TH2D*) InputFile->Get("rate_histo")->Clone("rate_histo");
  // } else 
  if (coneCorr) {
    //fake rate as function of pT+pT*RelIso, eta
    rate_histo_e = (TH2D*) InputFile->Get("rate_cone_histo_e")->Clone("rate_cone_histo_e");
    rate_histo_mu = (TH2D*) InputFile->Get("rate_cone_histo_mu")->Clone("rate_cone_histo_mu");
  } else {
    //fake rate as function of pT, eta
    rate_histo_e = (TH2D*) InputFile->Get("rate_histo_e")->Clone("rate_histo_e");
    rate_histo_mu = (TH2D*) InputFile->Get("rate_histo_mu")->Clone("rate_histo_mu");
  }

  //errors
  TH2D *Npn_histo_err2_pred_mu[40] = {0};
  TH2D *Npn_histo_err2_pred_el[40] = {0};
  for (int h=0;h<40;++h) {
    Npn_histo_err2_pred_mu[h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_err2_pred_mu_sr%i",h));
    Npn_histo_err2_pred_mu[h]->Reset();
    Npn_histo_err2_pred_mu[h]->SetDirectory(rootdir);
    //Npn_histo_err2_pred_mu[h]->Sumw2();

    Npn_histo_err2_pred_el[h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_err2_pred_el_sr%i",h));
    Npn_histo_err2_pred_el[h]->Reset();
    Npn_histo_err2_pred_el[h]->SetDirectory(rootdir);
    //Npn_histo_err2_pred_el[h]->Sumw2();
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
  float e = 0.;  //rate = Nt/Nl
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

    // Get File Content
    TFile *file = new TFile( currentFile->GetTitle() );
    TTree *tree = (TTree*)file->Get("t");
    if(fast) TTreeCache::SetLearnEntries(10);
    if(fast) tree->SetCacheSize(128*1024*1024);
    ss.Init(tree);
    
    // Loop over Events in current file
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
    
      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      ss.GetEntry(event);
      ++nEventsTotal;
    
      // Progress
      lepfilter::progress( nEventsTotal, nEventsChain );
	  
      // Analysis Code
      float weight = ss.scale1fb()*10.0;
	  
	  //remove pt cut and rely on pT region cuts.
	  //if( !( ss.lep1_p4().pt() > 15 && ss.lep2_p4().pt() > 15  &&ss.njets() >= 2 && (ss.ht() > 500 ? 1 : ss.met() > 30) ) )
	  if( !(ss.njets() >= 2 && (ss.ht() > 500 ? 1 : ss.met() > 30) ) )
	  	{
	  	  {continue;}
	  	} 

	  if (doBonly) {
	    //consider only prompt or bs
	    if (ss.lep2_motherID()!=1 && ss.lep2_motherID()!=-1) continue;
	    if (ss.lep1_motherID()!=1 && ss.lep1_motherID()!=-1) continue;
	  }
	  else if (doConly) {
	    //consider only prompt or cs
	    if (ss.lep2_motherID()!=1 && ss.lep2_motherID()!=-2) continue;
	    if (ss.lep1_motherID()!=1 && ss.lep1_motherID()!=-2) continue;
	  }
	  else if (doLightonly) {
	    //consider only prompt or lights
	    if (ss.lep2_motherID()!=1 && ss.lep2_motherID()!=0) continue;
	    if (ss.lep1_motherID()!=1 && ss.lep1_motherID()!=0) continue;
	    //EMEnriched starts at 20 GeV
	    if ( (abs(ss.lep1_id())==11 && ss.lep1_motherID()==0 && ss.lep1_p4().pt() < 20) || 
		 (abs(ss.lep2_id())==11 && ss.lep2_motherID()==0 && ss.lep2_p4().pt() < 20) ) continue;

	  }

	  if (lowPtRel6) {
	    if (ss.lep1_ptrel_v1()>6. || ss.lep2_ptrel_v1()>6.) continue;
	  }	  
	  if (useMiniIso) {
	    if (ss.lep1_ptrel_v1()<=6. && ss.lep2_ptrel_v1()<=6.) continue;
	  }	  

	  //if (std::min(ss.mt(),ss.mt_l2())<100) continue;	  

	  //------------------------------------------------------------------------
	  unsigned int ac_base = analysisCategory(ss.lep1_p4().pt(), ss.lep2_p4().pt());
	  passesBaselineCuts(ss.njets(), ss.nbtags(), ss.met(), ss.ht(), ac_base);
	  //cout << " ac=" << ac_base;
	  if (ac_base==0) continue;
	  int br = baselineRegion(ss.nbtags());
	  unsigned int ac_sig = ac_base;
	  passesSignalRegionCuts(ss.ht(), ss.met(), ac_sig);
	  //if (debug) cout << "ac_base=" << ac_base << " ac_sig=" << ac_sig << endl;
	  int sr = ac_sig!=0 ? signalRegion(ss.njets(), ss.nbtags(), ss.met(), ss.ht()) : -1;
	  //if (sr<=0) continue;
	  //------------------------------------------------------------------------

	  if ( (ss.lep1_p4().pt() > 25. && ss.lep2_p4().pt() > 25.) ) {
	    if( ss.lep1_id()*ss.lep2_id() > 0 ) {
	      if (ss.lep1_motherID()<=0 && ss.lep1_iso()>0.1 && fabs(ss.lep1_ip3d()/ss.lep1_ip3d_err())<4. && ss.lep2_motherID()==1) {
		if (abs(ss.lep1_id())==11) {
		  pTrelvsIso_histo_el->Fill( std::min(ss.lep1_iso(),float(0.99)), std::min(ss.lep1_ptrel_v1(),float(29.9)) );
		  pTrel_histo_el->Fill(std::min(ss.lep1_ptrel_v1(),float(29.9)) );
		} else {
		  pTrelvsIso_histo_mu->Fill( std::min(ss.lep1_iso(),float(0.99)), std::min(ss.lep1_ptrel_v1(),float(29.9)) );
		  pTrel_histo_mu->Fill(std::min(ss.lep1_ptrel_v1(),float(29.9)) );
		}
	      }
	      if (ss.lep2_motherID()<=0 && ss.lep2_iso()>0.1 && fabs(ss.lep2_ip3d()/ss.lep2_ip3d_err())<4. && ss.lep1_motherID()==1) {
		if (abs(ss.lep2_id())==11) {
		  pTrelvsIso_histo_el->Fill( std::min(ss.lep2_iso(),float(0.99)), std::min(ss.lep2_ptrel_v1(),float(29.9)) );
		  pTrel_histo_el->Fill(std::min(ss.lep2_ptrel_v1(),float(29.9)) );
		} else {
		  pTrelvsIso_histo_mu->Fill( std::min(ss.lep2_iso(),float(0.99)), std::min(ss.lep2_ptrel_v1(),float(29.9)) );
		  pTrel_histo_mu->Fill(std::min(ss.lep2_ptrel_v1(),float(29.9)) );
		}
	      }
	    }
	  }

	  if (ss.hyp_class() == 3)
		{
		  //reco->ss on reco level
		  //-----------------------------------------------------------------------
		  //consider only highhigh pT for the moment
		  if(highhigh)
			{if ( !(ss.lep1_p4().pt() > 25. && ss.lep2_p4().pt() > 25.) ) continue;}
		  else if(lowlow)
			{if ( !((ss.lep1_p4().pt() > 10. && ss.lep1_p4().pt() < 25.) && (ss.lep2_p4().pt() > 10. && ss.lep2_p4().pt() < 25.)) )  continue;}
		  else if(highlow)
			{if ( !((ss.lep1_p4().pt() > 10. && ss.lep1_p4().pt() < 25. && ss.lep2_p4().pt() > 25.) || (ss.lep2_p4().pt() > 10. && ss.lep2_p4().pt() < 25. && ss.lep1_p4().pt() > 25.)) ) continue;}
		  
		  counter++;
		  
		  //check for charge misID on reco level.
		  if( ss.lep1_id()*ss.lep2_id() < 0 )
			{
			  //it's an opp sign pair
			  sign_misid_reco = sign_misid_reco + weight;
			}
		  else
			{
			  //It's a same sign pair.
			  Nss_reco = Nss_reco + weight;
			  if( ss.lep1_motherID()==1 && ss.lep2_motherID()==1 ) //both are prompt
				{
				  prompt2_reco = prompt2_reco + weight;
				  NpromptL1_reco = NpromptL1_reco + weight;
				  NpromptL2_reco = NpromptL2_reco + weight;
				}
			  else if ( ss.lep1_motherID()==2 || ss.lep2_motherID()==2 ) 
			    {
			      //this is a charge flip, do something
			    } 
			  else if( ss.lep1_motherID()==1 && ss.lep2_motherID()<=0 ) //lep2 is nonprompt
				{
				  if (unIso && ss.lep2_iso()<=0.1) continue;
				  if (lowPtRel14 && ss.lep2_ptrel_v1()>14.) continue;
				  if (lowPtRel6  && ss.lep2_ptrel_v1()>6. ) continue;
				  if (useMiniIso && ss.lep2_ptrel_v1()<=6.) continue;
				  prompt1_reco = prompt1_reco + weight;  
				  NpromptL1_reco = NpromptL1_reco + weight;
				  Npn_histo_obs->Fill(sr, weight);
				  Npn_histo_obs->Fill(br, weight);
				  if(abs(ss.lep2_id()) == 11) {
				    Npn_histo_obs_el->Fill(sr, weight);
				    Npn_histo_obs_el->Fill(br, weight);
				  } else if(abs(ss.lep2_id()) == 13) {
				    Npn_histo_obs_mu->Fill(sr, weight);
				    Npn_histo_obs_mu->Fill(br, weight);
				  }
				}
			  else if( ss.lep1_motherID()<=0 && ss.lep2_motherID()==1 ) //lep1 is nonprompt
				{
				  if (unIso && ss.lep1_iso()<=0.1) continue;
				  if (lowPtRel14 && ss.lep1_ptrel_v1()>14.) continue;
				  if (lowPtRel6  && ss.lep1_ptrel_v1()>6. ) continue;
				  if (useMiniIso && ss.lep1_ptrel_v1()<=6.) continue;
				  prompt1_reco = prompt1_reco + weight; 
				  NpromptL2_reco = NpromptL2_reco + weight;				
				  Npn_histo_obs->Fill(sr, weight);
				  Npn_histo_obs->Fill(br, weight);
				  if(abs(ss.lep1_id()) == 11) {
				    Npn_histo_obs_el->Fill(sr, weight);
				    Npn_histo_obs_el->Fill(br, weight);
				  } else if(abs(ss.lep1_id()) == 13) {
				    Npn_histo_obs_mu->Fill(sr, weight);
				    Npn_histo_obs_mu->Fill(br, weight);
				  }
				}
			  else if( (ss.lep1_motherID()<=0 && ss.lep2_motherID()<=0) ) //don't need to explicitly write it.  can just use else
				{
				  if (unIso && ss.lep1_iso()<=0.1) continue;
				  if (unIso && ss.lep2_iso()<=0.1) continue;
				  if (lowPtRel14 && ss.lep1_ptrel_v1()>14.) continue;
				  if (lowPtRel14 && ss.lep2_ptrel_v1()>14.) continue;
				  if (lowPtRel6  && ss.lep1_ptrel_v1()>6. ) continue;
				  if (lowPtRel6  && ss.lep2_ptrel_v1()>6. ) continue;
				  if (useMiniIso && ss.lep1_ptrel_v1()<=6.) continue;
				  if (useMiniIso && ss.lep2_ptrel_v1()<=6.) continue;
				  prompt0_reco = prompt0_reco + weight;
				}
			}
		  //----------------------------------------------------------------------------------
		  //gen->ss on gen level
		  //----------------------------------------------------------------------------------
		  //check for charge misID on gen level.
		  if( abs(ss.lep1_id())==abs(ss.lep1_mc_id()) && abs(ss.lep2_id())==abs(ss.lep2_mc_id())  &&  ss.lep1_mc_id()*ss.lep2_mc_id() < 0 )
			{
			  //it's an opp sign pair
			  sign_misid_gen = sign_misid_gen + weight;
			}
		  else
			{
			  //It's a same sign pair.
			  Nss_gen = Nss_gen + weight;
			  if( ss.lep1_motherID()==1 && ss.lep2_motherID()==1 )
				{
				  prompt2_gen = prompt2_gen + weight;
				  NpromptL1_gen = NpromptL1_gen + weight;
				  NpromptL2_gen = NpromptL2_gen + weight;
				}
			  else if( ss.lep1_motherID()==1 && ss.lep2_motherID()!=1 )
				{
				  prompt1_gen = prompt1_gen + weight;
				  NpromptL1_gen = NpromptL1_gen + weight;
				}
			  else if( ss.lep1_motherID()!=1 && ss.lep2_motherID()==1 )
				{
				  prompt1_gen = prompt1_gen + weight;
				  NpromptL2_gen = NpromptL2_gen + weight;
				}
				  
			  else if( (ss.lep1_motherID()!=1 && ss.lep2_motherID()!=1) ) //don't need to explicitly write it.  can just use else
				{prompt0_gen = prompt0_gen + weight;}
			}
		  //----------------------------------------------------------------------------------
		} //end hyp = 3 if statement

	  //--------------------------------------------------------------------------------------
	  //-------------------------Estimate Npn from QCD fake rate------------------------------
	  //--------------------------------------------------------------------------------------
	  //find N^pn_tt using N_tl and e(pT, eta).  DONT USE GEN LEVEL INFO HERE!!! (ok, we can still cheat to check closure under ideal conditions)

	  e1 = 0.; //rate for lep1
	  e2 = 0.; //rate for lep2
	  
	  //prompt-nonprompt background
	  if( ss.hyp_class() == 2 )  //if tight-loose!tight
		{
		  int nbjets = ss.nbtags();
		  if(nbjets>3) nbjets=3; //overflow for abundance plot
		  
		  float lep1_conepT = ss.lep1_p4().pt()*(1+std::max(0.,ss.lep1_iso()-0.1));
		  float lep2_conepT = ss.lep2_p4().pt()*(1+std::max(0.,ss.lep2_iso()-0.1));
		  if (useMiniIso) {
		    lep1_conepT = ss.lep1_p4().pt()*(1+std::max(0.,ss.lep1_miniIso()-0.05));
		    lep2_conepT = ss.lep2_p4().pt()*(1+std::max(0.,ss.lep2_miniIso()-0.05));
		  } else if (useNewMiniIso) {
		    //cout << "pt=" << ss.lep1_p4().pt() << " ptrel=" << ss.lep1_ptrel_v1() << " miniIso=" << ss.lep1_miniIso() << " jetpt=" << ss.jet_close_lep1().pt() <<  " jetpt2=" << ss.jet_close_lep2().pt() << endl;
		    if (ss.lep1_p4().pt()>25. && ss.lep1_ptrel_v1()>7.) 
		      lep1_conepT = ss.lep1_p4().pt()*(abs(ss.lep1_id())==11 ? std::max(0.,ss.lep1_miniIso()-0.075) : std::max(0.,ss.lep1_miniIso()-0.1));
		    else
		      lep1_conepT = (abs(ss.lep1_id())==11 ? ss.jet_close_lep1().pt()*0.725 : ss.jet_close_lep1().pt()*0.70);
		    if (ss.lep2_p4().pt()>25. && ss.lep2_ptrel_v1()>7.) 
		      lep2_conepT = ss.lep2_p4().pt()*(abs(ss.lep2_id())==11 ? std::max(0.,ss.lep2_miniIso()-0.075) : std::max(0.,ss.lep2_miniIso()-0.1));
		    else
		      lep2_conepT = (abs(ss.lep2_id())==11 ? ss.jet_close_lep2().pt()*0.725 : ss.jet_close_lep2().pt()*0.70);
		  }

		  //apply pTcuts for highhigh region for now, with cone correction if needed
		  if(highhigh){
			if (ss.lep1_passes_id() || coneCorr==false) {
			  if (ss.lep1_p4().pt()<25.) continue;
			} else {
			  if (lep1_conepT<25.) continue;
			}
			if (ss.lep2_passes_id() || coneCorr==false) {
			  if (ss.lep2_p4().pt()<25.) continue;
			} else {
			  if (lep2_conepT<25.) continue;
			}
		  }
		  else if(lowlow){
			if (ss.lep1_passes_id() || coneCorr==false) {
			  if (ss.lep1_p4().pt()>25. || ss.lep1_p4().pt()<10.) continue;
			} else {
			  if (lep1_conepT>25. || lep1_conepT<10.) continue;
			}
			if (ss.lep2_passes_id() || coneCorr==false) {
			  if (ss.lep2_p4().pt()>25. || ss.lep2_p4().pt()<10.) continue;
			} else {
			  if (lep2_conepT>25. || lep2_conepT<10.) continue;
			}
		  }
		  else if(highlow){
			if ((ss.lep1_passes_id() && ss.lep2_passes_id()) || coneCorr==false) {      //both are tight, don't need to use cone corr.  OR cone corr is off
			  if ((ss.lep1_p4().pt()>25. && ss.lep2_p4().pt()>25.) || (ss.lep1_p4().pt()<25. && ss.lep2_p4().pt()<25.) || ss.lep1_p4().pt()<10. || ss.lep2_p4().pt()<10.) continue;
			} else if(ss.lep1_passes_id() && !ss.lep2_passes_id() && coneCorr==true) {  //lep2 fails tight, so use cone corr for it
			  if ((ss.lep1_p4().pt()>25. && lep2_conepT>25.) || (ss.lep1_p4().pt()<25. && lep2_conepT<25.) || ss.lep1_p4().pt()<10. || lep2_conepT<10.) continue;
			} else if(!ss.lep1_passes_id() && ss.lep2_passes_id() && coneCorr==true) {  //lep1 fails tight, so use cone corr for it
				if ((lep1_conepT>25. && ss.lep2_p4().pt()>25.) || (lep1_conepT<25. && ss.lep2_p4().pt()<25.) || lep1_conepT<10. || ss.lep2_p4().pt()<10.) continue;
			  }
		  }

		  //make sure ss on reco level. 
		  if( ss.lep1_id()*ss.lep2_id() > 0 )
			{			  
			  if( ss.lep1_passes_id() && !ss.lep2_passes_id() )  //lep1 is tight, lep2 is loose-not-tight
				{	
				  if (noSIP && fabs(ss.lep2_ip3d()/ss.lep2_ip3d_err())>4.) continue;
				  if (unIso && ss.lep2_iso()<0.1) continue;
				  if (lowPtRel14 && ss.lep2_ptrel_v1()>14.) continue;
				  if (lowPtRel6  && ss.lep2_ptrel_v1()>6. ) continue;
				  if (useMiniIso && ss.lep2_ptrel_v1()<=6.) continue;
				  if (extrPtRel && ss.lep2_ptrel_v1()<6.0 ) continue;
				  float pt = ss.lep2_p4().pt();
				  if (coneCorr) {
				    pt = pt+pt*std::max(0.,ss.lep2_iso()-0.1);
				    if (useMiniIso) pt = pt+pt*std::max(0.,ss.lep2_miniIso()-0.05);
				    if (useNewMiniIso) {
				      if (ss.lep2_p4().pt()>25. && ss.lep2_ptrel_v1()>7.) 
					pt = pt+pt*(abs(ss.lep2_id())==11 ? std::max(0.,ss.lep2_miniIso()-0.075) : std::max(0.,ss.lep2_miniIso()-0.1));
				      else
					pt = (abs(ss.lep2_id())==11 ? ss.jet_close_lep2().pt()*0.725 : ss.jet_close_lep2().pt()*0.70);
				    }
				  }
				  if( abs(ss.lep2_id()) == 11 )  //if el, use el rate.  FILL WITH NONPROMPT
					{
					  e2 = getFakeRate( rate_histo_e, pt, fabs(ss.lep2_p4().eta()), ss.ht(), extrPtRel );					  
					  Npn_histo_pred_el->Fill(sr, (e2/(1-e2))*weight);
					  Npn_histo_pred_el->Fill(br, (e2/(1-e2))*weight);
					  if (sr>=0) Npn_histo_err2_pred_el[sr]->Fill(pt, fabs(ss.lep2_p4().eta()), weight);
					  Npn_histo_err2_pred_el[br]->Fill(pt, fabs(ss.lep2_p4().eta()), weight);
					  // //fill el abundance histos here w/ nbtags
					  if(ss.lep2_motherID() == -1) NBs_BR_histo_e->Fill(nbjets, weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
					  if(ss.lep2_motherID() == -2 || ss.lep2_motherID() == 0) NnotBs_BR_histo_e->Fill(nbjets, weight);
					  // if(ss.lep1_motherID() == -1) NBs_BR_histo_e->Fill(nbjets, weight); //USE ONLY FOR NUM ABUNDANCE PLOTS
					  // if(ss.lep1_motherID() == -2 || ss.lep1_motherID() == 0) NnotBs_BR_histo_e->Fill(nbjets, weight); //USE ONLY FOR NUM ABUNDANCE PLOTS
					  if(ss.lep2_motherID() == -1) Bs_e = Bs_e + weight;
					  if(ss.lep2_motherID() == -2 || ss.lep2_motherID() == 0) notBs_e = notBs_e + weight;
					}
				  else if( abs(ss.lep2_id()) == 13 )  //if mu, use mu rate.  FILL WITH NONPROMPT
					{
					  e2 = getFakeRate( rate_histo_mu, pt, fabs(ss.lep2_p4().eta()), ss.ht(), extrPtRel ) ;
					  Npn_histo_pred_mu->Fill(sr, (e2/(1-e2))*weight);
					  Npn_histo_pred_mu->Fill(br, (e2/(1-e2))*weight);
					  if (sr>=0) Npn_histo_err2_pred_mu[sr]->Fill(pt, fabs(ss.lep2_p4().eta()), weight);
					  Npn_histo_err2_pred_mu[br]->Fill(pt, fabs(ss.lep2_p4().eta()), weight);
					  // //fill mu abundance histos here w/ nbtags
					  if(ss.lep2_motherID() == -1) NBs_BR_histo_mu->Fill(nbjets, weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
					  if(ss.lep2_motherID() == -2 || ss.lep2_motherID() == 0) NnotBs_BR_histo_mu->Fill(nbjets, weight);
					  // if(ss.lep1_motherID() == -1) NBs_BR_histo_mu->Fill(nbjets, weight); //USE ONLY FOR NUM ABUNDANCE PLOTS
					  // if(ss.lep1_motherID() == -2 || ss.lep1_motherID() == 0) NnotBs_BR_histo_mu->Fill(nbjets, weight); //USE ONLY FOR NUM ABUNDANCE PLOTS
					  if(ss.lep2_motherID() == -1) Bs_mu = Bs_mu + weight;
					  if(ss.lep2_motherID() == -2 || ss.lep2_motherID() == 0) notBs_mu = notBs_mu + weight;
					}
				  Npn = Npn + (e2/(1-e2))*weight;
				  if (ss.lep2_motherID()==1) Npn_s = Npn_s + (e2/(1-e2))*weight;
				  Npn_histo_pred->Fill(sr, (e2/(1-e2))*weight);
				  Npn_histo_pred->Fill(br, (e2/(1-e2))*weight);
				}
			  else if( !ss.lep1_passes_id() && ss.lep2_passes_id() )   //lep1 is loose-not-tight, lep2 is tight
				{
				  if (noSIP && fabs(ss.lep1_ip3d()/ss.lep1_ip3d_err())>4.) continue;
				  if (unIso && ss.lep1_iso()<0.1) continue;
				  if (lowPtRel14 && ss.lep1_ptrel_v1()>14.) continue;
				  if (lowPtRel6  && ss.lep1_ptrel_v1()>6. ) continue;
				  if (useMiniIso && ss.lep1_ptrel_v1()<=6.) continue;
				  if (extrPtRel && ss.lep1_ptrel_v1()<6.0 ) continue;
				  float pt = ss.lep1_p4().pt();
				  if (coneCorr) {
				    pt = pt+pt*std::max(0.,ss.lep1_iso()-0.1);
				    if (useMiniIso) pt = pt+pt*std::max(0.,ss.lep1_miniIso()-0.05);
				    if (useNewMiniIso) {
				      if (ss.lep1_p4().pt()>25. && ss.lep1_ptrel_v1()>7.) 
					pt = pt+pt*(abs(ss.lep1_id())==11 ? std::max(0.,ss.lep1_miniIso()-0.075) : std::max(0.,ss.lep1_miniIso()-0.1));
				      else
					pt = (abs(ss.lep1_id())==11 ? ss.jet_close_lep1().pt()*0.725 : ss.jet_close_lep1().pt()*0.70);
				    }
				  }
				  if( abs(ss.lep1_id()) == 11 )	//if el, use el rate.  FILL WITH NONPROMPT			  
					{
					  e1 = getFakeRate(rate_histo_e, pt, fabs(ss.lep1_p4().eta()), ss.ht(), extrPtRel );
					  Npn_histo_pred_el->Fill(sr, (e1/(1-e1))*weight);
					  Npn_histo_pred_el->Fill(br, (e1/(1-e1))*weight);
					  if (sr>=0) Npn_histo_err2_pred_el[sr]->Fill(pt, fabs(ss.lep1_p4().eta()), weight);
					  Npn_histo_err2_pred_el[br]->Fill(pt, fabs(ss.lep1_p4().eta()), weight);
					  // //fill el abundance histos here w/ nbtags
					  if(ss.lep1_motherID() == -1) NBs_BR_histo_e->Fill(nbjets, weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
					  if(ss.lep1_motherID() == -2 || ss.lep1_motherID() == 0) NnotBs_BR_histo_e->Fill(nbjets, weight);
					  // if(ss.lep2_motherID() == -1) NBs_BR_histo_e->Fill(nbjets, weight); //USE ONLY FOR NUM ABUNDANCE PLOTS
					  // if(ss.lep2_motherID() == -2 || ss.lep2_motherID() == 0) NnotBs_BR_histo_e->Fill(nbjets, weight); //USE ONLY FOR NUM ABUNDANCE PLOTS
					  if(ss.lep1_motherID() == -1) Bs_e = Bs_e + weight;
					  if(ss.lep1_motherID() == -2 || ss.lep1_motherID() == 0) notBs_e = notBs_e + weight;
					}
				  else if( abs(ss.lep1_id()) == 13 ) //if mu, use mu rate.  FILL WITH NONPROMPT				  
					{
					  e1 = getFakeRate(rate_histo_mu, pt, fabs(ss.lep1_p4().eta()), ss.ht(), extrPtRel );
					  Npn_histo_pred_mu->Fill(sr, (e1/(1-e1))*weight);
					  Npn_histo_pred_mu->Fill(br, (e1/(1-e1))*weight);
					  if (sr>=0) Npn_histo_err2_pred_mu[sr]->Fill(pt, fabs(ss.lep1_p4().eta()), weight);
					  Npn_histo_err2_pred_mu[br]->Fill(pt, fabs(ss.lep1_p4().eta()), weight);
					  // //fill el abundance histos here w/ nbtags
					  if(ss.lep1_motherID() == -1) NBs_BR_histo_mu->Fill(nbjets, weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
					  if(ss.lep1_motherID() == -2 || ss.lep1_motherID() == 0) NnotBs_BR_histo_mu->Fill(nbjets, weight);
					  // if(ss.lep2_motherID() == -1) NBs_BR_histo_mu->Fill(nbjets, weight); //USE ONLY FOR NUM ABUNDANCE PLOTS
					  // if(ss.lep2_motherID() == -2 || ss.lep2_motherID() == 0) NnotBs_BR_histo_mu->Fill(nbjets, weight); //USE ONLY FOR NUM ABUNDANCE PLOTS
					  if(ss.lep1_motherID() == -1) Bs_mu = Bs_mu + weight;
					  if(ss.lep1_motherID() == -2 || ss.lep1_motherID() == 0) notBs_mu = notBs_mu + weight;
					}
				  Npn = Npn + (e1/(1-e1))*weight;
				  if (ss.lep1_motherID()==1) Npn_s = Npn_s + (e1/(1-e1))*weight;
				  Npn_histo_pred->Fill(sr, (e1/(1-e1))*weight);
				  Npn_histo_pred->Fill(br, (e1/(1-e1))*weight);
				}
			}
		} //end hyp = 2 if statement
	  //nonprompt-nonprompt background
	  else if( ss.hyp_class() == 1 )
		{
		  if( ss.lep1_id()*ss.lep2_id() > 0 )
			{
			  if( !ss.lep1_passes_id() && !ss.lep2_passes_id() )   //just making sure
			    {
				  if (noSIP && fabs(ss.lep1_ip3d()/ss.lep1_ip3d_err())>4.) continue;
				  if (noSIP && fabs(ss.lep2_ip3d()/ss.lep2_ip3d_err())>4.) continue;
				  float pt1 = ss.lep1_p4().pt();
				  if (coneCorr) {
				    pt1 = pt1+pt1*std::max(0.,ss.lep2_iso()-0.1);
				    if (useMiniIso) pt1 = pt1+pt1*std::max(0.,ss.lep2_miniIso()-0.05);
				    if (useNewMiniIso) {
				      if (ss.lep2_p4().pt()>25. && ss.lep2_ptrel_v1()>7.) 
					pt1 = pt1+pt1*(abs(ss.lep2_id())==11 ? std::max(0.,ss.lep2_miniIso()-0.075) : std::max(0.,ss.lep2_miniIso()-0.1));
				      else
					pt1 = (abs(ss.lep2_id())==11 ? ss.jet_close_lep2().pt()*0.725 : ss.jet_close_lep2().pt()*0.70);
				    }
				  }
				  float pt2 = ss.lep2_p4().pt();
				  if (coneCorr) {
				    pt2 = pt2+pt2*std::max(0.,ss.lep2_iso()-0.1);
				    if (useMiniIso) pt2 = pt2+pt2*std::max(0.,ss.lep2_miniIso()-0.05);
				    if (useNewMiniIso) {
				      if (ss.lep2_p4().pt()>25. && ss.lep2_ptrel_v1()>7.) 
					pt2 = pt2+pt2*(abs(ss.lep2_id())==11 ? std::max(0.,ss.lep2_miniIso()-0.075) : std::max(0.,ss.lep2_miniIso()-0.1));
				      else
					pt2 = (abs(ss.lep2_id())==11 ? ss.jet_close_lep2().pt()*0.725 : ss.jet_close_lep2().pt()*0.70);
				    }
				  }
				  if( abs(ss.lep2_id()) == 11 )
					{e2 = getFakeRate( rate_histo_e, pt2, fabs(ss.lep2_p4().eta()), ss.ht(), extrPtRel );}
				  else if( abs(ss.lep2_id()) == 13 )
					{e2 = getFakeRate( rate_histo_mu, pt2, fabs(ss.lep2_p4().eta()), ss.ht(), extrPtRel );}				
				  if( abs(ss.lep1_id()) == 11 )				  
					{e1 = getFakeRate( rate_histo_e, pt1, fabs(ss.lep1_p4().eta()), ss.ht(), extrPtRel );}
				  else if( abs(ss.lep1_id()) == 13 )				  
					{e1 = getFakeRate( rate_histo_mu, pt1, fabs(ss.lep1_p4().eta()), ss.ht(), extrPtRel );}
				  Nnn = Nnn + (e1/(1-e1))*(e2/(1-e2))*weight;					
			   }
			}
		} //end hyp = 1 if statement
	  
	  //---------------------------------------------------------------------------------------------------------------------------
	  
	}//end event loop
  
    // Clean Up
    delete tree;
    file->Close();
    delete file;
  }
  if ( nEventsChain != nEventsTotal ) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }

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
  //  TCanvas *c0=new TCanvas("c0","Fake Rate vs Pt, eta",800,800);
  //  rate_histo->Draw("colz,texte");
  TCanvas *c1=new TCanvas("c1","Fake Rate vs Pt, eta (electron)",800,800);
  rate_histo_e->Draw("colz,texte");
  TCanvas *c2=new TCanvas("c2","Fake Rate vs Pt, eta (muon)",800,800);
  rate_histo_mu->Draw("colz,texte");

  //Signal region plots
  //---------------------------------------------------------------------------------------
  TCanvas *c3=new TCanvas("c3","Predicted and Observed Prompt-NonPrompt Background", 800,800);
  TPad *pad_h3 = new TPad("pad_h3","Histo Pad3",0., 0.2, 1., 1.0);
  TPad *pad_r3 = new TPad("pad_r3","Ratio Pad3",0., 0.0, 1., 0.2);
  pad_h3->Draw();
  pad_r3->Draw();
  TLegend *leg3 = new TLegend(0.65, 0.70, 0.85, 0.85); //(0.78, 0.63, 0.87, 0.89)
  cout << "\ndump SR all" << endl;
  DrawPlots(Npn_histo_pred, Npn_histo_obs, Npn_histo_err2_pred_mu, Npn_histo_err2_pred_el, rate_histo_mu, rate_histo_e, c3, pad_h3, pad_r3, leg3);

  TH2D *nullarr[40] = {0};

  TCanvas *c4=new TCanvas("c4","Predicted and Observed Prompt-NonPrompt Background (Single mu)", 800,800);
  TPad *pad_h4 = new TPad("pad_h4","Histo Pad4",0., 0.2, 1., 1.0);
  TPad *pad_r4 = new TPad("pad_r4","Ratio Pad4",0., 0.0, 1., 0.2);
  pad_h4->Draw();
  pad_r4->Draw();
  TLegend *leg4 = new TLegend(0.65, 0.70, 0.85, 0.85); //(0.78, 0.63, 0.87, 0.89)
  cout << "\ndump SR mu" << endl;
  DrawPlots(Npn_histo_pred_mu, Npn_histo_obs_mu, Npn_histo_err2_pred_mu, nullarr, rate_histo_mu, rate_histo_e, c4, pad_h4, pad_r4, leg4);

  TCanvas *c5=new TCanvas("c5","Predicted and Observed Prompt-NonPrompt Background (Single el)", 800,800);
  TPad *pad_h5 = new TPad("pad_h5","Histo Pad5",0., 0.2, 1., 1.0);
  TPad *pad_r5 = new TPad("pad_r5","Ratio Pad5",0., 0.0, 1., 0.2);
  pad_h5->Draw();
  pad_r5->Draw();
  TLegend *leg5 = new TLegend(0.65, 0.70, 0.85, 0.85); //(0.78, 0.63, 0.87, 0.89)
  cout << "\ndump SR ele" << endl;
  DrawPlots(Npn_histo_pred_el, Npn_histo_obs_el, nullarr, Npn_histo_err2_pred_el, rate_histo_mu, rate_histo_e, c5, pad_h5, pad_r5, leg5);
  //---------------------------------------------------------------------------------

  TH1F *total_BR_histo_e = (TH1F*) NBs_BR_histo_e->Clone("total_BR_histo_e");
  TH1F *total_BR_histo_mu = (TH1F*) NBs_BR_histo_mu->Clone("total_BR_histo_mu");
  total_BR_histo_e->Add(NnotBs_BR_histo_e);
  total_BR_histo_mu->Add(NnotBs_BR_histo_mu);
  NBs_BR_histo_e->Divide(NBs_BR_histo_e, total_BR_histo_e,1,1,"B");
  NBs_BR_histo_mu->Divide(NBs_BR_histo_mu, total_BR_histo_mu,1,1,"B");
  NBs_BR_histo_e->GetXaxis()->SetTitle("Nbtags"); 
  NBs_BR_histo_e->GetYaxis()->SetTitle("Abundance");
  NBs_BR_histo_e->GetYaxis()->SetRangeUser(0.,1.);
  NBs_BR_histo_e->SetTitle("B Abundance vs Nbtags (Njets >= 2) (electrons)");
  NBs_BR_histo_mu->GetXaxis()->SetTitle("Nbtags"); 
  NBs_BR_histo_mu->GetYaxis()->SetTitle("Abundance");
  NBs_BR_histo_mu->GetYaxis()->SetRangeUser(0.,1.);
  NBs_BR_histo_mu->SetTitle("B Abundance vs Nbtags (Njets >= 2) (muons)");

  TCanvas *c6=new TCanvas("c6","B Abundance vs Nbtags (Njets >= 2) (electrons)", 800,800);
  NBs_BR_histo_e->Draw("histE");
  TCanvas *c7=new TCanvas("c7","B Abundance vs Nbtags (Njets >= 2) (muons)", 800,800);
  NBs_BR_histo_mu->Draw("histE");

  c3->SaveAs("sr_all"+option+".png");
  c4->SaveAs("sr_mu"+option+".png");
  c5->SaveAs("sr_el"+option+".png");

  TCanvas c8;
  pTrel_histo_mu->Draw();
  c8.SaveAs("pTrel_mu"+option+".png");
  pTrel_histo_el->Draw();
  c8.SaveAs("pTrel_el"+option+".png");
  c8.SetLogz();
  pTrelvsIso_histo_mu->Draw("colz");
  c8.SaveAs("pTrelvsIso_mu"+option+".png");
  pTrelvsIso_histo_el->Draw("colz");
  c8.SaveAs("pTrelvsIso_el"+option+".png");

  //benchmark stuff
  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}
