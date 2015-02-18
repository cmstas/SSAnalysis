{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  ch->Add("/nfs-7/userdata/ss2015/ssBabies/v1.03/ttbar.root");
  // ch->Add("/nfs-7/userdata/ss2015/ssBabies/v1.03/ttz.root");
  // ch->Add("/nfs-7/userdata/ss2015/ssBabies/v1.03/ttw.root");
  ScanChain(ch); 
}
