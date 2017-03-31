#include <bitset>
#include <iostream>
#include <algorithm>
#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"
#include "TSystem.h"
#include "../../../../classFiles/v8.02/SS.h"
#include "../../../../CORE/SSSelections.h"
#include "../../../../commonUtils.h"
#include "../../../../CORE/Tools/dorky/dorky.h"
#include "../../../../CORE/Tools/utils.h"
#include "../../test.h"

float lumiAG = getLumiUnblind();
string tag = getTag().Data();  
string tagData = getTagData().Data();  

int nsr = 18;
int nHHsr = 51;
int nHLsr = 41;
int nLLsr = 8;

bool doCustomSelection = true; // use custom selection // FIXME
bool applyttZSF = true; // true if we want to apply the data driven scale factor for ttZ in addition to WZ
bool useBtagSF = true; // default true of cours
bool doTTTTLimits = true; // turn on when doing TTTT limits  // FIXME
bool doMoriondPUw = true; // test Moriond MC PUw 
bool doPUrw = true; // do PU reweighting of MC
bool useMCtriggers = true; // force usage of triggers in MC and disable trigger SFs
bool doMoriondMC = true;
bool requireRA2Filter = false; // require the muonFraction RA2 filter
bool makeTree = false; // make a root tree with SR events
bool DOWEIGHTS = false; // FIXME
float scaleLumi = 1.;//3.0/1.264;//careful!!!

bool doLatex = 0;


bool suppressWarns = true;


// For output tree
float tree_met = -1, tree_ht = -1, tree_mtmin = -1, tree_weight = -1;
int tree_l1id = -1, tree_l2id = -1, tree_njets = -1, tree_nbtags = -1, tree_kine = -1, tree_SR = -1;
int tree_event = -1, tree_lumi = -1, tree_run = -1;
TString tree_name = "";
// TFile *out_file;
// TTree *out_tree;

struct yields_t { float EE; float EM; float MM; float TOTAL; }; 
struct SR_t     { TH1F* EE; TH1F* EM; TH1F* MM; TH1F* TOTAL; }; 
struct plots_t  { TH1F* h_ht; TH1F* h_met; TH1F* h_mllem; TH1F* h_mlle;TH1F* h_mllmu;  TH1F* h_mll; TH1F* h_mtmin; TH1F* h_mt1; TH1F* h_mt2; TH1F* h_mt2real; TH1F* h_njets; TH1F* h_nbtags; TH1F* h_l1pt; TH1F* h_l2pt; TH1F* h_l1eta; TH1F* h_l2eta; TH1F* h_type; TH1F* h_charge; TH1F* h_lep1_miniIso; TH1F* h_lep2_miniIso; TH1F* h_lep1_ptRatio; TH1F* h_lep2_ptRatio; TH1F* h_lep1_ptRel; TH1F* h_lep2_ptRel; SR_t SRHH_old; SR_t SRHL_old; SR_t SRLL_old; SR_t SR; SR_t SRHH; SR_t SRHL; SR_t SRLL; SR_t SRAGG; TH1F* BR; TH1F* h_dxy; TH1F* h_dz; TH1F* h_sip3d_lep1; TH1F* h_sip3d_lep2; TH1F* h_mva; TH1F* h_nleps; TH1F* h_wcands; TH1F* h_topcands; TH1F *h_mu_dxy; TH1F *h_mu_dz; TH1F *h_mu_sip3d_lep1; TH1F *h_mu_mt1; TH1F *h_mu_l1pt; TH1F *h_mu_l1eta; TH1F *h_mu_lep1_miniIso; TH1F *h_mu_lep1_ptRel; TH1F *h_mu_lep1_ptRatio; TH1F *h_el_dxy; TH1F *h_el_dz; TH1F *h_el_sip3d_lep1; TH1F *h_el_mt1; TH1F *h_el_l1pt; TH1F *h_el_l1eta; TH1F *h_el_lep1_miniIso; TH1F *h_el_lep1_ptRel; TH1F *h_el_lep1_ptRatio; TH1F *h_mu_sip3d_lep2; TH1F *h_mu_lep2_miniIso; TH1F *h_mu_lep2_ptRel; TH1F *h_mu_lep2_ptRatio; TH1F *h_el_sip3d_lep2; TH1F *h_el_lep2_miniIso; TH1F *h_el_lep2_ptRel; TH1F *h_el_lep2_ptRatio; TH1F *h_mu_l2pt; TH1F *h_mu_l2eta; TH1F *h_el_l2pt; TH1F *h_el_l2eta; TH1F *h_fakeapp_HH; TH1F *h_fakeapp_HL; TH1F *h_fakeapp_LL; TH1F *h_mbb; };

//Total
yields_t total; 

//function declaration
pair<yields_t, plots_t> run(TChain *chain, bool isData = 0, bool doFlips = 0, int doFakes = 0, int exclude = 0, bool isSignal = 0, bool isGamma = 0);
int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt);
void initHistError(bool usePoisson, TH1F* plot);

static float roughSystTTW   = 0.2;
static float roughSystTTZH  = 0.2;
static float roughSystWZ    = 0.3;
static float roughSystWW    = 0.3;
static float roughSystXG    = 0.5;
static float roughSystRARES = 0.5;
static float roughSystFLIPS = 0.3;
static float roughSystFAKES = 0.35;
static float roughSystFAKESHH = 0.40;
static float roughSystFAKESXL = 0.30;

void getyields(){

    // out_file = new TFile("output.root", "RECREATE");
    // out_file->cd();
    // out_tree = new TTree("t", "From yieldMaker");
    // out_tree->Branch("event" , &tree_event );
    // out_tree->Branch("lumi" , &tree_lumi );
    // out_tree->Branch("run" , &tree_run );
    // out_tree->Branch("weight" , &tree_weight );
    // out_tree->Branch("met" , &tree_met );
    // out_tree->Branch("ht" , &tree_ht );
    // out_tree->Branch("mtmin" , &tree_mtmin );
    // out_tree->Branch("l1id" , &tree_l1id );
    // out_tree->Branch("l2id" , &tree_l2id );
    // out_tree->Branch("njets" , &tree_njets );
    // out_tree->Branch("nbtags" , &tree_nbtags );
    // out_tree->Branch("kine" , &tree_kine );
    // out_tree->Branch("SR" , &tree_SR );
    // out_tree->Branch("name" , &tree_name );

    cout << "Running with lumi=" << lumiAG*scaleLumi << endl;

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
    TChain *xg_chain      = new TChain("t","xg");
    TChain *rares_chain   = new TChain("t","rares");
    TChain *tttt_chain    = new TChain("t","tttt");
    TChain *qqww_chain    = new TChain("t","qqww"); // separate this out to check shape uncertainties
    //data
    TChain *data_chain    = new TChain("t","data"); 
    TChain *flips_chain   = new TChain("t","flips"); 
    TChain *fakes_chain   = new TChain("t","fakes");
    //signals full sim

    TString pfx  = Form("/nfs-7/userdata/ss2015/ssBabies/%s/", tag.c_str());

    TString pfx_moriond  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/";
    TString pfxData  = Form("/nfs-7/userdata/ss2015/ssBabies/%s/", tagData.c_str());

    if (doMoriondMC) {
        pfx = pfx_moriond;
        pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/";
    }

    // TString pfx  = "./skim_Dec1/";
    // TString pfxData  = pfx;

    // // //Fill chains
    tttt_chain   ->Add(Form("%s/TTTT.root"           , pfx.Data()));

    ttbar_chain  ->Add(Form("%s/TTBAR_PH*.root"       , pfx.Data())); 
    wjets_chain  ->Add(Form("%s/WJets.root"       , pfx.Data()));
    dy_chain     ->Add(Form("%s/DY_high*.root"        , pfx.Data()));
    dy_chain     ->Add(Form("%s/DY_low*.root"         , pfx.Data()));
    ttw_chain    ->Add(Form("%s/TTWnlo.root"            , pfx.Data())); 
    ttzh_chain   ->Add(Form("%s/TTZnlo.root"           , pfx.Data())); 
    ttzh_chain   ->Add(Form("%s/TTZLOW.root"         , pfx.Data())); 
    ttzh_chain   ->Add(Form("%s/TTHtoNonBB.root"     , pfx.Data()));
    wz_chain     ->Add(Form("%s/WZ.root"             , pfx.Data()));
    ww_chain     ->Add(Form("%s/QQWW.root"           , pfx.Data()));
    qqww_chain     ->Add(Form("%s/QQWW.root"           , pfx.Data()));
    xg_chain     ->Add(Form("%s/TG.root"             , pfx.Data()));
    xg_chain     ->Add(Form("%s/TTG.root"            , pfx.Data()));
    xg_chain     ->Add(Form("%s/WGToLNuG.root"           , pfx.Data()));
    xg_chain     ->Add(Form("%s/ZG.root"             , pfx.Data()));
    rares_chain  ->Add(Form("%s/ZZ.root"             , pfx.Data()));
    rares_chain  ->Add(Form("%s/GGHtoZZto4L.root"    , pfx.Data()));
    rares_chain  ->Add(Form("%s/WWZ.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/WZZ.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/WWW.root"            , pfx.Data()));
    if (doMoriondMC) {
        rares_chain  ->Add(Form("%s/WWG.root"            , pfx.Data()));
        rares_chain  ->Add(Form("%s/WZG.root"            , pfx.Data()));
    }
    rares_chain  ->Add(Form("%s/VHtoNonBB.root"      , pfx.Data()));
    rares_chain  ->Add(Form("%s/TZQ.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/TWZ.root"            , pfx.Data()));
    if (!doTTTTLimits) {
        rares_chain  ->Add(Form("%s/TTTT.root"           , pfx.Data()));
    }
    rares_chain  ->Add(Form("%s/WWDPS.root"          , pfx.Data()));
    //data
    data_chain   ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
    data_chain   ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
    data_chain   ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
    data_chain   ->Add(Form("%s/JetHT*.root"      , pfxData.Data()));
    //flips
    flips_chain  ->Add(Form("%s/DataMuonEG*.root"     , pfxData.Data()));
    flips_chain  ->Add(Form("%s/DataDoubleEG*.root"      , pfxData.Data()));
    //fakes
    fakes_chain  ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/JetHT*.root"      , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/TTWnlo.root"                   , pfx.Data()));
    fakes_chain  ->Add(Form("%s/TTZnlo.root"                  , pfx.Data()));
    fakes_chain  ->Add(Form("%s/WZ.root"                    , pfx.Data()));
    fakes_chain  ->Add(Form("%s/TTHtoNonBB.root"            , pfx.Data()));
    fakes_chain  ->Add(Form("%s/QQWW.root"                  , pfx.Data()));



    pair<yields_t, plots_t> results_ttw      = run(ttw_chain);
    pair<yields_t, plots_t> results_ttzh     = run(ttzh_chain);
    pair<yields_t, plots_t> results_qqww     = run(qqww_chain);
    pair<yields_t, plots_t> results_wz       = run(wz_chain);
    pair<yields_t, plots_t> results_ww       = run(ww_chain);
    pair<yields_t, plots_t> results_xg       = run(xg_chain, 0, 0, 0, 0, 0, 1);
    pair<yields_t, plots_t> results_rares    = run(rares_chain);
    pair<yields_t, plots_t> results_tttt     = run(tttt_chain);
    pair<yields_t, plots_t> results_data     = run(data_chain, 1);
    duplicate_removal::clear_list();
    pair<yields_t, plots_t> results_flips    = run(flips_chain, 1, 1);
    duplicate_removal::clear_list();
    pair<yields_t, plots_t> results_fakes    = run(fakes_chain, 1, 0, 1);
    duplicate_removal::clear_list();

    plots_t& p_tttt       = results_tttt.second;

    plots_t& p_ttw      = results_ttw.second;
    plots_t& p_ttzh     = results_ttzh.second;
    plots_t& p_wz       = results_wz.second;
    plots_t& p_ww       = results_ww.second;
    plots_t& p_xg       = results_xg.second;
    plots_t& p_rares    = results_rares.second;

    plots_t& p_data     = results_data.second;
    plots_t& p_flips    = results_flips.second;
    plots_t& p_fakes    = results_fakes.second;


    TFile *fout = new TFile("histos.root", "RECREATE");
    p_data.h_ht->Write();
    p_ttw.h_ht->Write();
    p_ttzh.h_ht->Write();
    p_wz.h_ht->Write();
    p_ww.h_ht->Write();
    p_xg.h_ht->Write();
    p_rares.h_ht->Write();
    p_flips.h_ht->Write();
    p_fakes.h_ht->Write();
    p_tttt.h_ht->Write();

    p_data.h_met->Write();
    p_ttw.h_met->Write();
    p_ttzh.h_met->Write();
    p_wz.h_met->Write();
    p_ww.h_met->Write();
    p_xg.h_met->Write();
    p_rares.h_met->Write();
    p_flips.h_met->Write();
    p_fakes.h_met->Write();
    p_tttt.h_met->Write();

    p_data.h_njets->Write();
    p_ttw.h_njets->Write();
    p_ttzh.h_njets->Write();
    p_wz.h_njets->Write();
    p_ww.h_njets->Write();
    p_xg.h_njets->Write();
    p_rares.h_njets->Write();
    p_flips.h_njets->Write();
    p_fakes.h_njets->Write();
    p_tttt.h_njets->Write();

    p_data.h_nbtags->Write();
    p_ttw.h_nbtags->Write();
    p_ttzh.h_nbtags->Write();
    p_wz.h_nbtags->Write();
    p_ww.h_nbtags->Write();
    p_xg.h_nbtags->Write();
    p_rares.h_nbtags->Write();
    p_flips.h_nbtags->Write();
    p_fakes.h_nbtags->Write();
    p_tttt.h_nbtags->Write();

    p_data.h_mtmin->Write();
    p_ttw.h_mtmin->Write();
    p_ttzh.h_mtmin->Write();
    p_wz.h_mtmin->Write();
    p_ww.h_mtmin->Write();
    p_xg.h_mtmin->Write();
    p_rares.h_mtmin->Write();
    p_flips.h_mtmin->Write();
    p_fakes.h_mtmin->Write();
    p_tttt.h_mtmin->Write();

    p_data.h_charge->Write();
    p_ttw.h_charge->Write();
    p_ttzh.h_charge->Write();
    p_wz.h_charge->Write();
    p_ww.h_charge->Write();
    p_xg.h_charge->Write();
    p_rares.h_charge->Write();
    p_flips.h_charge->Write();
    p_fakes.h_charge->Write();
    p_tttt.h_charge->Write();

    p_data.h_nleps->Write();
    p_ttw.h_nleps->Write();
    p_ttzh.h_nleps->Write();
    p_wz.h_nleps->Write();
    p_ww.h_nleps->Write();
    p_xg.h_nleps->Write();
    p_rares.h_nleps->Write();
    p_flips.h_nleps->Write();
    p_fakes.h_nleps->Write();
    p_tttt.h_nleps->Write();

    p_data.h_topcands->Write();
    p_ttw.h_topcands->Write();
    p_ttzh.h_topcands->Write();
    p_wz.h_topcands->Write();
    p_ww.h_topcands->Write();
    p_xg.h_topcands->Write();
    p_rares.h_topcands->Write();
    p_flips.h_topcands->Write();
    p_fakes.h_topcands->Write();
    p_tttt.h_topcands->Write();

    p_data.h_wcands->Write();
    p_ttw.h_wcands->Write();
    p_ttzh.h_wcands->Write();
    p_wz.h_wcands->Write();
    p_ww.h_wcands->Write();
    p_xg.h_wcands->Write();
    p_rares.h_wcands->Write();
    p_flips.h_wcands->Write();
    p_fakes.h_wcands->Write();
    p_tttt.h_wcands->Write();

    p_data.h_mlle->Write();
    p_ttw.h_mlle->Write();
    p_ttzh.h_mlle->Write();
    p_wz.h_mlle->Write();
    p_ww.h_mlle->Write();
    p_xg.h_mlle->Write();
    p_rares.h_mlle->Write();
    p_flips.h_mlle->Write();
    p_fakes.h_mlle->Write();
    p_tttt.h_mlle->Write();

    p_data.h_mllmu->Write();
    p_ttw.h_mllmu->Write();
    p_ttzh.h_mllmu->Write();
    p_wz.h_mllmu->Write();
    p_ww.h_mllmu->Write();
    p_xg.h_mllmu->Write();
    p_rares.h_mllmu->Write();
    p_flips.h_mllmu->Write();
    p_fakes.h_mllmu->Write();
    p_tttt.h_mllmu->Write();

    p_data.h_mllem->Write();
    p_ttw.h_mllem->Write();
    p_ttzh.h_mllem->Write();
    p_wz.h_mllem->Write();
    p_ww.h_mllem->Write();
    p_xg.h_mllem->Write();
    p_rares.h_mllem->Write();
    p_flips.h_mllem->Write();
    p_fakes.h_mllem->Write();
    p_tttt.h_mllem->Write();


    p_data.h_mll->Write();
    p_ttw.h_mll->Write();
    p_ttzh.h_mll->Write();
    p_wz.h_mll->Write();
    p_ww.h_mll->Write();
    p_xg.h_mll->Write();
    p_rares.h_mll->Write();
    p_flips.h_mll->Write();
    p_fakes.h_mll->Write();
    p_tttt.h_mll->Write();


    p_data.h_type->Write();
    p_ttw.h_type->Write();
    p_ttzh.h_type->Write();
    p_wz.h_type->Write();
    p_ww.h_type->Write();
    p_xg.h_type->Write();
    p_rares.h_type->Write();
    p_flips.h_type->Write();
    p_fakes.h_type->Write();
    p_tttt.h_type->Write();

    p_data.h_mt2real->Write();
    p_ttw.h_mt2real->Write();
    p_ttzh.h_mt2real->Write();
    p_wz.h_mt2real->Write();
    p_ww.h_mt2real->Write();
    p_xg.h_mt2real->Write();
    p_rares.h_mt2real->Write();
    p_flips.h_mt2real->Write();
    p_fakes.h_mt2real->Write();
    p_tttt.h_mt2real->Write();

    p_data.SR.TOTAL->Write();
    p_ttw.SR.TOTAL->Write();
    p_ttzh.SR.TOTAL->Write();
    p_wz.SR.TOTAL->Write();
    p_ww.SR.TOTAL->Write();
    p_xg.SR.TOTAL->Write();
    p_rares.SR.TOTAL->Write();
    p_flips.SR.TOTAL->Write();
    p_fakes.SR.TOTAL->Write();
    p_tttt.SR.TOTAL->Write();

    p_data.SRHH.TOTAL->Write();
    p_ttw.SRHH.TOTAL->Write();
    p_ttzh.SRHH.TOTAL->Write();
    p_wz.SRHH.TOTAL->Write();
    p_ww.SRHH.TOTAL->Write();
    p_xg.SRHH.TOTAL->Write();
    p_rares.SRHH.TOTAL->Write();
    p_flips.SRHH.TOTAL->Write();
    p_fakes.SRHH.TOTAL->Write();
    p_tttt.SRHH.TOTAL->Write();

    p_data.SRLL.TOTAL->Write();
    p_ttw.SRLL.TOTAL->Write();
    p_ttzh.SRLL.TOTAL->Write();
    p_wz.SRLL.TOTAL->Write();
    p_ww.SRLL.TOTAL->Write();
    p_xg.SRLL.TOTAL->Write();
    p_rares.SRLL.TOTAL->Write();
    p_flips.SRLL.TOTAL->Write();
    p_fakes.SRLL.TOTAL->Write();
    p_tttt.SRLL.TOTAL->Write();

    p_data.SRHL.TOTAL->Write();
    p_ttw.SRHL.TOTAL->Write();
    p_ttzh.SRHL.TOTAL->Write();
    p_wz.SRHL.TOTAL->Write();
    p_ww.SRHL.TOTAL->Write();
    p_xg.SRHL.TOTAL->Write();
    p_rares.SRHL.TOTAL->Write();
    p_flips.SRHL.TOTAL->Write();
    p_fakes.SRHL.TOTAL->Write();
    p_tttt.SRHL.TOTAL->Write();

    fout->Write();
    fout->Close();

    // // Write output tree
    // out_file->cd();
    // out_tree->Write();
    // out_file->Close();

}

//doFakes = 1 for QCD, 2 for inSitu
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes, int exclude, bool isSignal, bool isGamma){

    // out_file->cd();


    TString chainTitle = chain->GetTitle();
    const char* chainTitleCh = chainTitle.Data();
    std::cout << "Working on " << chainTitle << std::endl;

    bool isWZ = (chainTitle=="wz");
    bool isttZ = (chainTitle=="ttzh");
    bool isOS = (chainTitle=="ttbar_os");

    yields_t y_result;
    plots_t  p_result;

    //Initialize
    y_result.EE    = 0;
    y_result.EM    = 0;
    y_result.MM    = 0;
    y_result.TOTAL = 0;

    p_result.h_ht           = new TH1F(Form("ht_%s"          , chainTitleCh) , Form("ht_%s"          , chainTitleCh), 15, 80  ,1800);  // extended binning for tail SRs
    p_result.h_met          = new TH1F(Form("met_%s"         , chainTitleCh) , Form("met_%s"         , chainTitleCh), 15, 0   , 600);  // extended binning for tail SRs
    p_result.h_mll          = new TH1F(Form("mll_%s"         , chainTitleCh) , Form("mll_%s"         , chainTitleCh), 20, 0   , 200); 
    p_result.h_mlle          = new TH1F(Form("mlle_%s"         , chainTitleCh) , Form("mlle_%s"         , chainTitleCh), 20, 0   , 200); 
    p_result.h_mllem          = new TH1F(Form("mllem_%s"         , chainTitleCh) , Form("mllem_%s"         , chainTitleCh), 20, 0   , 200); 
    p_result.h_mllmu          = new TH1F(Form("mllmu_%s"         , chainTitleCh) , Form("mllmu_%s"         , chainTitleCh), 20, 0   , 200); 
    p_result.h_mtmin        = new TH1F(Form("mtmin_%s"       , chainTitleCh) , Form("mtmin_%s"       , chainTitleCh), 10, 0   , 200); 
    p_result.h_mt2real        = new TH1F(Form("mt2real_%s"       , chainTitleCh) , Form("mt2real_%s"       , chainTitleCh), 20, 0   , 200); 
    p_result.h_njets        = new TH1F(Form("njets_%s"       , chainTitleCh) , Form("njets_%s"       , chainTitleCh), 10 , 1.5 , 11.5); 
    p_result.h_nleps        = new TH1F(Form("nleps_%s"       , chainTitleCh) , Form("nleps_%s"       , chainTitleCh), 6 , 0   , 6);   
    p_result.h_wcands        = new TH1F(Form("wcands_%s"       , chainTitleCh) , Form("wcands_%s"       , chainTitleCh), 10 , 0   , 10);   
    p_result.h_topcands        = new TH1F(Form("topcands_%s"       , chainTitleCh) , Form("topcands_%s"       , chainTitleCh), 4 , 0   , 4);   
    p_result.h_nbtags       = new TH1F(Form("nbtags_%s"      , chainTitleCh) , Form("nbtags_%s"      , chainTitleCh), 7 , -0.5, 6.5); 
    p_result.SR.TOTAL     = new TH1F(Form("SR_TOTAL_%s"  , chainTitleCh) , Form("SR_TOTAL_%s"  , chainTitleCh), nsr, 0.5 , nsr+0.5);
    p_result.SRHH.TOTAL     = new TH1F(Form("SRHH_TOTAL_%s"  , chainTitleCh) , Form("SRHH_TOTAL_%s"  , chainTitleCh), nHHsr, 0.5 , nHHsr+0.5);
    p_result.SRHL.TOTAL     = new TH1F(Form("SRHL_TOTAL_%s"  , chainTitleCh) , Form("SRHL_TOTAL_%s"  , chainTitleCh), nHLsr, 0.5 , nHLsr+0.5);
    p_result.SRLL.TOTAL     = new TH1F(Form("SRLL_TOTAL_%s"  , chainTitleCh) , Form("SRLL_TOTAL_%s"  , chainTitleCh), nLLsr , 0.5 ,  nLLsr+0.5);
    p_result.h_type         = new TH1F(Form("type_%s"        , chainTitleCh) , Form("type_%s"        , chainTitleCh), 4 , 0   , 4);   
    p_result.h_charge       = new TH1F(Form("charge_%s"      , chainTitleCh) , Form("charge_%s"      , chainTitleCh), 3 , -1  , 2);   


    bool doPoisson = isData && !doFlips && !doFakes;
    initHistError(doPoisson, p_result.h_ht         );
    initHistError(doPoisson, p_result.h_met        );
    initHistError(doPoisson, p_result.h_mll        );
    initHistError(doPoisson, p_result.h_mlle        );
    initHistError(doPoisson, p_result.h_mllem        );
    initHistError(doPoisson, p_result.h_mllmu        );
    initHistError(doPoisson, p_result.h_mtmin      );
    initHistError(doPoisson, p_result.h_mt2real    );
    initHistError(doPoisson, p_result.h_njets      );
    initHistError(doPoisson, p_result.h_nleps      );
    initHistError(doPoisson, p_result.h_nbtags     );
    initHistError(doPoisson, p_result.SRHH.TOTAL   );
    initHistError(doPoisson, p_result.SRHL.TOTAL   );
    initHistError(doPoisson, p_result.SRLL.TOTAL   );
    initHistError(doPoisson, p_result.h_type       );
    initHistError(doPoisson, p_result.h_charge     );

    //nEvents in chain
    unsigned int nEventsTotal = 0;
    unsigned int nEventsChain = chain->GetEntries();

    //Set up iterator
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    TFile *currentFile = 0;

    //Number of selected events
    //int nSelected = 0; 

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

            if (ss::is_real_data()){
                if (!doFlips && !doFakes) continue; 
                else if (!isUnblindRun(ss::run())) continue;
            }

            //Reject not triggered
            if (useMCtriggers) {
                if (!ss::fired_trigger()) continue;
            } else {
                if (!ss::fired_trigger() && ss::is_real_data()) continue;
            }
            if (!ss::passedFilterList()) continue;
            if (!ss::passes_met_filters()) continue;
            if (requireRA2Filter && ss::failsRA2Filter()) continue;

            if (doCustomSelection) {

                // if (ss::nbtags() < 3) continue;
                // if (ss::njets() < 5) continue;

            }

            float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;
            weight*=scaleLumi;

            if (DOWEIGHTS) {
                if (doPUrw && !ss::is_real_data()) {
                    if (doMoriondPUw) weight *= getTruePUw_Moriond(ss::trueNumInt()[0]);
                    else weight *= getTruePUw(ss::trueNumInt()[0]);
                }
                if (useBtagSF) weight*=ss::weight_btagsf();
                if (isWZ) weight*=getWZSF();
                if (isttZ && applyttZSF) weight*=getttZSF();
                if(!ss::is_real_data()) {
                    if (!useMCtriggers) {
                    }
                }
                //apply lepton scale factors
                if (ss::is_real_data()==0 && (!isWZ) && (!isttZ || !applyttZSF)) {
                    weight*=eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
                }

                //electron FO is tighter for iso triggers, make sure it is passed
                if (ss::ht()<300.) {
                    if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA(), ss::lep1_p4().pt())) continue;
                    if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA(), ss::lep2_p4().pt())) continue;
                } 

            }



            float lep1_pt = ss::lep1_coneCorrPt();
            float lep2_pt = ss::lep2_coneCorrPt();
            //now recompute mtmin
            float mtl1 = MT(lep1_pt, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
            float mtl2 = MT(lep2_pt, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
            float mtmin = mtl1 > mtl2 ? mtl2 : mtl1;

            //drop electrons below 15 GeV
            if (abs(ss::lep1_id())==11 && lep1_pt<15) continue;
            if (abs(ss::lep2_id())==11 && lep2_pt<15) continue;	

            //Progress
            SSAG::progress(nEventsTotal, nEventsChain);

            //Only keep good events
            int ssclass = 3;
            if (isOS) {
                ssclass = 4;
                weight *= 0.001;
            }
            if (!doFlips && !doFakes && exclude == 0) {
                if (ss::hyp_class() != ssclass) continue;
                //first check the charge (since isPrompt does not know about it)
                if (!isData && !isGamma && ss::lep1_motherID()==2) continue;
                if (!isData && !isGamma && ss::lep2_motherID()==2) continue;
                //then allow events with two prompt leptons or with prompt photons
                //photons can give fakes from conversions (not accounted by data-driven method) 
                if (!isData && !( (ss::lep1_motherID()==1 && ss::lep2_motherID()==1) || (ss::lep1_motherID()==-3 || ss::lep2_motherID()==-3)) ) continue;
                //if (!isData && !isGamma && (ss::lep1_motherID()!=1 || ss::lep2_motherID()!=1)) continue;
                //if (isGamma && (ss::lep1_motherID()!=-3 && ss::lep2_motherID()!=-3)) continue;
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

            //QCD Fakes
            if (doFakes == 1){
                if (ss::hyp_class() != 2 && ss::hyp_class() != 1) continue;
                if (ss::lep1_passes_id()==0) {
                    float fr = fakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
                    weight *= fr/(1.-fr);
                }
                if (ss::lep2_passes_id()==0) {
                    float fr = fakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
                    weight *= fr/(1.-fr);
                }
                //subtract double FO
                if (ss::hyp_class() == 1) weight *= -1.;
                if (!ss::is_real_data() && isData) {
                    weight *= -1.;
                }


            }

            //Require baseline selections
            int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, true);
            if (BR < 0) continue;

            // int SR = signalRegionChargeSplit(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);
            int SR = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);
            if (SR < 0) continue;

            //Reject duplicates (after selection otherwise flips are ignored...)
            if (isData && ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)){ continue; }
            }



            int nleps = 2;
            if (ss::lep3_passes_id()) nleps++;
            if (ss::lep4_passes_id()) nleps++;

            // NJA

            std::vector<LorentzVector> bjets;
            std::vector<LorentzVector> jets;
            for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                bjets.push_back(ss::btags()[ijet]);
            }
            for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                // skip b-tagged jets
                if (ss::jets_disc_ivf().at(ijet) > 0.8484) continue;
                jets.push_back(ss::jets()[ijet]);
            }

            int wcands = 0;
            int topcands = 0;
            // int ntotpairs = (jet.size()*(jets.size()-1))/2;
            // float fracw = 0.;
            for (unsigned int i = 0; i < jets.size(); i++) {
                for (unsigned int j = i+1; j < jets.size(); j++) {
                    float wmass = (jets[i]+jets[j]).M();
                    if (fabs(wmass-80.4) < 30.0) {
                        wcands++;
                        for (unsigned int k = 0; k < jets.size(); k++) {
                            if (k == i) continue;
                            if (k == j) continue;

                            float topmass = (jets[i]+jets[j]+jets[k]).M();
                            if (fabs(topmass-173) < 45.0) {
                                topcands++;
                            }
                        }

                    }
                    // std::cout << " i: " << i << " j: " << j << " mass: " << mass << " jets[i].pt(): " << jets[i].pt() << " jets[j].pt(): " << jets[j].pt() << std::endl;
                }
            }
            topcands = min(topcands,2);

            // std::cout << " wcands: " << wcands << std::endl;
            p_result.h_wcands->Fill(wcands, weight);
            p_result.h_topcands->Fill(topcands, weight);

            // if (ntotpairs > 0) {
            //     fracw = 1.0*wcands/ntotpairs;
            // }

            //Get the category
            anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt());

            // if (makeTree) {
            //     // Fill tree
            //     tree_event = ss::event();
            //     tree_run = ss::run();
            //     tree_lumi = ss::lumi();
            //     tree_met = ss::met();
            //     tree_ht = ss::ht();
            //     tree_mtmin = mtmin;
            //     tree_l1id = ss::lep1_id();
            //     tree_l2id = ss::lep2_id();
            //     tree_njets = ss::njets();
            //     tree_nbtags = ss::nbtags();
            //     tree_kine = categ;
            //     tree_SR = SR;
            //     tree_name = chainTitle;
            //     tree_weight = weight;
            //     // out_tree->Fill();
            // }

            int mytype = ss::hyp_type();
            if (mytype==2 && abs(ss::lep1_id())==13) mytype = 1;

            p_result.h_njets       ->Fill(ss::njets()                                                                             , weight);
            p_result.h_nbtags      ->Fill(ss::nbtags()                                                                            , weight);
            p_result.h_type        ->Fill(mytype                                                                                  , weight);
            p_result.h_charge      ->Fill(ss::lep1_id() > 0 ? -0.5: 0.5                                                               , weight);
            p_result.h_nleps       ->Fill(nleps                                                                                   , weight);
            p_result.h_ht          ->Fill(ss::ht()                                                                                , weight);
            p_result.h_met         ->Fill(ss::met()                                                                               , weight);
            p_result.h_mll         ->Fill((ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M() , weight);
            if (mytype == 3) p_result.h_mlle         ->Fill((ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M() , weight);
            if (mytype == 1 || mytype == 2) p_result.h_mllem         ->Fill((ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M() , weight);
            if (mytype == 0) p_result.h_mllmu         ->Fill((ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M() , weight);
            p_result.h_mtmin       ->Fill(mtmin                                                                                   , weight);
            p_result.h_mt2real       ->Fill(ss::mt2()                                                                                   , weight);

            if (ss::is_real_data() && ss::hyp_class() == 3) {
                if ((ss::run()==280385 && ss::event()==  150744040 ) ||
                        (ss::run()==278962 && ss::event()==  192249174 ) ||
                        (ss::run()==274250 && ss::event()==  776141785 ) ||
                        (ss::run()==282800 && ss::event()==  238479114 ) ||
                        (ss::run()==276525 && ss::event()== 3433833261 ) ||
                        (ss::run()==274335 && ss::event()== 1743620665 ) ||
                        (ss::run()==276935 && ss::event()==  606057504 ) ||
                        (ss::run()==276501 && ss::event()== 2111733195 ) ||
                        (ss::run()==274422 && ss::event()== 1051239668 ) ||
                        (ss::run()==278406 && ss::event()==   68028278 ) ||
                        (ss::run()==277127 && ss::event()==  871511309 ) ||
                        (ss::run()==276808 && ss::event()==  785380556 ) ||
                        (ss::run()==283877 && ss::event()==  739904253 ) ||
                        (ss::run()==278167 && ss::event()== 2782266704 ) ||
                        (ss::run()==274968 && ss::event()==  702006072 ) ||
                        (ss::run()==276935 && ss::event()==  209782894 ) ||
                        (ss::run()==276525 && ss::event()==  561914544 ) ||
                        (ss::run()==283820 && ss::event()==  720129244 ) ||
                        (ss::run()==276794 && ss::event()==  193995368 ) ||
                        (ss::run()==275068 && ss::event()== 1664269198 ) ||
                        (ss::run()==276870 && ss::event()== 1954779814 ) ||
                        (ss::run()==284043 && ss::event()==   30498383 ) ||
                        (ss::run()==277096 && ss::event()== 1896362069 ) ||
                        (ss::run()==276242 && ss::event()==  486011205 )) {
                            std::cout << "Filtering/skipping this event " << ss::run() << ":" << ss::lumi() << ":" << ss::event() << std::endl;
                            continue;
                        }
            }


            //Fill SR plots
            p_result.SR.TOTAL->Fill(SR, weight); 
            // if (categ == HighHigh){
            //     p_result.SRHH.TOTAL->Fill(SR, weight); 
            // }

            // if (categ == HighLow){
            //     p_result.SRHL.TOTAL->Fill(SR, weight); 
            // }

            // if (categ == LowLow){
            //     p_result.SRLL.TOTAL->Fill(SR, weight); 
            // }


        }//event loop
        file->Close(); 
    }//file loop

    //Return yield
    return pair<yields_t, plots_t>(y_result,p_result); 

}

void initHistError(bool usePoisson, TH1F* plot) {
    if (plot==0) return;
    if (usePoisson) plot->SetBinErrorOption(TH1::kPoisson);
    else  plot->Sumw2();
}
