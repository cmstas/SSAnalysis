#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "SS.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../CORE/SSSelections.h"

//truth-matching
bool truthMatching = false;

//cutting
bool cutting = false;

//underflow/overflow
bool overflow = false;

//lumi
float theLumi = 0.04003;

using namespace ss;

float getFlipFactor(TH2D* rate){

  //Figure out pt and eta
  float pt1  = lep1_p4().pt();  
  float eta1 = abs(lep1_p4().eta()); 
  float pt2  = lep2_p4().pt();  
  float eta2 = abs(lep2_p4().eta()); 

  //Figure out flip rates
  int bin1 = rate->FindBin(pt1, eta1);
  int bin2 = rate->FindBin(pt2, eta2);
  float FR1 = rate->GetBinContent(bin1);
  float FR2 = rate->GetBinContent(bin2);

  //Return factor
  return (FR1/(1-FR1) + FR2/(1-FR2)); 

}

void closure(){

  //Load histo
  TFile *flip_file = new TFile("../derivation/flip_rate.root"); 
  TH2D  *rate = (TH2D*)flip_file->Get("flips"); 

  //Closure vs. inv mass plot
  TH1F* clos_MC   = new TH1F("clos_plot_MC"  , "clos_plot_MC"  , 9, 70, 115); 
  TH1F* clos_data = new TH1F("clos_plot_data", "clos_plot_data", 9, 70, 115); 

  //Errors -- keep track of number of events in each FR bin so we can get the error 
  int nBinsX = 5; 
  int nBinsY = 3; 
  float xbins[] = { 0, 20, 40, 60, 80, 100 }; 
  float ybins[] = { 0, 1, 2, 3 }; 
  //TH2D* errors  = new TH2D("errors" , "errors" , nBinsX, xbins, nBinsY, ybins);
  TH2D* errors[9] = { 0 };
  for (unsigned int i = 0; i < 9; i++){
    errors[i] = new TH2D(Form("errors_%i", i) , "errors" , nBinsX, xbins, nBinsY, ybins);
  }

  //Results
  float nObs  = 0;
  float nPred = 0;

  //Pred err variables
  float stat2 = 0; 
  float fr_err2 = 0; 

  //Set up chains
  TChain *chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.08/DataDoubleEG_0.root");

  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/dy_0.root"); 
  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/DY1_0.root"); 
  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/DY2_0.root"); 
  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/DY3_0.root"); 
  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/DY4_0.root"); 

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

      //Not in int lumi
      if (run() == 251864) continue;

      //Throw away unneeded events
      if (hyp_class() != 3 && hyp_class() != 4 && hyp_class() != 6) continue;

      //Hyp-6 events don't necessarily pass ID (in v1.26, to be fixed); impose that here
      if (!lep1_passes_id() || !lep2_passes_id()) continue;

      //Keep only electron-electron events
      if (abs(lep1_id()) != 11 || abs(lep2_id()) != 11) continue;

      //Cutting
      if (cutting){
        if (met() > 100) continue; 
        if (mt() > 100) continue; 
      }

      //Figure out if SS
      bool isSS = false;
      if (sgn(lep1_id()) == sgn(lep2_id())) isSS = true;

      //Truth-Matching
      if (truthMatching){
        if (!isSS &&  (lep1_motherID() != 1 || lep2_motherID() != 1)) continue;
        if ( isSS && !((lep1_motherID() == 1 && lep2_motherID() == 2) || (lep1_motherID() == 2 && lep2_motherID() == 1))) continue;
      }

      //Weight
      float weight = 1.0; //scale1fb()*theLumi;

      //Observation
      if (isSS){
        nObs += weight; 
        clos_data->Fill( (lep1_p4() + lep2_p4()).M(), weight); 
      }

      //Prediction
      if (!isSS){
        float ff = getFlipFactor(rate);
        nPred += ff*weight;
        float mll = (lep1_p4() + lep2_p4()).M();
        clos_MC->Fill(mll, ff*weight); 
        int bin_in_errors = (mll-70)/5.0;
        if (bin_in_errors > 8) overflow ? bin_in_errors = 8 : bin_in_errors = 999; 
        if (bin_in_errors < 0) overflow ? bin_in_errors = 0 : bin_in_errors = 999;
        if (bin_in_errors != 999) errors[bin_in_errors]->Fill(lep1_p4().pt(), fabs(lep1_p4().eta()), weight);
        if (bin_in_errors != 999) errors[bin_in_errors]->Fill(lep2_p4().pt(), fabs(lep2_p4().eta()), weight);
        stat2 += pow(ff*weight, 2); 
      }

    }//event loop
  }//file loop

  //Figure out error from "errors" plot:
  float theerror[9] = { 0 } ;
  for (int k = 0; k < 9; k++){
    float error = 0; 
    for (int i = 1; i <= errors[0]->GetNbinsX()+1; i++){
      for (int j = 1; j <= errors[0]->GetNbinsY(); j++){
        float FR_val = rate->GetBinContent(i, j);
        float FR_err = rate->GetBinError(i, j);
        float nEntries = errors[k]->GetBinContent(i, j); 
        error = FR_err*nEntries/pow(1-FR_val,2);
        theerror[k] += pow(error, 2);
        fr_err2 += pow(error, 2); 
      }
    }
    clos_MC->SetBinError(k+1, sqrt(theerror[k] + pow(clos_MC->GetBinError(k+1),2)));
  }

  cout << "pred: " << nPred << " pm " << sqrt(stat2 + fr_err2) << endl;
  cout << " obs: " << nObs << " pm " << sqrt(nObs) << endl;


  //Make plot
  vector <TH1F*> bkgd;
  bkgd.push_back(clos_MC); 
  vector <string> titles;
  titles.push_back("pred"); 
  dataMCplotMaker(clos_data, bkgd, titles, "flip closure", "", Form("--lumi 40.03 --lumiUnit pb --outputName flip_closure%s.pdf --xAxisLabel M_{ll} --dataName obs --topYaxisTitle obs/pred --isLinear --noFill --histoErrors %s", truthMatching ? "_TM" : "", "--noOverflow")); 

}
