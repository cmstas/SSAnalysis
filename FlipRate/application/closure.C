#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "SS.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"

//lumi
float theLumi = 1.0;

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
  //TH1F* clos_MC   = new TH1F("clos_plot_MC"  , "clos_plot_MC"  , 9, 70, 115); 
  //TH1F* clos_data = new TH1F("clos_plot_data", "clos_plot_data", 9, 70, 115); 
  TH1F* clos_MC   = new TH1F("clos_plot_MC"  , "clos_plot_MC"  , 17, 30, 115); 
  TH1F* clos_data = new TH1F("clos_plot_data", "clos_plot_data", 17, 30, 115); 

  float nObs  = 0;
  float nPred = 0;

  //Set up chains
  TChain *chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/DY1_0.root"); 
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/DY2_0.root"); 
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/DY3_0.root"); 
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v1.26/DY4_0.root"); 

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

      //Throw away unneeded events
      if (hyp_class() != 3 && hyp_class() != 4) continue;

      //Weight
      float weight = scale1fb()*theLumi;

      //Observation
      if (hyp_class() == 3){
        nObs += weight; 
        clos_data->Fill( (lep1_p4() + lep2_p4()).M(), weight); 
      }

      //Prediction
      if (hyp_class() == 4){
        float ff = getFlipFactor(rate);
        nPred += ff*weight;
        clos_MC->Fill( (lep1_p4() + lep2_p4()).M(), ff*weight); 
      }

    }//event loop
  }//file loop

  cout << "pred: " << nPred << endl;
  cout << " obs: " << nObs << endl;

  //Make plot
  vector <TH1F*> bkgd;
  bkgd.push_back(clos_MC); 
  vector <string> titles;
  titles.push_back("pred"); 
  dataMCplotMaker(clos_data, bkgd, titles, "flip closure", "", "--outputName flip_closure.pdf --xAxisLabel M_{ll} --lumi 1 --dataName obs --topYaxisTitle obs/pred --isLinear --noFill"); 
  cout << bkgd[0]->Integral() << endl;
  cout << clos_data->Integral() << endl;
  for (unsigned int i = 0; i < clos_data->GetNbinsX()+1; i++) cout << i << " " << clos_data->GetBinContent(i) << endl;

}
