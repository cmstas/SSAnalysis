{

  gROOT->ProcessLine(".L ../../commonUtils.h");

  // bool doMu = true;
  // bool useIsoTrig = false;
  // bool do8 = false;

  for(int doMu = 0; doMu < 2; doMu++) {
  for(int useIsoTrig = 0; useIsoTrig < 2; useIsoTrig++) {
  for(int do8 = 0; do8 < 2; do8++) {

  // TString tag = getTag();
  TString tag = "v1.10FR";
  TString intlumi = Form("%f",getLumi());


  TString hlt = (doMu ? "HLT_Mu17" : "HLT_Ele17_CaloIdM_TrackIdM_PFJet30");
  if (useIsoTrig) hlt = (doMu ? "HLT_Mu17_TrkIsoVVL" : "HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30");
  if(do8) {
      hlt = (doMu ? "HLT_Mu8" : "HLT_Ele8_CaloIdM_TrackIdM_PFJet30");
      if (useIsoTrig) hlt = (doMu ? "HLT_Mu8_TrkIsoVVL" : "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30");
  }

  TChain* t_data = new TChain("t");
  TChain* t_dy = new TChain("t");

  t_data->Add(doMu ? "/nfs-7/userdata/leptonTree/"+tag+"/2016DoubleMuon*.root" : "/nfs-7/userdata/leptonTree/"+tag+"/2016DoubleEG*.root");
  t_dy->Add("/nfs-7/userdata/leptonTree/"+tag+"/DY_madgraph*.root");

  TH1F* nvtx_data = new TH1F("nvtx_data","nvtx_data",40,0,40);
  TH1F* nvtx_dy = new TH1F("nvtx_dy","nvtx_dy",40,0,40);
  nvtx_data->Sumw2();  
  nvtx_dy->Sumw2();

  int idlep = (doMu ? 13 : 11); 

  t_data->Draw("nvtx>>nvtx_data",Form("%s*(abs(id)==%i && passes_SS_tight_v5 && %s>0 && tag_p4.pt()>30. && p4.pt()>25.)",hlt.Data(),idlep,hlt.Data()),"goff");
  t_dy->Draw("nvtx>>nvtx_dy",Form("scale1fb*%s*(abs(id)==%i && passes_SS_tight_v5 && tag_p4.pt()>30. && p4.pt()>25.)",intlumi.Data(),idlep),"goff");

  nvtx_data->SetMarkerStyle(kFullCircle);
  nvtx_dy->SetFillColor(kGreen+2);

  cout << nvtx_dy->GetEntries() << endl;

  for(unsigned int ivtx = 1; ivtx < nvtx_dy->GetNbinsX(); ivtx++) {
      // cout << ivtx << " " << getTruePUw(ivtx) << " ";
      // cout << nvtx_data->GetBinContent(ivtx+1) << endl;
      float orig = nvtx_dy->GetBinContent(ivtx);
      nvtx_dy->SetBinContent(ivtx, orig*getTruePUw(ivtx)); // scale MC by PUw
  }

  nvtx_dy->Scale(nvtx_data->Integral()/nvtx_dy->Integral()); // rescale MC to data

  TCanvas c1;
  nvtx_dy->Draw("hist");
  nvtx_data->Draw("PE,same");


  TString suffix = "";
  suffix += useIsoTrig ? "_iso" : "";
  suffix += do8 ? "_8" : "_17";
  suffix += doMu ? "_mu" : "_el";

  cout << "Saving nvtx" << suffix << ".png" << endl;

  c1.SaveAs("nvtx" + suffix + ".png");

  // for(unsigned int ivtx = 1; ivtx <= nvtx_data->GetNbinsX(); ivtx++) {
  //     float data = nvtx_data->GetBinContent(ivtx+1);
  //     float dy = nvtx_dy->GetBinContent(ivtx+1);
  //     cout << data << " " << dy << endl;
  // }

  // TCanvas c2;
  // TH1F* nvtx_ratio = (TH1F*) nvtx_data->Clone("nvtx_ratio");
  // nvtx_ratio->Scale(1./nvtx_ratio->Integral());
  // TH1F* nvtx_den = (TH1F*) nvtx_dy->Clone("nvtx_den");
  // nvtx_den->Scale(1./nvtx_den->Integral());
  // nvtx_ratio->Divide(nvtx_den);
  // nvtx_ratio->Draw();
  // c2.SaveAs("nvtx_ratio.png");

  // TCanvas c3;
  // TH1F* nvtx_rw = (TH1F*) nvtx_dy->Clone("nvtx_rw");
  // for (int bin=1;bin<=nvtx_rw->GetNbinsX();++bin) {
  //   nvtx_rw->SetBinContent(bin, nvtx_ratio->GetBinContent(bin)*nvtx_rw->GetBinContent(bin));
  // }

  // nvtx_rw->SetFillColor(kGreen+2);
  // nvtx_data->Draw("PE");
  // nvtx_rw->Draw("hist,same");
  // nvtx_data->Draw("PEsame");
  // c3.SaveAs("nvtx_rw.png");

  // cout << "float getPUw(int nvtx) {" << endl;
  // for (int bin=1;bin<=nvtx_ratio->GetNbinsX();++bin) {
  //   cout << Form("   if (nvtx>=%f && nvtx<%f) return %f;",nvtx_ratio->GetBinLowEdge(bin),nvtx_ratio->GetBinLowEdge(bin+1),nvtx_ratio->GetBinContent(bin) ) << endl;
  // }
  // cout << "   return 0.;" << endl;
  // cout << "}" << endl;

  }
  }
  }


}
