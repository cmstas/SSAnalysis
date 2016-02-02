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
#include "../commonUtils.h"
#include "../CORE/SSSelections.h"
#include "../CORE/Tools/dorky/dorky.cc"

using namespace std;
using namespace duplicate_removal;

int scan(){
    TChain *ch = new TChain("t");
    std::vector<std::string> titles;

    TString tag = getTag();

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTBAR_PH.root");  titles.push_back("Fakes");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/SINGLETOP*.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DY_high.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DY_low.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WJets.root");

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTZL.root");    titles.push_back("ttZH");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTZLOW.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTHtoNonBB.root");

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTW.root");   titles.push_back("ttW");

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/ZZ.root");     titles.push_back("Rares");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/GGHtoZZto4L.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WWZ.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WZZ.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WWDPS.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/VHtoNonBB.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTTT.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TZQ.root");
    //ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/T6TTWW_600_425_50.root ");

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/QQWW.root");     titles.push_back("WW");

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WGToLNuG.root");     titles.push_back("XG");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/ZG.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TG.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/TTG.root");

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/WZ.root");     titles.push_back("WZ");

    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleEGC_05oct.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleEGD_05oct.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleEGD_v4.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleMuonC_05oct.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleMuonD_05oct.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataDoubleMuonD_v4.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataMuonEGC_05oct.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataMuonEGD_05oct.root");
    ch->Add("/nfs-7/userdata/ss2015/ssBabies/"+tag+"/DataMuonEGD_v4.root");

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();
    int nGoodEvents = 0;
    int nGoodEventsData = 0;
    float nGoodEventsWeighted = 0;
    float luminosity = getLumi();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    vector<TH1F*> h1D_njets_vec;
    vector<TH1F*> h1D_ht_vec;
    vector<TH1F*> h1D_met_vec;
    vector<TH1F*> h1D_mt_vec;
    vector<TH1F*> h1D_zmass_vec;
    vector<TH1F*> h1D_hyp_class_vec;
    vector<TH1F*> h1D_nbtags_vec;
    vector<TH1F*> h1D_mtmin_vec;

    vector<TH1F*> h1D_lep1pt_vec;
    vector<TH1F*> h1D_lep2pt_vec;

    vector<TH1F*> h1D_yields_HH_vec;
    vector<TH1F*> h1D_yields_HL_vec;
    vector<TH1F*> h1D_yields_LL_vec;

    TH2F* h2D_met_mtmin_wz = new TH2F("met_mtmin", "", 20, 0, 300, 20, 0, 300); 
    TH2F* h2D_njets_nbtags_wz = new TH2F("njets_nbtags", "", 7,0,7, 7,0,7);
    TH2F* h2D_ptlep1_ptlep2_wz = new TH2F("ptlep1_ptlep2", "", 40,0,400, 40,0,400);
    TH2F* h2D_ht_njets_wz = new TH2F("ht_njets_wz", "", 20,0,1000, 7,0,7);
    TH2F* h2D_ht_sumleppt_wz = new TH2F("ht_sumleppt_wz", "", 20,0,1000, 40,0,400);

    vector<TString> files = {"Fakes","ttZH","TTW","Rares","WW","XG","WZ","Data"}; 
    for(int i = 0; i < files.size(); i++) {

        TH1F* h1D_njets_file = new TH1F("njets"+files.at(i), "Njets;;Entries", 10, 0, 10);
        TH1F* h1D_ht_file = new TH1F("ht"+files.at(i), "H_{T};GeV;Entries", 20, 0, 1000); 
        TH1F* h1D_met_file = new TH1F("met"+files.at(i), "#slash{E}_{T};GeV;Entries", 20, 0, 300); 
        TH1F* h1D_mt_file = new TH1F("mt"+files.at(i), "M_{T};GeV;Entries", 20, 0, 450); 
        TH1F* h1D_zmass_file = new TH1F("zmass"+files.at(i), "Z Mass;GeV;Entries", 42, 70, 112); 
        TH1F* h1D_hyp_class_file = new TH1F("hypclass"+files.at(i), "hyp_class;ID;Entries", 7, 0, 7); 
        TH1F* h1D_nbtags_file = new TH1F("nbtags"+files.at(i), "Nbtags;nbtags;Entries", 4, 0, 4); 
        TH1F* h1D_mtmin_file = new TH1F("mtmin"+files.at(i), "mtmin;mtmin;Entries", 20, 0, 200); 

        TH1F* h1D_lep1pt_file = new TH1F("lep1pt"+files.at(i), "lep1pt;lep1pt;Entries", 40, 0, 400); 
        TH1F* h1D_lep2pt_file = new TH1F("lep2pt"+files.at(i), "lep2pt;lep2pt;Entries", 40, 0, 400); 

        TH1F* h1D_yields_HH_file = new TH1F("h1D_yields_HH"+files.at(i), "", 32, 1, 33); 
        TH1F* h1D_yields_HL_file = new TH1F("h1D_yields_HL"+files.at(i), "", 26, 1, 27); 
        TH1F* h1D_yields_LL_file = new TH1F("h1D_yields_LL"+files.at(i), "", 8, 1, 9); 

        h1D_njets_vec.push_back(h1D_njets_file); 
        h1D_ht_vec.push_back(h1D_ht_file); 
        h1D_met_vec.push_back(h1D_met_file); 
        h1D_mt_vec.push_back(h1D_mt_file); 
        h1D_mtmin_vec.push_back(h1D_mtmin_file); 
        h1D_zmass_vec.push_back(h1D_zmass_file); 
        h1D_hyp_class_vec.push_back(h1D_hyp_class_file); 
        h1D_nbtags_vec.push_back(h1D_nbtags_file); 
        h1D_lep1pt_vec.push_back(h1D_lep1pt_file); 
        h1D_lep2pt_vec.push_back(h1D_lep2pt_file); 

        h1D_yields_HH_vec.push_back(h1D_yields_HH_file);
        h1D_yields_HL_vec.push_back(h1D_yields_HL_file);
        h1D_yields_LL_vec.push_back(h1D_yields_LL_file);

    }

    initCounter();
    // File Loop
    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        // Get File Content
        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        TString filename_(currentFile->GetTitle());

        int iSample_ = -1;

        // if(filename_.Contains("TTBAR") || filename_.Contains("SINGLETOP")  || filename_.Contains("DY_")  || filename_.Contains("WJets") )                          { filename_ = "Fakes"; iSample_ = 0; }
             if(filename_.Contains("TTZL") || filename_.Contains("TTZLOW") || filename_.Contains("TTHtoNonBB"))                                                   { filename_ = "TTZ";   iSample_ = 1; }
        else if(filename_.Contains("TTW"))                                                                                                                      { filename_ = "TTW";   iSample_ = 2; }
        else if(filename_.Contains("/ZZ.root") || filename_.Contains("/GGHtoZZto4L.root") || filename_.Contains("/WWZ.root") || filename_.Contains("/WZZ.root") || //filename_.Contains("/T6TTWW_600_425_50.root") ||
                filename_.Contains("/WWDPS.root") || filename_.Contains("/VHtoNonBB.root") || filename_.Contains("/TTTT.root") || filename_.Contains("/TZQ.root") ) { filename_ = "Rares"; iSample_ = 3; }
        else if(filename_.Contains("QQWW.root"))                                                                                                                { filename_ = "WW";    iSample_ = 4; }
        else if(filename_.Contains("/WGToLNuG.root") || filename_.Contains("/ZG.root") || filename_.Contains("/TG.root") || filename_.Contains("/TTG.root"))       { filename_ = "XG";    iSample_ = 5; }
        else if(filename_.Contains("/WZ.root"))                                                                                                                 { filename_ = "WZ";    iSample_ = 6; }
        else if(filename_.Contains("Data"))                                                                                                                     { filename_ = "Data";  iSample_ = 7; }
        else { std::cout << "ERROR: I don't know what " << filename_ << " is! Skipping file " << filename_ << std::endl; continue; }

        // Loop over Events in current file
        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            // Get Event Content
            samesign.GetEntry(event);
            nEventsTotal++;

            //Progress
            SSAG::progress(nEventsTotal, nEventsChain);

            float scale = 1.0;
            if(!ss::is_real_data()) {
                scale = ss::scale1fb() * luminosity * getTruePUw(ss::trueNumInt()[0]);
            } else {
                DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (is_duplicate(id) ) continue;
            }

            TString filename = filename_;
            int iSample = iSample_;

            // fill these before making cuts
            fill(h1D_hyp_class_vec.at(iSample), ss::hyp_class(), scale);

            if (!ss::fired_trigger()) continue;
            if (ss::is_real_data()) {
                if (!ss::passedFilterList()) continue;
                if (!ss::passes_met_filters()) continue;
            }

            // this guarantees that the third lepton makes a Z with one of the first two leptons
            if( ! (ss::hyp_class() == 6) ) continue;

            if(ss::met() < 30.0) continue;//relax to 30 for low lumi
            if(ss::njets() < 2) continue;
            
            // if MC and leptons are not prompt, skip
            if (!ss::is_real_data()) {
                if (ss::lep1_motherID()==2) continue;
                if (ss::lep2_motherID()==2) continue;
                if (ss::lep1_motherID()!=1 && ss::lep1_isPrompt()!=1 && ss::lep1_isDirectPrompt()!=1) continue;
                if (ss::lep2_motherID()!=1 && ss::lep2_isPrompt()!=1 && ss::lep2_isDirectPrompt()!=1) continue;
                if (ss::lep2_motherID()!=1 && ss::lep2_isPrompt()!=1 && ss::lep2_isDirectPrompt()!=1) continue;
                if (!(abs(ss::lep3_mcid()) == 11 || abs(ss::lep3_mcid()) == 13)) continue;

                scale *=  triggerScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::ht())
                        * leptonScaleFactor( ss::lep1_id(), ss::lep1_p4().pt(), ss::lep1_p4().eta(), ss::ht())
                        * leptonScaleFactor( ss::lep2_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::ht())
                        * leptonScaleFactor( ss::lep3_id(), ss::lep3_p4().pt(), ss::lep3_p4().eta(), ss::ht());
            }

            float zmass;
            float zmass23 = ss::lep2_id() == -ss::lep3_id() ? (ss::lep2_p4()+ss::lep3_p4()).mass() : -999.0;
            float zmass31 = ss::lep3_id() == -ss::lep1_id() ? (ss::lep3_p4()+ss::lep1_p4()).mass() : -999.0;

            if( fabs(zmass31 - 91.2) < fabs(zmass23 - 91.2) ) zmass = zmass31;
            else zmass = zmass23;

            if (fabs(zmass-91.2) > 15) continue;


            float fr = 0.0;
            bool isDataFake = false;
            if(ss::is_real_data() && (ss::lep3_fo() && !ss::lep3_tight())) {  // lep3 fake
                fr = fakeRate(ss::lep3_id(),ss::lep3_p4().pt(),ss::lep3_p4().eta(),ss::ht()); isDataFake = true;
            }
            float eff = fr / (1-fr);

            if( !(  isDataFake || (ss::lep1_passes_id() && ss::lep2_passes_id() && ss::lep3_passes_id())  ) ) continue;

            //this is what santiago is doing:
            // if (ss::lep1_id()*ss::lep2_id()<0) continue;

            // if this is a data event with a fake lepton, fill it instead as a "Fakes" MC event with weight given by fr efficiency
            if(isDataFake) {
                filename = "Fakes";
                iSample = 0;
                scale = eff;
            }

            // all 4 of these define the CR
            bool goodBtags = ss::nbtags() < 1;
            bool goodNjets = ss::njets() < 5;
            bool goodMet = true;//ss::met() < 200.0;
            bool goodHH = ss::lep1_p4().pt() > 25.0 && ss::lep2_p4().pt() > 20.0;
            bool goodMtmin = true;//ss::mtmin() < 120;

            if(filename.Contains("/WZ.root"))    {
                h2D_ptlep1_ptlep2_wz->Fill(ss::lep2_p4().pt(),ss::lep1_p4().pt());
                h2D_ht_njets_wz->Fill(ss::ht(),ss::njets());
                h2D_ht_sumleppt_wz->Fill( ss::ht(), ss::lep2_p4().pt()+ss::lep1_p4().pt() );
                h2D_njets_nbtags_wz->Fill(ss::nbtags(),ss::njets());
                h2D_met_mtmin_wz->Fill(ss::mtmin(),ss::met());
            }

            if(goodNjets && goodMet && goodHH && goodMtmin)  {
                if(ss::nbtags() < 1)  addToCounter("2:nbtags<1_" +filename,scale);
                if(ss::nbtags() >= 1) addToCounter("2:nbtags>=1_"+filename,scale);
                fill(h1D_nbtags_vec.at(iSample), ss::nbtags(), scale);
            }


            if(goodBtags && goodNjets && goodMet && goodMtmin)  {
                anal_type_t categ = analysisCategory(abs(ss::lep1_id()),abs(ss::lep2_id()),ss::lep1_p4().pt(), ss::lep2_p4().pt());  
                if(categ == HighHigh) addToCounter("6:HH_" +filename,scale);
                if(categ == HighLow)  addToCounter("6:HL_" +filename,scale);
                fill(h1D_lep1pt_vec.at(iSample),ss::lep1_p4().pt(), scale);
                fill(h1D_lep2pt_vec.at(iSample),ss::lep2_p4().pt(), scale);
            }

            if(goodBtags && goodNjets && goodMet && goodHH)  {
                if(ss::mtmin() < 120)  addToCounter("5:mtmin<120_" +filename,scale);
                if(ss::mtmin() >= 120) addToCounter("5:mtmin>=120_"+filename,scale);
                fill(h1D_mtmin_vec.at(iSample),ss::mtmin(), scale);
            }


            if(goodBtags && goodMet && goodHH && goodMtmin)  {
                if(ss::njets() < 5)  addToCounter("1:njets<5_" +filename,scale);
                if(ss::njets() >= 5) addToCounter("1:njets>=5_"+filename,scale);
                fill(h1D_njets_vec.at(iSample),ss::njets(), scale);
            }

            if(goodBtags && goodNjets && goodHH && goodMtmin)  {
                if(ss::met() < 200)  addToCounter("3:met<200_" +filename,scale);
                if(ss::met() >= 200) addToCounter("3:met>=200_"+filename,scale);
                fill(h1D_met_vec.at(iSample),ss::met(), scale);
            }


            if(! (goodBtags && goodNjets && goodMet && goodHH && goodMtmin) ) continue;

            // now we are in the CR

            if(ss::ht() < 300)  addToCounter("4:ht<300_" +filename,scale);
            if(ss::ht() >= 300) addToCounter("4:ht>=300_"+filename,scale);

            fill(h1D_zmass_vec.at(iSample),zmass, scale); 
            fill(h1D_ht_vec.at(iSample),ss::ht(), scale);

            addToCounter(filename, scale);

            if( (!ss::is_real_data()) ) {
                nGoodEvents++;
                nGoodEventsWeighted+=scale;
            } else {
                nGoodEventsData++;
            }




        }//event loop

        delete file;
    }//file loop

    std::cout << " nGoodEventsWeighted: " << nGoodEventsWeighted << " nGoodEvents: " << nGoodEvents << " nGoodEventsData: " << nGoodEventsData << " nEventsTotal: " << nEventsTotal << std::endl;
    printCounter(true);

    TH1F* data;
    TString comt = Form(" --errHistAtBottom --doCounts --colorTitle --lumi %.2f --lumiUnit fb --percentageInBox --legendRight 0.05 --legendUp 0.05 --noDivisionLabel --noType --outputName pdfs/",getLumi());
    std::string com = comt.Data();
    std::string pct = " --showPercentage ";
    std::string spec = "";

    std::string HHbins = " --xAxisVerticalBinLabels --xAxisBinLabels 1A,2A,3A,4A,5A,6A,7A,8A,9A,10A,11A,12A,13A,14A,15A,16A,17A,18A,19A,20A,21A,22A,23A,24A,25A,26A,27A,28A,29A,30A,31A,32A";
    std::string HLbins = " --xAxisVerticalBinLabels --xAxisBinLabels 1B,2B,3B,4B,5B,6B,7B,8B,9B,10B,11B,12B,13B,14B,15B,16B,17B,18B,19B,20B,21B,22B,23B,24B,25B,26B";
    std::string LLbins = " --xAxisVerticalBinLabels --xAxisBinLabels 1C,2C,3C,4C,5C,6C,7C,8C";

    data = h1D_njets_vec.back(); h1D_njets_vec.pop_back(); dataMCplotMaker(data,h1D_njets_vec    ,titles,"Njets",spec,com+"h1D_njets.pdf           --isLinear --vLine 5 --xAxisOverride njets");
    data = h1D_ht_vec.back(); h1D_ht_vec.pop_back(); dataMCplotMaker(data,h1D_ht_vec             ,titles,"H_{T}",spec,com+"h1D_ht.pdf              --isLinear             --xAxisOverride H_{T} [GeV]");
    data = h1D_mtmin_vec.back(); h1D_mtmin_vec.pop_back(); dataMCplotMaker(data,h1D_mtmin_vec    ,titles,"m_{T,min}",spec,com+"h1D_mtmin.pdf       --isLinear             --xAxisOverride m_{T,min} [GeV]");
    data = h1D_met_vec.back(); h1D_met_vec.pop_back(); dataMCplotMaker(data,h1D_met_vec          ,titles,"#slash{E}_{T}",spec,com+"h1D_met.pdf     --isLinear             --xAxisOverride #slash{E}_{T} [GeV]");

    data = h1D_nbtags_vec.back(); h1D_nbtags_vec.pop_back(); dataMCplotMaker(data,h1D_nbtags_vec ,titles,"Nbtags",spec,com+"h1D_nbtags.pdf         --isLinear --vLine 1 --xAxisOverride nbtags");
    data = h1D_lep1pt_vec.back(); h1D_lep1pt_vec.pop_back(); dataMCplotMaker(data,h1D_lep1pt_vec ,titles,"p_{T}(lep_{1})",spec,com+"h1D_lep1pt.pdf --isLinear --vLine 25 --xAxisOverride p_{T}(lep_{1}) [GeV]");
    data = h1D_lep2pt_vec.back(); h1D_lep2pt_vec.pop_back(); dataMCplotMaker(data,h1D_lep2pt_vec ,titles,"p_{T}(lep_{2})",spec,com+"h1D_lep2pt.pdf --isLinear --vLine 25 --xAxisOverride p_{T}(lep_{2}) [GeV]");

    drawHist2D(h2D_ht_sumleppt_wz , "pdfs/h2D_ht_sumleppt_wz.pdf" , "--logscale --title WZ: p_{T}(lep_{1})+p_{T}(lep_{2}) vs H_{T} --xlabel  sum H_{T} --ylabel leppt");
    drawHist2D(h2D_ht_njets_wz , "pdfs/h2D_ht_njets_wz.pdf" , "--logscale --title WZ: Njets vs H_{T} --xlabel  H_{T} --ylabel Njets");
    drawHist2D(h2D_ptlep1_ptlep2_wz , "pdfs/h2D_ptlep1_ptlep2_wz.pdf" , "--logscale --title WZ: p_{T}(lep_{1}) vs p_{T}(lep_{2}) --xlabel  ptlep2 --ylabel ptlep1");
    drawHist2D(h2D_njets_nbtags_wz  , "pdfs/h2D_njets_nbtags_wz.pdf"  , "--logscale --title WZ: Njets vs Nbtags --xlabel  nbtags --ylabel njets");
    drawHist2D(h2D_met_mtmin_wz     , "pdfs/h2D_met_mtmin_wz.pdf"     , "--logscale --title WZ: #slash{E}_{T} vs m_{T,min} --xlabel  mtmin --ylabel met");

    return 0;
}
