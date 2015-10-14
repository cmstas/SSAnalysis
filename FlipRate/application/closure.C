#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "SS.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../CORE/SSSelections.h"
#include "../../CORE/Tools/dorky/dorky.h"
#include "../../commonUtils.h"

//underflow/overflow
bool overflow = false;

using namespace ss;

float getFlipFactor(TH2D* rate){

  //Figure out pt and eta
  float pt1  = lep1_p4().pt();  
  float eta1 = fabs(lep1_p4().eta()); 
  float pt2  = lep2_p4().pt();  
  float eta2 = fabs(lep2_p4().eta()); 

  if (pt1<rate->GetXaxis()->GetBinCenter(1)) pt1=rate->GetXaxis()->GetBinCenter(1);
  if (pt1>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt1=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
  if (eta1<rate->GetYaxis()->GetBinCenter(1)) eta1=rate->GetYaxis()->GetBinCenter(1);
  if (eta1>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta1=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());

  if (pt2<rate->GetXaxis()->GetBinCenter(1)) pt2=rate->GetXaxis()->GetBinCenter(1);
  if (pt2>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt2=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
  if (eta2<rate->GetYaxis()->GetBinCenter(1)) eta2=rate->GetYaxis()->GetBinCenter(1);
  if (eta2>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta2=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());

  //Figure out flip rates
  int bin1 = rate->FindBin(pt1, eta1);
  int bin2 = rate->FindBin(pt2, eta2);
  float FR1 = rate->GetBinContent(bin1);
  float FR2 = rate->GetBinContent(bin2);

  //Return factor
  return (FR1/(1.-FR1) + FR2/(1.-FR2)); 

}

void closure(){

  //Load histo
  TFile *flip_file = new TFile("../derivation/flip_rate.root"); 
  TH2D  *rate = (TH2D*)flip_file->Get("flips"); 

  //Closure vs. inv mass plot
  constexpr int clos_nBinsX = 5;
  // TH1F* clos_MC   = new TH1F("clos_plot_MC"  , "clos_plot_MC"  , 9, 70, 115); 
  // TH1F* clos_data = new TH1F("clos_plot_data", "clos_plot_data", 9, 70, 115); 
  TH1F* clos_MC   = new TH1F("clos_plot_MC"  , "clos_plot_MC"  , clos_nBinsX, 70, 120); 
  TH1F* clos_data = new TH1F("clos_plot_data", "clos_plot_data", clos_nBinsX, 70, 120); 
  TH1F* osee_data = new TH1F("osee_plot_data", "osee_plot_data", clos_nBinsX, 70, 120); 
  //Sumw2
  clos_MC->Sumw2();

  //Errors -- keep track of number of events in each FR bin so we can get the error 
  int nBinsX = rate->GetNbinsX(); 
  int nBinsY = rate->GetNbinsY(); 
  const TArrayD* xbins = rate->GetXaxis()->GetXbins();
  const TArrayD* ybins = rate->GetYaxis()->GetXbins();
  //TH2D* errors  = new TH2D("errors" , "errors" , nBinsX, xbins, nBinsY, ybins);
  TH2D* errors[clos_nBinsX] = { 0 };
  for (unsigned int i = 0; i < clos_nBinsX; i++){
    errors[i] = new TH2D(Form("errors_%i", i) , "errors" , nBinsX, xbins->GetArray(), nBinsY, ybins->GetArray());
  }

  //Number duplicates rejected
  int reject = 0;

  //Results
  float nObs  = 0;
  float nPred = 0;

  //Pred err variables
  float stat2 = 0; 
  float fr_err2 = 0; 

  //Set up chains
  TChain *chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"/DataDoubleEGD.root");
  //chain->Add("./Data_DoubleEG2015D.root");

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      samesign.GetEntry(event);
      nEventsTotal++;
    
      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //Reject not triggered
      if (!fired_trigger()) continue;

      //Reject duplicates
      if (ss::is_real_data()){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)) { reject++; continue; }
      }

      //MET filters
      //if (!ss::passes_met_filters()) continue;

      //Throw away unneeded events
      if (hyp_class() != 3 && hyp_class() != 4) continue;

      //Hyp-6 events don't necessarily pass ID (in v1.26, to be fixed); impose that here
      //if (!lep1_passes_id() || !lep2_passes_id()) continue;

      //Keep only electron-electron events
      if (abs(lep1_id()) != 11 || abs(lep2_id()) != 11) continue;

      //Figure out if SS
      bool isSS = false;
      //if (sgn(lep1_id()) == sgn(lep2_id())) isSS = true;
      if (hyp_class() == 3) isSS = true;

      //Weight
      float weight = 1.0; //scale1fb()*theLumi;

      //Observation
      if (isSS){
        float mll = (lep1_p4() + lep2_p4()).M();
        if (mll > 80 && mll < 100) nObs += weight; 
        clos_data->Fill(mll, weight); 
      } 

      //Prediction
      if (!isSS){
	float pt1  = lep1_p4().pt();  
	float eta1 = fabs(lep1_p4().eta()); 
	float pt2  = lep2_p4().pt();  
	float eta2 = fabs(lep2_p4().eta()); 
	if (pt1<rate->GetXaxis()->GetBinCenter(1)) pt1=rate->GetXaxis()->GetBinCenter(1);
	if (pt1>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt1=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
	if (eta1<rate->GetYaxis()->GetBinCenter(1)) eta1=rate->GetYaxis()->GetBinCenter(1);
	if (eta1>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta1=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());	  
	if (pt2<rate->GetXaxis()->GetBinCenter(1)) pt2=rate->GetXaxis()->GetBinCenter(1);
	if (pt2>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt2=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
	if (eta2<rate->GetYaxis()->GetBinCenter(1)) eta2=rate->GetYaxis()->GetBinCenter(1);
	if (eta2>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta2=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());
	
	float ff = getFlipFactor(rate);
	float mll = (lep1_p4() + lep2_p4()).M();
        if (mll > 80 && mll < 100) nPred += ff*weight;
	clos_MC->Fill(mll, ff*weight); 
	osee_data->Fill(mll, weight);
	int bin_in_errors = clos_MC->FindBin(mll);
	if (bin_in_errors >= clos_nBinsX) overflow ? bin_in_errors = clos_nBinsX-1 : bin_in_errors = 999; 
	if (bin_in_errors < 0) overflow ? bin_in_errors = 0 : bin_in_errors = 999;
	if (bin_in_errors != 999) errors[bin_in_errors]->Fill(pt1, eta1, weight);
	if (bin_in_errors != 999) errors[bin_in_errors]->Fill(pt2, eta2, weight);
      }
      
    }//event loop
  }//file loop

  cout << "number of duplicates removed: " << reject << endl;

  for (int i = 1; i <= clos_nBinsX; i++) stat2 += pow(clos_MC->GetBinError(i), 2);  

  //Figure out error from "errors" plot:
  float theerror[clos_nBinsX] = { 0 } ;
  for (int k = 0; k < clos_nBinsX; k++){
    //cout << endl << clos_MC->GetBinContent(k+1) << endl;
    float yield = 0.;
    float error = 0; 
    for (int i = 1; i <= errors[0]->GetNbinsX(); i++){
      for (int j = 1; j <= errors[0]->GetNbinsY(); j++){
        float FR_val = rate->GetBinContent(i, j);
        float FR_err = rate->GetBinError(i, j);
        float nEntries = errors[k]->GetBinContent(i, j); 
        error = FR_err*nEntries/pow(1-FR_val,2);
	//cout << "FR_err[%]=" << 100*FR_err/FR_val << " FR_val=" << FR_val << " nEntries=" << nEntries << endl;
        theerror[k] += pow(error, 2);
	//cout << nEntries*FR_val/(1.-FR_val) << " +/- " << error << endl;
	yield+=(nEntries*FR_val/(1.-FR_val));
        fr_err2 += pow(error, 2); 
      }
    }
    //clos_MC->SetBinError(k+1, sqrt(theerror[k] + pow(clos_MC->GetBinError(k+1),2)));
    //cout << "set error = " <<  sqrt(theerror[k] + pow(clos_MC->GetBinError(k+1),2)) << " yield=" << yield << endl;
  }

  cout << "pred: " << nPred << " pm " << sqrt(stat2 + fr_err2) << endl;
  cout << " obs: " << nObs << " pm " << sqrt(nObs) << endl;

  //Split it
  cout << "pred stat: " << sqrt(stat2) << endl;
  cout << "pred syst: " << sqrt(fr_err2) << endl;

  //Make plot
  TH1F* null = new TH1F("","",1,0,1);
  vector <TH1F*> bkgd;
  bkgd.push_back(clos_MC); 
  vector <string> titles;
  vector <TH1F*> signals;
  signals.push_back(clos_data); 
  vector <string> sigTit; 
  sigTit.push_back("Observed Same-Sign Events");
  titles.push_back("Predicted Same-Sign Events"); 
  dataMCplotMaker(null, bkgd, titles, "", "", "--lumi 209.5 --lumiUnit pb --outputName flip_closure.pdf --xAxisLabel M_{ll} --blackSignals  --isLinear --noOverflow --legendRight -0.35 --legendWider 0.35 --outOfFrame --legendBox --legendUp 0.03 --sigError --largeLabels --yTitleOffset -0.2", signals, sigTit); //--setMaximum 10 

  TCanvas cosee;
  osee_data->Draw("hist,goff");
  cosee.SaveAs("osee_data.pdf");

}
