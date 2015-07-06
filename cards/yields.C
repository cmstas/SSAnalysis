#include "../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../software/tableMaker/CTable.h"
#include "../CORE/SSSelections.h"
#include "SS.h"
#include "TH2D.h"

//Tables on/off
bool makeTables = 1;

//Root files on/off
bool makeRootFiles = 1;

//Lumi
float lumi = 10.0;//3.0;

struct results_t { TH1F* hh; TH1F* hl; TH1F* ll; }; 

float getYield(results_t results, int bin, int which){
  if (which == 0) return results.hh->Integral(bin, bin); 
  if (which == 1) return results.hl->Integral(bin, bin); 
  if (which == 2) return results.ll->Integral(bin, bin); 
  return -1;
}

void writeStatUpDown(TH1F* central,string name,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* statUpDown = new TH1F(Form("%s_stat%s",name.c_str(),updown.Data()),
			      Form("%s_stat%s",name.c_str(),updown.Data()),
			      central->GetNbinsX(),central->GetXaxis()->GetXmin(),central->GetXaxis()->GetXmax());
  for (int bin=1;bin<=statUpDown->GetNbinsX();++bin) {
    float val = down ? (central->GetBinContent(bin)-central->GetBinError(bin)) : (central->GetBinContent(bin)+central->GetBinError(bin));
    if (val>0) statUpDown->SetBinContent(bin,val);
    else statUpDown->SetBinContent(bin,1E-6);
  }
  statUpDown->Write();
}

void writeTTVExtrSyst(TH1F* central,string name,TString kine,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* systUpDown = (TH1F*) central->Clone(Form("%s_extr%s",name.c_str(),updown.Data()));
  systUpDown->SetTitle(Form("%s_extr%s",name.c_str(),updown.Data()));
  float systValue = 1.25;//25%
  for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
    if (kine.Contains("hihi")) {
      if (bin!=4 && bin!=6 && bin!=8 && bin!=12 && bin!=14 && bin!=16 && bin!=20 && bin!=22 && bin!=24 && 
	  bin!=26 && bin!=28 && bin!=30 && bin!=31 && bin!=32) continue;
    }
    if (kine.Contains("hilow")) {
      if (bin!=4 && bin!=6 && bin!=10 && bin!=12 && bin!=16 && bin!=18 && bin!=20 && bin!=22 && bin!=24 && 
	  bin!=25 && bin!=26) continue;
    }
    if (kine.Contains("lowlow")) {
      systValue = 1.20;//20%
    }
    float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
    if (val>0) systUpDown->SetBinContent(bin,val);
  }
  systUpDown->Write();
}

void writeWZExtrSyst(TH1F* central,string name,TString kine,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* systUpDown = (TH1F*) central->Clone(Form("%s_extr%s",name.c_str(),updown.Data()));
  systUpDown->SetTitle(Form("%s_extr%s",name.c_str(),updown.Data()));
  float systValue = 1.15;//15%
  for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
    if (kine.Contains("hihi")) {
      if (bin<9) continue;
    }
    if (kine.Contains("hilow")) {
      if (bin<7) continue;
    }
    if (kine.Contains("lowlow")) {
      if (bin<3) continue;
    }
    float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
    if (val>0) systUpDown->SetBinContent(bin,val);
  }
  systUpDown->Write();
}

void writeJesSyst(TH1F* central,string name,TString kine,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* systUpDown = (TH1F*) central->Clone(Form("jes%s",updown.Data()));
  systUpDown->SetTitle(Form("jes%s",updown.Data()));
  float systValue = 1.;

  //TTV
  if (TString(name)=="ttw" || TString(name)=="ttz") {
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      if (kine.Contains("hihi")) {
	if (bin==1 || bin==3 || bin==9 || bin==11 || bin==17 || bin==19 || bin==25 || bin==27 || bin==29) systValue=1.05;
	if (bin==2 || bin==5 || bin==7 || bin==10 || bin==13 || bin==15 || bin==18 || bin==21 || bin==23 || 
	    bin==26 || bin==28 || bin==30 || bin==31 || bin==32) systValue=1.08;
	if (bin==4 || bin==6 || bin==8 || bin==12 || bin==14 || bin==16 || bin==20 || bin==22 || bin==24) systValue=1.04;
      }
      if (kine.Contains("hilow")) {
	if (bin==1 || bin==3 || bin==7 || bin==9 || bin==13 || bin==15 || bin==19 || bin==21 || bin==23) systValue=1.03;
	else systValue = 1.08;
      }
      if (kine.Contains("lowlow")) {
	systValue = 1.15;
      }
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="t1tttt_1500") {
    systValue = 1.06;
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="t1tttt_1200") {
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      if (kine.Contains("hihi")) {
	if (bin==20 || bin==26 || bin==28 || bin==30) systValue = 1.03;
	else systValue = 1.10;
      }
      if (kine.Contains("hilow")) {
	if (bin==16 || bin==22 || bin==24) systValue = 1.03;
	else systValue = 1.05;
      } else systValue = 1.00;
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="t1ttbbww_1000" || TString(name)=="t1ttbbww_1300") {
    systValue = 1.08;
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="t5tttt_1000") {
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      if (kine.Contains("hihi")) {
	if (bin==1 || bin==3 || bin==9 || bin==11 || bin==17 || bin==19 || bin==25 || bin==27 || bin==29) systValue = 1.05;
	else if (bin==31) systValue = 1.10;
	else systValue = 1.03;
      }
      if (kine.Contains("hilow")) {
	if (bin==1 || bin==3 || bin==7 || bin==9 || bin==13 || bin==15 || bin==19 || bin==21 || bin==23) systValue = 1.05;
	else systValue = 1.10;
      }
      if (kine.Contains("lowlow")) {
	systValue = 1.10;
      }
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="t5qqww_1200") {
    if (kine.Contains("hihi")) {
      systValue = 1.07;
    }
    if (kine.Contains("hilow")) {
      systValue = 1.10;
    }
    if (kine.Contains("lowlow")) {
      systValue = 1.20;
    }
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="t5qqww_1500") {
    systValue = 1.05;
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="t5qqww_deg") {
    if (kine.Contains("hihi")) {
      systValue = 1.15;
    }
    if (kine.Contains("hilow")) {
      systValue = 1.10;
    }
    if (kine.Contains("lowlow")) {
      systValue = 1.02;
    }
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  if (TString(name)=="t6ttww_600" || TString(name)=="t6ttww_650") {
    systValue = 1.10;
    for (int bin=1;bin<=systUpDown->GetNbinsX();++bin) {
      float val = down ? (central->GetBinContent(bin)/systValue) : (central->GetBinContent(bin)*systValue);
      if (val>0) systUpDown->SetBinContent(bin,val);
    }
  }

  systUpDown->Write();
}

results_t run(TChain* chain, string name, hyp_type_t flavor = UNASSIGNED, est_type_t est_type = MCSS){

  //Declare hists
  TH1F *HighHighPlot = new TH1F(Form("%s_hh", name.c_str()), Form("%s_hh", name.c_str()), 32, 1, 33); 
  TH1F *HighLowPlot  = new TH1F(Form("%s_hl", name.c_str()), Form("%s_hl", name.c_str()), 26, 1, 27); 
  TH1F *LowLowPlot   = new TH1F(Form("%s_ll", name.c_str()), Form("%s_ll", name.c_str()),  8, 1,  9); 
  HighHighPlot->Sumw2();
  HighLowPlot->Sumw2();
  LowLowPlot->Sumw2();

  //FR files
  TFile* f_fr_mu = 0;
  TH2D*  h_fr_mu = 0;
  TFile* f_fr_el = 0;
  TH2D*  h_fr_el = 0;
  if (est_type == SFAKEDD || est_type == DFAKEDD) {
    f_fr_mu = TFile::Open("../FakeRate/measurement_region/rate_histos_qcd.root");
    h_fr_mu = (TH2D*) f_fr_mu->Get("rate_cone_histo_mu");
    f_fr_el = TFile::Open("../FakeRate/measurement_region/rate_histos_qcd_LooseEMVA.root");
    h_fr_el = (TH2D*) f_fr_el->Get("rate_cone_histo_e");
  }

  //Event Counting
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      samesign.GetEntry(event);
      nEventsTotal++;
    
      //Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      float lep1_pT = ss::lep1_p4().pt();
      float lep2_pT = ss::lep2_p4().pt();

      //fixme bug in cross section
      float fixit = 1.;
      if (name=="t1ttbbww_1000") fixit=0.325388 /0.0856418;
      if (name=="t1ttbbww_1300") fixit=0.0460525/0.0856418;
      if (name=="t5tttt_1000"  ) fixit=0.325388 /0.0856418;
      if (name=="t6ttww_600"   ) fixit=0.174599 /0.0856418;
      if (name=="t6ttww_650"   ) fixit=0.107045 /0.0856418;

      float evt_weight = 1.;
      //Reject non-SS
      if (est_type == SFAKEDD) {
	if (ss::hyp_class() != 2) continue;
	if (fabs(ss::lep1_ip3d()/ss::lep1_ip3d_err())>4.) continue;
	if (fabs(ss::lep2_ip3d()/ss::lep2_ip3d_err())>4.) continue;
	//cone correction
	if (abs(ss::lep1_id())==11) {
	  if (ss::lep1_ptrel_v1()>7.0) lep1_pT = ss::lep1_p4().pt()*(1+std::max(0.,ss::lep1_miniIso()-0.10));
	  else lep1_pT = std::max(ss::lep1_p4().pt(),ss::jet_close_lep1().pt()*float(0.70));
	} else {
	  if (ss::lep1_ptrel_v1()>6.7) lep1_pT = ss::lep1_p4().pt()*(1+std::max(0.,ss::lep1_miniIso()-0.14));
	  else lep1_pT = std::max(ss::lep1_p4().pt(),ss::jet_close_lep1().pt()*float(0.68));
	}
	if (abs(ss::lep2_id())==11) {
	  if (ss::lep2_ptrel_v1()>7.0) lep2_pT = ss::lep2_p4().pt()*(1+std::max(0.,ss::lep2_miniIso()-0.10));
	  else lep2_pT = std::max(ss::lep2_p4().pt(),ss::jet_close_lep2().pt()*float(0.70));
	} else {
	  if (ss::lep2_ptrel_v1()>6.7) lep2_pT = ss::lep2_p4().pt()*(1+std::max(0.,ss::lep2_miniIso()-0.14));
	  else lep2_pT = std::max(ss::lep2_p4().pt(),ss::jet_close_lep2().pt()*float(0.68));
	}
	//now get fr
	float fr = 0;
	if (ss::lep1_passes_id() && ss::lep1_motherID()==1 && ss::lep2_motherID()<1) {//fixme remove MC truth and add spillage
	  //lep2 is FO
	  if (abs(ss::lep2_id())==13) {
	    fr = h_fr_mu->GetBinContent(h_fr_mu->FindBin(std::min(lep2_pT,float(69.)),std::min(fabs(ss::lep2_p4().eta()),float(2.39))));
	  } else {
	    fr = h_fr_el->GetBinContent(h_fr_el->FindBin(std::min(lep2_pT,float(69.)),std::min(fabs(ss::lep2_p4().eta()),float(2.39))));
	  }
	} else if (ss::lep2_passes_id() && ss::lep2_motherID()==1 && ss::lep1_motherID()<1) {
	  //lep1 is FO
	  if (abs(ss::lep1_id())==13) {
	    fr = h_fr_mu->GetBinContent(h_fr_mu->FindBin(std::min(lep1_pT,float(69.)),std::min(fabs(ss::lep1_p4().eta()),float(2.39))));
	  } else {
	    fr = h_fr_el->GetBinContent(h_fr_el->FindBin(std::min(lep1_pT,float(69.)),std::min(fabs(ss::lep1_p4().eta()),float(2.39))));
	  }
	} else fr = 0.;
	//compute weight
	evt_weight = fr/(1.-fr);
      } else {
	if (ss::hyp_class() != 3) continue;
      } 

      //Flavor selection
      if (flavor == MM && ss::hyp_type() != 3) continue;
      if (flavor == EM && ss::hyp_type() != 1 && ss::hyp_type() != 2) continue;
      if (flavor == EE && ss::hyp_type() != 0) continue;
   
      //Calculate mtmin
      float mtmin = ss::mt() > ss::mt_l2() ? ss::mt_l2() : ss::mt(); //fixme
 
      //Figure out region, fill plot
      anal_type_t categ = analysisCategory(lep1_pT, lep2_pT);  
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, lep1_pT, lep2_pT);

      if (est_type == MCSS    && (ss::lep1_motherID()!=1 || ss::lep2_motherID()!=1) && TString(name)!="t5qqww_deg" ) continue;
      if (est_type == FLIPMC  && (ss::lep1_motherID()!=2 && ss::lep2_motherID()!=2) ) continue;
      if (est_type == SFAKEMC && !( (ss::lep1_motherID()<1 && ss::lep2_motherID()==1) || (ss::lep1_motherID()==1 && ss::lep2_motherID()<1) ) ) continue;
      if (est_type == DFAKEMC && (ss::lep1_motherID()>=1 || ss::lep2_motherID()>=1 ) ) continue;

      if (SR > 0 && categ == HighHigh) HighHighPlot->Fill(SR, ss::scale1fb()*lumi*evt_weight*fixit);
      if (SR > 0 && categ == HighLow)  HighLowPlot ->Fill(SR, ss::scale1fb()*lumi*evt_weight*fixit);
      if (SR > 0 && categ == LowLow)   LowLowPlot  ->Fill(SR, ss::scale1fb()*lumi*evt_weight*fixit);

    }//event loop
  }//file loop

  if (est_type == SFAKEDD || est_type == DFAKEDD) {
    f_fr_mu->Close();
    f_fr_el->Close();
  }

  //Make root files
  if (makeRootFiles && flavor==UNASSIGNED) {

    TString kinRegs[] = {"hihi","hilow","lowlow"};

    for (int kr = 0; kr<3;kr++) {

      TFile *fileOut = TFile::Open(Form("./gc.v1.24/%s_histos_%s_%.1fifb.root",name.c_str(),kinRegs[kr].Data(),lumi),"RECREATE");
      TH1F* plot = 0;
      if (kinRegs[kr]=="hihi") plot=HighHighPlot;
      else if (kinRegs[kr]=="hilow") plot=HighLowPlot;
      else if (kinRegs[kr]=="lowlow") plot=LowLowPlot;
      else exit(1);
      TH1F* h_sr = (TH1F*) plot->Clone("sr");
      h_sr->Write();
      
      //now do systematics
      
      //mc stats
      writeStatUpDown(h_sr,name,0);
      writeStatUpDown(h_sr,name,1);
      
      //ttv
      if (name=="ttw" || name=="ttz") {
	writeTTVExtrSyst(h_sr,name,kinRegs[kr],0);
	writeTTVExtrSyst(h_sr,name,kinRegs[kr],1);
      }
      
      //ttv
      if (name=="wz") {
	writeWZExtrSyst(h_sr,name,kinRegs[kr],0);
	writeWZExtrSyst(h_sr,name,kinRegs[kr],1);
      }
      
      //jes
      if (name=="ttw"           || name=="ttz"           || 
	  name=="t1tttt_1200"   || name=="t1tttt_1500"   || 
	  name=="t5qqww_1200"   || name=="t5qqww_deg"    ||
	  name=="t1ttbbww_1000" || name=="t1ttbbww_1300" ||
	  name=="t6ttww_600"    || name=="t6ttww_650"    ||
	  name=="t5tttt_1000" ) {
	writeJesSyst(h_sr,name,kinRegs[kr],0);
	writeJesSyst(h_sr,name,kinRegs[kr],1);
      }
      
      //end systematics
      
      fileOut->Close();
    }
    
  }

  //Return hists
  results_t result; 
  result.hh = HighHighPlot;
  result.hl = HighLowPlot;
  result.ll = LowLowPlot;
  return result;

}

void yields(){

  //Make chains
  TChain* ttbar         = new TChain("t");
  TChain* ttw           = new TChain("t");
  TChain* ttz           = new TChain("t");
  TChain* wz            = new TChain("t");
  TChain* wjets         = new TChain("t");
  TChain* dy            = new TChain("t");

  TChain* t1tttt_1200   = new TChain("t");
  TChain* t1tttt_1500   = new TChain("t");
  TChain* t1ttbbww_1000 = new TChain("t");
  TChain* t1ttbbww_1300 = new TChain("t");
  TChain* t5tttt_1000   = new TChain("t");
  TChain* t6ttww_600    = new TChain("t");
  TChain* t6ttww_650    = new TChain("t");
  TChain* t5qqww_1200   = new TChain("t");
  TChain* t5qqww_1500   = new TChain("t");
  TChain* t5qqww_deg    = new TChain("t");
 
  //Fill chains
  //backgrounds
  ttbar      ->Add("../babies/v1.26/TTBAR_0.root"                    );
  ttw        ->Add("../babies/v1.26/TTW_0.root"                      );
  ttz        ->Add("../babies/v1.26/TTZ_0.root"                      );
  wz         ->Add("../babies/v1.26/WZ_0.root"                       );
  wjets      ->Add("../babies/v1.26/WJets1_0.root"                   );
  wjets      ->Add("../babies/v1.26/WJets2_0.root"                   );
  wjets      ->Add("../babies/v1.26/WJets3_0.root"                   );
  wjets      ->Add("../babies/v1.26/WJets4_0.root"                   );
  dy         ->Add("../babies/v1.26/DY1_0.root"                      );
  dy         ->Add("../babies/v1.26/DY2_0.root"                      );
  dy         ->Add("../babies/v1.26/DY3_0.root"                      );
  dy         ->Add("../babies/v1.26/DY4_0.root"                      );
  //signals
  t1tttt_1200  ->Add("../babies/v1.26/t1tttt_1200_1.root"                                              );
  t1tttt_1500  ->Add("../babies/v1.26/t1tttt_1500_1.root"                                              );
  t1ttbbww_1000->Add("../babies/v1.26/T1ttbbWW_2J_mGo1000_mCh725_mChi720_3bodydec_v2_baby.root"        );
  t1ttbbww_1300->Add("../babies/v1.26/T1ttbbWW_2J_mGo1300_mCh300_mChi290_3bodydec_v2_baby.root"        );
  t5tttt_1000  ->Add("../babies/v1.26/T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_23bodydec_v2_baby.root");
  t6ttww_600   ->Add("../babies/v1.26/T6ttWW_600_425_50_v2_baby.root"                                  );
  t6ttww_650   ->Add("../babies/v1.26/T6ttWW_650_150_50_v2_baby.root"                                  );
  t5qqww_1200  ->Add("../babies/v1.26/t5qqqqWW_1200_1000_800_1.root"                                   );
  t5qqww_1500  ->Add("../babies/v1.26/T5Full_Gl1500_Chi800_LSP100_baby.root"                           );
  t5qqww_deg   ->Add("../babies/v1.26/t5qqqqWW_deg_1000_315_300_1.root"                                );

  //Chains for type
  TChain* all_bkgd = new TChain("t");
  all_bkgd->Add(ttbar);
  all_bkgd->Add(ttw);
  all_bkgd->Add(ttz);
  all_bkgd->Add(wz);
  all_bkgd->Add(wjets);
  all_bkgd->Add(dy);

  //Make the histograms from MC processes
  results_t ttw_graphs         = run(ttw        , "ttw"        ,UNASSIGNED,MCSS);
  results_t ttz_graphs         = run(ttz        , "ttz"        ,UNASSIGNED,MCSS);
  results_t wz_graphs          = run(wz         , "wz"         ,UNASSIGNED,MCSS);
  results_t t1tttt_1200_graphs = run(t1tttt_1200, "t1tttt_1200",UNASSIGNED,MCSS);
  results_t t1tttt_1500_graphs = run(t1tttt_1500, "t1tttt_1500",UNASSIGNED,MCSS);
  results_t t1ttbbww_1000_graphs = run(t1ttbbww_1000, "t1ttbbww_1000",UNASSIGNED,MCSS);
  results_t t1ttbbww_1300_graphs = run(t1ttbbww_1300, "t1ttbbww_1300",UNASSIGNED,MCSS);
  results_t t5tttt_1000_graphs = run(t5tttt_1000, "t5tttt_1000",UNASSIGNED,MCSS);
  results_t t6ttww_600_graphs = run(t6ttww_600, "t6ttww_600",UNASSIGNED,MCSS);
  results_t t6ttww_650_graphs = run(t6ttww_650, "t6ttww_650",UNASSIGNED,MCSS);
  results_t t5qqww_1200_graphs = run(t5qqww_1200, "t5qqww_1200",UNASSIGNED,MCSS);
  results_t t5qqww_1500_graphs = run(t5qqww_1500, "t5qqww_1500",UNASSIGNED,MCSS);
  results_t t5qqww_deg_graphs  = run(t5qqww_deg , "t5qqww_deg" ,UNASSIGNED,MCSS);

  //mc predictions
  results_t flips_mc_graphs    = run(all_bkgd   , "flips_mc"   ,UNASSIGNED,FLIPMC);
  results_t sfakes_mc_graphs   = run(all_bkgd   , "sfakes_mc"  ,UNASSIGNED,SFAKEMC);
  results_t dfakes_mc_graphs   = run(all_bkgd   , "dfakes_mc"  ,UNASSIGNED,DFAKEMC);

  // results_t ttbar_graphs       = run(ttbar      , "ttbar"      ,UNASSIGNED,MCSS);
  // results_t wjets_graphs       = run(wjets      , "wjets"      ,UNASSIGNED,MCSS);
  // results_t dy_graphs          = run(dy         , "dy"         ,UNASSIGNED,MCSS);
  // results_t ttbarAll_graphs    = run(ttbar      , "ttbarAll"   ,UNASSIGNED,ALLMC);

  //data driven prediction
  results_t sfakes_dd_graphs   = run(all_bkgd   , "sfakes_dd"  ,UNASSIGNED,SFAKEDD);

  //pseudo observation
  results_t pdata_graphs       = run(all_bkgd   , "pdata"      ,UNASSIGNED,ALLMC);

  //Make the type histograms
  results_t bkgd_graphs_ee  = run(all_bkgd, "bkgd_ee", EE,MCSS);
  results_t bkgd_graphs_em  = run(all_bkgd, "bkgd_em", EM,MCSS);
  results_t bkgd_graphs_mm  = run(all_bkgd, "bkgd_mm", MM,MCSS);

  //Prepare for plots -- null
  TH1F* null = new TH1F("","",1,0,1);

  //Prepare for plots -- backgrounds, sample
  vector <TH1F*> background_high; 
  background_high.push_back(sfakes_dd_graphs.hh);
  background_high.push_back(dfakes_mc_graphs.hh);
  background_high.push_back(flips_mc_graphs.hh);
  background_high.push_back(ttw_graphs.hh);
  background_high.push_back(ttz_graphs.hh);
  background_high.push_back(wz_graphs.hh);
  vector <TH1F*> background_hl; 
  background_hl.push_back(sfakes_dd_graphs.hl);
  background_hl.push_back(dfakes_mc_graphs.hl);
  background_hl.push_back(flips_mc_graphs.hl);
  background_hl.push_back(ttw_graphs.hl);
  background_hl.push_back(ttz_graphs.hl);
  background_hl.push_back(wz_graphs.hl);
  vector <TH1F*> background_low; 
  background_low.push_back(sfakes_dd_graphs.ll);
  background_low.push_back(dfakes_mc_graphs.ll);
  background_low.push_back(flips_mc_graphs.ll);
  background_low.push_back(ttw_graphs.ll);
  background_low.push_back(ttz_graphs.ll);
  background_low.push_back(wz_graphs.ll);

  //Prepare for plots -- backgrounds, type
  vector <TH1F*> background_type_high; 
  background_type_high.push_back(bkgd_graphs_ee.hh);
  background_type_high.push_back(bkgd_graphs_em.hh);
  background_type_high.push_back(bkgd_graphs_mm.hh);
  vector <TH1F*> background_type_hl; 
  background_type_hl.push_back(bkgd_graphs_ee.hl);
  background_type_hl.push_back(bkgd_graphs_em.hl);
  background_type_hl.push_back(bkgd_graphs_mm.hl);
  vector <TH1F*> background_type_low; 
  background_type_low.push_back(bkgd_graphs_ee.ll);
  background_type_low.push_back(bkgd_graphs_em.ll);
  background_type_low.push_back(bkgd_graphs_mm.ll);

  //Prepare for plots -- signals, sample
  vector <TH1F*> signal_high; 
  signal_high.push_back(t1tttt_1200_graphs.hh);
  signal_high.push_back(t1tttt_1500_graphs.hh);
  signal_high.push_back(t5qqww_1200_graphs.hh);
  signal_high.push_back(t5qqww_deg_graphs.hh);
  vector <TH1F*> signal_hl; 
  signal_hl.push_back(t1tttt_1200_graphs.hl);
  signal_hl.push_back(t1tttt_1500_graphs.hl);
  signal_hl.push_back(t5qqww_1200_graphs.hl);
  signal_hl.push_back(t5qqww_deg_graphs.hl);
  vector <TH1F*> signal_low; 
  signal_low.push_back(t1tttt_1200_graphs.ll);
  signal_low.push_back(t1tttt_1500_graphs.ll);
  signal_low.push_back(t5qqww_1200_graphs.ll);
  signal_low.push_back(t5qqww_deg_graphs.ll);

  //Prepare for plots -- background titles
  vector <string> titles;
  titles.push_back("sfakes_dd");
  titles.push_back("dfakes_mc");
  titles.push_back("flips_mc");
  titles.push_back("ttw");
  titles.push_back("ttz");
  titles.push_back("wz");

  //Prepare for plots -- background titles (type)
  vector <string> typetitles;
  typetitles.push_back("ee");
  typetitles.push_back("e#mu");
  typetitles.push_back("#mu#mu");

  //Prepare for plots -- signal titles
  vector <string> signal_titles;
  signal_titles.push_back("t1tttt (1.2, 0.8)");
  signal_titles.push_back("t1tttt (1.5, 0.1)");
  signal_titles.push_back("t5qqww (1.2, 1.0, 0.8)");
  signal_titles.push_back("t5qqww (1.0, 0.315, 0.3)");

  //Make plots -- sample
  dataMCplotMaker(null, background_high, titles, "H-H", "", Form("--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields_%.1fifb --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.1f --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear",lumi,lumi)); 
  dataMCplotMaker(null, background_high, titles, "H-H", "", Form("--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields_log_%.1fifb --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.1f --nDivisions 210 --legendRight -0.00 --noXaxisUnit --setMaximum 500 --setMinimum 0.01  --legendTextSize 0.0325",lumi,lumi)); 
  dataMCplotMaker(null, background_hl  , titles, "H-L", "", Form("--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName hl_yields_%.1fifb --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.1f --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear",lumi,lumi)); 
  dataMCplotMaker(null, background_hl  , titles, "H-L", "", Form("--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName hl_yields_log_%.1fifb --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.1f --nDivisions 210 --legendRight -0.00 --noXaxisUnit --setMaximum 800 --setMinimum 0.01  --legendTextSize 0.0325",lumi,lumi)); 

  dataMCplotMaker(null, signal_high, signal_titles, "H-H", "", Form("--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields_s_%.1fifb --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.1f --legendRight -0.12 --noXaxisUnit  --legendTextSize 0.0325 --noStack --nDivisions 210 --isLinear",lumi,lumi)); 
  dataMCplotMaker(null, signal_hl  , signal_titles, "H-L", "", Form("--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName hl_yields_s_%.1fifb --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.1f --legendRight -0.12 --noXaxisUnit  --legendTextSize 0.03 --noStack  --nDivisions 210 --isLinear",lumi,lumi)); 

  //Make plots -- type
  dataMCplotMaker(null, background_type_high, typetitles, "H-H", "", Form("--vLine 9 --vLine 17 --vLine 25 --vLine 31 --outputName high_yields_t_%.1fifb --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.1f --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear",lumi,lumi)); 
  dataMCplotMaker(null, background_type_hl  , typetitles, "H-L", "", Form("--vLine 7 --vLine 13 --vLine 19 --vLine 23 --vLine 25 --outputName hl_yields_t_%.1fifb --noDivisionLabel --xAxisLabel SR --energy 13 --lumi %.1f --nDivisions 210 --legendRight -0.00 --noXaxisUnit  --legendTextSize 0.0325 --isLinear",lumi,lumi)); 

  //Make tables
  if (!makeTables) return; 
  int nSRs[3] = { 32, 26, 8 }; 

  //backgrounds
  for (int j = 0; j < 3; j++){
    CTable table;
    table.setTable() 
      ("", "ttW", "ttZ", "WZ", "SingleFakes", /*"SingleFakesPred",*/ "DoubleFakes", "ChargeFlips", "PseudoData" ); 
    table.setPrecision(1); 
    if (j == 0) table.setTitle("H-H Yields");
    if (j == 1) table.setTitle("H-L Yields");
    if (j == 2) table.setTitle("L-L Yields");
    table.useTitle(); 
    for (int i = 1; i <= nSRs[j]; i++){
      table.setRowLabel(Form("SR%i", i), i);
      int cnt = 0;
      table.setCell(getYield(ttw_graphs        , i, j), i, cnt++); 
      table.setCell(getYield(ttz_graphs        , i, j), i, cnt++); 
      table.setCell(getYield(wz_graphs         , i, j), i, cnt++); 
      table.setCell(getYield(sfakes_mc_graphs  , i, j), i, cnt++); 
      //table.setCell(getYield(sfakes_dd_graphs  , i, j), i, cnt++); 
      table.setCell(getYield(dfakes_mc_graphs  , i, j), i, cnt++); 
      table.setCell(getYield(flips_mc_graphs   , i, j), i, cnt++); 
      table.setCell(getYield(pdata_graphs      , i, j), i, cnt++); 
    }
    table.printTex();
  }

  //signals
  for (int j = 0; j < 3; j++){
    CTable table;
    table.setTable() 
      (   "T1tttt"    ,"T1tttt"    ,"T1ttbbww"      ,"T5tttt"            ,"T5qqww"        ,"T5qqww"         ,"T5qqww"        ,"T6ttww"      ,"T6ttww"      ) 
      ("","(1500,100)","(1200,800)","(1300,300,290)","(1000,300,285,280)","(1500,800,100)","(1200,1000,800)","(1000,315,300)","(650,150,50)","(600,425,50)"); 
    table.setPrecision(1); 
    if (j == 0) table.setTitle("H-H Yields");
    if (j == 1) table.setTitle("H-L Yields");
    if (j == 2) table.setTitle("L-L Yields");
    table.useTitle(); 
    for (int i = 1; i <= nSRs[j]; i++){
      table.setRowLabel(Form("SR%i", i), i);
      int cnt = 0;
      table.setCell(getYield(t1tttt_1500_graphs,   i, j), i, cnt++); 
      table.setCell(getYield(t1tttt_1200_graphs,   i, j), i, cnt++); 
      table.setCell(getYield(t1ttbbww_1300_graphs, i, j), i, cnt++); 
      table.setCell(getYield(t5tttt_1000_graphs,   i, j), i, cnt++); 
      table.setCell(getYield(t5qqww_1500_graphs,   i, j), i, cnt++); 
      table.setCell(getYield(t5qqww_1200_graphs,   i, j), i, cnt++); 
      table.setCell(getYield(t5qqww_deg_graphs ,   i, j), i, cnt++); 
      table.setCell(getYield(t6ttww_650_graphs,    i, j), i, cnt++); 
      table.setCell(getYield(t6ttww_600_graphs,    i, j), i, cnt++); 
    }
    table.printTex();
  }

}
