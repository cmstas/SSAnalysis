#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "/home/users/cgeorge/software/dataMCplotMaker/dataMCplotMaker.h"

//Regular includes
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1.h"

enum dim_t { D1, D2 };

vector <Color_t> Colors;

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

void makePlots(dim_t dim, vector<string> var, string title, int nbins, int maxbin, bool doPtRel, string options = "", int nbinsy = 70, int maxbiny = 700){

  //Generic possible cuts
  TCut ht200("ht > 200"); 
  TCut met50("met > 50");
  TCut njets2("njets >= 2");
  TCut hh("lep1_p4.pt() >= 25 && lep2_p4.pt() >= 25");
  TCut ll("lep1_p4.pt() >= 10 && lep2_p4.pt() >= 10");
  TCut class3("hyp_class == 3");
  TCut classSS("hyp_class <= 3");

  //Selections
  TCut baseline;
  if (title != "ptrel") baseline += class3;
  if (title == "ptrel") baseline += classSS;
  if (dim == D1 && title == "ht") baseline += met50 + njets2 + hh; 
  else if (dim == D1 && title == "met") baseline += ht200 + njets2 + hh;
  else if (dim == D1 && title == "njets") baseline += met50 + hh;
  else if (dim == D1 && title == "lep_pt") baseline += met50 + ht200 + njets2 + ll;
  else if (dim == D1) baseline += met50 + ht200 + njets2 + hh; 
  TCut weight("scale1fb*10.0");

  //Get files 
  TFile *ttz_file             = new TFile("../v1.08/TTZ_multiIso.root"           ); 
  TFile *ttw_file             = new TFile("../v1.08/TTW_multiIso.root"           ); 
  TFile *ttbar_file           = new TFile("../v1.08/TTBAR_multiIso.root"         ); 
  TFile *wz_file              = new TFile("../v1.08/WZ_multiIso.root"            ); 
  TFile *t1tttt_1200_800_file = new TFile("../v1.08/T1TTTT_1200_multiIso.root"   ); 
  TFile *t1tttt_1500_100_file = new TFile("../v1.08/T1TTTT_1500_multiIso.root"   ); 
  TFile *t5qqqqWW1200_file    = new TFile("../v1.08/private/t5qqqqWW_1200_1000_800_baby_multiIso.root");
  //TFile *t5qqqqWW1500_file    = new TFile(Form("../v1.05/t5qqqqww1500%s_unoff.root"   , ptrel.c_str()));
  //TFile *t6ttWW600_425_file   = new TFile(Form("../v1.05/T6TTWW600_425%s.root"  , ptrel.c_str()));
  //TFile *t6ttWW600_150_file   = new TFile(Form("../v1.05/T6TTWW600_150%s.root"  , ptrel.c_str()));
 
  //Get trees
  TTree *ttz             = (TTree*)ttz_file->Get("t");
  TTree *ttw             = (TTree*)ttw_file->Get("t");
  TTree *ttbar           = (TTree*)ttbar_file->Get("t");
  TTree *wz              = (TTree*)wz_file->Get("t");
  TTree *t1tttt_1200_800 = (TTree*)t1tttt_1200_800_file->Get("t");
  TTree *t1tttt_1500_100 = (TTree*)t1tttt_1500_100_file->Get("t");
  TTree *t5qqqqWW1200    = (TTree*)t5qqqqWW1200_file->Get("t");
  //TTree *t5qqqqWW1500    = (TTree*)t5qqqqWW1500_file->Get("t");
  //TTree *t6ttWW600_425   = (TTree*)t6ttWW600_425_file->Get("t");
  //TTree *t6ttWW600_150   = (TTree*)t6ttWW600_150_file->Get("t");

  if (dim == D1){
    //Declare individual plots
    TH1F *hist_ttz             = new TH1F("hist_ttz"            , "hist_ttz"            , nbins, 0, maxbin);
    TH1F *hist_ttw             = new TH1F("hist_ttw"            , "hist_ttw"            , nbins, 0, maxbin);
    TH1F *hist_ttbar           = new TH1F("hist_ttbar"          , "hist_ttbar"          , nbins, 0, maxbin);
    TH1F *hist_wz              = new TH1F("hist_wz"             , "hist_wz"             , nbins, 0, maxbin);
    TH1F *hist_bkgd            = new TH1F("hist_bkgd"           , "hist_bkgd"           , nbins, 0, maxbin);
    TH1F *hist_t1tttt_1200_800 = new TH1F("hist_t1tttt_1200_800", "hist_t1tttt_1200_800", nbins, 0, maxbin);
    TH1F *hist_t1tttt_1500_100 = new TH1F("hist_t1tttt_1500_100", "hist_t1tttt_1500_100", nbins, 0, maxbin);
    TH1F *hist_t5qqqqWW1200    = new TH1F("hist_t5qqqqWW1200"   , "hist_t5qqqqWW1200"   , nbins, 0, maxbin);
    //TH1F *hist_t5qqqqWW1500    = new TH1F("hist_t5qqqqWW1500"   , "hist_t5qqqqWW1500"   , nbins, 0, maxbin);
    //TH1F *hist_t6ttWW600_425   = new TH1F("hist_t6ttWW600_425"  , "hist_t6ttWW600_425"  , nbins, 0, maxbin);
    //TH1F *hist_t6ttWW600_150   = new TH1F("hist_t6ttWW600_150"  , "hist_t6ttWW600_150"  , nbins, 0, maxbin);

    //Draw simple plots
    for (unsigned int i = 0; i < var.size(); i++){
      ttz->Draw(Form("%s>>hist_ttz", var[i].c_str()), baseline*weight); 
      ttw->Draw(Form("%s>>hist_ttw", var[i].c_str()), baseline*weight); 
      wz->Draw(Form("%s>>hist_wz", var[i].c_str()), baseline*weight); 
      ttbar->Draw(Form("%s>>hist_ttbar", var[i].c_str()), baseline*weight); 
      ttz->Draw(Form("%s>>hist_bkgd", var[i].c_str()), baseline*weight); 
      ttw->Draw(Form("%s>>+hist_bkgd", var[i].c_str()), baseline*weight); 
      wz->Draw(Form("%s>>+hist_bkgd", var[i].c_str()), baseline*weight); 
      ttbar->Draw(Form("%s>>+hist_bkgd", var[i].c_str()), baseline*weight); 
      t1tttt_1200_800->Draw(Form("%s>>hist_t1tttt_1200_800", var[i].c_str()), baseline*weight); 
      t1tttt_1500_100->Draw(Form("%s>>hist_t1tttt_1500_100", var[i].c_str()), baseline*weight); 
      t5qqqqWW1200->Draw(Form("%s>>hist_t5qqqqWW1200", var[i].c_str()), baseline*weight); 
      //t5qqqqWW1500->Draw(Form("%s>>hist_t5qqqqWW1500", var[i].c_str()), baseline*weight); 
      //t6ttWW600_425->Draw(Form("%s>>hist_t6ttWW600_425", var[i].c_str()), baseline*weight); 
      //t6ttWW600_150->Draw(Form("%s>>hist_t6ttWW600_150", var[i].c_str()), baseline*weight); 
    }

    //Make plots cumulative
    //makePlotCumulative(hist_ttz            );
    //makePlotCumulative(hist_ttw            );
    //makePlotCumulative(hist_ttbar          );
    //makePlotCumulative(hist_wz             );
    //makePlotCumulative(hist_bkgd           );
    //makePlotCumulative(hist_t1tttt_1200_800);
    //makePlotCumulative(hist_t1tttt_1500_100);
    //makePlotCumulative(hist_t5qqqqWW1200   );
    //makePlotCumulative(hist_t5qqqqWW1500   );
    //makePlotCumulative(hist_t6ttWW600_425  );
    //makePlotCumulative(hist_t6ttWW600_150  );

    //Make all background + 2 signal plot
    TH1F* null = new TH1F("","",1,0,1);
    vector <TH1F*> null_vector;
    null_vector.push_back(null);
    vector <TH1F*> backgrounds;
    backgrounds.push_back(hist_ttbar);
    backgrounds.push_back(hist_ttz);
    backgrounds.push_back(hist_ttw);
    backgrounds.push_back(hist_wz);
    vector <TH1F*> signals;
    signals.push_back(hist_t5qqqqWW1200); 
    signals.push_back(hist_t1tttt_1200_800); 
    vector <char*> titles;
    titles.push_back("ttbar");
    titles.push_back("wz");
    titles.push_back("ttw");
    titles.push_back("ttz");
    vector <char*> signalTitles;
    signalTitles.push_back("T5qqqqWW (1200, 1000, 800)");
    signalTitles.push_back("T1tttt (1200, 800)");
    dataMCplotMaker(null, backgrounds, titles, "SS baseline H-H", "multiIso", Form("--outputName %s%s.pdf --xAxisLabel %s --nDivisions -108 --energy 13 --lumi 10 --legendTextSize 0.03 --legendRight -0.2 --noXaxisUnit %s --noOverflow --yAxisLabel Entries --isLinear", title.c_str(), "_multiIso", title.c_str(), options.c_str()), signals, signalTitles); 
    
    //Make total background + 6 signals
    vector <TH1F*> backgrounds2;
    backgrounds2.push_back(hist_bkgd);
    backgrounds2.push_back(hist_t1tttt_1500_100);
    backgrounds2.push_back(hist_t1tttt_1200_800);
    //backgrounds2.push_back(hist_t5qqqqWW1500); 
    backgrounds2.push_back(hist_t5qqqqWW1200);
    //backgrounds2.push_back(hist_t6ttWW600_425);
    //backgrounds2.push_back(hist_t6ttWW600_150);
    vector <char*> titles2;
    titles2.push_back("background");
    titles2.push_back("T1tttt (1500, 100)");
    titles2.push_back("T1tttt (1200, 800)");
    //titles2.push_back("T5qqqqWW (1500, 800, 100)");
    titles2.push_back("T5qqqqWW (1200, 1000, 800)");
    //titles2.push_back("T6ttWW (600, 425)");
    //titles2.push_back("T6ttWW (600, 150)");
    vector <char*> blank;
    vector <Color_t> colors;
    colors.push_back(kBlack);
    colors.push_back(kRed);
    colors.push_back(kBlue);
    colors.push_back(kGreen);
    colors.push_back(kCyan);
    colors.push_back(kMagenta);
    colors.push_back(kGray+2);
    dataMCplotMaker(null, backgrounds2, titles2, "SS baseline H-H", doPtRel ? "with pTrel" : "iso only", Form("--outputName %s%s_sig.pdf --xAxisLabel %s cut --energy 13 --lumi 10 --legendTextSize 0.025 --legendRight -0.00 --legendUp 0.05  --noStack %s --noOverflow  --nDivisions -108 --yAxisLabel Entries Passing Cut %s", title.c_str(), "_multiIso", title.c_str(), options.c_str(), title == "njets" ? "--noXaxisUnit" : " "), null_vector, blank, colors);

    //Memory management
    delete hist_ttz;
    delete hist_ttw;
    delete hist_ttbar;
    delete hist_wz;
    delete hist_bkgd;
    delete hist_t1tttt_1200_800;
    delete hist_t1tttt_1500_100;
    delete hist_t5qqqqWW1200;
    //delete hist_t5qqqqWW1500;
    //delete hist_t6ttWW600_425;
    //delete hist_t6ttWW600_150;
  }

  if (dim == D2){
    //Selections
    baseline += hh + njets2;

    //Declare individual plots
    TH2F *hist_bkgd            = new TH2F("hist_bkgd"           , "hist_bkgd"           , nbins, 0, maxbin, nbinsy, 0, maxbiny);
    TH2F *hist_t1tttt_1200_800 = new TH2F("hist_t1tttt_1200_800", "hist_t1tttt_1200_800", nbins, 0, maxbin, nbinsy, 0, maxbiny);
    TH2F *hist_t1tttt_1500_100 = new TH2F("hist_t1tttt_1500_100", "hist_t1tttt_1500_100", nbins, 0, maxbin, nbinsy, 0, maxbiny);
    TH2F *hist_t5qqqqWW1200    = new TH2F("hist_t5qqqqWW1200"   , "hist_t5qqqqWW1200"   , nbins, 0, maxbin, nbinsy, 0, maxbiny);
    //TH2F *hist_t5qqqqWW1500    = new TH2F("hist_t5qqqqWW1500"   , "hist_t5qqqqWW1500"   , nbins, 0, maxbin, nbinsy, 0, maxbiny);
    //TH2F *hist_t6ttWW600_425   = new TH2F("hist_t6ttWW600_425"  , "hist_t6ttWW600_425"  , nbins, 0, maxbin, nbinsy, 0, maxbiny);
    //TH2F *hist_t6ttWW600_150   = new TH2F("hist_t6ttWW600_150"  , "hist_t6ttWW600_150"  , nbins, 0, maxbin, nbinsy, 0, maxbiny);

    //Draw simple plots
    for (unsigned int i = 0; i < var.size(); i++){
      ttz->Draw(Form("%s>>hist_bkgd", var[i].c_str()), baseline*weight); 
      ttw->Draw(Form("%s>>+hist_bkgd", var[i].c_str()), baseline*weight); 
      wz->Draw(Form("%s>>+hist_bkgd", var[i].c_str()), baseline*weight); 
      ttbar->Draw(Form("%s>>+hist_bkgd", var[i].c_str()), baseline*weight); 
      t1tttt_1200_800->Draw(Form("%s>>hist_t1tttt_1200_800", var[i].c_str()), baseline*weight); 
      t1tttt_1500_100->Draw(Form("%s>>hist_t1tttt_1500_100", var[i].c_str()), baseline*weight); 
      t5qqqqWW1200->Draw(Form("%s>>hist_t5qqqqWW1200", var[i].c_str()), baseline*weight); 
      //t5qqqqWW1500->Draw(Form("%s>>hist_t5qqqqWW1500", var[i].c_str()), baseline*weight); 
      //t6ttWW600_425->Draw(Form("%s>>hist_t6ttWW600_425", var[i].c_str()), baseline*weight); 
      //t6ttWW600_150->Draw(Form("%s>>hist_t6ttWW600_150", var[i].c_str()), baseline*weight); 
    }
 
    vector <TH2F*> D2plots;
    D2plots.push_back(hist_bkgd           );
    D2plots.push_back(hist_t1tttt_1200_800);
    D2plots.push_back(hist_t1tttt_1500_100);
    D2plots.push_back(hist_t5qqqqWW1200   );
    //D2plots.push_back(hist_t5qqqqWW1500   );
    //D2plots.push_back(hist_t6ttWW600_425  );
    //D2plots.push_back(hist_t6ttWW600_150  );

    for (unsigned int i = 0; i < D2plots.size(); i++){
      TCanvas *canvas = new TCanvas("canvas"); 
      gStyle->SetOptStat(0);
      D2plots[i]->Draw("BOX");
      canvas->Print(Form("%s_%s_2d_multiIso.pdf", title.c_str(), D2plots[i]->GetTitle()));
      delete canvas;
    }

    //Memory management
    delete hist_bkgd;
    delete hist_t1tttt_1200_800;
    delete hist_t1tttt_1500_100;
    delete hist_t5qqqqWW1200;
    //delete hist_t5qqqqWW1500;
    //delete hist_t6ttWW600_425;
    //delete hist_t6ttWW600_150;

  }

  //Memory management
  delete ttz_file;
  delete ttw_file;
  delete ttbar_file;
  delete wz_file;
  delete t1tttt_1200_800_file;
  delete t1tttt_1500_100_file;
  delete t5qqqqWW1200_file;
  //delete t5qqqqWW1500_file;
  //delete t6ttWW600_425_file;
  //delete t6ttWW600_150_file;
}

int plots(){

  //Colors.push_back(kGreen-3);
  //Colors.push_back(kCyan);
  Colors.push_back(kOrange-4);
  Colors.push_back(kMagenta-8);
  Colors.push_back(kRed);
  Colors.push_back(kBlue);
  Colors.push_back(kGreen+3);
  Colors.push_back(kYellow-7);

  vector <string> var;
  //var.push_back("ht:met");
  //makePlots(D2, var, "ht_met", 70, 700, 0, " ", 90, 900);
  //var.clear(); 
  //var.push_back("nbtags:njets");
  //makePlots(D2, var, "nbtags_njets", 10, 10, 0, " ", 5, 5);
  //var.clear(); 

  //var.push_back("ht:met");
  //makePlots(D2, var, "ht_met", 70, 700, 1, " ", 90, 900);
  //var.clear(); 
  //var.push_back("nbtags:njets");
  //makePlots(D2, var, "nbtags_njets", 5, 5, 1, " ", 10, 10);
  //var.clear(); 

  //Plots without ptrel
  //var.push_back("ht");
  //makePlots(D1, var, "ht"       , 70, 700, 0, "--vLine 200 --vLine 400");
  //var.clear();
  //var.push_back("met");
  //makePlots(D1, var, "met"      , 70, 700, 0, "--vLine 50 --vLine 120");
  //var.clear();
  var.push_back("njets");
  makePlots(D1, var, "njets"    ,  8,   8, 0, "--vLine 2 --vLine 4 --noDivisionLabel");
  var.clear();
  //var.push_back("nbtags");
  //makePlots(D1, var, "nbtags"   ,  6,   6, 0);
  //var.clear();
  //var.push_back("mt > mt_l2 ? mt_l2 : mt");
  //makePlots(D1, var, "min_mt"   , 40, 400, 0);
  //var.clear();
  //var.push_back("lep1_ptrel_v1");
  //var.push_back("lep2_ptrel_v1");
  //makePlots(D1, var, "ptrel_v1" , 80, 400, 0); 
  //var.clear();
  //var.push_back("lep1_p4.pt()");
  //var.push_back("lep2_p4.pt()");
  //makePlots(D1, var, "lep_pt"   , 80, 400, 0, "--vLine 25"); 
  //var.clear();
  //var.push_back("jets.pt()");
  //makePlots(D1, var, "jets_pt"  , 70, 700, 0);
  //var.clear();
  //var.push_back("abs(jets.eta())");
  //makePlots(D1, var, "jets_eta" , 24,   3, 0, "--setMaximum 1000");
  //var.clear();
  //var.push_back("btags.pt()");
  //makePlots(D1, var, "btags_pt" , 50, 500, 0, "--noDivisionLabel");
  //var.clear();
  //var.push_back("abs(btags.eta())");
  //makePlots(D1, var, "btags_eta", 24, 2.4, 0, "--setMaximum 1000");

  //Plots with ptrel
  //var.clear();
  //var.push_back("ht");
  //makePlots(D1, var, "ht"       , 70, 700, 1);
  //var.clear();
  //var.push_back("met");
  //makePlots(D1, var, "met"      , 70, 700, 1);
  //var.clear();
  //var.push_back("njets");
  //makePlots(D1, var, "njets"    ,  8,   8, 1);
  //var.clear();
  //var.push_back("nbtags");
  //makePlots(D1, var, "nbtags"   ,  6,   6, 1);
  //var.clear();
  //var.push_back("mt > mt_l2 ? mt_l2 : mt");
  //makePlots(D1, var, "min_mt"   , 40, 400, 1);
  //var.clear();
  //var.push_back("lep1_ptrel_v1");
  //var.push_back("lep2_ptrel_v1");
  //makePlots(D1, var, "ptrel_v1" , 80, 400, 1); 
  //var.clear();
  //var.push_back("lep1_p4.pt()");
  //var.push_back("lep2_p4.pt()");
  //makePlots(D1, var, "lep_pt"   , 80, 400, 1); 
  //var.clear();
  //var.push_back("jets.pt()");
  //makePlots(D1, var, "jets_pt"  , 70, 700, 1);
  //var.clear();
  //var.push_back("abs(jets.eta())");
  //makePlots(D1, var, "jets_eta" , 24,   3, 1, "--setMaximum 1000");
  //var.clear();
  //var.push_back("btags.pt()");
  //makePlots(D1, var, "btags_pt" , 50, 500, 1);
  //var.clear();
  //var.push_back("abs(btags.eta())");
  //makePlots(D1, var, "btags_eta", 24, 2.4, 1, "--setMaximum 1000");

  return 0;

}
