{

  //TFile *file = new TFile("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/T5QQQQVV_1000to1075_1to950.root");
  //TFile *file = new TFile("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/T5QQQQVV_1100to1175_1to1050.root");
  //TFile *file = new TFile("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/T5QQQQVV_1200to1275_1to1150.root");
  TChain *chain = new TChain("t"); 
  ////chain->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/T1TTBB_*_*.root"); 
  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/T5QQQQWW_*_*.root"); 
  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/T5QQQQWW_*_*.root"); 
  //chain->Add("T5ttttDM175_*_*.root"); 
  chain->Add("T1*_*_*.root"); 
  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/T1TTBB_*_*.root"); 
  //chain->Add("/nfs-7/userdata/ss2015/ssBabies/v6.02-fs/T5ttttDM*.root");
  //chain->Add("T5ttcc_1025to1200_0to1025.root");
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ){
    TFile *file = new TFile(currentFile->GetTitle());
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
     cout << "if (mSp1 == " << mGluino << " && mSp2 == " << mLSP << ") return " << hist->GetBinContent(1) << ";" << endl;
    }
  }


}
