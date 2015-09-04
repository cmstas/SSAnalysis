{
  //string tag = "leptree-v01";

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *data = new TChain("t"); 
  data->Add( "/hadoop/cms/store/user/namin/condor/forCommissioning/merged_files/2015CDoubleEG.root" );
  data->Add( "/hadoop/cms/store/user/namin/condor/forCommissioning/merged_files/2015CDoubleMuon.root" );
  ScanChain(data,"./rate_histos_data_LooseEMVA.root","useLooseEMVA"); 

  TChain *dy = new TChain("t"); 
  dy->Add( "/hadoop/cms/store/user/namin/condor/forCommissioning/merged_files/DY_madgraph.root" );
  ScanChain(dy,"./rate_histos_dy_LooseEMVA.root","useLooseEMVA"); 

  TChain *wj = new TChain("t"); 
  wj->Add( "/hadoop/cms/store/user/namin/condor/forCommissioning/merged_files/WJets.root" );
  ScanChain(wj,"./rate_histos_wj_LooseEMVA.root","useLooseEMVA"); 

  // TChain *qcd_mu = new TChain("t"); 
  // qcd_mu->Add( "/nfs-7/userdata/leptonTree/v0.08/QCD_Mu_Enriched.root" );
  // canChain(qcd_mu,"./rate_histos_qcd_mu_LooseEMVA.root","useLooseEMVA"); 

  // TChain *qcd_el = new TChain("t"); 
  // qcd_el->Add( "/nfs-7/userdata/leptonTree/v0.08/QCD_EM_Enriched.root" );
  // ScanChain(qcd_el,"./rate_histos_qcd_el_LooseEMVA.root","useLooseEMVA"); 

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

  // TChain *qcd_nonEnriched = new TChain("t"); 
  // qcd_nonEnriched->Add( Form("../fake_rate_output/%s/qcd_nonEnriched.root",tag.c_str()) );
  // ScanChain(qcd_nonEnriched,"./rate_histos_qcd_nonEnriched.root",""); 
  
  // TChain *qcd_HT = new TChain("t"); 
  // qcd_HT->Add( Form("../fake_rate_output/%s/qcd_HT.root",tag.c_str()) );
  // ScanChain(qcd_HT,"./rate_histos_qcd_HT.root",""); 

  // TChain *ttbar = new TChain("t"); 
  // ttbar->Add( Form("../fake_rate_output/%s/ttbar.root",tag.c_str()) ); 
  // ScanChain(ttbar, "./rate_histos_ttbar.root",""); 
  
}
