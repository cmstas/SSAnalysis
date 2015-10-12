pair<float,float> normalizeZpeak() {

  // we want to output the MTCR SFs so we can lazily chain the macros together
  float sfel, sfmu;

  gStyle->SetOptStat(0);

  for(int doMu = 0; doMu < 2; doMu++) {
  gROOT->Reset();

  TString tag = "v4.00";

  TString dataf = (doMu ? "/nfs-7/userdata/leptonTree/"+tag+"/2015DDoubleMuon.root" : "/nfs-7/userdata/leptonTree/"+tag+"/2015DDoubleEG.root");
  TString dyf = "/nfs-7/userdata/leptonTree/"+tag+"/DY_amcnlo_M50.root";

  TString intlumi="0.2095"; // fb^-1
  TString leptype = (doMu ? "Mu17" : "Ele12");

  TFile *_file_data = TFile::Open(dataf);
  TFile *_file_dy = TFile::Open(dyf);

  TTree* t_data = (TTree*) _file_data->Get("t");
  TTree* t_dy = (TTree*) _file_dy->Get("t");

  TH1F* mll_data = new TH1F("mll_data","mll_data",80,0,200);
  TH1F* mll_dy = new TH1F("mll_dy","mll_dy",80,0,200);
  mll_data->Sumw2();  
  mll_dy->Sumw2();

  if (doMu) {
    //mm
    t_data->Draw("dilep_mass>>mll_data","HLT_Mu17*(abs(id)==13 && passes_SS_tight_v5 && HLT_Mu17>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
    t_dy->Draw("dilep_mass>>mll_dy",intlumi+"*scale1fb*HLT_Mu17*(abs(id)==13 && passes_SS_tight_v5 && HLT_Mu17>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
  } else {
    //ee
    t_data->Draw("dilep_mass>>mll_data","HLT_Ele12_CaloIdM_TrackIdM_PFJet30 * (abs(id)==11 && passes_SS_tight_v5 && HLT_Ele12_CaloIdM_TrackIdM_PFJet30>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
    t_dy->Draw("dilep_mass>>mll_dy",intlumi+"*scale1fb*HLT_Ele12_CaloIdM_TrackIdM_PFJet30*(abs(id)==11 && passes_SS_tight_v5 && HLT_Ele12_CaloIdM_TrackIdM_PFJet30>0 && tag_p4.pt()>30. && p4.pt()>25.)","goff");
  }

  float mc_zpeak   = mll_dy->Integral(mll_dy->FindBin(75),mll_dy->FindBin(105));
  float data_zpeak = mll_data->Integral(mll_data->FindBin(75),mll_data->FindBin(105));

  cout << "doMu=" << doMu << " mc_zpeak=" << mc_zpeak << " data_zpeak=" << data_zpeak << " sf=" << data_zpeak/mc_zpeak << endl;  
  if(doMu) sfmu = data_zpeak/mc_zpeak;
  else sfel = data_zpeak/mc_zpeak;

  TCanvas c1;

  mll_data->SetMarkerStyle(kFullCircle);
  mll_dy->SetFillColor(kGreen+2);
  mll_dy->SetTitle("m_{ll} "+leptype);
  
  //mll_data->Draw("PE");
  mll_dy->Draw("hist");
  mll_data->Draw("PEsame");

  TLegend* leg = new TLegend(0.7,0.7,0.89,0.89);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetHeader("L=209.5/pb");
  leg->AddEntry(mll_data,"data","pe");
  leg->AddEntry(mll_dy  ,"DY","f");
  leg->AddEntry((TObject*)0  ,Form( "SF: %.2f", data_zpeak/mc_zpeak ),"");
  leg->Draw();

  c1.SaveAs( (doMu ? "pdfs/zpeak_mu.pdf" : "pdfs/zpeak_el.pdf") );


  }

  return make_pair(sfel,sfmu);
}
