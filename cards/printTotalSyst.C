//root -b -q printTotalSyst.C'("ttw","hihi","3.0","v4.01")'
void printTotalSyst(TString process, TString kine, TString lumi, TString dir);

void printTotalSyst(TString lumi, TString dir){
  TString procs[] = {"ttw","ttzh","wz","ww"};
  TString kines[] = {"hihi","hilow","lowlow"};
  for (int p=0;p<4;++p)
    for (int k=0;k<3;++k)
      printTotalSyst(procs[p], kines[k], lumi, dir);
}

void printTotalSyst(TString process, TString kine, TString lumi, TString dir){

  gROOT->Reset();
  gStyle->SetOptStat(0);
  TFile *_file = TFile::Open(Form("%s/%s_histos_%s_%sifb.root",dir.Data(),process.Data(),kine.Data(),lumi.Data()));
  _file->cd();

  TH1F* nominal = (TH1F*) _file->Get("sr");
  if (kine=="hihi") nominal->SetTitle(process+" SRHH L="+lumi+"/fb");
  if (kine=="hilow") nominal->SetTitle(process+" SRHL L="+lumi+"/fb");
  if (kine=="lowlow") nominal->SetTitle(process+" SRLL L="+lumi+"/fb");
  TH1F* nominal2 = (TH1F*) nominal->Clone("band");
  nominal2->SetFillColor(kGray);

  float flatSyst2 = 0.;
  if (process=="ttw")  flatSyst2 = pow(0.13,2)+pow(0.04,2)+pow(0.04,2)+pow(0.02,2)+pow(0.04,2);
  if (process=="ttzh") flatSyst2 = pow(0.11,2)+pow(0.04,2)+pow(0.04,2)+pow(0.02,2)+pow(0.04,2);
  if (process=="ww")   flatSyst2 = pow(0.30,2)+pow(0.04,2)+pow(0.04,2)+pow(0.02,2)+pow(0.04,2);
  if (process=="wz" )  flatSyst2 = pow(0.30,2);

  cout << "flatSyst2=" << flatSyst2 << endl;

  TH1F* band = (TH1F*) nominal->Clone("band");
  band->Reset();
  TH1F* band2 = (TH1F*) band->Clone("band");
  band2->SetFillColor(kGray);

  TH1F* upvar = (TH1F*) nominal->Clone("upvar");
  upvar->Reset();

  TString discriminator = "Up";

  TIter next(_file->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1")) continue;
    TH1F *h = (TH1F*)key->ReadObj();
    if (TString(h->GetName()).Contains(discriminator.Data())==0) continue;
    if (TString(h->GetName()).Contains("stat")) continue;
    TH1F* h_clone = new TH1F();
    h->Copy(*h_clone);
    h_clone->Divide(nominal);
    cout << h->GetName() << " " << h_clone->GetBinContent(1) << endl;
    for (int bin=1;bin<=h_clone->GetNbinsX();++bin) {
      h_clone->SetBinContent(bin,pow(1.-h_clone->GetBinContent(bin),2));
    }
    upvar->Add(h_clone);
  }

  for (int bin=1;bin<=upvar->GetNbinsX();++bin) {
    upvar->SetBinContent(bin,sqrt(upvar->GetBinContent(bin)+flatSyst2));
  } 

  for (int bin=1;bin<=nominal->GetNbinsX();++bin) {
    nominal2->SetBinError(bin,upvar->GetBinContent(bin)*nominal->GetBinContent(bin));
    band->SetBinError(bin,nominal->GetBinError(bin)/nominal->GetBinContent(bin));
    band2->SetBinError(bin,upvar->GetBinContent(bin));
  }

  TCanvas c1("c1","c1",800,800);
  gStyle->SetPadBorderMode(0);

  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.26);
  pad2->SetTopMargin(0.);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();
  pad2->SetGridx(); 
  pad2->SetGridy(); 
  pad2->cd();
  band2->SetTitle("");
  band2->GetYaxis()->SetRangeUser(-0.5,0.5);
  band2->GetYaxis()->SetNdivisions(4,0);
  band2->GetYaxis()->SetLabelSize(0.08);
  band2->GetXaxis()->SetLabelSize(0.11);
  band2->GetXaxis()->SetTitleSize(0.12);
  band2->GetXaxis()->SetTitle("SR");
  band2->SetFillColor(kGray);
  band2->Draw("E2");
  pad2->Update();
  pad2->RedrawAxis("g");
  pad2->Update();

  c1.cd();

  TPad *pad1 = new TPad("pad1","pad1",0,0.25,1,1);
  pad1->SetBottomMargin(0.0);
  pad1->Draw();
  pad1->cd();

  pad1->SetGridx(); 
  nominal->GetYaxis()->SetRangeUser(0,2.*nominal->GetMaximum());
  nominal->GetXaxis()->SetLabelSize(0.);
  nominal->Draw("E1");
  nominal2->Draw("E2,SAME");
  nominal->Draw("E1,SAME");

  TLegend* leg = new TLegend(0.35,0.91,0.85,0.99);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetNColumns(2);
  leg->AddEntry(nominal ," Yield+Stat","le");
  leg->AddEntry(nominal2," Total Syst","f");
  leg->Draw();

  pad1->Update();
  pad1->RedrawAxis("g");
  pad1->Update();

  c1.SaveAs(Form("%s/%s_%s_%sifb_totUnc.png",dir.Data(),process.Data(),kine.Data(),lumi.Data()));

  // TCanvas c2;
  // band2->GetYaxis()->SetRangeUser(-0.5,0.5);
  // band2->Draw("E2");
  // c2.SaveAs(Form("%s/%s_%s_%sifb_totUnc_band.png",dir.Data(),process.Data(),kine.Data(),lumi.Data()));


}
