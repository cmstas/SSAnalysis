{
  //not sure which ones I need
  gROOT->ProcessLine(".L ../../CORE/CMS3.cc+");
  gROOT->ProcessLine(".L ../../CORE/Base.cc+");
  gROOT->ProcessLine(".L ../../CORE/ElectronSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/MuonSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/MCSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/JetSelections.cc+");
  gROOT->ProcessLine(".L ../../CORE/IsolationTools.cc+");
  gROOT->ProcessLine(".L ../../CORE/SSSelections.cc+");

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  ch->Add("/nfs-7/userdata/ss2015/ssBabies/v1.03/ttbar.root");
  // ch->Add("/nfs-7/userdata/ss2015/ssBabies/v1.03/ttz.root");
  // ch->Add("/nfs-7/userdata/ss2015/ssBabies/v1.03/ttw.root");
  ScanChain(ch); 
}
