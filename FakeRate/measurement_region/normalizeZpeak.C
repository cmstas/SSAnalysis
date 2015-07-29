{

  bool doMu = 0;

  TString dataf = (doMu ? "/nfs-7/userdata/leptonTree/v0.08/2015DoubleMuon.root" : "/nfs-7/userdata/leptonTree/v0.08/2015DoubleEG.root");

  TFile *_file_data = TFile::Open(dataf);
  TFile *_file_dy = TFile::Open("/nfs-7/userdata/leptonTree/v0.08/DY.root");

  TTree* t_data = (TTree*) _file_data->Get("t");
  TTree* t_dy = (TTree*) _file_dy->Get("t");

  TH1F* mll_data = new TH1F("mll_data","mll_data",40,0,200);
  TH1F* mll_dy = new TH1F("mll_dy","mll_dy",40,0,200);
  mll_data->Sumw2();  
  mll_dy->Sumw2();

  if (doMu) {
    //mm
    t_data->Draw("dilep_mass>>mll_data","HLT_Mu24*(abs(id)==13 && passes_SS_tight_v3 && HLT_Mu24>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
    t_dy->Draw("dilep_mass>>mll_dy","0.040*scale1fb*(abs(id)==13 && passes_SS_tight_v3 && HLT_Mu24!=0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
  } else {
    //ee
    t_data->Draw("dilep_mass>>mll_data","abs(id)==11 && passes_SS_tight_v3 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30==1 && tag_p4.pt()>30. && p4.pt()>25.","goff");
    t_dy->Draw("dilep_mass>>mll_dy","0.35*0.040*scale1fb*(abs(id)==11 && passes_SS_tight_v3 && HLT_Ele23_CaloIdM_TrackIdM_PFJet30!=0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
  }
  
  mll_data->SetMarkerStyle(kFullCircle);
  mll_dy->SetFillColor(kGreen+2);
  
  mll_data->Draw("PE");
  mll_dy->Draw("hist,same");
  mll_data->Draw("PEsame");

}
