#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "SS.h"

TH1F* makeInvtMassPlot(TChain* chain, string name);

void compare_74x_75x(){

  //Load up 74x babies
  TChain *chain_74x = new TChain("t");
  chain_74x->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataDoubleEG2_0.root");
  chain_74x->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataDoubleMuon2_0.root");
  chain_74x->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataDoubleEG_0.root");
  chain_74x->Add("/nfs-7/userdata/ss2015/ssBabies/v2.11/DataDoubleMuon_0.root");

  //Load up 75x babies
  TChain *chain_75x = new TChain("t");
  chain_75x->Add("/nfs-7/userdata/ss2015/ssBabies/v3.01/dataDoubleEGandDoubleMu05Aug.root");

  //Make invariant mass plots
  cout << "Running the first!" << endl;
  TH1F* plot_invtmass_74x = makeInvtMassPlot(chain_74x, "74x"); 
  cout << "Running the second!" << endl;
  TH1F* plot_invtmass_75x = makeInvtMassPlot(chain_75x, "75x"); 

  //Prepare the beautiful plot
  vector <TH1F*> backgrounds;
  backgrounds.push_back(plot_invtmass_74x); 
  vector <string> titles;
  titles.push_back("74X"); 
  
  //Make the beautiful plot
  dataMCplotMaker(plot_invtmass_75x, backgrounds, titles, "SS mass peak", "75X vs 74X rereco", "--outputName invtMass_74x_75x --isLinear --dataName 75X --xAxisLabel M_{ll} --overrideHeader (13 TeV)"); 

}

TH1F* makeInvtMassPlot(TChain* chain, string name){

  //Make plot
  TH1F* plot = new TH1F(Form("plot_%s", name.c_str()), "plot", 10, 0, 250); 

  //nEvents in chain
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //Clear duplicate list
  duplicate_removal::clear_list();  

  //Nentries
  int nEntries_74x = 0;
  int nEntries_75x = 0;

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
      if (!ss::fired_trigger()) continue;

      //Consider only the two runs
      if (ss::run() != 251643 && ss::run() != 251721) continue;
    
      //Reject duplicates
	  if (ss::is_real_data()){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)) continue; 
      }

      //MET filters
      if (!ss::passes_met_filters()) continue;

      //Throw away unneeded events
      if (ss::hyp_class() != 3 && ss::hyp_class() != 2 && ss::hyp_class() != 6) continue;

      //Keep only electron-electron and muon-muon events
      if (ss::hyp_type() != 0 && ss::hyp_type() != 3) continue;

      //calculate and plot invariant mass
      float mll = (ss::lep1_p4() + ss::lep2_p4()).M();
      plot->Fill(mll); 

      if (name == "74x") nEntries_74x++;
      if (name == "75x") nEntries_75x++;
      cout << name << " " << ss::event() << " " << mll << endl;

    }//event loop
  }//file loop

  if (name == "74x") cout << "nEntries 74X: " << nEntries_74x << endl;
  if (name == "75x") cout << "nEntries 75X: " << nEntries_75x << endl;

  //Return the plot
  return plot;

}
