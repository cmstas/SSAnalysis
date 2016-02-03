void merge(const TString& in, const TString& out) {
  //First merge trees
  TChain *chain = new TChain("t");
  chain->SetMaxTreeSize(5000000000LL); //default is 100000000000LL
  std::cout << "Merging files from dir: " << in << std::endl
        << "Ouputting to: " << out << std::endl;
  chain->Add(in);
  chain->Merge("/nfs-7/userdata/ss2015/ssBabies/" + out, "fast");
  std::cout << "Total events merged: " << chain->GetEntries() << std::endl;

  //Then merge 1D hists
  std::vector <TH1F*> ourHists; 
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  //file loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){
    TFile *file = TFile::Open(currentFile->GetTitle());
    //Hist loop
    for (auto&& keyAsObj : *file->GetListOfKeys()){
      auto key = (TKey*)keyAsObj;
      if (strncmp(key->GetClassName(), "TH1F", 1000) != 0) continue; 
      TH1F *hist = (TH1F*)key->ReadObj(); 
      //OurHist loop
      bool foundIt = false;
      for (unsigned int i = 0; i < ourHists.size(); i++){
        if (strncmp(ourHists[i]->GetTitle(), hist->GetTitle(), 1000) == 0){ ourHists[i]->Add(hist); foundIt = true; }
      }
      if (!foundIt) ourHists.push_back((TH1F*)hist->Clone()); 
    }
  }
  
  TFile *file = new TFile("/nfs-7/userdata/ss2015/ssBabies/" + out, "UPDATE"); 
  for (unsigned int i = 0; i < ourHists.size(); i++) ourHists[i]->Write();  
  delete file; 

}
