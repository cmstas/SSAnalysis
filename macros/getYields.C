{

  // TFile *sync_file = TFile::Open("./synctest_histos.root");
  // cout << "sync BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31)
  //      << " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;
  // cout << "sync SR0I=" << hyp_hihi_sr->GetBinContent(1)+hyp_hihi_sr->GetBinContent(11)+hyp_hihi_sr->GetBinContent(21)+hyp_hihi_sr->GetBinContent(31)
  //      << " SR0E=" << hyp_hihi_sr->GetBinContent(1) << " SR10=" << hyp_hihi_sr->GetBinContent(11) << " SR20=" << hyp_hihi_sr->GetBinContent(21) << " SR30=" << hyp_hihi_sr->GetBinContent(31) << endl;
  // cout << "ee em mm : " << hyp_hihi_type->GetBinContent(4) << " " << hyp_hihi_type->GetBinContent(2)+hyp_hihi_type->GetBinContent(3) << " " << hyp_hihi_type->GetBinContent(1) << endl;
  // return;

  TString dir = "results_PHYS14";

  TFile *ttbar_file = TFile::Open(dir+"/ttbar_histos.root");
  cout << "ttbar BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31)
       << " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  TFile *TTWJets_file = TFile::Open(dir+"/TTWJets_histos.root");
  cout << "TTWJets BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31)
       << " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  TFile *TTZJets_file = TFile::Open(dir+"/TTZJets_histos.root");
  cout << "TTZJets BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31) 
       << " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  TFile *WZJets_file = TFile::Open(dir+"/WZJets_histos.root");
  cout << "WZJets BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31) 
       << " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  // TFile *WW_file = TFile::Open(dir+"/WW_histos.root");
  // cout << "WW BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31) 
  //<< " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  // TFile *WHZH_file = TFile::Open(dir+"/WHZH_histos.root");
  // cout << "WHZH BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31) 
  //<< " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  TFile *T1ttttG1200_file = TFile::Open(dir+"/T1ttttG1200_histos.root");
  cout << "T1ttttG1200 BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31) 
       << " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  TFile *T1ttttG1500_file = TFile::Open(dir+"/T1ttttG1500_histos.root");
  cout << "T1ttttG1500 BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31) 
       << " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  // TFile *T5Full1200_file = TFile::Open(dir+"/T5Full1200_histos.root");
  // cout << "T5Full1200 BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31) 
  //<< " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;

  // TFile *T5Full1500_file = TFile::Open(dir+"/T5Full1500_histos.root");
  // cout << "T5Full100 BR0I=" << hyp_hihi_br->GetBinContent(1)+hyp_hihi_br->GetBinContent(11)+hyp_hihi_br->GetBinContent(21)+hyp_hihi_br->GetBinContent(31) 
  //<< " BR0E=" << hyp_hihi_br->GetBinContent(1) << " BR10=" << hyp_hihi_br->GetBinContent(11) << " BR20=" << hyp_hihi_br->GetBinContent(21) << " BR30=" << hyp_hihi_br->GetBinContent(31) << endl;


  // TString dataset = "T1ttttG1500";
  // TFile *_file = TFile::Open(dir+"/"+dataset+"_histos.root");
  // TH1F* hsr = (TH1F*) _file->Get("hyp_hihimt_sr");
  // TH1F* hbr = (TH1F*) _file->Get("hyp_hihimt_br");
  // cout << "Dataset = " << dataset << endl
  //      << Form("BR0I=%.2f",hbr->GetBinContent(1)+hbr->GetBinContent(11)+hbr->GetBinContent(21))  << endl
  //      << Form("SR01=%.2f",hsr->GetBinContent(2)) << endl
  //      << Form("SR02=%.2f",hsr->GetBinContent(3)) << endl
  //      << Form("SR03=%.2f",hsr->GetBinContent(4)) << endl
  //      << Form("SR04=%.2f",hsr->GetBinContent(5)) << endl
  //      << Form("SR05=%.2f",hsr->GetBinContent(6)) << endl
  //      << Form("SR06=%.2f",hsr->GetBinContent(7)) << endl
  //      << Form("SR07=%.2f",hsr->GetBinContent(8)) << endl
  //      << Form("SR08=%.2f",hsr->GetBinContent(9)) << endl
  //      << Form("BR10=%.2f",hbr->GetBinContent(11))  << endl
  //      << Form("SR11=%.2f",hsr->GetBinContent(12)) << endl
  //      << Form("SR12=%.2f",hsr->GetBinContent(13)) << endl
  //      << Form("SR13=%.2f",hsr->GetBinContent(14)) << endl
  //      << Form("SR14=%.2f",hsr->GetBinContent(15)) << endl
  //      << Form("SR15=%.2f",hsr->GetBinContent(16)) << endl
  //      << Form("SR16=%.2f",hsr->GetBinContent(17)) << endl
  //      << Form("SR17=%.2f",hsr->GetBinContent(18)) << endl
  //      << Form("SR18=%.2f",hsr->GetBinContent(19)) << endl
  //      << Form("BR20=%.2f",hbr->GetBinContent(21))  << endl
  //      << Form("SR21=%.2f",hsr->GetBinContent(22)) << endl
  //      << Form("SR22=%.2f",hsr->GetBinContent(23)) << endl
  //      << Form("SR23=%.2f",hsr->GetBinContent(24)) << endl
  //      << Form("SR24=%.2f",hsr->GetBinContent(25)) << endl
  //      << Form("SR25=%.2f",hsr->GetBinContent(26)) << endl
  //      << Form("SR26=%.2f",hsr->GetBinContent(27)) << endl
  //      << Form("SR27=%.2f",hsr->GetBinContent(28)) << endl
  //      << Form("SR28=%.2f",hsr->GetBinContent(29)) << endl;
  // cout << Form("BR30=%.2f",hbr->GetBinContent(31))  << endl
  //      << Form("SR31=%.2f",hsr->GetBinContent(32)) << endl
  //      << Form("SR32=%.2f",hsr->GetBinContent(33)) << endl
  //      << Form("SR33=%.2f",hsr->GetBinContent(34)) << endl
  //      << Form("SR34=%.2f",hsr->GetBinContent(35)) << endl
  //      << Form("SR35=%.2f",hsr->GetBinContent(36)) << endl
  //      << Form("SR36=%.2f",hsr->GetBinContent(37)) << endl
  //      << Form("SR37=%.2f",hsr->GetBinContent(38)) << endl
  //      << Form("SR38=%.2f",hsr->GetBinContent(39)) << endl;


}
