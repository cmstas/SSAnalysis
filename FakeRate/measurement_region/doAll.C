{
  string tag = "v1.03";

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *qcd = new TChain("t"); 
  qcd->Add( Form("../fake_rate_output/%s/QCD_Mu_Enriched.root",tag.c_str()) );
  qcd->Add( Form("../fake_rate_output/%s/QCD_EM_Enriched.root",tag.c_str()) );

  // default versions
  ScanChain(qcd,"./rate_histos_qcd.root",""); 
  ScanChain(qcd,"./rate_histos_qcd_LooseEMVA.root","useLooseEMVA"); 

  // alternative versions
  // ScanChain(qcd,"./rate_histos_qcd_PtRatioCor.root","usePtRatioCor"); 
  // ScanChain(qcd,"./rate_histos_qcd_InvPtRatio.root","useInvPtRatio"); 
  // ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_PtRatioCor.root","useLooseEMVA,usePtRatioCor"); 
  // ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_InvPtRatio.root","useLooseEMVA,useInvPtRatio"); 

  // versions for debug
  // ScanChain(qcd,"./rate_histos_qcd_onlyMuPt15.root","onlyMuPt15"); 
  // ScanChain(qcd,"./rate_histos_qcd_onlyEM80to170.root","onlyEM80to170"); 
  // ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_onlyEM30to80.root","useLooseEMVA_onlyEM30to80"); 
  // ScanChain(qcd,"./rate_histos_qcd_noPtRel.root","noPtRel"); 

  // old versions
  // ScanChain(qcd,"./rate_histos_qcd_RelIso.root","useRelIso"); 
  // ScanChain(qcd,"./rate_histos_qcd_RelIso_LooseEMVA.root","useRelIso,useLooseEMVA"); 

  // //flavor specific
  // ScanChain(qcd,"./rate_histos_qcd_doBonly.root","doBonly"); 
  // ScanChain(qcd,"./rate_histos_qcd_doConly.root","doConly"); 
  // ScanChain(qcd,"./rate_histos_qcd_doLightonly.root","doLightonly"); 
  // ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_doBonly.root","useLooseEMVA_doBonly"); 
  // ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_doConly.root","useLooseEMVA_doConly"); 
  // ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_doLightonly.root","useLooseEMVA_doLightonly"); 

  TChain *qcd_nonEnriched = new TChain("t"); 
  qcd_nonEnriched->Add( Form("../fake_rate_output/%s/qcd_nonEnriched.root",tag.c_str()) );
  // ScanChain(qcd_nonEnriched,"./rate_histos_qcd_nonEnriched.root",""); 
  
  TChain *qcd_HT = new TChain("t"); 
  qcd_HT->Add( Form("../fake_rate_output/%s/qcd_HT.root",tag.c_str()) );
  // ScanChain(qcd_HT,"./rate_histos_qcd_HT.root",""); 

  TChain *ttbar = new TChain("t"); 
  ttbar->Add( Form("../fake_rate_output/%s/ttbar.root",tag.c_str()) ); 
  // ScanChain(ttbar, "./rate_histos_ttbar.root",""); 
  
}
