#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "../../classFiles/v4.00/SS.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../CORE/SSSelections.h"
#include "../../CORE/Tools/dorky/dorky.h"
#include "../../commonUtils.h"

//underflow/overflow
bool overflow = false;

using namespace ss;

float getFlipFactor(TH2D* rate){

  // //Figure out pt and eta
  // float pt1  = lep1_p4().pt();  
  // float eta1 = fabs(lep1_p4().eta()); 
  // float pt2  = lep2_p4().pt();  
  // float eta2 = fabs(lep2_p4().eta()); 

  // if (pt1<rate->GetXaxis()->GetBinCenter(1)) pt1=rate->GetXaxis()->GetBinCenter(1);
  // if (pt1>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt1=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
  // if (eta1<rate->GetYaxis()->GetBinCenter(1)) eta1=rate->GetYaxis()->GetBinCenter(1);
  // if (eta1>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta1=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());

  // if (pt2<rate->GetXaxis()->GetBinCenter(1)) pt2=rate->GetXaxis()->GetBinCenter(1);
  // if (pt2>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt2=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
  // if (eta2<rate->GetYaxis()->GetBinCenter(1)) eta2=rate->GetYaxis()->GetBinCenter(1);
  // if (eta2>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta2=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());

  // //Figure out flip rates
  // int bin1 = rate->FindBin(pt1, eta1);
  // int bin2 = rate->FindBin(pt2, eta2);
  // float FR1 = rate->GetBinContent(bin1);
  // float FR2 = rate->GetBinContent(bin2);

  float FR1 = flipRate(lep1_p4().pt(), lep1_p4().eta());
  float FR2 = flipRate(lep2_p4().pt(), lep2_p4().eta());

  //Return factor
  return (FR1/(1.-FR1) + FR2/(1.-FR2)); 

}

void closure(){

  //Load histo
  TFile *flip_file = new TFile("../derivation/flip_rate.root"); 
  TH2D  *rate = (TH2D*)flip_file->Get("flips"); 

  //Closure vs. inv mass plot
  constexpr int clos_mll_nBinsX = 9;
  TH1F* clos_mll_MC   = new TH1F("clos_mll_plot_MC"  , "clos_mll_plot_MC"  , clos_mll_nBinsX, 70, 115); 
  TH1F* clos_mll_MCp  = new TH1F("clos_mll_plot_MCp" , "clos_mll_plot_MCp" , clos_mll_nBinsX, 70, 115); 
  TH1F* clos_mll_data = new TH1F("clos_mll_plot_data", "clos_mll_plot_data", clos_mll_nBinsX, 70, 115); 
  TH1F* osee_data = new TH1F("osee_plot_data", "osee_plot_data", clos_mll_nBinsX, 70, 115); 
  //Sumw2
  clos_mll_MC->Sumw2();
  clos_mll_MCp->Sumw2();

  TH1F* clos_leppt_MC   = new TH1F("clos_leppt_plot_MC"  , "clos_leppt_plot_MC"  , 30, 0, 150); 
  TH1F* clos_leppt_MCp  = new TH1F("clos_leppt_plot_MCp" , "clos_leppt_plot_MCp" , 30, 0, 150); 
  TH1F* clos_leppt_data = new TH1F("clos_leppt_plot_data", "clos_leppt_plot_data", 30, 0, 150); 
  clos_leppt_MC->Sumw2();
  clos_leppt_MCp->Sumw2();

  TH1F* clos_lepeta_MC   = new TH1F("clos_lepeta_plot_MC"  , "clos_lepeta_plot_MC"  , 12, -3, 3); 
  TH1F* clos_lepeta_MCp  = new TH1F("clos_lepeta_plot_MCp" , "clos_lepeta_plot_MCp" , 12, -3, 3); 
  TH1F* clos_lepeta_data = new TH1F("clos_lepeta_plot_data", "clos_lepeta_plot_data", 12, -3, 3); 
  clos_lepeta_MC->Sumw2();
  clos_lepeta_MCp->Sumw2();

  TH1F* clos_lepphi_MC   = new TH1F("clos_lepphi_plot_MC"  , "clos_lepphi_plot_MC"  , 14, -4, 4); 
  TH1F* clos_lepphi_MCp  = new TH1F("clos_lepphi_plot_MCp" , "clos_lepphi_plot_MCp" , 14, -4, 4); 
  TH1F* clos_lepphi_data = new TH1F("clos_lepphi_plot_data", "clos_lepphi_plot_data", 14, -4, 4); 
  clos_lepphi_MC->Sumw2();
  clos_lepphi_MCp->Sumw2();

  TH1F* clos_ht_MC   = new TH1F("clos_ht_plot_MC"  , "clos_ht_plot_MC"  , 15, 0, 300); 
  TH1F* clos_ht_MCp  = new TH1F("clos_ht_plot_MCp" , "clos_ht_plot_MCp" , 15, 0, 300); 
  TH1F* clos_ht_data = new TH1F("clos_ht_plot_data", "clos_ht_plot_data", 15, 0, 300); 
  clos_ht_MC->Sumw2();
  clos_ht_MCp->Sumw2();

  //Errors -- keep track of number of events in each FR bin so we can get the error 
  int nBinsX = rate->GetNbinsX(); 
  int nBinsY = rate->GetNbinsY(); 
  const TArrayD* xbins = rate->GetXaxis()->GetXbins();
  const TArrayD* ybins = rate->GetYaxis()->GetXbins();
  //TH2D* errors  = new TH2D("errors" , "errors" , nBinsX, xbins, nBinsY, ybins);
  TH2D* errors[clos_mll_nBinsX] = { 0 };
  for (unsigned int i = 0; i < clos_mll_nBinsX; i++){
    errors[i] = new TH2D(Form("errors_%i", i) , "errors" , nBinsX, xbins->GetArray(), nBinsY, ybins->GetArray());
  }

  //Number duplicates rejected
  int reject = 0;

  //Results
  float nObs   = 0;
  float nObsMC = 0;
  float nPred  = 0;

  //Pred err variables
  float stat2 = 0; 
  float fr_err2 = 0; 

  //Set up chains
  TChain *chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"-data848p7ipb/DataDoubleEGD.root");
  //chain->Add("./Data_DoubleEG2015D.root");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/"+getTag()+"/DY_high.root");

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
      // if (!ss::passes_met_filters()) continue;

      //Throw away unneeded events
      if (hyp_class() != 3 && hyp_class() != 4) continue;

      //Keep only electron-electron events
      if (abs(lep1_id()) != 11 || abs(lep2_id()) != 11) continue;

      //Figure out if SS
      bool isSS = false;
      if (hyp_class() == 3) isSS = true;

      //Weight
      float weight = 1.0;

      //Observation
      if (isSS){
        float mll = (lep1_p4() + lep2_p4()).M();
	if (ss::is_real_data()) {
	  clos_mll_data->Fill(mll, weight); 
	  if (mll > 80 && mll < 100) {
	    nObs += weight; 
	    clos_leppt_data->Fill(lep1_p4().pt(), weight); 
	    clos_leppt_data->Fill(lep2_p4().pt(), weight); 
	    clos_lepeta_data->Fill(lep1_p4().eta(), weight); 
	    clos_lepeta_data->Fill(lep2_p4().eta(), weight); 
	    if (lep1_p4().eta()<-1.5) clos_lepphi_data->Fill(lep1_p4().phi(), weight); 
	    if (lep2_p4().eta()<-1.5) clos_lepphi_data->Fill(lep2_p4().phi(), weight); 
	    clos_ht_data->Fill(ss::ht_corr(), weight); 
	  }
	} else {
	  weight = scale1fb()*getLumi()*getPUw(ss::nGoodVertices());
	  clos_mll_MCp->Fill(mll, weight); 
	  if (mll > 80 && mll < 100) {
	    nObsMC += weight; 
	    clos_leppt_MCp->Fill(lep1_p4().pt(), weight); 
	    clos_leppt_MCp->Fill(lep2_p4().pt(), weight); 
	    clos_lepeta_MCp->Fill(lep1_p4().eta(), weight); 
	    clos_lepeta_MCp->Fill(lep2_p4().eta(), weight); 
	    if (lep1_p4().eta()<-1.5) clos_lepphi_MCp->Fill(lep1_p4().phi(), weight); 
	    if (lep2_p4().eta()<-1.5) clos_lepphi_MCp->Fill(lep2_p4().phi(), weight); 
	    clos_ht_MCp->Fill(ss::ht_corr(), weight); 
	  }
	}
      } 

      //Prediction
      if (!isSS && ss::is_real_data()){
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
	clos_mll_MC->Fill(mll, ff*weight); 
	osee_data->Fill(mll, weight);
	int bin_in_errors = clos_mll_MC->FindBin(mll);
	if (bin_in_errors >= clos_mll_nBinsX) overflow ? bin_in_errors = clos_mll_nBinsX-1 : bin_in_errors = 999; 
	if (bin_in_errors < 0) overflow ? bin_in_errors = 0 : bin_in_errors = 999;
	if (bin_in_errors != 999) errors[bin_in_errors]->Fill(pt1, eta1, weight);
	if (bin_in_errors != 999) errors[bin_in_errors]->Fill(pt2, eta2, weight);
        if (mll > 80 && mll < 100) {
	  nPred += ff*weight;
	  clos_leppt_MC->Fill(lep1_p4().pt(), ff*weight); 
	  clos_leppt_MC->Fill(lep2_p4().pt(), ff*weight); 
	  clos_lepeta_MC->Fill(lep1_p4().eta(), ff*weight); 
	  clos_lepeta_MC->Fill(lep2_p4().eta(), ff*weight); 
	  if (lep1_p4().eta()<-1.5) clos_lepphi_MC->Fill(lep1_p4().phi(), ff*weight); 
	  if (lep2_p4().eta()<-1.5) clos_lepphi_MC->Fill(lep2_p4().phi(), ff*weight); 
	  clos_ht_MC->Fill(ss::ht_corr(), ff*weight); 
	}
      }
      
    }//event loop
  }//file loop

  cout << "number of duplicates removed: " << reject << endl;

  for (int i = 1; i <= clos_mll_nBinsX; i++) stat2 += pow(clos_mll_MC->GetBinError(i), 2);  

  //Figure out error from "errors" plot:
  float theerror[clos_mll_nBinsX] = { 0 } ;
  for (int k = 0; k < clos_mll_nBinsX; k++){
    //cout << endl << clos_mll_MC->GetBinContent(k+1) << endl;
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
    //clos_mll_MC->SetBinError(k+1, sqrt(theerror[k] + pow(clos_mll_MC->GetBinError(k+1),2)));
    //cout << "set error = " <<  sqrt(theerror[k] + pow(clos_mll_MC->GetBinError(k+1),2)) << " yield=" << yield << endl;
  }

  cout << "obsMC: " << nObsMC << endl;
  cout << " pred: " << nPred  << " pm " << sqrt(stat2 + fr_err2) << endl;
  cout << "  obs: " << nObs   << " pm " << sqrt(nObs) << endl;

  //Split it
  cout << "pred stat: " << sqrt(stat2) << endl;
  cout << "pred syst: " << sqrt(fr_err2) << endl;

  //Make plot
  vector <TH1F*> bkgd;
  bkgd.push_back(clos_mll_MC); 
  vector <string> titles;
  vector <TH1F*> signals;
  signals.push_back(clos_mll_MCp); 
  vector <string> sigTit; 
  sigTit.push_back("MC Same-Sign Events");
  titles.push_back("Predicted Same-Sign Events"); 
  dataMCplotMaker(clos_mll_data, bkgd, titles, "", "", Form("--lumi %.1f --lumiUnit pb --outputName flip_closure.pdf --xAxisLabel M_{ll} --isLinear --noOverflow --legendRight -0.35 --legendWider 0.35 --outOfFrame --legendBox --legendUp 0.03 --sigError --largeLabels --yTitleOffset -0.2 --topYaxisTitle data/Pred", 1000*getLumi()), signals, sigTit);

  bkgd.clear();
  bkgd.push_back(clos_leppt_MC); 
  signals.clear();
  signals.push_back(clos_leppt_MCp); 
  dataMCplotMaker(clos_leppt_data, bkgd, titles, "", "", Form("--lumi %.1f --lumiUnit pb --outputName flip_closure_leppt.pdf --xAxisLabel Lepton p_{T}  --isLinear --noOverflow --legendRight -0.35 --legendWider 0.35 --outOfFrame --legendBox --legendUp 0.03 --sigError --largeLabels --yTitleOffset -0.2 --topYaxisTitle data/Pred", 1000*getLumi()), signals, sigTit);

  bkgd.clear();
  bkgd.push_back(clos_lepeta_MC); 
  signals.clear();
  signals.push_back(clos_lepeta_MCp); 
  dataMCplotMaker(clos_lepeta_data, bkgd, titles, "", "", Form("--lumi %.1f --lumiUnit pb --outputName flip_closure_lepeta.pdf --xAxisLabel Lepton #eta --noXaxisUnit  --isLinear --noOverflow --legendRight -0.35 --legendWider 0.35 --outOfFrame --legendBox --legendUp 0.03 --sigError --largeLabels --yTitleOffset -0.2 --topYaxisTitle data/Pred", 1000*getLumi()), signals, sigTit);

  bkgd.clear();
  bkgd.push_back(clos_lepphi_MC); 
  signals.clear();
  signals.push_back(clos_lepphi_MCp); 
  dataMCplotMaker(clos_lepphi_data, bkgd, titles, "", "", Form("--lumi %.1f --lumiUnit pb --outputName flip_closure_lepphi.pdf --xAxisLabel Lepton #phi --noXaxisUnit  --isLinear --noOverflow --legendRight -0.35 --legendWider 0.35 --outOfFrame --legendBox --legendUp 0.03 --sigError --largeLabels --yTitleOffset -0.2 --topYaxisTitle data/Pred", 1000*getLumi()), signals, sigTit);

  bkgd.clear();
  bkgd.push_back(clos_ht_MC); 
  signals.clear();
  signals.push_back(clos_ht_MCp); 
  dataMCplotMaker(clos_ht_data, bkgd, titles, "", "", Form("--lumi %.1f --lumiUnit pb --outputName flip_closure_ht.pdf --xAxisLabel HT  --isLinear --noOverflow --legendRight -0.35 --legendWider 0.35 --outOfFrame --legendBox --legendUp 0.03 --sigError --largeLabels --yTitleOffset -0.2 --topYaxisTitle data/Pred", 1000*getLumi()), signals, sigTit);

  TCanvas cosee;
  osee_data->Draw("hist,goff");
  cosee.SaveAs("osee_data.pdf");

}
