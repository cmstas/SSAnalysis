{
  string tag = "V00-00-04";

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *qcd = new TChain("t"); 
  qcd->Add( Form("../fake_rate_output/%s/qcd_MuEnriched.root",tag.c_str()) );
  qcd->Add( Form("../fake_rate_output/%s/qcd_EMEnriched.root",tag.c_str()) );
  ScanChain(qcd,"./rate_histos_qcd.root",""); 
  ScanChain(qcd,"./rate_histos_qcd_noSIP.root","noSIP"); 

  // TChain *ttbar = new TChain("t"); 
  // ttbar->Add( Form("../fake_rate_output/%s/ttbar1.root",tag.c_str()) ); 
  // ScanChain(ttbar); 
  
}
