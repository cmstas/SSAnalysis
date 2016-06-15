pair<float,float> normalizeZpeak(float intlumi, TString tag, bool useIsoTrig, bool do8=false /*default is false*/) {

  // we want to output the MTCR SFs so we can lazily chain the macros together
  float sfel, sfmu;

  gStyle->SetOptStat(0);

  for(int doMu = 0; doMu < 2; doMu++) {
  gROOT->Reset();

  TString hlt = (doMu ? "HLT_Mu17" : "HLT_Ele17_CaloIdM_TrackIdM_PFJet30");
  if (useIsoTrig) hlt = (doMu ? "HLT_Mu17_TrkIsoVVL" : "HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30");

  if(do8) {
      hlt = (doMu ? "HLT_Mu8" : "HLT_Ele8_CaloIdM_TrackIdM_PFJet30");
      if (useIsoTrig) hlt = (doMu ? "HLT_Mu8_TrkIsoVVL" : "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30");
  }


  float mult = 1.0;

  float e8i = 1767.61;
  float e17i = 84.5516;
  float e8 = 1767.61;
  float e17 = 85.1862;

  float m8i = 238.145;
  float m17i = 28.3003;
  float m8 = 496.07;
  float m17 = 13.1493;

  if(do8) {
      if(useIsoTrig) mult = (doMu ? m8i : e8i);
      else mult = (doMu ? m8 : e8);
  } else {
      if(useIsoTrig) mult = (doMu ? m17i : e17i);
      else mult = (doMu ? m17 : e17);
  }


  TString suffix = (useIsoTrig ? "_IsoTrigs" : "");

  int idlep = (doMu ? 13 : 11); 

  TString dataf = (doMu ? "/nfs-7/userdata/leptonTree/"+tag+"/2016DoubleMu*.root" : "/nfs-7/userdata/leptonTree/"+tag+"/2016DoubleEG.root");
  TString dyf = "/nfs-7/userdata/leptonTree/"+tag+"/DY_madgraph*.root";

  TString leptype = (doMu ? "Mu17" : "Ele17");

  TChain* t_data = new TChain("t");
  TChain* t_dy = new TChain("t");
  t_data->Add(dataf);
  t_dy->Add(dyf);

  TH1F* mll_data = new TH1F("mll_data","mll_data",80,0,200);
  TH1F* mll_dy = new TH1F("mll_dy","mll_dy",80,0,200);
  mll_data->Sumw2();  
  mll_dy->Sumw2();

  t_data->Draw("dilep_mass>>mll_data",Form("%f*%s*(abs(id)==%i && passes_SS_tight_v5 && %s>0 && tag_p4.pt()>30. && p4.pt()>25.)",mult,hlt.Data(),idlep,hlt.Data()),"goff");
  // t_dy->Draw("dilep_mass>>mll_dy",Form("%f*scale1fb*%s*(abs(id)==%i && passes_SS_tight_v5 && %s>0 && tag_p4.pt()>30. && p4.pt()>25.)",intlumi,hlt.Data(),idlep,hlt.Data()),"goff");
  // NO electron triggers in 80X MC?
  hlt = "1";
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
  leg->SetHeader(Form("L=%.1f/pb",intlumi));
  leg->AddEntry(mll_data,"data","pe");
  leg->AddEntry(mll_dy  ,"DY","f");
  leg->AddEntry((TObject*)0  ,Form( "SF: %.2f", data_zpeak/mc_zpeak ),"");
  leg->Draw();

  c1.SaveAs( (doMu ? "pdfs/zpeak_mu"+suffix+".pdf" : "pdfs/zpeak_el"+suffix+".pdf") );


  }

  return make_pair(sfel,sfmu);
}
