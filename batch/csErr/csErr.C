#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../classFiles/v4.05/SS.h"

void csErr(){

  TFile *file = new TFile("../T1TTTT_1950_750to950.root"); 
  for (auto&& keyAsObj : *file->GetListOfKeys()){
    auto key = (TKey*) keyAsObj;
    TH1F *hist = (TH1F*)key; 
    string title = hist->GetTitle();
    if (title == "SS2015 Baby Ntuple") continue;
    int find = title.find('_');
    int find2 = title.find('_', find+1);
    int find3 = title.length();
    int cs_int = hist->GetBinContent(2);
    string mGluino = title.substr(find+1, find2-find-1);
    string mLSP = title.substr(find2+1, find3-find2); 
    TH1F* cs_pdf_hist = new TH1F("cs_pdf_hist", "cs_pdf_hist", 10000, 0, 1000); 
    for (int i = 0; i < 100; i++){
      cs_pdf_hist->Fill( 1000*hist->GetBinContent(i+5)/hist->GetBinContent(1) ); 
    }
    float cs_pdf_error = cs_pdf_hist->GetRMS(); 
    cout << "mGluino,mLSP: " << mGluino << "," << mLSP << endl;
    cout << "cs from generator: " << 1000*cs_int << endl;
    cout << "cs from generator, stat err: " << 100*hist->GetBinError(2)/cs_int << endl;
    cout << "cs from generator, scale up/dn: " << 100*(hist->GetBinError(3) - cs_int)/cs_int << "/" << -100*(hist->GetBinError(4) - cs_int)/cs_int << endl;
    cout << "cs from generator, PDF error: " << 100*cs_pdf_error/cs_value << endl; 
    cout << "cs from generator, alpha_S error: " << -100*(hist->GetBinError(105) - cs_int)/cs_int << endl;
    cout << "cs from generator, alpha_S error: " <<  100*(hist->GetBinError(106) - cs_int)/cs_int << endl;
  }

}
