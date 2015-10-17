pair<float,float> normalizeZpeak(float intlumi, bool useIsoTrig) {

  // we want to output the MTCR SFs so we can lazily chain the macros together
  float sfel, sfmu;

  gStyle->SetOptStat(0);

  for(int doMu = 0; doMu < 2; doMu++) {
  gROOT->Reset();

  TString tag = "v4.00";

  TString hlt = (doMu ? "HLT_Mu17" : "HLT_Ele12_CaloIdM_TrackIdM_PFJet30");
  if (useIsoTrig) hlt = (doMu ? "HLT_Mu17_TrkIsoVVL" : "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30");

  TString suffix = (useIsoTrig ? "_IsoTrigs" : "");

  int idlep = (doMu ? 13 : 11); 

  TString dataf = (doMu ? "/nfs-7/userdata/leptonTree/"+tag+"/2015DDoubleMuon.root" : "/nfs-7/userdata/leptonTree/"+tag+"/2015DDoubleEG.root");
  TString dyf = "/nfs-7/userdata/leptonTree/"+tag+"/DY_amcnlo_M50.root";

  TString leptype = (doMu ? "Mu17" : "Ele12");

  TFile *_file_data = TFile::Open(dataf);
  TFile *_file_dy = TFile::Open(dyf);

  TTree* t_data = (TTree*) _file_data->Get("t");
  TTree* t_dy = (TTree*) _file_dy->Get("t");

  TH1F* mll_data = new TH1F("mll_data","mll_data",80,0,200);
  TH1F* mll_dy = new TH1F("mll_dy","mll_dy",80,0,200);
  mll_data->Sumw2();  
  mll_dy->Sumw2();

  t_data->Draw("dilep_mass>>mll_data",Form("%s*(abs(id)==%i && passes_SS_tight_v5 && %s>0 && tag_p4.pt()>30. && p4.pt()>25.)",hlt.Data(),idlep,hlt.Data()),"goff");
  t_dy->Draw("dilep_mass>>mll_dy",Form("%f*scale1fb*%s*(abs(id)==%i && passes_SS_tight_v5 && %s>0 && tag_p4.pt()>30. && p4.pt()>25.)",intlumi,hlt.Data(),idlep,hlt.Data()),"goff");

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

  c1.SaveAs( (doMu ? "pdfs/zpeak_mu"+suffix+".pdf" : "pdfs/zpeak_el"+suffix+".pdf") );


  }

  return make_pair(sfel,sfmu);
}
