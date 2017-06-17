{

  // Load libraries and macros
  gROOT->ProcessLine(".L ../../commonUtils.h");
  gROOT->ProcessLine(".L vtx_reweight.h");
  gSystem->Load("../../CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ScanChain.C+");

  // Get tag name
  TString tag = getTag();
  tag = "v1.14FR";
  TString tag_data = tag;

  TChain *data = new TChain("t");
  data->Add( "/nfs-7/userdata/leptonTree/"+tag_data+"/2016DoubleEG*.root" );
  data->Add( "/nfs-7/userdata/leptonTree/"+tag_data+"/2016DoubleMuon*.root" );
  ScanChain(data,"./rate_histos_data_LooseEMVA_IsoTrigs.root",myAnalysisID,"useLooseEMVA,IsoTrigs");

  TChain *dy = new TChain("t");
  dy->Add( "/nfs-7/userdata/leptonTree/"+tag+"/DY_madgraph*.root" );
  ScanChain(dy,"./rate_histos_dy_LooseEMVA_IsoTrigs.root",myAnalysisID,"useLooseEMVA,IsoTrigs");

  TChain *wj = new TChain("t");
  wj->Add( "/nfs-7/userdata/leptonTree/"+tag+"/WJets*.root" );
  ScanChain(wj,"./rate_histos_wj_LooseEMVA_IsoTrigs.root",myAnalysisID,"useLooseEMVA,IsoTrigs");

  TChain *qcd_el = new TChain("t");
  qcd_el->Add( "/nfs-7/userdata/leptonTree/"+tag+"/QCD_EM_Enriched_80X*.root" );
  ScanChain(qcd_el,"./rate_histos_qcd_el_LooseEMVA_IsoTrigs.root",myAnalysisID,"useLooseEMVA,IsoTrigs");

  TChain *qcd_mu = new TChain("t");
  qcd_mu->Add( "/nfs-7/userdata/leptonTree/"+tag+"/QCD_Mu_Enriched*.root" );
  ScanChain(qcd_mu,"./rate_histos_qcd_mu_LooseEMVA_IsoTrigs.root",myAnalysisID,"useLooseEMVA,IsoTrigs");
}
