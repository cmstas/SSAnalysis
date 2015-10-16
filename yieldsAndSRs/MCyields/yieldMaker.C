#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

#include "../../classFiles/v4.00/SS.h"
#include "../../CORE/SSSelections.h"
#include "../../software/tableMaker/CTable.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../commonUtils.h"
#include "../../CORE/Tools/dorky/dorky.h"

float lumiAG = getLumi();
string tag = getTag().Data();  

struct yields_t { float EE; float EM; float MM; float TOTAL; }; 
struct SR_t     { TH1F* EE; TH1F* EM; TH1F* MM; TH1F* TOTAL; }; 
struct plots_t  { TH1F* h_ht; TH1F* h_met; TH1F* h_mll; TH1F* h_mtmin; TH1F* h_njets; TH1F* h_nbtags; TH1F* h_l1pt; TH1F* h_l2pt; TH1F* h_type; TH1F* h_lep1_miniIso; TH1F* h_lep2_miniIso; TH1F* h_lep1_ptRatio; TH1F* h_lep2_ptRatio; TH1F* h_lep1_ptRel; TH1F* h_lep2_ptRel; SR_t SRHH; SR_t SRHL; SR_t SRLL; }; 

//Total
yields_t total; 

//function declaration
pair<yields_t, plots_t> run(TChain *chain, bool isData = 0, bool doFlips = 0, int doFakes = 0, int exclude = 0);

void getyields(){
  
  cout << lumiAG << endl;
  cout << tag << endl;

  //Chains
  //fakes
  TChain *ttbar_chain   = new TChain("t","ttbar");
  TChain *st_chain      = new TChain("t","st");
  TChain *wjets_chain   = new TChain("t","wjets");
  //rares
  TChain *ttw_chain     = new TChain("t","ttw");
  TChain *ttz_chain     = new TChain("t","ttz");
  TChain *wz_chain      = new TChain("t","wz");
  TChain *wzz_chain     = new TChain("t","wzz");
  TChain *wwdps_chain   = new TChain("t","wwdps");
  TChain *ttg_chain     = new TChain("t","ttg");
  TChain *dy_chain      = new TChain("t","dy");
  TChain *vhnonbb_chain = new TChain("t","vhnonbb");
  TChain *tzq_chain     = new TChain("t","tzq");
  TChain *tth_chain     = new TChain("t","tth");
  TChain *qqww_chain    = new TChain("t","qqww");
  TChain *wgamma_chain  = new TChain("t","wgamma"); 
  TChain *tttt_chain    = new TChain("t","tttt"); 
  //data
  TChain *data_chain    = new TChain("t","data"); 
  TChain *flips_chain   = new TChain("t","flips"); 
  TChain *fakes_chain   = new TChain("t","fakes"); 

  //Fill chains
  ttbar_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR.root"          , tag.c_str())); 
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP1.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP2.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP3.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP4.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP5.root"     , tag.c_str()));
  wjets_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets.root"          , tag.c_str()));

  ttw_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  ttz_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  wz_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ3LNU.root"         , tag.c_str()));
  wzz_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZZ.root"            , tag.c_str()));
  wwdps_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WWDPS.root"          , tag.c_str()));
  ttg_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTG.root"            , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_high.root"        , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_low.root"         , tag.c_str()));
  vhnonbb_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/VHtoNonBB.root"      , tag.c_str()));
  tzq_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TZQ.root"            , tag.c_str()));
  tth_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"     , tag.c_str()));
  wgamma_chain ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WGToLNuG.root"       , tag.c_str()));
  qqww_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"           , tag.c_str()));
  tttt_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTTT.root"           , tag.c_str()));

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
  pair<yields_t, plots_t> results_ttbar_fa = run(ttbar_chain, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_ttbar_fl = run(ttbar_chain, 0, 0, 0, 2);
  pair<yields_t, plots_t> results_ttbar_ff = run(ttbar_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_st_ff    = run(st_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_wjets_ff = run(wjets_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_dy_ff    = run(dy_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_st       = run(st_chain);
  pair<yields_t, plots_t> results_wjets    = run(wjets_chain);

  pair<yields_t, plots_t> results_ttw      = run(ttw_chain);
  pair<yields_t, plots_t> results_ttz      = run(ttz_chain);
  pair<yields_t, plots_t> results_wz       = run(wz_chain);
  //missing: ZZ4l
  pair<yields_t, plots_t> results_wzz      = run(wzz_chain);
  //missing: WWZ
  pair<yields_t, plots_t> results_wwdps    = run(wwdps_chain);
  pair<yields_t, plots_t> results_ttg      = run(ttg_chain);
  pair<yields_t, plots_t> results_dy       = run(dy_chain);
  pair<yields_t, plots_t> results_vhnonbb  = run(vhnonbb_chain);
  pair<yields_t, plots_t> results_tzq      = run(tzq_chain);
  pair<yields_t, plots_t> results_tth      = run(tth_chain);
  pair<yields_t, plots_t> results_wgamma   = run(wgamma_chain);
  pair<yields_t, plots_t> results_qqww     = run(qqww_chain);
  pair<yields_t, plots_t> results_tttt     = run(tttt_chain);

  pair<yields_t, plots_t> results_data     = run(data_chain, 1);
  duplicate_removal::clear_list();
  pair<yields_t, plots_t> results_flips    = run(flips_chain, 1, 1);
  duplicate_removal::clear_list();
  pair<yields_t, plots_t> results_fakes    = run(fakes_chain, 1, 0, 1);
  duplicate_removal::clear_list();
  fakes_chain->SetTitle("fakes_is");
  pair<yields_t, plots_t> results_fakes_is = run(fakes_chain, 1, 0, 2);

  yields_t& ttbar    = results_ttbar.first;
  yields_t& ttbar_ff = results_ttbar_ff.first;
  yields_t& wjets_ff = results_wjets_ff.first;
  yields_t& dy_ff    = results_dy_ff.first;
  yields_t& st_ff    = results_st_ff.first;
  yields_t& ttbar_fa = results_ttbar_fa.first;
  yields_t& ttbar_fl = results_ttbar_fl.first;
  yields_t& st       = results_st.first;
  yields_t& wjets    = results_wjets.first;
  yields_t& ttw      = results_ttw.first;
  yields_t& ttz      = results_ttz.first;
  yields_t& wz       = results_wz.first;
  yields_t& wzz      = results_wzz.first;
  yields_t& wwdps    = results_wwdps.first;
  yields_t& ttg      = results_ttg.first;
  yields_t& dy       = results_dy.first;
  yields_t& vhnonbb  = results_vhnonbb.first;
  yields_t& tzq      = results_tzq.first;
  yields_t& tth      = results_tth.first;
  yields_t& wgamma   = results_wgamma.first;
  yields_t& qqww     = results_qqww.first;
  yields_t& tttt     = results_tttt.first;
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
                   ("ttbar_fa" , ttbar_fa.EE , ttbar_fa.EM , ttbar_fa.MM , ttbar_fa.TOTAL)
                   ("ttbar_fl" , ttbar_fl.EE , ttbar_fl.EM , ttbar_fl.MM , ttbar_fl.TOTAL)
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
                   ("tttt"     , tttt.EE     , tttt.EM     , tttt.MM     , tttt.TOTAL   )
                   ("wgamma"   , wgamma.EE   , wgamma.EM   , wgamma.MM   , wgamma.TOTAL )
                   ("flips"    , flips.EE    , flips.EM    , flips.MM    , flips.TOTAL  )
                   ("fakes"    , fakes.EE    , fakes.EM    , fakes.MM    , fakes.TOTAL  )
                   ("fakes_is" , fakes_is.EE , fakes_is.EM , fakes_is.MM , fakes_is.TOTAL  )
                   ("total"    , total.EE    , total.EM    , total.MM    , total.TOTAL  )
                   ("data"     , data.EE     , data.EM     , data.MM     , data.TOTAL   );
  table.print();

  plots_t& p_ttbar    = results_ttbar.second;   
  plots_t& p_ttbar_ff = results_ttbar_ff.second;   
  plots_t& p_wjets_ff = results_wjets_ff.second;   
  plots_t& p_dy_ff    = results_dy_ff.second;   
  plots_t& p_st_ff    = results_st_ff.second;   
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
  plots_t& p_tttt     = results_tttt.second;
  plots_t& p_data     = results_data.second;
  plots_t& p_flips    = results_flips.second;
  plots_t& p_fakes    = results_fakes.second;
  plots_t& p_fakes_is = results_fakes_is.second;

  //Titles for legend
  vector <string> titles;
  titles.push_back("ttW"); 
  titles.push_back("ttZ"); 
  titles.push_back("Rares"); 
  titles.push_back("WZ"); 
  titles.push_back("Fakes"); 
  titles.push_back("Flips"); 

  //Titles for MC-only plots
  vector <string> titles2;
  titles2.push_back("ttW"); 
  titles2.push_back("ttZ"); 
  titles2.push_back("rares"); 
  titles2.push_back("WZ"); 
  titles2.push_back("ttbar"); 
  titles2.push_back("dy"); 
  titles2.push_back("wjets"); 
  titles2.push_back("st"); 

  p_qqww.h_ht->Add(p_wzz.h_ht);
  p_qqww.h_ht->Add(p_wwdps.h_ht);
  p_qqww.h_ht->Add(p_ttg.h_ht);
  p_qqww.h_ht->Add(p_dy.h_ht);
  p_qqww.h_ht->Add(p_vhnonbb.h_ht);
  p_qqww.h_ht->Add(p_tth.h_ht);
  p_qqww.h_ht->Add(p_wgamma.h_ht);
  p_qqww.h_ht->Add(p_tttt.h_ht);

  //SR plots
  vector <TH1F*> SRHH_plots;
  SRHH_plots.push_back(p_ttw.SRHH.TOTAL  );
  SRHH_plots.push_back(p_ttz.SRHH.TOTAL  );
  SRHH_plots.push_back(p_qqww.SRHH.TOTAL );
  SRHH_plots.push_back(p_wz.SRHH.TOTAL   );
  SRHH_plots.push_back(p_fakes.SRHH.TOTAL   );
  SRHH_plots.push_back(p_flips.SRHH.TOTAL   );
  dataMCplotMaker(p_data.SRHH.TOTAL, SRHH_plots, titles, "H-H SRs", "data-driven", Form("--lumi %.1f --lumiUnit pb --outputName HHSR.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F*> SRHL_plots;
  SRHL_plots.push_back(p_ttw.SRHL.TOTAL  );
  SRHL_plots.push_back(p_ttz.SRHL.TOTAL  );
  SRHL_plots.push_back(p_qqww.SRHL.TOTAL );
  SRHL_plots.push_back(p_wz.SRHL.TOTAL   );
  SRHL_plots.push_back(p_fakes.SRHL.TOTAL   );
  SRHL_plots.push_back(p_flips.SRHL.TOTAL   );
  dataMCplotMaker(p_data.SRHL.TOTAL, SRHL_plots, titles, "H-L SRs", "data-driven", Form("--lumi %.1f --lumiUnit pb --outputName HLSR.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F*> SRLL_plots;
  SRLL_plots.push_back(p_ttw.SRLL.TOTAL  );
  SRLL_plots.push_back(p_ttz.SRLL.TOTAL  );
  SRLL_plots.push_back(p_qqww.SRLL.TOTAL );
  SRLL_plots.push_back(p_wz.SRLL.TOTAL   );
  SRLL_plots.push_back(p_fakes.SRLL.TOTAL   );
  SRLL_plots.push_back(p_flips.SRLL.TOTAL   );
  dataMCplotMaker(p_data.SRLL.TOTAL, SRLL_plots, titles, "L-L SRs", "data-driven", Form("--lumi %.1f --lumiUnit pb --outputName LLSR.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  //SR plots
  vector <TH1F*> SRHHMC_plots;
  SRHHMC_plots.push_back(p_ttw.SRHH.TOTAL  );
  SRHHMC_plots.push_back(p_ttz.SRHH.TOTAL  );
  SRHHMC_plots.push_back(p_qqww.SRHH.TOTAL );
  SRHHMC_plots.push_back(p_wz.SRHH.TOTAL   );
  SRHHMC_plots.push_back(p_ttbar_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_dy_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_wjets_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_st_ff.SRHH.TOTAL);
  dataMCplotMaker(p_data.SRHH.TOTAL, SRHHMC_plots, titles2, "H-H SRs", "from MC", Form("--lumi %.1f --lumiUnit pb --outputName HHSRMC.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F*> SRHLMC_plots;
  SRHLMC_plots.push_back(p_ttw.SRHL.TOTAL  );
  SRHLMC_plots.push_back(p_ttz.SRHL.TOTAL  );
  SRHLMC_plots.push_back(p_qqww.SRHL.TOTAL );
  SRHLMC_plots.push_back(p_wz.SRHL.TOTAL   );
  SRHLMC_plots.push_back(p_ttbar_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_dy_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_wjets_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_st_ff.SRHL.TOTAL);
  dataMCplotMaker(p_data.SRHL.TOTAL, SRHLMC_plots, titles2, "H-L SRs", "from MC", Form("--lumi %.1f --lumiUnit pb --outputName HLSRMC.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F*> SRLLMC_plots;
  SRLLMC_plots.push_back(p_ttw.SRLL.TOTAL  );
  SRLLMC_plots.push_back(p_ttz.SRLL.TOTAL  );
  SRLLMC_plots.push_back(p_qqww.SRLL.TOTAL );
  SRLLMC_plots.push_back(p_wz.SRLL.TOTAL   );
  SRLLMC_plots.push_back(p_ttbar_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_dy_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_wjets_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_st_ff.SRLL.TOTAL);
  dataMCplotMaker(p_data.SRLL.TOTAL, SRLLMC_plots, titles2, "L-L SRs", "from MC", Form("--lumi %.1f --lumiUnit pb --outputName LLSRMC.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > type_plots;
  type_plots.push_back(p_ttw.h_type);
  type_plots.push_back(p_ttz.h_type);
  type_plots.push_back(p_qqww.h_type);
  type_plots.push_back(p_wz.h_type);
  type_plots.push_back(p_flips.h_type);
  type_plots.push_back(p_fakes.h_type);
  dataMCplotMaker(p_data.h_type, type_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName type_all_SS.pdf --xAxisLabel type --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep1_miniIso_plots;
  lep1_miniIso_plots.push_back(p_ttw.h_lep1_miniIso);
  lep1_miniIso_plots.push_back(p_ttz.h_lep1_miniIso);
  lep1_miniIso_plots.push_back(p_qqww.h_lep1_miniIso);
  lep1_miniIso_plots.push_back(p_wz.h_lep1_miniIso);
  lep1_miniIso_plots.push_back(p_flips.h_lep1_miniIso);
  lep1_miniIso_plots.push_back(p_fakes.h_lep1_miniIso);
  dataMCplotMaker(p_data.h_lep1_miniIso, lep1_miniIso_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep1_miniIso_all_SS.pdf --xAxisLabel lep1_miniIso --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep2_miniIso_plots;
  lep2_miniIso_plots.push_back(p_ttw.h_lep2_miniIso);
  lep2_miniIso_plots.push_back(p_ttz.h_lep2_miniIso);
  lep2_miniIso_plots.push_back(p_qqww.h_lep2_miniIso);
  lep2_miniIso_plots.push_back(p_wz.h_lep2_miniIso);
  lep2_miniIso_plots.push_back(p_flips.h_lep2_miniIso);
  lep2_miniIso_plots.push_back(p_fakes.h_lep2_miniIso);
  dataMCplotMaker(p_data.h_lep2_miniIso, lep2_miniIso_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep2_miniIso_all_SS.pdf --xAxisLabel lep2_miniIso --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep1_ptRel_plots;
  lep1_ptRel_plots.push_back(p_ttw.h_lep1_ptRel);
  lep1_ptRel_plots.push_back(p_ttz.h_lep1_ptRel);
  lep1_ptRel_plots.push_back(p_qqww.h_lep1_ptRel);
  lep1_ptRel_plots.push_back(p_wz.h_lep1_ptRel);
  lep1_ptRel_plots.push_back(p_flips.h_lep1_ptRel);
  lep1_ptRel_plots.push_back(p_fakes.h_lep1_ptRel);
  dataMCplotMaker(p_data.h_lep1_ptRel, lep1_ptRel_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep1_ptRel_all_SS.pdf --xAxisLabel lep1_ptRel --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep2_ptRel_plots;
  lep2_ptRel_plots.push_back(p_ttw.h_lep2_ptRel);
  lep2_ptRel_plots.push_back(p_ttz.h_lep2_ptRel);
  lep2_ptRel_plots.push_back(p_qqww.h_lep2_ptRel);
  lep2_ptRel_plots.push_back(p_wz.h_lep2_ptRel);
  lep2_ptRel_plots.push_back(p_flips.h_lep2_ptRel);
  lep2_ptRel_plots.push_back(p_fakes.h_lep2_ptRel);
  dataMCplotMaker(p_data.h_lep2_ptRel, lep2_ptRel_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep2_ptRel_all_SS.pdf --xAxisLabel lep2_ptRel --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep1_ptRatio_plots;
  lep1_ptRatio_plots.push_back(p_ttw.h_lep1_ptRatio);
  lep1_ptRatio_plots.push_back(p_ttz.h_lep1_ptRatio);
  lep1_ptRatio_plots.push_back(p_qqww.h_lep1_ptRatio);
  lep1_ptRatio_plots.push_back(p_wz.h_lep1_ptRatio);
  lep1_ptRatio_plots.push_back(p_flips.h_lep1_ptRatio);
  lep1_ptRatio_plots.push_back(p_fakes.h_lep1_ptRatio);
  dataMCplotMaker(p_data.h_lep1_ptRatio, lep1_ptRatio_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep1_ptRatio_all_SS.pdf --xAxisLabel lep1_ptRatio --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep2_ptRatio_plots;
  lep2_ptRatio_plots.push_back(p_ttw.h_lep2_ptRatio);
  lep2_ptRatio_plots.push_back(p_ttz.h_lep2_ptRatio);
  lep2_ptRatio_plots.push_back(p_qqww.h_lep2_ptRatio);
  lep2_ptRatio_plots.push_back(p_wz.h_lep2_ptRatio);
  lep2_ptRatio_plots.push_back(p_flips.h_lep2_ptRatio);
  lep2_ptRatio_plots.push_back(p_fakes.h_lep2_ptRatio);
  dataMCplotMaker(p_data.h_lep2_ptRatio, lep2_ptRatio_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep2_ptRatio_all_SS.pdf --xAxisLabel lep2_ptRatio --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100
  
  //Redo iso with MC only
  vector <TH1F* > lep1_miniIso_plotsMC;
  lep1_miniIso_plotsMC.push_back(p_ttw.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_ttz.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_qqww.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_wz.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_ttbar_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_dy_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_wjets_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_st_ff.h_lep1_miniIso);
  dataMCplotMaker(p_data.h_lep1_miniIso, lep1_miniIso_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep1_miniIso_all_SSMC.pdf --xAxisLabel lep1_miniIso --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep2_miniIso_plotsMC;
  lep2_miniIso_plotsMC.push_back(p_ttw.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_ttz.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_qqww.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_wz.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_ttbar_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_dy_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_wjets_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_st_ff.h_lep2_miniIso);
  dataMCplotMaker(p_data.h_lep2_miniIso, lep2_miniIso_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep2_miniIso_all_SSMC.pdf --xAxisLabel lep2_miniIso --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep1_ptRel_plotsMC;
  lep1_ptRel_plotsMC.push_back(p_ttw.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_ttz.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_qqww.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_wz.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_ttbar_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_dy_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_wjets_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_st_ff.h_lep1_ptRel);
  dataMCplotMaker(p_data.h_lep1_ptRel, lep1_ptRel_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep1_ptRel_all_SSMC.pdf --xAxisLabel lep1_ptRel --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep2_ptRel_plotsMC;
  lep2_ptRel_plotsMC.push_back(p_ttw.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_ttz.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_qqww.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_wz.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_ttbar_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_dy_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_wjets_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_st_ff.h_lep2_ptRel);
  dataMCplotMaker(p_data.h_lep2_ptRel, lep2_ptRel_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep2_ptRel_all_SSMC.pdf --xAxisLabel lep2_ptRel --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep1_ptRatio_plotsMC;
  lep1_ptRatio_plotsMC.push_back(p_ttw.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_ttz.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_qqww.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_wz.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_ttbar_ff.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_dy_ff.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_wjets.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_st_ff.h_lep1_ptRatio);
  dataMCplotMaker(p_data.h_lep1_ptRatio, lep1_ptRatio_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep1_ptRatio_all_SSMC.pdf --xAxisLabel lep1_ptRatio --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > lep2_ptRatio_plotsMC;
  lep2_ptRatio_plotsMC.push_back(p_ttw.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_ttz.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_qqww.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_wz.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_ttbar_ff.h_lep1_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_dy_ff.h_lep1_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_wjets.h_lep1_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_st_ff.h_lep1_ptRatio);
  dataMCplotMaker(p_data.h_lep2_ptRatio, lep2_ptRatio_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName lep2_ptRatio_all_SSMC.pdf --xAxisLabel lep2_ptRatio --isLinear --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > ht_plots;
  ht_plots.push_back(p_ttw.h_ht);
  ht_plots.push_back(p_ttz.h_ht);
  ht_plots.push_back(p_qqww.h_ht);
  ht_plots.push_back(p_wz.h_ht);
  ht_plots.push_back(p_flips.h_ht);
  ht_plots.push_back(p_fakes.h_ht);
  dataMCplotMaker(p_data.h_ht, ht_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName ht_all_SS.pdf --xAxisLabel HT --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  p_qqww.h_met->Add(p_wzz.h_met);
  p_qqww.h_met->Add(p_wwdps.h_met);
  p_qqww.h_met->Add(p_ttg.h_met);
  p_qqww.h_met->Add(p_dy.h_met);
  p_qqww.h_met->Add(p_vhnonbb.h_met);
  p_qqww.h_met->Add(p_tth.h_met);
  p_qqww.h_met->Add(p_wgamma.h_met);
  p_qqww.h_met->Add(p_tttt.h_met);
  vector <TH1F* > met_plots;
  met_plots.push_back(p_ttw.h_met);
  met_plots.push_back(p_ttz.h_met);
  met_plots.push_back(p_qqww.h_met);
  met_plots.push_back(p_wz.h_met);
  met_plots.push_back(p_flips.h_met);
  met_plots.push_back(p_fakes.h_met);
  dataMCplotMaker(p_data.h_met, met_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName met_all_SS.pdf --xAxisLabel MET --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  p_qqww.h_mll->Add(p_wzz.h_mll);
  p_qqww.h_mll->Add(p_wwdps.h_mll);
  p_qqww.h_mll->Add(p_ttg.h_mll);
  p_qqww.h_mll->Add(p_dy.h_mll);
  p_qqww.h_mll->Add(p_vhnonbb.h_mll);
  p_qqww.h_mll->Add(p_tth.h_mll);
  p_qqww.h_mll->Add(p_wgamma.h_mll);
  p_qqww.h_mll->Add(p_tttt.h_mll);
  vector <TH1F* > mll_plots;
  mll_plots.push_back(p_ttw.h_mll);
  mll_plots.push_back(p_ttz.h_mll);
  mll_plots.push_back(p_qqww.h_mll);
  mll_plots.push_back(p_wz.h_mll);
  mll_plots.push_back(p_flips.h_mll);
  mll_plots.push_back(p_fakes.h_mll);
  dataMCplotMaker(p_data.h_mll, mll_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName mll_all_SS.pdf --xAxisLabel M_{LL} --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  p_qqww.h_mtmin->Add(p_wzz.h_mtmin);
  p_qqww.h_mtmin->Add(p_wwdps.h_mtmin);
  p_qqww.h_mtmin->Add(p_ttg.h_mtmin);
  p_qqww.h_mtmin->Add(p_dy.h_mtmin);
  p_qqww.h_mtmin->Add(p_vhnonbb.h_mtmin);
  p_qqww.h_mtmin->Add(p_tth.h_mtmin);
  p_qqww.h_mtmin->Add(p_wgamma.h_mtmin);
  p_qqww.h_mtmin->Add(p_tttt.h_mtmin);
  vector <TH1F* > mtmin_plots;
  mtmin_plots.push_back(p_ttw.h_mtmin);
  mtmin_plots.push_back(p_ttz.h_mtmin);
  mtmin_plots.push_back(p_qqww.h_mtmin);
  mtmin_plots.push_back(p_wz.h_mtmin);
  mtmin_plots.push_back(p_flips.h_mtmin);
  mtmin_plots.push_back(p_fakes.h_mtmin);
  dataMCplotMaker(p_data.h_mtmin, mtmin_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName mtmin_all_SS.pdf --xAxisLabel MT^{min} --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  p_qqww.h_njets->Add(p_wzz.h_njets);
  p_qqww.h_njets->Add(p_wwdps.h_njets);
  p_qqww.h_njets->Add(p_ttg.h_njets);
  p_qqww.h_njets->Add(p_dy.h_njets);
  p_qqww.h_njets->Add(p_vhnonbb.h_njets);
  p_qqww.h_njets->Add(p_tth.h_njets);
  p_qqww.h_njets->Add(p_wgamma.h_njets);
  p_qqww.h_njets->Add(p_tttt.h_njets);
  vector <TH1F* > njets_plots;
  njets_plots.push_back(p_ttw.h_njets);
  njets_plots.push_back(p_ttz.h_njets);
  njets_plots.push_back(p_qqww.h_njets);
  njets_plots.push_back(p_wz.h_njets);
  njets_plots.push_back(p_flips.h_njets);
  njets_plots.push_back(p_fakes.h_njets);
  dataMCplotMaker(p_data.h_njets, njets_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName njets_all_SS.pdf --xAxisLabel Number of Jets --noXaxisUnit --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  p_qqww.h_nbtags->Add(p_wzz.h_nbtags);
  p_qqww.h_nbtags->Add(p_wwdps.h_nbtags);
  p_qqww.h_nbtags->Add(p_ttg.h_nbtags);
  p_qqww.h_nbtags->Add(p_dy.h_nbtags);
  p_qqww.h_nbtags->Add(p_vhnonbb.h_nbtags);
  p_qqww.h_nbtags->Add(p_tth.h_nbtags);
  p_qqww.h_nbtags->Add(p_wgamma.h_nbtags);
  p_qqww.h_nbtags->Add(p_tttt.h_nbtags);

  vector <TH1F* > nbtags_plots;
  nbtags_plots.push_back(p_ttw.h_nbtags);
  nbtags_plots.push_back(p_ttz.h_nbtags);
  nbtags_plots.push_back(p_qqww.h_nbtags);
  nbtags_plots.push_back(p_wz.h_nbtags);
  nbtags_plots.push_back(p_flips.h_nbtags);
  nbtags_plots.push_back(p_fakes.h_nbtags);
  dataMCplotMaker(p_data.h_nbtags, nbtags_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName nbtags_all_SS.pdf --xAxisLabel Number of BTags --noXaxisUnit --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  p_qqww.h_l1pt->Add(p_wzz.h_l1pt);
  p_qqww.h_l1pt->Add(p_wwdps.h_l1pt);
  p_qqww.h_l1pt->Add(p_ttg.h_l1pt);
  p_qqww.h_l1pt->Add(p_dy.h_l1pt);
  p_qqww.h_l1pt->Add(p_vhnonbb.h_l1pt);
  p_qqww.h_l1pt->Add(p_tth.h_l1pt);
  p_qqww.h_l1pt->Add(p_wgamma.h_l1pt);
  p_qqww.h_l1pt->Add(p_tttt.h_l1pt);
  
  //lep1 pt plots
  vector <TH1F* > l1pt_plots;
  l1pt_plots.push_back(p_ttw.h_l1pt);
  l1pt_plots.push_back(p_ttz.h_l1pt);
  l1pt_plots.push_back(p_qqww.h_l1pt);
  l1pt_plots.push_back(p_wz.h_l1pt);
  l1pt_plots.push_back(p_flips.h_l1pt);
  l1pt_plots.push_back(p_fakes.h_l1pt);
  dataMCplotMaker(p_data.h_l1pt, l1pt_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName l1pt_all_SS.pdf --xAxisLabel Leading Lepton pT --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F* > l1pt_plotsMC;
  l1pt_plotsMC.push_back(p_ttw.h_l1pt);
  l1pt_plotsMC.push_back(p_ttz.h_l1pt);
  l1pt_plotsMC.push_back(p_qqww.h_l1pt);
  l1pt_plotsMC.push_back(p_wz.h_l1pt);
  l1pt_plotsMC.push_back(p_ttbar_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_wjets_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_dy_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_st_ff.h_l1pt);
  dataMCplotMaker(p_data.h_l1pt, l1pt_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName l1pt_all_SSMC.pdf --xAxisLabel Leading Lepton pT --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  p_qqww.h_l2pt->Add(p_wzz.h_l2pt);
  p_qqww.h_l2pt->Add(p_wwdps.h_l2pt);
  p_qqww.h_l2pt->Add(p_ttg.h_l2pt);
  p_qqww.h_l2pt->Add(p_dy.h_l2pt);
  p_qqww.h_l2pt->Add(p_vhnonbb.h_l2pt);
  p_qqww.h_l2pt->Add(p_tth.h_l2pt);
  p_qqww.h_l2pt->Add(p_wgamma.h_l2pt);
  p_qqww.h_l2pt->Add(p_tttt.h_l2pt);
 
  //lep2 pt plots
  vector <TH1F*> l2pt_plots;
  l2pt_plots.push_back(p_ttw.h_l2pt);
  l2pt_plots.push_back(p_ttz.h_l2pt);
  l2pt_plots.push_back(p_qqww.h_l2pt);
  l2pt_plots.push_back(p_wz.h_l2pt);
  l2pt_plots.push_back(p_flips.h_l2pt);
  l2pt_plots.push_back(p_fakes.h_l2pt);
  dataMCplotMaker(p_data.h_l2pt, l2pt_plots, titles, "", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName l2pt_all_SS.pdf --xAxisLabel Trailing Lepton pT --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

  vector <TH1F*> l2pt_plotsMC;
  l2pt_plotsMC.push_back(p_ttw.h_l2pt);
  l2pt_plotsMC.push_back(p_ttz.h_l2pt);
  l2pt_plotsMC.push_back(p_qqww.h_l2pt);
  l2pt_plotsMC.push_back(p_wz.h_l2pt);
  l2pt_plotsMC.push_back(p_ttbar_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_wjets_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_st_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_dy_ff.h_l2pt);
  dataMCplotMaker(p_data.h_l2pt, l2pt_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --lumiUnit pb --outputName l2pt_all_SSMC.pdf --xAxisLabel Trailing Lepton pT --percentageInBox --legendUp -.05", lumiAG*1000)); // --setMinimum 0.1 --setMaximum 100

}

//doFakes = 1 for QCD, 2 for inSitu
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes, int exclude){

  yields_t y_result;
  plots_t  p_result;

  //Initialize
  y_result.EE    = 0;
  y_result.EM    = 0;
  y_result.MM    = 0;
  y_result.TOTAL = 0;

  p_result.h_ht           = new TH1F(Form("ht_%s"           , chain->GetTitle()) , Form("ht_%s"           , chain->GetTitle()) , 20  , 0 , 1000);
  p_result.h_met          = new TH1F(Form("met_%s"          , chain->GetTitle()) , Form("met_%s"          , chain->GetTitle()) , 15  , 0 , 300);
  p_result.h_mll          = new TH1F(Form("mll_%s"          , chain->GetTitle()) , Form("mll_%s"          , chain->GetTitle()) , 15  , 0 , 300);
  p_result.h_mtmin        = new TH1F(Form("mtmin_%s"        , chain->GetTitle()) , Form("mtmin_%s"        , chain->GetTitle()) , 10  , 0 , 200);
  p_result.h_njets        = new TH1F(Form("njets_%s"        , chain->GetTitle()) , Form("njets_%s"        , chain->GetTitle()) , 10  , 0 , 10);
  p_result.h_nbtags       = new TH1F(Form("nbtags_%s"       , chain->GetTitle()) , Form("nbtags_%s"       , chain->GetTitle()) , 7   , 0 , 7);
  p_result.h_l1pt         = new TH1F(Form("l1pt_%s"         , chain->GetTitle()) , Form("l1pt_%s"         , chain->GetTitle()) , 20  , 0 , 150);
  p_result.h_l2pt         = new TH1F(Form("l2pt_%s"         , chain->GetTitle()) , Form("l2pt_%s"         , chain->GetTitle()) , 20  , 0 , 100);
  p_result.SRHH.EE        = new TH1F(Form("SRHH_EE_%s"      , chain->GetTitle()) , Form("SRHH_EE_%s"      , chain->GetTitle()) , 32  , 1 , 33);
  p_result.SRHH.EM        = new TH1F(Form("SRHH_EM_%s"      , chain->GetTitle()) , Form("SRHH_EM_%s"      , chain->GetTitle()) , 32  , 1 , 33);
  p_result.SRHH.MM        = new TH1F(Form("SRHH_MM_%s"      , chain->GetTitle()) , Form("SRHH_MM_%s"      , chain->GetTitle()) , 32  , 1 , 33);
  p_result.SRHH.TOTAL     = new TH1F(Form("SRHH_TOTAL_%s"   , chain->GetTitle()) , Form("SRHH_TOTAL_%s"   , chain->GetTitle()) , 32  , 1 , 33);
  p_result.SRHL.EE        = new TH1F(Form("SRHL_EE_%s"      , chain->GetTitle()) , Form("SRHL_EE_%s"      , chain->GetTitle()) , 26  , 1 , 27);
  p_result.SRHL.EM        = new TH1F(Form("SRHL_EM_%s"      , chain->GetTitle()) , Form("SRHL_EM_%s"      , chain->GetTitle()) , 26  , 1 , 27);
  p_result.SRHL.MM        = new TH1F(Form("SRHL_MM_%s"      , chain->GetTitle()) , Form("SRHL_MM_%s"      , chain->GetTitle()) , 26  , 1 , 27);
  p_result.SRHL.TOTAL     = new TH1F(Form("SRHL_TOTAL_%s"   , chain->GetTitle()) , Form("SRHL_TOTAL_%s"   , chain->GetTitle()) , 26  , 1 , 27);
  p_result.SRLL.EE        = new TH1F(Form("SRLL_EE_%s"      , chain->GetTitle()) , Form("SRLL_EE_%s"      , chain->GetTitle()) , 8   , 1 , 9);
  p_result.SRLL.EM        = new TH1F(Form("SRLL_EM_%s"      , chain->GetTitle()) , Form("SRLL_EM_%s"      , chain->GetTitle()) , 8   , 1 , 9);
  p_result.SRLL.MM        = new TH1F(Form("SRLL_MM_%s"      , chain->GetTitle()) , Form("SRLL_MM_%s"      , chain->GetTitle()) , 8   , 1 , 9);
  p_result.SRLL.TOTAL     = new TH1F(Form("SRLL_TOTAL_%s"   , chain->GetTitle()) , Form("SRLL_TOTAL_%s"   , chain->GetTitle()) , 8   , 1 , 9);
  p_result.h_type         = new TH1F(Form("type_%s"         , chain->GetTitle()) , Form("type_%s"         , chain->GetTitle()) , 4   , 0 , 4);
  p_result.h_lep1_miniIso = new TH1F(Form("lep1_miniIso_%s" , chain->GetTitle()) , Form("lep1_miniIso_%s" , chain->GetTitle()) , 30 , 0 , 0.2);
  p_result.h_lep2_miniIso = new TH1F(Form("lep2_miniIso_%s" , chain->GetTitle()) , Form("lep2_miniIso_%s" , chain->GetTitle()) , 30 , 0 , 0.2);
  p_result.h_lep1_ptRatio = new TH1F(Form("lep1_ptRatio_%s" , chain->GetTitle()) , Form("lep1_ptRatio_%s" , chain->GetTitle()) , 30 , 0 , 1.5);
  p_result.h_lep2_ptRatio = new TH1F(Form("lep2_ptRatio_%s" , chain->GetTitle()) , Form("lep2_ptRatio_%s" , chain->GetTitle()) , 30 , 0 , 1.5);
  p_result.h_lep1_ptRel   = new TH1F(Form("lep1_ptRel_%s"   , chain->GetTitle()) , Form("lep1_ptRel_%s"   , chain->GetTitle()) , 25 , 0 , 25);
  p_result.h_lep2_ptRel   = new TH1F(Form("lep2_ptRel_%s"   , chain->GetTitle()) , Form("lep2_ptRel_%s"   , chain->GetTitle()) , 25 , 0 , 25);

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
      if (!doFlips && !doFakes && exclude == 0) {
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && ss::lep1_motherID()!=1) continue;
	    if (!isData && ss::lep2_motherID()!=1) continue;
      }

      if (!doFlips && !doFakes && exclude == 1){
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && !((ss::lep1_motherID() == 1 && ss::lep2_motherID() < 0) || (ss::lep1_motherID() < 0 && ss::lep2_motherID() == 1))) continue;
      }

      if (!doFlips && !doFakes && exclude == 2){
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && !((ss::lep1_motherID() == 1 && ss::lep2_motherID() == 2) || (ss::lep1_motherID() == 2 && ss::lep2_motherID() == 1))) continue;
      }

      if (!doFlips && !doFakes && exclude == 3){
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && !((ss::lep1_motherID() == 1 || ss::lep2_motherID() == 1))) continue;
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

      //Get pt ratio
      float ptratio_1 = ss::lep1_closeJet().pt() > 0 ? ss::lep1_p4().pt()/ss::lep1_closeJet().pt() : 1.0;
      float ptratio_2 = ss::lep2_closeJet().pt() > 0 ? ss::lep2_p4().pt()/ss::lep2_closeJet().pt() : 1.0; 

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
      p_result.h_ht          ->Fill(ss::ht_corr()                    , weight);
      p_result.h_met         ->Fill(ss::corrMET()                    , weight);
      p_result.h_mll         ->Fill((ss::lep1_p4()+ss::lep2_p4()).M(), weight);
      p_result.h_mtmin       ->Fill(ss::mtmin()                      , weight);
      p_result.h_njets       ->Fill(ss::njets_corr()                 , weight);
      p_result.h_nbtags      ->Fill(ss::nbtags_corr()                , weight);
      p_result.h_l1pt        ->Fill(ss::lep1_p4().pt()               , weight);
      p_result.h_l2pt        ->Fill(ss::lep2_p4().pt()               , weight);
      p_result.h_type        ->Fill(ss::hyp_type()                   , weight); 
      p_result.h_lep1_miniIso->Fill(ss::lep1_miniIso()               , weight); 
      p_result.h_lep2_miniIso->Fill(ss::lep2_miniIso()               , weight); 
      p_result.h_lep1_ptRel  ->Fill(ss::lep1_ptrel_v1()              , weight); 
      p_result.h_lep2_ptRel  ->Fill(ss::lep2_ptrel_v1()              , weight); 
      p_result.h_lep1_ptRatio->Fill(ptratio_1                        , weight); 
      p_result.h_lep2_ptRatio->Fill(ptratio_2                        , weight); 

      //Fill SR plots
      if (categ == HighHigh){
        if      (ss::hyp_type() == 3) p_result.SRHH.EE->Fill(SR, weight); 
        else if (ss::hyp_type() == 0) p_result.SRHH.MM->Fill(SR, weight); 
        else                          p_result.SRHH.EM->Fill(SR, weight); 
                                      p_result.SRHH.TOTAL->Fill(SR, weight); 
      }

      if (categ == HighLow){
        if      (ss::hyp_type() == 3) p_result.SRHL.EE->Fill(SR, weight); 
        else if (ss::hyp_type() == 0) p_result.SRHL.MM->Fill(SR, weight); 
        else                          p_result.SRHL.EM->Fill(SR, weight); 
                                      p_result.SRHL.TOTAL->Fill(SR, weight); 
      }

      if (categ == LowLow){
        if      (ss::hyp_type() == 3) p_result.SRLL.EE->Fill(SR, weight); 
        else if (ss::hyp_type() == 0) p_result.SRLL.MM->Fill(SR, weight); 
        else                          p_result.SRLL.EM->Fill(SR, weight); 
                                      p_result.SRLL.TOTAL->Fill(SR, weight); 
      }


    }//event loop
  }//file loop

  if (y_result.EE<0) y_result.EE=0.;
  if (y_result.MM<0) y_result.MM=0.;
  if (y_result.EM<0) y_result.EM=0.;
  if (y_result.TOTAL<0) y_result.TOTAL=0.;

  //Update total
  if (!exclude && (!isData || doFlips || doFakes == 1)){
    total.EE    += y_result.EE;
    total.EM    += y_result.EM;
    total.MM    += y_result.MM;
    total.TOTAL += y_result.TOTAL;
  }

  //Return yield
  return pair<yields_t, plots_t>(y_result,p_result); 

}
