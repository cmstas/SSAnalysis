{

  bool doMu = 0;

  TString tag = "v0.12-T1MET3p0";
  // tag = "v0.12";
  // tag = "v0.08";

  TString dataf = (doMu ? "/nfs-7/userdata/leptonTree/"+tag+"/2015DoubleMuon.root" : "/nfs-7/userdata/leptonTree/"+tag+"/2015DoubleEG.root");

  TFile *_file_data = TFile::Open(dataf);
  TFile *_file_dy = TFile::Open( (tag=="v0.08" ? "/nfs-7/userdata/leptonTree/"+tag+"/DY.root" : "/nfs-7/userdata/leptonTree/"+tag+"/DY50ns.root") );

  TTree* t_data = (TTree*) _file_data->Get("t");
  TTree* t_dy = (TTree*) _file_dy->Get("t");

  TH1F* mll_data = new TH1F("mll_data","mll_data",40,0,200);
  TH1F* mll_dy = new TH1F("mll_dy","mll_dy",40,0,200);
  mll_data->Sumw2();  
  mll_dy->Sumw2();

  if (doMu) {
    //mm
    t_data->Draw("dilep_mass>>mll_data","HLT_Mu17*(abs(id)==13 && passes_SS_tight_v3 && HLT_Mu17>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
    t_dy->Draw("dilep_mass>>mll_dy","0.042*scale1fb*(abs(id)==13 && passes_SS_tight_v3 && HLT_Mu17>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
  } else {
    //ee
    t_data->Draw("dilep_mass>>mll_data","abs(id)==11 && passes_SS_tight_v3 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30==1 && tag_p4.pt()>30. && p4.pt()>25.","goff");
    t_dy->Draw("dilep_mass>>mll_dy","0.042*scale1fb*(abs(id)==11 && passes_SS_tight_v3 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30!=0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
  }

  float mc_zpeak   = mll_dy->Integral(mll_dy->FindBin(75),mll_dy->FindBin(105));
  float data_zpeak = mll_data->Integral(mll_data->FindBin(75),mll_data->FindBin(105));

  cout << "mc_zpeak=" << mc_zpeak << " data_zpeak=" << data_zpeak << " sf=" << data_zpeak/mc_zpeak << endl;  

  TCanvas c1;

  mll_data->SetMarkerStyle(kFullCircle);
  mll_dy->SetFillColor(kGreen+2);
  
  //mll_data->Draw("PE");
  mll_dy->Draw("hist");
  mll_data->Draw("PEsame");

  c1.SaveAs( (doMu ? "zpeak_mu.png" : "zpeak_el.png") );

}
