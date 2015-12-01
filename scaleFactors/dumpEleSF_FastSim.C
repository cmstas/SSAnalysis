{

  TFile* f = TFile::Open("sf_el_tight_IDEmu_ISOEMu_ra5.root");

  TH3D* h_id = (TH3D*) f->Get("histo3D");

  cout << "float electronScaleFactorFastSim(float pt, float eta, int nvtx) {" << endl;
  cout << "  if (pt>=" << h_id->GetXaxis()->GetXmax() << ") pt = " << h_id->GetXaxis()->GetXmax()-0.1 << ";" << endl;
  cout << "  if (fabs(eta)>=" << h_id->GetYaxis()->GetXmax() << ") eta = " << h_id->GetYaxis()->GetXmax()-0.1 << ";" << endl;
  cout << "  if (nvtx>=" << h_id->GetZaxis()->GetXmax() << ") nvtx = " << h_id->GetZaxis()->GetXmax()-1 << ";" << endl;
  for (int xbin=1;xbin<=h_id->GetNbinsX();xbin++) {
    for (int ybin=1;ybin<=h_id->GetNbinsY();ybin++) {
      for (int zbin=1;zbin<=h_id->GetNbinsZ();zbin++) {
	  cout << "  if (pt>=" << h_id->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << h_id->GetXaxis()->GetBinUpEdge(xbin) 
	       << " && fabs(eta)>=" << h_id->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << h_id->GetYaxis()->GetBinUpEdge(ybin) 
	       << " && nvtx>=" << h_id->GetZaxis()->GetBinLowEdge(zbin) << " && nvtx<" << h_id->GetZaxis()->GetBinUpEdge(zbin) 
	       << " ) return " <<  h_id->GetBinContent(xbin,ybin,zbin) << ";" << endl;
      }
    }
  }
  cout << "   return 0.;" << endl;
  cout << "}" << endl;
  

}
