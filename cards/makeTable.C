pair<float, float> dumpCardForOneSR(TString process, TString kine, int sr, TString lumi, TString dir);

void makeTable(TString the_dir="", TString the_lumi="") {

  bool doLatex = true;

  char* sep = "&";
  char* pm  = "$\\pm$";
  if (!doLatex) {
    sep = "|";
    pm  = "+/-";
  }

  // TString lumi = "4.0";
  // TString dir = "v8.02_4p0_newbins";

  // TString lumi = "6.3";
  // TString dir = "v8.03";

  // TString lumi = "12.9";
  // TString lumi = "17.3";
  // TString lumi = "36.5";
  // TString lumi = "36.8";
  // TString lumi = "36.8";
  TString lumi = "35.9";
  // TString lumi = "27.2";
  // TString dir = "v8.04_properSF";
  // TString dir = "v8.04_July25";
  // TString dir = "v8.04_July28";
  // TString dir = "v8.04_t5qqqqdm20_July28";
  // TString dir = "v8.04_Nov5_17p3_qsplit";
  // TString dir = "v8.04_Nov16_agg";
  // TString dir = "v8.07_Dec1_17p3_rereco_agg";
  // TString dir = "v8.07_Dec2_17p3_rerecojetht_v1";
  // TString dir = "v8.07_Dec5_17p3_v2";
  // TString dir = "v8.07_Dec15_36p5_nobtagsf";
  // TString dir = "v9.02_Jan25_36p8_nobtagsf";
  // TString dir = "v9.03_Jan26_36p8_nobtagsf_LaurentSF";
  // TString dir = "v9.03_Jan26_36p8_nobtagsf_LaurentSF_agg";
  // TString dir = "v9.04_Jan31_36p8_blah2";
  TString dir = "v9.06_Mar6_35p9_reminiaodfilt_nomhiggs";
  // TString dir = "v9.04_Jan31_36p8_blah_exclagg";
  if (the_dir != "") dir = the_dir;
  if (the_lumi != "") lumi = the_lumi;

    const int nHHsr = 51;
    const int nHLsr = 41;
    const int nLLsr = 8;
    const int nInclAggsr = 15;

  // TString lumi = "0.8";
  // TString dir = "v8.02_800pb_unblind";

  TString procs[] = {"ttw","ttzh","wz","ww","xg","rares","flips","fakes"};

  float tHH[nHHsr] = {0.};
  float tHL[nHHsr] = {0.};
  float tLL[nHHsr] = {0.};
  float eHH[nHHsr] = {0.};
  float eHL[nHHsr] = {0.};
  float eLL[nHHsr] = {0.};
  int   nHH[nHHsr] = {0};
  int   nHL[nHHsr] = {0};
  int   nLL[nHHsr] = {0};
  float dHH[nHHsr] = {0.};
  float dHL[nHHsr] = {0.};
  float dLL[nHHsr] = {0.};

  if (doLatex) {
    cout << "\\documentclass[10pt,a4paper]{article}" << endl;
    cout << "\\usepackage{lscape}" << endl;
    cout << "\\begin{document}" << endl;
    cout << endl;
  }

  TString header = "     & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %10s \\\\";
  if (!doLatex) header = "     | %17s | %17s | %17s | %17s | %17s | %17s | %17s | %17s | %17s | %6s | %10s ";

  TString kine = "";

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
  kine = "hihi";
  for (int sr=1;sr<nHHsr+1;++sr) {
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
  for (int sr=1;sr<nHLsr+1;++sr) {
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
  for (int sr=1;sr<nLLsr+1;++sr) {
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

  cout << endl;
  cout << endl;

  if (doLatex) {
    cout << "\\begin{landscape}" << endl;
    cout << "\\begin{table}" << endl;
    cout << "\\footnotesize" << endl;
    cout << "\\caption{Event yields in inclusive aggregate regions.}" << endl;
    cout << "\\label{tab:yieldsInclAgg}" << endl;
    cout << "\\begin{tabular}{c|cccccccc|c|c|c}\\hline" << endl;
    cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
    cout << "\\hline\\hline" << endl;
  } else {
    cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
  }
  kine = "agg";
  for (int sr=1;sr<nInclAggsr+1;++sr) {
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
    for (int sr=0;sr<nHHsr;sr++) {
      if (sr<nLLsr) 
	cout << Form("SR%i  & %.*f $\\pm$ %.*f & %.0f  & %.*f $\\pm$ %.*f & %.0f &  %.*f $\\pm$  %.*f &  %.0f \\\\",
		     sr+1,nHH[sr],tHH[sr],nHH[sr],eHH[sr],dHH[sr],nHL[sr],tHL[sr],nHL[sr],eHL[sr],dHL[sr],nLL[sr],tLL[sr],nLL[sr],eLL[sr],dLL[sr]) 
	     << endl;
      else if (sr<nHLsr) 
	cout << Form("SR%i  & %.*f $\\pm$ %.*f & %.0f  & %.*f $\\pm$ %.*f & %.0f &  &   \\\\",
		     sr+1,nHH[sr],tHH[sr],nHH[sr],eHH[sr],dHH[sr],nHL[sr],tHL[sr],nHL[sr],eHL[sr],dHL[sr]) 
	     << endl;
      else
	cout << Form("SR%i  & %.*f $\\pm$ %.*f & %.0f  & & &  &  \\\\",
		     sr+1,nHH[sr],tHH[sr],nHH[sr],eHH[sr],dHH[sr]) 
	     << endl;
      if (sr==nLLsr-1 || sr==nHLsr-1) cout << "\\hline" << endl;
    }
    cout << "\\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << "\\end{table}" << endl;
    
    cout << "\\end{document}" << endl;
  }
}


pair<float, float> dumpCardForOneSR(TString process, TString kine, int sr, TString lumi, TString dir){

  TFile *_file = TFile::Open(Form("%s/%s_histos_%s_%sifb.root",dir.Data(),process.Data(),kine.Data(),lumi.Data()));

  pair<float, float> result;

  if (!_file) return make_pair(0.0,0.0);

  _file->cd();

  TH1F* nominal = (TH1F*) _file->Get("sr");
  if (kine=="hihi")   nominal->SetTitle(process+" SRHH L="+lumi+"/fb");
  if (kine=="hilow")  nominal->SetTitle(process+" SRHL L="+lumi+"/fb");
  if (kine=="lowlow") nominal->SetTitle(process+" SRLL L="+lumi+"/fb");


  float yield = nominal->GetBinContent(sr);
  result.first = yield;

  float ttw = pow(0.15,2);
  float ttz = pow(0.30,2);
  float ww  = pow(0.20,2);
  float wz  = pow(0.12,2)+pow(0.15,2);
  float fakes  = pow(0.30,2);
  float flips  = pow(0.20,2);
  float xg = pow(0.50,2);
  float rares = pow(0.50,2);
  float lepeff = pow(0.04,2)+pow(0.02,2);//both offline and hlt
  float luminosity = pow(0.026,2);

  float error = 0.;
  if (process=="ttw"  ) error = ttw+lepeff+luminosity;
  if (process=="ttzh" ) error = ttz;
  if (process=="ww"   ) error = ww+lepeff+luminosity;
  if (process=="wz"   ) error = wz;
  if (process=="fakes") error = fakes;
  if (process=="flips") error = flips;
  if (process=="rares") error = rares+lepeff+luminosity;
  if (process=="xg"   ) error = xg+lepeff+luminosity;

  error = error*yield*yield;



  // if (process=="ttw" && sr==1) cout << endl << yield << " " << sqrt(error) << endl;

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
      // if (name.Contains("_AH")) continue;
      if (TMath::IsNaN(h->GetBinContent(sr))) continue;
      error += pow( h->GetBinContent(sr)-yield ,2);
      //cout << process << " " << h->GetBinContent(sr)-yield << endl;
      // if (process=="ttw" && sr==1) cout << process << " " << name << " " << h->GetBinContent(sr)-yield << endl;
      // if (process=="fakes" && sr==29) cout << process << " " << h->GetBinContent(sr)-yield << endl;
    }
  }

  result.second = error;
  _file->Close();
  return result;
}
