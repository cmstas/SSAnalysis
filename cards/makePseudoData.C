void addThisProcess(TH1F* h_sr, TString process, TString kine, TString lumi, TString dir) {
  TFile *file = TFile::Open(Form("%s/%s_histos_%s_%sifb.root",dir.Data(),process.Data(),kine.Data(),lumi.Data()));
  TH1F* h_sr_proc = (TH1F*) file->Get("sr");
  h_sr->Add(h_sr_proc);
  file->Close();
}

void makePseudoData(TString lumi, TString dir) {

  TString procs[] = {"ttw","ttzh","wz","ww","tg","vg","rares","fakes","flips"};
  TString kines[] = {"hihi","hilow","lowlow"};
  for (int k=0;k<3;++k) {
    TFile* fout = TFile::Open(dir+"/pdata_histos_"+kines[k]+"_"+lumi+"ifb.root","RECREATE");
    int nsr = 32;
    if (kines[k]=="hilow") nsr = 26;
    if (kines[k]=="lowlow")  nsr = 8;
    TH1F* h_sr = new TH1F("sr","sr",nsr,1,nsr+1);
    for (int p=0;p<9;++p) {
      addThisProcess(h_sr,procs[p], kines[k], lumi, dir);
    }
    fout->cd();
    h_sr->Write();
    fout->Close();
  }

}
