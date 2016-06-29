#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "../../CORE/Tools/dorky/dorky.h"
#include "../../CORE/SSSelections.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../commonUtils.h"
#include "../../classFiles/v4.04/SS.h"

vector <vector <TH1F*> >  makeAllPlots(vector <TChain*> chains, bool isOS = 0);

TString tag = getTag(); 

vector <vector <TH1F*> > flipVectors(vector <vector <TH1F*> > vin){
  int nVectors = vin.size(); 
  int nInEachVector = 0;
  if (nVectors > 0) nInEachVector = vin[0].size(); 
  vector <vector <TH1F*> > vout; 
  for (int i = 0; i < nInEachVector; i++){
    vector <TH1F*> temp;    
    for (int j = 0; j < nVectors; j++){
      temp.push_back(vin[j][i]);  
    }
    vout.push_back(temp); 
  }
  return vout;
}

void kinem(){

  //Make Run2 chains
  TChain *ttbar = new TChain("t");
  TChain *ttv   = new TChain("t");
  TChain *dy    = new TChain("t");
  TChain *wjets = new TChain("t");
  TChain *wz    = new TChain("t");
  TChain *qqww  = new TChain("t");
  TChain *data  = new TChain("t"); 
  //TChain *t1tttt     = new TChain("t");

  //Fill Run2 chains
  ttbar ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR.root"                , tag.Data()));
  ttv   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"                 , tag.Data()));
  ttv   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZQ.root"                 , tag.Data()));
  ttv   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"                  , tag.Data()));
  ttv   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTWQQ.root"                , tag.Data()));
  dy    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_high.root"              , tag.Data()));
  wjets ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets.root"                , tag.Data()));
  wz    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ3LNU.root"               , tag.Data()));
  qqww  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"                 , tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGC_05oct.root"  , tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonC_05oct.root", tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGC_05oct.root"    , tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_05oct.root"  , tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_05oct.root", tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_05oct.root"    , tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_v4.root"     , tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_v4.root"       , tag.Data()));
  data  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_v4.root"   , tag.Data()));

  //Make vector of these TChains
  vector <TChain*> run2   = {dy, ttbar, wjets, ttv, wz, qqww}; 
  //vector <TChain*> sig    = {t1tttt };
  vector <TChain*> data2  = {data}; 

  //Make plots
  vector <vector <TH1F*> > run2_plots = makeAllPlots(run2); 
  vector <vector <TH1F*> > data_plots = makeAllPlots(data2); 

  //Flip vector of vectors
  vector <vector <TH1F*> > bkgd_plots2 = flipVectors(run2_plots);
  vector <vector <TH1F*> > data_plots2 = flipVectors(data_plots);

  //Prepare for output
  vector <string> titles = { "DY", "TTBAR", "WJets", "TTV", "WZ", "QQWW" };
  vector <string> plot_title = { "Lep pT", "M_{ll}", "MET", "H_{T}", "M_{T} min", "nJets", "nBtags", "jet p_{T}", "MET 3.0" };

  //Options string.  
  vector <string> options; 
  options.push_back("--xAxisLabel lep p_{T}        --outputName pdfs/lep_pt"        );
  options.push_back("--xAxisLabel M_{ll}           --outputName pdfs/mll"           );
  options.push_back("--xAxisLabel MET              --outputName pdfs/met"           );
  options.push_back("--xAxisLabel H_{T}            --outputName pdfs/ht"            );
  options.push_back("--xAxisLabel M_{T} min        --outputName pdfs/mtmin"         );
  options.push_back("--xAxisLabel nJets            --outputName pdfs/njets"         );
  options.push_back("--xAxisLabel nBtags           --outputName pdfs/nbtags"        );
  options.push_back("--xAxisLabel jet p_{T}        --outputName pdfs/jetpt"         );
  options.push_back("--xAxisLabel MET 3.0          --outputName pdfs/met3p0"        );

  //Make final plot
  for (unsigned int j = 0; j < data_plots[0].size(); j++){
    dataMCplotMaker(data_plots[0][j], bkgd_plots2[j], titles, plot_title[j], "SS Baseline", Form("--legendRight -0.05 %s --lumi %f --setMinimum 0.2 --legendUp -0.07 --legendRight 0.03", options[j].c_str(), getLumi())); 
  }

}

vector <vector <TH1F*> >  makeAllPlots(vector <TChain*> chains, bool isOS){
  
  //Initialize result
  vector <vector <TH1F*> > result;

  //Loop over chains, make plots
  for (unsigned int iChain = 0; iChain < chains.size(); iChain++){

    //For each chain, need vector of plots
    vector <TH1F*> plots; 

    //Get chain
    TChain *chain = chains[iChain];

    //Create plot we want
    TH1F* plot_leppt        = new TH1F(Form("plot_leppt_%i"       , iChain), "plot", 25, 0, 500);
    TH1F* plot_mll          = new TH1F(Form("plot_mll_%i"         , iChain), "plot", 25, 0, 500);
    TH1F* plot_met          = new TH1F(Form("plot_met_%i"         , iChain), "plot", 25, 0, 500);
    TH1F* plot_ht           = new TH1F(Form("plot_ht_%i"          , iChain), "plot", 20, 0, 800);
    TH1F* plot_mtmin        = new TH1F(Form("plot_mtmin_%i"       , iChain), "plot", 40, 0, 400);
    TH1F* plot_njets        = new TH1F(Form("plot_njets_%i"       , iChain), "plot", 8 , 0,   8);
    TH1F* plot_nbtags       = new TH1F(Form("plot_nbtags_%i"      , iChain), "plot", 8 , 0,   8);
    TH1F* plot_jetpt        = new TH1F(Form("plot_jetpt_%i"       , iChain), "plot", 25, 0, 500);
    TH1F* plot_met3p0       = new TH1F(Form("plot_met3p0_%i"      , iChain), "plot", 25, 0, 500);

    //nEvents in chain
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

        //Set the lumi 
        float lumiAG = ss::is_real_data() ? 1.0 : getLumi();

        //Only SS events
        if (ss::hyp_class() != 3) continue; 

        //Triggers
        if (!ss::fired_trigger()) continue; 
        if (!ss::passedFilterList()) continue; 

        //Reject duplicates
        if (ss::is_real_data()){
          duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
          if (duplicate_removal::is_duplicate(id)) continue;
        }  
  
        //Require baseline region 
        int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
        if (BR < 0) continue;

        //Calculate the variables for plots
        float mll = (ss::lep1_p4() + ss::lep2_p4()).M();

        //Fill jet pT plot
        for (unsigned int i = 0; i < ss::jet_pt().size(); i++){
          plot_jetpt->Fill(ss::jet_pt().at(i), ss::scale1fb()*lumiAG);
        }

        //Fill other plots
        if (ss::is_real_data() && mll < 20) cout << ss::event() << endl;
        plot_leppt        ->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumiAG);
        plot_leppt        ->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumiAG);
        plot_mll          ->Fill(mll               , ss::scale1fb()*lumiAG);
        plot_met          ->Fill(ss::met()         , ss::scale1fb()*lumiAG);
        plot_ht           ->Fill(ss::ht()          , ss::scale1fb()*lumiAG);
        plot_mtmin        ->Fill(ss::mtmin()       , ss::scale1fb()*lumiAG);
        plot_njets        ->Fill(ss::njets()       , ss::scale1fb()*lumiAG);
        plot_nbtags       ->Fill(ss::nbtags()      , ss::scale1fb()*lumiAG);
        plot_met3p0       ->Fill(ss::met3p0()      , ss::scale1fb()*lumiAG);

      }//event loop
    }//file loop
   
    //Keep plots
    plots.push_back(plot_leppt       ); 
    plots.push_back(plot_mll         ); 
    plots.push_back(plot_met         ); 
    plots.push_back(plot_ht          ); 
    plots.push_back(plot_mtmin       ); 
    plots.push_back(plot_njets       ); 
    plots.push_back(plot_nbtags      ); 
    plots.push_back(plot_jetpt       ); 
    plots.push_back(plot_met3p0      ); 

    //Add plots from first chain to result
    result.push_back(plots); 

  }//chain loop

  //Return result
  return result;
}
