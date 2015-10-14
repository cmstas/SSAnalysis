{

  for (int doMu=0;doMu<2;doMu++) {

    TString lep = doMu ? "muon" : "elec";
    TString l = doMu ? "mu" : "e";

    TFile* f = TFile::Open("inSituFR_cone_FR_histos_normal.root");

    TH2D* numer = (TH2D*) f->Get(lep);
    
    cout << "float "+lep+"FakeRate(float pt, float eta) {" << endl;
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
    
    cout << "float "+lep+"FakeRateError(float pt, float eta) {" << endl;
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

  }

}
