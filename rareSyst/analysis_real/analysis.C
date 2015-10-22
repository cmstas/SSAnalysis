#include <iostream> 
#include "TH1F.h"
#include "THStack.h"
#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "SS.h"
#include "../../CORE/Tools/utils.h"
#include "../../CORE/SSSelections.h"
#include "../../software/tableMaker/CTable.h"

using namespace std;

//Structs
struct yield_t { float value; float stat; float scale_up; float scale_dn; float alphas_up; float alphas_dn; float pdf; }; 
struct sr_t    { yield_t yield; yield_t eff; }; 
struct result_t { yield_t c_s; sr_t sr[3][34]; }; 

yield_t high_HH; //high means high HT
yield_t low_HH; //low means high HT

//V. IMPT!!  The nEvents needs to be the number of events IN THE LHE FILE (not the GEN-SIM/MINIAOD/CMS3).  This converts the multiplicative weights to an additive weight.  
result_t run(TChain* chain, int nEvents, string name){

  //Constants
  float lumi = 10.0;
  float kfact = 1.0; //k-factor; will adjust resulting cross-section
  float scale1fb = 1000.0/nEvents; 

  //Histograms for cross-section calculation
  TH1F* cs = new TH1F("cs","cs", 1, 0, 1);
  cs->Sumw2();

  //Histograms for cross-section scale errors
  float cs_scale_up = 0;
  float cs_scale_dn = 0;

  //Histograms for total number of events
  TH1F *total = new TH1F("total", "total", 1, 0, 1);
  total->Sumw2();
 
  //Floats for total nEvents scale errors
  float nTotal_scale_up = 0;
  float nTotal_scale_dn = 0;

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //PDF floats for nTotal
  float cs_pdf[102] = { 0 }; 
  float nTotalPDF[102] = { 0 }; 

  //SR Histos
  TH1F* SR[3][32];
  for (unsigned int j = 0; j < 3; j++) { 
    for (unsigned int i = 0; i < 32; i++){
      SR[j][i] = new TH1F(Form("histo%i_%i", j, i), "hist", 1, 0, 1); 
      SR[j][i]->Sumw2(); 
    }
  }

  //High HH SRs
  float high_HH_value    = 0;
  float high_HH_scale_up = 0; 
  float high_HH_scale_dn = 0; 
  float  low_HH_value    = 0;
  float  low_HH_scale_up = 0; 
  float  low_HH_scale_dn = 0; 

  //Scale Errors for SR
  float SR_scale_up[3][32];
  float SR_scale_dn[3][32];
 
  //PDF errors for SR
  float sr_pdf[3][32][102];
  for (int n = 0; n < 3; n++) { for (int i = 0; i < 32; i++){ for (int j = 0; j < 102; j++) sr_pdf[n][i][j] = 0; } } 

  //Initialize arrays
  for (int j = 0; j < 3; j++) { for (int i = 0; i < 32; i++) SR_scale_up[j][i] = 0; }; 
  for (int j = 0; j < 3; j++) { for (int i = 0; i < 32; i++) SR_scale_dn[j][i] = 0; }; 

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

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
      SS::progress(nEventsTotal, nEventsChain);

      //Fill c-s histo
      cs->Fill(0.5, ss::genweights().at(0));  
 
      //Fill c-s scale error floats
      cs_scale_up += ss::genweights().at(4);
      cs_scale_dn += ss::genweights().at(8);
  
      //Get c-s PDF errors
      for (int i = 0; i < 102; i++) cs_pdf[i] += ss::genweights().at(i+9);  

      //Fill total nEvents histos
      total->Fill(0.5, ss::genweights().at(0)*scale1fb*lumi);
      nTotal_scale_up += ss::genweights().at(4)*scale1fb*lumi;
      nTotal_scale_dn += ss::genweights().at(8)*scale1fb*lumi;
      for (int i = 0; i < 102; i++) nTotalPDF[i] += ss::genweights().at(i+9)*scale1fb*lumi;  

      //Require SS
      if (ss::hyp_class() != 3) continue;
    
      //Determine SR for the event
      int SR_ = signalRegion(ss::njets_corr(), ss::nbtags_corr(), ss::corrMET(), ss::ht_corr(), ss::mtmin(), ss::lep1_p4().pt(), ss::lep2_p4().pt());
      if (SR_ <= 0) continue;
      anal_type_t hyp_type = analysisCategory(ss::lep1_p4().pt(), ss::lep2_p4().pt());

      //Fill SR histograms -- yields
      if (hyp_type == HighHigh) SR[0][SR_-1]->Fill(0.5, ss::genweights().at(0)*scale1fb*lumi);
      if (hyp_type == HighLow)  SR[1][SR_-1]->Fill(0.5, ss::genweights().at(0)*scale1fb*lumi);
      if (hyp_type == LowLow)   SR[2][SR_-1]->Fill(0.5, ss::genweights().at(0)*scale1fb*lumi);

      //Determine SR -- scale errors
      if (hyp_type == HighHigh) SR_scale_up[0][SR_-1] += ss::genweights().at(4)*scale1fb*lumi;
      if (hyp_type == HighLow)  SR_scale_up[1][SR_-1] += ss::genweights().at(4)*scale1fb*lumi;
      if (hyp_type == LowLow)   SR_scale_up[2][SR_-1] += ss::genweights().at(4)*scale1fb*lumi;
      if (hyp_type == HighHigh) SR_scale_dn[0][SR_-1] += ss::genweights().at(8)*scale1fb*lumi;
      if (hyp_type == HighLow)  SR_scale_dn[1][SR_-1] += ss::genweights().at(8)*scale1fb*lumi;
      if (hyp_type == LowLow)   SR_scale_dn[2][SR_-1] += ss::genweights().at(8)*scale1fb*lumi;

      //Determine SR -- PDF errors
      if (hyp_type == HighHigh) for (int i = 0; i < 102; i++) sr_pdf[0][SR_-1][i] += ss::genweights().at(i+9)*scale1fb*lumi;  
      if (hyp_type == HighLow)  for (int i = 0; i < 102; i++) sr_pdf[1][SR_-1][i] += ss::genweights().at(i+9)*scale1fb*lumi;  
      if (hyp_type == LowLow)   for (int i = 0; i < 102; i++) sr_pdf[2][SR_-1][i] += ss::genweights().at(i+9)*scale1fb*lumi;  

      //Also fill high_HH
      if (hyp_type == HighHigh && ss::ht() >= 300) high_HH_value    += ss::genweights().at(0)*scale1fb*lumi;
      if (hyp_type == HighHigh && ss::ht() >= 300) high_HH_scale_up += ss::genweights().at(4)*scale1fb*lumi;
      if (hyp_type == HighHigh && ss::ht() >= 300) high_HH_scale_dn += ss::genweights().at(8)*scale1fb*lumi;
      if (hyp_type == HighHigh && ss::ht() <  300) low_HH_value     += ss::genweights().at(0)*scale1fb*lumi;
      if (hyp_type == HighHigh && ss::ht() <  300) low_HH_scale_up  += ss::genweights().at(4)*scale1fb*lumi;
      if (hyp_type == HighHigh && ss::ht() <  300) low_HH_scale_dn  += ss::genweights().at(8)*scale1fb*lumi;

    }//event loop
  }//file loop

  //Calculate c-s PDF errors
  TH1F* cs_pdf_hist = new TH1F("cs_pdf_hist", "cs_pdf_hist", 10000, 0, 1000); 
  for (int i = 0; i < 100; i++){
    cs_pdf_hist->Fill( 1000*cs_pdf[i]/cs->GetEntries() ); 
  }
  float cs_pdf_error = cs_pdf_hist->GetRMS(); 

  //Declare structs
  result_t result;

  //Cross-section results
  result.c_s.value     =  1000*cs->Integral()/cs->GetEntries();
  result.c_s.stat      =  100*cs->GetBinError(1)/cs->Integral(); 
  result.c_s.scale_up  =  100*(cs_scale_dn - cs->Integral())/cs->Integral();
  result.c_s.scale_dn  = -100*(cs_scale_up - cs->Integral())/cs->Integral();
  result.c_s.pdf       =  100*cs_pdf_error/result.c_s.value;
  result.c_s.alphas_up = -100*(cs_pdf[100] - cs->Integral())/cs->Integral();
  result.c_s.alphas_dn =  100*(cs_pdf[101] - cs->Integral())/cs->Integral();

  //Print results 
  cout << "Cross-Section (" << name << "): " << result.c_s.value << endl;
  cout << "  --> Stat err (%): " << result.c_s.stat     << endl;
  cout << "  --> Scale up (%): " << result.c_s.scale_up << endl;
  cout << "  --> Scale dn (%): " << result.c_s.scale_dn << endl;
  cout << "  --> PDF up (%):   " << result.c_s.pdf   << endl;

  //Delete c-s histos
  delete cs; 

  //Calculate nTotal with statistical error
  float nTotal      = total->Integral();
  float nTotal_stat = total->GetBinError(1)/nTotal;

  //Calculate c-s PDF errors
  TH1F* ntotal_pdf_hist = new TH1F("ntotal_pdf_hist", "ntotal_pdf_hist", 10000, 0, 1000); 
  for (int i = 0; i < 100; i++){
    ntotal_pdf_hist->Fill( nTotalPDF[i] ); 
  }
  float ntotal_pdf_error = ntotal_pdf_hist->GetRMS(); 

  //Print results
  cout << " " << endl;
  cout << "nTotal (" << name << "): " << nTotal << endl;
  cout << "  --> Stat err (%): " <<  100*nTotal_stat                       << endl;
  cout << "  --> Scale up (%): " <<  100*(nTotal_scale_dn - nTotal)/nTotal << endl;
  cout << "  --> Scale dn (%): " << -100*(nTotal_scale_up - nTotal)/nTotal << endl;
  cout << "  --> PDF up (%):   " <<  100*sqrt(ntotal_pdf_error)/nTotal        << endl;
  cout << "  --> PDF dn (%):   " <<  100*sqrt(ntotal_pdf_error)/nTotal        << endl;

  //Calculate PDF errors -- SR
  TH1F* SR_pdf_hist[3][32]; 
  for (unsigned int k = 0; k < 3; k++){
    for (unsigned int j = 0; j < 32; j++){
      SR_pdf_hist[k][j] = new TH1F(Form("sr_pdf_hist%i%i", k, j), Form("sr_pdf_hist%i%i", k, j), 10000, 0, 1000); 
      for (int i = 0; i < 100; i++){
        SR_pdf_hist[k][j]->Fill( sr_pdf[k][j][i] ); 
      }
    }
  }
  float SR_pdf_error[3][32] = { { 0 } };
  for (unsigned int k = 0; k < 3; k++){ for (unsigned int j = 0; j < 32; j++) SR_pdf_error[k][j] = SR_pdf_hist[k][j]->GetRMS();  }

  //Print High HH scale variations:
  cout << "High HT (HH) scale variations up: " << -100*(high_HH_scale_up - high_HH_value)/high_HH_value << endl;
  cout << "High HT (HH) scale variations dn: " << 100*(high_HH_scale_dn - high_HH_value)/high_HH_value << endl;
  cout << "Low  HT (HH) scale variations up: " << -100*(low_HH_scale_up - low_HH_value)/low_HH_value << endl;
  cout << "Low  HT (HH) scale variations dn: " << 100*(low_HH_scale_dn - low_HH_value)/low_HH_value << endl;

  //Print SR results
  for (unsigned int n = 0; n < 3; n++){
    for (unsigned int i = 0; i < (n == 0 ? 32 : (n == 1 ? 26 : 8)); i++){
      float scale_up = SR_scale_up[n][i];
      float scale_dn = SR_scale_dn[n][i];
      float value = SR[n][i]->Integral();
      float stat = SR[n][i]->GetBinError(1);
      float eff   = float(value)/float(nTotal);
      float eff_stat = sqrt(((1-2*eff)*pow(stat, 2) + pow(eff*nTotal_stat, 2)) / pow(nTotal, 2));
      float eff_scale_1 = scale_dn/nTotal_scale_dn - eff;
      float eff_scale_2 = scale_up/nTotal_scale_up - eff;
      float eff_pdf = (value + sqrt(SR_pdf_error[n][i]))/(nTotal + sqrt(ntotal_pdf_error)) - eff;

      //Yields
      result.sr[n][i].yield.value     =  value;
      result.sr[n][i].yield.stat      =  100*stat/value;
      result.sr[n][i].yield.scale_up  =  100*(SR_scale_dn[n][i] - value)/value;
      result.sr[n][i].yield.scale_dn  = -100*(SR_scale_up[n][i] - value)/value;
      result.sr[n][i].yield.pdf       =  100*SR_pdf_error[n][i]/value;
      result.sr[n][i].yield.alphas_up = -100*(sr_pdf[n][i][100] - value)/value;
      result.sr[n][i].yield.alphas_dn =  100*(sr_pdf[n][i][101] - value)/value;
    
      //Efficiencies
      result.sr[n][i].eff.value    =  eff;
      result.sr[n][i].eff.stat     =  100*eff_stat/eff;
      result.sr[n][i].eff.scale_up =  100*max(eff_scale_1, eff_scale_2)/eff;
      result.sr[n][i].eff.scale_dn = -100*min(eff_scale_1, eff_scale_2)/eff;
      result.sr[n][i].eff.pdf      =  100*eff_pdf/eff;

      //cout << " " << endl;
      //cout << name << " SR " << i+1 << ": "       << Form("% 9.5f",  result.sr[n][i].yield.value   ) << endl;
      //cout << "  --> Stat err (%):     " << Form("% 9.5f",  result.sr[n][i].yield.stat    ) << endl;
      //cout << "  --> Scale up (%):     " << Form("% 9.5f",  result.sr[n][i].yield.scale_up) << endl;
      //cout << "  --> Scale dn (%):     " << Form("% 9.5f",  result.sr[n][i].yield.scale_dn) << endl;
      //cout << "  --> PDF up (%):       " << Form("% 9.5f",  result.sr[n][i].yield.pdf     ) << endl;
      //cout << "  --> Efficiency:       " << Form("% 9.5f",  result.sr[n][i].eff.value     ) << endl;
      //cout << "  --> Eff stat (%):     " << Form("% 9.5f",  result.sr[n][i].eff.stat      ) << endl;
      //cout << "  --> Eff scale up (%): " << Form("% 9.5f",  result.sr[n][i].eff.scale_up  ) << endl;
      //cout << "  --> Eff scale dn (%): " << Form("% 9.5f",  result.sr[n][i].eff.scale_dn  ) << endl;
      //cout << "  --> Eff PDF up (%):   " << Form("% 9.5f",  result.sr[n][i].eff.pdf       ) << endl;

    }//SR loop
  }

  return result;

}

void makeTables(result_t fixed, string name){

  //Make Tables
  CTable table1, table2, table3, table4, table5, table6, table7, table8, table9;
  vector <CTable> tables = { table1, table2, table3, table4, table5, table6, table7, table8, table9 }; 

  //Format Tables
  for (unsigned int i = 0; i < tables.size(); i++){
    tables[i].setPrecision(2); 
    tables[i].setTitle(Form("%s: Predicted %s -- %s", name.c_str(), i < 3 ? "Yields" : "Efficiencies", i%3 == 0 ? "HH" : (i%3==1 ? "HL" : "LL")));
    tables[i].useTitle(); 
    if (i < 6) tables[i].printHLine(1);
    tables[i].setColLine(3); 
    tables[i].setColLine(4); 
    tables[i].setColLine(0); 
    tables[i].setColLine(7); 
    tables[i].setColLine(8); 
    if (i >= 6) tables[i].setColLine(9); 
  }

  //Fill yields tables
  for (int j = 0; j < 3; j++){
    tables[j].setTable() ("yield", "stat", "scale up/dn", "pdf up/dn", "alphaS", "total")
                         ("c-s", fixed.c_s.value, fixed.c_s.stat, Form("%.2f/%.2f", fixed.c_s.scale_up, fixed.c_s.scale_dn), Form("%.2f", fixed.c_s.pdf), Form("%.2f/%.2f", fixed.c_s.alphas_up, fixed.c_s.alphas_dn), sqrt( pow(fixed.c_s.pdf, 2) + pow(max(fixed.c_s.scale_up, fixed.c_s.scale_dn), 2) + pow(max(fixed.c_s.alphas_up, fixed.c_s.alphas_dn), 2)));
    for (int i = 0; i < (j == 0 ? 32 : (j == 1 ? 26 : 8)); i++){
      int row = i+1; 
      tables[j].setRowLabel(Form("%s SR%i", (j == 0 ? "HH" : (j == 1 ? "HL" : "LL")), i+1), row); 
      tables[j].setCell(fixed.sr[j][i].yield.value, row, 0); 
      tables[j].setCell(fixed.sr[j][i].yield.stat, row, 1); 
      tables[j].setCell(Form("%.2f/%.2f", fixed.sr[j][i].yield.scale_up, fixed.sr[j][i].yield.scale_dn), row, 2); 
      tables[j].setCell(Form("%.2f", fixed.sr[j][i].yield.pdf), row, 3); 
      tables[j].setCell(Form("%.2f/%.2f", fixed.sr[j][i].yield.alphas_up, fixed.sr[j][i].yield.alphas_dn), row, 4); 
      tables[j].setCell(Form("%.2f", sqrt( pow(fixed.sr[j][i].yield.pdf, 2) + pow(max(fixed.sr[j][i].yield.scale_up, fixed.sr[j][i].yield.scale_dn), 2) + pow(max(fixed.sr[j][i].yield.alphas_up, fixed.sr[j][i].yield.alphas_dn), 2))), row, 5); 
    }
    tables[j].print(); 
    tables[j].saveTex(Form("tables/yields_%s_%i.tex", name.c_str(), j));
  }

  //Fill eff tables
  for (int j = 0; j < 3; j++){
    int tablenumber = j + 3;
    tables[tablenumber].setTable() ("", "1000*eff", "stat", "scale up/dn", "pdf up/dn");
    for (int i = 0; i < (j == 0 ? 32 : (j == 1 ? 26 : 8)); i++){
      int row = i+1; 
      tables[tablenumber].setRowLabel(Form("%s SR%i", (j == 0 ? "HH" : (j == 1 ? "HL" : "LL")), i+1), row); 
      tables[tablenumber].setCell(1000*fixed.sr[j][i].eff.value, row, 0); 
      tables[tablenumber].setCell(fixed.sr[j][i].eff.stat, row, 1); 
      tables[tablenumber].setCell(Form("%.2f/%.2f", fixed.sr[j][i].eff.scale_up, fixed.sr[j][i].eff.scale_dn), row, 2); 
      tables[tablenumber].setCell(Form("%.2f", fixed.sr[j][i].eff.pdf), row, 3); 
    }
    tables[tablenumber].saveTex(Form("tables/eff_%s_%i.tex", name.c_str(), j));
  }

}

int analysis(){

  //Declare Chains
  //TChain *ttz_chain = new TChain("t");
  TChain *ttw_chain = new TChain("t");

  //Fill Chains
  //ttz_chain->Add("/nfs-7/userdata/ss2015/ssBabies/v3.07/TTZL.root");
  ttw_chain->Add("/nfs-7/userdata/ss2015/ssBabies/v3.07/TTW.root");

  //Do everything
  //result_t ttz = run(ttz_chain, 400000, "ttz");
  result_t ttw = run(ttw_chain, 425000, "ttw");

  //Make tables
  //makeTables(ttz, "ttz"); 
  makeTables(ttw, "ttw"); 

  return 0;

}
