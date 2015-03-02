{
  string tag = "V00-00-05";

  gROOT->ProcessLine(".L ScanChain_pTrelVsIso.C+");

  TChain *qcd = new TChain("t"); 
  qcd->Add( Form("../fake_rate_output/%s/qcd_MuEnriched.root",tag.c_str()) );
  qcd->Add( Form("../fake_rate_output/%s/qcd_EMEnriched.root",tag.c_str()) );

  ScanChain(qcd,"./pTrelVsIso_histos_qcd.root",""); 
  ScanChain(qcd,"./pTrelVsIso_histos_qcd_doBonly.root","doBonly"); 
  ScanChain(qcd,"./pTrelVsIso_histos_qcd_doConly.root","doConly"); 
  ScanChain(qcd,"./pTrelVsIso_histos_qcd_doLightonly.root","doLightonly"); 

  TChain *ttbar = new TChain("t"); 
  ttbar->Add( Form("../fake_rate_output/%s/ttbar.root",tag.c_str()) ); 
  ScanChain(ttbar, "./pTrelVsIso_histos_ttbar.root",""); 
  ScanChain(ttbar,"./pTrelVsIso_histos_ttbar_doBonly.root","doBonly"); 
  ScanChain(ttbar,"./pTrelVsIso_histos_ttbar_doConly.root","doConly"); 
  ScanChain(ttbar,"./pTrelVsIso_histos_ttbar_doLightonly.root","doLightonly"); 
  
}
