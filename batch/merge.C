void merge(const TString& in, const TString& out) {

  //Verbose flag
  bool verbose = 1;

  //Set up chains and iterator
  TChain *chain = new TChain("t");
  chain->SetMaxTreeSize(15000000000LL); //default is 100000000000LL
  if (verbose) std::cout << "Merging files from dir: " << in << std::endl << "Ouputting to: " << out << std::endl;
  chain->Add(in + "_*.root");
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //First check for missing files
  int nFilesInChain = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ){
    nFilesInChain++; 
  }
  ifstream lastfile(Form("%s_%i.root", in.Data(), nFilesInChain)); 
  if (lastfile.good()){
    lastfile.close();
    if (verbose) cout << "Right number of files" << endl;
  }
  else {
    lastfile.close(); 
    cout << "Error! " << in << " doesn't have enough files!" << endl;
    return;
  }

  //Next corrupt files
  currentFile = 0;
  fileIter.Reset(); 
  while ( (currentFile = (TFile*)fileIter.Next()) ){
      std::cout << " currentFile->GetTitle(): " << currentFile->GetTitle() << std::endl;
    TFile *file = TFile::Open(currentFile->GetTitle());
    if (file->IsZombie()){
      cout << "Error! " << currentFile->GetTitle() << " is corrupt" << endl;
      return;  
    }
  }
  cout << "No corrupt files, all good."  << endl;

  //Then merge trees
  chain->Merge("/nfs-7/userdata/ss2015/ssBabies/" + out, "fast");
  if (verbose) std::cout << "Total events merged: " << chain->GetEntries() << std::endl;

  //Then merge 1D hists
  std::vector <TH1F*> ourHists; 
  currentFile = 0;
  fileIter.Reset(); 
  //file loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){
    TFile *file = TFile::Open(currentFile->GetTitle());
    //Hist loop
    for (auto&& keyAsObj : *file->GetListOfKeys()){
      auto key = (TKey*)keyAsObj;
      std::cout << " key->GetName(): " << key->GetName() << std::endl;
      if (strncmp(key->GetClassName(), "TH1F", 1000) != 0) continue; 
      TH1F *hist = (TH1F*)key->ReadObj(); 
      //OurHist loop
      bool foundIt = false;
      for (unsigned int i = 0; i < ourHists.size(); i++){
        if (strncmp(ourHists[i]->GetTitle(), hist->GetTitle(), 1000) == 0){ ourHists[i]->Add(hist); foundIt = true; }
      }
      if (!foundIt) ourHists.push_back(hist); 
    }
  }
  cout << ourHists.size() << endl;
  cout << ourHists[0]->GetBinContent(0) << endl; 
  
  TFile *file = new TFile("/nfs-7/userdata/ss2015/ssBabies/" + out, "UPDATE"); 
  for (unsigned int i = 0; i < ourHists.size(); i++) ourHists[i]->Write();  
  delete file; 

  if (verbose) cout << "done!" << endl;

}
