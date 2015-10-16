#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

#include "../../CORE/SSSelections.h"
#include "../../software/tableMaker/CTable.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../commonUtils.h"
#include "SS.h"
#include "../../CORE/Tools/dorky/dorky.h"

float lumiAG = getLumi();
string tag = getTag().Data();  

struct yields_t { float EE; float EM; float MM; float TOTAL; }; 
struct SR_t     { TH1F* EE; TH1F* EM; TH1F* MM; TH1F* TOTAL; }; 
struct plots_t  { TH1F* h_ht; TH1F* h_met; TH1F* h_mll; TH1F* h_mtmin; TH1F* h_njets; TH1F* h_nbtags; SR_t SRHH; SR_t SRHL; SR_t SRLL; }; 

//Total
yields_t total; 

//function declaration
pair<yields_t, plots_t> run(TChain *chain, bool isData = 0, bool doFlips = 0, int doFakes = 0);

void getyields(){
  
  cout << lumiAG << endl;
  cout << tag << endl;

  //Chains
  TChain *ttbar_chain   = new TChain("t","ttbar");
  TChain *ttw_chain     = new TChain("t","ttw");
  TChain *ttz_chain     = new TChain("t","ttz");
  TChain *st_chain      = new TChain("t","st");
  TChain *wz_chain      = new TChain("t","wz");
  TChain *wzz_chain     = new TChain("t","wzz");
  TChain *wjets_chain   = new TChain("t","wjets");
  TChain *wwdps_chain   = new TChain("t","wwdps");
  TChain *ttg_chain     = new TChain("t","ttg");
  TChain *dy_chain      = new TChain("t","dy");
  TChain *vhnonbb_chain = new TChain("t","vhnonbb");
  TChain *tzq_chain     = new TChain("t","tzq");
  TChain *tth_chain     = new TChain("t","tth");
  TChain *qqww_chain    = new TChain("t","qqww");
  TChain *data_chain    = new TChain("t","data"); 
  TChain *wgamma_chain  = new TChain("t","wgamma"); 
  TChain *flips_chain    = new TChain("t","flips"); 
  TChain *fakes_chain    = new TChain("t","fakes"); 

  //Fill chains
  ttbar_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR.root"          , tag.c_str())); 
  ttw_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  ttz_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP1.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP2.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP3.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP4.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP5.root"     , tag.c_str()));
  wz_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ3LNU.root"         , tag.c_str()));
  wzz_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZZ.root"            , tag.c_str()));
  wjets_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets.root"          , tag.c_str()));
  wwdps_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WWDPS.root"          , tag.c_str()));
  ttg_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTG.root"            , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_high.root"        , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_low.root"         , tag.c_str()));
  vhnonbb_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/VHtoNonBB.root"      , tag.c_str()));
  tzq_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TZQ.root"            , tag.c_str()));
  tth_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"     , tag.c_str()));
  qqww_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"           , tag.c_str()));
  wgamma_chain ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WGToLNuG.root"       , tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD.root", tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD.root"  , tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD.root"    , tag.c_str()));
  flips_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD.root"  , tag.c_str()));
  flips_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD.root"    , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD.root", tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD.root"  , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD.root"    , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ3LNU.root"         , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"     , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"           , tag.c_str()));

  //Get yields
  pair<yields_t, plots_t> results_ttbar    = run(ttbar_chain);
  pair<yields_t, plots_t> results_ttw      = run(ttw_chain);
  pair<yields_t, plots_t> results_ttz      = run(ttz_chain);
  pair<yields_t, plots_t> results_st       = run(st_chain);
  pair<yields_t, plots_t> results_wz       = run(wz_chain);
  pair<yields_t, plots_t> results_wzz      = run(wzz_chain);
  pair<yields_t, plots_t> results_wjets    = run(wjets_chain);
  pair<yields_t, plots_t> results_wwdps    = run(wwdps_chain);
  pair<yields_t, plots_t> results_ttg      = run(ttg_chain);
  pair<yields_t, plots_t> results_dy       = run(dy_chain);
  pair<yields_t, plots_t> results_vhnonbb  = run(vhnonbb_chain);
  pair<yields_t, plots_t> results_tzq      = run(tzq_chain);
  pair<yields_t, plots_t> results_tth      = run(tth_chain);
  pair<yields_t, plots_t> results_wgamma   = run(wgamma_chain);
  pair<yields_t, plots_t> results_qqww     = run(qqww_chain);
  pair<yields_t, plots_t> results_data     = run(data_chain, 1);
  duplicate_removal::clear_list();
  pair<yields_t, plots_t> results_flips    = run(flips_chain, 1, 1);
  duplicate_removal::clear_list();
  pair<yields_t, plots_t> results_fakes    = run(fakes_chain, 1, 0, 1);
  duplicate_removal::clear_list();
  fakes_chain->SetTitle("fakes_is");
  pair<yields_t, plots_t> results_fakes_is = run(fakes_chain, 1, 0, 2);

  yields_t& ttbar    = results_ttbar.first;   
  yields_t& ttw      = results_ttw.first; 
  yields_t& ttz      = results_ttz.first;
  yields_t& st       = results_st.first;
  yields_t& wz       = results_wz.first;
  yields_t& wzz      = results_wzz.first;
  yields_t& wjets    = results_wjets.first;
  yields_t& wwdps    = results_wwdps.first;
  yields_t& ttg      = results_ttg.first;
  yields_t& dy       = results_dy.first;
  yields_t& vhnonbb  = results_vhnonbb.first;
  yields_t& tzq      = results_tzq.first;
  yields_t& tth      = results_tth.first;
  yields_t& wgamma   = results_wgamma.first;
  yields_t& qqww     = results_qqww.first;
  yields_t& data     = results_data.first;
  yields_t& flips    = results_flips.first;
  yields_t& fakes    = results_fakes.first;
  yields_t& fakes_is = results_fakes_is.first;
  
  //Make yield table
  CTable table; 
  table.setPrecision(2); 
  table.setTitle("SS Baseline Yields"); 
  table.useTitle(); 
  table.setTable() (                  "EE"  , "EM"       , "MM"         , "TOTAL"      )
                   ("ttbar"    , ttbar.EE    , ttbar.EM    , ttbar.MM    , ttbar.TOTAL  )
                   ("ttw"      , ttw.EE      , ttw.EM      , ttw.MM      , ttw.TOTAL    )
                   ("ttz"      , ttz.EE      , ttz.EM      , ttz.MM      , ttz.TOTAL    )
                   ("st"       , st.EE       , st.EM       , st.MM       , st.TOTAL     )
                   ("wz"       , wz.EE       , wz.EM       , wz.MM       , wz.TOTAL     )
                   ("wzz"      , wzz.EE      , wzz.EM      , wzz.MM      , wzz.TOTAL    )
                   ("wjets"    , wjets.EE    , wjets.EM    , wjets.MM    , wjets.TOTAL  )
                   ("wwdps"    , wwdps.EE    , wwdps.EM    , wwdps.MM    , wwdps.TOTAL  )
                   ("ttg"      , ttg.EE      , ttg.EM      , ttg.MM      , ttg.TOTAL    )
                   ("dy"       , dy.EE       , dy.EM       , dy.MM       , dy.TOTAL     )
                   ("vhnonbb"  , vhnonbb.EE  , vhnonbb.EM  , vhnonbb.MM  , vhnonbb.TOTAL)
                   ("tzq"      , tzq.EE      , tzq.EM      , tzq.MM      , tzq.TOTAL    )
                   ("tth"      , tth.EE      , tth.EM      , tth.MM      , tth.TOTAL    )
                   ("qqww"     , qqww.EE     , qqww.EM     , qqww.MM     , qqww.TOTAL   )
                   ("wgamma"   , wgamma.EE   , wgamma.EM   , wgamma.MM   , wgamma.TOTAL )
                   ("flips"    , flips.EE    , flips.EM    , flips.MM    , flips.TOTAL  )
                   ("fakes"    , fakes.EE    , fakes.EM    , fakes.MM    , fakes.TOTAL  )
                   ("fakes_is" , fakes_is.EE , fakes_is.EM , fakes_is.MM , fakes_is.TOTAL  )
                   ("total"    , total.EE    , total.EM    , total.MM    , total.TOTAL  )
                   ("data"     , data.EE     , data.EM     , data.MM     , data.TOTAL   );
  table.print();

  plots_t& p_ttbar    = results_ttbar.second;   
  plots_t& p_ttw      = results_ttw.second; 
  plots_t& p_ttz      = results_ttz.second;
  plots_t& p_st       = results_st.second;
  plots_t& p_wz       = results_wz.second;
  plots_t& p_wzz      = results_wzz.second;
  plots_t& p_wjets    = results_wjets.second;
  plots_t& p_wwdps    = results_wwdps.second;
  plots_t& p_ttg      = results_ttg.second;
  plots_t& p_dy       = results_dy.second;
  plots_t& p_vhnonbb  = results_vhnonbb.second;
  plots_t& p_tzq      = results_tzq.second;
  plots_t& p_tth      = results_tth.second;
  plots_t& p_wgamma   = results_wgamma.second;
  plots_t& p_qqww     = results_qqww.second;
  plots_t& p_data     = results_data.second;
  plots_t& p_flips    = results_flips.second;
  plots_t& p_fakes    = results_fakes.second;
  plots_t& p_fakes_is = results_fakes_is.second;

  //Titles for legend
  vector <string> titles;
  titles.push_back("ttW"); 
  titles.push_back("ttZ"); 
  titles.push_back("qqWW"); 
  titles.push_back("WZ"); 
  titles.push_back("Flips"); 
  titles.push_back("Fakes"); 

  //SR plots
  vector <TH1F*> SRHH_plots;
  SRHH_plots.push_back(p_ttw.SRHH.TOTAL  );
  SRHH_plots.push_back(p_ttz.SRHH.TOTAL  );
  SRHH_plots.push_back(p_qqww.SRHH.TOTAL );
  SRHH_plots.push_back(p_wz.SRHH.TOTAL   );
  SRHH_plots.push_back(p_flips.SRHH.TOTAL);
  SRHH_plots.push_back(p_fakes.SRHH.TOTAL);
  dataMCplotMaker(p_data.SRHH.TOTAL, SRHH_plots, titles, "H-H SRs", "", Form("--lumi %.1f --lumiUnit pb --outputName HHSR.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > ht_plots;
  ht_plots.push_back(p_ttw.h_ht);
  ht_plots.push_back(p_ttz.h_ht);
  ht_plots.push_back(p_qqww.h_ht);
  ht_plots.push_back(p_wz.h_ht);
  ht_plots.push_back(p_flips.h_ht);
  ht_plots.push_back(p_fakes.h_ht);
  dataMCplotMaker(p_data.h_ht, ht_plots, titles, "ht", "test", Form("--lumi %.1f --lumiUnit pb --outputName ht_all_SS.pdf --xAxisLabel HT --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > met_plots;
  met_plots.push_back(p_ttw.h_met);
  met_plots.push_back(p_ttz.h_met);
  met_plots.push_back(p_qqww.h_met);
  met_plots.push_back(p_wz.h_met);
  met_plots.push_back(p_flips.h_met);
  met_plots.push_back(p_fakes.h_met);
  dataMCplotMaker(p_data.h_met, met_plots, titles, "met", "test", Form("--lumi %.1f --lumiUnit pb --outputName met_all_SS.pdf --xAxisLabel MET --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > mll_plots;
  mll_plots.push_back(p_ttw.h_mll);
  mll_plots.push_back(p_ttz.h_mll);
  mll_plots.push_back(p_qqww.h_mll);
  mll_plots.push_back(p_wz.h_mll);
  mll_plots.push_back(p_flips.h_mll);
  mll_plots.push_back(p_fakes.h_mll);
  dataMCplotMaker(p_data.h_mll, mll_plots, titles, "mll", "test", Form("--lumi %.1f --lumiUnit pb --outputName mll_all_SS.pdf --xAxisLabel MLL --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > mtmin_plots;
  mtmin_plots.push_back(p_ttw.h_mtmin);
  mtmin_plots.push_back(p_ttz.h_mtmin);
  mtmin_plots.push_back(p_qqww.h_mtmin);
  mtmin_plots.push_back(p_wz.h_mtmin);
  mtmin_plots.push_back(p_flips.h_mtmin);
  mtmin_plots.push_back(p_fakes.h_mtmin);
  dataMCplotMaker(p_data.h_mtmin, mtmin_plots, titles, "mtmin", "test", Form("--lumi %.1f --lumiUnit pb --outputName mtmin_all_SS.pdf --xAxisLabel MTMIN --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > njets_plots;
  njets_plots.push_back(p_ttw.h_njets);
  njets_plots.push_back(p_ttz.h_njets);
  njets_plots.push_back(p_qqww.h_njets);
  njets_plots.push_back(p_wz.h_njets);
  njets_plots.push_back(p_flips.h_njets);
  njets_plots.push_back(p_fakes.h_njets);
  dataMCplotMaker(p_data.h_njets, njets_plots, titles, "njets", "test", Form("--lumi %.1f --lumiUnit pb --outputName njets_all_SS.pdf --xAxisLabel NJETS --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > nbtags_plots;
  nbtags_plots.push_back(p_ttw.h_nbtags);
  nbtags_plots.push_back(p_ttz.h_nbtags);
  nbtags_plots.push_back(p_qqww.h_nbtags);
  nbtags_plots.push_back(p_wz.h_nbtags);
  nbtags_plots.push_back(p_flips.h_nbtags);
  nbtags_plots.push_back(p_fakes.h_nbtags);
  dataMCplotMaker(p_data.h_nbtags, nbtags_plots, titles, "nbtags", "test", Form("--lumi %.1f --lumiUnit pb --outputName nbtags_all_SS.pdf --xAxisLabel NBTAGS --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

}

//doFakes = 1 for QCD, 2 for inSitu
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes){

  yields_t y_result;
  plots_t  p_result;

  //Initialize
  y_result.EE    = 0;
  y_result.EM    = 0;
  y_result.MM    = 0;
  y_result.TOTAL = 0;

  p_result.h_ht       = new TH1F(Form("ht_%s"        , chain->GetTitle()), Form("ht_%s"        , chain->GetTitle()), 25, 0, 500);
  p_result.h_met      = new TH1F(Form("met_%s"       , chain->GetTitle()), Form("met_%s"       , chain->GetTitle()), 25, 0, 500);
  p_result.h_mll      = new TH1F(Form("mll_%s"       , chain->GetTitle()), Form("mll_%s"       , chain->GetTitle()), 25, 0, 500);
  p_result.h_mtmin    = new TH1F(Form("mtmin_%s"     , chain->GetTitle()), Form("mtmin_%s"     , chain->GetTitle()), 25, 0, 500);
  p_result.h_njets    = new TH1F(Form("njets_%s"     , chain->GetTitle()), Form("njets_%s"     , chain->GetTitle()), 10, 0, 10);
  p_result.h_nbtags   = new TH1F(Form("nbtags_%s"    , chain->GetTitle()), Form("nbtags_%s"    , chain->GetTitle()), 10, 0, 10);
  p_result.SRHH.EE    = new TH1F(Form("SRHH_EE_%s"   , chain->GetTitle()), Form("SRHH_EE_%s"   , chain->GetTitle()), 32, 1, 33);
  p_result.SRHH.EM    = new TH1F(Form("SRHH_EM_%s"   , chain->GetTitle()), Form("SRHH_EM_%s"   , chain->GetTitle()), 32, 1, 33);
  p_result.SRHH.MM    = new TH1F(Form("SRHH_MM_%s"   , chain->GetTitle()), Form("SRHH_MM_%s"   , chain->GetTitle()), 32, 1, 33);
  p_result.SRHH.TOTAL = new TH1F(Form("SRHH_TOTAL_%s", chain->GetTitle()), Form("SRHH_TOTAL_%s", chain->GetTitle()), 32, 1, 33);

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

      float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG*getPUw(ss::nGoodVertices());

      //Progress
      //SSAG::progress(nEventsTotal, nEventsChain);

      //Only keep good events
      if (!doFlips && !doFakes) {
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && ss::lep1_motherID()!=1) continue;
	    if (!isData && ss::lep2_motherID()!=1) continue;
      }

      if (doFlips) {
	if (ss::hyp_class() != 4) continue;
	    float flipFact = 0.;
	    if (abs(ss::lep1_id())==11){
	      float flr = flipRate(ss::lep1_p4().pt(), ss::lep1_p4().eta());
	      flipFact += (flr/(1-flr));
	    }
	    if (abs(ss::lep2_id())==11){
	      float flr = flipRate(ss::lep2_p4().pt(), ss::lep2_p4().eta());
	      flipFact += (flr/(1-flr));
	    }
	    weight *= flipFact;
      } 

      if (doFakes){
        if (ss::hyp_class() != 2) continue;
        float fr = 0.;
        if (ss::lep1_passes_id()==0){
          if (doFakes == 1) fr = fakeRate(ss::lep1_id(),ss::lep1_p4().pt(), ss::lep1_p4().eta());
          if (doFakes == 2) fr = fakeRateInSitu(ss::lep1_id(),ss::lep1_p4().pt(), ss::lep1_p4().eta());
        }
        if (ss::lep2_passes_id()==0){
          if (doFakes == 1) fr = fakeRate(ss::lep2_id(),ss::lep2_p4().pt(), ss::lep2_p4().eta());
          if (doFakes == 2) fr = fakeRateInSitu(ss::lep2_id(),ss::lep2_p4().pt(), ss::lep2_p4().eta());
        }
        weight *= fr/(1.-fr);
        if (!ss::is_real_data()) weight *= -1.;
      } 

      //Reject duplicates (after selection otherwise flips are ignored...)
      if (isData == true){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)){ continue; }
      }

      //Require baseline selections
      int BR = baselineRegion(ss::njets_corr(), ss::nbtags_corr(), ss::corrMET(), ss::ht_corr(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      if (BR < 0) continue;
   
      //Get the SR
      int SR = signalRegion(ss::njets_corr(), ss::nbtags_corr(), ss::corrMET(), ss::ht_corr(), ss::mtmin(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
 
      //Get the category
      anal_type_t categ = analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt());

      //Calculate the baseline yield
      if      (ss::hyp_type() == 3) y_result.EE    += weight;
      else if (ss::hyp_type() == 0) y_result.MM    += weight;
      else                          y_result.EM    += weight;
                                    y_result.TOTAL += weight;

      //Fill kinem plots
      p_result.h_ht    ->Fill(ss::ht_corr()    ,weight);
      p_result.h_met   ->Fill(ss::corrMET()    ,weight);
      p_result.h_mll   ->Fill((ss::lep1_p4()+ss::lep2_p4()).M()    ,weight);
      p_result.h_mtmin ->Fill(ss::mtmin()      ,weight);
      p_result.h_njets ->Fill(ss::njets_corr() ,weight);
      p_result.h_nbtags->Fill(ss::nbtags_corr(),weight);

      //Fill SR plots
      if (categ == HighHigh){
        if      (ss::hyp_type() == 3) p_result.SRHH.EE->Fill(SR, weight); 
        else if (ss::hyp_type() == 0) p_result.SRHH.MM->Fill(SR, weight); 
        else                          p_result.SRHH.EM->Fill(SR, weight); 
                                      p_result.SRHH.TOTAL->Fill(SR, weight); 
      }


    }//event loop
  }//file loop

  if (y_result.EE<0) y_result.EE=0.;
  if (y_result.MM<0) y_result.MM=0.;
  if (y_result.EM<0) y_result.EM=0.;
  if (y_result.TOTAL<0) y_result.TOTAL=0.;

  //Update total
  if (!isData || doFlips || doFakes == 1){
    total.EE    += y_result.EE;
    total.EM    += y_result.EM;
    total.MM    += y_result.MM;
    total.TOTAL += y_result.TOTAL;
  }

  //Return yield
  return pair<yields_t, plots_t>(y_result,p_result); 

}
