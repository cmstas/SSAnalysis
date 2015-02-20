{
  string tag = "V00-00-04";

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *qcd = new TChain("t"); 
  qcd->Add( Form("../fake_rate_output/%s/qcd1.root",tag.c_str()) );

  // TChain *ttbar = new TChain("t"); 
  // ttbar->Add( Form("../fake_rate_output/%s/ttbar1.root",tag.c_str()) ); 
  
  ScanChain(qcd); 
  //ScanChain(ttbar); 
}
