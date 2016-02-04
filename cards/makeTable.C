pair<float, float> dumpCardForOneSR(TString process, TString kine, int sr, TString lumi, TString dir);

void makeTable() {

  bool doLatex = true;

  char* sep = "&";
  char* pm  = "$\\pm$";
  if (!doLatex) {
    sep = "|";
    pm  = "+/-";
  }

  TString lumi = "2.3";
  TString dir = "v6.02";

  TString procs[] = {"ttw","ttzh","wz","ww","xg","rares","flips","fakes"};

  float tHH[32] = {0.};
  float tHL[32] = {0.};
  float tLL[32] = {0.};
  float eHH[32] = {0.};
  float eHL[32] = {0.};
  float eLL[32] = {0.};
  int   nHH[32] = {0};
  int   nHL[32] = {0};
  int   nLL[32] = {0};
  float dHH[32] = {0.};
  float dHL[32] = {0.};
  float dLL[32] = {0.};

  if (doLatex) {
    cout << "\\documentclass[10pt,a4paper]{article}" << endl;
    cout << "\\usepackage{lscape}" << endl;
    cout << "\\begin{document}" << endl;
    cout << endl;
  }

  TString header = "     & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %10s \\\\";
  if (!doLatex) header = "     | %17s | %17s | %17s | %17s | %17s | %17s | %17s | %17s | %17s | %6s | %10s ";

  if (doLatex) {
    cout << "\\begin{landscape}" << endl;
    cout << "\\begin{table}" << endl;
    cout << "\\footnotesize" << endl;
    cout << "\\caption{Event yields in HH regions.}" << endl;
    cout << "\\label{tab:yieldsHH}" << endl;
    cout << "\\begin{tabular}{c|cccccccc|c|c|c}\\hline" << endl;
    cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
    cout << "\\hline\\hline" << endl;
  } else {
    cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
  }
  TString kine = "hihi";
  for (int sr=1;sr<33;++sr) {
    pair<float, float> tye(0,0);
    cout << Form("SR%2i ",sr);
    for (int p=0;p<8;++p) {
      pair<float, float> ye = dumpCardForOneSR(procs[p], kine, sr, lumi, dir);
      tye.first+=ye.first;
      tye.second+=ye.second;
      int ndec = 1;
      if (ye.first<1.    ||sqrt(ye.second)<1.) ndec = 2; 
      if (ye.first<0.01  ||sqrt(ye.second)<0.01) ndec = 3;
      if (ye.first<0.0005||sqrt(ye.second)<0.0005) ndec = 1;
      cout << Form("%s %6.*f %s %6.*f ",sep,ndec,ye.first,pm,ndec,sqrt(ye.second));
    } 
    int ndec = 1;
    if (tye.first<1.    ||sqrt(tye.second)<1.) ndec = 2; 
    if (tye.first<0.01  ||sqrt(tye.second)<0.01) ndec = 3;
    if (tye.first<0.0005||sqrt(tye.second)<0.0005) ndec = 1;
    cout << Form("%s %6.*f %s %6.*f ",sep,ndec,tye.first,pm,ndec,sqrt(tye.second));
    float dye = dumpCardForOneSR(TString("data"), kine, sr, lumi, dir).first;
    cout << Form("%s %6.0f ",sep,dye);
    float sye = dumpCardForOneSR(TString("t1tttt_1200"), kine, sr, lumi, dir).first;
    cout << Form("%s %6.3f ",sep,sye);
    if (doLatex) cout << " \\\\ " << endl;
    else cout << endl;
    tHH[sr-1] = tye.first;
    eHH[sr-1] = sqrt(tye.second);
    nHH[sr-1] = ndec;
    dHH[sr-1] = dye;
  }
  if (doLatex) {
    cout << "\\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << "\\end{table}" << endl;
    cout << "\\end{landscape}" << endl;
  }

  cout << endl;
  cout << endl;
  
  if (doLatex) {
    cout << "\\begin{landscape}" << endl;
    cout << "\\begin{table}" << endl;
    cout << "\\footnotesize" << endl;
    cout << "\\caption{Event yields in HL regions.}" << endl;
    cout << "\\label{tab:yieldsHL}" << endl;
    cout << "\\begin{tabular}{c|cccccccc|c|c|c}\\hline" << endl;
    cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
    cout << "\\hline\\hline" << endl;
  } else {
    cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
  }
  kine = "hilow";
  for (int sr=1;sr<27;++sr) {
    pair<float, float> tye(0,0);
    cout << Form("SR%2i ",sr);
    for (int p=0;p<8;++p) {
      pair<float, float> ye = dumpCardForOneSR(procs[p], kine, sr, lumi, dir);
      tye.first+=ye.first;
      tye.second+=ye.second;      
      int ndec = 1;
      if (ye.first<1.    ||sqrt(ye.second)<1.) ndec = 2; 
      if (ye.first<0.01  ||sqrt(ye.second)<0.01) ndec = 3;
      if (ye.first<0.0005||sqrt(ye.second)<0.0005) ndec = 1;
      cout << Form("%s %6.*f %s %6.*f ",sep,ndec,ye.first,pm,ndec,sqrt(ye.second));
    }
    int ndec = 1;
    if (tye.first<1.    ||sqrt(tye.second)<1.) ndec = 2; 
    if (tye.first<0.01  ||sqrt(tye.second)<0.01) ndec = 3;
    if (tye.first<0.0005||sqrt(tye.second)<0.0005) ndec = 1;
    cout << Form("%s %6.*f %s %6.*f ",sep,ndec,tye.first,pm,ndec,sqrt(tye.second));
    float dye = dumpCardForOneSR(TString("data"), kine, sr, lumi, dir).first;
    cout << Form("%s %6.0f ",sep,dye);
    float sye = dumpCardForOneSR(TString("t1tttt_1200"), kine, sr, lumi, dir).first;
    cout << Form("%s %6.3f ",sep,sye);
    if (doLatex) cout << " \\\\ " << endl;
    else cout << endl;
    tHL[sr-1] = tye.first;
    eHL[sr-1] = sqrt(tye.second);
    nHL[sr-1] = ndec;
    dHL[sr-1] = dye;
  }
  if (doLatex) {
    cout << "\\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << "\\end{table}" << endl;
    cout << "\\end{landscape}" << endl;
  }

  cout << endl;
  cout << endl;
  
  if (doLatex) {
    cout << "\\begin{landscape}" << endl;
    cout << "\\begin{table}" << endl;
    cout << "\\footnotesize" << endl;
    cout << "\\caption{Event yields in LL regions.}" << endl;
    cout << "\\label{tab:yieldsLL}" << endl;
    cout << "\\begin{tabular}{c|cccccccc|c|c|c}\\hline" << endl;
    cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
    cout << "\\hline\\hline" << endl;
  } else {
    cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
  }
  kine = "lowlow";
  for (int sr=1;sr<9;++sr) {
    pair<float, float> tye(0,0);
    cout << Form("SR%2i ",sr);
    for (int p=0;p<8;++p) {
      pair<float, float> ye = dumpCardForOneSR(procs[p], kine, sr, lumi, dir);
      tye.first+=ye.first;
      tye.second+=ye.second;
      int ndec = 1;
      if (ye.first<1.    ||sqrt(ye.second)<1.) ndec = 2; 
      if (ye.first<0.01  ||sqrt(ye.second)<0.01) ndec = 3;
      if (ye.first<0.0005||sqrt(ye.second)<0.0005) ndec = 1;
      cout << Form("%s %6.*f %s %6.*f ",sep,ndec,ye.first,pm,ndec,sqrt(ye.second));
    }
    int ndec = 1;
    if (tye.first<1.    ||sqrt(tye.second)<1.) ndec = 2; 
    if (tye.first<0.01  ||sqrt(tye.second)<0.01) ndec = 3;
    if (tye.first<0.0005||sqrt(tye.second)<0.0005) ndec = 1;
    cout << Form("%s %6.*f %s %6.*f ",sep,ndec,tye.first,pm,ndec,sqrt(tye.second));
    float dye = dumpCardForOneSR(TString("data"), kine, sr, lumi, dir).first;
    cout << Form("%s %6.0f ",sep,dye);
    float sye = dumpCardForOneSR(TString("t1tttt_1200"), kine, sr, lumi, dir).first;
    cout << Form("%s %6.3f ",sep,sye);
    if (doLatex) cout << " \\\\ " << endl;
    else cout << endl;
    tLL[sr-1] = tye.first;
    eLL[sr-1] = sqrt(tye.second);
    nLL[sr-1] = ndec;
    dLL[sr-1] = dye;
  }
  if (doLatex) {
    cout << "\\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << "\\end{table}" << endl;
    cout << "\\end{landscape}" << endl;
  }

  if (doLatex) {
    cout << "\\begin{table}" << endl;
    cout << "\\footnotesize" << endl;
    cout << "\\caption{Event yields in signal regions.}" << endl;
    cout << "\\label{tab:yieldsSR}" << endl;
    cout << "\\begin{tabular}{c|cc|cc|cc}" << endl;
    cout << "\\hline" << endl;
    cout << "& \\multicolumn{2}{c|}{HH regions} & \\multicolumn{2}{c|}{HL regions} & \\multicolumn{2}{c}{LL regions} \\\\" << endl;
    cout << "\\hline" << endl;
    cout << "& Expected SM & Observed data  & Expected SM & Observed data  & Expected SM & Observed data \\\\" << endl;
    cout << "\\hline" << endl;
    for (int sr=0;sr<32;sr++) {
      if (sr<8) 
	cout << Form("SR%i  & %.*f $\\pm$ %.*f & %.0f  & %.*f $\\pm$ %.*f & %.0f &  %.*f $\\pm$  %.*f &  %.0f \\\\",
		     sr+1,nHH[sr],tHH[sr],nHH[sr],eHH[sr],dHH[sr],nHL[sr],tHL[sr],nHL[sr],eHL[sr],dHL[sr],nLL[sr],tLL[sr],nLL[sr],eLL[sr],dLL[sr]) 
	     << endl;
      else if (sr<26) 
	cout << Form("SR%i  & %.*f $\\pm$ %.*f & %.0f  & %.*f $\\pm$ %.*f & %.0f &  &   \\\\",
		     sr+1,nHH[sr],tHH[sr],nHH[sr],eHH[sr],dHH[sr],nHL[sr],tHL[sr],nHL[sr],eHL[sr],dHL[sr]) 
	     << endl;
      else
	cout << Form("SR%i  & %.*f $\\pm$ %.*f & %.0f  & & &  &  \\\\",
		     sr+1,nHH[sr],tHH[sr],nHH[sr],eHH[sr],dHH[sr]) 
	     << endl;
      if (sr==7 || sr==25) cout << "\\hline" << endl;
    }
    cout << "\\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << "\\end{table}" << endl;
    
    cout << "\\end{document}" << endl;
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
  float luminosity = pow(0.046,2);

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
      if (name.Contains("stat_")) continue;
      error += pow( h->GetBinContent(sr)-yield ,2);
      //cout << process << " " << h->GetBinContent(sr)-yield << endl;
      // if (process=="fakes" && sr==28) cout << process << " " << h->GetBinContent(sr)-yield << endl;
      // if (process=="fakes" && sr==29) cout << process << " " << h->GetBinContent(sr)-yield << endl;
    }
  }

  result.second = error;
  _file->Close();
  return result;
}
