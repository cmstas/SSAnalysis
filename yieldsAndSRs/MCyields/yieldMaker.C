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

//Root files on/off
bool makeRootFiles = 1;

float lumiAG = getLumiUnblind();
string tag = getTag().Data();  

bool doNoData = true;

float scaleLumi = 1.0;//3.0/1.264;//careful!!!

struct yields_t { float EE; float EM; float MM; float TOTAL; }; 
struct SR_t     { TH1F* EE; TH1F* EM; TH1F* MM; TH1F* TOTAL; }; 
struct plots_t  { TH1F* h_ht; TH1F* h_met; TH1F* h_mll; TH1F* h_mtmin; TH1F* h_njets; TH1F* h_nbtags; TH1F* h_l1pt; TH1F* h_l2pt; TH1F* h_type; TH1F* h_lep1_miniIso; TH1F* h_lep2_miniIso; TH1F* h_lep1_ptRatio; TH1F* h_lep2_ptRatio; TH1F* h_lep1_ptRel; TH1F* h_lep2_ptRel; SR_t SRHH; SR_t SRHL; SR_t SRLL; }; 

//Total
yields_t total; 

//function declaration
pair<yields_t, plots_t> run(TChain *chain, bool isData = 0, bool doFlips = 0, int doFakes = 0, int exclude = 0, bool isSignal = 0, bool isGamma = 0);
void avoidNegativeYields(TH1F* plot);
void writeStatUpDown(TH1F* central,string name,bool down);
void writeTTVExtrSyst(TH1F* central,string name,TString kine,bool down);
void writeTTVExtrSyst(TH1F* central,string name,TString kine,bool down);
void writeWZExtrSyst(TH1F* central,string name,TString kine,bool down);
void writeJesSyst(TH1F* central,string name,TString kine,bool down);
void writeLepSyst(TH1F* central,string name,TString kine,bool down);
void writeBtagSyst(TH1F* central,string name,TString kine,bool down);

void getyields(){

  if (doNoData) lumiAG = scaleLumi*getLumi();

  //Chains
  //fakes&flips in mc
  TChain *ttbar_chain   = new TChain("t","ttbar");
  TChain *st_chain      = new TChain("t","st");
  TChain *wjets_chain   = new TChain("t","wjets");
  TChain *dy_chain      = new TChain("t","dy");
  //rares
  TChain *ttw_chain     = new TChain("t","ttw");
  TChain *ttzh_chain    = new TChain("t","ttzh");
  TChain *wz_chain      = new TChain("t","wz");
  TChain *ww_chain      = new TChain("t","ww");
  TChain *tg_chain      = new TChain("t","tg");
  TChain *vg_chain      = new TChain("t","vg");
  TChain *rares_chain   = new TChain("t","rares");
  //data
  TChain *data_chain    = new TChain("t","data"); 
  TChain *flips_chain   = new TChain("t","flips"); 
  TChain *fakes_chain   = new TChain("t","fakes");
  //signals
  TChain *t1tttt_1200_chain = new TChain("t","t1tttt_1200"); 
  TChain *t1tttt_1500_chain = new TChain("t","t1tttt_1500"); 
  TChain *t5tttt_deg_chain = new TChain("t","t5tttt_deg"); 
  TChain *t6ttww_650_chain = new TChain("t","t6ttww_650"); 
  TChain *t6ttww_600_chain = new TChain("t","t6ttww_600"); 

  //Fill chains
  ttbar_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR.root"          , tag.c_str())); 
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP1.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP2.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP3.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP4.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP5.root"     , tag.c_str()));
  wjets_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets.root"          , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_high.root"        , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_low.root"         , tag.c_str()));

  ttw_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  ttzh_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  ttzh_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"     , tag.c_str()));
  wz_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ3LNU.root"         , tag.c_str()));
  ww_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WWDPS.root"          , tag.c_str()));
  ww_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"           , tag.c_str()));
  tg_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TG.root"             , tag.c_str()));
  tg_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTG.root"            , tag.c_str()));
  //missing ZG
  vg_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WGToLNuG.root"       , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/ZZ.root"             , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WWZ.root"            , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZZ.root"            , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/VHtoNonBB.root"      , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TZQ.root"            , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTTT.root"           , tag.c_str()));

  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-data1p280ifb/DataDoubleMuonD.root", tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-data1p280ifb/DataDoubleEGD.root"  , tag.c_str()));
  data_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-data1p280ifb/DataMuonEGD.root"    , tag.c_str()));
  flips_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-data1p280ifb/DataDoubleEGD.root"  , tag.c_str()));
  flips_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-data1p280ifb/DataMuonEGD.root"    , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-data1p280ifb/DataDoubleMuonD.root", tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-data1p280ifb/DataDoubleEGD.root"  , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-data1p280ifb/DataMuonEGD.root"    , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ3LNU.root"         , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"     , tag.c_str()));
  fakes_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"           , tag.c_str()));

  t1tttt_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T1TTTT_1200.root" , tag.c_str()));
  t1tttt_1500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T1TTTT_1500.root" , tag.c_str()));
  t5tttt_deg_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T5ttttDeg_1000_300_280.root" , tag.c_str()));
  t6ttww_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T6TTWW_650_150_50.root" , tag.c_str()));
  t6ttww_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T6TTWW_600_425_50.root" , tag.c_str()));

  //Get yields
  pair<yields_t, plots_t> results_ttbar    = run(ttbar_chain);
  ttbar_chain->SetTitle("ttbar_fa");
  pair<yields_t, plots_t> results_ttbar_fa = run(ttbar_chain, 0, 0, 0, 1);
  ttbar_chain->SetTitle("ttbar_fl");
  pair<yields_t, plots_t> results_ttbar_fl = run(ttbar_chain, 0, 0, 0, 2);
  ttbar_chain->SetTitle("ttbar_ff");
  pair<yields_t, plots_t> results_ttbar_ff = run(ttbar_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_st       = run(st_chain);
  st_chain->SetTitle("st_ff");
  pair<yields_t, plots_t> results_st_ff    = run(st_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_wjets    = run(wjets_chain);
  wjets_chain->SetTitle("wjets_ff");
  pair<yields_t, plots_t> results_wjets_ff = run(wjets_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_dy       = run(dy_chain);
  dy_chain->SetTitle("dy_ff");
  pair<yields_t, plots_t> results_dy_ff    = run(dy_chain, 0, 0, 0, 3);

  pair<yields_t, plots_t> results_ttw      = run(ttw_chain);
  pair<yields_t, plots_t> results_ttzh     = run(ttzh_chain);
  pair<yields_t, plots_t> results_wz       = run(wz_chain);
  pair<yields_t, plots_t> results_ww       = run(ww_chain);

  pair<yields_t, plots_t> results_tg       = run(tg_chain, 0, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_vg       = run(vg_chain, 0, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_rares    = run(rares_chain);

  pair<yields_t, plots_t> results_data     = run(data_chain, 1);
  duplicate_removal::clear_list();
  pair<yields_t, plots_t> results_flips    = run(flips_chain, 1, 1);
  duplicate_removal::clear_list();
  pair<yields_t, plots_t> results_fakes    = run(fakes_chain, 1, 0, 1);
  duplicate_removal::clear_list();
  fakes_chain->SetTitle("fakes_is");
  pair<yields_t, plots_t> results_fakes_is = run(fakes_chain, 1, 0, 2);

  pair<yields_t, plots_t> results_t1tttt_1200 = run(t1tttt_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t1tttt_1500 = run(t1tttt_1500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t5tttt_deg = run(t5tttt_deg_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t6ttww_650 = run(t6ttww_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t6ttww_600 = run(t6ttww_600_chain, 0, 0, 0, 0, 1);

  yields_t& ttbar    = results_ttbar.first;
  yields_t& ttbar_ff = results_ttbar_ff.first;
  yields_t& wjets_ff = results_wjets_ff.first;
  yields_t& dy_ff    = results_dy_ff.first;
  yields_t& st_ff    = results_st_ff.first;
  yields_t& ttbar_fa = results_ttbar_fa.first;
  yields_t& ttbar_fl = results_ttbar_fl.first;
  yields_t& st       = results_st.first;
  yields_t& wjets    = results_wjets.first;
  yields_t& dy       = results_dy.first;

  yields_t& ttw      = results_ttw.first;
  yields_t& ttzh     = results_ttzh.first;
  yields_t& wz       = results_wz.first;
  yields_t& ww       = results_ww.first;
  yields_t& tg       = results_tg.first;
  yields_t& vg       = results_vg.first;
  yields_t& rares    = results_rares.first;

  yields_t& data     = results_data.first;
  yields_t& flips    = results_flips.first;
  yields_t& fakes    = results_fakes.first;
  yields_t& fakes_is = results_fakes_is.first;

  yields_t& t1tttt_1200    = results_t1tttt_1200.first;
  yields_t& t6ttww_650    = results_t6ttww_650.first;

  //Make yield table
  CTable table; 
  table.setPrecision(2); 
  table.setTitle("SS Baseline Yields"); 
  table.useTitle(); 
  table.setTable() (                  "EE"  , "EM"       , "MM"         , "TOTAL"      )
                   ("ttw"      , ttw.EE      , ttw.EM      , ttw.MM      , ttw.TOTAL    )
                   ("ttzh"     , ttzh.EE     , ttzh.EM     , ttzh.MM     , ttzh.TOTAL   )
                   ("wz"       , wz.EE       , wz.EM       , wz.MM       , wz.TOTAL     )
                   ("ww"       , ww.EE       , ww.EM       , ww.MM       , ww.TOTAL     )
                   ("tg"       , tg.EE       , tg.EM       , tg.MM       , tg.TOTAL     )
                   ("vg"       , vg.EE       , vg.EM       , vg.MM       , vg.TOTAL     )
                   ("rares"    , rares.EE    , rares.EM    , rares.MM    , rares.TOTAL  )
                   ("flips"    , flips.EE    , flips.EM    , flips.MM    , flips.TOTAL  )
                   ("fakes"    , fakes.EE    , fakes.EM    , fakes.MM    , fakes.TOTAL  )
                   ("total"    , total.EE    , total.EM    , total.MM    , total.TOTAL  )
                   ("data"     , data.EE     , data.EM     , data.MM     , data.TOTAL   )
                   ("fakes_is" , fakes_is.EE , fakes_is.EM , fakes_is.MM , fakes_is.TOTAL  )
                   ("ttbar"    , ttbar.EE    , ttbar.EM    , ttbar.MM    , ttbar.TOTAL  )
                   ("ttbar_fa" , ttbar_fa.EE , ttbar_fa.EM , ttbar_fa.MM , ttbar_fa.TOTAL)
                   ("ttbar_fl" , ttbar_fl.EE , ttbar_fl.EM , ttbar_fl.MM , ttbar_fl.TOTAL)
                   ("st"       , st.EE       , st.EM       , st.MM       , st.TOTAL     )
                   ("wjets"    , wjets.EE    , wjets.EM    , wjets.MM    , wjets.TOTAL  )
                   ("dy"       , dy.EE       , dy.EM       , dy.MM       , dy.TOTAL     )
                   ("t1tttt_1200"    , t1tttt_1200.EE    , t1tttt_1200.EM    , t1tttt_1200.MM    , t1tttt_1200.TOTAL  )
                   ("t6ttww_650"    , t6ttww_650.EE    , t6ttww_650.EM    , t6ttww_650.MM    , t6ttww_650.TOTAL  );
  table.print();

  plots_t& p_ttbar    = results_ttbar.second;
  plots_t& p_ttbar_ff = results_ttbar_ff.second;
  plots_t& p_wjets_ff = results_wjets_ff.second;
  plots_t& p_dy_ff    = results_dy_ff.second;
  plots_t& p_st_ff    = results_st_ff.second;
  plots_t& p_ttbar_fa = results_ttbar_fa.second;
  plots_t& p_ttbar_fl = results_ttbar_fl.second;
  plots_t& p_st       = results_st.second;
  plots_t& p_wjets    = results_wjets.second;
  plots_t& p_dy       = results_dy.second;

  plots_t& p_ttw      = results_ttw.second;
  plots_t& p_ttzh     = results_ttzh.second;
  plots_t& p_wz       = results_wz.second;
  plots_t& p_ww       = results_ww.second;
  plots_t& p_tg       = results_tg.second;
  plots_t& p_vg       = results_vg.second;
  plots_t& p_rares    = results_rares.second;

  plots_t& p_data     = results_data.second;
  plots_t& p_flips    = results_flips.second;
  plots_t& p_fakes    = results_fakes.second;
  plots_t& p_fakes_is = results_fakes_is.second;

  plots_t& p_t1tttt_1200 = results_t1tttt_1200.second;

  //Titles for legend
  vector <string> titles;
  titles.push_back("ttW"); 
  titles.push_back("ttZ/H"); 
  titles.push_back("WZ"); 
  titles.push_back("WW"); 
  titles.push_back("TG"); 
  titles.push_back("VG"); 
  titles.push_back("Rares"); 
  titles.push_back("Flips"); 
  titles.push_back("Fakes"); 

  //Titles for MC-only plots
  vector <string> titles2;
  titles2.push_back("ttW"); 
  titles2.push_back("ttZ/H"); 
  titles2.push_back("WZ"); 
  titles2.push_back("WW"); 
  titles2.push_back("TG"); 
  titles2.push_back("VG"); 
  titles2.push_back("Rares"); 
  titles2.push_back("ttbar"); 
  titles2.push_back("dy"); 
  titles2.push_back("wjets"); 
  titles2.push_back("st"); 

  cout << endl;
  cout <<   Form("        %5s %5s %5s %5s %5s %5s %5s %5s %5s | %5s | %11s","TTW","TTZ/H","WZ","WW","TG","VG","RARES","FLIPS","FAKES","TOTAL","T1TTTT1200") << endl;
  for (int bin=1;bin<=p_ttw.SRHH.TOTAL->GetNbinsX(); ++bin) {
    cout << Form("HH SR%2i %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f | %5.2f |  %10.2f",bin,
		 p_ttw.SRHH.TOTAL->GetBinContent(bin),p_ttzh.SRHH.TOTAL->GetBinContent(bin),p_wz.SRHH.TOTAL->GetBinContent(bin),
		 p_ww.SRHH.TOTAL->GetBinContent(bin),p_tg.SRHH.TOTAL->GetBinContent(bin),p_vg.SRHH.TOTAL->GetBinContent(bin),
		 p_rares.SRHH.TOTAL->GetBinContent(bin),p_flips.SRHH.TOTAL->GetBinContent(bin),p_fakes.SRHH.TOTAL->GetBinContent(bin),
		 (p_ttw.SRHH.TOTAL->GetBinContent(bin)+p_ttzh.SRHH.TOTAL->GetBinContent(bin)+p_wz.SRHH.TOTAL->GetBinContent(bin)+
		  p_ww.SRHH.TOTAL->GetBinContent(bin)+p_tg.SRHH.TOTAL->GetBinContent(bin)+p_vg.SRHH.TOTAL->GetBinContent(bin)+
		  p_rares.SRHH.TOTAL->GetBinContent(bin)+p_flips.SRHH.TOTAL->GetBinContent(bin)+p_fakes.SRHH.TOTAL->GetBinContent(bin)),
		 p_t1tttt_1200.SRHH.TOTAL->GetBinContent(bin)
		 )
	 << endl;
  }

  cout << endl;
  cout <<   Form("        %5s %5s %5s %5s %5s %5s %5s %5s %5s | %5s | %11s","TTW","TTZ/H","WZ","WW","TG","VG","RARES","FLIPS","FAKES","TOTAL","T1TTTT1200") << endl;
  for (int bin=1;bin<=p_ttw.SRHL.TOTAL->GetNbinsX(); ++bin) {
    cout << Form("HL SR%2i %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f | %5.2f |  %10.2f",bin,
		 p_ttw.SRHL.TOTAL->GetBinContent(bin),p_ttzh.SRHL.TOTAL->GetBinContent(bin),p_wz.SRHL.TOTAL->GetBinContent(bin),
		 p_ww.SRHL.TOTAL->GetBinContent(bin),p_tg.SRHL.TOTAL->GetBinContent(bin),p_vg.SRHL.TOTAL->GetBinContent(bin),
		 p_rares.SRHL.TOTAL->GetBinContent(bin),p_flips.SRHL.TOTAL->GetBinContent(bin),p_fakes.SRHL.TOTAL->GetBinContent(bin),
		 (p_ttw.SRHL.TOTAL->GetBinContent(bin)+p_ttzh.SRHL.TOTAL->GetBinContent(bin)+p_wz.SRHL.TOTAL->GetBinContent(bin)+
		  p_ww.SRHL.TOTAL->GetBinContent(bin)+p_tg.SRHL.TOTAL->GetBinContent(bin)+p_vg.SRHL.TOTAL->GetBinContent(bin)+
		  p_rares.SRHL.TOTAL->GetBinContent(bin)+p_flips.SRHL.TOTAL->GetBinContent(bin)+p_fakes.SRHL.TOTAL->GetBinContent(bin)),
		 p_t1tttt_1200.SRHL.TOTAL->GetBinContent(bin)
		 )
	 << endl;
  }

  cout << endl;
  cout <<   Form("        %5s %5s %5s %5s %5s %5s %5s %5s %5s | %5s | %11s","TTW","TTZ/H","WZ","WW","TG","VG","RARES","FLIPS","FAKES","TOTAL","T1TTTT1200") << endl;
  for (int bin=1;bin<=p_ttw.SRLL.TOTAL->GetNbinsX(); ++bin) {
    cout << Form("LL SR%2i %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f | %5.2f |  %10.2f",bin,
		 p_ttw.SRLL.TOTAL->GetBinContent(bin),p_ttzh.SRLL.TOTAL->GetBinContent(bin),p_wz.SRLL.TOTAL->GetBinContent(bin),
		 p_ww.SRLL.TOTAL->GetBinContent(bin),p_tg.SRLL.TOTAL->GetBinContent(bin),p_vg.SRLL.TOTAL->GetBinContent(bin),
		 p_rares.SRLL.TOTAL->GetBinContent(bin),p_flips.SRLL.TOTAL->GetBinContent(bin),p_fakes.SRLL.TOTAL->GetBinContent(bin),
		 (p_ttw.SRLL.TOTAL->GetBinContent(bin)+p_ttzh.SRLL.TOTAL->GetBinContent(bin)+p_wz.SRLL.TOTAL->GetBinContent(bin)+
		  p_ww.SRLL.TOTAL->GetBinContent(bin)+p_tg.SRLL.TOTAL->GetBinContent(bin)+p_vg.SRLL.TOTAL->GetBinContent(bin)+
		  p_rares.SRLL.TOTAL->GetBinContent(bin)+p_flips.SRLL.TOTAL->GetBinContent(bin)+p_fakes.SRLL.TOTAL->GetBinContent(bin)),
		 p_t1tttt_1200.SRLL.TOTAL->GetBinContent(bin)
		 )
	 << endl;
  }

  //SR plots
  vector <TH1F*> SRHH_plots;
  SRHH_plots.push_back(p_ttw.SRHH.TOTAL  );
  SRHH_plots.push_back(p_ttzh.SRHH.TOTAL );
  SRHH_plots.push_back(p_wz.SRHH.TOTAL   );
  SRHH_plots.push_back(p_ww.SRHH.TOTAL );
  SRHH_plots.push_back(p_tg.SRHH.TOTAL );
  SRHH_plots.push_back(p_vg.SRHH.TOTAL );
  SRHH_plots.push_back(p_rares.SRHH.TOTAL   );
  SRHH_plots.push_back(p_flips.SRHH.TOTAL   );
  SRHH_plots.push_back(p_fakes.SRHH.TOTAL   );
  dataMCplotMaker(p_data.SRHH.TOTAL, SRHH_plots, titles, "H-H SRs", "data-driven", Form("--lumi %.1f --outputName HHSR.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow ", lumiAG));

  vector <TH1F*> SRHL_plots;
  SRHL_plots.push_back(p_ttw.SRHL.TOTAL  );
  SRHL_plots.push_back(p_ttzh.SRHL.TOTAL );
  SRHL_plots.push_back(p_wz.SRHL.TOTAL   );
  SRHL_plots.push_back(p_ww.SRHL.TOTAL );
  SRHL_plots.push_back(p_tg.SRHL.TOTAL );
  SRHL_plots.push_back(p_vg.SRHL.TOTAL );
  SRHL_plots.push_back(p_rares.SRHL.TOTAL   );
  SRHL_plots.push_back(p_flips.SRHL.TOTAL   );
  SRHL_plots.push_back(p_fakes.SRHL.TOTAL   );
  dataMCplotMaker(p_data.SRHL.TOTAL, SRHL_plots, titles, "H-L SRs", "data-driven", Form("--lumi %.1f --outputName HLSR.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG));

  vector <TH1F*> SRLL_plots;
  SRLL_plots.push_back(p_ttw.SRLL.TOTAL  );
  SRLL_plots.push_back(p_ttzh.SRLL.TOTAL );
  SRLL_plots.push_back(p_wz.SRLL.TOTAL   );
  SRLL_plots.push_back(p_ww.SRLL.TOTAL );
  SRLL_plots.push_back(p_tg.SRLL.TOTAL );
  SRLL_plots.push_back(p_vg.SRLL.TOTAL );
  SRLL_plots.push_back(p_rares.SRLL.TOTAL   );
  SRLL_plots.push_back(p_flips.SRLL.TOTAL   );
  SRLL_plots.push_back(p_fakes.SRLL.TOTAL   );
  dataMCplotMaker(p_data.SRLL.TOTAL, SRLL_plots, titles, "L-L SRs", "data-driven", Form("--lumi %.1f --outputName LLSR.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG));

  //SR plots
  vector <TH1F*> SRHHMC_plots;
  SRHHMC_plots.push_back(p_ttw.SRHH.TOTAL  );
  SRHHMC_plots.push_back(p_ttzh.SRHH.TOTAL );
  SRHHMC_plots.push_back(p_wz.SRHH.TOTAL   );
  SRHHMC_plots.push_back(p_ww.SRHH.TOTAL   );
  SRHHMC_plots.push_back(p_tg.SRHH.TOTAL   );
  SRHHMC_plots.push_back(p_vg.SRHH.TOTAL   );
  SRHHMC_plots.push_back(p_rares.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_ttbar_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_dy_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_wjets_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_st_ff.SRHH.TOTAL);
  dataMCplotMaker(p_data.SRHH.TOTAL, SRHHMC_plots, titles2, "H-H SRs", "from MC", Form("--lumi %.1f --outputName HHSRMC.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG));

  vector <TH1F*> SRHLMC_plots;
  SRHLMC_plots.push_back(p_ttw.SRHL.TOTAL  );
  SRHLMC_plots.push_back(p_ttzh.SRHL.TOTAL );
  SRHLMC_plots.push_back(p_wz.SRHL.TOTAL   );
  SRHLMC_plots.push_back(p_ww.SRHL.TOTAL   );
  SRHLMC_plots.push_back(p_tg.SRHL.TOTAL   );
  SRHLMC_plots.push_back(p_vg.SRHL.TOTAL   );
  SRHLMC_plots.push_back(p_rares.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_ttbar_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_dy_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_wjets_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_st_ff.SRHL.TOTAL);
  dataMCplotMaker(p_data.SRHL.TOTAL, SRHLMC_plots, titles2, "H-L SRs", "from MC", Form("--lumi %.1f --outputName HLSRMC.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG));

  vector <TH1F*> SRLLMC_plots;
  SRLLMC_plots.push_back(p_ttw.SRLL.TOTAL  );
  SRLLMC_plots.push_back(p_ttzh.SRLL.TOTAL );
  SRLLMC_plots.push_back(p_wz.SRLL.TOTAL   );
  SRLLMC_plots.push_back(p_ww.SRLL.TOTAL   );
  SRLLMC_plots.push_back(p_tg.SRLL.TOTAL   );
  SRLLMC_plots.push_back(p_vg.SRLL.TOTAL   );
  SRLLMC_plots.push_back(p_rares.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_ttbar_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_dy_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_wjets_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_st_ff.SRLL.TOTAL);
  dataMCplotMaker(p_data.SRLL.TOTAL, SRLLMC_plots, titles2, "L-L SRs", "from MC", Form("--lumi %.1f --outputName LLSRMC.pdf --xAxisLabel SR --percentageInBox --isLinear --legendUp -.05 --noOverflow", lumiAG));

  vector <TH1F* > type_plots;
  type_plots.push_back(p_ttw.h_type  );
  type_plots.push_back(p_ttzh.h_type );
  type_plots.push_back(p_wz.h_type   );
  type_plots.push_back(p_ww.h_type );
  type_plots.push_back(p_tg.h_type );
  type_plots.push_back(p_vg.h_type );
  type_plots.push_back(p_rares.h_type   );
  type_plots.push_back(p_flips.h_type   );
  type_plots.push_back(p_fakes.h_type   );
  dataMCplotMaker(p_data.h_type, type_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName type_all_SS.pdf --xAxisLabel type --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep1_miniIso_plots;
  lep1_miniIso_plots.push_back(p_ttw.h_lep1_miniIso  );
  lep1_miniIso_plots.push_back(p_ttzh.h_lep1_miniIso );
  lep1_miniIso_plots.push_back(p_wz.h_lep1_miniIso   );
  lep1_miniIso_plots.push_back(p_ww.h_lep1_miniIso );
  lep1_miniIso_plots.push_back(p_tg.h_lep1_miniIso );
  lep1_miniIso_plots.push_back(p_vg.h_lep1_miniIso );
  lep1_miniIso_plots.push_back(p_rares.h_lep1_miniIso   );
  lep1_miniIso_plots.push_back(p_flips.h_lep1_miniIso   );
  lep1_miniIso_plots.push_back(p_fakes.h_lep1_miniIso   );
  dataMCplotMaker(p_data.h_lep1_miniIso, lep1_miniIso_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName lep1_miniIso_all_SS.pdf --xAxisLabel lep1_miniIso --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep2_miniIso_plots;
  lep2_miniIso_plots.push_back(p_ttw.h_lep2_miniIso  );
  lep2_miniIso_plots.push_back(p_ttzh.h_lep2_miniIso );
  lep2_miniIso_plots.push_back(p_wz.h_lep2_miniIso   );
  lep2_miniIso_plots.push_back(p_ww.h_lep2_miniIso );
  lep2_miniIso_plots.push_back(p_tg.h_lep2_miniIso );
  lep2_miniIso_plots.push_back(p_vg.h_lep2_miniIso );
  lep2_miniIso_plots.push_back(p_rares.h_lep2_miniIso   );
  lep2_miniIso_plots.push_back(p_flips.h_lep2_miniIso   );
  lep2_miniIso_plots.push_back(p_fakes.h_lep2_miniIso   );
  dataMCplotMaker(p_data.h_lep2_miniIso, lep2_miniIso_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName lep2_miniIso_all_SS.pdf --xAxisLabel lep2_miniIso --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep1_ptRel_plots;
  lep1_ptRel_plots.push_back(p_ttw.h_lep1_ptRel  );
  lep1_ptRel_plots.push_back(p_ttzh.h_lep1_ptRel );
  lep1_ptRel_plots.push_back(p_wz.h_lep1_ptRel   );
  lep1_ptRel_plots.push_back(p_ww.h_lep1_ptRel );
  lep1_ptRel_plots.push_back(p_tg.h_lep1_ptRel );
  lep1_ptRel_plots.push_back(p_vg.h_lep1_ptRel );
  lep1_ptRel_plots.push_back(p_rares.h_lep1_ptRel   );
  lep1_ptRel_plots.push_back(p_flips.h_lep1_ptRel   );
  lep1_ptRel_plots.push_back(p_fakes.h_lep1_ptRel   );
  dataMCplotMaker(p_data.h_lep1_ptRel, lep1_ptRel_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName lep1_ptRel_all_SS.pdf --xAxisLabel lep1_ptRel --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep2_ptRel_plots;
  lep2_ptRel_plots.push_back(p_ttw.h_lep2_ptRel  );
  lep2_ptRel_plots.push_back(p_ttzh.h_lep2_ptRel );
  lep2_ptRel_plots.push_back(p_wz.h_lep2_ptRel   );
  lep2_ptRel_plots.push_back(p_ww.h_lep2_ptRel );
  lep2_ptRel_plots.push_back(p_tg.h_lep2_ptRel );
  lep2_ptRel_plots.push_back(p_vg.h_lep2_ptRel );
  lep2_ptRel_plots.push_back(p_rares.h_lep2_ptRel   );
  lep2_ptRel_plots.push_back(p_flips.h_lep2_ptRel   );
  lep2_ptRel_plots.push_back(p_fakes.h_lep2_ptRel   );
  dataMCplotMaker(p_data.h_lep2_ptRel, lep2_ptRel_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName lep2_ptRel_all_SS.pdf --xAxisLabel lep2_ptRel --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep1_ptRatio_plots;
  lep1_ptRatio_plots.push_back(p_ttw.h_lep1_ptRatio  );
  lep1_ptRatio_plots.push_back(p_ttzh.h_lep1_ptRatio );
  lep1_ptRatio_plots.push_back(p_wz.h_lep1_ptRatio   );
  lep1_ptRatio_plots.push_back(p_ww.h_lep1_ptRatio );
  lep1_ptRatio_plots.push_back(p_tg.h_lep1_ptRatio );
  lep1_ptRatio_plots.push_back(p_vg.h_lep1_ptRatio );
  lep1_ptRatio_plots.push_back(p_rares.h_lep1_ptRatio   );
  lep1_ptRatio_plots.push_back(p_flips.h_lep1_ptRatio   );
  lep1_ptRatio_plots.push_back(p_fakes.h_lep1_ptRatio   );
  dataMCplotMaker(p_data.h_lep1_ptRatio, lep1_ptRatio_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName lep1_ptRatio_all_SS.pdf --xAxisLabel lep1_ptRatio --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep2_ptRatio_plots;
  lep2_ptRatio_plots.push_back(p_ttw.h_lep2_ptRatio  );
  lep2_ptRatio_plots.push_back(p_ttzh.h_lep2_ptRatio );
  lep2_ptRatio_plots.push_back(p_wz.h_lep2_ptRatio   );
  lep2_ptRatio_plots.push_back(p_ww.h_lep2_ptRatio );
  lep2_ptRatio_plots.push_back(p_tg.h_lep2_ptRatio );
  lep2_ptRatio_plots.push_back(p_vg.h_lep2_ptRatio );
  lep2_ptRatio_plots.push_back(p_rares.h_lep2_ptRatio   );
  lep2_ptRatio_plots.push_back(p_flips.h_lep2_ptRatio   );
  lep2_ptRatio_plots.push_back(p_fakes.h_lep2_ptRatio   );
  dataMCplotMaker(p_data.h_lep2_ptRatio, lep2_ptRatio_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName lep2_ptRatio_all_SS.pdf --xAxisLabel lep2_ptRatio --isLinear --percentageInBox --legendUp -.05", lumiAG));
  
  //Redo iso with MC only
  vector <TH1F* > lep1_miniIso_plotsMC;
  lep1_miniIso_plotsMC.push_back(p_ttw.h_lep1_miniIso  );
  lep1_miniIso_plotsMC.push_back(p_ttzh.h_lep1_miniIso );
  lep1_miniIso_plotsMC.push_back(p_wz.h_lep1_miniIso   );
  lep1_miniIso_plotsMC.push_back(p_ww.h_lep1_miniIso   );
  lep1_miniIso_plotsMC.push_back(p_tg.h_lep1_miniIso   );
  lep1_miniIso_plotsMC.push_back(p_vg.h_lep1_miniIso   );
  lep1_miniIso_plotsMC.push_back(p_rares.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_ttbar_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_dy_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_wjets_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_st_ff.h_lep1_miniIso);
  dataMCplotMaker(p_data.h_lep1_miniIso, lep1_miniIso_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --outputName lep1_miniIso_all_SSMC.pdf --xAxisLabel lep1_miniIso --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep2_miniIso_plotsMC;
  lep2_miniIso_plotsMC.push_back(p_ttw.h_lep2_miniIso  );
  lep2_miniIso_plotsMC.push_back(p_ttzh.h_lep2_miniIso );
  lep2_miniIso_plotsMC.push_back(p_wz.h_lep2_miniIso   );
  lep2_miniIso_plotsMC.push_back(p_ww.h_lep2_miniIso   );
  lep2_miniIso_plotsMC.push_back(p_tg.h_lep2_miniIso   );
  lep2_miniIso_plotsMC.push_back(p_vg.h_lep2_miniIso   );
  lep2_miniIso_plotsMC.push_back(p_rares.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_ttbar_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_dy_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_wjets_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_st_ff.h_lep2_miniIso);
  dataMCplotMaker(p_data.h_lep2_miniIso, lep2_miniIso_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --outputName lep2_miniIso_all_SSMC.pdf --xAxisLabel lep2_miniIso --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep1_ptRel_plotsMC;
  lep1_ptRel_plotsMC.push_back(p_ttw.h_lep1_ptRel  );
  lep1_ptRel_plotsMC.push_back(p_ttzh.h_lep1_ptRel );
  lep1_ptRel_plotsMC.push_back(p_wz.h_lep1_ptRel   );
  lep1_ptRel_plotsMC.push_back(p_ww.h_lep1_ptRel   );
  lep1_ptRel_plotsMC.push_back(p_tg.h_lep1_ptRel   );
  lep1_ptRel_plotsMC.push_back(p_vg.h_lep1_ptRel   );
  lep1_ptRel_plotsMC.push_back(p_rares.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_ttbar_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_dy_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_wjets_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_st_ff.h_lep1_ptRel);
  dataMCplotMaker(p_data.h_lep1_ptRel, lep1_ptRel_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --outputName lep1_ptRel_all_SSMC.pdf --xAxisLabel lep1_ptRel --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep2_ptRel_plotsMC;
  lep2_ptRel_plotsMC.push_back(p_ttw.h_lep2_ptRel  );
  lep2_ptRel_plotsMC.push_back(p_ttzh.h_lep2_ptRel );
  lep2_ptRel_plotsMC.push_back(p_wz.h_lep2_ptRel   );
  lep2_ptRel_plotsMC.push_back(p_ww.h_lep2_ptRel   );
  lep2_ptRel_plotsMC.push_back(p_tg.h_lep2_ptRel   );
  lep2_ptRel_plotsMC.push_back(p_vg.h_lep2_ptRel   );
  lep2_ptRel_plotsMC.push_back(p_rares.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_ttbar_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_dy_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_wjets_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_st_ff.h_lep2_ptRel);
  dataMCplotMaker(p_data.h_lep2_ptRel, lep2_ptRel_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --outputName lep2_ptRel_all_SSMC.pdf --xAxisLabel lep2_ptRel --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep1_ptRatio_plotsMC;
  lep1_ptRatio_plotsMC.push_back(p_ttw.h_lep1_ptRatio  );
  lep1_ptRatio_plotsMC.push_back(p_ttzh.h_lep1_ptRatio );
  lep1_ptRatio_plotsMC.push_back(p_wz.h_lep1_ptRatio   );
  lep1_ptRatio_plotsMC.push_back(p_ww.h_lep1_ptRatio   );
  lep1_ptRatio_plotsMC.push_back(p_tg.h_lep1_ptRatio   );
  lep1_ptRatio_plotsMC.push_back(p_vg.h_lep1_ptRatio   );
  lep1_ptRatio_plotsMC.push_back(p_rares.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_ttbar_ff.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_dy_ff.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_wjets_ff.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_st_ff.h_lep1_ptRatio);
  dataMCplotMaker(p_data.h_lep1_ptRatio, lep1_ptRatio_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --outputName lep1_ptRatio_all_SSMC.pdf --xAxisLabel lep1_ptRatio --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > lep2_ptRatio_plotsMC;
  lep2_ptRatio_plotsMC.push_back(p_ttw.h_lep2_ptRatio  );
  lep2_ptRatio_plotsMC.push_back(p_ttzh.h_lep2_ptRatio );
  lep2_ptRatio_plotsMC.push_back(p_wz.h_lep2_ptRatio   );
  lep2_ptRatio_plotsMC.push_back(p_ww.h_lep2_ptRatio   );
  lep2_ptRatio_plotsMC.push_back(p_tg.h_lep2_ptRatio   );
  lep2_ptRatio_plotsMC.push_back(p_vg.h_lep2_ptRatio   );
  lep2_ptRatio_plotsMC.push_back(p_rares.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_ttbar_ff.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_dy_ff.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_wjets_ff.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_st_ff.h_lep2_ptRatio);
  dataMCplotMaker(p_data.h_lep2_ptRatio, lep2_ptRatio_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --outputName lep2_ptRatio_all_SSMC.pdf --xAxisLabel lep2_ptRatio --isLinear --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > ht_plots;
  ht_plots.push_back(p_ttw.h_ht  );
  ht_plots.push_back(p_ttzh.h_ht );
  ht_plots.push_back(p_wz.h_ht   );
  ht_plots.push_back(p_ww.h_ht );
  ht_plots.push_back(p_tg.h_ht );
  ht_plots.push_back(p_vg.h_ht );
  ht_plots.push_back(p_rares.h_ht   );
  ht_plots.push_back(p_flips.h_ht   );
  ht_plots.push_back(p_fakes.h_ht   );
  dataMCplotMaker(p_data.h_ht, ht_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName ht_all_SS.pdf --xAxisLabel HT --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > met_plots;
  met_plots.push_back(p_ttw.h_met  );
  met_plots.push_back(p_ttzh.h_met );
  met_plots.push_back(p_wz.h_met   );
  met_plots.push_back(p_ww.h_met );
  met_plots.push_back(p_tg.h_met );
  met_plots.push_back(p_vg.h_met );
  met_plots.push_back(p_rares.h_met   );
  met_plots.push_back(p_flips.h_met   );
  met_plots.push_back(p_fakes.h_met   );
  dataMCplotMaker(p_data.h_met, met_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName met_all_SS.pdf --xAxisLabel MET --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > mll_plots;
  mll_plots.push_back(p_ttw.h_mll  );
  mll_plots.push_back(p_ttzh.h_mll );
  mll_plots.push_back(p_wz.h_mll   );
  mll_plots.push_back(p_ww.h_mll );
  mll_plots.push_back(p_tg.h_mll );
  mll_plots.push_back(p_vg.h_mll );
  mll_plots.push_back(p_rares.h_mll   );
  mll_plots.push_back(p_flips.h_mll   );
  mll_plots.push_back(p_fakes.h_mll   );
  dataMCplotMaker(p_data.h_mll, mll_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName mll_all_SS.pdf --xAxisLabel M_{LL} --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > mtmin_plots;
  mtmin_plots.push_back(p_ttw.h_mtmin  );
  mtmin_plots.push_back(p_ttzh.h_mtmin );
  mtmin_plots.push_back(p_wz.h_mtmin   );
  mtmin_plots.push_back(p_ww.h_mtmin );
  mtmin_plots.push_back(p_tg.h_mtmin );
  mtmin_plots.push_back(p_vg.h_mtmin );
  mtmin_plots.push_back(p_rares.h_mtmin   );
  mtmin_plots.push_back(p_flips.h_mtmin   );
  mtmin_plots.push_back(p_fakes.h_mtmin   );
  dataMCplotMaker(p_data.h_mtmin, mtmin_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName mtmin_all_SS.pdf --xAxisLabel MT^{min} --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > njets_plots;
  njets_plots.push_back(p_ttw.h_njets  );
  njets_plots.push_back(p_ttzh.h_njets );
  njets_plots.push_back(p_wz.h_njets   );
  njets_plots.push_back(p_ww.h_njets );
  njets_plots.push_back(p_tg.h_njets );
  njets_plots.push_back(p_vg.h_njets );
  njets_plots.push_back(p_rares.h_njets   );
  njets_plots.push_back(p_flips.h_njets   );
  njets_plots.push_back(p_fakes.h_njets   );
  dataMCplotMaker(p_data.h_njets, njets_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName njets_all_SS.pdf --xAxisLabel Number of Jets --noXaxisUnit --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > nbtags_plots;
  nbtags_plots.push_back(p_ttw.h_nbtags  );
  nbtags_plots.push_back(p_ttzh.h_nbtags );
  nbtags_plots.push_back(p_wz.h_nbtags   );
  nbtags_plots.push_back(p_ww.h_nbtags );
  nbtags_plots.push_back(p_tg.h_nbtags );
  nbtags_plots.push_back(p_vg.h_nbtags );
  nbtags_plots.push_back(p_rares.h_nbtags   );
  nbtags_plots.push_back(p_flips.h_nbtags   );
  nbtags_plots.push_back(p_fakes.h_nbtags   );
  dataMCplotMaker(p_data.h_nbtags, nbtags_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName nbtags_all_SS.pdf --xAxisLabel Number of BTags --noXaxisUnit --percentageInBox --legendUp -.05", lumiAG));

  //lep1 pt plots
  vector <TH1F* > l1pt_plots;
  l1pt_plots.push_back(p_ttw.h_l1pt  );
  l1pt_plots.push_back(p_ttzh.h_l1pt );
  l1pt_plots.push_back(p_wz.h_l1pt   );
  l1pt_plots.push_back(p_ww.h_l1pt );
  l1pt_plots.push_back(p_tg.h_l1pt );
  l1pt_plots.push_back(p_vg.h_l1pt );
  l1pt_plots.push_back(p_rares.h_l1pt   );
  l1pt_plots.push_back(p_flips.h_l1pt   );
  l1pt_plots.push_back(p_fakes.h_l1pt   );
  dataMCplotMaker(p_data.h_l1pt, l1pt_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName l1pt_all_SS.pdf --xAxisLabel Leading Lepton pT --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F* > l1pt_plotsMC;
  l1pt_plotsMC.push_back(p_ttw.h_l1pt  );
  l1pt_plotsMC.push_back(p_ttzh.h_l1pt );
  l1pt_plotsMC.push_back(p_wz.h_l1pt   );
  l1pt_plotsMC.push_back(p_ww.h_l1pt   );
  l1pt_plotsMC.push_back(p_tg.h_l1pt   );
  l1pt_plotsMC.push_back(p_vg.h_l1pt   );
  l1pt_plotsMC.push_back(p_rares.h_l1pt);
  l1pt_plotsMC.push_back(p_ttbar_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_dy_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_wjets_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_st_ff.h_l1pt);
  dataMCplotMaker(p_data.h_l1pt, l1pt_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --outputName l1pt_all_SSMC.pdf --xAxisLabel Leading Lepton pT --percentageInBox --legendUp -.05", lumiAG));
 
  //lep2 pt plots
  vector <TH1F*> l2pt_plots;
  l2pt_plots.push_back(p_ttw.h_l2pt  );
  l2pt_plots.push_back(p_ttzh.h_l2pt );
  l2pt_plots.push_back(p_wz.h_l2pt   );
  l2pt_plots.push_back(p_ww.h_l2pt );
  l2pt_plots.push_back(p_tg.h_l2pt );
  l2pt_plots.push_back(p_vg.h_l2pt );
  l2pt_plots.push_back(p_rares.h_l2pt   );
  l2pt_plots.push_back(p_flips.h_l2pt   );
  l2pt_plots.push_back(p_fakes.h_l2pt   );
  dataMCplotMaker(p_data.h_l2pt, l2pt_plots, titles, "", "SS Baseline", Form("--lumi %.1f --outputName l2pt_all_SS.pdf --xAxisLabel Trailing Lepton pT --percentageInBox --legendUp -.05", lumiAG));

  vector <TH1F*> l2pt_plotsMC;
  l2pt_plotsMC.push_back(p_ttw.h_l2pt  );
  l2pt_plotsMC.push_back(p_ttzh.h_l2pt );
  l2pt_plotsMC.push_back(p_wz.h_l2pt   );
  l2pt_plotsMC.push_back(p_ww.h_l2pt   );
  l2pt_plotsMC.push_back(p_tg.h_l2pt   );
  l2pt_plotsMC.push_back(p_vg.h_l2pt   );
  l2pt_plotsMC.push_back(p_rares.h_l2pt);
  l2pt_plotsMC.push_back(p_ttbar_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_dy_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_wjets_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_st_ff.h_l2pt);
  dataMCplotMaker(p_data.h_l2pt, l2pt_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.1f --outputName l2pt_all_SSMC.pdf --xAxisLabel Trailing Lepton pT --percentageInBox --legendUp -.05", lumiAG)); 
}

//doFakes = 1 for QCD, 2 for inSitu
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes, int exclude, bool isSignal, bool isGamma){

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
      weight*=scaleLumi;

      if (doNoData) {
	if (ss::is_real_data() && !doFlips && !doFakes) continue;
      } else if (ss::is_real_data() && !isUnblindRun(ss::run())) continue;

      //Reject not triggered
      if (!ss::fired_trigger()) continue;

      //Progress
      //SSAG::progress(nEventsTotal, nEventsChain);

      //electron FO is tighter for iso triggers, make sure it is passed
      if (ss::ht_corr()<300.) {
	if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA())) continue;
	if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA())) continue;
      } 

      //drop electrons below 15 GeV
      if (doFakes==1) {
	if (abs(ss::lep1_id())==11 && ss::lep1_coneCorrPt()<15) continue;
	if (abs(ss::lep2_id())==11 && ss::lep2_coneCorrPt()<15) continue;	
      } else {
	if (abs(ss::lep1_id())==11 && ss::lep1_p4().pt()<15) continue;
	if (abs(ss::lep2_id())==11 && ss::lep2_p4().pt()<15) continue;
      }

      //Only keep good events
      if (!doFlips && !doFakes && exclude == 0) {
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && !isGamma && ss::lep1_motherID()!=1) continue;
	    if (!isData && !isGamma && ss::lep2_motherID()!=1) continue;
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
        if (doFakes == 1 && ss::hyp_class() != 2) continue;
        if (doFakes == 2 && ss::hyp_class() != 5) continue;
        float fr = 0.;
        if (ss::lep1_passes_id()==0){
          if (doFakes == 1) fr = fakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht_corr());
          if (doFakes == 2) fr = fakeRateInSitu(ss::lep1_id(),ss::lep1_p4().pt(), ss::lep1_p4().eta());
        }
        if (ss::lep2_passes_id()==0){
          if (doFakes == 1) fr = fakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht_corr());
          if (doFakes == 2) fr = fakeRateInSitu(ss::lep2_id(),ss::lep2_p4().pt(), ss::lep2_p4().eta());
        }
        weight *= fr/(1.-fr);
        if (!ss::is_real_data()) weight *= -1.;
      }

      //Get pt ratio
      float ptratio_1 = ss::lep1_closeJet().pt() > 0 ? ss::lep1_p4().pt()/ss::lep1_closeJet().pt() : 1.0;
      float ptratio_2 = ss::lep2_closeJet().pt() > 0 ? ss::lep2_p4().pt()/ss::lep2_closeJet().pt() : 1.0; 

      //Reject duplicates (after selection otherwise flips are ignored...)
      if (isData && ss::is_real_data()){
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

  avoidNegativeYields(p_result.SRHH.TOTAL);
  avoidNegativeYields(p_result.SRHL.TOTAL);
  avoidNegativeYields(p_result.SRLL.TOTAL);

  //Update total
  if (!exclude && (!isData || doFlips || doFakes == 1)){
    total.EE    += y_result.EE;
    total.EM    += y_result.EM;
    total.MM    += y_result.MM;
    total.TOTAL += y_result.TOTAL;
  }

  //now save the root files with nominal hists and shape variations
  //Make root files
  if (makeRootFiles) {

    TString kinRegs[] = {"hihi","hilow","lowlow"};

    for (int kr = 0; kr<3;kr++) {

      string name = chain->GetTitle();

      TFile *fileOut = TFile::Open(Form("../../cards/%s/%s_histos_%s_%.1fifb.root",tag.c_str(),name.c_str(),kinRegs[kr].Data(),lumiAG),"RECREATE");
      TH1F* plot = 0;
      if (kinRegs[kr]=="hihi") plot=p_result.SRHH.TOTAL;
      else if (kinRegs[kr]=="hilow") plot=p_result.SRHL.TOTAL;
      else if (kinRegs[kr]=="lowlow") plot=p_result.SRLL.TOTAL;
      else exit(1);
      TH1F* h_sr = (TH1F*) plot->Clone("sr");
      h_sr->Write();
      
      //now do systematics
      
      //mc stats
      writeStatUpDown(h_sr,name,0);
      writeStatUpDown(h_sr,name,1);
      
      //ttv
      if (name=="ttw" || name=="ttz") {
	writeTTVExtrSyst(h_sr,name,kinRegs[kr],0);
	writeTTVExtrSyst(h_sr,name,kinRegs[kr],1);
      }
      
      //wz
      // if (name=="wz") {
      // 	writeWZExtrSyst(h_sr,name,kinRegs[kr],0);
      // 	writeWZExtrSyst(h_sr,name,kinRegs[kr],1);
      // }
      
      if (!isData && !doFlips && !doFakes) {
	//jes
	writeJesSyst(h_sr,name,kinRegs[kr],0);
	writeJesSyst(h_sr,name,kinRegs[kr],1);
	//btag
	writeBtagSyst(h_sr,name,kinRegs[kr],0);
	writeBtagSyst(h_sr,name,kinRegs[kr],1);
	//leptons
	writeLepSyst(h_sr,name,kinRegs[kr],0);
	writeLepSyst(h_sr,name,kinRegs[kr],1);
      }
      
      //end systematics
      
      fileOut->Close();
    }
    
  }

  //Return yield
  return pair<yields_t, plots_t>(y_result,p_result); 

}

void avoidNegativeYields(TH1F* plot) {
  for (int bin=1;bin<=plot->GetNbinsX();++bin) {
    if (plot->GetBinContent(bin)<0) {
      cout << "warning: plot " << plot->GetName() << " has negative yield in bin " << bin << " value=" << plot->GetBinContent(bin) << "; setting to zero."<< endl;
      plot->SetBinContent(bin,0);
    } 
  }
}

void writeStatUpDown(TH1F* central,string name,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* statUpDown = new TH1F(Form("%s_stat%s",name.c_str(),updown.Data()),
			      Form("%s_stat%s",name.c_str(),updown.Data()),
			      central->GetNbinsX(),central->GetXaxis()->GetXmin(),central->GetXaxis()->GetXmax());
  for (int bin=1;bin<=statUpDown->GetNbinsX();++bin) {
    float val = down ? (central->GetBinContent(bin)-central->GetBinError(bin)) : (central->GetBinContent(bin)+central->GetBinError(bin));
    if (val>0) statUpDown->SetBinContent(bin,val);
    else statUpDown->SetBinContent(bin,1E-6);
  }
  statUpDown->Write();
}

void writeTTVExtrSyst(TH1F* central,string name,TString kine,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";

  if (kine.Contains("lowlow")) {
    TH1F* systUpDown = (TH1F*) central->Clone(Form("%s_extr_ll%s",name.c_str(),updown.Data()));
    systUpDown->SetTitle(Form("%s_extr_ll%s",name.c_str(),updown.Data()));
    float systValue = 1.08;
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
    systUpDown->Write();
  } else {
    //hihi and hilow have different extrapolation unceratinties for hi and low HT
    TH1F* systUpDownHTH = (TH1F*) central->Clone(Form("%s_extr_hth%s",name.c_str(),updown.Data()));
    systUpDownHTH->SetTitle(Form("%s_extr_hth%s",name.c_str(),updown.Data()));
    float systValue = 1.08;
    for (int bin=1;bin<=systUpDownHTH->GetNbinsX();++bin) {
      //skip low ht regions
      if (kine.Contains("hihi")) {
	if (bin==1 || bin==3 || bin==9 || bin==11 || bin==17 || bin==19 || bin==25 || bin==27 || bin==29) continue;
      }
      if (kine.Contains("hilow")) {
	if (bin==1 || bin==3 || bin==7 || bin==9 || bin==13 || bin==15 || bin==19 || bin==21 || bin==23) continue;
      }
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDownHTH->SetBinContent(bin,val);
    }
    systUpDownHTH->Write();
    TH1F* systUpDownHTL = (TH1F*) central->Clone(Form("%s_extr_htl%s",name.c_str(),updown.Data()));
    systUpDownHTL->SetTitle(Form("%s_extr_htl%s",name.c_str(),updown.Data()));
    systValue = 1.08;
    for (int bin=1;bin<=systUpDownHTL->GetNbinsX();++bin) {
      //skip high ht regions (i.e. negate requirement used above)
      if (kine.Contains("hihi")) {
	if (!(bin==1 || bin==3 || bin==9 || bin==11 || bin==17 || bin==19 || bin==25 || bin==27 || bin==29)) continue;
      }
      if (kine.Contains("hilow")) {
	if (!(bin==1 || bin==3 || bin==7 || bin==9 || bin==13 || bin==15 || bin==19 || bin==21 || bin==23)) continue;
      }
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDownHTL->SetBinContent(bin,val);
    }
    systUpDownHTL->Write();
  }
}

void writeWZExtrSyst(TH1F* central,string name,TString kine,bool down) {
  //deprecated!
  return;
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* systUpDown = (TH1F*) central->Clone(Form("%s_extr%s",name.c_str(),updown.Data()));
  systUpDown->SetTitle(Form("%s_extr%s",name.c_str(),updown.Data()));
  float systValue = 1.15;//15%
  for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
    if (kine.Contains("hihi")) {
      if (bin<9) continue;
    }
    if (kine.Contains("hilow")) {
      if (bin<7) continue;
    }
    if (kine.Contains("lowlow")) {
      if (bin<3) continue;
    }
    float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
    if (val>0) systUpDown->SetBinContent(bin,val);
  }
  systUpDown->Write();
}

void writeLepSyst(TH1F* central,string name,TString kine,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* systUpDown = (TH1F*) central->Clone(Form("lep%s",updown.Data()));
  systUpDown->SetTitle(Form("lep%s",updown.Data()));
  float systValue = 1.;

  if (kine.Contains("hihi")) {
    systValue = 1.10;
  }
  if (kine.Contains("hilow")) {
    systValue = 1.15;
  }
  if (kine.Contains("lowlow")) {
    systValue = 1.20;
  }
  for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
    float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
    if (val>0) systUpDown->SetBinContent(bin,val);
  }
  systUpDown->Write();
}

void writeJesSyst(TH1F* central,string name,TString kine,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* systUpDown = (TH1F*) central->Clone(Form("jes%s",updown.Data()));
  systUpDown->SetTitle(Form("jes%s",updown.Data()));
  float systValue = 1.;

  if (kine.Contains("hihi")) {
    systValue = 1.08;
  }
  if (kine.Contains("hilow")) {
    systValue = 1.08;
  }
  if (kine.Contains("lowlow")) {
    systValue = 1.15;
  }
  for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
    float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
    if (val>0) systUpDown->SetBinContent(bin,val);
  }

  // //TTV
  // if (TString(name)=="ttw" || TString(name)=="ttz") {
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     if (kine.Contains("hihi")) {
  // 	if (bin==1 || bin==3 || bin==9 || bin==11 || bin==17 || bin==19 || bin==25 || bin==27 || bin==29) systValue=1.05;
  // 	if (bin==2 || bin==5 || bin==7 || bin==10 || bin==13 || bin==15 || bin==18 || bin==21 || bin==23 || 
  // 	    bin==26 || bin==28 || bin==30 || bin==31 || bin==32) systValue=1.08;
  // 	if (bin==4 || bin==6 || bin==8 || bin==12 || bin==14 || bin==16 || bin==20 || bin==22 || bin==24) systValue=1.04;
  //     }
  //     if (kine.Contains("hilow")) {
  // 	if (bin==1 || bin==3 || bin==7 || bin==9 || bin==13 || bin==15 || bin==19 || bin==21 || bin==23) systValue=1.03;
  // 	else systValue = 1.08;
  //     }
  //     if (kine.Contains("lowlow")) {
  // 	systValue = 1.15;
  //     }
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  // if (TString(name)=="t1tttt_1500") {
  //   systValue = 1.06;
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  // if (TString(name)=="t1tttt_1200") {
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     if (kine.Contains("hihi")) {
  // 	if (bin==20 || bin==26 || bin==28 || bin==30) systValue = 1.03;
  // 	else systValue = 1.10;
  //     }
  //     if (kine.Contains("hilow")) {
  // 	if (bin==16 || bin==22 || bin==24) systValue = 1.03;
  // 	else systValue = 1.05;
  //     } else systValue = 1.00;
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  // if (TString(name)=="t1ttbbww_1000" || TString(name)=="t1ttbbww_1300") {
  //   systValue = 1.08;
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  // if (TString(name)=="t5tttt_1000") {
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     if (kine.Contains("hihi")) {
  // 	if (bin==1 || bin==3 || bin==9 || bin==11 || bin==17 || bin==19 || bin==25 || bin==27 || bin==29) systValue = 1.05;
  // 	else if (bin==31) systValue = 1.10;
  // 	else systValue = 1.03;
  //     }
  //     if (kine.Contains("hilow")) {
  // 	if (bin==1 || bin==3 || bin==7 || bin==9 || bin==13 || bin==15 || bin==19 || bin==21 || bin==23) systValue = 1.05;
  // 	else systValue = 1.10;
  //     }
  //     if (kine.Contains("lowlow")) {
  // 	systValue = 1.10;
  //     }
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  // if (TString(name)=="t5qqww_1200") {
  //   if (kine.Contains("hihi")) {
  //     systValue = 1.07;
  //   }
  //   if (kine.Contains("hilow")) {
  //     systValue = 1.10;
  //   }
  //   if (kine.Contains("lowlow")) {
  //     systValue = 1.20;
  //   }
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  // if (TString(name)=="t5qqww_1500") {
  //   systValue = 1.05;
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  // if (TString(name)=="t5qqww_deg") {
  //   if (kine.Contains("hihi")) {
  //     systValue = 1.15;
  //   }
  //   if (kine.Contains("hilow")) {
  //     systValue = 1.10;
  //   }
  //   if (kine.Contains("lowlow")) {
  //     systValue = 1.02;
  //   }
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  // if (TString(name)=="t6ttww_600" || TString(name)=="t6ttww_650") {
  //   systValue = 1.10;
  //   for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
  //     float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
  //     if (val>0) systUpDown->SetBinContent(bin,val);
  //   }
  // }

  systUpDown->Write();
}

void writeBtagSyst(TH1F* central,string name,TString kine,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* systUpDown = (TH1F*) central->Clone(Form("btag%s",updown.Data()));
  systUpDown->SetTitle(Form("btag%s",updown.Data()));
  float systValue = 1.;

  //fixme add signals

  if (TString(name)=="wz" || TString(name)=="ww" || TString(name)=="vg" || TString(name)=="rares" ) {
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      if (kine.Contains("hihi")) {
	if (bin>=1 && bin<=8)        systValue = 0.98;//0 btag
	else if (bin>=9  && bin<=16) systValue = 1.13;//1 btag
	else if (bin>=17 && bin<=24) systValue = 1.18;//2 btag
	else if (bin>=17 && bin<=24) systValue = 2.00;//3+btag
	else                         systValue = 1.00;//inclusive regions
      }
      if (kine.Contains("hilow")) {
	if (bin>=1 && bin<=6)        systValue = 0.98;//0 btag
	else if (bin>=7  && bin<=12) systValue = 1.13;//1 btag
	else if (bin>=13 && bin<=18) systValue = 1.18;//2 btag
	else if (bin>=19 && bin<=22) systValue = 2.00;//3+btag
	else                         systValue = 1.00;//inclusive regions
      }
      if (kine.Contains("lowlow")) {
	if (bin>=1 && bin<=2)        systValue = 0.98;//0 btag
	else if (bin>=3 && bin<=4)   systValue = 1.13;//1 btag
	else if (bin>=5 && bin<=6)   systValue = 1.18;//2 btag
	else if (bin==7)             systValue = 2.00;//3+btag
	else                         systValue = 1.00;//inclusive regions
      }
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="ttw" || TString(name)=="ttzh" || TString(name)=="tg" 
      || TString(name)=="t1tttt_1200" || TString(name)=="t1tttt_1500" 
      || TString(name)=="t6ttww_650" || TString(name)=="t6ttww_600" 
      || TString(name)=="t5tttt_deg") {
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      if (kine.Contains("hihi")) {
	if (bin>=1 && bin<=8)        systValue = 1.02;//0 btag
	else if (bin>=9  && bin<=16) systValue = 0.95;//1 btag
	else if (bin>=17 && bin<=24) systValue = 1.05;//2 btag
	else if (bin>=17 && bin<=24) systValue = 1.15;//3+btag
	else                         systValue = 1.00;//inclusive regions
      }
      if (kine.Contains("hilow")) {
	if (bin>=1 && bin<=6)        systValue = 1.02;//0 btag
	else if (bin>=7  && bin<=12) systValue = 0.95;//1 btag
	else if (bin>=13 && bin<=18) systValue = 1.05;//2 btag
	else if (bin>=19 && bin<=22) systValue = 1.15;//3+btag
	else                         systValue = 1.00;//inclusive regions
      }
      if (kine.Contains("lowlow")) {
	if (bin>=1 && bin<=2)        systValue = 1.02;//0 btag
	else if (bin>=3 && bin<=4)   systValue = 0.95;//1 btag
	else if (bin>=5 && bin<=6)   systValue = 1.05;//2 btag
	else if (bin==7)             systValue = 1.15;//3+btag
	else                         systValue = 1.00;//inclusive regions
      }
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }
  systUpDown->Write();
}
