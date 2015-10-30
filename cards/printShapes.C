//root -b -q printShapes.C'("ttw","hihi","10.0","sr","ttw_extr","dir")'

void printShapes(TString process, TString kine, TString lumi, TString nameN, TString nameA, TString dir){

  gROOT->Reset();
  gStyle->SetOptStat(0);
  TFile *_file = TFile::Open(Form("%s/%s_histos_%s_%sifb.root",dir.Data(),process.Data(),kine.Data(),lumi.Data()));

  TH1F* nominal = (TH1F*) _file->Get(nameN);
  TH1F* up      = (TH1F*) _file->Get(nameA+"Up");
  TH1F* down    = (TH1F*) _file->Get(nameA+"Down");

  TCanvas c1;

  float max1 = nominal->GetBinContent(nominal->GetMaximumBin());
  float max2 = up->GetBinContent(up->GetMaximumBin());
  float max3 = down->GetBinContent(down->GetMaximumBin());
  float max = TMath::Max(max1,TMath::Max(max2,max3));

  up->SetLineColor(kRed);
  down->SetLineColor(kBlue);

  nominal->GetYaxis()->SetRangeUser(0,1.2*max);

  nominal->SetLineWidth(2.);
  up->SetLineWidth(2.);
  down->SetLineWidth(2.);

  nominal->SetTitle(Form("%s %s %s shape",process.Data(),kine.Data(),nameA.Data()));
  nominal->GetXaxis()->SetTitle("SR bins");

  nominal->Draw("E1");
  up->Draw("HIST same");
  down->Draw("HIST same");

  TLegend* leg = new TLegend(0.63,0.70,0.85,0.88);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry(nominal," Default+Stat","le");
  leg->AddEntry(up," Syst Up","l");
  leg->AddEntry(down," Syst Down","l");
  leg->Draw();

  c1.SaveAs(Form("%s/%s_%s_%sifb_%s_shape.png",dir.Data(),process.Data(),kine.Data(),lumi.Data(),nameA.Data()));

}
