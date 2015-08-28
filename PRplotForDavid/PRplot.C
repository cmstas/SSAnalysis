#include "TFile.h"
#include "TChain.h"
#include "TH2D.h"
#include "SS.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../CORE/SSSelections.h"
#include "../CORE/Tools/dorky/dorky.h"

//underflow/overflow
bool overflow = false;

//lumi
float theLumi = 0.042;

using namespace ss;

void closure(){

  //Closure vs. inv mass plot
  TH1F* OS_MC   = new TH1F("OS_MC"  , "OS_MC"  , 35, 70, 140); 
  TH1F* OS_data = new TH1F("OS_data", "OS_data", 35, 70, 140); 
  TH1F* SS_MC   = new TH1F("SS_MC"  , "SS_MC"  , 35, 70, 140); 
  TH1F* SS_data = new TH1F("SS_data", "SS_data", 35, 70, 140); 

  //Set up chains
  TChain *chain = new TChain("t");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataDoubleEG2_0.root");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataMuonEG2_0.root");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataDoubleMuon2_0.root");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataDoubleEG_0.root");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataMuonEG_0.root");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataDoubleMuon_0.root");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DY_high_0.root");
  chain->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DY_low_0.root");

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Number duplicates rejected
  int reject = 0;

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //Declare these SFs
  float OS_data_ = 0;
  float SS_data_ = 0;
  float OS_MC_ = 0;
  float SS_MC_ = 0;
  float overall_data_ = 0;
  float overall_MC_ = 0;
  

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
        if (duplicate_removal::is_duplicate(id)){ reject++; continue; }
      }

      //MET filters
      if (!ss::passes_met_filters()) continue;

      //Throw away unneeded events
      if (hyp_class() != 3 && hyp_class() != 4) continue;

      //Keep only electron-electron events
      if (abs(lep1_id()) != 11 || abs(lep2_id()) != 11) continue;

      //Weight
      float weight = is_real_data() ? 1.0 : scale1fb()*theLumi;

      //Fill Plots
      float mll = (lep1_p4() + lep2_p4()).M();
  
      if (is_real_data() && hyp_class() == 4){
        OS_data->Fill(mll, weight); 
        if (mll > 70 && mll < 140) OS_data_ += weight;
        if (mll > 70 && mll < 140) overall_data_ += weight;
      }
      else if (is_real_data() && hyp_class() == 3){
        SS_data->Fill(mll, weight); 
        if (mll > 70 && mll < 140) SS_data_ += weight;
        if (mll > 70 && mll < 140) overall_data_ += weight;
      }
      else if (!is_real_data() && hyp_class() == 4){
        OS_MC->Fill(mll, weight); 
        if (mll > 70 && mll < 140) OS_MC_ += weight;
        if (mll > 70 && mll < 140) overall_MC_ += weight;
      }
      else if (!is_real_data() && hyp_class() == 3){
        SS_MC->Fill(mll, weight); 
        if (mll > 70 && mll < 140) SS_MC_ += weight;
        if (mll > 70 && mll < 140) overall_MC_ += weight;
      }

    }//event loop
  }//file loop

  //Determine SFs
  float SS_SF = SS_data_/SS_MC_;
  float OS_SF = OS_data_/OS_MC_;
  float overall_SF = overall_data_/overall_MC_;

  //Apply SFs
  OS_MC->Scale(overall_SF); 
  SS_MC->Scale(overall_SF); 

  //SFs
  cout << "The OS SF is " << OS_SF << endl;
  cout << "The SS SF is " << SS_SF << endl;
  cout << "The overall SF is " << overall_SF << endl;

  //Make plot
  vector <TH1F*> bkgd;
  bkgd.push_back(OS_MC); 
  bkgd.push_back(SS_MC); 
  vector <TH1F*> signals;
  signals.push_back(OS_data); 
  signals.push_back(SS_data); 
  
  vector <string> titles;
  titles.push_back("Z+jets OS"); 
  titles.push_back("Z+jets SS"); 
 
  vector <string> sigTit;
  sigTit.push_back("data OS"); 
  sigTit.push_back("data SS"); 

  TH1F* null = new TH1F("","",1,0,1);

  std::vector <Color_t> Colors;
  Colors.push_back(kAzure+7);
  Colors.push_back(kGreen+3);
  Colors.push_back(kBlack);
  Colors.push_back(kBlack);
  Colors.push_back(kCyan-4);
  Colors.push_back(kBlue-10);   
  Colors.push_back(kViolet+4);
  Colors.push_back(kYellow-4); 
  
  dataMCplotMaker(null, bkgd, titles, "", "", Form("--lumi 42 --lumiUnit pb --outputName DiElectronMass.pdf --xAxisLabel Uncalibrated Di-electron Mass --noOverflow --sigError --setMinimum .1 --blackSignals --legendBox --setMaximum 10000 --outOfFrame --markerStyle2 --legendTextSize 0.045 --legendWider 0.03 --legendRight -0.02 --largeLabels"), signals, sigTit); 

  cout << "number of duplicates removed: " << reject << endl;

}
