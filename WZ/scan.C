#pragma GCC diagnostic ignored "-Wsign-compare"
#include "../software/dataMCplotMaker/dataMCplotMaker.h"

//Regular includes
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1.h"
#include "TChain.h"

//Class files
#include "../classFiles/v6.02/SS.h"

#include "utils.C"
#define USE_OLD_LEPSF 0
#define USE_TEST_LEPSF 0
#include "../commonUtils.h"
#include "../CORE/SSSelections.h"
#include "../CORE/Tools/dorky/dorky.cc"
#include "../CORE/Tools/utils.h"

using namespace std;
using namespace duplicate_removal;


int scan(){

    // to make the 3 sets of plots for the AN, need to do
    //     true, false (and then also make the prefit/postfit plots, along with SFs)
    //     false, false (WZ enriched)
    //     true, true (ttZ enriched)
    bool relaxNbtags = true; // fully unconstrained in nbtags --> TRUE FOR SIMULTANEOUS FIT
    bool Nbtagsqeq2 = false; // require nb>=2 (ttZ enriched) --> false for simultaneous fit
    bool scaleWZttZ = true; // obviously want this to be false to derive the SFs in simultaneous fit

    bool useBtagSF = true;
    bool useLeptonSFs = true;
    bool useMoriondSamples = true;

    // default false as these are debug variables
    bool doEE = false;
    bool doMM = false;
    bool doE = false;
    bool doM = false;
    bool inclusive = false;
    bool relaxNjets = false;
    bool doRA7 = false;
    bool requireNb0 = false;
    bool WZandDataOnly = false;
    bool noData = false;
    bool doLowPU = false;
    bool doHighPU = false;

    HLTEfficiency HLTEff("../hlt/HLT_Efficiencies_7p65fb_2016.root");

    TChain *ch = new TChain("t");
    std::vector<std::string> titles;

    TString tag = getTag();
    TString tagData = getTagData();

    std::vector<TString> files;
    std::vector<float> systs;

    TString pfx  = "/nfs-7/userdata/ss2015/ssBabies/"+getTag();
    TString pfxData  = "/nfs-7/userdata/ss2015/ssBabies/"+getTagData();

    // TString pfx_moriond  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.03/output/";
    // std::cout << "USING JET30" << std::endl;
    // TString pfx_moriond  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04_jet30/output/";
    // TString pfx_moriond  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04_jet30_sel1/output/";

    if (useMoriondSamples) {
    TString pfx_moriond  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/";
        pfx = pfx_moriond;
    pfxData  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/";
    }

    bool doICHEP = getTag() == "v8.04";

    titles.push_back("Nonprompt Lep."); files.push_back("Fakes"); systs.push_back(0.5);


    titles.push_back("t#bar{t}Z"); files.push_back("ttZ"); systs.push_back(0.5);

    if (!WZandDataOnly) ch->Add(pfx+"/TTZnlo.root");


    titles.push_back("t#bar{t}W"); files.push_back("TTW"); systs.push_back(0.5);

    if (!WZandDataOnly) ch->Add(pfx+"/TTWnlo.root");

    titles.push_back("Rare SM"); files.push_back("Rares"); systs.push_back(0.5);

    if (!WZandDataOnly) ch->Add(pfx+"/ZZ.root");
    if (!WZandDataOnly) ch->Add(pfx+"/GGHtoZZto4L.root");
    if (!WZandDataOnly) ch->Add(pfx+"/WWZ.root");
    if (!WZandDataOnly) ch->Add(pfx+"/WZZ.root");
    if (!doICHEP) {
        if (!WZandDataOnly) ch->Add(pfx+"/TWZ.root");
    }
    if (!WZandDataOnly) ch->Add(pfx+"/WWW.root");
    if (!WZandDataOnly) ch->Add(pfx+"/WWDPS.root");
    if (!WZandDataOnly) ch->Add(pfx+"/VHtoNonBB.root");
    if (!WZandDataOnly) ch->Add(pfx+"/TTTT.root");
    if (!WZandDataOnly) ch->Add(pfx+"/TZQ.root");

    titles.push_back("WW"); files.push_back("WW"); systs.push_back(0.5);

    if (!WZandDataOnly) ch->Add(pfx+"/QQWW.root");

    titles.push_back("X+#gamma"); files.push_back("XG"); systs.push_back(0.5);

    if (!WZandDataOnly) ch->Add(pfx+"/WGToLNuG.root");
    if (!WZandDataOnly) ch->Add(pfx+"/ZG.root");
    if (!WZandDataOnly) ch->Add(pfx+"/TG.root");
    if (!WZandDataOnly) ch->Add(pfx+"/TTG.root");

    titles.push_back("WZ"); files.push_back("WZ"); systs.push_back(0.0);

    ch->Add(pfx+"/WZ.root");

    titles.push_back("t#bar{t}H"); files.push_back("ttH"); systs.push_back(0.5);

    if (!WZandDataOnly) ch->Add(pfx+"/TTHtoNonBB.root");

    files.push_back("Data"); systs.push_back(0.0);

    if (!noData) ch->Add(pfxData+"/DataDouble*.root");
    if (!noData) ch->Add(pfxData+"/DataMuonEG*.root");

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
    int nGoodEvents = 0;
    int nGoodEventsData = 0;
    float nGoodEventsWeighted = 0;
    float luminosity = getLumi();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    vector<pair<TH1F*,float> > h1D_njets_vec;
    vector<pair<TH1F*,float> > h1D_njetsnb2_vec;
    vector<pair<TH1F*,float> > h1D_ht_vec;
    vector<pair<TH1F*,float> > h1D_mll_vec;
    vector<pair<TH1F*,float> > h1D_met_vec;
    vector<pair<TH1F*,float> > h1D_mt_vec;
    vector<pair<TH1F*,float> > h1D_mtmin_vec;
    vector<pair<TH1F*,float> > h1D_mtw_vec;
    vector<pair<TH1F*,float> > h1D_lep1pt_vec;
    vector<pair<TH1F*,float> > h1D_lep2pt_vec;
    vector<pair<TH1F*,float> > h1D_lep3pt_vec;
    vector<pair<TH1F*,float> > h1D_olep1pt_vec;
    vector<pair<TH1F*,float> > h1D_olep2pt_vec;
    vector<pair<TH1F*,float> > h1D_olep3pt_vec;
    vector<pair<TH1F*,float> > h1D_lepwpt_vec;
    vector<pair<TH1F*,float> > h1D_nvtx_vec;
    vector<pair<TH1F*,float> > h1D_type_vec;

    vector<pair<TH1F*,float> > h1D_nbtags_vec;
    vector<pair<TH1F*,float> > h1D_nbtags_btag_up_vec;
    vector<pair<TH1F*,float> > h1D_nbtags_btag_dn_vec;
    vector<pair<TH1F*,float> > h1D_nbtags_jes_up_vec;
    vector<pair<TH1F*,float> > h1D_nbtags_jes_dn_vec;

    initCounter();

    for(int i = 0; i < files.size(); i++) {

        TH1F* h1D_njets_file = new TH1F("njets"+files.at(i), "Njets;;Entries", 10, 0, 10);
        TH1F* h1D_njetsnb2_file = new TH1F("njetsnb2"+files.at(i), "Njets;;Entries", 10, 0, 10);
        TH1F* h1D_ht_file = new TH1F("ht"+files.at(i), "H_{T};GeV;Entries", 20, 0, 1000); 
        TH1F* h1D_mll_file = new TH1F("mll"+files.at(i), "m_{ll};GeV;Entries", 20, 70, 110); 
        TH1F* h1D_met_file = new TH1F("met"+files.at(i), "#slash{E}_{T};GeV;Entries", 8, 0, 200); 
        TH1F* h1D_mt_file = new TH1F("mt"+files.at(i), "M_{T};GeV;Entries", 20, 0, 450); 
        TH1F* h1D_mtmin_file = new TH1F("mtmin"+files.at(i), "mtmin;mtmin;Entries", 20, 0, 200); 
        TH1F* h1D_mtw_file = new TH1F("mtw"+files.at(i), "mtw;mtw;Entries", 20, 0, 200); 
        TH1F* h1D_lep1pt_file = new TH1F("lep1pt"+files.at(i), "lep1pt;lep1pt;Entries", 40, 0, 400); 
        TH1F* h1D_lep2pt_file = new TH1F("lep2pt"+files.at(i), "lep2pt;lep2pt;Entries", 40, 0, 400); 
        TH1F* h1D_lep3pt_file = new TH1F("lep3pt"+files.at(i), "lep3pt;lep3pt;Entries", 40, 0, 400); 
        TH1F* h1D_olep1pt_file = new TH1F("olep1pt"+files.at(i), "olep1pt;olep1pt;Entries", 40, 0, 400); 
        TH1F* h1D_olep2pt_file = new TH1F("olep2pt"+files.at(i), "olep2pt;olep2pt;Entries", 40, 0, 400); 
        TH1F* h1D_olep3pt_file = new TH1F("olep3pt"+files.at(i), "olep3pt;olep3pt;Entries", 40, 0, 400); 
        TH1F* h1D_lepwpt_file = new TH1F("lepwpt"+files.at(i), "lepwpt;lepwpt;Entries", 40, 0, 400); 
        TH1F* h1D_nvtx_file = new TH1F("nvtx"+files.at(i), "nvtx;nvtx;Entries", 50,0,50); 
        TH1F* h1D_type_file = new TH1F("type"+files.at(i), "type;type;Entries", 4,0,4); 

        TH1F* h1D_nbtags_file = new TH1F("nbtags"+files.at(i), "Nbtags;nbtags;Entries", 4, 0, 4); 
        TH1F* h1D_nbtags_btag_up_file = new TH1F("nbtags_btag_up"+files.at(i), "Nbtags Btag SF Up;nbtags;Entries", 4, 0, 4); 
        TH1F* h1D_nbtags_btag_dn_file = new TH1F("nbtags_btag_dn"+files.at(i), "Nbtags Btag SF Down;nbtags;Entries", 4, 0, 4); 
        TH1F* h1D_nbtags_jes_up_file = new TH1F("nbtags_jes_up"+files.at(i), "Nbtags JES Up;nbtags;Entries", 4, 0, 4); 
        TH1F* h1D_nbtags_jes_dn_file = new TH1F("nbtags_jes_dn"+files.at(i), "Nbtags JES Down;nbtags;Entries", 4, 0, 4); 

        h1D_njets_vec.push_back(std::make_pair(h1D_njets_file,systs.at(i)));
        h1D_njetsnb2_vec.push_back(std::make_pair(h1D_njetsnb2_file,systs.at(i)));
        h1D_ht_vec.push_back(std::make_pair(h1D_ht_file,systs.at(i)));
        h1D_mll_vec.push_back(std::make_pair(h1D_mll_file,systs.at(i)));
        h1D_met_vec.push_back(std::make_pair(h1D_met_file,systs.at(i)));
        h1D_mt_vec.push_back(std::make_pair(h1D_mt_file,systs.at(i)));
        h1D_mtmin_vec.push_back(std::make_pair(h1D_mtmin_file,systs.at(i)));
        h1D_mtw_vec.push_back(std::make_pair(h1D_mtw_file,systs.at(i)));
        h1D_lep1pt_vec.push_back(std::make_pair(h1D_lep1pt_file,systs.at(i)));
        h1D_lep2pt_vec.push_back(std::make_pair(h1D_lep2pt_file,systs.at(i)));
        h1D_lep3pt_vec.push_back(std::make_pair(h1D_lep3pt_file,systs.at(i)));
        h1D_olep1pt_vec.push_back(std::make_pair(h1D_olep1pt_file,systs.at(i)));
        h1D_olep2pt_vec.push_back(std::make_pair(h1D_olep2pt_file,systs.at(i)));
        h1D_olep3pt_vec.push_back(std::make_pair(h1D_olep3pt_file,systs.at(i)));
        h1D_lepwpt_vec.push_back(std::make_pair(h1D_lepwpt_file,systs.at(i)));
        h1D_nvtx_vec.push_back(std::make_pair(h1D_nvtx_file,systs.at(i)));
        h1D_type_vec.push_back(std::make_pair(h1D_type_file,systs.at(i)));

        h1D_nbtags_vec.push_back(std::make_pair(h1D_nbtags_file,systs.at(i)));
        h1D_nbtags_btag_up_vec.push_back(std::make_pair(h1D_nbtags_btag_up_file,systs.at(i)));
        h1D_nbtags_btag_dn_vec.push_back(std::make_pair(h1D_nbtags_btag_dn_file,systs.at(i)));
        h1D_nbtags_jes_up_vec.push_back(std::make_pair(h1D_nbtags_jes_up_file,systs.at(i)));
        h1D_nbtags_jes_dn_vec.push_back(std::make_pair(h1D_nbtags_jes_dn_file,systs.at(i)));


    }

    // File Loop
    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        // Get File Content
        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        TString filename_(currentFile->GetTitle());
        std::cout << " filename_: " << filename_ << std::endl;

        int iSample_ = -1;

             if(filename_.Contains("TTZ") || filename_.Contains("TTZLOW") )                                                                                     { filename_ = "TTZ";   iSample_ = 1; }
        else if(filename_.Contains("TTW"))                                                                                                                      { filename_ = "TTW";   iSample_ = 2; }
        else if(filename_.Contains("/ZZ.root") || filename_.Contains("/GGHtoZZto4L.root") || filename_.Contains("/WWZ.root") || filename_.Contains("/TWZ.root") || filename_.Contains("/WZZ.root") || filename_.Contains("/WWW.root") ||
                filename_.Contains("/WWDPS.root") || filename_.Contains("/VHtoNonBB.root") || filename_.Contains("/TTTT.root") || filename_.Contains("/TZQ.root") ) { filename_ = "Rares"; iSample_ = 3; }
        else if(filename_.Contains("QQWW.root"))                                                                                                                { filename_ = "WW";    iSample_ = 4; }
        else if(filename_.Contains("/WGToLNuG.root") || filename_.Contains("/ZG.root") || filename_.Contains("/TG.root") || filename_.Contains("/TTG.root"))       { filename_ = "XG";    iSample_ = 5; }
        else if(filename_.Contains("/WZ.root"))                                                                                                                 { filename_ = "WZ";    iSample_ = 6; }
        else if(filename_.Contains("TTHto"))                                                                                                                    { filename_ = "TTH";   iSample_ = 7; }
        else if(filename_.Contains("Data"))                                                                                                                     { filename_ = "Data";  iSample_ = 8; }
        else { std::cout << "ERROR: I don't know what " << filename_ << " is! Skipping file " << filename_ << std::endl; continue; }

        std::cout << "   " << filename_ << std::endl;

        // Loop over Events in current file
        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            // Get Event Content
            samesign.GetEntry(event);
            nEventsTotal++;

            //Progress
            SSAG::progress(nEventsTotal, nEventsChain);

            float scale = 1.0;

            if (ss::is_real_data()) {
                if (!ss::triggers()) continue;
            }

            if(!ss::is_real_data()) {
                float HLTEffcomplement = 0.0;
                scale = ss::scale1fb() * luminosity;

                // if (useMoriondSamples) {
                //       HLTEffcomplement = (1-TotalTriggerSF(ss::lep1_id(), ss::lep1_p4().pt(), ss::lep1_p4().eta(), ss::lep2_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::ht()))
                //                         *(1-TotalTriggerSF(ss::lep2_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::lep3_id(), ss::lep3_p4().pt(), ss::lep3_p4().eta(), ss::ht()))
                //                         *(1-TotalTriggerSF(ss::lep1_id(), ss::lep1_p4().pt(), ss::lep1_p4().eta(), ss::lep3_id(), ss::lep3_p4().pt(), ss::lep3_p4().eta(), ss::ht()));
                // } else {
                //       HLTEffcomplement = (1-HLTEff.getEfficiency(ss::lep1_p4().pt(),ss::lep1_p4().eta(), ss::lep1_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::lep2_id(), ss::ht(), 0))
                //                         *(1-HLTEff.getEfficiency(ss::lep2_p4().pt(),ss::lep2_p4().eta(), ss::lep2_id(), ss::lep3_p4().pt(), ss::lep3_p4().eta(), ss::lep3_id(), ss::ht(), 0))
                //                         *(1-HLTEff.getEfficiency(ss::lep1_p4().pt(),ss::lep1_p4().eta(), ss::lep1_id(), ss::lep3_p4().pt(), ss::lep3_p4().eta(), ss::lep3_id(), ss::ht(), 0));
                // }

                scale *= 1-HLTEffcomplement;
            } else {
                DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (is_duplicate(id) ) continue;
            }

            TString filename = filename_;
            int iSample = iSample_;

            if (doLowPU && !(ss::trueNumInt()[0] < 15 && ss::trueNumInt()[0] >= 10)) continue;
            if (doHighPU && !(ss::trueNumInt()[0] < 30 && ss::trueNumInt()[0] >= 25)) continue;


            if (requireNb0 && ss::nbtags() != 0) continue;

            // if (useMoriondSamples) {
            //     if (!ss::fired_trigger()) continue;
            // } else {
            //     if (!ss::fired_trigger() && ss::is_real_data()) continue;
            // }

            // if (ss::is_real_data()) {
                // if (!ss::passedFilterList()) continue;
                if (!ss::passes_met_filters()) continue;
            // }

            // this guarantees that the third lepton makes a Z with one of the first two leptons
            if ( (ss::hyp_class() != 6)) continue;


            if (doRA7) {
                if(ss::njets() >= 2) continue;
                if(ss::met() < 30.0) continue;//relax to 30 for low lumi
                if(ss::nbtags() != 0) continue;//relax to 30 for low lumi
                
            } else {
                if (!inclusive) {
                    if(ss::met() < 30.0) continue;//relax to 30 for low lumi
                    if (!relaxNjets) {
                        if(ss::njets() < 2) continue;
                    }
                }

            }



            int nb = ss::nbtags();

            // if MC and leptons are not prompt, skip
            if (!ss::is_real_data()) {
                if (ss::lep1_motherID()==2) continue;
                if (ss::lep2_motherID()==2) continue;
                if (ss::lep1_motherID()!=1 && ss::lep1_isPrompt()!=1 && ss::lep1_isDirectPrompt()!=1) continue;
                if (ss::lep2_motherID()!=1 && ss::lep2_isPrompt()!=1 && ss::lep2_isDirectPrompt()!=1) continue;
                if (ss::lep2_motherID()!=1 && ss::lep2_isPrompt()!=1 && ss::lep2_isDirectPrompt()!=1) continue;
                if (!(abs(ss::lep3_mcid()) == 11 || abs(ss::lep3_mcid()) == 13)) continue;


            }


            TString flav = "";

            float zmass;
            float zmass23 = ss::lep2_id() == -ss::lep3_id() ? (ss::lep2_p4()+ss::lep3_p4()).mass() : -999.0;
            float zmass31 = ss::lep3_id() == -ss::lep1_id() ? (ss::lep3_p4()+ss::lep1_p4()).mass() : -999.0;

            int iLepFromW = -1;
            if( fabs(zmass31 - 91.2) < fabs(zmass23 - 91.2) ) {
                zmass = zmass31;
                iLepFromW = 2;

                if (doEE && abs(ss::lep1_id()) != 11) continue;
                if (doMM && abs(ss::lep1_id()) != 13) continue;
                if (doE && abs(ss::lep2_id()) != 11) continue;
                if (doM && abs(ss::lep2_id()) != 13) continue;

                if (abs(ss::lep1_id()) == 11) flav += "ee";
                if (abs(ss::lep1_id()) == 13) flav += "mm";

                if (abs(ss::lep2_id()) == 11) flav += "e";
                if (abs(ss::lep2_id()) == 13) flav += "m";

            } else {
                zmass = zmass23;
                iLepFromW = 1;

                if (doEE && abs(ss::lep2_id()) != 11) continue;
                if (doMM && abs(ss::lep2_id()) != 13) continue;
                if (doE && abs(ss::lep1_id()) != 11) continue;
                if (doM && abs(ss::lep1_id()) != 13) continue;

                if (abs(ss::lep2_id()) == 11) flav += "ee";
                if (abs(ss::lep2_id()) == 13) flav += "mm";

                if (abs(ss::lep1_id()) == 11) flav += "e";
                if (abs(ss::lep1_id()) == 13) flav += "m";

            }

            if (fabs(zmass-91.2) > 15) continue;


            float mtw = -1;
            if(iLepFromW == 1) mtw = MT(ss::lep1_p4().pt(), ss::lep1_p4().phi(), ss::met(), ss::metPhi());
            else mtw = MT(ss::lep2_p4().pt(), ss::lep2_p4().phi(), ss::met(), ss::metPhi());

            // if (doRA7) {
            //     if(mtw < 50) continue;
            //     if(mtw > 100) continue;
            // }

            float fr = 0.0;
            float eff = 1.0;
            bool isDataFake = false;
            if(ss::is_real_data() && (ss::lep3_fo() && !ss::lep3_tight()) && ss::lep1_passes_id() && ss::lep2_passes_id()) {  // lep3 fake
                fr = fakeRate(ss::lep3_id(),ss::lep3_p4().pt(),ss::lep3_p4().eta(),ss::ht()); isDataFake = true;
                eff *= fr / (1-fr);
            }
            if(ss::is_real_data() && (ss::lep2_fo() && !ss::lep2_tight()) && ss::lep1_passes_id() && ss::lep3_passes_id()) {  // lep2 fake
                fr = fakeRate(ss::lep2_id(),ss::lep2_p4().pt(),ss::lep2_p4().eta(),ss::ht()); isDataFake = true;
                eff *= fr / (1-fr);
            }
            if(ss::is_real_data() && (ss::lep1_fo() && !ss::lep1_tight()) && ss::lep2_passes_id() && ss::lep3_passes_id()) {  // lep1 fake
                fr = fakeRate(ss::lep1_id(),ss::lep1_p4().pt(),ss::lep1_p4().eta(),ss::ht()); isDataFake = true;
                eff *= fr / (1-fr);
            }

            if( !(  isDataFake || (ss::lep1_passes_id() && ss::lep2_passes_id() && ss::lep3_passes_id())  ) ) continue;

            // if this is a data event with a fake lepton, fill it instead as a "Fakes" MC event with weight given by fr efficiency
            if(isDataFake) {
                filename = "Fakes";
                iSample = 0;
                scale = eff;
            }

            // if (scaleWZttZ) {
            //     if (filename_ == "TTZ") scale *= getWZSF();
            //     if (filename_ == "WZ") scale *= getttZSF();
            //     if (filename_ == "TTZ" || filename_ == "WZ") {
            //         std::cout << " filename_: " << filename_ << " scale: " << scale << " getWZSF(): " << getWZSF() << " getttZSF(): " << getttZSF() << std::endl;
            //     }
            // }

            // all 4 of these define the CR
            bool goodBtags = nb < 1;
            bool goodNjets = true; // ss::njets() < 5;
            bool goodMet = true;//ss::met() < 200.0;
            bool goodHH = ss::lep1_p4().pt() > 25.0 && ss::lep2_p4().pt() > 20.0;

            float lep1pt = ss::lep1_p4().pt();
            float lep2pt = ss::lep2_p4().pt();
            float lep3pt = ss::lep3_p4().pt();
            float p1 = max(max(lep1pt,lep2pt),lep3pt);
            float p3 = min(min(lep1pt,lep2pt),lep3pt);
            float p2 = lep1pt+lep2pt+lep3pt-p1-p3;

            if (doRA7) {

                goodHH = true;
                if (!(p1 > 25. && p2 > 20. &&  p3 > 20.  )) continue;

                // if (iLepFromW == 1) {
                //     // emulation of thirdIsZ (i.e., want lowest pt lepton to be in Z)
                //     // if lep1 is from W, we're fine since lep1 will never have lowest pt
                // } else if (iLepFromW == 2) {
                //     // if lep2 is from W, want to check that lep2pt is not p3 (lowest pt)
                //     if (fabs(lep2pt-p3) < 0.0001) continue;
                // }

            }

            bool goodMtmin = true;//ss::mtmin() < 120;

            if (relaxNbtags) goodBtags = true;
            if (Nbtagsqeq2) goodBtags = nb >= 2;



            if(! (goodBtags && goodNjets && goodMet && goodHH && goodMtmin) ) continue;

            addToCounter(filename+"_nolepsf_"+flav, scale);
            if (!ss::is_real_data()) {
               if (useLeptonSFs) {
                scale *=  1.0
                    * leptonScaleFactor( ss::lep1_id(), ss::lep1_p4().pt(), ss::lep1_p4().eta(), ss::ht())
                    * leptonScaleFactor( ss::lep2_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::ht())
                    * leptonScaleFactor( ss::lep3_id(), ss::lep3_p4().pt(), ss::lep3_p4().eta(), ss::ht())
                    ;
               }

            addToCounter(filename+"_nopuw_"+flav, scale);

                if (useMoriondSamples) scale *= getTruePUw_Moriond(ss::trueNumInt()[0]);
                else scale *= getTruePUw(ss::trueNumInt()[0]);
            }

            addToCounter(filename+"_"+flav, scale);

            // if (filename == "Data") {
            //     std::cout << ss::run() << " " << ss::lumi() << " " << ss::event() << std::endl;
            // }

            // now we are in the CR


            fill(h1D_ht_vec.at(iSample).first,ss::ht(), scale);
            fill(h1D_mll_vec.at(iSample).first,zmass, scale);

            fill(h1D_met_vec.at(iSample).first,ss::met(), scale);
            fill(h1D_njets_vec.at(iSample).first,ss::njets(), scale);
            if(nb == 2) fill(h1D_njetsnb2_vec.at(iSample).first,ss::njets(), scale);
            fill(h1D_mtmin_vec.at(iSample).first,ss::mtmin(), scale);
            fill(h1D_mtw_vec.at(iSample).first,mtw, scale);
            fill(h1D_lep1pt_vec.at(iSample).first,ss::lep1_p4().pt(), scale);
            fill(h1D_lep2pt_vec.at(iSample).first,ss::lep2_p4().pt(), scale);
            fill(h1D_lep3pt_vec.at(iSample).first,ss::lep3_p4().pt(), scale);

            fill(h1D_olep1pt_vec.at(iSample).first,p1, scale);
            fill(h1D_olep2pt_vec.at(iSample).first,p2, scale);
            fill(h1D_olep3pt_vec.at(iSample).first,p3, scale);

            if (iLepFromW == 2) fill(h1D_lepwpt_vec.at(iSample).first,ss::lep2_p4().pt(), scale);
            if (iLepFromW == 1) fill(h1D_lepwpt_vec.at(iSample).first,ss::lep1_p4().pt(), scale);

            int type = 0;
            if (abs(ss::lep1_id()) == 11) type++;
            if (abs(ss::lep2_id()) == 11) type++;
            if (abs(ss::lep3_id()) == 11) type++;
            fill(h1D_type_vec.at(iSample).first,type, scale);
            
            if (ss::is_real_data()) fill(h1D_nvtx_vec.at(iSample).first,ss::nGoodVertices(), scale);
            else fill(h1D_nvtx_vec.at(iSample).first,ss::nGoodVertices(), scale);

            if (useBtagSF) {
                fill(h1D_nbtags_vec.at(iSample).first, nb, (!ss::is_real_data() && ss::weight_btagsf()>0) ? scale*ss::weight_btagsf() : scale);
            } else {
                fill(h1D_nbtags_vec.at(iSample).first, nb, scale);
            }
            fill(h1D_nbtags_btag_up_vec.at(iSample).first, nb, (!ss::is_real_data() && ss::weight_btagsf()>0) ? scale*ss::weight_btagsf_UP()/ss::weight_btagsf() : scale);
            fill(h1D_nbtags_btag_dn_vec.at(iSample).first, nb, (!ss::is_real_data() && ss::weight_btagsf()>0) ? scale*ss::weight_btagsf_DN()/ss::weight_btagsf() : scale);
            fill(h1D_nbtags_jes_up_vec.at(iSample).first, ss::is_real_data() ? nb : ss::nbtags_unc_up(), scale);
            fill(h1D_nbtags_jes_dn_vec.at(iSample).first, ss::is_real_data() ? nb : ss::nbtags_unc_dn(), scale);

            if( isDataFake || (!ss::is_real_data()) ) {
                nGoodEvents++;
                nGoodEventsWeighted+=scale;
            } else if (!isDataFake) {
                nGoodEventsData++;
            }

        }//event loop

        delete file;
    }//file loop
    
  //Titles for legend

  vector <Color_t> colors; // want this to match yieldMaker.C
  colors.push_back(18);  // fakes
  colors.push_back(kGreen-6);  // ttzh
  colors.push_back(kGreen+3);  // tw
  colors.push_back(kMagenta-7);  // rares
  colors.push_back(kOrange-3);  // ww
  colors.push_back(kViolet+2);  // xg
  colors.push_back(kOrange); // wz
  colors.push_back(kGreen-7);  // tth

  // hacky
  if (scaleWZttZ) {
      h1D_nbtags_vec.at(1).first->Scale(getttZSF());
      h1D_nbtags_vec.at(6).first->Scale(getWZSF());

      float fake_sf = 1.14;
      h1D_nbtags_vec.at(0).first->Scale(fake_sf);

      float rare_sf = 1.14;
      h1D_nbtags_vec.at(2).first->Scale(rare_sf);
      h1D_nbtags_vec.at(3).first->Scale(rare_sf);
      h1D_nbtags_vec.at(4).first->Scale(rare_sf);
      h1D_nbtags_vec.at(5).first->Scale(rare_sf);
      h1D_nbtags_vec.at(7).first->Scale(rare_sf);

  }


    std::cout << " nGoodEventsWeighted: " << nGoodEventsWeighted << " nGoodEvents: " << nGoodEvents << " nGoodEventsData: " << nGoodEventsData << " nEventsTotal: " << nEventsTotal << std::endl;
    printCounter(true);

    TH1F* data;
    // TString comt = Form(" --errHistAtBottom --doCounts --colorTitle --lumi %.2f --lumiUnit fb --percentageInBox --legendRight 0.05 --legendUp 0.05 --noDivisionLabel --noType --outputName pdfs/",luminosity);
    // std::string pct = " --showPercentage ";
    TString comt = Form(" --errHistAtBottom --lumi %.2f --makeTable --lumiUnit fb --noDivisionLabel --legendTaller 0.15 --legendUp -0.15 --dataName Data --legendRight -0.08 --outOfFrame --type Preliminary --legendCounts --outputName pdfs/",luminosity);
    std::string com = comt.Data();

    data = h1D_njetsnb2_vec.back().first; h1D_njetsnb2_vec.pop_back(); dataMCplotMaker(data,h1D_njetsnb2_vec ,titles,"","",com+"h1D_njetsnb2.pdf --isLinear --xAxisOverride N_{jets}", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_njets_vec.back().first; h1D_njets_vec.pop_back(); dataMCplotMaker(data,h1D_njets_vec ,titles,"","",com+"h1D_njets.pdf --isLinear --xAxisOverride N_{jets} --makeTable", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_ht_vec.back().first; h1D_ht_vec.pop_back(); dataMCplotMaker(data,h1D_ht_vec ,titles,"","",com+"h1D_ht.pdf --isLinear --xAxisOverride H_{T} (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_mll_vec.back().first; h1D_mll_vec.pop_back(); dataMCplotMaker(data,h1D_mll_vec ,titles,"","",com+"h1D_mll.pdf --isLinear --xAxisOverride m_{ll} (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_mtmin_vec.back().first; h1D_mtmin_vec.pop_back(); dataMCplotMaker(data,h1D_mtmin_vec ,titles,"","",com+"h1D_mtmin.pdf --isLinear --xAxisOverride m_{T,min} (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_mtw_vec.back().first; h1D_mtw_vec.pop_back(); dataMCplotMaker(data,h1D_mtw_vec ,titles,"","",com+"h1D_mtw.pdf --isLinear --xAxisOverride m_{T}^{W} (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_lep1pt_vec.back().first; h1D_lep1pt_vec.pop_back(); dataMCplotMaker(data,h1D_lep1pt_vec ,titles,"","",com+"h1D_lep1pt.pdf --isLinear --xAxisOverride p_{T}(lep_{1}) (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_lep2pt_vec.back().first; h1D_lep2pt_vec.pop_back(); dataMCplotMaker(data,h1D_lep2pt_vec ,titles,"","",com+"h1D_lep2pt.pdf --isLinear --xAxisOverride p_{T}(lep_{2}) (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_lep3pt_vec.back().first; h1D_lep3pt_vec.pop_back(); dataMCplotMaker(data,h1D_lep3pt_vec ,titles,"","",com+"h1D_lep3pt.pdf --isLinear --xAxisOverride p_{T}(lep_{3}) (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_olep1pt_vec.back().first; h1D_olep1pt_vec.pop_back(); dataMCplotMaker(data,h1D_olep1pt_vec ,titles,"","",com+"h1D_olep1pt.pdf --isLinear --xAxisOverride ordered p_{T}(lep_{1}) (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_olep2pt_vec.back().first; h1D_olep2pt_vec.pop_back(); dataMCplotMaker(data,h1D_olep2pt_vec ,titles,"","",com+"h1D_olep2pt.pdf --isLinear --xAxisOverride ordered p_{T}(lep_{2}) (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_olep3pt_vec.back().first; h1D_olep3pt_vec.pop_back(); dataMCplotMaker(data,h1D_olep3pt_vec ,titles,"","",com+"h1D_olep3pt.pdf --isLinear --xAxisOverride ordered p_{T}(lep_{3}) (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_lepwpt_vec.back().first; h1D_lepwpt_vec.pop_back(); dataMCplotMaker(data,h1D_lepwpt_vec ,titles,"","",com+"h1D_lepwpt.pdf --isLinear --xAxisOverride p_{T}(lep_{W}) (GeV)", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_nvtx_vec.back().first; h1D_nvtx_vec.pop_back(); dataMCplotMaker(data,h1D_nvtx_vec ,titles,"","",com+"h1D_nvtx.pdf --isLinear --xAxisOverride nGoodVtx --makeRootFile", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_type_vec.back().first; h1D_type_vec.pop_back(); dataMCplotMaker(data,h1D_type_vec ,titles,"","",com+"h1D_type.pdf --isLinear --xAxisOverride flavor --makeTable --xAxisBinLabels MMM,MME,MEE,EEE ", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_met_vec.back().first; h1D_met_vec.pop_back(); dataMCplotMaker(data,h1D_met_vec ,titles,"","",com+"h1D_met.pdf --isLinear --xAxisOverride #slash{E}_{T} (GeV)", vector <TH1F*>(), vector<string>(), colors);

    data = h1D_nbtags_vec.back().first; h1D_nbtags_vec.pop_back(); dataMCplotMaker(data,h1D_nbtags_vec ,titles,"","",com+"h1D_nbtags.pdf --isLinear --xAxisOverride nbtags  --makeRootFile --makeTable ", vector <TH1F*>(), vector<string>(), colors);
    dataMCplotMaker(data, h1D_nbtags_vec, titles, "", "", Form("--lumi %.1f --outputName pdfs/3lep_nbtags_all_SS.pdf --noDivisionLabel --noXaxisUnit --xAxisLabel N_{b} --nDivisions 4 --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros   --outOfFrame ", getLumi()), vector <TH1F*>(), vector <string>(), colors);



    data = h1D_nbtags_btag_up_vec.back().first; h1D_nbtags_btag_up_vec.pop_back(); dataMCplotMaker(data,h1D_nbtags_btag_up_vec ,titles,"","",com+"h1D_nbtags_btag_up.pdf --isLinear --xAxisOverride nbtags --makeRootFile ", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_nbtags_btag_dn_vec.back().first; h1D_nbtags_btag_dn_vec.pop_back(); dataMCplotMaker(data,h1D_nbtags_btag_dn_vec ,titles,"","",com+"h1D_nbtags_btag_dn.pdf --isLinear --xAxisOverride nbtags --makeRootFile ", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_nbtags_jes_up_vec.back().first; h1D_nbtags_jes_up_vec.pop_back(); dataMCplotMaker(data,h1D_nbtags_jes_up_vec ,titles,"","",com+"h1D_nbtags_jes_up.pdf --isLinear --xAxisOverride nbtags --makeRootFile ", vector <TH1F*>(), vector<string>(), colors);
    data = h1D_nbtags_jes_dn_vec.back().first; h1D_nbtags_jes_dn_vec.pop_back(); dataMCplotMaker(data,h1D_nbtags_jes_dn_vec ,titles,"","",com+"h1D_nbtags_jes_dn.pdf --isLinear --xAxisOverride nbtags --makeRootFile ", vector <TH1F*>(), vector<string>(), colors);


    return 0;
}
