#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "../../CORE/SSSelections.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "SS.h"

vector <vector <TH1F*> >  makeAllPlots(vector <TChain*> chains, bool isOS = 0);

float lumiAG = 2.5;

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
  TChain *ttbar_run2 = new TChain("t");
  TChain *ttz_run2   = new TChain("t");
  TChain *ttw_run2   = new TChain("t");
  TChain *dy_run2    = new TChain("t");
  TChain *wjets_run2 = new TChain("t");
  TChain *wz_run2    = new TChain("t");
  TChain *t1tttt     = new TChain("t");

  //Fill Run2 chains
  ttbar_run2->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/TTBAR.root");
  ttz_run2  ->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/TTZL.root"); 
  ttz_run2  ->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/TTZQ.root"); 
  ttw_run2  ->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/TTW.root"); 
  ttw_run2  ->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/TTWQQ.root"); 
  dy_run2   ->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/DY_high.root"); 
  wjets_run2->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/WJets.root");   
  wz_run2   ->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/WZ3LNU.root");   
  t1tttt    ->Add("/nfs-7/userdata/ss2015/ssBabies/v3.06/T1TTTT_1200.root");   

  //Make vector of these TChains
  vector <TChain*> run2   = {dy_run2  , ttbar_run2  , ttw_run2  ,  wjets_run2  , ttz_run2  , wz_run2  }; 
  vector <TChain*> sig    = {t1tttt };

  //Make plots -- phys14
  vector <vector <TH1F*> > run2_plots = makeAllPlots(run2); 
  vector <vector <TH1F*> > sig_plots  = makeAllPlots(sig); 

  //Flip vector of vectors
  vector <vector <TH1F*> > run2_plots2     = flipVectors(run2_plots); 
  vector <vector <TH1F*> > sig_plots2      = flipVectors(sig_plots); 

  //Prepare for output
  vector <string> titles = { "DY", "TTBAR", "TTW", "WJets", "TTZ", "WZ" };
  vector <string> plot_title = { "Lep pT", "M_{ll}", "MET", "MET uncorr", "H_{T}", "H_{T} uncorr", "M_{T} min", "nJets", "nJets uncorr", "nBtags", "nBtags uncorr", "jet p_{T}", "jet p_{T} uncorr", "MET 3.0" };
  TH1F* null = new TH1F("","",1,0,1);
  vector <string> sig_title = { "T1tttt (1200,800)" };

  //Options string.  
  vector <string> options; 
  options.push_back("--xAxisLabel lep p_{T}        --outputName lep_pt"        );
  options.push_back("--xAxisLabel M_{ll}           --outputName mll"           );
  options.push_back("--xAxisLabel MET              --outputName met"           );
  options.push_back("--xAxisLabel MET uncorr       --outputName metuncorr"     );
  options.push_back("--xAxisLabel H_{T}            --outputName ht"            );
  options.push_back("--xAxisLabel H_{T}            --outputName htuncorr"      );
  options.push_back("--xAxisLabel M_{T} min        --outputName mtmin"         );
  options.push_back("--xAxisLabel nJets            --outputName njets"         );
  options.push_back("--xAxisLabel nJets uncorr     --outputName njets_uncorr"  );
  options.push_back("--xAxisLabel nBtags           --outputName nbtags"        );
  options.push_back("--xAxisLabel nBtags uncorr    --outputName nbtags_uncorr" );
  options.push_back("--xAxisLabel jet p_{T}        --outputName jetpt"         );
  options.push_back("--xAxisLabel jet p_{T} uncorr --outputName jetpt_uncorr"  );
  options.push_back("--xAxisLabel MET 3.0          --outputName met3p0"        );

  //Make final plot
    for (unsigned int j = 0; j <= 13; j++){
      dataMCplotMaker(null, run2_plots2[j], titles, plot_title[j], "SS Baseline", Form("--legendRight -0.05 %s --lumi %.1f", options[j].c_str(), lumiAG), sig_plots2[j], sig_title); 
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
    TH1F* plot_metuncorr    = new TH1F(Form("plot_metuncorr_%i"   , iChain), "plot", 25, 0, 500);
    TH1F* plot_ht           = new TH1F(Form("plot_ht_%i"          , iChain), "plot", 40, 0, 800);
    TH1F* plot_htuncorr     = new TH1F(Form("plot_htuncorr_%i"    , iChain), "plot", 40, 0, 800);
    TH1F* plot_mtmin        = new TH1F(Form("plot_mtmin_%i"       , iChain), "plot", 40, 0, 400);
    TH1F* plot_njets        = new TH1F(Form("plot_njets_%i"       , iChain), "plot", 8 , 0,   8);
    TH1F* plot_njetsuncorr  = new TH1F(Form("plot_njetsuncorr_%i" , iChain), "plot", 8 , 0,   8);
    TH1F* plot_nbtags       = new TH1F(Form("plot_nbtags_%i"      , iChain), "plot", 8 , 0,   8);
    TH1F* plot_nbtagsuncorr = new TH1F(Form("plot_nbtagsuncorr_%i", iChain), "plot", 8 , 0,   8);
    TH1F* plot_jetpt        = new TH1F(Form("plot_jetpt_%i"       , iChain), "plot", 25, 0, 500);
    TH1F* plot_jetptuncorr  = new TH1F(Form("plot_jetptuncorr_%i" , iChain), "plot", 25, 0, 500);
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

        //Only SS events
        if (ss::hyp_class() != 3) continue; 
  
        //Require baseline region (use corrected quantities except for MET)
        int BR = baselineRegion(ss::njets_corr(), ss::nbtags_corr(), ss::met(), ss::ht_corr(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
        if (BR < 0) continue;

        //Calculate the variables for plots
        float mll = (ss::lep1_p4() + ss::lep2_p4()).M();
        float mt_min = ss::lep1_p4().pt() > ss::lep2_p4().pt() ? ss::mt_l2() : ss::mt(); 
        float ht = 0; 
        for (unsigned int i = 0; i < ss::jets_corr().size(); i++){
          if (ss::jets_corr().at(i).pt()*ss::jets_corr_JEC().at(i)*ss::jets_corr_undoJEC().at(i) > 40){
            ht += ss::jets_corr().at(i).pt()*ss::jets_corr_JEC().at(i)*ss::jets_corr_undoJEC().at(i); 
            plot_jetpt->Fill(ss::jets_corr().at(i).pt()*ss::jets_corr_JEC().at(i)*ss::jets_corr_undoJEC().at(i), ss::scale1fb()*lumiAG);
          }
        }
        for (unsigned int i = 0; i < ss::jets().size(); i++){
          if (ss::jets().at(i).pt() > 40){
            plot_jetptuncorr->Fill(ss::jets().at(i).pt(), ss::scale1fb()*lumiAG);
          }
        }

        //Fill the plots
        plot_leppt        ->Fill(ss::lep1_p4().pt(), ss::scale1fb()*lumiAG);
        plot_leppt        ->Fill(ss::lep2_p4().pt(), ss::scale1fb()*lumiAG);
        plot_mll          ->Fill(mll               , ss::scale1fb()*lumiAG);
        plot_met          ->Fill(ss::corrMET()     , ss::scale1fb()*lumiAG);
        plot_metuncorr    ->Fill(ss::met()         , ss::scale1fb()*lumiAG);
        plot_ht           ->Fill(ht                , ss::scale1fb()*lumiAG);
        plot_htuncorr     ->Fill(ss::ht()          , ss::scale1fb()*lumiAG);
        plot_mtmin        ->Fill(mt_min            , ss::scale1fb()*lumiAG);
        plot_njets        ->Fill(ss::njets_corr()  , ss::scale1fb()*lumiAG);
        plot_njetsuncorr  ->Fill(ss::njets()       , ss::scale1fb()*lumiAG);
        plot_nbtags       ->Fill(ss::nbtags_corr() , ss::scale1fb()*lumiAG);
        plot_nbtagsuncorr ->Fill(ss::nbtags()      , ss::scale1fb()*lumiAG);
        plot_met3p0       ->Fill(ss::met3p0()      , ss::scale1fb()*lumiAG);

      }//event loop
    }//file loop
   
    //Keep plots
    plots.push_back(plot_leppt       ); 
    plots.push_back(plot_mll         ); 
    plots.push_back(plot_met         ); 
    plots.push_back(plot_metuncorr   ); 
    plots.push_back(plot_ht          ); 
    plots.push_back(plot_htuncorr    ); 
    plots.push_back(plot_mtmin       ); 
    plots.push_back(plot_njets       ); 
    plots.push_back(plot_njetsuncorr ); 
    plots.push_back(plot_nbtags      ); 
    plots.push_back(plot_nbtagsuncorr); 
    plots.push_back(plot_jetpt       ); 
    plots.push_back(plot_jetptuncorr ); 
    plots.push_back(plot_met3p0      ); 

    //Add plots from first chain to result
    result.push_back(plots); 

  }//chain loop

  //Return result
  return result;
}
