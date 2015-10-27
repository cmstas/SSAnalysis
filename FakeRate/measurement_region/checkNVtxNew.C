{

  gROOT->ProcessLine(".L ../../commonUtils.h");

  bool doMu = 1;

  TString tag = getTag();
  TString ilumi = Form("%f",getLumi());

  TString dataf = (doMu ? "/nfs-7/userdata/leptonTree/"+tag+"/2015DDoubleMuon.root" : "/nfs-7/userdata/leptonTree/"+tag+"/2015DDoubleEG.root");

  TFile *_file_data = TFile::Open(dataf);
  TFile *_file_dy = TFile::Open("/nfs-7/userdata/leptonTree/"+tag+"/DY_amcnlo.root");

  TTree* t_data = (TTree*) _file_data->Get("t");
  TTree* t_dy = (TTree*) _file_dy->Get("t");

  TH1F* nvtx_data = new TH1F("nvtx_data","nvtx_data",20,0,40);
  TH1F* nvtx_dy = new TH1F("nvtx_dy","nvtx_dy",20,0,40);
  nvtx_data->Sumw2();  
  nvtx_dy->Sumw2();

  if (doMu) {
    //mm
    t_data->Draw("nvtx>>nvtx_data","HLT_Mu24*(abs(id)==13 && passes_SS_tight_v5 && HLT_Mu24>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
    t_dy->Draw("nvtx>>nvtx_dy",ilumi+"*scale1fb*(abs(id)==13 && passes_SS_tight_v5 && HLT_Mu24>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
  } else {
    //ee
    t_data->Draw("nvtx>>nvtx_data","abs(id)==11 && passes_SS_tight_v5 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30>0 && tag_p4.pt()>30. && p4.pt()>25.","goff");
    t_dy->Draw("nvtx>>nvtx_dy",ilumi+"*scale1fb*(abs(id)==11 && passes_SS_tight_v5 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
  }

  nvtx_data->SetMarkerStyle(kFullCircle);
  nvtx_dy->SetFillColor(kGreen+2);

  TCanvas c1;
  nvtx_data->Draw("PE");
  nvtx_dy->Draw("hist,same");
  nvtx_data->Draw("PEsame");
  c1.SaveAs("nvtx_unw.png");

  TCanvas c2;
  TH1F* nvtx_ratio = (TH1F*) nvtx_data->Clone("nvtx_ratio");
  nvtx_ratio->Scale(1./nvtx_ratio->Integral());
  TH1F* nvtx_den = (TH1F*) nvtx_dy->Clone("nvtx_den");
  nvtx_den->Scale(1./nvtx_den->Integral());
  nvtx_ratio->Divide(nvtx_den);
  nvtx_ratio->Draw();
  c2.SaveAs("nvtx_ratio.png");

  TCanvas c3;
  TH1F* nvtx_rw = (TH1F*) nvtx_dy->Clone("nvtx_rw");
  for (int bin=1;bin<=nvtx_rw->GetNbinsX();++bin) {
    nvtx_rw->SetBinContent(bin, nvtx_ratio->GetBinContent(bin)*nvtx_rw->GetBinContent(bin));
  }

  nvtx_rw->SetFillColor(kGreen+2);

  nvtx_data->Draw("PE");
  nvtx_rw->Draw("hist,same");
  nvtx_data->Draw("PEsame");

  c3.SaveAs("nvtx_rw.png");

  cout << "float getPUw(int nvtx) {" << endl;
  for (int bin=1;bin<=nvtx_ratio->GetNbinsX();++bin) {
    cout << Form("   if (nvtx>=%f && nvtx<%f) return %f;",nvtx_ratio->GetBinLowEdge(bin),nvtx_ratio->GetBinLowEdge(bin+1),nvtx_ratio->GetBinContent(bin) ) << endl;
  }
  cout << "   return 0.;" << endl;
  cout << "}" << endl;

}
