{

  TFile* f = TFile::Open("kinematicBinSFele.root");

  TH2D* h_iso = (TH2D*) f->Get("MultiIsoTight_vs_AbsEta");
  TH2D* h_isoWemu = (TH2D*) f->Get("MultiIsoTight_and_IsoEmu_vs_AbsEta");
  TH2D* h_conv = (TH2D*) f->Get("ConvVeto_and_MissInnerHits0_and_ChgConsistent_vs_AbsEta");
  TH2D* h_id = (TH2D*) f->Get("MVATight_and_IDEmu_and_TightIP2D_and_TightIP3D");

  cout << "float electronScaleFactorHighHT(float pt, float eta) {" << endl;
  for (int xbin=1;xbin<=h_id->GetNbinsX();xbin++) {
    for (int ybin=1;ybin<=h_id->GetNbinsY();ybin++) {
      if (xbin!=h_id->GetNbinsX()) { 
	cout << "   if (pt>=" << h_id->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << h_id->GetXaxis()->GetBinUpEdge(xbin) 
	     << " && fabs(eta)>=" << h_id->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << h_id->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	     <<  h_id->GetBinContent(xbin,ybin)*h_conv->GetBinContent(xbin,ybin)*h_iso->GetBinContent(xbin,ybin) << ";" << endl;
      } else {
	cout << "   if (pt>=" << h_id->GetXaxis()->GetBinLowEdge(xbin)
	     << " && fabs(eta)>=" << h_id->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << h_id->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	     <<  h_id->GetBinContent(xbin,ybin)*h_conv->GetBinContent(xbin,ybin)*h_iso->GetBinContent(xbin,ybin) << ";" << endl;      
      }
    }
  }
  cout << "   return 0.;" << endl;
  cout << "}" << endl;

  cout << "float electronScaleFactorLowHT(float pt, float eta) {" << endl;
  for (int xbin=1;xbin<=h_id->GetNbinsX();xbin++) {
    for (int ybin=1;ybin<=h_id->GetNbinsY();ybin++) {
      if (xbin!=h_id->GetNbinsX()) { 
	cout << "   if (pt>=" << h_id->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << h_id->GetXaxis()->GetBinUpEdge(xbin) 
	     << " && fabs(eta)>=" << h_id->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << h_id->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	     <<  h_id->GetBinContent(xbin,ybin)*h_conv->GetBinContent(xbin,ybin)*h_isoWemu->GetBinContent(xbin,ybin) << ";" << endl;
      } else {
	cout << "   if (pt>=" << h_id->GetXaxis()->GetBinLowEdge(xbin)
	     << " && fabs(eta)>=" << h_id->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << h_id->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	     <<  h_id->GetBinContent(xbin,ybin)*h_conv->GetBinContent(xbin,ybin)*h_isoWemu->GetBinContent(xbin,ybin) << ";" << endl;      
      }
    }
  }
  cout << "   return 0.;" << endl;
  cout << "}" << endl;
  

}
