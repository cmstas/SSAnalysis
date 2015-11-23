{

  TFile* f_id = TFile::Open("TnP_MuonID_NUM_MediumID_DENOM_generalTracks_VAR_map_pt_eta.root");
  TH2F* h_id = (TH2F*) f_id->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_tag_IsoMu20_pass");

  TFile* f_ip2 = TFile::Open("TnP_MuonID_NUM_TightIP2D_DENOM_LooseID_VAR_map_pt_eta.root");
  TH2F* h_ip2 = (TH2F*) f_ip2->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_PF_pass_&_tag_IsoMu20_pass");

  TFile* f_ip3 = TFile::Open("TnP_MuonID_NUM_TightIP3D_DENOM_LooseID_VAR_map_pt_eta.root");
  TH2F* h_ip3 = (TH2F*) f_ip3->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_PF_pass_&_tag_IsoMu20_pass");

  TFile* f_iso = TFile::Open("TnP_MuonID_NUM_MultiIsoMedium_DENOM_MediumID_VAR_map_pt_eta.root");
  TH2F* h_iso = (TH2F*) f_iso->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_Medium_pass_&_tag_IsoMu20_pass");


  cout << "float muonScaleFactor(float pt, float eta) {" << endl;
  for (int xbin=1;xbin<=h_id->GetNbinsX();xbin++) {
    for (int ybin=1;ybin<=h_id->GetNbinsY();ybin++) {
      if (xbin!=h_id->GetNbinsX()) { 
	cout << "   if (pt>=" << h_id->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << h_id->GetXaxis()->GetBinUpEdge(xbin) 
	     << " && fabs(eta)>=" << h_id->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << h_id->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	     <<  h_id->GetBinContent(xbin,ybin)*h_ip2->GetBinContent(xbin,ybin)*h_ip3->GetBinContent(xbin,ybin)*h_iso->GetBinContent(xbin,ybin) << ";" << endl;
      } else {
	cout << "   if (pt>=" << h_id->GetXaxis()->GetBinLowEdge(xbin)
	     << " && fabs(eta)>=" << h_id->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << h_id->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
	     <<  h_id->GetBinContent(xbin,ybin)*h_ip2->GetBinContent(xbin,ybin)*h_ip3->GetBinContent(xbin,ybin)*h_iso->GetBinContent(xbin,ybin) << ";" << endl;      
      }
    }
  }
  cout << "   return 0.;" << endl;
  cout << "}" << endl;
  

}
