#include "../../software/dataMCplotMaker2/dataMCplotMaker.h"
#include "../../classFiles/v8.04/SS.h"
#include "../../CORE/Tools/dorky/dorky.h"
#include "../../software/tableMaker/CTable.h"
#include "../../commonUtils.h"
#include "../../CORE/SSSelections.h"

#include "TChain.h"
#include "TColor.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

//Namespace
using namespace std;

//Lumi
// float lumiAG = 6.26;
float lumiAG = getLumi();

//Data or Signal
enum type_ag { DATA, SIGNAL, FAKES }; 
type_ag type = DATA;  // opposite sign
// type_ag type = FAKES;  // tight-loose
bool checkTailRegions = false;
bool useNewBaseline = true;
int pickCharge = 0; // 1 for ++, -1 for -- and 0 for no charge requirement;

bool doE = false;
bool doMu = false;
bool doRelax = false;
bool doMoriond = true;
bool doBtagSF = true;


//Global Definitions
pair <vector <TH1D*>, vector <double> > makePlots(TChain *chain);
void drawPlot(int which, string title = "", string subtitle = "", string xaxis = "quantity", string name2 = "blah", string options = "");
pair <vector <TH1D*>, vector <double> > data_plots;
pair <vector <TH1D*>, vector <double> > ttx_plots;
pair <vector <TH1D*>, vector <double> > ttz_plots;
pair <vector <TH1D*>, vector <double> > ttw_plots;
pair <vector <TH1D*>, vector <double> > wz_plots;    
pair <vector <TH1D*>, vector <double> > ttbar_plots; 
// pair <vector <TH1D*>, vector <double> > st_plots; 
pair <vector <TH1D*>, vector <double> > dy_plots;
pair <vector <TH1D*>, vector <double> > wjets_plots;
pair <vector <TH1D*>, vector <double> > mb_plots;
pair <vector <TH1D*>, vector <double> > wzz_plots;
pair <vector <TH1D*>, vector <double> > zz_plots;
pair <vector <TH1D*>, vector <double> > qqww_plots; 
pair <vector <TH1D*>, vector <double> > fake_plots; 

//Main function
void plots(TString opts){

    if (opts.Contains("OS")) {
        type = DATA;  // opposite sign
        std::cout << "Doing OS" << std::endl;
    } else if (opts.Contains("TL")) {
        type = FAKES;  // tight-loose
        std::cout << "Doing TL" << std::endl;
    }

  //Declare chains

  TString pfx  = Form("/nfs-7/userdata/ss2015/ssBabies/%s/", getTag().Data());
  TString pfxData  = Form("/nfs-7/userdata/ss2015/ssBabies/%s/", getTagData().Data());

  // TString pfx_moriond  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.01/output/";
  TString pfx_moriond  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/";
  if (doMoriond) {
      pfx = pfx_moriond;
      pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/";
  }

  TChain *data  = new TChain("t", "data"); data ->Add(Form("%s/Data*.root"     , pfxData.Data()));
  TChain *ttz   = new TChain("t");         ttz  ->Add(Form("%s/TTZnlo.root"      , pfx.Data())); 
                                           ttz  ->Add(Form("%s/TTZLOW.root"      , pfx.Data())); 
  TChain *ttx   = new TChain("t");         ttx  ->Add(Form("%s/TTHtoNonBB.root" , pfx.Data())); 
  TChain *ttw   = new TChain("t");         ttw  ->Add(Form("%s/TTWnlo.root",     pfx.Data())); 
  TChain *wz    = new TChain("t");         wz   ->Add(Form("%s/WZ.root"        , pfx.Data())); 
  TChain *ttbar = new TChain("t");         ttbar->Add(Form("%s/TTBAR_PH*.root"  , pfx.Data())); 
  TChain *dy    = new TChain("t");         dy   ->Add(Form("%s/DY_high*.root"   , pfx.Data())); 
                                           dy   ->Add(Form("%s/DY_low*.root"    , pfx.Data())); 
  TChain *wjets = new TChain("t");         wjets->Add(Form("%s/WJets.root"     , pfx.Data())); 
  TChain *mb    = new TChain("t");         mb   ->Add(Form("%s/WWZ.root"       , pfx.Data())); 
                                           mb  ->Add(Form("%s/WWDPS.root"       , pfx.Data())); 
  TChain *wzz   = new TChain("t");         wzz  ->Add(Form("%s/WZZ.root"       , pfx.Data())); 
  TChain *zz    = new TChain("t");         zz   ->Add(Form("%s/ZZ.root"        , pfx.Data())); 
  TChain *qqww  = new TChain("t");         qqww ->Add(Form("%s/QQWW.root"      , pfx.Data())); 

  duplicate_removal::clear_list();

  //Make plots
  dy_plots    = makePlots(dy);
  if (type == DATA || type == FAKES) data_plots = makePlots(data);
  ttz_plots   = makePlots(ttz);
  ttw_plots   = makePlots(ttw);
  ttx_plots   = makePlots(ttx);
  for (unsigned int i = 0; i < ttx_plots.first.size(); i++) ttx_plots.first[i]->Add(ttz_plots.first[i]); 
  for (unsigned int i = 0; i < ttx_plots.first.size(); i++) ttx_plots.first[i]->Add(ttw_plots.first[i]); 
  wz_plots    = makePlots(wz);
  ttbar_plots = makePlots(ttbar);
  wzz_plots    = makePlots(wzz);
  zz_plots    = makePlots(zz);
  mb_plots    = makePlots(mb);
  for (unsigned int i = 0; i < mb_plots.first.size(); i++) mb_plots.first[i]->Add(zz_plots.first[i]); 
  for (unsigned int i = 0; i < mb_plots.first.size(); i++) mb_plots.first[i]->Add(wzz_plots.first[i]); 
  wjets_plots = makePlots(wjets);
  qqww_plots  = makePlots(qqww);

  string sig = (type == DATA) ? "OS" : "SS"; 
  if(type == FAKES) sig = "tight-loose";


  //Draw plots
  drawPlot(0 , Form("MET in %s dilepton pairs"        , sig.c_str()) , "MET > 50, #geq 2 jets"                         , "MET"        , "MET"         , "--yTitleOffset -0.10"                                 ); 
  drawPlot(1 , Form("HT in %s dilepton pairs"         , sig.c_str()) , "MET > 50, #geq 2 jets" , "HT"         , "HT"          , "--yTitleOffset -0.10"                                 );
  drawPlot(2 , Form("M_{ll} in %s dilepton pairs"     , sig.c_str()) , "MET > 50, #geq 2 jets" , "M_{ll}"     , "MLL"         , "--yTitleOffset -0.10"                                 );
  drawPlot(3 , Form("M_{T}^{min} in %s dilepton pairs", sig.c_str()) , "MET > 50, #geq 2 jets" , "M_{T}^{min}", "MTMIN"       , "--yTitleOffset -0.10"                                 );
  drawPlot(4 , Form("N_{jets} in %s dilepton pairs"   , sig.c_str()) , "MET > 50"              , "N_{jets}"   , "NJETS"       , "--nDivisions 6 --noDivisionLabel --yTitleOffset -0.10");
  drawPlot(5 , Form("N_{b-tags} in %s dilepton pairs" , sig.c_str()) , "MET > 50, #geq 2 jets" , "N_{b-tags}" , "NBTAGS"      , "--nDivisions 4 --noDivisionLabel --yTitleOffset -0.10");
  drawPlot(6 , Form("lep p_{T} in %s dilepton pairs"  , sig.c_str()) , "MET > 50, #geq 2 jets" , "p_{T}"      , "PT"          , "--yTitleOffset -0.10");
  drawPlot(7 , Form("#eta in %s dilepton pairs"       , sig.c_str()) , "MET > 50, #geq 2 jets" , "#eta"       , "ETA"         , "--yTitleOffset -0.10 --setMaximum 10000000 --setMinimum 1");
  drawPlot(8 , Form("jet p_{T} in %s dilepton pairs"  , sig.c_str()) , "MET > 50, #geq 2 jets" , "p_{T}"      , "JETPT"       , "--yTitleOffset -0.10");
  drawPlot(9 , Form("e p_{T} in %s dilepton pairs"  , sig.c_str()) , "MET > 50, #geq 2 jets" , "p_{T}^{e}"      , "PTE"          , "--yTitleOffset -0.10");
  drawPlot(10 , Form("#mu p_{T} in %s dilepton pairs"  , sig.c_str()) , "MET > 50, #geq 2 jets" , "p_{T}^{#mu}"      , "PTM"          , "--yTitleOffset -0.10");

  //Print yields
  vector <double> total = {0,0,0,0}; 
  for (int i = 0; i < 4; i++) total.at(i) += ttbar_plots.second.at(i);
  // for (int i = 0; i < 4; i++) total.at(i) += st_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += dy_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += wz_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += ttx_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += ttz_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += ttw_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += wjets_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += mb_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += wzz_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += zz_plots.second.at(i);
  for (int i = 0; i < 4; i++) total.at(i) += qqww_plots.second.at(i);

  CTable table;
  table.setPrecision(2);
  cout << data_plots.second.size(); 
  cout << fake_plots.second.size(); 
  table.setTable() ( "ee", "em", "mm", "total" ) 
                   ("ttbar", ttbar_plots.second.at(0), ttbar_plots.second.at(1), ttbar_plots.second.at(2), ttbar_plots.second.at(3))
                   ("dy", dy_plots.second.at(0), dy_plots.second.at(1), dy_plots.second.at(2), dy_plots.second.at(3))
                   ("wz", wz_plots.second.at(0), wz_plots.second.at(1), wz_plots.second.at(2), wz_plots.second.at(3))
                   ("tth", ttx_plots.second.at(0), ttx_plots.second.at(1), ttx_plots.second.at(2), ttx_plots.second.at(3))
                   ("ttz", ttz_plots.second.at(0), ttz_plots.second.at(1), ttz_plots.second.at(2), ttz_plots.second.at(3))
                   ("ttw", ttw_plots.second.at(0), ttw_plots.second.at(1), ttw_plots.second.at(2), ttw_plots.second.at(3))
                   ("wjets", wjets_plots.second.at(0), wjets_plots.second.at(1), wjets_plots.second.at(2), wjets_plots.second.at(3))
                   ("wwz", mb_plots.second.at(0), mb_plots.second.at(1), mb_plots.second.at(2), mb_plots.second.at(3))
                   ("wzz", wzz_plots.second.at(0), wzz_plots.second.at(1), wzz_plots.second.at(2), wzz_plots.second.at(3))
                   ("zz", zz_plots.second.at(0), zz_plots.second.at(1), zz_plots.second.at(2), zz_plots.second.at(3))
                   ("qqww", qqww_plots.second.at(0), qqww_plots.second.at(1), qqww_plots.second.at(2), qqww_plots.second.at(3))
                   ("total MC", total.at(0), total.at(1), total.at(2), total.at(3))
                   ("data", data_plots.second.at(0), data_plots.second.at(1), data_plots.second.at(2), data_plots.second.at(3));
 table.print(); 
 table.saveTex("table.tex"); 

}

//Fill plot function
pair <vector <TH1D*>, vector <double> > makePlots(TChain *chain){


  //Declare plots

  // TH1D *met    = new TH1D("met"    , "met"    , 50  ,   0, 1000);
  // TH1D *ht     = new TH1D("ht"     , "ht"     , 50  ,   0, 1800);
  // TH1D *mll    = new TH1D("mll"    , "mll"    , 50  ,   0, 300);
  // TH1D *mtmin  = new TH1D("mtmin"  , "mtmin"  , 50  ,   0, 300);
  // TH1D *njets  = new TH1D("njets"  , "njets"  , 6  ,    0, 6  );
  // TH1D *nbtags = new TH1D("nbtags" , "nbtags" , 4  ,    0, 4  );
  // TH1D *pt     = new TH1D("pt"     , "pt"     , 50 ,    0, 300);
  // TH1D *eta    = new TH1D("eta"    , "eta"    , 50 , -3.2, 3.2);
  // TH1D *jetpt  = new TH1D("jetpt"  , "jetpt"  , 50 ,    0, 300);
  // TH1D *pte     = new TH1D("pte"     , "pte"     , 50 ,    0, 300);
  // TH1D *ptm     = new TH1D("ptm"     , "ptm"     , 50 ,    0, 300);
  TH1D *met    = new TH1D("met"    , "met"    , 40  ,   0, 800);
  // TH1D *ht     = new TH1D("ht"     , "ht"     , 25  ,   0, 1800);
  TH1D *ht     = new TH1D("ht"     , "ht"     , 50  ,   0, 800);
  TH1D *mll    = new TH1D("mll"    , "mll"    , 25  ,   0, 300);
  // TH1D *mll    = new TH1D("mll"    , "mll"    , 60  ,   75, 105);
  TH1D *mtmin  = new TH1D("mtmin"  , "mtmin"  , 25  ,   0, 300);
  TH1D *njets  = new TH1D("njets"  , "njets"  , 6  ,    0, 6  );
  TH1D *nbtags = new TH1D("nbtags" , "nbtags" , 4  ,    0, 4  );
  TH1D *pt     = new TH1D("pt"     , "pt"     , 25 ,    0, 300);
  TH1D *eta    = new TH1D("eta"    , "eta"    , 25 , -3.2, 3.2);
  TH1D *jetpt  = new TH1D("jetpt"  , "jetpt"  , 25 ,    0, 300);
  TH1D *pte     = new TH1D("pte"     , "pte"     , 25 ,    0, 300);
  TH1D *ptm     = new TH1D("ptm"     , "ptm"     , 25 ,    0, 300);

  vector <double> counts = {0, 0, 0, 0};
 
  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    //Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      samesign.GetEntry(event);
      nEventsTotal++;
    
      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      //Reject duplicates 
      if (ss::is_real_data()){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)) continue; 
      }


      //Essential Cuts
      if (type == DATA && ss::hyp_class() != 4 && ss::hyp_class() != 6) continue;
      if (type == SIGNAL && ss::hyp_class() != 3) continue;
      if (type == FAKES && ss::hyp_class() != 2) continue;
      if(type!=FAKES) {
          if (!ss::lep1_passes_id()) continue;
          if (!ss::lep2_passes_id()) continue;
      }
      if (doMoriond) {
          if (!ss::fired_trigger())      continue;
          if (!ss::passes_met_filters()) continue;
      } else {
          if (!ss::fired_trigger() && ss::is_real_data())      continue;
          if (!ss::passes_met_filters() && ss::is_real_data()) continue;
      }
      

      bool checkICHEP = false;
      if (checkICHEP) {
          if (ss::run() > 276811) continue;
      } else {
          //electron FO is tighter for iso triggers, make sure it is passed
          if (ss::ht()<300.) {
              if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA(), ss::lep1_p4().pt())) continue;
              if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA(), ss::lep2_p4().pt())) continue;
          } 
      }

      //Lepton pT cuts
      if (ss::lep1_coneCorrPt() < 25) continue;
      if (ss::lep2_coneCorrPt() < 25) continue;

      if ((pickCharge != 0) && (pickCharge*ss::lep1_id() > 0)) continue;

      if (doE && abs(ss::lep1_id()*ss::lep2_id()) != 121) continue;
      if (doMu && abs(ss::lep1_id()*ss::lep2_id()) != 169) continue;

      
      // recalculate mtmin
      float mtl1 = MT(ss::lep1_coneCorrPt(), ss::lep1_p4().phi(), ss::met(), ss::metPhi());
      float mtl2 = MT(ss::lep2_coneCorrPt(), ss::lep2_p4().phi(), ss::met(), ss::metPhi());
      float mtminRecalc = mtl1 > mtl2 ? mtl2 : mtl1;

      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt(), useNewBaseline, doRelax);
      if (BR < 0) continue;
      int SR = signalRegion2016(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtminRecalc, ss::lep1_id(), ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt());
      if (checkTailRegions && ((BR<0) || (SR<30) || ss::hyp_class()==6)) continue; // only look at tail regions for excess study
      if (checkTailRegions && (abs(ss::dilep_p4().M()-91.2) <= 15)) continue; // only look at tail regions for excess study

      //Calculate weight
      float weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;
      if (doMoriond) weight *= getTruePUw_Moriond(ss::trueNumInt()[0]);
      else weight *= getTruePUw(ss::trueNumInt()[0]);

      weight *= ss::is_real_data() ? 1 : (ss::sparms().size() == 0 ? 
          eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht()) :
          eventScaleFactorFastSim(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht(), ss::nGoodVertices())); 

      if (doBtagSF) weight *= ss::is_real_data() ? 1 : ss::weight_btagsf();


      //Make met plots here
      met->Fill(ss::met(), weight);  
      
      njets->Fill(ss::njets(), weight);  

      //Make all other plots
      ht->Fill(ss::ht(), weight);  
      nbtags->Fill(ss::nbtags(), weight);  
      for (unsigned int i = 0; i < ss::jet_pt().size(); i++) jetpt->Fill(ss::jet_pt().at(i), weight);

      //Full cuts
      mll->Fill(ss::dilep_p4().M() , weight);
      mtmin->Fill(ss::mtmin()      , weight);
      pt->Fill(ss::lep1_p4().pt()  , weight);
      pt->Fill(ss::lep2_p4().pt()  , weight);
      abs(ss::lep1_id()) == 11 ? pte->Fill(ss::lep1_p4().pt()  , weight) : ptm->Fill(ss::lep1_p4().pt()  , weight);
      abs(ss::lep2_id()) == 11 ? pte->Fill(ss::lep2_p4().pt()  , weight) : ptm->Fill(ss::lep2_p4().pt()  , weight);
      eta->Fill(ss::lep1_p4().eta(), weight);
      eta->Fill(ss::lep2_p4().eta(), weight);

  
      //Counts
      // if (ss::dilep_p4().M() < 30) continue; 
      if (ss::hyp_type() == 0)      counts[2] += weight; 
      else if (ss::hyp_type() == 3) counts[0] += weight; 
      else                          counts[1] += weight;
                                    counts[3] += weight; 

    }//event loop
  }//file loop

  vector <TH1D*> results;
  results.push_back(met);
  results.push_back(ht);
  results.push_back(mll);
  results.push_back(mtmin);
  results.push_back(njets);
  results.push_back(nbtags);
  results.push_back(pt);
  results.push_back(eta);
  results.push_back(jetpt);
  results.push_back(pte);
  results.push_back(ptm);


  return make_pair(results, counts);

}

void drawPlot(int which, string title, string subtitle, string xaxis, string title2, string options){
  
  //Vector of backgrounds
  vector <TH1D*> backgrounds;
  backgrounds.push_back(ttx_plots.first.at(which));
  backgrounds.push_back(wz_plots.first.at(which));    
  backgrounds.push_back(ttbar_plots.first.at(which)); 
  backgrounds.push_back(dy_plots.first.at(which)); 
  backgrounds.push_back(wjets_plots.first.at(which)); 
  backgrounds.push_back(mb_plots.first.at(which)); 
  backgrounds.push_back(qqww_plots.first.at(which)); 
  // backgrounds.push_back(fake_plots.first.at(which)); 


  //Vector of background titles
  vector <string> titles;         vector <Color_t> colors;
  titles.push_back("ttX");        colors.push_back(kGreen+3); 
  titles.push_back("WZ");         colors.push_back(kOrange); 
  titles.push_back("ttbar");      colors.push_back(18); 
  titles.push_back("DY");         colors.push_back(kGray+2); 
  titles.push_back("W+Jets");     colors.push_back(16); 
  titles.push_back("MultiBoson"); colors.push_back(kMagenta-7); 
  titles.push_back("qqWW");       colors.push_back(kOrange-3); 

  //Null plot
  TH1D* null = new TH1D("","",1,0,1);


  TString folder = "pdfs";
    if (type == DATA) {
        folder  = "pdfs_os";
    } else if (type == FAKES) {
        folder  = "pdfs_tl";
    }

  //Make plots -- data
  if (type == DATA || type == FAKES) dataMCplotMaker(data_plots.first.at(which), backgrounds, titles, title, subtitle, Form("--dontShowZeroRatios --outputName %s/%s --xAxisLabel %s --lumi %f --outOfFrame %s --makeRootFile --makeTable ", folder.Data(), title2.c_str(), xaxis.c_str(), lumiAG, options.c_str()), {}, {}, colors); 
  if (type == SIGNAL) dataMCplotMaker(null, backgrounds, titles, title, subtitle, Form("--dontShowZeroRatios --outputName %s/%s --xAxisLabel %s --lumi %f --outOfFrame %s --legendRight -0.03 --blackSignals --makeRootFile --makeTable ",  folder.Data(),title2.c_str(), xaxis.c_str(), lumiAG, options.c_str()), {}, {}, colors); 
  
}

//Make plot cumulative function
TH1D* makePlotCumulative(TH1D* hist_bkgd){
  float integ = hist_bkgd->Integral(0,hist_bkgd->GetNbinsX()+1);
  float integ_no_overflow = hist_bkgd->Integral(1,hist_bkgd->GetNbinsX());
  Double_t *integral = hist_bkgd->GetIntegral(); 
  hist_bkgd->SetContent(integral);
  for (int i = 1; i <= hist_bkgd->GetNbinsX(); i++){
    float bin_content = hist_bkgd->GetBinContent(i);
    hist_bkgd->SetBinContent(i, integ-integ_no_overflow*bin_content);
  }
  hist_bkgd->SetBinContent(hist_bkgd->GetNbinsX()+1, 0); 
  return hist_bkgd;
}
