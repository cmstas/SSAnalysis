{

vector <string> files;
//files.push_back("T1TTTT_1100_1to775.root");
//files.push_back("T1TTTT_1100to1125_700to900.root");
//files.push_back("T1TTTT_1150_1to800.root");
//files.push_back("T1TTTT_1150to1175_750to925.root");
//files.push_back("T1TTTT_1175_950.root");
//files.push_back("T1TTTT_1200_1to825.root");
//files.push_back("T1TTTT_1200to1225_800to1000.root");
//files.push_back("T1TTTT_1250to1275_700to1050.root");
//files.push_back("T1TTTT_1275_900to975.root");
//files.push_back("T1TTTT_1300_1to1075.root");
//files.push_back("T1TTTT_1300to1325_700to1100.root");
//files.push_back("T1TTTT_1350to1375_50to1025.root");
//files.push_back("T1TTTT_1400to1425_50to1100.root");
//files.push_back("T1TTTT_1425to1450_1to1200.root");
//files.push_back("T1TTTT_1450to1475_50to1075.root");
//files.push_back("T1TTTT_1475to1500_1to1250.root");
//files.push_back("T1TTTT_1500to1525_50to1125.root");
//files.push_back("T1TTTT_1600to1650_1to1350.root");
//files.push_back("T1TTTT_1650to1700_1to1400.root");
//files.push_back("T1TTTT_1700to1750_1to1450.root");
//files.push_back("T1TTTT_1750_50to1450.root");
//files.push_back("T1TTTT_1800to1850_1to1450.root");
//files.push_back("T1TTTT_1850to1900_1to1450.root");
//files.push_back("T1TTTT_600_250to325.root");
//files.push_back("T1TTTT_625_275to375.root");
//files.push_back("T1TTTT_625to650_200to400.root");
//files.push_back("T1TTTT_650to675_250to425.root");
//files.push_back("T1TTTT_675_325to450.root");
//files.push_back("T1TTTT_700_1to450.root");
//files.push_back("T1TTTT_700to750_200to500.root");
//files.push_back("T1TTTT_750to775_350to525.root");
//files.push_back("T1TTTT_775_475to550.root");
//files.push_back("T1TTTT_800to825_1to575.root");
//files.push_back("T1TTTT_825to850_200to600.root");
//files.push_back("T1TTTT_850to875_450to625.root");
//files.push_back("T1TTTT_875to900_1to650.root");
//files.push_back("T1TTTT_950to975_350to725.root");
//files.push_back("T1TTTT_975_600to750.root");
//files.push_back("T1TTTT_1950_700to950.root");
//files.push_back("T1TTTT_1900to1950_0to1450.root"); 
//files.push_back("T1TTTT_1325to1350_1to1125.root");
//files.push_back("T1TTTT_1400_1to1175.root");
files.push_back("T1TTTT_1375_950to1150.root"); 

vector <string> stuff1;
vector <string> stuff2;
vector <string> stuff3;

for (unsigned int i = 0; i < files.size(); i++){
  //cout << files[i] << endl;
  //TFile *file = new TFile(Form("/nfs-7/userdata/ss2015/ssBabies/v5.06-fs/%s", files[i].c_str())); 
  TFile *file = new TFile(Form("%s", files[i].c_str())); 
  for (auto&& keyAsObj : *file->GetListOfKeys()){
   auto key = (TKey*) keyAsObj;
   TH1F *hist = (TH1F*)key->ReadObj(); 
   string title = hist->GetTitle();
   if (title == "SS2015 Baby Ntuple") continue;
   int find = title.find('_');
   int find2 = title.find('_', find+1);
   int find3 = title.length();
   string mGluino = title.substr(find+1, find2-find-1);
   string mLSP = title.substr(find2+1, find3-find2); 
  //cout << "  " << mGluino << " " <<  mLSP << endl;
  string blah1 = Form("TChain *fs_t1tttt_%s_%s_chain = new TChain(\"t\",\"fs_t1tttt_m%s_m%s\");", mGluino.c_str(), mLSP.c_str(), mGluino.c_str(), mLSP.c_str());
  string blah2 = Form("fs_t1tttt_%i_%i_chain->Add(Form(\"/nfs-7/userdata/ss2015/ssBabies/%%s-fs/%s\", tag.c_str()));", atoi(mGluino.c_str()), atoi(mLSP.c_str()), files[i].c_str());
  string blah3 = Form("pair<yields_t, plots_t> results_fs_t1tttt_%s_%s = run(fs_t1tttt_%s_%s_chain, 0, 0, 0, 0, 1);", mGluino.c_str(), mLSP.c_str(), mGluino.c_str(), mLSP.c_str()); 
  stuff1.push_back(blah1); 
  stuff2.push_back(blah2); 
  stuff3.push_back(blah3); 
  }
}
  for (unsigned int i = 0; i < stuff1.size(); i++) cout << stuff1.at(i) << endl;
  for (unsigned int i = 0; i < stuff2.size(); i++) cout << stuff2.at(i) << endl;
  for (unsigned int i = 0; i < stuff3.size(); i++) cout << stuff3.at(i) << endl;

}
