#include <iostream> 
#include "TH1F.h"
#include "THStack.h"
#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "SYST.h"
#include "../../../Tools/utils.h"
#include "../../../CORE/SSSelections.h"
#include "../../../software/tableMaker/CTable.h"

using namespace std;

//Switches
bool doOld = false;

//Structs
struct yield_t { float value; float stat; float scale_up; float scale_dn; float pdf_up; float pdf_dn; }; 
struct sr_t    { yield_t yield; yield_t eff; }; 
struct result_t { yield_t c_s; sr_t sr[3][34]; }; 

int signalRegionAG(int njets, int nbtags, float met, float ht, float mt_min, float lep1_pt, float lep2_pt){
  if (ht != ht) return -1; //this is to protect against NANs
  if (!doOld) return signalRegion(njets, nbtags, met, ht, mt_min, lep1_pt, lep2_pt);
  else {
    if (met < 50 || njets < 2 || lep1_pt < 20 || lep2_pt < 20 || ht < 200) return 0;
    if (met >  50 && met < 120 && ht >= 200 && ht <= 400 && njets >= 2 && njets < 4) return 1;
    if (met >  50 && met < 120 && ht >  400 &&              njets >= 2 && njets < 4) return 2;
    if (met >  50 && met < 120 && ht >= 200 && ht <= 400 && njets >= 4             ) return 3;
    if (met >  50 && met < 120 && ht >  400 &&              njets >= 4             ) return 4;
    if (met > 120 &&              ht >= 200 && ht <= 400 && njets >= 2 && njets < 4) return 5;
    if (met > 120 &&              ht >  400 &&              njets >= 2 && njets < 4) return 6;
    if (met > 120 &&              ht >= 200 && ht <= 400 && njets >= 4             ) return 7;
    if (met > 120 &&              ht >  400 &&              njets >= 4             ) return 8;
  }
  cout << "WARNING!  No old SR found! Should never get here!" << endl;
  return -1;
}

result_t run(TChain* chain){

  //Constants
  float lumi = 10.0;
  float scale1fb = 1000.0/1000000.0; //weight provided must be provided by nEvents (denom).  Numer is to convert to fb.

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
  float cs_pdf[40] = { 0 }; 
  float nTotalPDF[40] = { 0 }; 

  //SR Histos
  TH1F* SR[3][32];
  for (unsigned int j = 0; j < (doOld ? 1 : 3); j++) { 
    for (unsigned int i = 0; i < (doOld ? 8 : 32); i++){
      SR[j][i] = new TH1F(Form("histo%i_%i", j, i), "hist", 1, 0, 1); 
      SR[j][i]->Sumw2(); 
    }
  }

  //Scale Errors for SR
  float SR_scale_up[3][32];
  float SR_scale_dn[3][32];
 
  //PDF errors for SR
  float sr_pdf[3][32][40];
  for (int n = 0; n < (doOld ? 1 : 3); n++) { for (int i = 0; i < 32; i++){ for (int j = 0; j < 40; j++) sr_pdf[n][i][j] = 0; } } 

  //Initialize arrays
  for (int j = 0; j < 3; j++) { for (int i = 0; i < 32; i++) SR_scale_up[j][i] = 0; }; 
  for (int j = 0; j < 3; j++) { for (int i = 0; i < 32; i++) SR_scale_dn[j][i] = 0; }; 

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("Events");
    systematics.Init(tree);

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      systematics.GetEntry(event);
      nEventsTotal++;
    
      //Progress
      SYST::progress(nEventsTotal, nEventsChain);

      //Fill c-s histo
      cs->Fill(0.5, syst::weight());  
 
      //Fill c-s scale error floats
      cs_scale_up += syst::weights().at(4);
      cs_scale_dn += syst::weights().at(8);
  
      //Get c-s PDF errors
      for (int i = 0; i < 40; i++) cs_pdf[i] += syst::weights().at(i+9);  

      //Fill total nEvents histos
      total->Fill(0.5, syst::weight()*scale1fb*lumi);
      nTotal_scale_up += syst::weights().at(4)*scale1fb*lumi;
      nTotal_scale_dn += syst::weights().at(8)*scale1fb*lumi;
      for (int i = 0; i < 40; i++) nTotalPDF[i] += syst::weights().at(i+9)*scale1fb*lumi;  

      //Reject all events that are not "true SS"
      if (!syst::keep()) continue;
      if (syst::id1()*syst::id2() <= 0) continue;
    
      //Determine SR for the event
      float mt_l1 = MT(syst::lep1().pt(), syst::lep1().phi(), syst::met(), syst::metphi());
      float mt_l2 = MT(syst::lep2().pt(), syst::lep2().phi(), syst::met(), syst::metphi());
      float mt_min = (mt_l1 > mt_l2 ? mt_l2 : mt_l1); 
      int SR_ = signalRegionAG(syst::nJets40(), syst::nBtags(), syst::met(), syst::ht(), mt_min, syst::lep1().pt(), syst::lep2().pt());
      anal_type_t hyp_type = doOld ? HighHigh : analysisCategory(syst::lep1().pt(), syst::lep2().pt());
      if (SR_ <= 0) continue;

      //Fill SR histograms -- yields
      if (hyp_type == HighHigh) SR[0][SR_-1]->Fill(0.5, syst::weight()*scale1fb*lumi);
      if (hyp_type == HighLow)  SR[1][SR_-1]->Fill(0.5, syst::weight()*scale1fb*lumi);
      if (hyp_type == LowLow)   SR[2][SR_-1]->Fill(0.5, syst::weight()*scale1fb*lumi);

      //Determine SR -- scale errors
      if (hyp_type == HighHigh) SR_scale_up[0][SR_-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == HighLow)  SR_scale_up[1][SR_-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == LowLow)   SR_scale_up[2][SR_-1] += syst::weights().at(4)*scale1fb*lumi;
      if (hyp_type == HighHigh) SR_scale_dn[0][SR_-1] += syst::weights().at(8)*scale1fb*lumi;
      if (hyp_type == HighLow)  SR_scale_dn[1][SR_-1] += syst::weights().at(8)*scale1fb*lumi;
      if (hyp_type == LowLow)   SR_scale_dn[2][SR_-1] += syst::weights().at(8)*scale1fb*lumi;

      //Fill nEvents PDF error floats
      if (hyp_type == HighHigh) for (int i = 0; i < 40; i++) sr_pdf[0][SR_-1][i] += syst::weights().at(i+9)*scale1fb*lumi;  
      if (hyp_type == HighLow)  for (int i = 0; i < 40; i++) sr_pdf[1][SR_-1][i] += syst::weights().at(i+9)*scale1fb*lumi;  
      if (hyp_type == LowLow)   for (int i = 0; i < 40; i++) sr_pdf[2][SR_-1][i] += syst::weights().at(i+9)*scale1fb*lumi;  

    }//event loop
  }//file loop

  //Calculate c-s PDF errors
  float cs_pdf_up = 0;
  float cs_pdf_dn = 0;
  for (int i = 1; i <= 20; i++){
    float pdf_up_temp = max(cs_pdf[2*i-2]- cs->Integral(), cs_pdf[2*i-1] - cs->Integral());
    float pdf_dn_temp = max(cs->Integral() - cs_pdf[2*i-2], cs->Integral() - cs_pdf[2*i-1]);
    cs_pdf_up += (pdf_up_temp > 0) ? pow(pdf_up_temp, 2) : 0;
    cs_pdf_dn += (pdf_dn_temp > 0) ? pow(pdf_dn_temp, 2) : 0;
  }

  //Declare structs
  result_t result;

  //Cross-section results
  result.c_s.value    =  1000*cs->Integral()/cs->GetEntries();
  result.c_s.stat     =  100*cs->GetBinError(1)/cs->Integral(); 
  result.c_s.scale_up =  100*(cs_scale_dn - cs->Integral())/cs->Integral();
  result.c_s.scale_dn = -100*(cs_scale_up - cs->Integral())/cs->Integral();
  result.c_s.pdf_up   =  100*sqrt(cs_pdf_up)/cs->Integral();
  result.c_s.pdf_dn   =  100*sqrt(cs_pdf_dn)/cs->Integral();

  //Print results 
  cout << "Cross-Section: "      << result.c_s.value    << endl;
  cout << "  --> Stat err (%): " << result.c_s.stat     << endl;
  cout << "  --> Scale up (%): " << result.c_s.scale_up << endl;
  cout << "  --> Scale dn (%): " << result.c_s.scale_dn << endl;
  cout << "  --> PDF up (%):   " << result.c_s.pdf_up   << endl;
  cout << "  --> PDF dn (%):   " << result.c_s.pdf_dn   << endl;

  //Delete c-s histos
  delete cs; 

  //Calculate nTotal with statistical error
  float nTotal      = total->Integral();
  float nTotal_stat = total->GetBinError(1)/nTotal;

  //Calculate nTotal PDF errors
  float nTotal_pdf_up = 0;
  float nTotal_pdf_dn = 0;
  for (int i = 1; i <= 20; i++){
    float pdf_up_temp = max(nTotalPDF[2*i-2] - nTotal, nTotalPDF[2*i-1] - nTotal);
    float pdf_dn_temp = max(nTotal - nTotalPDF[2*i-2], nTotal - nTotalPDF[2*i-1]);
    nTotal_pdf_up += (pdf_up_temp > 0) ? pow(pdf_up_temp, 2) : 0;
    nTotal_pdf_dn += (pdf_dn_temp > 0) ? pow(pdf_dn_temp, 2) : 0;
  }

  //Print results
  cout << " " << endl;
  cout << "nTotal: " << nTotal << endl;
  cout << "  --> Stat err (%): " <<  100*nTotal_stat                       << endl;
  cout << "  --> Scale up (%): " <<  100*(nTotal_scale_dn - nTotal)/nTotal << endl;
  cout << "  --> Scale dn (%): " << -100*(nTotal_scale_up - nTotal)/nTotal << endl;
  cout << "  --> PDF up (%):   " <<  100*sqrt(nTotal_pdf_up)/nTotal        << endl;
  cout << "  --> PDF dn (%):   " <<  100*sqrt(nTotal_pdf_dn)/nTotal        << endl;

  //Calculate SR PDF errors
  float sr_pdf_up[3][32];
  float sr_pdf_dn[3][32];
  for (unsigned int n = 0; n < (doOld ? 1 : 3); n++){
    for (unsigned int k = 0; k < 32; k++) sr_pdf_up[n][k] = 0; 
    for (unsigned int k = 0; k < 32; k++) sr_pdf_dn[n][k] = 0; 
    for (unsigned int i = 0; i < (doOld ? 8 : (n == 0 ? 32 : (n == 1 ? 26 : 8))); i++){
      float value = SR[n][i]->Integral();
      for (int j = 1; j <= 20; j++){
        float pdf_up_temp = max(sr_pdf[n][i][2*j-2] - value, sr_pdf[n][i][2*j-1] - value);
        float pdf_dn_temp = doOld ? -min(sr_pdf[n][i][2*j-2] - value, sr_pdf[n][i][2*j-1] - value) : max(value - sr_pdf[n][i][2*j-2], value - sr_pdf[n][i][2*j-1]);
        sr_pdf_up[n][i] += (pdf_up_temp > 0) ? pow(pdf_up_temp, 2) : 0;
        sr_pdf_dn[n][i] += (pdf_dn_temp > 0) ? pow(pdf_dn_temp, 2) : 0;
      }
    }
  }

  //Print SR results
  for (unsigned int n = 0; n < (doOld ? 1 : 3); n++){
    for (unsigned int i = 0; i < (doOld ? 8 : (n == 0 ? 32 : (n == 1 ? 26 : 8))); i++){
      float scale_up = SR_scale_up[n][i];
      float scale_dn = SR_scale_dn[n][i];
      float value = SR[n][i]->Integral();
      float stat = SR[n][i]->GetBinError(1);
      float eff   = float(value)/float(nTotal);
      float eff_stat = sqrt(((1-2*eff)*pow(stat, 2) + pow(eff*nTotal_stat, 2)) / pow(nTotal, 2));
      float eff_scale_1 = scale_dn/nTotal_scale_dn - eff;
      float eff_scale_2 = scale_up/nTotal_scale_up - eff;
      float eff_pdf_1 = (value + sqrt(sr_pdf_up[n][i]))/(nTotal + sqrt(nTotal_pdf_up)) - eff;
      float eff_pdf_2 = (value - sqrt(sr_pdf_dn[n][i]))/(nTotal - sqrt(nTotal_pdf_dn)) - eff;
      if (doOld){ //bugs intentionally reproduced
        eff_pdf_1 = (value + sqrt(sr_pdf_up[n][i]))/(nTotal + sqrt(nTotal_pdf_up)) - eff;
        eff_pdf_2 = (value + sqrt(sr_pdf_dn[n][i]))/(nTotal + sqrt(nTotal_pdf_up)) - eff;
      }

      //Yields
      result.sr[n][i].yield.value    =  value;
      result.sr[n][i].yield.stat     =  100*stat/value;
      result.sr[n][i].yield.scale_up =  100*(SR_scale_dn[n][i] - value)/value;
      result.sr[n][i].yield.scale_dn = -100*(SR_scale_up[n][i] - value)/value;
      result.sr[n][i].yield.pdf_up   =  100*sqrt(sr_pdf_up[n][i])/value;
      result.sr[n][i].yield.pdf_dn   =  100*sqrt(sr_pdf_dn[n][i])/value;
    
      //Efficiencies
      result.sr[n][i].eff.value    =  eff;
      result.sr[n][i].eff.stat     =  100*eff_stat/eff;
      result.sr[n][i].eff.scale_up =  100*max(eff_scale_1, eff_scale_2)/eff;
      result.sr[n][i].eff.scale_dn = -100*min(eff_scale_1, eff_scale_2)/eff;
      result.sr[n][i].eff.pdf_up   =  100*max(eff_pdf_1, eff_pdf_2)/eff;
      result.sr[n][i].eff.pdf_dn   = -100*min(eff_pdf_1, eff_pdf_2)/eff;

      cout << " " << endl;
      cout << "SR " << i+1 << ": "       << Form("% 9.5f",  result.sr[n][i].yield.value   ) << endl;
      cout << "  --> Stat err (%):     " << Form("% 9.5f",  result.sr[n][i].yield.stat    ) << endl;
      cout << "  --> Scale up (%):     " << Form("% 9.5f",  result.sr[n][i].yield.scale_up) << endl;
      cout << "  --> Scale dn (%):     " << Form("% 9.5f",  result.sr[n][i].yield.scale_dn) << endl;
      cout << "  --> PDF up (%):       " << Form("% 9.5f",  result.sr[n][i].yield.pdf_up  ) << endl;
      cout << "  --> PDF dn (%):       " << Form("% 9.5f",  result.sr[n][i].yield.pdf_dn  ) << endl;
      cout << "  --> Efficiency:       " << Form("% 9.5f",  result.sr[n][i].eff.value     ) << endl;
      cout << "  --> Eff stat (%):     " << Form("% 9.5f",  result.sr[n][i].eff.stat      ) << endl;
      cout << "  --> Eff scale up (%): " << Form("% 9.5f",  result.sr[n][i].eff.scale_up  ) << endl;
      cout << "  --> Eff scale dn (%): " << Form("% 9.5f",  result.sr[n][i].eff.scale_dn  ) << endl;
      cout << "  --> Eff PDF up (%):   " << Form("% 9.5f",  result.sr[n][i].eff.pdf_up    ) << endl;
      cout << "  --> Eff PDF dn (%):   " << Form("% 9.5f",  result.sr[n][i].eff.pdf_dn    ) << endl;

    }//SR loop
  }

  return result;

}

int analysis(){

  //Declare Chains
  TChain *fixed_chain = new TChain("Events");
  TChain *dynam_chain = new TChain("Events");

  //Fill Chains
  string infix = "fall2014";
  string suffix = "_noFilt";
  fixed_chain->Add(Form("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/%s/myTTW_fixed%s.root", doOld ? infix.c_str() : "", doOld ? "" : suffix.c_str()));
  dynam_chain->Add(Form("/hadoop/cms/store/user/cgeorge/SS_Syst_Study/ttW/Babies/%s/myTTW_dynam%s.root", doOld ? infix.c_str() : "", doOld ? "" : suffix.c_str()));

  //Do everything
  result_t fixed = run(fixed_chain);
  result_t dynam = run(dynam_chain);

  //Make Tables
  vector <CTable> tables; 
  CTable table1;
  CTable table2;
  CTable table3; 
  CTable table4; 
  CTable table5; 
  CTable table6; 
  CTable table7; 
  CTable table8; 
  CTable table9; 
  tables.push_back(table1);
  tables.push_back(table2);
  tables.push_back(table3);
  tables.push_back(table4);
  tables.push_back(table5);
  tables.push_back(table6);
  tables.push_back(table7);
  tables.push_back(table8);
  tables.push_back(table9);

  //Format Tables
  for (unsigned int i = 0; i < tables.size(); i++){
    cout << " " << endl;
    tables[i].setPrecision(2); 
    tables[i].setTitle(Form("TTW: Predicted %s -- %s", i < 3 ? "Yields" : "Efficiencies", i%3==0 ? "HH" : (i%3==1 ? "HL" : "LL")));
    tables[i].useTitle(); 
    if (i < 6) tables[i].printHLine(1);
    if (i < 6) tables[i].multiColumn(-1, 0, 3); 
    if (i < 6) tables[i].multiColumn(-1, 4, 7); 
    tables[i].setColLine(3); 
    tables[i].setColLine(0); 
    tables[i].setColLine(7); 
    if (i >= 6) tables[i].setColLine(8); 
  }

  //Fill yields tables
  for (int j = 0; j < (doOld ? 1 : 3); j++){
    tables[j].setTable() ("dynmaic", "", "", "", "fixed", "", "", "")
                         ("yield", "stat", "scale up/dn", "pdf up/dn", "yield", "stat", "scale up/dn", "pdf up/dn")
                         ("c-s", dynam.c_s.value, dynam.c_s.stat, Form("%.2f/%.2f", dynam.c_s.scale_up, dynam.c_s.scale_dn), Form("%.2f/%.2f", dynam.c_s.pdf_up, dynam.c_s.pdf_dn),fixed.c_s.value, fixed.c_s.stat, Form("%.2f/%.2f", fixed.c_s.scale_up, fixed.c_s.scale_dn), Form("%.2f/%.2f", fixed.c_s.pdf_up, fixed.c_s.pdf_dn));
    for (int i = 0; i < (doOld ? 8 : (j == 0 ? 32 : (j == 1 ? 26 : 8))); i++){
      int row = i+1; 
      tables[j].setRowLabel(Form("%s SR%i", (j == 0 ? "HH" : (j == 1 ? "HL" : "LL")), i+1), row); 
      tables[j].setCell(dynam.sr[j][i].yield.value, row, 0); 
      tables[j].setCell(dynam.sr[j][i].yield.stat, row, 1); 
      tables[j].setCell(Form("%.2f/%.2f", dynam.sr[j][i].yield.scale_up, dynam.sr[j][i].yield.scale_dn), row, 2); 
      tables[j].setCell(Form("%.2f/%.2f", dynam.sr[j][i].yield.pdf_up, dynam.sr[j][i].yield.pdf_dn), row, 3); 
      tables[j].setCell(fixed.sr[j][i].yield.value, row, 4); 
      tables[j].setCell(fixed.sr[j][i].yield.stat, row, 5); 
      tables[j].setCell(Form("%.2f/%.2f", fixed.sr[j][i].yield.scale_up, fixed.sr[j][i].yield.scale_dn), row, 6); 
      tables[j].setCell(Form("%.2f/%.2f", fixed.sr[j][i].yield.pdf_up, fixed.sr[j][i].yield.pdf_dn), row, 7); 
    }
    //tables[j].print(); 
    tables[j].saveTex(Form("yields_%i.tex", j));
  }

  //Fill eff tables
  for (int j = 0; j < (doOld ? 1 : 3); j++){
    int tablenumber = j + 3;
    tables[tablenumber].setTable() ("fixed", "", "", "", "dynamic", "", "", "")
                                   ("", "1000*eff", "stat", "scale up/dn", "pdf up/dn", "1000*eff", "stat", "scale up/dn", "pdf up/dn");
    for (int i = 0; i < (doOld ? 8 : (j == 0 ? 32 : (j == 1 ? 26 : 8))); i++){
      int row = i+1; 
      tables[tablenumber].setRowLabel(Form("%s SR%i", (j == 0 ? "HH" : (j == 1 ? "HL" : "LL")), i+1), row); 
      tables[tablenumber].setCell(1000*fixed.sr[j][i].eff.value, row, 0); 
      tables[tablenumber].setCell(fixed.sr[j][i].eff.stat, row, 1); 
      tables[tablenumber].setCell(Form("%.2f/%.2f", fixed.sr[j][i].eff.scale_up, fixed.sr[j][i].eff.scale_dn), row, 2); 
      tables[tablenumber].setCell(Form("%.2f/%.2f", fixed.sr[j][i].eff.pdf_up, fixed.sr[j][i].eff.pdf_dn), row, 3); 
      tables[tablenumber].setCell(dynam.sr[j][i].eff.value, row, 4); 
      tables[tablenumber].setCell(dynam.sr[j][i].eff.stat, row, 5); 
      tables[tablenumber].setCell(Form("%.2f/%.2f", dynam.sr[j][i].eff.scale_up, dynam.sr[j][i].eff.scale_dn), row, 6); 
      tables[tablenumber].setCell(Form("%.2f/%.2f", dynam.sr[j][i].eff.pdf_up, dynam.sr[j][i].eff.pdf_dn), row, 7); 
    }
    //tables[tablenumber].print(); 
    tables[tablenumber].saveTex(Form("eff_%i.tex", j));
  }

  //Fill yields tables
  for (int j = 0; j < (doOld ? 1 : 3); j++){
    int tablenumber = j + 6;
    tables[tablenumber].setTable() ("yield", "stat", "scale up/dn", "pdf up/dn", "tot err");
    for (int i = 0; i < (doOld ? 8 : (j == 0 ? 32 : (j == 1 ? 26 : 8))); i++){
      int row = i; 
      tables[tablenumber].setRowLabel(Form("%s SR%i", (j == 0 ? "HH" : (j == 1 ? "HL" : "LL")), i+1), row); 
      tables[tablenumber].setCell(dynam.sr[j][i].yield.value, row, 0); 
      tables[tablenumber].setCell(dynam.sr[j][i].yield.stat, row, 1); 
      tables[tablenumber].setCell(Form("%.2f/%.2f", dynam.sr[j][i].yield.scale_up/2, dynam.sr[j][i].yield.scale_dn/2), row, 2); 
      tables[tablenumber].setCell(Form("%.2f/%.2f", dynam.sr[j][i].yield.pdf_up, dynam.sr[j][i].yield.pdf_dn), row, 3); 
      tables[tablenumber].setCell( sqrt( pow(0.25*(fabs(dynam.sr[j][i].yield.scale_up) + fabs(dynam.sr[j][i].yield.scale_dn)), 2) + pow(0.5*(fabs(dynam.sr[j][i].yield.pdf_up) + fabs(dynam.sr[j][i].yield.pdf_dn)), 2)), row, 4); 
    }
    tables[tablenumber].print(); 
    tables[tablenumber].saveTex(Form("yields_ra5_%i.tex", j));
  }

  return 0;

}
