pair<float, float> dumpCardForOneSR(TString process, TString kine, int sr, TString lumi, TString dir);

void makeTable() {

  TString lumi = "1.3";
  TString dir = "v5.03";

  TString procs[] = {"ttw","ttzh","wz","ww","xg","rares","flips","fakes"};

  TString kine = "hihi";
  for (int sr=1;sr<33;++sr) {
    pair<float, float> tye(0,0);
    cout << Form("SR%i ",sr);
    for (int p=0;p<8;++p) {
      pair<float, float> ye = dumpCardForOneSR(procs[p], kine, sr, lumi, dir);
      tye.first+=ye.first;
      tye.second+=ye.second;
      cout << Form("& %6.3f $\\pm$ %6.3f ",ye.first,sqrt(ye.second));
    }
    cout << Form("& %6.3f $\\pm$ %6.3f ",tye.first,sqrt(tye.second));
    float dye = dumpCardForOneSR(TString("data"), kine, sr, lumi, dir).first;
    cout << Form("& %6.0f ",dye);
    float sye = dumpCardForOneSR(TString("t1tttt_1200"), kine, sr, lumi, dir).first;
    cout << Form("& %6.3f ",sye);
    cout << " \\\\ " << endl;
  }
  cout << endl;
  cout << endl;
  
  kine = "hilow";
  for (int sr=1;sr<27;++sr) {
    pair<float, float> tye(0,0);
    cout << Form("SR%i ",sr);
    for (int p=0;p<8;++p) {
      pair<float, float> ye = dumpCardForOneSR(procs[p], kine, sr, lumi, dir);
      tye.first+=ye.first;
      tye.second+=ye.second;
      cout << Form("& %6.3f $\\pm$ %6.3f ",ye.first,sqrt(ye.second));
    }
    cout << Form("& %6.3f $\\pm$ %6.3f ",tye.first,sqrt(tye.second));
    float dye = dumpCardForOneSR(TString("data"), kine, sr, lumi, dir).first;
    cout << Form("& %6.0f ",dye);
    float sye = dumpCardForOneSR(TString("t1tttt_1200"), kine, sr, lumi, dir).first;
    cout << Form("& %6.3f ",sye);
    cout << " \\\\ " << endl;
  }
  cout << endl;
  cout << endl;
  
  kine = "lowlow";
  for (int sr=1;sr<9;++sr) {
    pair<float, float> tye(0,0);
    cout << Form("SR%i ",sr);
    for (int p=0;p<8;++p) {
      pair<float, float> ye = dumpCardForOneSR(procs[p], kine, sr, lumi, dir);
      tye.first+=ye.first;
      tye.second+=ye.second;
      cout << Form("& %6.3f $\\pm$ %6.3f ",ye.first,sqrt(ye.second));
    }
    cout << Form("& %6.3f $\\pm$ %6.3f ",tye.first,sqrt(tye.second));
    float dye = dumpCardForOneSR(TString("data"), kine, sr, lumi, dir).first;
    cout << Form("& %6.0f ",dye);
    float sye = dumpCardForOneSR(TString("t1tttt_1200"), kine, sr, lumi, dir).first;
    cout << Form("& %6.3f ",sye);
    cout << " \\\\ " << endl;
  }
  
}


pair<float, float> dumpCardForOneSR(TString process, TString kine, int sr, TString lumi, TString dir){

  TFile *_file = TFile::Open(Form("%s/%s_histos_%s_%sifb.root",dir.Data(),process.Data(),kine.Data(),lumi.Data()));
  _file->cd();

  pair<float, float> result;

  TH1F* nominal = (TH1F*) _file->Get("sr");
  if (kine=="hihi")   nominal->SetTitle(process+" SRHH L="+lumi+"/fb");
  if (kine=="hilow")  nominal->SetTitle(process+" SRHL L="+lumi+"/fb");
  if (kine=="lowlow") nominal->SetTitle(process+" SRLL L="+lumi+"/fb");

  float yield = nominal->GetBinContent(sr);
  result.first = yield;

  float ttw = pow(0.13,2)+pow(0.04,2);
  float ttz = pow(0.11,2)+pow(0.04,2);
  float ww  = pow(0.30,2);
  float wz  = pow(0.25,2);
  float fakes  = pow(0.30,2);
  float flips  = pow(0.26,2);
  float xg = pow(0.50,2);
  float rares = pow(0.50,2);
  float lepeff = pow(0.04,2)+pow(0.02,2);//both offline and hlt
  float luminosity = pow(0.12,2);

  float error = 0.;
  if (process=="ttw"  ) error = ttw+lepeff+luminosity;
  if (process=="ttzh" ) error = ttz+lepeff+luminosity;
  if (process=="ww"   ) error = ww+lepeff+luminosity;
  if (process=="wz"   ) error = wz;
  if (process=="fakes") error = fakes;
  if (process=="flips") error = flips;
  if (process=="rares") error = rares+lepeff+luminosity;
  if (process=="xg"   ) error = xg+lepeff+luminosity;

  error = error*yield*yield;

  // if (process=="fakes" && sr==1) cout << endl << yield << " " << sqrt(error) << endl;

  TString up = "Up";
  TIter next(_file->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1")) continue;
    TH1F *h = (TH1F*)key->ReadObj();
    TString name = h->GetName();
    bool isUp = name.Contains(up.Data());
    if ( isUp ) {
      if (name.Contains("stat") && name.Contains(Form("%i",sr))==0) continue;
      error += pow( h->GetBinContent(sr)-yield ,2);
      //cout << process << " " << h->GetBinContent(sr)-yield << endl;
      if (process=="fakes" && sr==28) cout << process << " " << h->GetBinContent(sr)-yield << endl;
      if (process=="fakes" && sr==29) cout << process << " " << h->GetBinContent(sr)-yield << endl;
    }
  }

  result.second = error;

  return result;
}
