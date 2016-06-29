#include "/home/users/cgeorge/software/dataMCplotMaker/dataMCplotMaker.h"
#include "/home/users/cgeorge/SSAnalysis/classFiles/v6.02/SS.h"
#include "/home/users/cgeorge/CORE/Tools/dorky/dorky.h"
#include "/home/users/cgeorge/software/tableMaker/CTable.h"
#include "/home/users/cgeorge/SSAnalysis/commonUtils.h"
#include "TChain.h"
#include "TColor.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

//Namespace
using namespace std;

//Lumi
float lumiAG = 3.99;

//Data or Signal
enum type_ag { DATA, SIGNAL, FAKES }; 
type_ag type = DATA; 
bool useSFs = true;

//Global Definitions
pair <vector <TH1F*>, vector <float> > makePlots(TChain *chain);
void drawPlot(int which, string title = "", string subtitle = "", string xaxis = "quantity", string name2 = "blah", string options = "");
pair <vector <TH1F*>, vector <float> > data_plots;
pair <vector <TH1F*>, vector <float> > ttx_plots;
pair <vector <TH1F*>, vector <float> > ttz_plots;
pair <vector <TH1F*>, vector <float> > ttw_plots;
pair <vector <TH1F*>, vector <float> > wz_plots;    
pair <vector <TH1F*>, vector <float> > ttbar_plots; 
// pair <vector <TH1F*>, vector <float> > st_plots; 
pair <vector <TH1F*>, vector <float> > dy_plots;
pair <vector <TH1F*>, vector <float> > wjets_plots;
pair <vector <TH1F*>, vector <float> > mb_plots;
pair <vector <TH1F*>, vector <float> > wzz_plots;
pair <vector <TH1F*>, vector <float> > zz_plots;
pair <vector <TH1F*>, vector <float> > qqww_plots; 
pair <vector <TH1F*>, vector <float> > fake_plots; 

//Main function
void plots(){

  //Declare chains
  TChain *data  = new TChain("t", "data"); data ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/Data*.root");
  TChain *ttz   = new TChain("t"); ttz  ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTZ*.root"         ); 
  TChain *ttx   = new TChain("t"); ttx  ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTW.root"          ); 
  TChain *ttw   = new TChain("t"); ttw  ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTHtoNonBB.root"   ); 
  TChain *wz    = new TChain("t"); wz   ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WZ.root"           ); 
  TChain *ttbar = new TChain("t"); ttbar->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/TTBAR_PH.root"     ); 
  TChain *dy    = new TChain("t"); dy   ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/DY_high.root"      ); 
                                   dy   ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/DY_low.root"       ); 
  TChain *wjets = new TChain("t"); wjets->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WJets.root"        ); 
  TChain *mb    = new TChain("t"); mb   ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WWZ.root"          ); 
  TChain *wzz   = new TChain("t"); wzz  ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/WZZ.root"          ); 
  TChain *zz    = new TChain("t"); zz   ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/ZZ.root"           ); 
  TChain *qqww  = new TChain("t"); qqww ->Add("/nfs-7/userdata/ss2015/ssBabies/v8.02/QQWW.root"         ); 

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
  std::cout << __LINE__ << std::endl;
  for (unsigned int i = 0; i < mb_plots.first.size(); i++) mb_plots.first[i]->Add(zz_plots.first[i]); 
  for (unsigned int i = 0; i < mb_plots.first.size(); i++) mb_plots.first[i]->Add(wzz_plots.first[i]); 
  std::cout << __LINE__ << std::endl;
  wjets_plots = makePlots(wjets);
  qqww_plots  = makePlots(qqww);

  string sig = (type == DATA) ? "OS" : "SS"; 
  if(type == FAKES) sig = "tight-loose";

  std::cout << __LINE__ << std::endl;

  //Draw plots
  drawPlot(0 , Form("MET in %s dilepton pairs"        , sig.c_str()) , "#geq 2 jets"                         , "MET"        , "MET"         , "--yTitleOffset -0.10"                                 ); 
  std::cout << __LINE__ << std::endl;
  drawPlot(1 , Form("HT in %s dilepton pairs"         , sig.c_str()) , "(MET > 30 OR HT > 500), #geq 2 jets" , "HT"         , "HT"          , "--yTitleOffset -0.10"                                 );
  std::cout << __LINE__ << std::endl;
  drawPlot(2 , Form("M_{ll} in %s dilepton pairs"     , sig.c_str()) , "(MET > 30 OR HT > 500), #geq 2 jets" , "M_{ll}"     , "MLL"         , "--yTitleOffset -0.10"                                 );
  std::cout << __LINE__ << std::endl;
  drawPlot(3 , Form("M_{T}^{min} in %s dilepton pairs", sig.c_str()) , "(MET > 30 OR HT > 500), #geq 2 jets" , "M_{T}^{min}", "MTMIN"       , "--yTitleOffset -0.10"                                 );
  std::cout << __LINE__ << std::endl;
  drawPlot(4 , Form("N_{jets} in %s dilepton pairs"   , sig.c_str()) , "(MET > 30 OR HT > 500)"              , "N_{jets}"   , "NJETS"       , "--nDivisions 6 --noDivisionLabel --yTitleOffset -0.10");
  std::cout << __LINE__ << std::endl;
  drawPlot(5 , Form("N_{b-tags} in %s dilepton pairs" , sig.c_str()) , "(MET > 30 OR HT > 500), #geq 2 jets" , "N_{b-tags}" , "NBTAGS"      , "--nDivisions 4 --noDivisionLabel --yTitleOffset -0.10");
  std::cout << __LINE__ << std::endl;
  drawPlot(6 , Form("lep p_{T} in %s dilepton pairs"  , sig.c_str()) , "(MET > 30 OR HT > 500), #geq 2 jets" , "p_{T}"      , "PT"          , "--yTitleOffset -0.10");
  std::cout << __LINE__ << std::endl;
  drawPlot(7 , Form("#eta in %s dilepton pairs"       , sig.c_str()) , "(MET > 30 OR HT > 500), #geq 2 jets" , "#eta"       , "ETA"         , "--yTitleOffset -0.10 --setMaximum 10000000 --setMinimum 1");
  std::cout << __LINE__ << std::endl;
  drawPlot(8 , Form("jet p_{T} in %s dilepton pairs"  , sig.c_str()) , "(MET > 30 OR HT > 500), #geq 2 jets" , "p_{T}"      , "JETPT"       , "--yTitleOffset -0.10");
  std::cout << __LINE__ << std::endl;

  //Print yields
  vector <float> total = {0,0,0,0}; 
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
  std::cout << __LINE__ << std::endl;

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
pair <vector <TH1F*>, vector <float> > makePlots(TChain *chain){

  //Declare plots
  TH1F *met    = new TH1F("met"    , "met"    , 50  ,   0, 500);
  TH1F *ht     = new TH1F("ht"     , "ht"     , 50  ,   0, 800);
  TH1F *mll    = new TH1F("mll"    , "mll"    , 50  ,   0, 300);
  TH1F *mtmin  = new TH1F("mtmin"  , "mtmin"  , 50  ,   0, 300);
  TH1F *njets  = new TH1F("njets"  , "njets"  , 6  ,    0, 6  );
  TH1F *nbtags = new TH1F("nbtags" , "nbtags" , 4  ,    0, 4  );
  TH1F *pt     = new TH1F("pt"     , "pt"     , 50 ,    0, 300);
  TH1F *eta    = new TH1F("eta"    , "eta"    , 50 , -3.2, 3.2);
  TH1F *jetpt  = new TH1F("jetpt"  , "jetpt"  , 50 ,    0, 300);

  vector <float> counts = {0, 0, 0, 0};
 
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
      if (!ss::fired_trigger() && ss::is_real_data())      continue;
      if (!ss::passedFilterList() && ss::is_real_data())   continue;
      if (!ss::passes_met_filters() && ss::is_real_data()) continue;

      //Lepton pT cuts
      if (ss::lep1_p4().pt() < 25) continue;
      if (ss::lep2_p4().pt() < 25) continue;

      //Calculate weight
      float weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;//*getTruePUw(ss::trueNumInt()[0])*ss::weight_btagsf();
      weight *= ss::is_real_data() ? 1 : (ss::sparms().size() == 0 ? eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht()) : eventScaleFactorFastSim(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht(), ss::nGoodVertices())); 

      //Make met plots here
      if (ss::njets() >= 2) met->Fill(ss::met(), weight);  
      
      if (ss::met() > 30 || ss::ht() > 500) njets->Fill(ss::njets(), weight);  

      if (ss::njets() < 2) continue;

      //Make all other plots
      if ((ss::met() > 30 || ss::ht() > 500) ) ht->Fill(ss::ht(), weight);  
      if ((ss::met() > 30 || ss::ht() > 500) ) nbtags->Fill(ss::nbtags(), weight);  
      if ((ss::met() > 30 || ss::ht() > 500) ) for (unsigned int i = 0; i < ss::jet_pt().size(); i++) jetpt->Fill(ss::jet_pt().at(i), weight);

      //Full cuts
      if (ss::met() < 30 && ss::ht() < 500) continue;
      mll->Fill(ss::dilep_p4().M() , weight);
      mtmin->Fill(ss::mtmin()      , weight);
      pt->Fill(ss::lep1_p4().pt()  , weight);
      pt->Fill(ss::lep2_p4().pt()  , weight);
      eta->Fill(ss::lep1_p4().eta(), weight);
      eta->Fill(ss::lep2_p4().eta(), weight);
  
      //Counts
      if (ss::dilep_p4().M() < 30) continue; 
      if (ss::hyp_type() == 0)      counts[2] += weight; 
      else if (ss::hyp_type() == 3) counts[0] += weight; 
      else                          counts[1] += weight;
                                    counts[3] += weight; 

    }//event loop
  }//file loop

  vector <TH1F*> results;
  results.push_back(met);
  results.push_back(ht);
  results.push_back(mll);
  results.push_back(mtmin);
  results.push_back(njets);
  results.push_back(nbtags);
  results.push_back(pt);
  results.push_back(eta);
  results.push_back(jetpt);

  return make_pair(results, counts);

}

void drawPlot(int which, string title, string subtitle, string xaxis, string title2, string options){
  
  //Vector of backgrounds
  vector <TH1F*> backgrounds;
  std::cout << __LINE__ << " " << which << std::endl;
  backgrounds.push_back(ttx_plots.first.at(which));
  std::cout << __LINE__ << std::endl;
  backgrounds.push_back(wz_plots.first.at(which));    
  std::cout << __LINE__ << std::endl;
  backgrounds.push_back(ttbar_plots.first.at(which)); 
  std::cout << __LINE__ << std::endl;
  backgrounds.push_back(dy_plots.first.at(which)); 
  std::cout << __LINE__ << std::endl;
  backgrounds.push_back(wjets_plots.first.at(which)); 
  std::cout << __LINE__ << std::endl;
  backgrounds.push_back(mb_plots.first.at(which)); 
  std::cout << __LINE__ << std::endl;
  backgrounds.push_back(qqww_plots.first.at(which)); 
  std::cout << __LINE__ << std::endl;
  // backgrounds.push_back(fake_plots.first.at(which)); 

  std::cout << __LINE__ << std::endl;

  //Vector of background titles
  vector <string> titles;         vector <Color_t> colors;
  titles.push_back("ttX");        colors.push_back(kGreen+3); 
  titles.push_back("WZ");         colors.push_back(kOrange); 
  titles.push_back("ttbar");      colors.push_back(18); 
  titles.push_back("DY");         colors.push_back(kGray+2); 
  titles.push_back("W+Jets");     colors.push_back(18); 
  titles.push_back("MultiBoson"); colors.push_back(kMagenta-7); 
  titles.push_back("qqWW");       colors.push_back(kOrange-3); 
  std::cout << __LINE__ << std::endl;

  //Null plot
  TH1F* null = new TH1F("","",1,0,1);

  std::cout << __LINE__ << std::endl;
  //Make plots -- data
  if (type == DATA || type == FAKES) dataMCplotMaker(data_plots.first.at(which), backgrounds, titles, title, subtitle, Form("--dontShowZeroRatios --outputName pdfs/%s --xAxisLabel %s --lumi %f --outOfFrame %s", title2.c_str(), xaxis.c_str(), lumiAG, options.c_str()), {}, {}, colors); 
  if (type == SIGNAL) dataMCplotMaker(null, backgrounds, titles, title, subtitle, Form("--dontShowZeroRatios --outputName %s --xAxisLabel %s --lumi %f --outOfFrame %s --legendRight -0.03 --blackSignals", title2.c_str(), xaxis.c_str(), lumiAG, options.c_str()), {}, {}, colors); 
  std::cout << __LINE__ << std::endl;
  
}

//Make plot cumulative function
TH1F* makePlotCumulative(TH1F* hist_bkgd){
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
