//root -b -q printShapes.C'("ttw","hihi","10.0","sr","ttw_extr","dir")'
//for p in ttw wz; do for s in btag jes; do root -b -q printShapes.C'("'$p'","hihi","1.3","sr","'$s'","v4.06")'; done; done

void printShapes(TString process, TString kine, TString lumi, TString nameN, TString nameA, TString dir){

  gROOT->Reset();
  gStyle->SetOptStat(0);

  bool doLogy = 0;

  TFile *_file = TFile::Open(Form("%s/%s_histos_%s_%sifb.root",dir.Data(),process.Data(),kine.Data(),lumi.Data()));

  TH1F* nominal = (TH1F*) _file->Get(nameN);
  if (kine=="hihi")   nominal->SetTitle(process+" "+nameA+" shape SRHH L="+lumi+"/fb");
  if (kine=="hilow")  nominal->SetTitle(process+" "+nameA+" shape SRHL L="+lumi+"/fb");
  if (kine=="lowlow") nominal->SetTitle(process+" "+nameA+" shape SRLL L="+lumi+"/fb");
  TH1F* up      = (TH1F*) _file->Get(nameA+"Up");
  TH1F* down    = (TH1F*) _file->Get(nameA+"Down");
  up->SetLineColor(kRed);
  down->SetLineColor(kBlue);


  TH1F* ratioup = (TH1F*) up->Clone("ratioup");
  TH1F* ratiodn = (TH1F*) down->Clone("ratiodn");
  ratioup->Add(nominal,-1.);
  ratioup->Divide(nominal);
  ratiodn->Add(nominal,-1.);
  ratiodn->Divide(nominal);

  TCanvas c1("c1","c1",800,800);
  gStyle->SetPadBorderMode(0);

  float max1 = nominal->GetBinContent(nominal->GetMaximumBin());
  float max2 = up->GetBinContent(up->GetMaximumBin());
  float max3 = down->GetBinContent(down->GetMaximumBin());
  float max = TMath::Max(max1,TMath::Max(max2,max3));

  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.26);
  pad2->SetTopMargin(0.);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();
  pad2->SetGridx(); 
  pad2->SetGridy(); 
  pad2->cd();
  ratioup->SetTitle("");
  ratioup->GetYaxis()->SetRangeUser(-0.6,0.6);
  ratioup->GetYaxis()->SetNdivisions(4,3,0,0);
  ratioup->GetYaxis()->SetLabelSize(0.08);
  ratioup->GetXaxis()->SetLabelSize(0.11);
  ratioup->GetXaxis()->SetTitleSize(0.12);
  ratioup->GetXaxis()->SetTitle("SR");
  ratioup->GetYaxis()->SetTitleSize(0.11);
  ratioup->GetYaxis()->SetTitleOffset(0.35);
  ratioup->GetYaxis()->SetTitle("#Delta/Yield   ");
  ratioup->Draw("HIST");
  ratiodn->Draw("HIST,SAME");
  pad2->Update();
  pad2->RedrawAxis("g");
  pad2->Update();

  c1.cd();

  TPad *pad1 = new TPad("pad1","pad1",0,0.25,1,1);
  pad1->SetBottomMargin(0.0);
  pad1->Draw();
  pad1->cd();

  pad1->SetGridx();
  if (doLogy) {
    nominal->GetYaxis()->SetRangeUser(0.01,1.2*max);
    c1.SetLogy();
  } else {
    nominal->GetYaxis()->SetRangeUser(0,1.2*max);
  }

  nominal->SetLineWidth(2.);
  up->SetLineWidth(2.);
  down->SetLineWidth(2.);

  nominal->GetXaxis()->SetTitle("");
  nominal->GetXaxis()->SetLabelSize(0.);

  nominal->Draw("E1");
  up->Draw("HIST same");
  down->Draw("HIST same");

  TLegend* leg = new TLegend(0.60,0.70,0.85,0.88);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(nominal," Nominal+Stat","le");
  leg->AddEntry(up,nameA+" Syst Up","l");
  leg->AddEntry(down,nameA+" Syst Down","l");
  leg->Draw();

  pad1->Update();
  pad1->RedrawAxis();
  pad1->Update();

  if (doLogy)
    c1.SaveAs(Form("%s/%s_%s_%sifb_%s_shape_log.pdf",dir.Data(),process.Data(),kine.Data(),lumi.Data(),nameA.Data()));
  else
    c1.SaveAs(Form("%s/%s_%s_%sifb_%s_shape.pdf",dir.Data(),process.Data(),kine.Data(),lumi.Data(),nameA.Data()));
}
