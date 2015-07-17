{
  string tag = "leptree-v01";

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *data = new TChain("t"); 
  data->Add( "../../LeptonBabyMaker/2015DoubleMuon.root" );
  data->Add( "/nfs-7/userdata/leptonTree/v0.03/Run2015B/DoubleEG.root" );
  ScanChain(data,"./rate_histos_data_LooseEMVA.root","useLooseEMVA"); 

  // TChain *dy = new TChain("t"); 
  // dy->Add( "/nfs-7/userdata/leptonTree/v0.03/DY_M10_50ns.root" );
  // dy->Add( "/nfs-7/userdata/leptonTree/v0.03/DY_M50_50ns.root" );
  // ScanChain(dy,"./rate_histos_dy_LooseEMVA.root","useLooseEMVA"); 

  // TChain *wj = new TChain("t"); 
  // wj->Add( "/nfs-7/userdata/leptonTree/v0.03/WJets_50ns.root" );
  // ScanChain(wj,"./rate_histos_wj_LooseEMVA.root","useLooseEMVA"); 

  TChain *qcd = new TChain("t"); 
  qcd->Add( Form("../fake_rate_output/%s/QCD_Mu_Enriched.root",tag.c_str()) );
  qcd->Add( Form("../fake_rate_output/%s/QCD_EM_Enriched.root",tag.c_str()) );

  /*
  //default
  ScanChain(qcd,"./rate_histos_qcd.root",""); 
  //alternative versions
  ScanChain(qcd,"./rate_histos_qcd_LooseEMVA.root","useLooseEMVA"); 
  ScanChain(qcd,"./rate_histos_qcd_PtRatioCor.root","usePtRatioCor"); 
  ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_PtRatioCor.root","useLooseEMVA,usePtRatioCor"); 
  // ScanChain(qcd,"./rate_histos_qcd_RelIso.root","useRelIso"); 
  // ScanChain(qcd,"./rate_histos_qcd_RelIso_LooseEMVA.root","useRelIso,useLooseEMVA"); 
  //flavor specific
  ScanChain(qcd,"./rate_histos_qcd_doBonly.root","doBonly"); 
  ScanChain(qcd,"./rate_histos_qcd_doConly.root","doConly"); 
  ScanChain(qcd,"./rate_histos_qcd_doLightonly.root","doLightonly"); 
  ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_doBonly.root","useLooseEMVA_doBonly"); 
  ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_doConly.root","useLooseEMVA_doConly"); 
  ScanChain(qcd,"./rate_histos_qcd_LooseEMVA_doLightonly.root","useLooseEMVA_doLightonly"); 
  */

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
