{

    // bool doPromptCorrected = true;
    bool doPromptCorrected = false;

    // for (int highHT = 0; highHT < 2; highHT++) {
    for (int highHT = -1; highHT < 2; highHT++) {
  for (int doMu=0;doMu<2;doMu++) {

    TString lep = doMu ? "muon" : "elec";
    TString l = doMu ? "mu" : "e";

    TString pfx = "";
    TString sfx = "";

    // TFile* f = TFile::Open("inSituFR_cone_FR_histos_data_normal.root");

    if(doPromptCorrected) pfx += "corr_";

    if(highHT == 1) { pfx = "highht_"; sfx = "HighHT"; }
    else if(highHT == 0) { pfx = "lowht_"; }
    else if(highHT == -1) { pfx = "all_"; sfx = "AllHT"; }

    TFile* f = TFile::Open(pfx+"inSituFR_cone_FR_histos.root");

    TH2D* numer = (TH2D*) f->Get(lep);
    
    cout << "float "+lep+"FakeRateInSitu"+sfx+"(float pt, float eta) {" << endl;
    for (int xbin=1;xbin<=numer->GetNbinsX();xbin++) {
      for (int ybin=1;ybin<=numer->GetNbinsY();ybin++) {
	if (xbin!=numer->GetNbinsX()) { 
	  cout << "   if (pt>=" << numer->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << numer->GetXaxis()->GetBinUpEdge(xbin) 
	       << " && fabs(eta)>=" << numer->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << numer->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	       <<  numer->GetBinContent(xbin,ybin) << ";" << endl;
	} else {
	  cout << "   if (pt>=" << numer->GetXaxis()->GetBinLowEdge(xbin)
	       << " && fabs(eta)>=" << numer->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << numer->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	       <<  numer->GetBinContent(xbin,ybin) << ";" << endl;      
	}
      }
    }
    cout << "   return 0.;" << endl;
    cout << "}" << endl;
    
    cout << "float "+lep+"FakeRateInSituError"+sfx+"(float pt, float eta) {" << endl;
    for (int xbin=1;xbin<=numer->GetNbinsX();xbin++) {
      for (int ybin=1;ybin<=numer->GetNbinsY();ybin++) {
	if (xbin!=numer->GetNbinsX()) { 
	  cout << "   if (pt>=" << numer->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << numer->GetXaxis()->GetBinUpEdge(xbin) 
	       << " && fabs(eta)>=" << numer->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << numer->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	       <<  numer->GetBinError(xbin,ybin) << ";" << endl;
	} else {
	  cout << "   if (pt>=" << numer->GetXaxis()->GetBinLowEdge(xbin)
	       << " && fabs(eta)>=" << numer->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << numer->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	       <<  numer->GetBinError(xbin,ybin) << ";" << endl;      
	}
      }
    }
    cout << "   return 0.;" << endl;
    cout << "}" << endl;

  } // doMu
    } // highHT

}
