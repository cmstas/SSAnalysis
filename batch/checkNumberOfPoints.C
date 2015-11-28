{

  TFile *file = new TFile("ALLFASTSIM.root"); 
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
   cout << "if (mgl == " << mGluino << " && mlsp == " << mLSP << ") return " << hist->GetBinContent(1) << ";" << endl;
  }


}
