{
  string tag = "V00-00-04";

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *qcd = new TChain("t"); 
  qcd->Add( Form("../fake_rate_output/%s/qcd_MuEnriched.root",tag.c_str()) );
  qcd->Add( Form("../fake_rate_output/%s/qcd_EMEnriched.root",tag.c_str()) );
  ScanChain(qcd,"./rate_histos_qcd.root",""); 
  ScanChain(qcd,"./rate_histos_qcd_noSIP.root","noSIP"); 
  ScanChain(qcd,"./rate_histos_qcd_ptRel.root","ptRel"); 
  ScanChain(qcd,"./rate_histos_qcd_ptRel_noSIP.root","ptRel_noSIP"); 

  TChain *ttbar = new TChain("t"); 
  ttbar->Add( Form("../fake_rate_output/%s/ttbar.root",tag.c_str()) ); 
  //ScanChain(ttbar, "./rate_histos_ttbar.root"); 
  
}
